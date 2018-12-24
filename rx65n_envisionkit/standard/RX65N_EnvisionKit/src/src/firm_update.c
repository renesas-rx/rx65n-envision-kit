/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2014(2017) Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : firm_update.c
 * Description  :
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.06.2017 1.00 First Release
 *********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "r_tfat_lib.h"
#include "r_flash_rx_if.h"
#include "r_cryptogram.h"

/******************************************************************************
Typedef definitions
*******************************************************************************/

#define LIFECYCLE_STATE_BLANK             (0)
#define LIFECYCLE_STATE_ON_THE_MARKET     (1)
#define LIFECYCLE_STATE_UPDATING	      (2)

#define SECURE_BOOT_LO_BLOCK FLASH_CF_BLOCK_7
#define SECURE_BOOT_MIRROR_LO_BLOCK FLASH_CF_BLOCK_45
#define SECURE_BOOT_MIRROR_HI_BLOCK FLASH_CF_BLOCK_38
#define SECURE_BOOT_UPDATE_TEMPORARY_AREA FLASH_CF_LO_BANK_LO_ADDR
#define SECURE_BOOT_UPDATE_EXECUTE_AREA FLASH_CF_HI_BANK_LO_ADDR

#define SECURE_BOOT_MIRROR_BLOCK_NUM 8
#define SECURE_BOOT_PARAMETER_NUMBER 2

#define FIRMWARE_UPDATE_STATE_INITIALIZE 0
#define FIRMWARE_UPDATE_STATE_ERASE 1
#define FIRMWARE_UPDATE_STATE_ERASE_WAIT_COMPLETE 2
#define FIRMWARE_UPDATE_STATE_READ_WAIT_COMPLETE 3
#define FIRMWARE_UPDATE_STATE_WRITE_WAIT_COMPLETE 4
#define FIRMWARE_UPDATE_STATE_FINALIZE 5
#define FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_ERASE_DATA_FLASH 6
#define FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_WRITE_DATA_FLASH 7
#define FIRMWARE_UPDATE_STATE_FINALIZE_COMPLETED 8

#define FIRMWARE_UPDATE_STATE_CAN_SWAP_BANK 100
#define FIRMWARE_UPDATE_STATE_WAIT_START 101
#define FIRMWARE_UPDATE_STATE_COMPLETED 102
#define FIRMWARE_UPDATE_STATE_ERROR 103

#define SHA1_HASH_LENGTH_BYTE_SIZE 20

typedef struct _load_firmware_control_block {
	uint32_t status;
	uint8_t file_name[256];
	uint32_t flash_buffer[FLASH_CF_MEDIUM_BLOCK_SIZE / 4];
	uint32_t offset;
	volatile uint32_t flash_write_in_progress_flag;
	volatile uint32_t flash_erase_in_progress_flag;
	uint32_t progress;
	uint8_t hash_sha1[SHA1_HASH_LENGTH_BYTE_SIZE];
	uint32_t firmware_length;
}LOAD_FIRMWARE_CONTROL_BLOCK;

typedef struct _firmware_update_control_block_sub
{
	uint32_t user_program_max_cnt;
	uint32_t lifecycle_state;
	uint8_t hash0_sha1[SHA1_HASH_LENGTH_BYTE_SIZE];
	uint8_t hash1_sha1[SHA1_HASH_LENGTH_BYTE_SIZE];
}FIRMWARE_UPDATE_CONTROL_BLOCK_SUB;

typedef struct _firmware_update_control_block
{
	FIRMWARE_UPDATE_CONTROL_BLOCK_SUB data;
	uint8_t hash_sha1[SHA1_HASH_LENGTH_BYTE_SIZE];
}FIRMWARE_UPDATE_CONTROL_BLOCK;

/******************************************************************************
 External variables
 ******************************************************************************/

/******************************************************************************
 Private global variables
 ******************************************************************************/
static uint32_t g_ram_vector_table[256];      // RAM space to hold the vector table
static LOAD_FIRMWARE_CONTROL_BLOCK load_firmware_control_block;
static FIRMWARE_UPDATE_CONTROL_BLOCK firmware_update_control_block_image = {0};

/******************************************************************************
 External functions
 ******************************************************************************/
extern uint32_t base64_decode(uint8_t *source, uint8_t *result, uint32_t size);

/*******************************************************************************
 Private global variables and functions
********************************************************************************/
static int32_t firm_block_read(uint32_t *firmware, uint32_t offset);
static void dummy_int(void);
static void flash_load_firmware_callback_function(void *event);
static int32_t extract_update_file_parameters(char *string);
static uint32_t *flash_copy_vector_table(void);

void bank_swap(void);
void firmware_update_request(char *string);
bool is_firmupdating(void);
bool is_firmupdatewaitstart(void);

uint32_t load_firmware_process(void);
uint32_t get_update_data_size(void);
void flash_bank_swap_callback_function(void *event);
void load_firmware_status(uint32_t *now_status, uint32_t *finish_status);


/***********************************************************************************************************************
* Function Name: firm_block_read
* Description  : Does an example task. Making this longer just to see how it wraps. Making this long just to see how it
*                wraps.
* Arguments    : index -
*                    Where to start looking
*                p_output -
*                    Pointer of where to put the output data
* Return Value : count -
*                    How many entries were found
***********************************************************************************************************************/
static int32_t firm_block_read(uint32_t *firmware, uint32_t offset)
{
    FRESULT ret = TFAT_FR_OK;
    uint8_t buf[256] = {0};
    uint8_t arg1[256] = {0};
    uint8_t arg2[256] = {0};
    uint8_t arg3[256] = {0};
    uint16_t size = 0;
    FIL file = {0};
    uint32_t read_size = 0;
    static uint32_t upprogram[4 + 1] = {0};
    static uint32_t current_char_position = 0;
    static uint32_t current_file_position = 0;
    static uint32_t previous_file_position = 0;

    if (0 == offset)
    {
    	current_char_position = 0;
    	current_file_position = 0;
        previous_file_position = 0;
        memset(upprogram,0,sizeof(upprogram));
    }

    ret = R_tfat_f_open(&file, (char*)load_firmware_control_block.file_name, TFAT_FA_READ | TFAT_FA_OPEN_EXISTING);
    if (TFAT_RES_OK == ret)
    {
        current_char_position = 0;
        memset(buf, 0, sizeof(buf));

        R_tfat_f_lseek(&file, previous_file_position);
        if (TFAT_RES_OK == ret)
        {
			while(1)
			{
				ret = R_tfat_f_read(&file, &buf[current_char_position++], 1, &size);
				if (TFAT_RES_OK == ret)
				{
					if (0 == size)
					{
						break;
					}
					else
					{
						previous_file_position += size;

						/* received 1 line */
						if(strstr((char*)buf, "\r\n"))
						{
							sscanf((char*)buf, "%256s %256s %256s", (char*)arg1, (char*)arg2, (char*)arg3);
							if (!strcmp((char *)arg1, "upprogram"))
							{
								base64_decode(arg3, (uint8_t *)upprogram, strlen((char *)arg3));
								memcpy(&firmware[current_file_position], upprogram, 16);
								current_file_position += 4;
								read_size += 16;
							}
							if((current_file_position * 4) == FLASH_CF_MEDIUM_BLOCK_SIZE)
							{
								current_file_position = 0;
								break;
							}
							current_char_position = 0;
							memset(buf, 0, sizeof(buf));
						}
					}
				}
				else
				{
					goto firm_block_read_error;
					break;
				}
			}
        }
        else
        {
			goto firm_block_read_error;
        }
        if (TFAT_RES_OK == ret)
        {
            ret = R_tfat_f_close(&file);
            if (TFAT_RES_OK != ret)
            {
				goto firm_block_read_error;
            }
        }
    }
    else
    {
firm_block_read_error:
        return -1;
    }
    return 0;
}

static int32_t extract_update_file_parameters(char *string)
{
    FRESULT ret = TFAT_FR_OK;
    uint8_t buf[256] = {0};
    uint8_t arg1[256] = {0};
    uint8_t arg2[256] = {0};
    uint8_t arg3[256] = {0};
    uint16_t size = 0;
    FIL file = {0};
    static uint32_t current_char_position = 0;
    static uint32_t previous_file_position = 0;
    uint32_t format_check = 0;


    ret = R_tfat_f_open(&file, string, TFAT_FA_READ | TFAT_FA_OPEN_EXISTING);
    if (TFAT_RES_OK == ret)
    {
        current_char_position = 0;
        memset(buf, 0, sizeof(buf));

		while(1)
		{
			ret = R_tfat_f_read(&file, &buf[current_char_position++], 1, &size);
			if (TFAT_RES_OK == ret)
			{
				if (0 == size)
				{
					break;
				}
				else
				{
					previous_file_position += size;

					/* received 1 line */
					if(strstr((char*)buf, "\r\n"))
					{
						sscanf((char*)buf, "%256s %256s %256s", (char*)arg1, (char*)arg2, (char*)arg3);
						if (!strcmp((char *)arg1, "max_cnt"))
						{
							sscanf((char*) arg2, "%x", &load_firmware_control_block.firmware_length);
							format_check++;
						}
                        if (!strcmp((char *)arg1, "sha1"))
                        {
                        	base64_decode(arg2, (uint8_t *)load_firmware_control_block.hash_sha1, strlen((char *)arg2));
                        	format_check++;
                        }
						if (!strcmp((char *)arg1, "upprogram"))
						{
							break;
						}
						current_char_position = 0;
						memset(buf, 0, sizeof(buf));
					}
				}
			}
			else
			{
				goto extract_update_file_parameters_error;
				break;
			}
        }

        if (TFAT_RES_OK == ret)
        {
            ret = R_tfat_f_close(&file);
            if (TFAT_RES_OK != ret)
            {
				goto extract_update_file_parameters_error;
            }
        }
    }
    else
    {
extract_update_file_parameters_error:
        return -1;
    }
    if(format_check != SECURE_BOOT_PARAMETER_NUMBER)
    {
        return -1;
    }
    return 0;
}

uint32_t load_firmware_process(void)
{
	flash_interrupt_config_t cb_func_info;
	uint32_t required_dataflash_block_num;
	int32_t tsip_error_code;
	uint8_t hash_sha1[SHA1_HASH_LENGTH_BYTE_SIZE];

	switch(load_firmware_control_block.status)
	{
		case FIRMWARE_UPDATE_STATE_INITIALIZE: /* initialize */
			load_firmware_control_block.progress = 0;
			load_firmware_control_block.offset = 0;
			memset(&firmware_update_control_block_image, 0, sizeof(FIRMWARE_UPDATE_CONTROL_BLOCK));
			memcpy(&firmware_update_control_block_image, (void *)FLASH_DF_BLOCK_0, sizeof(FIRMWARE_UPDATE_CONTROL_BLOCK));
			cb_func_info.pcallback = flash_load_firmware_callback_function;
		    cb_func_info.int_priority = 15;
		    R_FLASH_Control(FLASH_CMD_SET_BGO_CALLBACK, (void *)&cb_func_info);
			load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_WAIT_START;
			break;
		case FIRMWARE_UPDATE_STATE_WAIT_START: /* wait start */
			/* this state will be changed by other process request using load_firmware_control_block.status */
			break;
		case FIRMWARE_UPDATE_STATE_ERASE: /* erase bank0 (0xFFE00000-0xFFEF0000) */
		    tsip_error_code = extract_update_file_parameters((char *)load_firmware_control_block.file_name);
    		if(0 == tsip_error_code)
    		{
				firmware_update_control_block_image.data.user_program_max_cnt = load_firmware_control_block.firmware_length;
				R_FLASH_Erase((flash_block_address_t)FLASH_CF_BLOCK_46, 30);
				load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERASE_WAIT_COMPLETE;
    		}
    		else
    		{
        		printf("extract_update_file_parameters() returns = %d\r\n", tsip_error_code);
				load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
    		}
			break;
		case FIRMWARE_UPDATE_STATE_ERASE_WAIT_COMPLETE:
			/* this state will be changed by callback routine */
			break;
		case FIRMWARE_UPDATE_STATE_READ_WAIT_COMPLETE:
			if(!firm_block_read(load_firmware_control_block.flash_buffer, load_firmware_control_block.offset))
			{
			    R_FLASH_Write((uint32_t)load_firmware_control_block.flash_buffer, (uint32_t)FLASH_CF_LO_BANK_LO_ADDR + load_firmware_control_block.offset, sizeof(load_firmware_control_block.flash_buffer));
				load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_WRITE_WAIT_COMPLETE;
			}
			else
			{
				load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
			}
			break;
		case FIRMWARE_UPDATE_STATE_WRITE_WAIT_COMPLETE:
			/* this state will be changed by callback routine */
			break;
		case FIRMWARE_UPDATE_STATE_FINALIZE: /* finalize */
		    R_Sha1((uint8_t*)SECURE_BOOT_UPDATE_TEMPORARY_AREA, hash_sha1, FLASH_CF_MEDIUM_BLOCK_SIZE * 30);
		    if(!memcmp(hash_sha1, load_firmware_control_block.hash_sha1, SHA1_HASH_LENGTH_BYTE_SIZE))
		    {
		    	/* confirm which hash(hash0/hash1 on dataflash) is same as current executed area hash */
			    R_Sha1((uint8_t*)SECURE_BOOT_UPDATE_EXECUTE_AREA, hash_sha1, FLASH_CF_MEDIUM_BLOCK_SIZE * 30);
				if(!memcmp(hash_sha1, firmware_update_control_block_image.data.hash0_sha1, SHA1_HASH_LENGTH_BYTE_SIZE))
				{
					/* hash0_sha1 is for current executed area mac, should write to hash1_sha1 */
					memcpy(firmware_update_control_block_image.data.hash1_sha1, load_firmware_control_block.hash_sha1, sizeof(firmware_update_control_block_image.data.hash1_sha1));
				}
				else
				{
					/* program_mac1 is for current executed area mac, should write to hash0_sha1 */
					memcpy(firmware_update_control_block_image.data.hash0_sha1, load_firmware_control_block.hash_sha1, sizeof(firmware_update_control_block_image.data.hash0_sha1));
				}
            	firmware_update_control_block_image.data.lifecycle_state = LIFECYCLE_STATE_UPDATING;
    		    R_Sha1((uint8_t *)&firmware_update_control_block_image.data, hash_sha1, sizeof(firmware_update_control_block_image.data));
    		    memcpy(firmware_update_control_block_image.hash_sha1, hash_sha1, SHA1_HASH_LENGTH_BYTE_SIZE);
    		    required_dataflash_block_num = sizeof(FIRMWARE_UPDATE_CONTROL_BLOCK) / FLASH_DF_BLOCK_SIZE;
    		    if(sizeof(FIRMWARE_UPDATE_CONTROL_BLOCK) % FLASH_DF_BLOCK_SIZE)
    		    {
    		    	required_dataflash_block_num++;
    		    }
				R_FLASH_Erase((flash_block_address_t)FLASH_DF_BLOCK_0, required_dataflash_block_num);
				load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_ERASE_DATA_FLASH;
		    }
		    else
		    {
			    load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
		    }
		    break;
		case FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_ERASE_DATA_FLASH:
			/* this state will be changed by callback routine */
			break;
		case FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_WRITE_DATA_FLASH:
			/* this state will be changed by callback routine */
			break;
		case FIRMWARE_UPDATE_STATE_FINALIZE_COMPLETED:
			load_firmware_control_block.progress = 100;
		    load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_COMPLETED;
		    break;
		case FIRMWARE_UPDATE_STATE_COMPLETED:
			break;
		case FIRMWARE_UPDATE_STATE_ERROR:
			load_firmware_control_block.progress = 100;
			break;
		default:
			break;
	}
	return load_firmware_control_block.progress;
}

void flash_load_firmware_callback_function(void *event)
{
	uint32_t event_code;
	event_code = *((uint32_t*)event);

	switch(event_code)
	{
		case FLASH_INT_EVENT_ERASE_COMPLETE:
			if(FIRMWARE_UPDATE_STATE_ERASE_WAIT_COMPLETE == load_firmware_control_block.status)
			{
				load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_READ_WAIT_COMPLETE;
			}
			else if(FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_ERASE_DATA_FLASH == load_firmware_control_block.status)
			{
				R_FLASH_Write((uint32_t)&firmware_update_control_block_image, FLASH_DF_BLOCK_0, sizeof(FIRMWARE_UPDATE_CONTROL_BLOCK));
				load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_WRITE_DATA_FLASH;
			}
			else
			{
				load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
			}
			break;
		case FLASH_INT_EVENT_WRITE_COMPLETE:
			if(FIRMWARE_UPDATE_STATE_WRITE_WAIT_COMPLETE == load_firmware_control_block.status)
			{
				load_firmware_control_block.offset += FLASH_CF_MEDIUM_BLOCK_SIZE;
		    	load_firmware_control_block.progress = (uint32_t)(((float)(load_firmware_control_block.offset)/(float)((FLASH_CF_MEDIUM_BLOCK_SIZE * 30))*100));
				if(get_update_data_size()-16 == load_firmware_control_block.offset)
				{
					load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_FINALIZE;
				}
				else
				{
					load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_READ_WAIT_COMPLETE;
				}
			}
			else if(FIRMWARE_UPDATE_STATE_FINALIZE_WAIT_WRITE_DATA_FLASH == load_firmware_control_block.status)
			{
				load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_FINALIZE_COMPLETED;
			}
			else
			{
				load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERROR;
			}
			break;
		case FLASH_INT_EVENT_TOGGLE_BANK:
	        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
	        SYSTEM.SWRR = 0xa501;
			while(1);	/* software reset */
			break;
		default:
			break;
	}
}

void firmware_update_request(char *string)
{
	if(FIRMWARE_UPDATE_STATE_COMPLETED == load_firmware_control_block.status)
	{
		load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_INITIALIZE;
	}
	if(FIRMWARE_UPDATE_STATE_INITIALIZE == load_firmware_control_block.status)
	{
		load_firmware_process();
	}
	if(FIRMWARE_UPDATE_STATE_CAN_SWAP_BANK < load_firmware_control_block.status)
	{
		strcpy((char *)load_firmware_control_block.file_name, string);
		load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_ERASE;
	}
}

void firmware_update_status_initialize(void)
{
	load_firmware_control_block.status = FIRMWARE_UPDATE_STATE_INITIALIZE;
}
uint32_t get_update_data_size(void)
{
	return (load_firmware_control_block.firmware_length * 4);
}

bool is_firmupdating(void)
{
	if(FIRMWARE_UPDATE_STATE_CAN_SWAP_BANK < load_firmware_control_block.status)
	{
		return false;
	}
	return true;
}

bool is_firmupdatewaitstart(void)
{
	if(FIRMWARE_UPDATE_STATE_WAIT_START != load_firmware_control_block.status)
	{
		return false;
	}
	return true;
}

/***********************************************************************************************************************
* Function Name: flash_copy_vector_table
* Description  : Moves the relocatable vector table to RAM. This is only needed if ROM operations are performed.
*                ROM cannot be accessed during operations. The vector table is located in ROM and will be accessed
*                if an interrupt occurs.
* Arguments    : none
* Return Value : uint32_t *flash_vect
***********************************************************************************************************************/
static uint32_t *flash_copy_vector_table(void)
{
    uint32_t *flash_vect;
    uint32_t   i;

    /* Get address of variable vector table in ROM */
    flash_vect = (uint32_t *)get_intb();

    /* Copy over variable vector table to RAM */
    for(i = 0; i < 256; i++ )
    {
    	g_ram_vector_table[i] = (uint32_t)dummy_int;      // Copy over entry
    }

    g_ram_vector_table[VECT_FCU_FIFERR] = flash_vect[VECT_FCU_FIFERR];
    g_ram_vector_table[VECT_FCU_FRDYI] = flash_vect[VECT_FCU_FRDYI];

    /* Set INTB to ram address */
#if __RENESAS_VERSION__ >= 0x01010000
    set_intb((void *)&g_ram_vector_table[0] );
#else
    set_intb( (uint32_t)&g_ram_vector_table[0] );
#endif
    return flash_vect;
}

/******************************************************************************
 Function Name   : bank_swap()
 Description     :
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void bank_swap(void)
{
	uint32_t level;
	flash_err_t flash_err;
	if(FIRMWARE_UPDATE_STATE_CAN_SWAP_BANK < load_firmware_control_block.status)
	{
		level = R_BSP_CpuInterruptLevelRead();
		R_BSP_CpuInterruptLevelWrite(14);
		flash_copy_vector_table();
		flash_err = R_FLASH_Control(FLASH_CMD_BANK_TOGGLE, NULL);
		if(FLASH_SUCCESS == flash_err)
		{
			while(1);	/* wait software reset in RAM */
		}
		while(1); /* death loop */
	}
}

void load_firmware_status(uint32_t *now_status, uint32_t *finish_status)
{
	*now_status    = load_firmware_control_block.status;
	*finish_status = FIRMWARE_UPDATE_STATE_COMPLETED;
}

#pragma section FRAM2
#pragma interrupt (dummy_int)
static void dummy_int(void)
{
	/* nothing to do */
}

/* end of file */

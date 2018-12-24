/***********************************************************************
*
*  FILE        : rx65n_2mb_rsk_secure_boot.c
*  DATE        : 2017-09-18
*  DESCRIPTION : Main Program
*
*  NOTE:THIS IS A TYPICAL EXAMPLE.
*
***********************************************************************/
#include <stdio.h>
#include <string.h>
#include "r_smc_entry.h"
#include "r_flash_rx_if.h"
#include "r_tfat_lib.h"
#include "r_usb_basic_if.h"
#include "r_usb_hmsc_if.h"
#include "Pin.h"
#include "base64_decode.h"
#include "r_cryptogram.h"
#include "r_simple_graphic_if.h"

#if SECURE_BOOT
#pragma section SECURE_BOOT
#endif

#define SECURE_BOOT_SUCCESS         (0)
#define SECURE_BOOT_FAIL            (-1)
#define SECURE_BOOT_GOTO_INSTALL    (-2)

#define LIFECYCLE_STATE_BLANK             (0)
#define LIFECYCLE_STATE_ON_THE_MARKET     (1)
#define LIFECYCLE_STATE_UPDATING          (2)

#define SECURE_BOOT_LO_BLOCK FLASH_CF_BLOCK_7
#define SECURE_BOOT_MIRROR_LO_BLOCK FLASH_CF_BLOCK_45
#define SECURE_BOOT_MIRROR_HI_BLOCK FLASH_CF_BLOCK_38
#define SECURE_BOOT_UPDATE_TEMPORARY_AREA FLASH_CF_LO_BANK_LO_ADDR
#define SECURE_BOOT_UPDATE_EXECUTE_AREA FLASH_CF_HI_BANK_LO_ADDR

#define SECURE_BOOT_MIRROR_BLOCK_NUM 8
#define SECURE_BOOT_PARAMETER_NUMBER 5

#define INITIAL_FIRMWARE_FILE_NAME "std.rsu"

#define MAX_CHECK_DATAFLASH_AREA_RETRY_COUNT 3
#define SHA1_HASH_LENGTH_BYTE_SIZE 20

typedef struct _load_firmware_control_block {
    uint32_t flash_buffer[FLASH_CF_MEDIUM_BLOCK_SIZE / 4];
    uint32_t offset;
    uint32_t progress;
    uint8_t hash_sha1[SHA1_HASH_LENGTH_BYTE_SIZE];
    uint32_t firmware_length;
}LOAD_FIRMWARE_CONTROL_BLOCK;

typedef struct _firmware_update_control_block_sub
{
    uint32_t user_program_max_cnt;
    uint32_t lifecycle_state;
    uint8_t program_hash0[SHA1_HASH_LENGTH_BYTE_SIZE];
    uint8_t program_hash1[SHA1_HASH_LENGTH_BYTE_SIZE];
}FIRMWARE_UPDATE_CONTROL_BLOCK_SUB;

typedef struct _firmware_update_control_block
{
    FIRMWARE_UPDATE_CONTROL_BLOCK_SUB data;
    uint8_t hash_sha1[SHA1_HASH_LENGTH_BYTE_SIZE];
}FIRMWARE_UPDATE_CONTROL_BLOCK;

void main(void);
static int32_t secure_boot(void);
static int32_t firm_block_read(uint32_t *firmware, uint32_t offset);
static void bank_swap_with_software_reset(void);
static void update_dataflash_data_from_image(void);
static void update_dataflash_data_mirror_from_image(void);
static void check_dataflash_area(uint32_t retry_counter);
static void software_reset(void);

extern int32_t usb_main(void);
extern void lcd_open(void);
extern void lcd_close(void);

#define FIRMWARE_UPDATE_CONTROL_BLOCK_INITIAL_DATA \
        /* FIRMWARE_UPDATE_CONTROL_BLOCK_SUB data; */\
        {\
            /* uint32_t user_program_max_cnt; */\
            0,\
            /* uint32_t lifecycle_state; */\
            LIFECYCLE_STATE_BLANK,\
            /* uint8_t program_hash0[SHA1_HASH_LENGTH_BYTE_SIZE]; */\
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},\
            /* uint8_t program_hash1[SHA1_HASH_LENGTH_BYTE_SIZE]; */\
			{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},\
        },\
        /* uint8_t hash_sha1[SHA1_HASH_LENGTH_BYTE_SIZE]; */\
        {0xc1, 0x7f, 0xd9, 0x26, 0x82, 0xca, 0x5b, 0x30, 0x4A, 0xc7, 0x10, 0x74, 0xb5, 0x58, 0xdd, 0xa9, 0xe8, 0xeb, 0x4d, 0x66},

#pragma section _FIRMWARE_UPDATE_CONTROL_BLOCK
static const FIRMWARE_UPDATE_CONTROL_BLOCK firmware_update_control_block_data = {FIRMWARE_UPDATE_CONTROL_BLOCK_INITIAL_DATA};
#pragma section

#pragma section _FIRMWARE_UPDATE_CONTROL_BLOCK_MIRROR
static const FIRMWARE_UPDATE_CONTROL_BLOCK firmware_update_control_block_data_mirror = {FIRMWARE_UPDATE_CONTROL_BLOCK_INITIAL_DATA};
#pragma section

static FIRMWARE_UPDATE_CONTROL_BLOCK firmware_update_control_block_image = {0};
static LOAD_FIRMWARE_CONTROL_BLOCK load_firmware_control_block;
static FATFS       g_fatfs;


#if SECURE_BOOT
#pragma section SECURE_BOOT
#endif

const char lifecycle_string[][64] = {
        {"LIFECYCLE_STATE_BLANK"},
        {"LIFECYCLE_STATE_ON_THE_MARKET"},
        {"LIFECYCLE_STATE_UPDATING"},
};

void main(void)
{
    int32_t result_secure_boot;
    static FILINFO filinfo;
    usb_ctrl_t  ctrl;
    usb_cfg_t   cfg;
    uint16_t    event;
    uint16_t    previous_event;
    flash_err_t flash_error_code = FLASH_SUCCESS;
    uint8_t hash_sha1[SHA1_HASH_LENGTH_BYTE_SIZE];
    R_Pins_Create();

    load_firmware_control_block.progress = 0;
    load_firmware_control_block.offset = 0;

    lcd_open();
    R_SIMPLE_GRAPHIC_Open();

    flash_error_code = R_FLASH_Open();
    if (FLASH_SUCCESS == flash_error_code)
    {
        /* nothing to do */
    }
    else
    {
        printf("R_FLASH_Open() returns error. %d.\r\n", flash_error_code);
        printf("system error.\r\n");
        while(1);
    }

    /* startup system */
    printf("-------------------------------------------------\r\n");
    printf("RX65N secure boot program\r\n");
    printf("-------------------------------------------------\r\n");

    printf("Checking data flash...\r\n");
    check_dataflash_area(0);
    printf("OK\r\n");

    memset(&firmware_update_control_block_image, 0, sizeof(FIRMWARE_UPDATE_CONTROL_BLOCK));
    memcpy(&firmware_update_control_block_image, &firmware_update_control_block_data, sizeof(FIRMWARE_UPDATE_CONTROL_BLOCK));

    result_secure_boot = secure_boot();
    if (SECURE_BOOT_SUCCESS == result_secure_boot)
    {
        R_BSP_InterruptsDisable();
        R_SIMPLE_GRAPHIC_Close();
        lcd_close();
        return;
    }
    else if (SECURE_BOOT_GOTO_INSTALL == result_secure_boot)
    {
        /* Set up the USB */
        ctrl.module     = USB_IP0;
        ctrl.type       = USB_HMSC;
        cfg.usb_speed   = USB_FS;
        cfg.usb_mode    = USB_HOST;
        R_USB_Open(&ctrl, &cfg);


        printf("========== install user program phase ==========\r\n");
        printf("insert USB memory includes \"%s\"\r\n", INITIAL_FIRMWARE_FILE_NAME);
        while(1)
        {
            event = R_USB_GetEvent(&ctrl);
            if(event != previous_event)
            {
                if(event == USB_STS_CONFIGURED)
                {
                    R_tfat_f_mount(0, &g_fatfs);
                    printf("usb memory attached.\r\n");
                    if(TFAT_FR_OK == R_tfat_f_stat(INITIAL_FIRMWARE_FILE_NAME, &filinfo))
                    {
                        printf("Detected file. %s.\r\n", INITIAL_FIRMWARE_FILE_NAME);

                        printf("erase install area: ");
                        flash_error_code = R_FLASH_Erase((flash_block_address_t)FLASH_CF_BLOCK_46, 30);
                        if (FLASH_SUCCESS == flash_error_code)
                        {
                            printf("OK\r\n");
                        }
                        else
                        {
                            printf("R_FLASH_Erase() returns error. %d.\r\n", flash_error_code);
                            printf("system error.\r\n");
                            while(1);
                        }

                        while(1)
                        {
                            if(!firm_block_read(load_firmware_control_block.flash_buffer, load_firmware_control_block.offset))
                            {
                                R_FLASH_Write((uint32_t)load_firmware_control_block.flash_buffer, (uint32_t)SECURE_BOOT_UPDATE_TEMPORARY_AREA + load_firmware_control_block.offset, sizeof(load_firmware_control_block.flash_buffer));
                                load_firmware_control_block.offset += FLASH_CF_MEDIUM_BLOCK_SIZE;
                                load_firmware_control_block.progress = (uint32_t)(((float)(load_firmware_control_block.offset)/(float)((FLASH_CF_MEDIUM_BLOCK_SIZE * 30))*100));
                                printf("installing firmware...%d%(%d/%dKB).\r", load_firmware_control_block.progress, load_firmware_control_block.offset/1024, (FLASH_CF_MEDIUM_BLOCK_SIZE * 30)/1024);
                                if(load_firmware_control_block.offset < (FLASH_CF_MEDIUM_BLOCK_SIZE * 30))
                                {
                                    /* one more loop */
                                }
                                else if(load_firmware_control_block.offset == (FLASH_CF_MEDIUM_BLOCK_SIZE * 30))
                                {
                                    printf("\n");
                                    printf("completed installing.\r\n");
                                    break;
                                }
                                else
                                {
                                    printf("\n");
                                    printf("fatal error occurred.\r\n");
                                    break;
                                }
                            }
                            else
                            {
                                printf("\n");
                                printf("filesystem output error.\r\n");
                                break;
                            }
                        }
                        printf("code flash hash check...");
                        
                        R_Sha1((uint8_t*)SECURE_BOOT_UPDATE_TEMPORARY_AREA, hash_sha1, FLASH_CF_MEDIUM_BLOCK_SIZE * 30);
                        if(0 == memcmp(hash_sha1, load_firmware_control_block.hash_sha1, SHA1_HASH_LENGTH_BYTE_SIZE))
                        {
                            printf("OK\r\n");

                            memcpy(firmware_update_control_block_image.data.program_hash1, hash_sha1, sizeof(hash_sha1));
                            firmware_update_control_block_image.data.lifecycle_state = LIFECYCLE_STATE_UPDATING;
                            firmware_update_control_block_image.data.user_program_max_cnt = load_firmware_control_block.firmware_length;

                            R_Sha1((uint8_t *)&firmware_update_control_block_image.data, hash_sha1, sizeof(firmware_update_control_block_image.data));
                            memcpy(firmware_update_control_block_image.hash_sha1, hash_sha1, sizeof(firmware_update_control_block_image.hash_sha1));
                            update_dataflash_data_from_image();
                            update_dataflash_data_mirror_from_image();

                            printf("swap bank...");
                            R_BSP_SoftwareDelay(3000, BSP_DELAY_MILLISECS);
                            R_USB_Close(&ctrl);
                            R_SIMPLE_GRAPHIC_Close();
                            lcd_close();
                            bank_swap_with_software_reset();
                            while(1);
                        }
                        else
                        {
                            printf("NG\r\n");
                            while(1);
                        }
                    }
                    else
                    {
                        printf("File Not Found. %s.\r\n", INITIAL_FIRMWARE_FILE_NAME);
                    }
                }
                else if(event == USB_STS_DETACH)
                {
                    printf("usb memory detached.\r\n");
                }
            }
            previous_event = event;
        }
    }
    else if (SECURE_BOOT_FAIL == result_secure_boot)
    {
        printf("secure boot sequence:");
        printf("fail.");
        while(1)
        {
            /* infinity loop */
        }
    }
    else
    {
        printf("unknown status.\n");
        while(1)
        {
            /* infinite loop */
        }
    }
}

static int32_t secure_boot(void)
{
    flash_err_t flash_error_code = FLASH_SUCCESS;
    int32_t secure_boot_error_code;
    uint8_t hash_sha1[SHA1_HASH_LENGTH_BYTE_SIZE];
    uint32_t bank_info = 255;

    printf("Checking flash ROM status.\r\n");

    printf("status = %s\r\n", lifecycle_string[firmware_update_control_block_image.data.lifecycle_state]);

    R_FLASH_Control(FLASH_CMD_BANK_GET, &bank_info);
    printf("bank info = %d. (start bank = %d)\r\n", bank_info, (bank_info ^ 0x01));

    switch(firmware_update_control_block_image.data.lifecycle_state)
    {
        case LIFECYCLE_STATE_BLANK:
            printf("start installing user program.\r\n");
            printf("erase bank1 secure boot mirror area...");
            flash_error_code = R_FLASH_Erase(SECURE_BOOT_MIRROR_HI_BLOCK, SECURE_BOOT_MIRROR_BLOCK_NUM);
            if(FLASH_SUCCESS == flash_error_code)
            {
                printf("OK\r\n");
            }
            else
            {
                printf("NG\r\n");
                printf("R_FLASH_Erase() returns error code = %d.\r\n", flash_error_code);
                secure_boot_error_code = SECURE_BOOT_FAIL;
                break;
            }

            printf("copy secure boot from bank0 to bank1...");
            flash_error_code = R_FLASH_Write((uint32_t)SECURE_BOOT_LO_BLOCK, (uint32_t)SECURE_BOOT_MIRROR_LO_BLOCK, (uint32_t)SECURE_BOOT_MIRROR_BLOCK_NUM * FLASH_CF_SMALL_BLOCK_SIZE);
            if(FLASH_SUCCESS == flash_error_code)
            {
                printf("OK\r\n");
            }
            else
            {
                printf("NG\r\n");
                printf("R_FLASH_Write() returns error code = %d.\r\n", flash_error_code);
                secure_boot_error_code = SECURE_BOOT_FAIL;
                break;
            }
            secure_boot_error_code = SECURE_BOOT_GOTO_INSTALL;
            break;
        case LIFECYCLE_STATE_UPDATING:
            printf("update data flash\r\n");
            memcpy(hash_sha1, firmware_update_control_block_image.data.program_hash1, sizeof(hash_sha1));
            memcpy(firmware_update_control_block_image.data.program_hash1, firmware_update_control_block_image.data.program_hash0, sizeof(hash_sha1));
            memcpy(firmware_update_control_block_image.data.program_hash0, hash_sha1, sizeof(hash_sha1));
            firmware_update_control_block_image.data.lifecycle_state = LIFECYCLE_STATE_ON_THE_MARKET;

            R_Sha1((uint8_t *)&firmware_update_control_block_image.data, hash_sha1, sizeof(firmware_update_control_block_data.data));
            memcpy(firmware_update_control_block_image.hash_sha1, hash_sha1, sizeof(hash_sha1));
            update_dataflash_data_from_image();
            update_dataflash_data_mirror_from_image();
            //break;    /* in this case, next state "LIFECYCLE_STATE_ON_THE_MARKET" is needed to execute */
        case LIFECYCLE_STATE_ON_THE_MARKET:
            check_dataflash_area(0);
            printf("code flash hash check...");
            R_Sha1((uint8_t*)SECURE_BOOT_UPDATE_EXECUTE_AREA, hash_sha1, FLASH_CF_MEDIUM_BLOCK_SIZE * 30);
            if(!memcmp(firmware_update_control_block_data.data.program_hash0, hash_sha1, sizeof(hash_sha1)) || !memcmp(firmware_update_control_block_data.data.program_hash1, hash_sha1, sizeof(hash_sha1)))
            {
                printf("OK\r\n");
                printf("jump to user program\r\n");
                R_BSP_SoftwareDelay(1000, BSP_DELAY_MILLISECS);
                secure_boot_error_code = SECURE_BOOT_SUCCESS;
            }
            else
            {
                R_Sha1((uint8_t*)SECURE_BOOT_UPDATE_TEMPORARY_AREA, hash_sha1, FLASH_CF_MEDIUM_BLOCK_SIZE * 30);
                if(!memcmp(firmware_update_control_block_data.data.program_hash0, hash_sha1, sizeof(hash_sha1)) || !memcmp(firmware_update_control_block_data.data.program_hash1, hash_sha1, sizeof(hash_sha1)) )
                {
                    printf("NG.\r\n");
                    printf("But other bank %d is still alive.\r\n", (bank_info ^ 0x01) ^ 0x01);
                    printf("swap bank...");
                    R_BSP_SoftwareDelay(3000, BSP_DELAY_MILLISECS);
                    bank_swap_with_software_reset();
                    while(1);
                }
                else
                {
                    printf("NG.\r\n");
                    printf("Code flash is completely broken.\r\n");
                    printf("Please erase all code flash.\r\n");
                    printf("And, write secure boot using debugger.\r\n");
                    secure_boot_error_code = SECURE_BOOT_FAIL;
                }
            }
            break;
        default:
            printf("illegal flash rom status code 0x%x.\r\n", firmware_update_control_block_image.data.lifecycle_state);
            check_dataflash_area(0);
            R_BSP_SoftwareDelay(1000, BSP_DELAY_MILLISECS);
            software_reset();
            while(1);
    }
    return secure_boot_error_code;
}

static void update_dataflash_data_from_image(void)
{
    uint32_t required_dataflash_block_num;
    flash_err_t flash_error_code = FLASH_SUCCESS;

    required_dataflash_block_num = sizeof(FIRMWARE_UPDATE_CONTROL_BLOCK) / FLASH_DF_BLOCK_SIZE;
    if(sizeof(FIRMWARE_UPDATE_CONTROL_BLOCK) % FLASH_DF_BLOCK_SIZE)
    {
        required_dataflash_block_num++;
    }

    printf("erase dataflash(main)...");
    flash_error_code = R_FLASH_Erase((flash_block_address_t)&firmware_update_control_block_data, required_dataflash_block_num);
    if(FLASH_SUCCESS == flash_error_code)
    {
        printf("OK\r\n");
    }
    else
    {
        printf("NG\r\n");
        printf("R_FLASH_Erase() returns error code = %d.\r\n", flash_error_code);
    }

    printf("write dataflash(main)...");
    flash_error_code = R_FLASH_Write((flash_block_address_t)&firmware_update_control_block_image, (flash_block_address_t)&firmware_update_control_block_data, FLASH_DF_BLOCK_SIZE * required_dataflash_block_num);
    if(FLASH_SUCCESS == flash_error_code)
    {
        printf("OK\r\n");
    }
    else
    {
        printf("NG\r\n");
        printf("R_FLASH_Write() returns error code = %d.\r\n", flash_error_code);
        return;
    }
    return;
}

static void update_dataflash_data_mirror_from_image(void)
{
    uint32_t required_dataflash_block_num;
    flash_err_t flash_error_code = FLASH_SUCCESS;

    required_dataflash_block_num = sizeof(FIRMWARE_UPDATE_CONTROL_BLOCK) / FLASH_DF_BLOCK_SIZE;
    if(sizeof(FIRMWARE_UPDATE_CONTROL_BLOCK) % FLASH_DF_BLOCK_SIZE)
    {
        required_dataflash_block_num++;
    }

    printf("erase dataflash(mirror)...");
    flash_error_code = R_FLASH_Erase((flash_block_address_t)&firmware_update_control_block_data_mirror, required_dataflash_block_num);
    if(FLASH_SUCCESS == flash_error_code)
    {
        printf("OK\r\n");
    }
    else
    {
        printf("NG\r\n");
        printf("R_FLASH_Erase() returns error code = %d.\r\n", flash_error_code);
        return;
    }

    printf("write dataflash(mirror)...");
    flash_error_code = R_FLASH_Write((flash_block_address_t)&firmware_update_control_block_image, (flash_block_address_t)&firmware_update_control_block_data_mirror, FLASH_DF_BLOCK_SIZE * required_dataflash_block_num);
    if(FLASH_SUCCESS == flash_error_code)
    {
        printf("OK\r\n");
    }
    else
    {
        printf("NG\r\n");
        printf("R_FLASH_Write() returns error code = %d.\r\n", flash_error_code);
        return;
    }
    if(!memcmp(&firmware_update_control_block_data, &firmware_update_control_block_data_mirror, sizeof(FIRMWARE_UPDATE_CONTROL_BLOCK)))
    {
        printf("data flash setting OK.\r\n");
    }
    else
    {
        printf("data flash setting NG.\r\n");
        return;
    }
    return;
}

static void check_dataflash_area(uint32_t retry_counter)
{
    uint8_t hash_sha1[SHA1_HASH_LENGTH_BYTE_SIZE];

    if(retry_counter)
    {
        printf("recover retry count = %d.\r\n", retry_counter);
        if(retry_counter == MAX_CHECK_DATAFLASH_AREA_RETRY_COUNT)
        {
            printf("retry over the limit.\r\n");
            while(1);
        }
    }
    printf("data flash(main) hash check...");
    R_Sha1((uint8_t *)&firmware_update_control_block_data.data, hash_sha1, sizeof(firmware_update_control_block_data.data));
    if(!memcmp(firmware_update_control_block_data.hash_sha1, hash_sha1, sizeof(hash_sha1)))
    {
        printf("OK\r\n");
        printf("data flash(mirror) hash check...");
        R_Sha1((uint8_t *)&firmware_update_control_block_data_mirror.data, hash_sha1, sizeof(firmware_update_control_block_data.data));
        if(!memcmp(firmware_update_control_block_data_mirror.hash_sha1, hash_sha1, sizeof(hash_sha1)))
        {
            printf("OK\r\n");
        }
        else
        {
            printf("NG\r\n");
            printf("recover mirror from main.\r\n");
            memcpy(&firmware_update_control_block_image, &firmware_update_control_block_data, sizeof(firmware_update_control_block_data));
            update_dataflash_data_mirror_from_image();
            check_dataflash_area(retry_counter+1);
        }
    }
    else
    {
        printf("NG\r\n");
        printf("data flash(mirror) hash check...");
        R_Sha1((uint8_t *)&firmware_update_control_block_data_mirror.data, hash_sha1, sizeof(firmware_update_control_block_data_mirror.data));
        if(!memcmp(firmware_update_control_block_data_mirror.hash_sha1, hash_sha1, sizeof(hash_sha1)))
        {
            printf("OK\r\n");
            printf("recover main from mirror.\r\n");
            memcpy(&firmware_update_control_block_image, &firmware_update_control_block_data_mirror, sizeof(firmware_update_control_block_data_mirror));
            update_dataflash_data_from_image();
            check_dataflash_area(retry_counter+1);
        }
        else
        {
            printf("NG\r\n");
            printf("Data flash is completely broken.\r\n");
            printf("Please erase all code flash.\r\n");
            printf("And, write secure boot using debugger.\r\n");
            while(1);
        }
    }
}

static void software_reset(void)
{
    R_BSP_InterruptsDisable();
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    SYSTEM.SWRR = 0xa501;
    while(1);   /* software reset */
}

static void bank_swap_with_software_reset(void)
{
    R_BSP_InterruptsDisable();
    R_FLASH_Control(FLASH_CMD_BANK_TOGGLE, NULL);
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    SYSTEM.SWRR = 0xa501;
    while(1);   /* software reset */
}


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

    ret = R_tfat_f_open(&file, INITIAL_FIRMWARE_FILE_NAME, TFAT_FA_READ | TFAT_FA_OPEN_EXISTING);
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
                            if (!strcmp((char *)arg1, "sha1"))
                            {
                                base64_decode(arg2, (uint8_t *)load_firmware_control_block.hash_sha1, strlen((char *)arg2));
                            }
                            if (!strcmp((char *)arg1, "max_cnt"))
                            {
                                sscanf((char*) arg2, "%x", load_firmware_control_block.firmware_length);
                            }
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

#if SECURE_BOOT
#pragma section
#endif

/***********************************************************************************************************************
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
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : benchmark.c
* Version      : 1.0 <- Optional as long as history is shown below
* Description  : This module solves all the world's problems
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.01.2007 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"
#include "r_cmt_rx_if.h"
#include "DIALOG.h"
#include "r_tfat_lib.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
uint32_t g_benchmark_cnt;
uint32_t g_benchmark_channel;

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
extern uint32_t g_benchmark_channel;
extern WM_HWIN hWinStorageBenchmark, hWinCryptoBenchmark2;
extern void display_update_storagebenchmark2(WM_HWIN hWin,  uint32_t id, uint32_t result, uint32_t test_time_sec, int32_t okng);
void cb_benchmark(void);

extern void display_update_storagebenchmark_bar(WM_HWIN hWin, uint32_t id, int32_t value, int32_t max);

void BenchmarkInit(void)
{
}


void cb_benchmark(void)
{
   g_benchmark_cnt++;
}

int benchmark_enable = 0;
void storage_benchmark_enable(void)
{
	if(benchmark_enable == 0)
	{
		benchmark_enable = 1;
	}
}


uint32_t    filebuffer2[4*1024 / sizeof(uint32_t)];
void storage_benchmark_main(void)
{
	FIL g_file_usb, g_file_sdhi;
	uint16_t	file_rw_cnt_sdhi, file_rw_cnt_usb;
	uint32_t	count;
	uint32_t	now_display_update_count;
	uint32_t	previous_display_update_count;
	FRESULT 	err;
	int32_t     success;

	if(benchmark_enable == 1)
	{
		benchmark_enable = 2;

		memset(&g_file_sdhi,0,sizeof(g_file_sdhi));

		/* sD write */
		err = R_tfat_f_open(&g_file_sdhi, (const uint8_t *)"1:renesas.txt", TFAT_FA_CREATE_ALWAYS | TFAT_FA_WRITE);
		if(err == TFAT_FR_OK)
		{
			/* Copy the contents to the newly created file	*/
			count = 0;
			g_benchmark_cnt = 0;
			previous_display_update_count = 0;
			while(g_benchmark_cnt < BENCHMARK_TEST_TIME)
			{
				err = R_tfat_f_write(&g_file_sdhi, (void*)filebuffer2, sizeof(filebuffer2), &file_rw_cnt_sdhi);
				if(err != TFAT_FR_OK)
				{
					/* error !*/
					break;
				}
				count += file_rw_cnt_sdhi;
				display_update_storagebenchmark_bar(hWinStorageBenchmark,0,g_benchmark_cnt, BENCHMARK_TEST_TIME);
				now_display_update_count = g_benchmark_cnt % CMT_INTERVAL_1SEC_COUNT;
				if(now_display_update_count < previous_display_update_count)
				{
					GUI_Delay(1);
				}
				previous_display_update_count = now_display_update_count;
			}
			R_tfat_f_close(&g_file_sdhi);
		}
		if(err == TFAT_FR_OK)
		{
			success = 0;
		}
		else
		{
			success = -1;
		}
		display_update_storagebenchmark_bar(hWinStorageBenchmark,0,BENCHMARK_TEST_TIME, BENCHMARK_TEST_TIME);
		display_update_storagebenchmark2(hWinStorageBenchmark,0,count, BENCHMARK_TEST_TIME_SEC, success);
		GUI_Delay(1);

		/* USB write */
		err = R_tfat_f_open(&g_file_usb, (const uint8_t *)"0:renesas.txt", TFAT_FA_CREATE_ALWAYS | TFAT_FA_WRITE);
		if(err == TFAT_FR_OK)
		{
			/* Copy the contents to the newly created file	*/
			count = 0;
			g_benchmark_cnt = 0;
			previous_display_update_count = 0;
			while(g_benchmark_cnt < BENCHMARK_TEST_TIME) /* 100us * 100000 = 10s */
			{
				err = R_tfat_f_write(&g_file_usb, (void*)filebuffer2, sizeof(filebuffer2), &file_rw_cnt_usb);
				if(err != TFAT_FR_OK)
				{
					/* error !*/
					break;
				}
				count += file_rw_cnt_usb;
				display_update_storagebenchmark_bar(hWinStorageBenchmark,1,g_benchmark_cnt ,BENCHMARK_TEST_TIME);
				now_display_update_count = g_benchmark_cnt % CMT_INTERVAL_1SEC_COUNT;
				if(now_display_update_count < previous_display_update_count)
				{
					GUI_Delay(1);
				}
				previous_display_update_count = now_display_update_count;
			}
			R_tfat_f_close(&g_file_usb);
		}
		if(err == TFAT_FR_OK)
		{
			success = 0;
		}
		else
		{
			success = -1;
		}
		display_update_storagebenchmark_bar(hWinStorageBenchmark,1,BENCHMARK_TEST_TIME ,BENCHMARK_TEST_TIME);
		display_update_storagebenchmark2(hWinStorageBenchmark,1,count, BENCHMARK_TEST_TIME_SEC, success);
		GUI_Delay(1);

		/* SD Read */
		err = R_tfat_f_open(&g_file_sdhi, (const uint8_t *)"1:renesas.txt", TFAT_FA_READ);
		if(err == TFAT_FR_OK)
		{
			/* Copy the contents to the newly created file	*/
			count = 0;
			g_benchmark_cnt = 0;
			previous_display_update_count = 0;
			while(g_benchmark_cnt < BENCHMARK_TEST_TIME) /* 100us * 100000 = 10s */
			{
				err = R_tfat_f_read(&g_file_sdhi, (void*)filebuffer2, sizeof(filebuffer2), &file_rw_cnt_sdhi);
				if(err != TFAT_FR_OK)
				{
					/* error !*/
					break;
				}
				count += file_rw_cnt_sdhi;
				err = R_tfat_f_lseek(&g_file_sdhi, 0);
				if(err != TFAT_FR_OK)
				{
					/* error !*/
					break;
				}
				display_update_storagebenchmark_bar(hWinStorageBenchmark,2,g_benchmark_cnt ,BENCHMARK_TEST_TIME);
				now_display_update_count = g_benchmark_cnt % CMT_INTERVAL_1SEC_COUNT;
				if(now_display_update_count < previous_display_update_count)
				{
					GUI_Delay(1);
				}
				previous_display_update_count = now_display_update_count;
			}
			R_tfat_f_close(&g_file_sdhi);
		}
		if(err == TFAT_FR_OK)
		{
			success = 0;
		}
		else
		{
			success = -1;
		}
		display_update_storagebenchmark_bar(hWinStorageBenchmark,2,BENCHMARK_TEST_TIME ,BENCHMARK_TEST_TIME);
		display_update_storagebenchmark2(hWinStorageBenchmark,2,count, BENCHMARK_TEST_TIME_SEC, success);
		GUI_Delay(1);




		/* USB read */
		err = R_tfat_f_open(&g_file_usb, (const uint8_t *)"0:renesas.txt", TFAT_FA_READ);
		if(err == TFAT_FR_OK)
		{
			/* Copy the contents to the newly created file	*/
			count = 0;
			g_benchmark_cnt = 0;
			previous_display_update_count = 0;
			while(g_benchmark_cnt < BENCHMARK_TEST_TIME) /* 100us * 100000 = 10s */
			{
				err = R_tfat_f_read(&g_file_usb, (void*)filebuffer2, sizeof(filebuffer2), &file_rw_cnt_usb);
				if(err != TFAT_FR_OK)
				{
					/* error !*/
					break;
				}
				count += file_rw_cnt_usb;
				err = R_tfat_f_lseek(&g_file_usb, 0);
				if(err != TFAT_FR_OK)
				{
					/* error !*/
					break;
				}
				display_update_storagebenchmark_bar(hWinStorageBenchmark,3,g_benchmark_cnt, BENCHMARK_TEST_TIME);
				now_display_update_count = g_benchmark_cnt % CMT_INTERVAL_1SEC_COUNT;
				if(now_display_update_count < previous_display_update_count)
				{
					GUI_Delay(1);
				}
				previous_display_update_count = now_display_update_count;
			}
			R_tfat_f_close(&g_file_usb);
		}
		if(err == TFAT_FR_OK)
		{
			success = 0;
		}
		else
		{
			success = -1;
		}
		display_update_storagebenchmark_bar(hWinStorageBenchmark,3,BENCHMARK_TEST_TIME, BENCHMARK_TEST_TIME);
		display_update_storagebenchmark2(hWinStorageBenchmark,3,count, BENCHMARK_TEST_TIME_SEC, success);
		GUI_Delay(1);

		R_tfat_f_unlink((const uint8_t *)"0:renesas.txt");
		R_tfat_f_unlink((const uint8_t *)"1:renesas.txt");

		benchmark_enable = 0;

	}
}





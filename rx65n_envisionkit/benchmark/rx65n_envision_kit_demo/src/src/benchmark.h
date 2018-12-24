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
* File Name    : benchmark.h
* Version      : 1.0 <- Optional as long as history is shown below
* Device(s)    : R5F562TAANFP
* Tool-Chain   : IDE, Compiler (inc. version)
* OS           : uITRON      <- Required only if RTOS present
* H/W Platform : RSKM16C62P  <- OPTIONAL
* Description  : This is the main tutorial code.
* Operation    : 1. Compile and download the sample code. Click 'Reset Go'
*                   to start the software.   <- Required for main app file only
* Limitations  : Interrupts must be disabled for this sample  <- OPTIONAL
* xxx          : Make your own other sections as needed. (e.g. describe endian)
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.01.2007 1.00     First Release
*         : 15.10.2007 1.01     Modification of SFR setting. Showing how you would add multiple lines to the history
*                               description.
*         : 16.11.2007 1.60     More descriptions
***********************************************************************************************************************/

#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define COUNT_FREQENCY (100)	/* 100 = 10ms */

#define BENCHMARK_TEST_TIME_SEC  (5)
#define CMT_INTERVAL_USEC  (10000)
#define CMT_INTERVAL_1SEC_COUNT  COUNT_FREQENCY
#define BENCHMARK_TEST_TIME  (BENCHMARK_TEST_TIME_SEC * CMT_INTERVAL_1SEC_COUNT)

#define DATA_VOLUME (1) /*Kbyte*/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
	BENCHMARK_AES = 0,
	BENCHMARK_RANDOM,
	BENCHMARK_SHA,
	BENCHMARK_RSA,
	BENCHMARK_STORAGE
} bench_commands_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
typedef struct _benchmark_result
{
	uint32_t tsip_aes128ecb;                 /*[μs]*/
	uint32_t sofwaret_library_aes128ecb;     /*[μs]*/
	uint32_t open_source_aes128ecb;          /*[μs]*/
	uint32_t tsip_randomnumber_generation;   /*[μs]*/

	uint32_t tsip_sha_hashing;               /*[μs]*/
	uint32_t software_library_sha_hashing;   /*[μs]*/
	uint32_t tsip_rsa2048;                   /*[μs]*/
	uint32_t software_library_rsa2048;       /*[μs]*/

	uint32_t sdhi_write;                     /*[μs]*/
	uint32_t usb_host_write;                 /*[μs]*/
	uint32_t sdhi_read;                      /*[μs]*/
	uint32_t usb_host_read;                  /*[μs]*/
}BENCHMARK_RESULT;

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

void BenchmarkInit(void);
BENCHMARK_RESULT Benchmark(bench_commands_t command);
void cb_benchmark(void);
extern uint32_t g_benchmark_cnt;

#endif /* BENCHMARK_H_ */

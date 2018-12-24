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
 * Copyright (C) 2014(2016) Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : main.c
 * Description  : main process
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 07.08.2017 1.00 First Release
 *********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

/* for using C standard library */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* for using FIT Module */
#include "platform.h"
#include "r_pinset.h"
#include "r_sys_time_rx_if.h"
#include "r_t4_itcpip.h"
#include "r_t4_http_server_rx_if.h"
#include "r_t4_ftp_server_rx_if.h"
#include "r_t4_dns_client_rx_if.h"
#include "r_t4_sntp_client_rx_if.h"
#include "r_gpio_rx_if.h"
#include "r_flash_rx_if.h"
#include "r_usb_basic_if.h"
#include "r_tfat_lib.h"
#include "benchmark.h"
#include "Pin.h"

/* for using Segger emWin */
#include "GUI.h"
#include "DIALOG.h"

/* for getting demo information */
#include "r_t4_http_server_rx_config.h"

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
#define DEBUG_PRINT
#define LOCAL_TIME_ZONE SYS_TIME_UTC_PLUS_0900
#define SNTP_PRIMARY_SERVER_NAME "ntp1.jst.mfeed.ad.jp"
#define SNTP_SECONDARY_SERVER_NAME "ntp2.jst.mfeed.ad.jp"

#define DEMO_NAME_NETWORK_STAT "Network Statistics"
#define DEMO_NAME_SYSTEM_LOG "System Log"
#define DEMO_NAME_STORAGEBENCH  "Storage Benchmark"
#define DEMO_NAME_SECURE_UPDATE "Secure Update"
#define DEMO_NAME_TITLE_LOGO "Title Logo"

#define VERSION "V.1.04"
#define MY_RSU_FILE_NAME "bench.rsu"

typedef struct _demo_window_list
{
	WM_HWIN demo_window_handle;
	uint32_t current_displayed;
	char demo_name[32];
	struct  _demo_window_list *next;
}DEMO_WINDOW_LIST;

/******************************************************************************
 External variables
 ******************************************************************************/

/******************************************************************************
 Private global variables
 ******************************************************************************/
static UW tcpudp_work[17520/4];
static DHCP* gpt_dhcp[1];
static SYS_TIME sys_time;
static R_TCPIP_SNTP_CLIENT_SETTINGS sntp_client_settings;
static DEMO_WINDOW_LIST *demo_window_list_head;
static WM_HWIN hWinFrameWindow, hWinSystemLogWindow, hWinNetworkStatWindow;
static DEMO_WINDOW_LIST* demo_window_add_list(DEMO_WINDOW_LIST *pdemo_window_list_head, WM_HWIN new_handle, char *demo_name);
static void demo_window_free_list(DEMO_WINDOW_LIST *pdemo_window_list);
static void demo_window_display_previous(DEMO_WINDOW_LIST *pdemo_window_list_head);
static void demo_window_display_next(DEMO_WINDOW_LIST *pdemo_window_list_head);
static void firmware_update_update_file_search(void);

static int32_t next_button_id, prev_button_id;
static int32_t first_wait_flag;
static usb_cfg_t usb_cfg;
static usb_ctrl_t  usb_ctrl;
static const uint8_t firmware_version[] = VERSION;
static FATFS fatfs;
static FILINFO filinfo;
static DIR dir;

/******************************************************************************
 External functions
 ******************************************************************************/
extern WM_HWIN CreateFrameWindow(void);
extern WM_HWIN CreateSystemLogWindow(void);
extern WM_HWIN CreateNetworkStatWindow(void);
extern WM_HWIN CreateStorageBenchmark(void);
extern WM_HWIN CreateTitleLogoWindow(void);
extern WM_HWIN CreateSecureUpdateWindow(void);

extern void display_update_usb_stat(WM_HWIN hWin, int8_t usb_stat);
extern void display_update_sd_stat(WM_HWIN hWin, int8_t sd_stat);
extern void display_update_ip_stat(WM_HWIN hWin, uint8_t *ip_address);
extern void display_update_time(WM_HWIN hWin, SYS_TIME *sys_time);
extern void display_update_demo_name(WM_HWIN hWin, char *demo_name);
extern void display_update_network_stat(WM_HWIN hWin);
extern void display_syslog_putchar(WM_HWIN hWin, char data);
extern int get_prev_button_id(void);
extern int get_next_button_id(void);
extern void firmware_update_init(void);
extern void firmware_update_mainloop(void);
extern void firmware_update_list_add(char *pstring);
extern void firmware_update_list_clear(void);
extern void bank_swap(void);
extern void firmware_update_editor_move(void);
extern void firmware_update_log_string(char *pstring);
extern int frame_next_button_enable(WM_HWIN hWin, uint8_t onoff);
extern int frame_prev_button_enable(WM_HWIN hWin, uint8_t onoff);
extern void firmware_update_status_initialize(void);
extern void firmware_update_ng_after_message(void);
extern void firmware_update_ok_after_message(void);
extern uint32_t get_update_data_size(void);
extern void firmware_update_temporary_area_string(U32 prog, U32 kilobyte);
extern bool is_firmupdatewaitstart(void);
extern void load_firmware_status(uint32_t *now_status, uint32_t *finish_status);
extern uint32_t load_firmware_process(void);
extern void storage_benchmark_main(void);
extern bool is_firmupdating(void);

/*******************************************************************************
 global variables and functions
********************************************************************************/
ER R_TCPIP_UserCallback(UB channel, UW eventid, VP param);
int32_t R_TCPIP_SntpUserCallback(int32_t ercd, int32_t stamp);
void main_100ms_display_update(void);
void main_sw2_status_update(void);
void emWinCallback(WM_MESSAGE * pMsg);
void callback_frame_window_to_main(int32_t id, int32_t event);
int32_t wait_first_display(void);
void firmware_version_read(char **ver_str);
void SetSwbankchangeRebootBotton(void);

WM_HWIN hWinSecureUpdatewindow, hWinTitleLogoWindow, hWinStorageBenchmark, hWinCryptoBenchmark1, hWinCryptoBenchmark2;

/******************************************************************************
 Function Name   : main
 Description     : Main task
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void main(void)
{
	UW tcpip_ramsize;
	uint32_t bank_info;
    uint16_t    usb_event, previous_usb_event;
    volatile uint32_t gui_delay_counter;
    uint32_t progress, previous_progress;
    char buff[256];
    uint32_t firmware_update_complete_flag = 0;
    uint32_t firmware_update_status;
    uint32_t firmware_update_finish_status;

    /* workaround for FLASH API */
    uint8_t * p_rom_section;    // ROM source location
    uint8_t * p_ram_section;    // RAM copy destination
    uint32_t  bytes_copied;

    /* enable MCU pins */
    R_Pins_Create();

    p_ram_section = (uint8_t *)__sectop("RPFRAM");
    p_rom_section = (uint8_t *)__sectop("PFRAM");

    /* Copy code from ROM to RAM. */
    for (bytes_copied = 0; bytes_copied < __secsize("PFRAM"); bytes_copied++)
    {
        p_ram_section[bytes_copied] = p_rom_section[bytes_copied];
    }


	first_wait_flag = -1;
	memset((void *)0x800000,0xff,0x60000);
	/* pin settings */
	R_ETHER_PinSet_ETHERC0_RMII();
	BenchmarkInit();

	/* reset touch ic */
	R_GPIO_PinDirectionSet(GPIO_PORT_0_PIN_7, GPIO_DIRECTION_OUTPUT);
	R_GPIO_PinWrite(GPIO_PORT_0_PIN_7, GPIO_LEVEL_LOW);
	R_BSP_SoftwareDelay(1, BSP_DELAY_MILLISECS);
	R_GPIO_PinWrite(GPIO_PORT_0_PIN_7, GPIO_LEVEL_HIGH);

	/* GUI initialization */
	GUI_Init();

	/* Prevention of flicker 1 */
	LCD_X_DisplayDriver(0, LCD_X_OFF, 0);

	/* generate frame window */
	demo_window_free_list(demo_window_list_head);
	hWinFrameWindow = CreateFrameWindow();

	/* generate sub windows */
	hWinSystemLogWindow = CreateSystemLogWindow();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, hWinSystemLogWindow, DEMO_NAME_SYSTEM_LOG);

	hWinNetworkStatWindow = CreateNetworkStatWindow();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, hWinNetworkStatWindow, DEMO_NAME_NETWORK_STAT);

	hWinStorageBenchmark = CreateStorageBenchmark();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, hWinStorageBenchmark, DEMO_NAME_STORAGEBENCH);

	hWinSecureUpdatewindow = CreateSecureUpdateWindow();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, hWinSecureUpdatewindow, DEMO_NAME_SECURE_UPDATE);

	hWinTitleLogoWindow = CreateTitleLogoWindow();
	demo_window_list_head = demo_window_add_list(demo_window_list_head, hWinTitleLogoWindow, DEMO_NAME_TITLE_LOGO);


	/* Prevention of flicker 2 */
	GUI_Delay(100);
	LCD_X_DisplayDriver(0, LCD_X_ON, 0);
	first_wait_flag = 0;

	/* get each GUI IDs */
	prev_button_id = get_prev_button_id();
	next_button_id = get_next_button_id();

	/* hello message */
	sprintf(buff, "hello, this is RX65N Envision Kit system log.\n");
	printf("%s", buff);
	firmware_update_log_string(buff);
	sprintf(buff, "built in %s, %s\n", __DATE__, __TIME__);
	printf("%s", buff);
	firmware_update_log_string(buff);
	sprintf(buff, "\n");
	printf("%s", buff);
	firmware_update_log_string(buff);

	/* TCP/IP initialization */
	R_SYS_TIME_Open();
	R_SYS_TIME_RegisterPeriodicCallback(main_100ms_display_update, 10);
	R_SYS_TIME_RegisterPeriodicCallback(cb_benchmark, 1);
    if(E_OK == lan_open())
    {
		tcpip_ramsize = tcpudp_get_ramsize();
		if(tcpip_ramsize > sizeof(tcpudp_work))
		{
			printf("lack of tcpip work ram, require %d byte, allocated %d byte.\n", tcpip_ramsize, sizeof(tcpudp_work));
			while(1);
		}
		tcpudp_open(tcpudp_work);
		printf("tcpip startup OK, require %d byte, allocated %d byte.\n", tcpip_ramsize, sizeof(tcpudp_work));
		printf("waiting ether link on...\n");
		printf("\n");
		R_ftp_srv_open(tcpudp_env[0].ipaddr);
		R_TCPIP_DnsClientOpen();

		/* register the process */
		R_SYS_TIME_RegisterPeriodicCallback(R_ftpd, 10);
		R_SYS_TIME_RegisterPeriodicCallback(R_httpd, 10);
    }
    R_FLASH_Open();
    R_FLASH_Control(FLASH_CMD_BANK_GET, &bank_info);
    sprintf(buff, "bank info = %d, start bank = %d\n", bank_info, bank_info ^ 0x01);
	printf("%s", buff);
	firmware_update_log_string(buff);

    usb_ctrl.module     = USB_IP0;
    usb_ctrl.type       = USB_HMSC;
    usb_cfg.usb_speed   = USB_FS;
	usb_cfg.usb_mode    = USB_HOST;
	R_USB_Open(&usb_ctrl, &usb_cfg);

	/* main loop */
	while(1)
	{
		usb_event = R_USB_GetEvent(&usb_ctrl);
		if(usb_event != previous_usb_event)
		{
			previous_usb_event = usb_event;
			display_update_usb_stat(hWinFrameWindow, usb_event);
			if(USB_STS_CONFIGURED == usb_event)
			{
				R_tfat_f_mount(0, &fatfs);
				firmware_update_update_file_search();
			}
			else if(USB_STS_DETACH == usb_event)
			{
				firmware_update_list_clear();
			}
		}

		storage_benchmark_main();
		progress = load_firmware_process();
		load_firmware_status(&firmware_update_status, &firmware_update_finish_status);

		if(true == is_firmupdating())
		{
			firmware_update_complete_flag = 0;
			if(previous_progress != progress)
			{
				previous_progress = progress;
				sprintf(buff, "installed %d %%\r\n", progress);
				firmware_update_log_string(buff);
				firmware_update_temporary_area_string(progress, (int)((((float)(progress)/100)*(float)(get_update_data_size()))/1024));
			}
		}
		else if(true == is_firmupdatewaitstart())
		{
			firmware_update_complete_flag = 0;
		}
		else
		{
			if(firmware_update_status == firmware_update_finish_status)
			{
				if(0 == firmware_update_complete_flag)
				{
					sprintf(buff, "installed %d %%\r\n", progress);
					firmware_update_temporary_area_string(progress, (int)((((float)(progress)/100)*(float)(get_update_data_size()))/1024));
					firmware_update_log_string(buff);
					firmware_update_ok_after_message();
					firmware_update_complete_flag = 1;
				}
			}
			else
			{
				if(0 == firmware_update_complete_flag)
				{
					firmware_update_ng_after_message();
					firmware_update_status_initialize();
					firmware_update_complete_flag = 1;
				}
			}
		}

		if(!(gui_delay_counter++ % 1000))
		{
			GUI_Delay(1);
		}
	}
}
/******************************************************************************
 End of function main()
 ******************************************************************************/

ER R_TCPIP_UserCallback(UB channel, UW eventid, VP param)
{
#if defined(DEBUG_PRINT)
    uint8_t*    ev_tbl[] =
    {
        "ETHER_EV_LINK_OFF",
        "ETHER_EV_LINK_ON",
        "ETHER_EV_COLLISION_IP",
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
        "DHCP_EV_LEASE_IP",
        "DHCP_EV_LEASE_OVER",
        "DHCP_EV_INIT",
        "DHCP_EV_INIT_REBOOT",
        "DHCP_EV_APIPA",
        "DHCP_EV_NAK",
        "DHCP_EV_FATAL_ERROR",
        "DHCP_EV_PLEASE_RESET"
    };
    printf("^^>>>R_TCPIP_UserCallback<<< ch:%d,eventID = %s\n", channel, ev_tbl[eventid]);
#endif /*#if defined(DEBUG_PRINT)*/
    switch(eventid)
    {
        case ETHER_EV_LINK_OFF:
        	R_ftp_srv_close();
        	R_TCPIP_SntpClientClose();
            break;

        case ETHER_EV_LINK_ON:
            break;

        case ETHER_EV_COLLISION_IP:
            break;

        case DHCP_EV_LEASE_IP:
            gpt_dhcp[channel] = (DHCP*)param;
        	R_ftp_srv_ip_address_update(tcpudp_env[0].ipaddr);
        	if(gpt_dhcp[channel]->dnsaddr[0] == 0)
        	{
        		R_TCPIP_DnsClientRegisterServerAddress(gpt_dhcp[channel]->gwaddr, gpt_dhcp[channel]->gwaddr);
        	}
        	else
        	{
        		R_TCPIP_DnsClientRegisterServerAddress(gpt_dhcp[channel]->dnsaddr, gpt_dhcp[channel]->dnsaddr2);
        	}

        	/* SNTP initialization */
        	sntp_client_settings.global_primary_sntp_server_name = SNTP_PRIMARY_SERVER_NAME;
        	sntp_client_settings.global_secondary_sntp_server_name = SNTP_SECONDARY_SERVER_NAME;
        	sntp_client_settings.time_offset = LOCAL_TIME_ZONE;
        	R_TCPIP_SntpClientOpen(R_TCPIP_SntpUserCallback, &sntp_client_settings);
        	R_TCPIP_SntpClientStartRequest();
        	printf("send sntp request\n");

#if defined(DEBUG_PRINT)
            printf("DHCP.ipaddr[4]   %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->ipaddr[0], gpt_dhcp[channel]->ipaddr[1],
                   gpt_dhcp[channel]->ipaddr[2], gpt_dhcp[channel]->ipaddr[3]);
            printf("DHCP.maskaddr[4] %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->maskaddr[0], gpt_dhcp[channel]->maskaddr[1],
                   gpt_dhcp[channel]->maskaddr[2], gpt_dhcp[channel]->maskaddr[3]);
            printf("DHCP.gwaddr[4]   %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->gwaddr[0], gpt_dhcp[channel]->gwaddr[1],
                   gpt_dhcp[channel]->gwaddr[2], gpt_dhcp[channel]->gwaddr[3]);
            printf("DHCP.dnsaddr[4]  %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->dnsaddr[0], gpt_dhcp[channel]->dnsaddr[1],
                   gpt_dhcp[channel]->dnsaddr[2], gpt_dhcp[channel]->dnsaddr[3]);
            printf("DHCP.dnsaddr2[4] %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->dnsaddr2[0], gpt_dhcp[channel]->dnsaddr2[1],
                   gpt_dhcp[channel]->dnsaddr2[2], gpt_dhcp[channel]->dnsaddr2[3]);
            printf("DHCP.macaddr[6]  %02X:%02X:%02X:%02X:%02X:%02X\n",
                   gpt_dhcp[channel]->macaddr[0],  gpt_dhcp[channel]->macaddr[1],  gpt_dhcp[channel]->macaddr[2],
                   gpt_dhcp[channel]->macaddr[3],  gpt_dhcp[channel]->macaddr[4],  gpt_dhcp[channel]->macaddr[5]);
            printf("DHCP.domain[%d] %s\n", strlen(gpt_dhcp[channel]->domain), gpt_dhcp[channel]->domain);
            printf("\n");
        	printf("start sntp request.\n");
#endif /*#if defined(DEBUG_PRINT)*/
            break;
        case DHCP_EV_LEASE_OVER:
            break;

        case DHCP_EV_INIT:
            break;

        case DHCP_EV_INIT_REBOOT:
            break;

        case DHCP_EV_APIPA:
            gpt_dhcp[channel] = (DHCP*)param;
#if defined(DEBUG_PRINT)
            printf("DHCP.ipaddr[4]   %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->ipaddr[0], gpt_dhcp[channel]->ipaddr[1],
                   gpt_dhcp[channel]->ipaddr[2], gpt_dhcp[channel]->ipaddr[3]);
            printf("DHCP.maskaddr[4] %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->maskaddr[0], gpt_dhcp[channel]->maskaddr[1],
                   gpt_dhcp[channel]->maskaddr[2], gpt_dhcp[channel]->maskaddr[3]);
            printf("DHCP.gwaddr[4]   %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->gwaddr[0], gpt_dhcp[channel]->gwaddr[1],
                   gpt_dhcp[channel]->gwaddr[2], gpt_dhcp[channel]->gwaddr[3]);
            printf("DHCP.dnsaddr[4]  %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->dnsaddr[0], gpt_dhcp[channel]->dnsaddr[1],
                   gpt_dhcp[channel]->dnsaddr[2], gpt_dhcp[channel]->dnsaddr[3]);
            printf("DHCP.dnsaddr2[4] %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->dnsaddr2[0], gpt_dhcp[channel]->dnsaddr2[1],
                   gpt_dhcp[channel]->dnsaddr2[2], gpt_dhcp[channel]->dnsaddr2[3]);
            printf("DHCP.macaddr[6]  %02X:%02X:%02X:%02X:%02X:%02X\n",
                   gpt_dhcp[channel]->macaddr[0],  gpt_dhcp[channel]->macaddr[1],  gpt_dhcp[channel]->macaddr[2],
                   gpt_dhcp[channel]->macaddr[3],  gpt_dhcp[channel]->macaddr[4],  gpt_dhcp[channel]->macaddr[5]);
            printf("DHCP.domain[%d] %s\n", strlen(gpt_dhcp[channel]->domain), gpt_dhcp[channel]->domain);
            printf("\n");
#endif /*#if defined(DEBUG_PRINT)*/
            break;

        case DHCP_EV_NAK:
            break;

        case DHCP_EV_FATAL_ERROR:
            break;

        case DHCP_EV_PLEASE_RESET:
        	tcpudp_close();
        	lan_close();
        	lan_open();
        	tcpudp_open(tcpudp_work);
            break;

        default:
            break;
    }
    return 0;
}

int32_t R_TCPIP_SntpUserCallback(int32_t ercd, int32_t stamp)
{
    uint8_t*    ev_tbl[] =
    {
        "SNTP_SUCCESS",
        "SNTP_ERR_BAD_UDP_SETTINGS",
        "SNTP_ERR_TIMEOUT",
        "SNTP_ERR_DNS_NOT_WORKED",
        "SNTP_ERR_SERVER_NOT_WORKED",
        "SNTP_ERR_NOT_OPENED",
        "SNTP_ERR_MODULE_NOT_REGISTERED",
        "SNTP_ERR_REQUEST_ALREADY_STARTED",
    };

    printf("^^>>>R_TCPIP_SntpUserCallback<<< eventID = %s\n", ev_tbl[ercd]);
	if(ercd == E_OK)
	{
		R_SYS_TIME_ConvertUnixTimeToSystemTime(stamp - 0x83AA7E80, &sys_time, LOCAL_TIME_ZONE);
		printf("current time is %02d/%02d/%04d %02d:%02d:%02d\n", sys_time.month, sys_time.day, sys_time.year, sys_time.hour, sys_time.min, sys_time.sec);
		printf("local time zone setting is %s\n", LOCAL_TIME_ZONE);
	}
	return 0;
}

char my_sw_charget_function(void)
{
	return 0;
}

void my_sw_charput_function(char data)
{
	display_syslog_putchar(hWinSystemLogWindow, data);
}

void main_100ms_display_update(void)
{
	DEMO_WINDOW_LIST *p;
	static DEMO_WINDOW_LIST *prev_p;

	R_SYS_TIME_GetCurrentTime(&sys_time);

	display_update_ip_stat(hWinFrameWindow, tcpudp_env[0].ipaddr);
	display_update_time(hWinFrameWindow, &sys_time);
	display_update_network_stat(hWinNetworkStatWindow);

	p = demo_window_list_head;
	while(1)
	{
		if(p->current_displayed)
		{
			if(prev_p != p)
			{
				prev_p = p;
				display_update_demo_name(hWinFrameWindow, p->demo_name);
				WM_BringToTop(p->demo_window_handle);
				if(p->demo_window_handle != hWinTitleLogoWindow)
				{
		    		WM_DeleteWindow(hWinTitleLogoWindow);
				}
			}
			break;
		}
		else
		{
			if(p->next == 0)
			{
				break;
			}
			else
			{
				p = p->next;
			}
		}
	}
}

void emWinCallback(WM_MESSAGE * pMsg)
{

}

void callback_frame_window_to_main(int32_t id, int32_t event)
{
	if(id == next_button_id)
	{
		if(event == WM_NOTIFICATION_CLICKED)
		{
			demo_window_display_next(demo_window_list_head);
		}
	}
	else if(id == prev_button_id)
	{
		if(event == WM_NOTIFICATION_CLICKED)
		{
			demo_window_display_previous(demo_window_list_head);
		}
	}
}

void goto_user_program_screen(void)
{
	demo_window_display_next(demo_window_list_head);
}
int32_t wait_first_display(void)
{
	return first_wait_flag;
}

void delete_window_to_main(WM_HWIN delete_handle)
{
	DEMO_WINDOW_LIST *p;
	DEMO_WINDOW_LIST *pbefore;

	p = demo_window_list_head; /*global */
	pbefore = NULL;
	if(p != NULL)
	{
		while (p->next != NULL)
		{
			if(p->demo_window_handle == delete_handle)
			{
				break;
			}
			if(pbefore == NULL)
			{
				pbefore = demo_window_list_head;
			}
			else
			{
				pbefore = p;
			}
			p = p->next;
		}
		if(p->next == NULL)
		{
			return;
		}
		if(pbefore != NULL)
		{
			pbefore->next = p->next;
			if(p->current_displayed == 1)
			{
				pbefore->current_displayed = 1;
			}
		}
		else
		{
			demo_window_list_head = p->next;
			if(p->current_displayed == 1)
			{
				demo_window_list_head->current_displayed = 1;
			}
		}
		free(p);
	}
}


static DEMO_WINDOW_LIST* demo_window_add_list(DEMO_WINDOW_LIST *pdemo_window_list_head, WM_HWIN new_handle, char *demo_name)
{
	DEMO_WINDOW_LIST *p;

	p = malloc(sizeof(DEMO_WINDOW_LIST));
	if(p == 0)
	{
		printf("malloc error occurred at demo_window_add_list()\n");
	}
	else
	{
		p->demo_window_handle = new_handle;
		strcpy(p->demo_name, demo_name);
		if(pdemo_window_list_head == NULL)
		{
			p->current_displayed = 1;
			p->next = NULL;
		}
		else
		{
			p->next = pdemo_window_list_head;
			pdemo_window_list_head = p;
			p = pdemo_window_list_head;
			pdemo_window_list_head->current_displayed = 1;
			pdemo_window_list_head->next->current_displayed = 0;
		}
	}
	return p;
}

static void demo_window_free_list(DEMO_WINDOW_LIST *pdemo_window_list_head)
{
	DEMO_WINDOW_LIST *p;

	if(pdemo_window_list_head != NULL)
	{
		while (pdemo_window_list_head->next != NULL)
		{
			p = pdemo_window_list_head->next;
			free(pdemo_window_list_head);
			pdemo_window_list_head = p;
		}
		pdemo_window_list_head = 0;
	}
}

static void demo_window_display_previous(DEMO_WINDOW_LIST *pdemo_window_list_head)
{
	DEMO_WINDOW_LIST *p, *p_prev;
	p = pdemo_window_list_head;

	while (1)
	{
		if(p->current_displayed)
		{
			break;
		}
		p_prev = p;
		p = p->next;
	}

	if(p == pdemo_window_list_head)
	{
		/* nothing to do */
	}
	else
	{
		p_prev->current_displayed = 1;
		p->current_displayed = 0;
		if(p_prev->demo_window_handle == pdemo_window_list_head->demo_window_handle)
		{
			frame_prev_button_enable(hWinFrameWindow, 0);
		}
	}
}

static void demo_window_display_next(DEMO_WINDOW_LIST *pdemo_window_list_head)
{
	DEMO_WINDOW_LIST *p;

	p = pdemo_window_list_head;

	while (1)
	{
		if(p->current_displayed)
		{
			break;
		}
		p = p->next;
	}

	if(p->next == 0)
	{
		/* nothing to do */
	}
	else
	{
		(p->next)->current_displayed = 1;
		p->current_displayed = 0;
		if((p->next)->next == NULL)
		{
			frame_next_button_enable(hWinFrameWindow, 0);
		}

	}
}

void firmware_version_read(char **ver_str)
{
	*ver_str = (char*)firmware_version;
}

void SetSwbankchangeRebootBotton(void)
{
	firmware_update_log_string(" Bank change ... ");
	GUI_Delay(1);
	firmware_update_editor_move();
	firmware_update_log_string("[ OK ]\r\n\r\n");
	GUI_Delay(1);
	firmware_update_log_string("Rebooting...");
	GUI_Delay(1);
	R_BSP_SoftwareDelay(3000, BSP_DELAY_MILLISECS);
	bank_swap();
    while(1);
}

static void firmware_update_update_file_search(void)
{
	FRESULT tfat_ret;
	int32_t i;

	firmware_update_list_clear();
	tfat_ret = R_tfat_f_opendir (&dir,"0:");
	if(tfat_ret == TFAT_FR_OK)
	{
		while(1)
		{
			tfat_ret = R_tfat_f_readdir (&dir,	&filinfo );
			if(tfat_ret == TFAT_FR_OK)
			{
				if(filinfo.fname[0] == '\0')
				{
					break;
				}
				if(TFAT_AM_DIR == (filinfo.fattrib & TFAT_AM_DIR) )
				{
					continue;
				}
				for(i= 0;i<sizeof(filinfo.fname);i++)
				{
					if('A' <=  filinfo.fname[i] && filinfo.fname[i] <= 'Z' )
					{
						filinfo.fname[i] += 0x20;
					}
				}
				if(0 != strstr(filinfo.fname,".rsu"))
				{
					firmware_update_list_add(filinfo.fname);
				}
			}
			else
			{
				break;
			}
		}
	}
}

/******************************************************************************
 End  Of File
 ******************************************************************************/

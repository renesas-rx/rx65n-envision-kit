/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.40                          *
*        Compiled Mar 17 2017, 15:35:10                              *
*        (c) 2017 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include "stdio.h"
#include "string.h"
#include "r_sys_time_rx_if.h"
#include "r_usb_basic_if.h"
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_TEXT_0    (GUI_ID_USER + 0x01)
#define ID_TEXT_1    (GUI_ID_USER + 0x02)
#define ID_TEXT_2    (GUI_ID_USER + 0x03)
#define ID_TEXT_3    (GUI_ID_USER + 0x04)
#define ID_TEXT_4    (GUI_ID_USER + 0x05)
#define ID_BUTTON_0    (GUI_ID_USER + 0x06)
#define ID_BUTTON_1    (GUI_ID_USER + 0x07)


// USER START (Optionally insert additional defines)
void display_update_usb_stat(WM_HWIN hWin, int8_t usb_stat);
void display_update_sd_stat(WM_HWIN hWin, int8_t sd_stat);
void display_update_time(WM_HWIN hWin, SYS_TIME *sys_time);
void display_update_ip_stat(WM_HWIN hWin, uint8_t *ip_address);
void display_update_demo_name(WM_HWIN hWin, uint8_t *demo_name);

int get_prev_button_id(void);
int get_next_button_id(void);

extern void callback_frame_window_to_main(int32_t id, int32_t event);
extern bool is_firmupdating(void);
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "FrameWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "System Log", ID_TEXT_0, 5, 2, 122, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "USB", ID_TEXT_1, 7, 250, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "SD", ID_TEXT_2, 85, 250, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Time", ID_TEXT_3, 312, 250, 157, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "IP", ID_TEXT_4, 157, 250, 109, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 309, 0, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 392, 0, 80, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'FrameWindow'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00000000));
    //
    // Initialization of 'System Log'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    //
    // Initialization of 'USB'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    TEXT_SetText(hItem, "USB: detach");
    //
    // Initialization of 'SD'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "SD: detach");
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    //
    // Initialization of 'Time'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "TIME: ");
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    //
    // Initialization of 'IP'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "IP: 0.0.0.0");
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetText(hItem, "prev");
    BUTTON_SetFont(hItem, GUI_FONT_13_1);
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetText(hItem, "next");
    BUTTON_SetFont(hItem, GUI_FONT_13_1);
    // USER START (Optionally insert additional code for further widget initialization)
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    WM_HideWindow(hItem);
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
    	if(true != is_firmupdating())
    	{
			callback_frame_window_to_main(ID_BUTTON_0, WM_NOTIFICATION_CLICKED);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
			WM_ShowWindow(hItem);
    	}
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
      	if(true != is_firmupdating())
      	{
			callback_frame_window_to_main(ID_BUTTON_1, WM_NOTIFICATION_CLICKED);
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
			WM_ShowWindow(hItem);
      	}
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateFrameWindow
*/
WM_HWIN CreateFrameWindow(void);
WM_HWIN CreateFrameWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
void display_update_usb_stat(WM_HWIN hWin, int8_t usb_stat)
{
	  WM_HWIN hItem;
	  char string[256] = {0};

	  hItem = WM_GetDialogItem(hWin, ID_TEXT_1);
	  if(USB_STS_CONFIGURED == usb_stat)
	  {
		  sprintf(string, "USB: attach");
		  TEXT_SetText(hItem, string);
	  }
	  else if(USB_STS_DETACH == usb_stat)
	  {
		  sprintf(string, "USB: detach");
		  TEXT_SetText(hItem, string);
	  }
	  else
	  {
		  return;
	  }
	  TEXT_SetText(hItem, string);
	  printf("%s", string);
	  printf("\n");
}

void display_update_sd_stat(WM_HWIN hWin, int8_t sd_stat)
{
	  WM_HWIN hItem;
	  char string[256] = {0};

	  hItem = WM_GetDialogItem(hWin, ID_TEXT_2);
	  if(sd_stat == 0)
	  {
		  sprintf(string, "SD: attach");

	  }
	  else 	  if(sd_stat == -1)
	  {
		  sprintf(string, "SD: detach");

	  }
	  TEXT_SetText(hItem, string);
	  printf("%s", string);
	  printf("\n");
}

void display_update_time(WM_HWIN hWin, SYS_TIME *sys_time)
{
	  WM_HWIN hItem;
	  char string[256] = {0};
	  static char pre_string[256] = {0};

	  hItem = WM_GetDialogItem(hWin, ID_TEXT_3);
	  sprintf(string, "Time: %02d/%02d/%04d %02d:%02d:%02d", sys_time->month, sys_time->day, sys_time->year, sys_time->hour, sys_time->min, sys_time->sec);
	  if(strcmp(string, pre_string))
	  {
		  TEXT_SetText(hItem, string);
	  }
	  strcpy(pre_string, string);
}

void display_update_ip_stat(WM_HWIN hWin, uint8_t *ip_address)
{
	  WM_HWIN hItem;
	  char string[256] = {0};
	  static char pre_string[256] = {0};

	  hItem = WM_GetDialogItem(hWin, ID_TEXT_4);
	  sprintf(string, "IP: %d.%d.%d.%d", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
	  if(strcmp(string, pre_string))
	  {
		  TEXT_SetText(hItem, string);
	  }
	  strcpy(pre_string, string);
}

void display_update_demo_name(WM_HWIN hWin, uint8_t *demo_name)
{
	  WM_HWIN hItem;
	  char string[256] = {0};
	  static char pre_string[256] = {0};

	  hItem = WM_GetDialogItem(hWin, ID_TEXT_0);
	  sprintf(string, "%s", demo_name);
	  if(strcmp(string, pre_string))
	  {
		  TEXT_SetText(hItem, string);
	  }
	  strcpy(pre_string, string);
}

int get_prev_button_id(void)
{
	return ID_BUTTON_0;
}

int get_next_button_id(void)
{
	return ID_BUTTON_1;
}

int frame_prev_button_enable(WM_HWIN hWin, uint8_t onoff)
{
  WM_HWIN hItem;

  hItem = WM_GetDialogItem(hWin, ID_BUTTON_0);
  if(onoff == 1)
  {
	    WM_ShowWindow(hItem);

  }else
  {
	    WM_HideWindow(hItem);

  }
	return 0;
}

int frame_next_button_enable(WM_HWIN hWin, uint8_t onoff)
{
  WM_HWIN hItem;

  hItem = WM_GetDialogItem(hWin, ID_BUTTON_1);
  if(onoff == 1)
  {
	    WM_ShowWindow(hItem);

  }else
  {
	    WM_HideWindow(hItem);

  }
//	WM_DisableWindow(hwin);
//	WM_CF_HIDE
	return 0;
}

// USER END

/*************************** End of file ****************************/

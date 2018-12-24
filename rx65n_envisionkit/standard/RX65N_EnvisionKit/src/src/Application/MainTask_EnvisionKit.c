/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : MainTask_EnvisionKit.c
Purpose     : Start of demo
----------------------------------------------------------------------
*/

#include "DIALOG.h"
#ifndef WIN32
  #include "platform.h"
  #include "r_gpio_rx_if.h"
  #include "r_usb_basic_if.h"
  #include "r_usb_hmsc_if.h"
  #include "r_tfat_lib.h"
#endif
#define MAINTASK_ENVISIONKIT

#include "EnvisionKit.h"
#include "Resource.h"
#include "LCDConf.h"

#ifndef WIN32
FATFS fatfs;
#endif

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
#define TEST 0

#if TEST

#include "GUI_Private.h"
#include "DIALOG.h"

int _Continue;

static void _cbWin(WM_MESSAGE * pMsg) {
  GUI_RECT Rect;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    WM_GetClientRectEx(pMsg->hWin, &Rect);
    GUI_SetBkColor(GUI_YELLOW);
    GUI_Clear();
    GUI_SetColor(GUI_RED);
    GUI_DrawRectEx(&Rect);
    GUI_SetColor(GUI_BLUE);
    GUI_DispStringInRect("Move me...", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

static void _DrawArcs(void) {
  GUI_SetPenSize(10);
  GUI_SetColor(GUI_CYAN);
  GUI_AA_DrawArc(300, 100, 50, 0, 0, 135);
  GUI_SetColor(GUI_BLUE);
  GUI_AA_DrawArc(320, 100, 50, 0, 0, 225);
  GUI_SetColor(GUI_RED);
  GUI_AA_DrawArc(340, 100, 50, 0, 315, 450);
  GUI_SetPenSize(1);
}

static void _TestArc(int Loop) {
  int i, j;

  do {
    GUI_SetPenSize(10);
    for (i = 0; i <= 360; i += 15) {
      for (j = 0; j < 2; j++) {
        if (j) {
          LCDCONF_EnableDave2D();
        } else {
          LCDCONF_DisableDave2D();
        }
        GUI_SetColor(GUI_CYAN);
        GUI_AA_DrawArc(50 + j * 100, 50, 40, 0, 0, i);
        GUI_SetColor(GUI_GREEN);
        GUI_AA_DrawArc(50 + j * 100, 150, 40, 0, 90, i + 90);
        GUI_SetColor(GUI_RED);
        GUI_AA_DrawArc(250 + j * 100, 50, 40, 0, 135, i + 135);
        GUI_SetColor(GUI_MAGENTA);
        GUI_AA_DrawArc(250 + j * 100, 150, 40, 0, 225, i + 225);
      }
      GUI_Delay(200);
      GUI_Clear();
    }
    GUI_SetPenSize(1);
  } while (Loop);
}

static void _cbBk(WM_MESSAGE * pMsg) {
  const GUI_POINT aPoints[] = {
      { 100, 150 },
      { 150, 100 },
      { 150, 150 },
  };
  int i, Id, NCode;
  static int Flag;
  WM_HWIN hItem;

  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);
    switch (Id) {
    case GUI_ID_BUTTON1:
      NCode = pMsg->Data.v;
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        _Continue = 1;
        break;
      }
      break;
    case GUI_ID_BUTTON0:
      NCode = pMsg->Data.v;
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        hItem = WM_GetDialogItem(pMsg->hWin, Id);
        Flag ^= 1;
        if (Flag) {
#ifndef WIN32
          LCDCONF_DisableDave2D();
#endif
          BUTTON_SetText(hItem, "Off");
        } else {
#ifndef WIN32
          LCDCONF_EnableDave2D();
#endif
          BUTTON_SetText(hItem, "On");
        }
        WM_InvalidateWindow(pMsg->hWin);
        break;
      }
      break;
    }
    break;
  case WM_PAINT:
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    for (i = 0; i < 100; i++) {
      GUI_AA_DrawLine(0, i * 2, 200, 150);
    }
    GUI_SetColor(GUI_RED);
    GUI_AA_FillCircle(100, 100, 20);
    GUI_SetColor(GUI_MAGENTA);
    GUI_SetPenSize(4);
    GUI_AA_DrawCircle(150, 120, 80);
    GUI_SetColor(GUI_GREEN);
    GUI_AA_FillPolygon(aPoints, GUI_COUNTOF(aPoints), 20, 10);
    GUI_SetColor(GUI_YELLOW);
    GUI_AA_DrawPolyOutline(aPoints, GUI_COUNTOF(aPoints), 5, 160, 30);
    _DrawArcs();
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

static void _Test(void) {
  int xSize, ySize;
  WM_HWIN hItem;
  WM_HWIN hWin;

  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  WM_SetSize(WM_HBKWIN, xSize, ySize);
  WM_MOTION_Enable(1);
  WM_SetCallback(WM_HBKWIN, _cbBk);
  hWin = WM_CreateWindow(50, 100, 80, 80, WM_CF_SHOW | WM_CF_MOTION_X | WM_CF_MOTION_Y, _cbWin, 0);
  hItem = BUTTON_Create(200, 250, 80, 20, GUI_ID_BUTTON0, WM_CF_SHOW);
  BUTTON_SetText(hItem, "On");
  hItem = BUTTON_Create(180, 2, 120, 20, GUI_ID_BUTTON1, WM_CF_SHOW);
  BUTTON_SetText(hItem, "continue with demo...");
  while (_Continue == 0) {
    GUI_Delay(10);
  }
  hItem = WM_GetDialogItem(hWin, GUI_ID_BUTTON0);
  WM_DeleteWindow(hItem);
  hItem = WM_GetDialogItem(hWin, GUI_ID_BUTTON1);
  WM_DeleteWindow(hItem);
}

#endif

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask_EnvisionKit
*/
void MainTask_EnvisionKit(void) {
#ifndef WIN32
  uint16_t    event;
  usb_ctrl_t  ctrl;
  usb_cfg_t   cfg;

  /* reset touch ic */
  R_GPIO_PinDirectionSet(GPIO_PORT_0_PIN_7, GPIO_DIRECTION_OUTPUT);
  R_GPIO_PinWrite(GPIO_PORT_0_PIN_7, GPIO_LEVEL_LOW);
  R_BSP_SoftwareDelay(1, BSP_DELAY_MILLISECS);
  R_GPIO_PinWrite(GPIO_PORT_0_PIN_7, GPIO_LEVEL_HIGH);
  ctrl.module     = USB_IP0;
  ctrl.type       = USB_HMSC;
  cfg.usb_speed   = USB_FS;
  cfg.usb_mode    = USB_HOST;
  R_USB_Open(&ctrl, &cfg);

  #endif
  GUI_Init();
#if TEST
  _TestArc(0);
  _DrawArcs();
  LCDCONF_EnableDave2D();
  GUI_Clear();
  _DrawArcs();
  GUI_Clear();
#endif
  LCDCONF_EnableDave2D();
  WM_MULTIBUF_Enable(1);
  BUTTON_SetReactOnLevel();
#if TEST
  _Test();
#endif
  InitUpdate();
  StartScreen();
  while (1) {
#ifndef WIN32
    event = R_USB_GetEvent(&ctrl);
    switch (event)
    {
        case USB_STS_CONFIGURED :
            R_tfat_f_mount(0, &fatfs);   /* Create a file object. */
            break;
        default:
        	break;
    }
#if (TEST == 0)
    CheckPressedState();
#endif
#endif
    GUI_Delay(1);
  }
}

/*************************** End of file ****************************/

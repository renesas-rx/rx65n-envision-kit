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
File        : APP_DrawingEngine.c
Purpose     : Drawing engine demo
----------------------------------------------------------------------
*/

#include "DIALOG.h"

#include "EnvisionKit.h"
#include "Resource.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_BUTTON_HOME   (GUI_ID_USER + 0)
#define ID_BUTTON_HELP   (GUI_ID_USER + 1)
#define ID_BUTTON_ONOFF  (GUI_ID_USER + 2)

#define BM_HOME       &bmHome_40x40
#define BM_HELP_0     &bmButtonHelp_0_60x40
#define BM_HELP_1     &bmButtonHelp_1_60x40
#define BM_BUTTON_0_0 &bmButtonOff_0_80x40
#define BM_BUTTON_0_1 &bmButtonOff_1_80x40
#define BM_BUTTON_1_0 &bmButtonOn_0_80x40
#define BM_BUTTON_1_1 &bmButtonOn_1_80x40
#define BM_BK         &bmGoldenGate_480x272
#define BM_CLOUD      &bmCloud_220x120
#define BM_RX         &bmRenesasRX_155x55

#define FONT_SMALL    &GUI_Font24_AA4
#define FONT_MEDIUM   &GUI_Font32_AA4

#define PIXEL_PER_SECOND 50  // Speed for moving cloud

#define BORDER 20  // Distance from buttons to border

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _cbButtonHelp
*/
static void _cbButtonHelp(WM_MESSAGE * pMsg) {
  const GUI_BITMAP * pBm;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    if (BUTTON_IsPressed(pMsg->hWin)) {
      pBm = BM_HELP_1;
    } else {
      pBm = BM_HELP_0;
    }
    GUI_DrawBitmap(pBm, 0, 0);
    break;
  default:
    BUTTON_Callback(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbButtonOnOff
*/
static void _cbButtonOnOff(WM_MESSAGE * pMsg) {
  const GUI_BITMAP * pBm;
  static int * pOnOff;
  int IsPressed;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    BUTTON_GetUserData(pMsg->hWin, &pOnOff, sizeof(pOnOff));
    IsPressed = BUTTON_IsPressed(pMsg->hWin);
    if (*pOnOff) {
      pBm = IsPressed ? BM_BUTTON_1_1 : BM_BUTTON_1_0;
    } else {
      pBm = IsPressed ? BM_BUTTON_0_1 : BM_BUTTON_0_0;
    }
    GUI_DrawBitmap(pBm, 0, 0);
    break;
  default:
    BUTTON_Callback(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbButtonHome
*/
static void _cbButtonHome(WM_MESSAGE * pMsg) {
  const GUI_BITMAP * pBm;
  static int * pOnOff;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    BUTTON_GetUserData(pMsg->hWin, &pOnOff, sizeof(pOnOff));
    pBm = BM_HOME;
    GUI_DrawBitmap(pBm, 0, 0);
    break;
  default:
    BUTTON_Callback(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbWin
*/
static void _cbWin(WM_MESSAGE * pMsg) {
  static GUI_TIMER_HANDLE hTimer;
  static int xPosCloud;
  static int xPosRX, yPosRX;
  static int IsPressed;
  static GUI_POINT Pos;
  static int OnOff;
  static int * pOnOff;
  static GUI_TIMER_TIME tLastEvent;
  GUI_TIMER_TIME tNow, tUsed;
  const GUI_BITMAP * pBm;
  GUI_PID_STATE * pState;
  WM_HWIN hItem;
  int xSizeWindow, ySizeWindow;
  int Id, NCode;

  switch (pMsg->MsgId) {
  case WM_CREATE:
#ifndef WIN32
    OnOff = LCDCONF_GetDaveActive();
#endif
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    //
    // Prepare cloud moving
    //
    hTimer = WM_CreateTimer(pMsg->hWin, 0, TIMER_PERIOD, 0);
    xPosCloud = xSizeWindow;
    tLastEvent = GUI_GetTime();
    //
    // Prepare RX moving
    //
    pBm = BM_RX;
    xPosRX = (xSizeWindow - pBm->XSize) / 2;
    yPosRX = (ySizeWindow - pBm->YSize) / 2;
    //
    // Create On/Off button
    //
    pBm = BM_BUTTON_0_0;
    hItem = BUTTON_CreateUser(xSizeWindow - pBm->XSize - BORDER, ySizeWindow - pBm->YSize - BORDER, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ONOFF, sizeof(pOnOff));
    pOnOff = &OnOff;
    BUTTON_SetUserData(hItem, &pOnOff, sizeof(pOnOff));
    WM_SetCallback(hItem, _cbButtonOnOff);
    //
    // Create home button
    //
    pBm = BM_HOME;
    hItem = BUTTON_CreateUser(BORDER, BORDER, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_HOME, 0);
    WM_SetCallback(hItem, _cbButtonHome);
    //
    // Create help button
    //
    pBm = BM_HELP_0;
    hItem = BUTTON_CreateUser(xSizeWindow - pBm->XSize - BORDER, BORDER, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_HELP, 0);
    WM_SetCallback(hItem, _cbButtonHelp);
    break;
  case WM_TIMER:
    WM_RestartTimer(hTimer, 0);
    pBm = BM_CLOUD;
    tNow = GUI_GetTime();
    tUsed = tNow - tLastEvent;
    tLastEvent = tNow;
    xPosCloud -= (PIXEL_PER_SECOND * tUsed) / 1000;
    if (xPosCloud < -pBm->XSize) {
      xPosCloud = WM_GetWindowSizeX(pMsg->hWin);
    }
    WM_InvalidateWindow(pMsg->hWin);
    break;
  case WM_TOUCH:
    pState = (GUI_PID_STATE *)pMsg->Data.p;
    if (pState) {
      if (pState->Pressed) {
        pBm = BM_RX;
        if (IsPressed == 0) {
          if ((pState->x >= xPosRX) && 
              (pState->y >= yPosRX) && 
              (pState->x <= (xPosRX + pBm->XSize - 1)) && 
              (pState->y <= (yPosRX + pBm->YSize - 1))) {
            IsPressed = 1;
            Pos.x = pState->x;
            Pos.y = pState->y;
            WM_SetCapture(pMsg->hWin, 1);
          }
        } else {
          xPosRX += (pState->x - Pos.x);
          yPosRX += (pState->y - Pos.y);
          WM_InvalidateWindow(pMsg->hWin);
          Pos.x = pState->x;
          Pos.y = pState->y;
        }
      } else {
        WM_ReleaseCapture();
        IsPressed = 0;
      }
    }
    break;
  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch (Id) {
    case ID_BUTTON_ONOFF:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        OnOff ^= 1;
        if (OnOff) {
          LCDCONF_EnableDave2D();
        } else {
          LCDCONF_DisableDave2D();
        }
        break;
      }
      break;
    case ID_BUTTON_HELP:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_DeleteWindow(pMsg->hWin);
        HelpDialog(DEMO_0, DrawingEngine);
        break;
      }
      break;
    case ID_BUTTON_HOME:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_DeleteWindow(pMsg->hWin);
        StartScreen();
        break;
      }
      break;
    }
    break;
  case WM_PAINT:
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    //
    // Background
    //
    pBm = BM_BK;
    GUI_DrawBitmap(pBm, 0, 0);
    //
    // Cloud
    //
    pBm = BM_CLOUD;
    GUI_DrawBitmap(pBm, xPosCloud, BORDER);
#if 0  // Leave code for generating bitmap...
    //
    // Title
    //
    pBm = BM_HOME;
    GUI_SetColor(GUI_BLACK);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(FONT_MEDIUM);
    GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
    GUI_DispStringAt("2D Drawing Engine demo", 220, BORDER + pBm->YSize / 2);
#endif
    //
    // Label
    //
    pBm = BM_BUTTON_0_0;
    GUI_SetColor(GUI_BLACK);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(FONT_SMALL);
    GUI_SetTextAlign(GUI_TA_RIGHT | GUI_TA_VCENTER);
    GUI_DispStringAt("2D Drawing Engine", xSizeWindow - pBm->XSize - BORDER - 10, ySizeWindow - pBm->YSize / 2 - BORDER);
    //
    // RX
    //
    pBm = BM_RX;
    GUI_DrawBitmap(pBm, xPosRX, yPosRX);
    break;
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
*       DrawingEngine
*/
void DrawingEngine(void) {
  WM_CreateWindow(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_CF_SHOW, _cbWin, 0);
}

/*************************** End of file ****************************/

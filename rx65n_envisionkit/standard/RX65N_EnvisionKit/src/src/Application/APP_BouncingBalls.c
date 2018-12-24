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
File        : APP_BouncingBalls.c
Purpose     : Bouncing ball demo
----------------------------------------------------------------------
*/

#include "APP_BouncingBalls_Private.h"
#include "LCDConf.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define TRIGGER_DAVE 1

/*********************************************************************
*
*       Static (const) data
*
**********************************************************************
*/
static const GUI_BITMAP * _apBalls[] = {
  &bmBallGray_30x30,
  &bmBallGreen_30x30,
  &bmBallRed_30x30,
  &bmBallYellow_30x30,
};

static const GUI_COLOR _aColors[] = {
  COLOR_GRAY,
  COLOR_GREEN,
  COLOR_RED,
  COLOR_YELLOW,
};

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static void (* _pfKeepArea) (BOUNCE_DATA * pData)                       = KeepArea_FPU;
static void (* _pfMoveBalls)(BOUNCE_DATA * pData, GUI_TIMER_TIME tDiff) = MoveBalls_FPU;

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
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
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
*       _CreateBall
*/
static void _CreateBall(BOUNCE_DATA * pData) {
  static int Index;
  BALL * pBall;

  pBall = calloc(1, sizeof(BALL));
  pBall->pBm = _apBalls[Index];
  pBall->Color = _aColors[Index];
  if (++Index == GUI_COUNTOF(_apBalls)) {
    Index = 0;
  }
  pBall->vx = VX_MIN + VX_RANGE + (float)((rand() % (VX_RANGE * 2)) - VX_RANGE);
  pBall->vy = 0;
  pBall->xPos = pBall->yPos = R_BALL + PENSIZE;
  pBall->pNext = pData->pFirst;
  if (pData->pFirst) {
    pData->pFirst->pPrev = pBall;
  }
  pData->pFirst = pBall;
  pData->NumBalls++;
}

/*********************************************************************
*
*       _DeleteSlowBalls
*/
static void _DeleteSlowBalls(BOUNCE_DATA * pData) {
  BALL * pBall;
  BALL * pBallDelete;
  
  pBall = pData->pFirst;
  while (pBall) {
    pBallDelete = pBall;
    pBall = pBall->pNext;
    if ((pBallDelete->yPos + R_BALL) > (pData->y1 - 5)) {
      if (abs(pBallDelete->vy) < THRESHOLD) {
        if (pBallDelete->pPrev) {
          pBallDelete->pPrev->pNext = pBallDelete->pNext;
        }
        if (pBallDelete->pNext) {
          pBallDelete->pNext->pPrev = pBallDelete->pPrev;
        }
        free(pBallDelete);
        pData->NumBalls--;
      }
    }
  }
}

/*********************************************************************
*
*       _cbBounce
*/
static void _cbBounce(WM_MESSAGE * pMsg) {
  static GUI_TIMER_TIME tLastEvent;
  static int FrameVisible;
  static BOUNCE_DATA Data;
  int xSizeWindow, ySizeWindow;
  GUI_TIMER_TIME tNow, tDiff;
  BALL * pBall;
  BALL * pBallDelete;

  switch (pMsg->MsgId) {
  case WM_DELETE:
    FrameVisible = 0;
    pBall = Data.pFirst;
    while (pBall) {
      pBallDelete = pBall;
      pBall = pBall->pNext;
      free(pBallDelete);
    }
    Data.NumBalls = 0;
    Data.pFirst = NULL;
    break;
  case WM_CREATE:
    tLastEvent = GUI_GetTime();
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    Data.x0 = Data.y0 = PENSIZE;
    Data.x1 = xSizeWindow - PENSIZE - 1;
    Data.y1 = ySizeWindow - PENSIZE - 1;
    _CreateBall(&Data);
    WM_CreateTimer(pMsg->hWin, ID_TIMER_MOVE, TIMER_PERIOD / 2, 0);
    WM_CreateTimer(pMsg->hWin, ID_TIMER_CREATE, CREATE_PERIOD, 0);
    break;
  case WM_TIMER:
    WM_RestartTimer(pMsg->Data.v, 0);
    switch (WM_GetTimerId(pMsg->Data.v)) {
    case ID_TIMER_CREATE:
      if (Data.NumBalls < MAX_NUM_BALLS) {
        _CreateBall(&Data);
      }
      break;
    case ID_TIMER_MOVE:
      tNow = GUI_GetTime();
      tDiff = tNow - tLastEvent;
      tLastEvent = tNow;
      _pfMoveBalls(&Data, tDiff);
      _pfKeepArea(&Data);
      _DeleteSlowBalls(&Data);
      break;
    }
    WM_InvalidateWindow(pMsg->hWin);
    break;
  case WM_PAINT:
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    if (FrameVisible == 0) {
      GUI_SetColor(COLOR_SHAPE);
      GUI_FillRect(0, 0, xSizeWindow - 1, PENSIZE - 1);
      GUI_FillRect(0, PENSIZE, PENSIZE - 1, ySizeWindow - PENSIZE - 1);
      GUI_FillRect(xSizeWindow - PENSIZE, PENSIZE, xSizeWindow - 1, ySizeWindow - PENSIZE - 1);
      GUI_FillRect(0, ySizeWindow - PENSIZE, xSizeWindow - 1, ySizeWindow - 1);
      FrameVisible = 1;
    }
    GUI_FillRect(PENSIZE, PENSIZE, xSizeWindow - PENSIZE- 1, ySizeWindow - PENSIZE - 1);
#if 0
    GUI_SetColor(GUI_BLACK);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_DispDecAt(Data.NumBalls, 300, 10, 2);
    GUI_SetColor(COLOR_BK);
#endif
    pBall = Data.pFirst;
    while (pBall) {
#if 0
      GUI_DrawBitmap(pBall->pBm, (int)pBall->xPos - R_BALL,(int)pBall->yPos - R_BALL);
#else
      GUI_SetColor(pBall->Color);
      GUI_FillCircle((int)pBall->xPos, (int)pBall->yPos, R_BALL - 1);
      GUI_SetPenSize(4);
      GUI_SetColor(COLOR_SHAPE);
      GUI_AA_DrawCircle((int)pBall->xPos, (int)pBall->yPos, R_BALL - PENSIZE / 2);
      GUI_SetPenSize(1);
#endif
      pBall = pBall->pNext;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _ManagePointers
*/
#if (TRIGGER_DAVE == 0)
static void _ManagePointers(int OnOff) {
  switch (OnOff) {
  case 0:
    _pfKeepArea  = KeepArea_NOFPU;
    _pfMoveBalls = MoveBalls_NOFPU;
    break;
  case 1:
    _pfKeepArea  = KeepArea_FPU;
    _pfMoveBalls = MoveBalls_FPU;
    break;
  }
}
#endif

/*********************************************************************
*
*       _cbWin
*/
static void _cbWin(WM_MESSAGE * pMsg) {
  const GUI_BITMAP * pBm;
  int xSizeWindow, ySizeWindow;
  int xSizeWindow_tmp;
  static int OnOff = 1;
  static int * pOnOff;
  WM_HWIN hItem;
  int Id, NCode;

  switch (pMsg->MsgId) {
  case WM_CREATE:
#if TRIGGER_DAVE
#ifndef WIN32
    OnOff = LCDCONF_GetDaveActive();
#endif
#else
    _ManagePointers(OnOff);
#endif
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    //
    // Create bouncing window
    //
    pBm = BM_BUTTON_0_0;
    WM_CreateWindowAsChild(BORDER, BORDER + pBm->YSize + BORDER, xSizeWindow - BORDER - pBm->XSize - 2 * BORDER, ySizeWindow - BORDER - pBm->YSize - 2 * BORDER, pMsg->hWin, WM_CF_SHOW, _cbBounce, 0);
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
  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch (Id) {
    case ID_BUTTON_ONOFF:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        OnOff ^= 1;
#if TRIGGER_DAVE
        if (OnOff) {
          LCDCONF_EnableDave2D();
        } else {
          LCDCONF_DisableDave2D();
        }
#else
        _ManagePointers(OnOff);
#endif
        break;
      }
      break;
    case ID_BUTTON_HELP:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_DeleteWindow(pMsg->hWin);
        HelpDialog(DEMO_1, BouncingBalls);
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
    GUI_SetBkColor(COLOR_BK);
    GUI_Clear();
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(FONT_MEDIUM);
    GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_VCENTER);
    pBm = BM_HOME;
    GUI_DispStringAt("Bouncing Balls Demo", BORDER + pBm->XSize + BORDER, BORDER + pBm->YSize / 2);
    GUI_SetTextAlign(GUI_TA_CENTER | GUI_TA_BOTTOM);
    pBm = BM_BUTTON_0_0;
#if TRIGGER_DAVE
    xSizeWindow_tmp = xSizeWindow - BORDER - pBm->XSize - 18;
    GUI_SetFont(FONT_SMALL);
    GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_BOTTOM);
    GUI_DispStringAt("2D Drawing", xSizeWindow_tmp, ySizeWindow - BORDER - pBm->YSize - 40);
    GUI_SetFont(FONT_SMALL);
    GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_BOTTOM);
    GUI_DispStringAt("Engine", xSizeWindow_tmp, ySizeWindow - BORDER - pBm->YSize - 10);
#else
    GUI_DispStringAt("FPU", xSizeWindow - BORDER - pBm->XSize / 2, ySizeWindow - BORDER - pBm->YSize - 10);
#endif
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
*       BouncingBalls
*/
void BouncingBalls(void) {
  WM_CreateWindow(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_CF_SHOW, _cbWin, 0);
}

/*************************** End of file ****************************/

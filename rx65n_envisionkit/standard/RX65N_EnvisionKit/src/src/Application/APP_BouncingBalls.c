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
#define TRIGGER_DAVE  0

#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_BUTTON_0   (GUI_ID_USER + 0x01)
#define ID_BUTTON_1   (GUI_ID_USER + 0x02)
#define ID_BUTTON_2   (GUI_ID_USER + 0x03)
#define ID_CHECKBOX_0 (GUI_ID_USER + 0x04)
#define ID_CHECKBOX_1 (GUI_ID_USER + 0x05)
#define ID_TEXT_0     (GUI_ID_USER + 0x06)
#define ID_SLIDER_0   (GUI_ID_USER + 0x07)

#define MIN_RANDOM_V  0           // Minimum velocity (x or y) to be used in generating random balls
#define MAX_RANDOM_V  80          // Maximum velocity (x or y) to be used in generating random balls
#define MIN_RANDOM_R  8           // Minimum radius to be used in generating random balls
#define MAX_RANDOM_R  20          // Maximum radius to be used in generating random balls
#define M_PI          3.1415926f
#define M_TO_A_RATIO  0.1f        // Ratio of mass to area used in generating random balls

#define TIME_SLICE        20
#define NUM_BALLS         24
#define GRAVITY           80
#define PERIOD_SENTINEL 1000

#define TIMER_ID_ADVANCE  123
#define TIMER_ID_SENTINEL 456

#define APP_RESTART       (WM_USER + 0)

#if GUI_VERSION < 54400
  #define WM_USER_DATA (WM_USER + 0)
#endif
#ifdef WIN32
#include <malloc.h>
#endif

/*********************************************************************
*
*       Static (const) data
*
**********************************************************************
*/
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
static void (* _pb_BALLSSIM_AdvanceSim)(BALLSIM * pBallsim, const float dt);

#ifdef WIN32
static size_t _AllocatedBytes;
#endif

static WM_HWIN _hWin;

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
  int * pOnOff;
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

#include "APP_BouncingBalls_AdvanceSim.h"

/*********************************************************************
*
*       Static code: Ballsim Window
*
**********************************************************************
*/
/*********************************************************************
*
*       _UpdateDisplay
*/
static void _UpdateDisplay(WM_HWIN hWin, BALLSIM * pBallsim, int Fps) {
  BALL * pBalli;
  U32    Color;

  //
  // Draw background
  //
  if (pBallsim->pConfig->pfDrawBk) {
    pBallsim->pConfig->pfDrawBk(hWin, pBallsim->pConfig);
  } else {
    GUI_SetBkColor(pBallsim->pConfig->ColorBk);
    GUI_Clear();
  }
  //
  // Draw all balls
  //
  pBalli = pBallsim->pFirstBall;
  while (pBalli) {
    if (pBallsim->pConfig->pfDrawBall) {
      pBallsim->pConfig->pfDrawBall(hWin, pBallsim->pConfig, pBalli->Index, pBalli->p.x, pBalli->p.y, pBalli->r);
    } else {
      Color = GUI_MAKE_COLOR(pBalli->Index);
      GUI_SetColor(Color);
      GUI_FillCircle(pBalli->p.x, pBalli->p.y, pBalli->r);
    }
    pBalli = pBalli->pNext;
  }
  //
  // FPS
  //
  GUI_SetFont(&GUI_Font16_F);
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_GotoXY(5, 5);
  GUI_DispString("FPS: ");
  GUI_DispDecMin(Fps);
}

/*********************************************************************
*
*       _AddRandomBalls
*/
static void _AddRandomBalls(BALLSIM * pBallsim) {
  unsigned i, PosOccupied, Cnt;
  float d, dx, dy;
  BALL * pBall;
  BALL * pBalli;
  BALLSIM_CONFIG * pConfig;

  pConfig = pBallsim->pConfig;
  for (i = 0; i < pConfig->NumBalls; i++) {
    pBall = _BALL_Create();
    if (pBallsim->pConfig->HasInitialVelocity) {
      pBall->v.x = _GetRandomNumber(pConfig->vMin, pConfig->vMax);
      pBall->v.y = _GetRandomNumber(pConfig->vMin, pConfig->vMax);
    }
    pBall->Index = (U32)_GetRandomNumber(0, pConfig->Range);
    if (pConfig->pRadius) {
      pBall->r = *(pConfig->pRadius + pBall->Index);
    } else {
      pBall->r = _GetRandomNumber(pConfig->rMin, pConfig->rMax);
    }
    pBall->m = M_TO_A_RATIO * M_PI * pBall->r * pBall->r;
    //
    // Generate legal position
    //
    Cnt = 0;
    do {
      pBall->p.x = _GetRandomNumber((float)pBall->r, (float)(pConfig->xSize) - pBall->r);
      pBall->p.y = _GetRandomNumber((float)pBall->r, (float)(pConfig->ySize) - pBall->r);
      PosOccupied = 0;
      pBalli = pBallsim->pFirstBall;
      while (pBalli) {
        dx = pBalli->p.x - pBall->p.x;
        dy = pBalli->p.y - pBall->p.y;
        d = sqrt(dx * dx + dy * dy);
        if (d < pBalli->r + pBall->r) {
          PosOccupied = 1;
          break;
        }
        pBalli = pBalli->pNext;
      }
    } while (PosOccupied && (Cnt++ < 20));
    //
    // Add ball to array
    //
    if (PosOccupied == 0) {
      _BALLSSIM_AddBall(pBallsim, pBall);
    }
  }
}

/*********************************************************************
*
*       _CreateBallsim
*/
static BALLSIM * _CreateBallsim(WM_HWIN hWin) {
  BALLSIM_CONFIG * pConfig;
  BALLSIM        * pBallsim;
  WALLS          * pWalls;

  WM_GetUserData(hWin, &pConfig, sizeof(void *));
  pBallsim = _BALLSIM_Create();
  pBallsim->pConfig = pConfig;
  pWalls = _WALLS_Create(0.f, 0.f, (float)pConfig->xSize, (float)pConfig->ySize);
  _BALLSSIM_MoveWalls(pBallsim, pWalls);
  _AddRandomBalls(pBallsim);
  _Free(pWalls);
  return pBallsim;
}

/*********************************************************************
*
*       _CheckBalls
*/
static int _CheckBalls(BALLSIM * pBallsim) {
  BALL * pBalli;

  //
  // Check all balls
  //
  pBalli = pBallsim->pFirstBall;
  while (pBalli) {
    if (pBalli->p.x < pBalli->r) {
      return 1;
    }
    if (pBalli->p.y < pBalli->r) {
      return 1;
    }
    if (pBalli->p.x > (pBallsim->pConfig->xSize - pBalli->r)) {
      return 1;
    }
    if (pBalli->p.y > (pBallsim->pConfig->ySize - pBalli->r)) {
      return 1;
    }
    pBalli = pBalli->pNext;
  }
  return 0;
}

/*********************************************************************
*
*       _OnIdle
*/
static void _OnIdle(int Period) {
  GUI_USE_PARA(Period);
  WM_InvalidateWindow(_hWin);
}

/*********************************************************************
*
*       _cbBallsim
*/
static void _cbBallsim(WM_MESSAGE * pMsg) {
  static BALLSIM            * pBallsim;
  static GUI_TIMER_TIME       Time;
  GUI_TIMER_TIME              tNow;
  static WM_HTIMER            hTimer;
  static WM_HTIMER            hTimerSentinel;

  static int Fps;
  static int FpsShow;
  static int Cnt;
  static GUI_TIMER_TIME tLastFrame;
  static GUI_TIMER_TIME tNextFPS;
  GUI_TIMER_TIME tUsed;
  int Id;
  WM_HWIN hParent;
  
  switch (pMsg->MsgId) {
  case WM_DELETE:
    WM_DeleteTimer(hTimer);
    WM_DeleteTimer(hTimerSentinel);
    LCDCONF_EnableDave2D();
    _BALLSSIM_Delete(pBallsim);
    GUI_SetWaitEventTimedFunc(NULL);
    break;
  case WM_USER_DATA:
    pBallsim   = _CreateBallsim(pMsg->hWin);
    hTimer     = WM_CreateTimer(pMsg->hWin, TIMER_ID_ADVANCE, TIME_SLICE, 0);
    Time       = GUI_GetTime();
    tNextFPS   = Time;
    tLastFrame = 0;
    Fps        = 0;
    Cnt        = 1;
    //
    // Additional sentinel timer
    //
    hTimerSentinel = WM_CreateTimer(pMsg->hWin, TIMER_ID_SENTINEL, PERIOD_SENTINEL, 0);
    //
    // Use idle management to achieve a maximum of FPS
    //
    GUI_SetWaitEventTimedFunc(_OnIdle);
    _hWin = pMsg->hWin;
    break;
  case WM_PRE_PAINT:
    tNow = GUI_GetTime();
    tUsed = tNow - tLastFrame;
    if (tUsed) {
      if (tNow >= tNextFPS) {
        FpsShow = Fps / Cnt;
        Fps = 1000 / tUsed;
        Cnt = 1;
        tNextFPS += 500;
      } else {
        Fps += 1000 / tUsed;
        Cnt++;
      }
    }
    tLastFrame = tNow;
    break;
  case WM_PAINT:
    _UpdateDisplay(pMsg->hWin, pBallsim, FpsShow);
    break;
  case WM_TIMER:
    Id = WM_GetTimerId(pMsg->Data.v);
    switch (Id) {
    case TIMER_ID_ADVANCE:
      WM_RestartTimer(pMsg->Data.v, pBallsim->pConfig->TimeSlice);
      tNow = GUI_GetTime();
      _pb_BALLSSIM_AdvanceSim(pBallsim, (float)(tNow - Time) / 1000);
      Time = tNow;
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case TIMER_ID_SENTINEL:
      WM_RestartTimer(pMsg->Data.v, 0);
      //
      // Restart demo if any ball is out of range
      //
      if (_CheckBalls(pBallsim)) {
        hParent = WM_GetParent(pMsg->hWin);
        WM_SendMessageNoPara(hParent, APP_RESTART);
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _CreateBallsimWindow
*/
static WM_HWIN _CreateBallsimWindow(BALLSIM_CONFIG * pConfig, WM_HWIN hParent) {
  WM_HWIN hWin;
  
  hWin = WM_CreateWindowAsChild(pConfig->xPos, pConfig->yPos, pConfig->xSize, pConfig->ySize, hParent, WM_CF_SHOW, _cbBallsim, sizeof(void *));
  WM_SetUserData(hWin, &pConfig, sizeof(void *));
#if GUI_VERSION < 54400
  WM_SendMessageNoPara(hWin, WM_USER_DATA);
#endif
  return hWin;
}
/*********************************************************************
*
*       _ManagePointers
*/
#if (TRIGGER_DAVE == 0)
static void _ManagePointers(int OnOff) {
  switch (OnOff) {
  case 0:
    _pb_BALLSSIM_AdvanceSim = BALLSSIM_AdvanceSim_NOFPU;
    break;
  case 1:
    _pb_BALLSSIM_AdvanceSim = BALLSSIM_AdvanceSim_FPU;
    _pb_BALLSSIM_AdvanceSim = _BALLSSIM_AdvanceSim;
    break;
  }
}
#endif

/*********************************************************************
*
*       _DrawBall
*/
static void _DrawBall(WM_HWIN hWin, void * pVoid, U32 Index, float x, float y, float r) {
  GUI_SetColor(_aColors[Index]);
  GUI_AA_FillCircle((int)x, (int)y, (int)r - 1);
  GUI_SetPenSize(4);
  GUI_SetColor(COLOR_SHAPE);
  GUI_AA_DrawCircle((int)x, (int)y, (int)r - PENSIZE / 2);
  GUI_SetPenSize(1);
}

/*********************************************************************
*
*       _DrawBk
*/
static void _DrawBk(WM_HWIN hWin, void * pVoid) {
  GUI_SetBkColor(COLOR_BK);
  GUI_Clear();
}

/*********************************************************************
*
*       _cbWin
*/
static void _cbWin(WM_MESSAGE * pMsg) {
  const GUI_BITMAP * pBm;
  int xSizeWindow, ySizeWindow;
  static int OnOff_FPU;
  static int * pOnOff_FPU;
  static int OnOff_GPU;
  static int * pOnOff_GPU;
  WM_HWIN hItem;
  int Id, NCode;
  static BALLSIM_CONFIG Config;

  switch (pMsg->MsgId) {
  case APP_RESTART:
    //
    // Restart demo in case of any ball is out of range
    //
    WM_DeleteWindow(pMsg->hWin);
    BouncingBalls();
    break;
  case WM_CREATE:
    OnOff_FPU = 1;
    OnOff_GPU = 1;
#if TRIGGER_DAVE
#ifndef WIN32
    OnOff_FPU = LCDCONF_GetDaveActive();
#endif
#else
    _ManagePointers(OnOff_FPU);
#endif
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    //
    // Create bouncing window
    //
    pBm = BM_BUTTON_0_0;
    //
    // Configuration of ball simulation
    //
    Config.xPos             = BORDER + PENSIZE;
    Config.yPos             = BORDER + PENSIZE + pBm->YSize + BORDER;
    Config.xSize            = xSizeWindow - BORDER - pBm->XSize - 2 * BORDER - 2 * PENSIZE;
    Config.ySize            = ySizeWindow - BORDER - pBm->YSize - 2 * BORDER - 2 * PENSIZE;
    Config.Range            = 0xE0E0E0;
    Config.NumBalls         = NUM_BALLS;
    Config.TimeSlice        = TIME_SLICE;
    Config.vMin             = MIN_RANDOM_V;
    Config.vMax             = MAX_RANDOM_V;
    Config.rMin             = MIN_RANDOM_R;
    Config.rMax             = MAX_RANDOM_R;
    Config.Gravity          = GRAVITY;
    Config.Range            = GUI_COUNTOF(_aColors);
    Config.HasGroundGravity = 1;
    Config.pfDrawBk         = _DrawBk;
    Config.pfDrawBall       = _DrawBall;
    _CreateBallsimWindow(&Config, pMsg->hWin);
    //
    // Create On/Off button (FPU)
    //
    pBm = BM_BUTTON_0_0;
    hItem = BUTTON_CreateUser(xSizeWindow - pBm->XSize - BORDER, ySizeWindow - pBm->YSize - BORDER, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ONOFF_FPU, sizeof(pOnOff_FPU));
    pOnOff_FPU = &OnOff_FPU;
    BUTTON_SetUserData(hItem, &pOnOff_FPU, sizeof(pOnOff_FPU));
    WM_SetCallback(hItem, _cbButtonOnOff);
    //
    // Create On/Off button (GPU)
    //
    pBm = BM_BUTTON_0_0;
    hItem = BUTTON_CreateUser(xSizeWindow - pBm->XSize - BORDER, ySizeWindow - pBm->YSize * 2 - BORDER * 2 - 32, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ONOFF_GPU, sizeof(pOnOff_GPU));
    pOnOff_GPU = &OnOff_GPU;
    BUTTON_SetUserData(hItem, &pOnOff_GPU, sizeof(pOnOff_GPU));
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
    case ID_BUTTON_ONOFF_FPU:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        OnOff_FPU ^= 1;
#if TRIGGER_DAVE
        if (OnOff_FPU) {
          LCDCONF_EnableDave2D();
        } else {
          LCDCONF_DisableDave2D();
        }
#else
        _ManagePointers(OnOff_FPU);
#endif
        break;
      }
      break;
    case ID_BUTTON_ONOFF_GPU:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        OnOff_GPU ^= 1;
        if (OnOff_GPU) {
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
    GUI_DispStringAt("Dave2D", xSizeWindow - BORDER - pBm->XSize / 2, ySizeWindow - BORDER - pBm->YSize - 10);
#else
    GUI_DispStringAt("FPU", xSizeWindow - BORDER - pBm->XSize / 2, ySizeWindow - BORDER - pBm->YSize - 10);
#endif
    GUI_SetTextAlign(GUI_TA_CENTER | GUI_TA_BOTTOM);
    GUI_DispStringAt("GPU", xSizeWindow - BORDER - pBm->XSize / 2, ySizeWindow - BORDER * 2 - pBm->YSize * 2 - 10 - 32);
    GUI_SetColor(COLOR_SHAPE);
    GUI_FillRect(Config.xPos - PENSIZE, Config.yPos - PENSIZE, Config.xPos + Config.xSize + PENSIZE - 1, Config.yPos - 1);
    GUI_FillRect(Config.xPos - PENSIZE, Config.yPos, Config.xPos - 1, Config.yPos + Config.ySize - 1);
    GUI_FillRect(Config.xPos + Config.xSize, Config.yPos, Config.xPos + Config.xSize + PENSIZE - 1, Config.yPos + Config.ySize - 1);
    GUI_FillRect(Config.xPos - PENSIZE, Config.yPos + Config.ySize, Config.xPos + Config.xSize + PENSIZE - 1, Config.yPos + Config.ySize + PENSIZE - 1);
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
*       _Free
*/
void _Free(void * p) {
#ifdef WIN32
  _AllocatedBytes -= _msize(p);
#endif
  free(p);
}

/*********************************************************************
*
*       _Calloc
*/
void * _Calloc(size_t Num, size_t Size) {
  void * p;
  p = calloc(Num, Size);
#ifdef WIN32
  _AllocatedBytes += _msize(p);
#endif
  return p;
}

/*********************************************************************
*
*       BouncingBalls
*/
void BouncingBalls(void) {
  WM_CreateWindow(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_CF_SHOW, _cbWin, 0);
}

/*************************** End of file ****************************/

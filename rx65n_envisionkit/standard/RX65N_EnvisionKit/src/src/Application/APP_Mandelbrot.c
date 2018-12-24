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
File        : APP_Mandelbrot.c
Purpose     : Mandelbrot demo
----------------------------------------------------------------------
*/

#include "APP_Mandelbrot_Private.h"

/*********************************************************************
*
*       Static (const) data
*
**********************************************************************
*/
/*********************************************************************
*
*       _apText
*/
static const char * _apText[] = {
  "",
  "Calculate",
  "Back",
  "Use touch to\nselect range",
  "Max depth reached",
};

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static void (* _pfDrawMandelbrot)(WM_MESSAGE * pMsg, MANDEL_DATA * pData, int x0, int y0, int x1, int y1) = DrawMandelbrot_FPU;

static MANDEL_RANGE aRange[MAX_DEPTH];

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
  switch (pMsg->MsgId) {
  case WM_PAINT: {
#if (USE_MULTIBUFFERING == 0)  // The difference is not using rounded buttons...
      int xSizeWindow, ySizeWindow;

      xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
      ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
      GUI_SetColor(COLOR_SHAPE);
      GUI_FillRect(0, 0, xSizeWindow - 1, ySizeWindow - 1);
      GUI_SetColor(BUTTON_IsPressed(pMsg->hWin) ? COLOR_BUTTON_1 : COLOR_BUTTON_0);
      GUI_FillRect(PENSIZE, PENSIZE, xSizeWindow - 1 - PENSIZE, ySizeWindow - 1 - PENSIZE);
      GUI_SetColor(GUI_BLACK);
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(FONT_SMALL);
      GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
      GUI_DispStringAt("HELP", xSizeWindow / 2, ySizeWindow / 2);
#else
      const GUI_BITMAP * pBm;

      pBm = BUTTON_IsPressed(pMsg->hWin) ? BM_HELP_1 : BM_HELP_0;
      GUI_DrawBitmap(pBm, 0, 0);
#endif
    }
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
*       _ButtonSkin
*/
static int _ButtonSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    GUI_SetColor(COLOR_SHAPE);
    GUI_FillRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1);
    GUI_SetColor(BUTTON_IsPressed(pDrawItemInfo->hWin) ? COLOR_BUTTON_1 : COLOR_BUTTON_0);
    GUI_FillRect(pDrawItemInfo->x0 + PENSIZE, pDrawItemInfo->y0 + PENSIZE, pDrawItemInfo->x1 - PENSIZE, pDrawItemInfo->y1 - PENSIZE);
    return 0;
  default:
    return BUTTON_DrawSkinFlex(pDrawItemInfo);
  }
}

/*********************************************************************
*
*       _DrawSelectionRect
*/
static void _DrawSelectionRect(GUI_RECT * pRect, MANDEL_DATA * pData) {
  int temp;

  pData->x0 = pRect->x0;
  pData->y0 = pRect->y0;
  pData->x1 = pRect->x1;
  pData->y1 = pRect->y1;
  if (pData->x1 < pData->x0) {
    temp = pData->x1;
    pData->x1 = pData->x0;
    pData->x0 = temp;
  }
  if (pData->y1 < pData->y0) {
    temp = pData->y1;
    pData->y1 = pData->y0;
    pData->y0 = temp;
  }
  GUI_SetDrawMode(GUI_DM_XOR);
  GUI_DrawRect(pData->x0, pData->y0, pData->x1, pData->y1);
  pData->IsVis ^= 1;
}

/*********************************************************************
*
*       _SetupButton
*/
static void _SetupButton(WM_HWIN hWin, int Id, int Enable, int IndexText) {
  WM_HWIN hItem;
  const char * pText;

  hItem = WM_GetDialogItem(hWin, Id);
  if (Enable) {
    WM_EnableWindow(hItem);
  } else {
    WM_DisableWindow(hItem);
  }
  pText = (IndexText >= 0) ? _apText[IndexText] : NULL;
  if (pText) {
    BUTTON_SetText(hItem, pText);
  }
  if ((Id == ID_BUTTON_BACK) && (IndexText == TEXT_EXIT)) {
    WM_HideWindow(hItem);
  } else {
    WM_ShowWindow(hItem);
  }
}

/*********************************************************************
*
*       _CreatePalette
*/
static void _CreatePalette(MANDEL_DATA * pData) {
  static const GUI_COLOR aGradient[] = {
    GUI_MAKE_COLOR(0xA02020), GUI_MAKE_COLOR(0xFF20FF), GUI_MAKE_COLOR(0x20FFFF), GUI_MAKE_COLOR(0x2020FF)
  };
  static U32 aColor[256];
  int i, j, BitsPerPixel;
  GUI_COLOR * p;

  BitsPerPixel = LCD_GetBitsPerPixel();
  pData->NumColors = (BitsPerPixel <= 8) ? 1 << BitsPerPixel : 0;
  if ((pData->NumColors <= 256) && (pData->NumColors != 0)) {
    //
    // Use hardware palette
    //
    pData->pColor = aColor;
    for (i = 0; i < pData->NumColors; i++) {
      *(pData->pColor + i) = LCD_Index2Color(i);
    }
  } else {
    //
    // Calculate palette with above defined colors
    //
    pData->NumColors = 0x40;
    pData->pColor = p = aColor;
    for (j = 0; j < 4; j++) {
      for (i = 0x00; i < (pData->NumColors >> 2); i++) {
        *p++ = GUI__pfMixColors(aGradient[(j + 1) & 3], aGradient[j], ((i + 1) * (0x400 / pData->NumColors)) - 1);
      }
    }
  }
}

/*********************************************************************
*
*       _CalcRange
*/
static void _CalcRange(MANDEL_DATA * pData) {
  double xMin, xMax, yMin, yMax, mx, my, dx, dy;

  //
  // Calculate new values
  //
  xMin = pData->Range.xMin + ((pData->Range.xMax - pData->Range.xMin) * pData->x0) / pData->xSize;
  xMax = pData->Range.xMin + ((pData->Range.xMax - pData->Range.xMin) * pData->x1) / pData->xSize;
  yMin = pData->Range.yMin + ((pData->Range.yMax - pData->Range.yMin) * pData->y0) / pData->ySize;
  yMax = pData->Range.yMin + ((pData->Range.yMax - pData->Range.yMin) * pData->y1) / pData->ySize;
  mx = (xMin + xMax) / 2;
  my = (yMin + yMax) / 2;
  dx = xMax - xMin;
  dy = yMax - yMin;
  //
  // Keep aspect ratio
  //
  if (dx > dy) {
    dy = (dx * pData->ySize) / pData->xSize;
    yMin = my - dy / 2;
    yMax = my + dy / 2;
  } else {
    dx = (dy * pData->xSize) / pData->ySize;
    xMin = mx - dx / 2;
    xMax = mx + dx / 2;
  }
  //
  // Use new values
  //
  pData->Range.xMin = xMin;
  pData->Range.xMax = xMax;
  pData->Range.yMin = yMin;
  pData->Range.yMax = yMax;
}

/*********************************************************************
*
*       _PushRange
*/
static int _PushRange(MANDEL_DATA * pData, MANDEL_RANGE * pRange) {
  if (pData->Depth == (MAX_DEPTH - 1)) {
    return 1;
  }
  *(pRange + pData->Depth) = pData->Range;
  pData->Depth++;
  return 0;
}

/*********************************************************************
*
*       _PopRange
*/
static int _PopRange(MANDEL_DATA * pData, MANDEL_RANGE * pRange) {
  if (pData->Depth == 0) {
    return 1;
  }
  pData->Range = *(pRange + pData->Depth - 1);
  pData->Depth--;
  return 0;
}

/*********************************************************************
*
*       _cbMandelbrot
*/
static void _cbMandelbrot(WM_MESSAGE * pMsg) {
  static GUI_RECT Rect;
  MANDEL_DATA * pData;
  GUI_RECT RectWindow;
  WM_PID_STATE_CHANGED_INFO * pInfo;
  GUI_PID_STATE * pState;
#if SHOW_PERFORMANCE
  static GUI_TIMER_TIME t0;
  GUI_TIMER_TIME tUsed;
#endif

  switch (pMsg->MsgId) {
  case WM_PID_STATE_CHANGED:
    WM_GetUserData(pMsg->hWin, &pData, sizeof(pData));
    pInfo = (WM_PID_STATE_CHANGED_INFO *)pMsg->Data.p;
    if ((pInfo->State == 1) && (pInfo->StatePrev == 0)) {
      if (pData->IsVis) {
        _DrawSelectionRect(&Rect, pData);
      }
      pData->IsDown = 1;
      Rect.x0 = pInfo->x;
      Rect.y0 = pInfo->y;
    } else if ((pInfo->State == 0) && (pInfo->StatePrev == 1)) {
      pData->IsDown = 0;
    }
    break;
  case WM_TOUCH:
    WM_GetUserData(pMsg->hWin, &pData, sizeof(pData));
    if (pData->IsDown) {
      WM_SelectWindow(pMsg->hWin);
      if (pData->IsVis) {
        _DrawSelectionRect(&Rect, pData);
      }
      pState = (GUI_PID_STATE *)pMsg->Data.p;
      if (pState) {
        Rect.x1 = pState->x;
        Rect.y1 = pState->y;
        _DrawSelectionRect(&Rect, pData);
      }
    }
    else {
      if (_PushRange(pData, aRange) == 0) {
        _CalcRange(pData);
        WM_InvalidateWindow(pData->hWin);
        pData->IsDown = pData->IsVis = 0;
        if (pData->Depth == MAX_DEPTH - 1) {
          _SetupButton(WM_GetParent(WM_GetParent(pMsg->hWin)), ID_BUTTON_BACK, 1, TEXT_MAX);
        } else {
          _SetupButton(WM_GetParent(WM_GetParent(pMsg->hWin)), ID_BUTTON_BACK, 1, TEXT_BACK);
        }
      }
    }
    break;
#if SHOW_PERFORMANCE
  case WM_PRE_PAINT:
    t0 = GUI_GetTime();
    break;
  case WM_POST_PAINT:
    tUsed = GUI_GetTime() - t0;
    WM_SetUserClipRect(NULL);
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_TRANS);
    WM_SelectWindow(pMsg->hWin);
    GUI_DispDecAt(tUsed, 10, 10, 4);
    break;
#endif
  case WM_PAINT:
    WM_GetUserData(pMsg->hWin, &pData, sizeof(pData));
    if (pData) {
      WM_GetClientRect(&RectWindow);
      GUI_SetColor(GUI_BLACK);
      GUI_DrawRectEx(&RectWindow);
      GUI__ReduceRect(&RectWindow, &RectWindow, 1);
      WM_SetUserClipRect(&RectWindow);
      _pfDrawMandelbrot(pMsg, pData, RectWindow.x0, RectWindow.y0, RectWindow.x1, RectWindow.y1);
      WM_SetUserClipRect(NULL);
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _ManagePointers
*/
static void _ManagePointers(int OnOff) {
  switch (OnOff) {
  case 0:
    _pfDrawMandelbrot = DrawMandelbrot_NOFPU;
    break;
  case 1:
    _pfDrawMandelbrot = DrawMandelbrot_FPU;
    break;
  }
}

/*********************************************************************
*
*       _cbWin
*/
static void _cbWin(WM_MESSAGE * pMsg) {
  int Id, NCode;
  int xSizeWindow, ySizeWindow;
  static int BkIsClear;
  static int OnOff;
  static int * pOnOff;
  static MANDEL_DATA Data;
  MANDEL_DATA * pData = &Data;
  WM_HWIN hItem;
  const GUI_BITMAP * pBm;

  switch (pMsg->MsgId) {
  case WM_DELETE:
    pData->Depth = 0;
    BkIsClear = 0;
    break;
  case WM_CREATE:
    //
    // Enable FPU per default
    //
    OnOff = 1;
    _ManagePointers(OnOff);
    //
    // Get size of this window
    //
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    //
    // Create home button
    //
    pBm = BM_HOME;
    hItem = BUTTON_CreateUser(BORDER, BORDER, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_HOME, 0);
    WM_SetCallback(hItem, _cbButtonHome);
    //
    // Create back button
    //
    hItem = BUTTON_CreateUser(FRAME_X, (ySizeWindow / 2) - YSIZE_BUTTON, XSIZE_BUTTON, YSIZE_BUTTON, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_BACK, 0);
    BUTTON_SetSkin(hItem, _ButtonSkin);
    _SetupButton(pMsg->hWin, ID_BUTTON_BACK, 1, TEXT_SELECT);
    //
    // Set up initial Mandelbrot range
    //
    pData->MaxIter = MAX_ITER;
    pData->xSize = xSizeWindow - (XSIZE_BUTTON + 3 * FRAME_X);
    pData->ySize = ySizeWindow - 32;
    pData->Range.xMin = -2.3f;
    pData->Range.xMax = +1.3f;
    pData->Range.yMin = -((pData->Range.xMax - pData->Range.xMin) * pData->ySize * 0.5f) / pData->xSize;
    pData->Range.yMax = +((pData->Range.xMax - pData->Range.xMin) * pData->ySize * 0.5f) / pData->xSize;
    //
    // Create Mandelbrot window
    //
    pData->hWin = WM_CreateWindowAsChild(xSizeWindow - pData->xSize - FRAME_X, (ySizeWindow - pData->ySize) / 2, pData->xSize, pData->ySize, pMsg->hWin, 0, _cbMandelbrot, sizeof(pData));
    WM_SetUserData(pData->hWin, &pData, sizeof(pData));
    //
    // Initialize palette to be used
    //
    _CreatePalette(pData);
    //
    // Create On/Off button
    //
    pBm = BM_BUTTON_0_0;
    hItem = BUTTON_CreateUser(FRAME_X, ySizeWindow - pBm->YSize - BORDER, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ONOFF, sizeof(pOnOff));
    pOnOff = &OnOff;
    BUTTON_SetUserData(hItem, &pOnOff, sizeof(pOnOff));
    WM_SetCallback(hItem, _cbButtonOnOff);
    //
    // Create help button
    //
    pBm = BM_HELP_0;
    hItem = BUTTON_CreateUser(xSizeWindow - pBm->XSize - BORDER, BORDER, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_HELP, 0);
#if (USE_MULTIBUFFERING == 0)
    WM_ClrHasTrans(hItem);
#endif
    WM_SetCallback(hItem, _cbButtonHelp);
    break;
  case WM_PAINT:
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    GUI_SetBkColor(COLOR_BK);
    GUI_Clear();
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(FONT_MEDIUM);
    GUI_SetTextAlign(GUI_TA_CENTER | GUI_TA_BOTTOM);
    pBm = BM_BUTTON_0_0;
    GUI_DispStringAt("FPU", FRAME_X + pBm->XSize / 2, ySizeWindow - BORDER - pBm->YSize - 10);
    //
    // Make sure to show the window after the background is properly cleared
    //
    if (BkIsClear == 0) {
      WM_ShowWindow(pData->hWin);
      BkIsClear = 1;
    }
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_ONOFF:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        OnOff ^= 1;
        WM_Exec();
        _ManagePointers(OnOff);
        WM_InvalidateWindow(pData->hWin);
        break;
      }
      break;
    case ID_BUTTON_HOME: // 'Home'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_DeleteWindow(pMsg->hWin);
#if (USE_MULTIBUFFERING == 0)
        WM_MULTIBUF_Enable(1);
#endif
        StartScreen();
        break;
      }
      break;
    case ID_BUTTON_BACK: // 'Back'
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_Exec();
        if (_PopRange(pData, aRange) == 0) {
          pData->IsDown = pData->IsVis = 0;
          WM_InvalidateWindow(pData->hWin);
          if (pData->Depth == 0) {
            _SetupButton(WM_GetParent(pMsg->hWin), ID_BUTTON_BACK, 1, TEXT_SELECT);
          } else {
            _SetupButton(WM_GetParent(pMsg->hWin), ID_BUTTON_BACK, 1, TEXT_BACK);
          }
        }
        break;
      }
      break;
    case ID_BUTTON_HELP:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_DeleteWindow(pMsg->hWin);
#if (USE_MULTIBUFFERING == 0)
        WM_MULTIBUF_Enable(1);
#endif
        HelpDialog(DEMO_2, Mandelbrot);
        break;
      }
      break;
    }
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
*       Mandelbrot
*/
void Mandelbrot(void) {
  WM_CreateWindow(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_CF_SHOW, _cbWin, 0);
#if (USE_MULTIBUFFERING == 0)
  WM_MULTIBUF_Enable(0);
#endif
}

/*************************** End of file ****************************/

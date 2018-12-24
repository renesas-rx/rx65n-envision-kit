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
File        : APP_StartScreen.c
Purpose     : Start screen of envision kit demo
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
#define ID_BUTTON_DRAWINGENGINE (GUI_ID_USER + 0)
#define ID_BUTTON_BANKSWAP      (GUI_ID_USER + 1)
#define ID_BUTTON_BOUNCINGBALLS (GUI_ID_USER + 2)
#define ID_BUTTON_MANDELBROT    (GUI_ID_USER + 3)

#define FONT_SMALL    &GUI_Font24_AA4
#define FONT_MEDIUM   &GUI_Font32_AA4
#define FONT_LARGE    &GUI_Font48B_AA4

#define BM_RX         &bmRenesasRX_155x55

#define COLOR_BUTTON_0 GUI_MAKE_COLOR(0xFCD7B9)
#define COLOR_BUTTON_1 GUI_MAKE_COLOR(0xA97144)

#define BORDER 20  // Distance from buttons to border
#define SPACE  10  // Distance from button to button

#define XSIZE_BUTTON 120

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _ButtonSkin
*/
static int _ButtonSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  GUI_COLOR Color;

  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    Color = BUTTON_IsPressed(pDrawItemInfo->hWin) ? COLOR_BUTTON_1 : COLOR_BUTTON_0;
    GUI_SetColor(GUI_BLACK);
    GUI_DrawRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1);
    GUI_SetColor(Color);
    GUI_FillRect(pDrawItemInfo->x0 + 1, pDrawItemInfo->y0 + 1, pDrawItemInfo->x1 - 1, pDrawItemInfo->y1 - 1);
    break;
  default:
    BUTTON_SKIN_FLEX(pDrawItemInfo);
    break;
  }
  return 0;
}

/*********************************************************************
*
*       _cbWin
*/
static void _cbWin(WM_MESSAGE * pMsg) {
  const GUI_BITMAP * pBm;
  WM_HWIN hItem;
  int xSizeWindow, ySizeWindow, yPos;
  int Id, NCode;

  switch (pMsg->MsgId) {
  case WM_CREATE:
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    //
    // Create DrawingEngine button
    //
    hItem = BUTTON_CreateUser(BORDER, 
                              ySizeWindow / 2, 
                              XSIZE_BUTTON, 
                              ySizeWindow / 2 - BORDER, 
                              pMsg->hWin, WM_CF_SHOW, 0, 
                              ID_BUTTON_DRAWINGENGINE, 0);
    BUTTON_SetText(hItem, "2D\nDrawing\nEngine");
    BUTTON_SetFont(hItem, FONT_SMALL);
    BUTTON_SetSkin(hItem, _ButtonSkin);
    //
    // Create BankSwap button
    //
    hItem = BUTTON_CreateUser(xSizeWindow - BORDER - XSIZE_BUTTON - 1, 
                              ySizeWindow / 2, 
                              XSIZE_BUTTON, 
                              ySizeWindow / 2 - BORDER, 
                              pMsg->hWin, WM_CF_SHOW, 0, 
                              ID_BUTTON_BANKSWAP, 0);
    BUTTON_SetText(hItem, "Bank\nSwap");
    BUTTON_SetFont(hItem, FONT_SMALL);
    BUTTON_SetSkin(hItem, _ButtonSkin);
    //
    // Create BouncingBalls button
    //
    hItem = BUTTON_CreateUser(BORDER + XSIZE_BUTTON + SPACE, 
                              ySizeWindow / 2, 
                              xSizeWindow - 2 * (BORDER + XSIZE_BUTTON + SPACE), 
                              (ySizeWindow / 2 - BORDER - SPACE) / 2, 
                              pMsg->hWin, WM_CF_SHOW, 0, 
                              ID_BUTTON_BOUNCINGBALLS, 0);
    BUTTON_SetText(hItem, "Bouncing Balls");
    BUTTON_SetFont(hItem, FONT_SMALL);
    BUTTON_SetSkin(hItem, _ButtonSkin);
    //
    // Create Mandelbrot button
    //
    hItem = BUTTON_CreateUser(BORDER + XSIZE_BUTTON + SPACE, 
                              ySizeWindow / 2 + (ySizeWindow / 2 - BORDER - SPACE) / 2 + SPACE, 
                              xSizeWindow - 2 * (BORDER + XSIZE_BUTTON + SPACE), 
                              (ySizeWindow / 2 - BORDER - SPACE) / 2, 
                              pMsg->hWin, WM_CF_SHOW, 0, 
                              ID_BUTTON_MANDELBROT, 0);
    BUTTON_SetText(hItem, "Mandelbrot");
    BUTTON_SetFont(hItem, FONT_SMALL);
    BUTTON_SetSkin(hItem, _ButtonSkin);
    break;
  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch (Id) {
    case ID_BUTTON_DRAWINGENGINE:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_DeleteWindow(pMsg->hWin);
        DrawingEngine();
        break;
      }
      break;
    case ID_BUTTON_MANDELBROT:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_DeleteWindow(pMsg->hWin);
        Mandelbrot();
        break;
      }
      break;
    case ID_BUTTON_BOUNCINGBALLS:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_DeleteWindow(pMsg->hWin);
        BouncingBalls();
        break;
      }
      break;
    case ID_BUTTON_BANKSWAP:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_DeleteWindow(pMsg->hWin);
        BankSwap();
        break;
      }
      break;
    }
    break;
  case WM_PAINT:
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    ySizeWindow = WM_GetWindowSizeY(pMsg->hWin);
    pBm = BM_RX;
    GUI_SetBkColor(COLOR_BK);
    GUI_Clear();
    GUI_SetFont(FONT_SMALL);
    GUI_SetColor(GUI_BLACK);
    GUI_DispStringAt("RX651/RX65N", BORDER, BORDER);
    GUI_SetTextAlign(GUI_TA_RIGHT);
#ifdef SWAP_VERSION
    GUI_SetColor(COLOR_GREEN);
    GUI_DispStringAt("Bank Swap\n", xSizeWindow - pBm->XSize - 2 * BORDER, BORDER);
#else
//    GUI_SetColor(GUI_RED);
//    GUI_DispStringAt("Rev. 1.1\n", xSizeWindow - pBm->XSize - 2 * BORDER, BORDER);
#endif
    yPos = GUI_GetDispPosY();
    GUI_SetFont(FONT_LARGE);
    GUI_SetColor(GUI_BLACK);
    GUI_SetTextAlign(GUI_TA_LEFT);
    GUI_DispStringAt("Envision Kit\n", BORDER, yPos);
    yPos = GUI_GetDispPosY();
    GUI_SetFont(FONT_SMALL);
    GUI_DispStringHCenterAt("Choose your demo:", xSizeWindow / 2, yPos);
    GUI_DrawBitmap(pBm, xSizeWindow - pBm->XSize - BORDER, BORDER);
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
*       StartScreen
*/
void StartScreen(void) {
  WM_CreateWindow(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_CF_SHOW, _cbWin, 0);
}

/*************************** End of file ****************************/

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
File        : APP_HelpDialog.c
Purpose     : Help screens
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
#define ID_BUTTON_EXIT   (GUI_ID_USER + 1)

#define BORDER 20  // Distance from buttons to border

#define BM_HOME       &bmHome_40x40
#define BM_EXIT_0     &bmButtonExit_0_60x40
#define BM_EXIT_1     &bmButtonExit_1_60x40

#define FONT_BODY    &GUI_Font16_AA4
#define FONT_HBODY   &GUI_Font21_AA4
#define FONT_HEADER  &GUI_Font32_AA4

static int _DemoIndex;
static void (*_pfOrigin)(void);

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const char * _apHeaderSpecific[][4] = {
  {
    "Drawing Engine Help",
    "Enjoy the 2D Drawing Engine demo!",
  },
  {
    "Bouncing Balls Help",
    "Enjoy the bouncing balls!",
  },
  {
    "Mandelbrot Help",
    "Enjoy the Mandelbrot demo!",
  },
  {
    "Bank Swap Help",
    "Enjoy the Bank Swap demo!",
  },
};

static const char * _apSmallHeader[] = {
  "Info:",
  "Usage Guide:"
};

static const char * _apBody[][2] = {
  {
    "The 2D Drawing Engine demo shows the efficiency of the 2D graphic accelerator embedded in the RX65N MCU working with semi-transparent images and alpha blending. The clouds 'sprite' moves smoothly over a fixed image background with hidden area redrawn dynamically as it is uncovered. The RX Logo can be moved by your finger tip using the touch screen.",
    "Using the touch screen you can freely move the RX logo over the fixed background and animated clouds. You can turn off the 2D Drawing Engine by the provided button to show the performance difference without the embedded hardware accelerator. With the Home button you can go back to main screen."
  },
  {
    "The Bouncing Ball demo shows the superior calculation performance of the 32-bit RX65N MCU.\nSeveral balls enter the screen at random angles from the top left and fall and bounce under the influence of a simulated gravity model. The RXv2 Core and the integrated Floating Point Unit (FPU) make the calculations an easy task for the RX65N.",
    "With the Home button you can go back to main screen."
  },
  {
    "A Mandelbrot set image is generated by resolving an algebraic function for a series of complex numbers and is used as an example of calculation intensive activity for any CPU.\nThe RXv2 Core structure includes:\n-  Single Precision Floating Point Unit (FPU) adapts pipeline processing to boost throughput\n-  DSP functionality achieves single-cycle throughput on 32bit multiply-accumulate operation using two sets of wide-range accumulators\n-  Dual-issue five-stage integer pipeline incorporates enhanced DSP/FPU functionality\nAll of this make the Mandelbrot calculation in real time an easy task for the RX65N.",
    "Using the touch screen to plot opposite corners of a rectangular area, you can zoom in to the Mandelbrot set image and define the next set of fractal calculations in real time. With the Back button you will go back to the previous shown screen of the Mandelbrot image. With the Home button you can go back to main screen."
  },
  {
    "It is possible to update the firmware during execution of any of the included demos by inserting a USB memory stick containing suitable firmware image into the USB Host interface of the Envision Kit. To obtain a firmware image to be copied to your blank USB memory stick, please go to www.renesas.com/envision. Once the new firmware image has been flashed into the alternate embedded flash memory bank, you can quickly swap between the two firmware versions using the Bank Swap dialogue.",
    "Insert USB memory stick while pushing user button on the Envision kit during any demo execution to flash new firmware to the RX. In the Bank Swap screen you can swap between the two firmware versions by pressing the Swap button. With the Home button you can go back to main screen."
  },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _DrawText
*/
static void _DrawText(GUI_RECT * pRect) {
  GUI_RECT RTemp;

  //
  // Fill a temporary rectangle since we need to adapt it while drawing
  //
  RTemp.x0 = pRect->x0;
  RTemp.y0 = pRect->y0;
  RTemp.x1 = pRect->x1;
  RTemp.y1 = pRect->y1;
  //
  // Display the main header with the biggest font and shrink the rectangle about the
  // y size of the font multiplied with the number of lines required
  //
  GUI_SetFont(FONT_HEADER);
  GUI_DispStringInRectWrap(_apHeaderSpecific[_DemoIndex][0], &RTemp, GUI_TA_HCENTER | GUI_TA_TOP, GUI_WRAPMODE_WORD);
  RTemp.y0 += GUI_WrapGetNumLines(_apHeaderSpecific[_DemoIndex][0], RTemp.x1 - RTemp.x0, GUI_WRAPMODE_WORD) * GUI_GetFontDistY();
  //
  // Display two other header with the medium font and shrink the rectangle again
  //
  GUI_SetFont(FONT_HBODY);
  GUI_DispStringInRectWrap(_apHeaderSpecific[_DemoIndex][1], &RTemp, GUI_TA_HCENTER | GUI_TA_TOP, GUI_WRAPMODE_WORD);
  RTemp.y0 += GUI_WrapGetNumLines(_apHeaderSpecific[_DemoIndex][1], RTemp.x1 - RTemp.x0, GUI_WRAPMODE_WORD) * GUI_GetFontDistY();
  GUI_DispStringInRectWrap(_apSmallHeader[0], &RTemp, GUI_TA_LEFT | GUI_TA_TOP, GUI_WRAPMODE_WORD);
  RTemp.y0 += GUI_WrapGetNumLines(_apSmallHeader[0], RTemp.x1 - RTemp.x0, GUI_WRAPMODE_WORD) * GUI_GetFontDistY();
  //
  // Display the first body text
  //
  GUI_SetFont(FONT_BODY);
  GUI_DispStringInRectWrap(_apBody[_DemoIndex][0], &RTemp, GUI_TA_LEFT | GUI_TA_TOP, GUI_WRAPMODE_WORD);
  RTemp.y0 += (GUI_WrapGetNumLines(_apBody[_DemoIndex][0], RTemp.x1 - RTemp.x0, GUI_WRAPMODE_WORD) + 1) * GUI_GetFontDistY();
  //
  // Another header
  //
  GUI_SetFont(FONT_HBODY);
  GUI_DispStringInRectWrap(_apSmallHeader[1], &RTemp, GUI_TA_LEFT | GUI_TA_TOP, GUI_WRAPMODE_WORD);
  RTemp.y0 += GUI_WrapGetNumLines(_apSmallHeader[1], RTemp.x1 - RTemp.x0, GUI_WRAPMODE_WORD) * GUI_GetFontDistY();
  //
  // Display the second body text
  //
  GUI_SetFont(FONT_BODY);
  GUI_DispStringInRectWrap(_apBody[_DemoIndex][1], &RTemp, GUI_TA_LEFT | GUI_TA_TOP, GUI_WRAPMODE_WORD);
  RTemp.y0 += (GUI_WrapGetNumLines(_apBody[_DemoIndex][1], RTemp.x1 - RTemp.x0, GUI_WRAPMODE_WORD) + 1) * GUI_GetFontDistY();
}

/*********************************************************************
*
*       _cbButtonExit
*/
static void _cbButtonExit(WM_MESSAGE * pMsg) {
  const GUI_BITMAP * pBm;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    pBm = BUTTON_IsPressed(pMsg->hWin) ? BM_EXIT_1 : BM_EXIT_0;
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
*       _cbWin
*/
static void _cbWin(WM_MESSAGE * pMsg) {
  const GUI_BITMAP * pBm;
  WM_HWIN            hButton;
  int                xSizeWindow;
  int                Id;
  int                NCode;
  static GUI_RECT    TRect;
  WM_MOTION_INFO   * pInfo;
  static int         yStart;
  static int         yEnd;

  switch (pMsg->MsgId) {
  case WM_MOTION:
    pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
    switch (pInfo->Cmd) {
    case WM_MOTION_INIT:
      //
      // Tell the motion module to move in y direction and that we manage it on our own
      //
      pInfo->Flags = WM_CF_MOTION_Y | WM_MOTION_MANAGE_BY_WINDOW;
      break;
    case WM_MOTION_MOVE:
      //
      // Move the text rectangle up or down
      //
      TRect.y0 += pInfo->dy;
      TRect.y1 += pInfo->dy;
      if (TRect.y0 < -yEnd) {
        //
        // If reach the end, make sure we stpo there
        //
        TRect.y0 = -yEnd;
        TRect.y1 = LCD_GetYSize();
        pInfo->StopMotion = 1;
      } else if (TRect.y0 > yStart) {
        //
        // If reach the top, make sure we stpo there
        //
        TRect.y0 = yStart;
        TRect.y1 = yEnd + LCD_GetYSize();
        pInfo->StopMotion = 1;
      }
      //
      // Tell the window to redraw
      //
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case WM_MOTION_GETPOS:
      pInfo->yPos = TRect.y0;
      break;
    }
    break;
  case WM_CREATE:
    xSizeWindow = WM_GetWindowSizeX(pMsg->hWin);
    //
    // Create home button
    //
    pBm = BM_HOME;
    hButton = BUTTON_CreateUser(BORDER, BORDER, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_HOME, 0);
    WM_SetCallback(hButton, _cbButtonHome);
    //
    // Create exit button
    //
    pBm = BM_EXIT_0;
    hButton = BUTTON_CreateUser(xSizeWindow - pBm->XSize - BORDER, BORDER, pBm->XSize, pBm->YSize, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT, 0);
    WM_SetCallback(hButton, _cbButtonExit);
    //
    // Calculate amount of space the help text requires and store it into a rectangle
    //
    // Make Sure it fits between the buttons to the left and right
    //
    pBm = BM_HOME;
    TRect.x0 = BORDER + pBm->XSize  + BORDER / 2;
    pBm = BM_EXIT_0;
    TRect.x1 = xSizeWindow - pBm->XSize - BORDER - BORDER / 2;
    //
    // Start with a small distance to the top
    //
    TRect.y0 = BORDER;
    //
    // Start calculating the end of the rectangle
    // 1 header with a big font
    // 3 header with a medium font
    // 2 body texts with a small font + 2 extra lines for spacing
    //
    GUI_SetFont(FONT_HEADER);
    TRect.y1 = GUI_WrapGetNumLines(_apHeaderSpecific[_DemoIndex][0], TRect.x1 - TRect.x0, GUI_WRAPMODE_WORD) * GUI_GetFontDistY();
    GUI_SetFont(FONT_HBODY);
    TRect.y1 += GUI_WrapGetNumLines(_apHeaderSpecific[_DemoIndex][1], TRect.x1 - TRect.x0, GUI_WRAPMODE_WORD) * GUI_GetFontDistY();
    TRect.y1 += GUI_WrapGetNumLines(_apSmallHeader[0], TRect.x1 - TRect.x0, GUI_WRAPMODE_WORD) * GUI_GetFontDistY();
    TRect.y1 += GUI_WrapGetNumLines(_apSmallHeader[1], TRect.x1 - TRect.x0, GUI_WRAPMODE_WORD) * GUI_GetFontDistY();
    GUI_SetFont(FONT_BODY);
    TRect.y1 += (GUI_WrapGetNumLines(_apBody[_DemoIndex][0], TRect.x1 - TRect.x0, GUI_WRAPMODE_WORD) + 1) * GUI_GetFontDistY();
    TRect.y1 += (GUI_WrapGetNumLines(_apBody[_DemoIndex][1], TRect.x1 - TRect.x0, GUI_WRAPMODE_WORD) + 1) * GUI_GetFontDistY();
    //
    // Remember the start and the end of the rectangle
    //
    yStart = TRect.y0;
    yEnd   = TRect.y1 - LCD_GetYSize();
    break;
  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch (Id) {
    case ID_BUTTON_EXIT:
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_DeleteWindow(pMsg->hWin);
        _pfOrigin();
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
    GUI_SetBkColor(COLOR_BK);
    GUI_Clear();
    GUI_SetColor(GUI_BLACK);
    //
    // Display the help text
    //
    _DrawText(&TRect);
    break;
  case WM_DELETE:
    //
    // Disable motion
    //
    WM_MOTION_Enable(0);
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
*       BankSwapHelp
*/
void HelpDialog(int DemoIndex, void (*pfOrigin)(void)) {
  //
  // Enable motion, gets disabled once the window gets deleted
  //
  _DemoIndex = DemoIndex;
  _pfOrigin = pfOrigin;
  WM_MOTION_Enable(1);
  WM_CreateWindow(0, 0, LCD_GetXSize(), LCD_GetYSize(), WM_CF_SHOW | WM_CF_MOTION_Y, _cbWin, 0);
}

/*************************** End of file ****************************/

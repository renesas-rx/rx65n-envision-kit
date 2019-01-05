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
File        : APP_Mandelbrot_Private.h
Purpose     : Private header file for Mandelbrot demo
----------------------------------------------------------------------
*/

#ifndef APP_MANDELBROT_PRIVATE_H
#define APP_MANDELBROT_PRIVATE_H

#include "GUI_Private.h"

#include "DIALOG.h"

#include "EnvisionKit.h"
#include "Resource.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_BUTTON_HOME (GUI_ID_USER + 0)
#define ID_BUTTON_HELP (GUI_ID_USER + 1)
#define ID_BUTTON_BACK (GUI_ID_USER + 2)
#define ID_BUTTON_CALC (GUI_ID_USER + 3)
#define ID_WINDOW_0    (GUI_ID_USER + 4)

#define ID_BUTTON_ONOFF  (GUI_ID_USER + 5)

#define BM_HOME       &bmHome_40x40
#define BM_HELP_0     &bmButtonHelp_0_60x40
#define BM_HELP_1     &bmButtonHelp_1_60x40

#define BM_BUTTON_0_0 &bmButtonOff_0_80x40
#define BM_BUTTON_0_1 &bmButtonOff_1_80x40
#define BM_BUTTON_1_0 &bmButtonOn_0_80x40
#define BM_BUTTON_1_1 &bmButtonOn_1_80x40

#define FONT_SMALL    &GUI_Font24_AA4
#define FONT_MEDIUM   &GUI_Font32_AA4

#define MAX_DEPTH 32
#define MAX_ITER  256

#define BORDER  20  // Distance from buttons to border
#define FRAME_X 10
#define FRAME_Y 16

#define XSIZE_BUTTON  80
#define YSIZE_BUTTON  40

#define PENSIZE 4
#define COLOR_SHAPE      GUI_MAKE_COLOR(0x7D5332)
#define COLOR_BUTTON_0   GUI_MAKE_COLOR(0xFCD7B9)
#define COLOR_BUTTON_1   GUI_MAKE_COLOR(0xA97144)

#define TEXT_NULL   -1
#define TEXT_EXIT    0
#define TEXT_CALC    1
#define TEXT_BACK    2
#define TEXT_SELECT  3
#define TEXT_MAX     4

#define USE_MULTIBUFFERING 0
#define SHOW_PERFORMANCE   0

#endif  // APP_MANDELBROT_PRIVATE_H

void DrawMandelbrot_FPU  (WM_MESSAGE * pMsg, MANDEL_DATA * pData, int x0, int y0, int x1, int y1);
void DrawMandelbrot_NOFPU(WM_MESSAGE * pMsg, MANDEL_DATA * pData, int x0, int y0, int x1, int y1);

/*************************** End of file ****************************/

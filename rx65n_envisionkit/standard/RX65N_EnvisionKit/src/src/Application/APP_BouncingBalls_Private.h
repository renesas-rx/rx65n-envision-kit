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
File        : APP_BouncingBalls_Private.h
Purpose     : Private header for BouncingBalls demo
----------------------------------------------------------------------
*/

#ifndef APP_BOUNCINGBALLS_PRIVATE_H
#define APP_BOUNCINGBALLS_PRIVATE_H

#include <stdlib.h>
#include <math.h>

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

#define PENSIZE       4
#define R_BALL        15

#define COLOR_SHAPE   GUI_MAKE_COLOR(0x7D5332)

#define FONT_SMALL    &GUI_Font24_AA4
#define FONT_MEDIUM   &GUI_Font32_AA4

#define BORDER 20  // Distance from buttons to border

#define ID_TIMER_MOVE   0
#define ID_TIMER_CREATE 1

#define CREATE_PERIOD   300

#define VX_MIN    20
#define VX_RANGE 120

#define ACCELLERATION 600.0f  // p/(s*s)
#define F_REVERSE      -0.96f
#define THRESHOLD      30.0f
#define MAX_NUM_BALLS  30

#endif  // APP_BOUNCINGBALLS_PRIVATE_H

/*************************** End of file ****************************/

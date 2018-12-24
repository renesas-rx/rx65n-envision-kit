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
File        : APP_BouncingBalls_NOFPU.c
Purpose     : Routines to be compiled with FPU disabled
----------------------------------------------------------------------
*/

#include "APP_BouncingBalls_Private.h"

/*********************************************************************
*
*       KeepArea_NOFPU
*/
void KeepArea_NOFPU(BOUNCE_DATA * pData) {
  #include "APP_BouncingBalls_KeepArea.h"
}

/*********************************************************************
*
*       MoveBalls_NOFPU
*/
void MoveBalls_NOFPU(BOUNCE_DATA * pData, GUI_TIMER_TIME tDiff) {
  #include "APP_BouncingBalls_MoveBalls.h"
}

/*************************** End of file ****************************/
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

#include "APP_BouncingBalls_AdvanceSim.h"

/*********************************************************************
*
*       BALLSSIM_AdvanceSim_NOFPU
*/
void BALLSSIM_AdvanceSim_NOFPU(BALLSIM * pBallsim, const float dt) {
  _AdvanceSim(pBallsim, dt);
}

/*************************** End of file ****************************/

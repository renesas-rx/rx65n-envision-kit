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
File        : APP_BouncingBalls_MoveBalls.h
Purpose     : Code to be compiled with different FPU settings
----------------------------------------------------------------------
*/

  BALL * pBall;

  pBall = pData->pFirst;
  while (pBall) {
    pBall->vx *= 1.0f - (float)tDiff / 8000;
    pBall->vy += (ACCELLERATION * tDiff) / 1000;
    pBall->xPos += (pBall->vx * tDiff) / 1000;
    pBall->yPos += (pBall->vy * tDiff) / 1000;
    pBall = pBall->pNext;
  }

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
File        : APP_BouncingBalls_KeepArea.h
Purpose     : Code to be compiled with different FPU settings
----------------------------------------------------------------------
*/

  BALL * pBall;

  pBall = pData->pFirst;
  while (pBall) {
    if ((pBall->xPos - R_BALL) <= pData->x0) {
      pBall->xPos = pData->x0 + R_BALL + (pData->x0 - (pBall->xPos - R_BALL));
      pBall->vx *= -1;
    }
    if ((pBall->xPos + R_BALL) >= pData->x1) {
      pBall->xPos = pData->x1 - R_BALL - ((pBall->xPos + R_BALL) - pData->x1);
      pBall->vx *= -1;
    }
    if ((pBall->yPos - R_BALL) <= pData->y0) {
      pBall->yPos = pData->y0 + R_BALL + (pData->y0 - (pBall->yPos - 15));
      pBall->vy *= F_REVERSE;
    }
    if ((pBall->yPos + R_BALL) >= pData->y1) {
      pBall->yPos = pData->y1 - R_BALL - ((pBall->yPos + 15) - pData->y1);
      pBall->vy *= F_REVERSE;
    }
    pBall = pBall->pNext;
  }

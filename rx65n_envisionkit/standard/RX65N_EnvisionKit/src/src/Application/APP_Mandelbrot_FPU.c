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
File        : APP_Mandelbrot_FPU.c
Purpose     : Routines to be compiled with FPU enabled
----------------------------------------------------------------------
*/

#include "APP_Mandelbrot_Private.h"

/*********************************************************************
*
*       DrawMandelbrot_FPU
*/
void DrawMandelbrot_FPU(WM_MESSAGE * pMsg, MANDEL_DATA * pData, int x0, int y0, int x1, int y1) {
  #include "APP_Mandelbrot_DrawMandelbrot.h"
}

/*************************** End of file ****************************/

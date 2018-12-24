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
File        : APP_Mandelbrot_DrawMandelbrot.h
Purpose     : Code to be compiled with different FPU settings
----------------------------------------------------------------------
*/

#if USE_MULTIBUFFERING
  double x, y;
  double u, v;
  int i, j, k;
  int xSize, ySize;
  int IsInner;
  double dx, dy, u2, v2;
  int xOrg, yOrg;
  int xPos, yPos;
  WM_HWIN hWin;
  GUI_RECT ClipRect;

  ClipRect = GUI_pContext->ClipRect;
  hWin = pMsg->hWin;
  xOrg = WM_GetWindowOrgX(hWin);
  yOrg = WM_GetWindowOrgY(hWin);
  xSize = x1 - x0 + 1;
  ySize = y1 - y0 + 1;
  dx = (pData->Range.xMax - pData->Range.xMin) / xSize;
  dy = (pData->Range.yMax - pData->Range.yMin) / ySize;
  IsInner = 0;
  for (j = 0; j < ySize; j++) {
    yPos = y1 - j + yOrg;
    if ((yPos >= ClipRect.y0) && (yPos <= ClipRect.y1)) {
      y = pData->Range.yMax - j * dy;
      for (i = 0; i < xSize; i++) {
        xPos = x0 + i + xOrg;
        if ((xPos >= ClipRect.x0) && (xPos <= ClipRect.x1)) {
          u = v = u2 = v2 = 0;
          x = pData->Range.xMin + i * dx;
          //
          // Iterate the point
          //
          for (k = 0; (k < pData->MaxIter) && (u2 + v2 < 4.0); k++) {
            v = 2 * u * v + y;
            u = u2 - v2 + x;
            u2 = u * u;
            v2 = v * v;
          };
          //
          // Compute pixel color
          //
          if (k >= pData->MaxIter) {
            //
            // Interior
            //
            if (IsInner == 0) {
              IsInner = 1;
              GUI_SetColor(GUI_BLACK);
            }
          } else {
            //
            // Exterior
            //
            IsInner = 0;
            GUI_SetColor(*(pData->pColor + (k % pData->NumColors)));
          };
          LCD_DrawPixel(xPos, yPos);
        }
      }
    }
  }
#else
  double x, y;
  double u, v;
  int i, j, k, s;
  int xSize, ySize;
  int IsInner;
  double dx, dy, u2, v2;
  int xOrg, yOrg;
  int xPos, yPos;
  WM_HWIN hWin;
  GUI_RECT ClipRect;
  GUI_PID_STATE State;

  ClipRect = GUI_pContext->ClipRect;
  hWin = pMsg->hWin;
  xOrg = WM_GetWindowOrgX(hWin);
  yOrg = WM_GetWindowOrgY(hWin);
  xSize = x1 - x0 + 1;
  ySize = y1 - y0 + 1;
  dx = (pData->Range.xMax - pData->Range.xMin) / xSize;
  dy = (pData->Range.yMax - pData->Range.yMin) / ySize;
  IsInner = 0;
  for (i = ySize / 2, s = 1; i >= 1; i >>= 1, s *= 2);
  do {
    for (j = s - 1; j < ySize; j += s + s) {
      yPos = y1 - j + yOrg;
      if ((yPos >= ClipRect.y0) && (yPos <= ClipRect.y1)) {
        y = pData->Range.yMax - j * dy;
        for (i = 0; i < xSize; i++) {
          xPos = x0 + i + xOrg;
          if ((xPos >= ClipRect.x0) && (xPos <= ClipRect.x1)) {
            u = v = u2 = v2 = 0;
            x = pData->Range.xMin + i * dx;
            //
            // Iterate the point
            //
            for (k = 0; (k < pData->MaxIter) && (u2 + v2 < 4.0); k++) {
              v = 2 * u * v + y;
              u = u2 - v2 + x;
              u2 = u * u;
              v2 = v * v;
            };
            //
            // Compute pixel color
            //
            if (k >= pData->MaxIter) {
              //
              // Interior
              //
              if (IsInner == 0) {
                IsInner = 1;
                GUI_SetColor(GUI_BLACK);
              }
            } else {
              //
              // Exterior
              //
              IsInner = 0;
              GUI_SetColor(*(pData->pColor + (k % pData->NumColors)));
            };
            if (s > 1) {
              LCD_DrawVLine(xPos, y1 - j - s + 1 + yOrg, yPos);
            } else {
              LCD_DrawPixel(xPos, yPos);
            }
          }
        }
        if (GUI_TOUCH_GetState(&State)) {
          return;
        }
      }
    }
  } while (s >>= 1);
#endif

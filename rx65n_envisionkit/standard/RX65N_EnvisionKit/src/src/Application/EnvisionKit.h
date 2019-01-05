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
File        : EnvisionKit.h
Purpose     : Interface of demo
----------------------------------------------------------------------
*/

#ifndef ENVISIONKIT_H
#define ENVISIONKIT_H

#define TIMER_PERIOD 20

#define DEMO_0        0  // 2D Drawing Engine
#define DEMO_1        1  // Bouncing Balls
#define DEMO_2        2  // Mandelbrot
#define DEMO_3        3  // Bank Swap

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
//
// Mandelbrot
//
typedef struct {
  double xMin;
  double xMax;
  double yMin;
  double yMax;
} MANDEL_RANGE;

typedef struct {
  MANDEL_RANGE Range;
  int MaxIter;
  int Depth;
  GUI_COLOR * pColor;
  int NumColors;
  int IsDown, IsVis, x0, y0, x1, y1;
  int xSize, ySize;
  WM_HWIN hWin;
} MANDEL_DATA;

/*********************************************************************
*
*       Interface
*
**********************************************************************
*/
void HelpDialog(int DemoIndex, void (*pfOrigin)(void));
void BankSwap(void);
void BouncingBalls(void);
void DoUpdate(void);
void DrawingEngine(void);
void Mandelbrot(void);
void StartScreen(void);
void InitUpdate(void);
void InitCheckPressedState(void);

void CheckPressedState(void);

#ifdef WIN32
#define LCDCONF_DisableDave2D()
#define LCDCONF_EnableDave2D()
#define bank_swap()
#endif

/*********************************************************************
*
*       Data
*
**********************************************************************
*/
#ifdef EXTERN
  #undef EXTERN
#endif

#ifdef MAINTASK_ENVISIONKIT
  #define EXTERN
#else
  #define EXTERN extern
#endif

EXTERN U8 UpdatePressed;

#undef EXTERN

#endif  // ENVISIONKIT_H

/*************************** End of file ****************************/

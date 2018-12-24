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

#ifndef WIN32
#include "rpbrx65n_2mb.h"
#endif
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

//
// Bouncing balls
//
typedef struct BALL BALL;

struct BALL {
  BALL * pNext;
  BALL * pPrev;
  const GUI_BITMAP * pBm;
  GUI_COLOR Color;
  float vx;
  float vy;
  float xPos;
  float yPos;
};

typedef struct {
  int x0, y0, x1, y1;
  WM_HWIN hWin;
  BALL * pFirst;
  int NumBalls;
} BOUNCE_DATA;

/*********************************************************************
*
*       Interface
*
**********************************************************************
*/
void HelpDialog(int DemoIndex, void (*pfOrigin)(void));
void BankSwap(void);
void BouncingBalls(void);
  void KeepArea_FPU  (BOUNCE_DATA * pData);
  void KeepArea_NOFPU(BOUNCE_DATA * pData);
  void MoveBalls_FPU  (BOUNCE_DATA * pData, GUI_TIMER_TIME tDiff);
  void MoveBalls_NOFPU(BOUNCE_DATA * pData, GUI_TIMER_TIME tDiff);
void DoUpdate(void);
void DrawingEngine(void);
void Mandelbrot(void);
  void DrawMandelbrot_FPU  (WM_MESSAGE * pMsg, MANDEL_DATA * pData, int x0, int y0, int x1, int y1);
  void DrawMandelbrot_NOFPU(WM_MESSAGE * pMsg, MANDEL_DATA * pData, int x0, int y0, int x1, int y1);
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

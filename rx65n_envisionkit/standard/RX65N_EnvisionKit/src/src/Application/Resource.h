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
File        : Resource.h
Purpose     : Contains prototypes of resources and color definitions
----------------------------------------------------------------------
*/

#ifndef RESOURCE_H
#define RESOURCE_H

//
// Bitmaps
//
extern GUI_CONST_STORAGE GUI_BITMAP bmBallGray_30x30;
extern GUI_CONST_STORAGE GUI_BITMAP bmBallGreen_30x30;
extern GUI_CONST_STORAGE GUI_BITMAP bmBallRed_30x30;
extern GUI_CONST_STORAGE GUI_BITMAP bmBallYellow_30x30;
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonExit_0_60x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonExit_1_60x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonOff_0_80x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonOff_1_80x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonOn_0_80x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonOn_1_80x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonHelp_0_60x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonHelp_1_60x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonSwap_0_60x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonSwap_1_60x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmCloud_220x120;
extern GUI_CONST_STORAGE GUI_BITMAP bmGoldenGate_480x272;
extern GUI_CONST_STORAGE GUI_BITMAP bmHome_40x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmProgbarDotL_21x21;
extern GUI_CONST_STORAGE GUI_BITMAP bmProgbarDotR_21x21;
extern GUI_CONST_STORAGE GUI_BITMAP bmRenesas_120x19;
extern GUI_CONST_STORAGE GUI_BITMAP bmRenesasRX_155x55;
extern GUI_CONST_STORAGE GUI_BITMAP bmSegger_120x60;
extern GUI_CONST_STORAGE GUI_BITMAP bmSkyline_480x272;

//
// Sprites
//
extern const GUI_BITMAP * apbmSwapBank_80x80[2];
extern const unsigned     aDelaySwapBank_80x80[2];

//
// Fonts
//
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16_F;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font21_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font24_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font48B_AA4;

//
// Colors
//
#define COLOR_RED     GUI_MAKE_COLOR(0x0000D7)
#define COLOR_GRAY    GUI_MAKE_COLOR(0x898B8B)
#define COLOR_YELLOW  GUI_MAKE_COLOR(0x99E9FF)
#define COLOR_GREEN   GUI_MAKE_COLOR(0x339966)

//#define SWAP_VERSION

#ifdef SWAP_VERSION
  #define COLOR_BK COLOR_YELLOW
#else
  #define COLOR_BK GUI_WHITE
#endif

#endif // RESOURCE_H

/*************************** End of file ****************************/

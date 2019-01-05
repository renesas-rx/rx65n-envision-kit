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
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
----------------------------------------------------------------------

MultiBuffering:
  emWin requires 3 buffers to be able to work with efficiently.
  The available On-chip RAM unfortunately restricts use of
  3 buffers for MultiBuffering to 1- and 4bpp mode.
  In 8bpp the line offset is 512, because LNOFF needs to be a
  multiple of 0x40 (see GRnFLM3.LNOFF). In that case
  0x200 * 0x110 * 3 = 0x66000 bytes are required. But
  unfortunately the size of the memory area is only 0x60000.
  In that case DoubleBuffering is used. In that mode the CPU has to
  wait for the next VSYNC interrupt in case of a new frame buffer.

32bpp mode:
  Has not been tested because of a lack of RAM.

---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>

#include "GUI_Private.h"
#include "GUIDRV_Lin.h"
#include "LCDConf.h"

#ifdef __RX
  #include "platform.h"
  #include "r_glcdc_rx_if.h"
  #include "r_pinset.h"
  #include "r_dmaca_rx_if.h"
#endif
#ifdef __ICCRX__
  #include "iorx65n.h"
#endif

#include "dave_base.h"
#include "dave_videomodes.h"
#include "dave_driver.h"

extern void drw_int_isr(void);

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/
//
// Physical display size
//
#define XSIZE_PHYS 480
#define YSIZE_PHYS 272

//
// Color depth
//
#define BITS_PER_PIXEL 16  // Allowed values: 1, 4, 8, 16, 32

/*********************************************************************
*
*       Determine driver, color conversion and size of frame buffer
*
**********************************************************************
*/
//
// Color format selection
//
#define FORMAT_RGB_565   (GLCDC_IN_FORMAT_16BITS_RGB565)
#define FORMAT_RGB_888   (GLCDC_IN_FORMAT_32BITS_RGB888)
#define FORMAT_ARGB_1555 (GLCDC_IN_FORMAT_16BITS_ARGB1555)
#define FORMAT_ARGB_4444 (GLCDC_IN_FORMAT_16BITS_ARGB4444)
#define FORMAT_ARGB_8888 (GLCDC_IN_FORMAT_32BITS_ARGB8888)
#define FORMAT_CLUT_8    (GLCDC_IN_FORMAT_CLUT8)
#define FORMAT_CLUT_4    (GLCDC_IN_FORMAT_CLUT4)
#define FORMAT_CLUT_1    (GLCDC_IN_FORMAT_CLUT1)

//
// Color conversion, display driver and multiple buffers
//
#if   (BITS_PER_PIXEL == 32)
  #define COLOR_CONVERSION GUICC_M8888I
  #define DISPLAY_DRIVER   GUIDRV_LIN_32
  //#define DISPLAY_DRIVER   GUIDRV_LIN_OSX_32
  //#define DISPLAY_DRIVER   GUIDRV_LIN_OSY_32
  //#define DISPLAY_DRIVER   GUIDRV_LIN_OXY_32
  #define COLOR_FORMAT     FORMAT_RGB_888
  #define NUM_BUFFERS      1
#elif (BITS_PER_PIXEL == 16)
  #define COLOR_CONVERSION GUICC_M565
  #define DISPLAY_DRIVER   GUIDRV_LIN_16      // Default
  //#define DISPLAY_DRIVER   GUIDRV_LIN_OSX_16  // CW
  //#define DISPLAY_DRIVER   GUIDRV_LIN_OSY_16  // CCW
  //#define DISPLAY_DRIVER   GUIDRV_LIN_OXY_16  // 180
  #define COLOR_FORMAT     FORMAT_RGB_565
  #define NUM_BUFFERS      2
#elif (BITS_PER_PIXEL == 8)
  #define COLOR_CONVERSION GUICC_8666
  #define DISPLAY_DRIVER   GUIDRV_LIN_8
  //#define DISPLAY_DRIVER   GUIDRV_LIN_OXY_8
  #define COLOR_FORMAT     FORMAT_CLUT_8
  #define NUM_BUFFERS      2
#elif (BITS_PER_PIXEL == 4)
  #define COLOR_CONVERSION GUICC_16
  #define DISPLAY_DRIVER   GUIDRV_LIN_4
  #define COLOR_FORMAT     FORMAT_CLUT_4
  #define NUM_BUFFERS      3
#elif (BITS_PER_PIXEL == 1)
  #define COLOR_CONVERSION GUICC_1
  #define DISPLAY_DRIVER   GUIDRV_LIN_1
  #define COLOR_FORMAT     FORMAT_CLUT_1
  #define NUM_BUFFERS      3
#endif

//
// Buffer size and stride
//
#define BYTES_PER_LINE   ((BITS_PER_PIXEL * XSIZE_PHYS) / 8)
#define LINE_OFFSET      (((BYTES_PER_LINE + 63) / 64) * 64)
#define VXSIZE_PHYS      ((LINE_OFFSET * 8) / BITS_PER_PIXEL)
#define BYTES_PER_BUFFER (LINE_OFFSET * YSIZE_PHYS)

//
// Timing
//
#define SYNC_H       4
#define SYNC_V       1
#define BGSYNC_HP    2
#define BGSYNC_VP    2
#define BGHSIZE_HP  (BGSYNC_HP + SYNC_H)
#define BGVSIZE_VP  (BGSYNC_VP + SYNC_V)
#define GRC_VS      (BGVSIZE_VP - BGSYNC_VP)
#define GRC_HS      (BGHSIZE_HP - BGSYNC_HP)

//
// Brightness & COntrast
//
#define _BRIGHTNESS 0x200
#define _CONTRAST   0x80

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const U32 _aBufferPTR[] = {
  0x00000100,  // Begin of On-Chip RAM
  0x00800000   // Begin of Expansion RAM
};

static volatile int _PendingBuffer = -1;

static unsigned _WriteBufferIndex;

static glcdc_runtime_cfg_t runtime_cfg;

static unsigned _DaveActive;
//
// Dave2D
//
static d2_device       * d2_handle;
static d2_renderbuffer * renderbuffer;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _VSYNC_ISR
*/
static void _VSYNC_ISR(void * p) {
#if (NUM_BUFFERS == 3)
  U32 Addr;
  glcdc_err_t ret;
#endif

 											    // Clears the STMON.VPOS flag (GLCDC FIT module clears STMON.VPOS flag)
#if (NUM_BUFFERS == 3)
  //
  // Makes the pending buffer visible
  //
  if (_PendingBuffer >= 0) {
    Addr = FRAMEBUFFER_START
         + BYTES_PER_BUFFER * _PendingBuffer;   // Calculate address of buffer to be used  as visible frame buffer
    runtime_cfg.input.p_base = (uint32_t *)Addr; // Specify the start address of the frame buffer
    ret = R_GLCDC_LayerChange(GLCDC_FRAME_LAYER_2, &runtime_cfg);	// Graphic 2 Register Value Reflection Enable
    if (ret != GLCDC_SUCCESS) {
    	while(1);
    }
    GUI_MULTIBUF_ConfirmEx(0, _PendingBuffer);  // Tell emWin that buffer is used
    _PendingBuffer = -1;                        // Clear pending buffer flag
  }
#elif (NUM_BUFFERS == 2)
  //
  // Sets a flag to be able to notice the interrupt
  //
  _PendingBuffer = 0;
#endif
}

/*********************************************************************
*
*       _SwitchBuffersOnVSYNC
*/
#if (NUM_BUFFERS == 2)

static void _SwitchBuffersOnVSYNC(int Index) {
  U32 Addr;
  glcdc_err_t ret;

  for (_PendingBuffer = 1; _PendingBuffer; );  // Wait until _PendingBuffer is cleared by ISR
  Addr = _aBufferPTR[Index];
  runtime_cfg.input.p_base = (uint32_t *)Addr; // Specify the start address of the frame buffer
  ret = R_GLCDC_LayerChange(GLCDC_FRAME_LAYER_2, &runtime_cfg);	// Graphic 2 Register Value Reflection Enable
  if (ret != GLCDC_SUCCESS) {
	  while(1);
  }
  GUI_MULTIBUF_ConfirmEx(0, Index);  // Tell emWin that buffer is used
}
#endif

/*********************************************************************
*
*       _InitController
*
* Purpose:
*   Should initialize the display controller
*/
static void _InitController(void) {
  glcdc_cfg_t glcdc_cfg;
  glcdc_err_t ret;
  //
  // Release stop state of GLCDC
  //
  // R_GLCDC_Open function release stop state of GLCDC.
  //
  // Function select of multiplex pins (Display B)
  //
  R_GLCDC_PinSet();
  //
  // Set DISP signal on P97 (Pin31)
  //
  PORT6.PDR.BIT.B3  = 1;  // Port direction: output
  PORT6.PODR.BIT.B3 = 1;  // DISP on
  //
  // Switch backlight on, no PWM
  //
  PORT6.PDR.BIT.B6  = 1;
  PORT6.PODR.BIT.B6 = 1;
  //
  // Set the BGEN.SWRST bit to 1 to release the GLCDC from a software reset
  //
  // R_GLCDC_Open function release the GLCDC from a software reset.
  //
  // Set the frequency of the LCD_CLK and PXCLK to suit the format and set the PANELCLK.CLKEN bit to 1
  //
  glcdc_cfg.output.clksrc = GLCDC_CLK_SRC_INTERNAL;   			  // Select PLL clock
  glcdc_cfg.output.clock_div_ratio = GLCDC_PANEL_CLK_DIVISOR_24;  // 240 / 24 = 10 MHz
  																  // No frequency division
  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  // Enable LCD_CLK output
  //
  // Definition of Background Screen
  //
  										   	   	   	   	   	   	  // Horizontal cycle (whole control screen)
  	  	  	  	  	  	  	  	  	  	   	   	   	   	   	   	  // Vertical cycle (whole control screen)
  glcdc_cfg.output.htiming.front_porch = 5;         			  // Horizontal Synchronization Signal Assertion Position
  glcdc_cfg.output.vtiming.front_porch = 8;         			  // Vertical Synchronization Assertion Position
  glcdc_cfg.output.htiming.back_porch = 40;                        // Horizontal Active Pixel Start Position (min. 6 pixels)
  glcdc_cfg.output.vtiming.back_porch = 8;
  glcdc_cfg.output.htiming.display_cyc = XSIZE_PHYS;              // Horizontal Active Pixel Width
  glcdc_cfg.output.vtiming.display_cyc = YSIZE_PHYS;              // Vertical Active Display Width
  glcdc_cfg.output.htiming.sync_width = 1;        				  // Vertical Active Display Start Position (min. 3 lines)
  glcdc_cfg.output.vtiming.sync_width = 1;

  //
  // Graphic 1 configuration
  //
  glcdc_cfg.input[GLCDC_FRAME_LAYER_1].p_base = NULL;			  // Disable Graphics 1

  //
  // Graphic 2 configuration
  //
  													  	  	  	  // Enable reading of the frame buffer
  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].p_base = (uint32_t *)_aBufferPTR[0];   // Specify the start address of the frame buffer
  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].offset = LINE_OFFSET;      // Offset value from the end address of the line to the start address of the next line
  																  // Single Line Data Transfer Count
  																  // Single Frame Line Count
  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].format = COLOR_FORMAT;     // Frame Buffer Color Format RGB565
  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].visible = true;
  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].blend_control = GLCDC_BLEND_CONTROL_NONE;	// Display Screen Control (current graphics)
  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].frame_edge = false;        // Rectangular Alpha Blending Area Frame Display Control
  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].frame_edge = false;        // Graphics Area Frame Display Control
  																  // Alpha Blending Control (Per-pixel alpha blending)
  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.y = 0;          // Graphics Area Vertical Start Position
  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].vsize = YSIZE_PHYS;        // Graphics Area Vertical Width
  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.x = 0;          // Graphics Area Horizontal Start Position
  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].hsize = XSIZE_PHYS;        // Graphics Area Horizontal Width
  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.x = 0;    // Rectangular Alpha Blending Area Vertical Start Position
  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.x= YSIZE_PHYS; // Rectangular Alpha Blending Area Vertical Width
  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.y = 0;    // Rectangular Alpha Blending Area Horizontal Start Position
  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.y= XSIZE_PHYS; // Rectangular Alpha Blending Area Horizontal Width
  																  // Graphic 2 Register Value Reflection Enable
  //
  // Timing configuration
  //
  //   Horizontal Synchronization Signal Reference Timing Offset (not support)
  //   Signal Reference Timing Select (not support)
  //   STVB Signal Assertion Timing
  //   STVB Signal Pulse Width
  //   STHB Signal Pulse Width
  glcdc_cfg.output.tcon_vsync = GLCDC_TCON_PIN_0;           // TCON0 Output Signal Select STVA (VSYNC)
  glcdc_cfg.output.tcon_hsync = GLCDC_TCON_PIN_2;           // TCON2 Output Signal Select STHA (HSYNC)
  glcdc_cfg.output.tcon_de    = GLCDC_TCON_PIN_3;           // TCON3 Output Signal Select DE (DEN)
  glcdc_cfg.output.data_enable_polarity = GLCDC_SIGNAL_POLARITY_HIACTIVE;
  glcdc_cfg.output.hsync_polarity = GLCDC_SIGNAL_POLARITY_LOACTIVE;
  glcdc_cfg.output.vsync_polarity = GLCDC_SIGNAL_POLARITY_LOACTIVE;
  glcdc_cfg.output.sync_edge = GLCDC_SIGNAL_SYNC_EDGE_RISING;
  //
  // Output interface
  //
  //   Serial RGB Data Output Delay Control (0 cycles) (not support)
  //   Serial RGB Scan Direction Select (forward) (not support)
  //   Pixel Clock Division Control (no division)
  glcdc_cfg.output.format = GLCDC_OUT_FORMAT_16BITS_RGB565; // Output Data Format Select (RGB565)
  glcdc_cfg.output.color_order = GLCDC_COLOR_ORDER_RGB; ///**/GLCDC_COLOR_ORDER_BGR;  	// Pixel Order Control (B-G-R)
  glcdc_cfg.output.endian = GLCDC_ENDIAN_LITTLE;  			// Bit Endian Control (Little endian)
  //
  // Brightness Adjustment
  //
  glcdc_cfg.output.brightness.b = _BRIGHTNESS;  // B
  glcdc_cfg.output.brightness.g = _BRIGHTNESS;  // G
  glcdc_cfg.output.brightness.r = _BRIGHTNESS;  // R
  //
  // Contrast Adjustment Value
  //
  glcdc_cfg.output.contrast.b = _CONTRAST;  // B
  glcdc_cfg.output.contrast.g = _CONTRAST;  // G
  glcdc_cfg.output.contrast.r = _CONTRAST;  // R
  //
  // Disable Gamma
  //
  glcdc_cfg.output.gamma.enable = false;
  //
  // Disable Chromakey
  //
  glcdc_cfg.chromakey[GLCDC_FRAME_LAYER_2].enable = false;
  //
  // Disable Dithering
  //
  glcdc_cfg.output.dithering.dithering_on = false;
  //
  // CLUT Adjustment Value
  //
  glcdc_cfg.clut[GLCDC_FRAME_LAYER_2].enable = false;
  //
  // Enable VPOS ISR
  //
  //   Detecting Scanline Setting
  glcdc_cfg.detection.vpos_detect = true;		         	// Enable detection of specified line notification in graphic 2
  glcdc_cfg.interrupt.vpos_enable = true;		           	// Enable VPOS interrupt request
  //   Interrupt Priority Level (r_glcdc_rx_config.h)
  //   Interrupt Request Enable
  //   Clears the STMON.VPOS flag
  //   VPOS (line detection)
  glcdc_cfg.detection.gr1uf_detect = false;
  glcdc_cfg.detection.gr2uf_detect = false;
  glcdc_cfg.interrupt.gr1uf_enable = false;
  glcdc_cfg.interrupt.gr2uf_enable = false;
  //
  // Set function to be called on VSYNC
  //
  glcdc_cfg.p_callback = (void (*)(glcdc_callback_args_t *))_VSYNC_ISR;

  runtime_cfg.blend = glcdc_cfg.blend[GLCDC_FRAME_LAYER_2];
  runtime_cfg.input = glcdc_cfg.input[GLCDC_FRAME_LAYER_2];
  runtime_cfg.chromakey = glcdc_cfg.chromakey[GLCDC_FRAME_LAYER_2];
  //
  // Register Dave2D interrupt
  //
  R_BSP_InterruptWrite(BSP_INT_SRC_AL1_DRW2D_DRW_IRQ, (bsp_int_cb_t)drw_int_isr);
  //
  // Register Reflection
  //
  ret = R_GLCDC_Open(&glcdc_cfg);
  if (ret != GLCDC_SUCCESS) {
	  while(1);
  }
  //
  // Init DMA
  //
  R_DMACA_Init();
  //
  // Extended Bus Master Priority Control Register
  //
  BSC.EBMAPCR.BIT.PR1SEL = 3;
  BSC.EBMAPCR.BIT.PR2SEL = 1;
  BSC.EBMAPCR.BIT.PR3SEL = 2;
  BSC.EBMAPCR.BIT.PR4SEL = 0;
  BSC.EBMAPCR.BIT.PR5SEL = 4;
}

/*********************************************************************
*
*       _SetLUTEntry
*
* Purpose:
*   Should set the desired LUT entry
*/
static void _SetLUTEntry(LCD_COLOR Color, U8 Pos) {
  glcdc_clut_cfg_t p_clut_cfg;

  p_clut_cfg.enable = true;
  p_clut_cfg.p_base = &Color;
  p_clut_cfg.size = 1;
  p_clut_cfg.start = Pos;

  R_GLCDC_ClutUpdate(GLCDC_FRAME_LAYER_2, &p_clut_cfg);
}

/*********************************************************************
*
*       _DisplayOnOff
*/
static void _DisplayOnOff(int OnOff) {
  if (OnOff) {
    R_GLCDC_Control(GLCDC_CMD_START_DISPLAY, FIT_NO_FUNC);  // Enable background generation block
    PORT6.PODR.BIT.B6 = 1;
  } else {
	R_GLCDC_Control(GLCDC_CMD_STOP_DISPLAY, FIT_NO_FUNC);  	// Disable background generation block
    PORT6.PODR.BIT.B6 = 0;
  }
}

/*********************************************************************
*
*       _GetD2Mode
*/
static U32 _GetD2Mode(void) {
  U32 r;
#if   (BITS_PER_PIXEL == 32)
  r = d2_mode_argb8888;
#elif (BITS_PER_PIXEL == 16)
  r = d2_mode_rgb565;
#elif (BITS_PER_PIXEL == 8)
  r = d2_mode_i8;
#elif (BITS_PER_PIXEL == 4)
  r = d2_mode_i4;
#elif (BITS_PER_PIXEL == 1)
  r = d2_mode_i1;
#endif
  return r;
}

/*********************************************************************
*
*       _DrawMemdevAlpha
*/
static void _DrawMemdevAlpha(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc) {
  U32 PitchSrc, PitchDst, Mode;

  PitchDst = BytesPerLineDst / 4;
  PitchSrc = BytesPerLineSrc / 4;
  Mode = _GetD2Mode();
  //
  // Set address of destination memory device as frame buffer to be used
  //
  d2_framebuffer(d2_handle, pDst, PitchDst, PitchDst, ySize, d2_mode_argb8888);
  //
  // Generate render operations
  //
  d2_selectrenderbuffer(d2_handle, renderbuffer);
  d2_setblitsrc(d2_handle, (void *)pSrc, PitchSrc, xSize, ySize, d2_mode_argb8888);
  d2_blitcopy(d2_handle, xSize, ySize, 0, 0, xSize * 16, ySize * 16, 0, 0, d2_bf_usealpha);
  //
  // Execute render operations
  //
  d2_executerenderbuffer(d2_handle, renderbuffer, 0);
  d2_flushframe(d2_handle);
  //
  // Restore frame buffer
  //
  d2_framebuffer(d2_handle, (void *)_aBufferPTR[_WriteBufferIndex], XSIZE_PHYS, XSIZE_PHYS, YSIZE_PHYS, Mode);
}

/*********************************************************************
*
*       _DrawBitmapAlpha
*/
static void _DrawBitmapAlpha(int LayerIndex, int x, int y, const void * p, int xSize, int ySize, int BytesPerLine) {
  U32 Pitch, Mode;

  Pitch = BytesPerLine / 4;
  Mode = _GetD2Mode();
  //
  // Generate render operations
  //
  d2_framebuffer(d2_handle, (void *)_aBufferPTR[_WriteBufferIndex], XSIZE_PHYS, XSIZE_PHYS, YSIZE_PHYS, Mode);
  d2_selectrenderbuffer(d2_handle, renderbuffer);
  d2_setblitsrc(d2_handle, (void *)p, Pitch, xSize, ySize, d2_mode_argb8888);
  d2_blitcopy(d2_handle, xSize, ySize, 0, 0, xSize * 16, ySize * 16, x * 16, y * 16, d2_bf_usealpha);
  //
  // Execute render operations
  //
  d2_executerenderbuffer(d2_handle, renderbuffer, 0);
  d2_flushframe(d2_handle);
}

/*********************************************************************
*
*       _CircleAA
*/
static int _CircleAA(int x0, int y0, int r, int w) {
  U32 Mode;
  int ret;

  Mode = _GetD2Mode();
  //
  // Generate render operations
  //
  d2_framebuffer(d2_handle, (void *)_aBufferPTR[_WriteBufferIndex], XSIZE_PHYS, XSIZE_PHYS, YSIZE_PHYS, Mode);
  d2_selectrenderbuffer(d2_handle, renderbuffer);
  d2_setcolor(d2_handle, 0, GUI_pContext->Color);
  d2_cliprect(d2_handle, GUI_pContext->ClipRect.x0,
                         GUI_pContext->ClipRect.y0,
                         GUI_pContext->ClipRect.x1,
                         GUI_pContext->ClipRect.y1);
  ret = d2_rendercircle(d2_handle, x0 * 16, y0 * 16, r * 16, w * 16);
  //
  // Execute render operations
  //
  d2_executerenderbuffer(d2_handle, renderbuffer, 0);
  d2_flushframe(d2_handle);
  return ret;
}

/*********************************************************************
*
*       _FillCircleAA
*/
static int _FillCircleAA(int x0, int y0, int r) {
  return _CircleAA(x0, y0, r, 0);
}

/*********************************************************************
*
*       _DrawCircleAA
*/
static int _DrawCircleAA(int x0, int y0, int r) {
  return _CircleAA(x0, y0, r, GUI_pContext->PenSize);
}

/*********************************************************************
*
*       _FillPolygonAA
*/
static int _FillPolygonAA(const GUI_POINT * pPoints, int NumPoints, int x0, int y0) {
  U32 Mode;
  d2_point * pData;
  d2_point * pDataI;
  int i, ret;

  Mode = _GetD2Mode();
  pData = malloc(sizeof(d2_point) * NumPoints * 2);
  ret = 1;
  if (pData) {
    pDataI = pData;
    for (i = 0; i < NumPoints; i++) {
      *pDataI++ = (d2_point)(pPoints->x + x0) * 16;
      *pDataI++ = (d2_point)(pPoints->y + y0) * 16;
      pPoints++;
    }
    //
    // Generate render operations
    //
    d2_framebuffer(d2_handle, (void *)_aBufferPTR[_WriteBufferIndex], XSIZE_PHYS, XSIZE_PHYS, YSIZE_PHYS, Mode);
    d2_selectrenderbuffer(d2_handle, renderbuffer);
    d2_setcolor(d2_handle, 0, GUI_pContext->Color);
    d2_cliprect(d2_handle, GUI_pContext->ClipRect.x0,
                           GUI_pContext->ClipRect.y0,
                           GUI_pContext->ClipRect.x1,
                           GUI_pContext->ClipRect.y1);
    ret = d2_renderpolygon(d2_handle, pData, NumPoints, d2_le_closed);
    //
    // Execute render operations
    //
    d2_executerenderbuffer(d2_handle, renderbuffer, 0);
    d2_flushframe(d2_handle);
    free(pData);
  }
  return ret;
}

/*********************************************************************
*
*       _DrawPolyOutlineAA
*/
static int _DrawPolyOutlineAA(const GUI_POINT * pPoints, int NumPoints, int Thickness, int x, int y) {
  U32 Mode;
  d2_point * pData;
  d2_point * pDataI;
  int i, ret;

  Mode = _GetD2Mode();
  pData = malloc(sizeof(d2_point) * NumPoints * 2);
  ret = 1;
  if (pData) {
    pDataI = pData;
    for (i = 0; i < NumPoints; i++) {
      *pDataI++ = (d2_point)(pPoints->x + x) * 16;
      *pDataI++ = (d2_point)(pPoints->y + y) * 16;
      pPoints++;
    }
    //
    // Generate render operations
    //
    d2_framebuffer(d2_handle, (void *)_aBufferPTR[_WriteBufferIndex], XSIZE_PHYS, XSIZE_PHYS, YSIZE_PHYS, Mode);
    d2_selectrenderbuffer(d2_handle, renderbuffer);
    d2_setcolor(d2_handle, 0, GUI_pContext->Color);
    d2_cliprect(d2_handle, GUI_pContext->ClipRect.x0,
                           GUI_pContext->ClipRect.y0,
                           GUI_pContext->ClipRect.x1,
                           GUI_pContext->ClipRect.y1);
    d2_selectrendermode(d2_handle, d2_rm_outline);
    ret = d2_renderpolyline(d2_handle, pData, NumPoints, Thickness * 16, d2_le_closed);
    d2_selectrendermode(d2_handle, d2_rm_solid);
    //
    // Execute render operations
    //
    d2_executerenderbuffer(d2_handle, renderbuffer, 0);
    d2_flushframe(d2_handle);
    free(pData);
  }
  return ret;
}

/*********************************************************************
*
*       _DrawLineAA
*/
static int _DrawLineAA(int x0, int y0, int x1, int y1) {
  U32 Mode;
  int ret;

  Mode = _GetD2Mode();
  //
  // Generate render operations
  //
  d2_framebuffer(d2_handle, (void *)_aBufferPTR[_WriteBufferIndex], XSIZE_PHYS, XSIZE_PHYS, YSIZE_PHYS, Mode);
  d2_selectrenderbuffer(d2_handle, renderbuffer);
  d2_setcolor(d2_handle, 0, GUI_pContext->Color);
  d2_cliprect(d2_handle, GUI_pContext->ClipRect.x0,
                         GUI_pContext->ClipRect.y0,
                         GUI_pContext->ClipRect.x1,
                         GUI_pContext->ClipRect.y1);
  ret = d2_renderline(d2_handle, x0 * 16, y0 * 16, x1 * 16, y1 * 16, GUI_pContext->PenSize * 16, d2_le_exclude_none);
  //
  // Execute render operations
  //
  d2_executerenderbuffer(d2_handle, renderbuffer, 0);
  d2_flushframe(d2_handle);
  return ret;
}

/*********************************************************************
*
*       _DrawArcAA
*/
int _DrawArcAA(int x0, int y0, int rx, int ry, int a0, int a1) {
  U32 Mode;
  int ret;
  I32 nx0, ny0, nx1, ny1;

  GUI_USE_PARA(ry);
  Mode = _GetD2Mode();
  while (a1 < a0) {
    a1 += 360;
  }
  a0 *= 1000;
  a1 *= 1000;
  nx0  = GUI__SinHQ(a0);
  ny0  = GUI__CosHQ(a0);
  nx1  = GUI__SinHQ(a1);
  ny1  = GUI__CosHQ(a1);
  if ((a1 - a0) <= 180000) {
    nx0 *= -1;
    ny0 *= -1;
  }
  //
  // Generate render operations
  //
  d2_framebuffer(d2_handle, (void *)_aBufferPTR[_WriteBufferIndex], XSIZE_PHYS, XSIZE_PHYS, YSIZE_PHYS, Mode);
  d2_selectrenderbuffer(d2_handle, renderbuffer);
  d2_setcolor(d2_handle, 0, GUI_pContext->Color);
  d2_cliprect(d2_handle, GUI_pContext->ClipRect.x0,
                         GUI_pContext->ClipRect.y0,
                         GUI_pContext->ClipRect.x1,
                         GUI_pContext->ClipRect.y1);
  d2_renderwedge(d2_handle, x0 * 16, y0 * 16, rx * 16, GUI_pContext->PenSize * 16, nx0, ny0, nx1, ny1, 0);
  //
  // Execute render operations
  //
  d2_executerenderbuffer(d2_handle, renderbuffer, 0);
  d2_flushframe(d2_handle);
  return ret;
}

/*********************************************************************
*
*       _CopyBuffer
*/
static void _CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst) {
  int ret;
  dmaca_transfer_data_cfg_t td_cfg;
  dmaca_stat_t dmac_status;

  GUI_USE_PARA(LayerIndex);
  ret = R_DMACA_Open(DMACA_CH0);
  if (ret != DMACA_SUCCESS) {
    while (1);  // Error
  }
  td_cfg.transfer_mode        = DMACA_TRANSFER_MODE_BLOCK;
  td_cfg.repeat_block_side    = DMACA_REPEAT_BLOCK_DISABLE;
  td_cfg.data_size            = DMACA_DATA_SIZE_LWORD;
  td_cfg.act_source           = (dmaca_activation_source_t)0;
  td_cfg.request_source       = DMACA_TRANSFER_REQUEST_SOFTWARE;
  td_cfg.dtie_request         = DMACA_TRANSFER_END_INTERRUPT_DISABLE;
  td_cfg.esie_request         = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
  td_cfg.rptie_request        = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
  td_cfg.sarie_request        = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
  td_cfg.darie_request        = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
  td_cfg.src_addr_mode        = DMACA_SRC_ADDR_INCR;
  td_cfg.src_addr_repeat_area = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
  td_cfg.des_addr_mode        = DMACA_DES_ADDR_INCR;
  td_cfg.des_addr_repeat_area = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
  td_cfg.offset_value         = BYTES_PER_LINE;
  td_cfg.interrupt_sel        = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;
  td_cfg.p_src_addr           = (void *)_aBufferPTR[IndexSrc];
  td_cfg.p_des_addr           = (void *)_aBufferPTR[IndexDst];
  td_cfg.transfer_count       = YSIZE_PHYS;
  td_cfg.block_size           = BYTES_PER_LINE >> 2;
  ret = R_DMACA_Create(DMACA_CH0, &td_cfg);
  if (ret != DMACA_SUCCESS) {
    while (1);  // Error
  }
  ret = R_DMACA_Control(DMACA_CH0, DMACA_CMD_ENABLE, &dmac_status);
  if (ret != DMACA_SUCCESS) {
    while (1);  // Error
  }
  ret = R_DMACA_Control(DMACA_CH0, DMACA_CMD_SOFT_REQ_NOT_CLR_REQ, &dmac_status);
  if (ret != DMACA_SUCCESS) {
    while (1);  // Error
  }
  do {
    ret = R_DMACA_Control(DMACA_CH0, DMACA_CMD_STATUS_GET, &dmac_status);
    if (ret != DMACA_SUCCESS) {
      while (1);  // Error
    }
  } while((dmac_status.dtif_stat) == 0);
  ret = R_DMACA_Close(DMACA_CH0);
  if (ret != DMACA_SUCCESS) {
    while (1);  // Error
  }
  _WriteBufferIndex = IndexDst;
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCDCONF_GetBufferAddr
*/
void * LCDCONF_GetBufferAddr(void) {
  return _aBufferPTR[_WriteBufferIndex];
}

/*********************************************************************
*
*       LCDCONF_GetD2
*/
d2_device * LCDCONF_GetD2(void) {
  return d2_handle;
}

/*********************************************************************
*
*       LCDCONF_EnableDave2D
*/
void LCDCONF_EnableDave2D(void) {
  GUI_SetFuncDrawAlpha         (_DrawMemdevAlpha, _DrawBitmapAlpha);
  GUI_AA_SetFuncFillCircle     (_FillCircleAA);
  GUI_AA_SetFuncFillPolygon    (_FillPolygonAA);
  GUI_AA_SetFuncDrawCircle     (_DrawCircleAA);
  GUI_AA_SetFuncDrawLine       (_DrawLineAA);
  GUI_AA_SetFuncDrawPolyOutline(_DrawPolyOutlineAA);
  GUI_AA_SetFuncDrawArc        (_DrawArcAA);
  _DaveActive = 1;
}

/*********************************************************************
*
*       LCDCONF_DisableDave2D
*/
void LCDCONF_DisableDave2D(void) {
  GUI_SetFuncDrawAlpha         (NULL, NULL);
  GUI_AA_SetFuncFillCircle     (NULL);
  GUI_AA_SetFuncFillPolygon    (NULL);
  GUI_AA_SetFuncDrawCircle     (NULL);
  GUI_AA_SetFuncDrawLine       (NULL);
  GUI_AA_SetFuncDrawPolyOutline(NULL);
  GUI_AA_SetFuncDrawArc        (NULL);
  _DaveActive = 0;
}

/*********************************************************************
*
*       LCDCONF_DisableDave2D
*/
unsigned LCDCONF_GetDaveActive(void) {
  return _DaveActive;
}

/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {
  #if (NUM_BUFFERS > 1)
  GUI_MULTIBUF_ConfigEx(0, NUM_BUFFERS);
  #endif
  //
  // Set display driver and color conversion for 1st layer
  //
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Display driver configuration
  //
  if (LCD_GetSwapXYEx(0)) {
    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(0, YSIZE_PHYS, XSIZE_PHYS);
  } else {
    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS);
  }
  LCD_SetBufferPtrEx(0, _aBufferPTR);
  //
  // Initialize Dave2D
  //
  d2_handle = d2_opendevice(0);
  d2_inithw(d2_handle, 0);
  renderbuffer = d2_newrenderbuffer(d2_handle, 20, 20);
  //
  // Set function pointers
  //
  LCD_SetDevFunc(0, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))_CopyBuffer);
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;

  GUI_USE_PARA(LayerIndex);
  switch (Cmd) {
  //
  // Required
  //
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    _InitController();
    return 0;
  }
  case LCD_X_SETLUTENTRY: {
    //
    // Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
    //
    LCD_X_SETLUTENTRY_INFO * p;

    p = (LCD_X_SETLUTENTRY_INFO *)pData;
    _SetLUTEntry(p->Color, p->Pos);
    return 0;
  }
  case LCD_X_SHOWBUFFER: {
    //
    // Required if multiple buffers are used. The 'Index' element of p contains the buffer index.
    //
    LCD_X_SHOWBUFFER_INFO * p;

    p = (LCD_X_SHOWBUFFER_INFO *)pData;
#if (NUM_BUFFERS == 2)
    _SwitchBuffersOnVSYNC(p->Index);
#else
    _PendingBuffer = p->Index;
#endif
    break;
  }
  case LCD_X_ON: {
    //
    // Required if the display controller should support switching on and off
    //
    _DisplayOnOff(1);
    break;
  }
  case LCD_X_OFF: {
    //
    // Required if the display controller should support switching on and off
    //
    _DisplayOnOff(0);
    break;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/

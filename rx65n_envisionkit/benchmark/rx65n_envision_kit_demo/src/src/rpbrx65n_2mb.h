/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : rpbrx65n_2mb.h.h
* H/W Platform : RPBRX65N_2MB
* Description  : 
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version   Description
*         : 15.05.2017 1.00      First Release
***********************************************************************************************************************/

#ifndef RPBRX65N_2MB_H
#define RPBRX65N_2MB_H

/* Local defines */
#define LED_ON              (0)
#define LED_OFF             (1)
#define SET_BIT_HIGH        (1)
#define SET_BIT_LOW         (0)
#define SET_BYTE_HIGH       (0xFF)
#define SET_BYTE_LOW        (0x00)

/* Switches */
#define SW_ACTIVE           0
#define SW_NEGATIVE         1
#define SW2                 PORT0.PIDR.BIT.B5
#define SW3_JOY_UP          PORT6.PIDR.BIT.B7
#define SW3_JOY_DOWN        PORTF.PIDR.BIT.B5
#define SW3_JOY_LEFT        PORT1.PIDR.BIT.B3
#define SW3_JOY_RIGHT       PORT0.PIDR.BIT.B3
#define SW3_JOY_CENTRE      PORT1.PIDR.BIT.B2

#define SW2_PDR             PORT0.PDR.BIT.B5
#define SW3_JOY_UP_PDR      PORT6.PDR.BIT.B7
#define SW3_JOY_DOWN_PDR    PORTF.PDR.BIT.B5
#define SW3_JOY_LEFT_PDR    PORT1.PDR.BIT.B3
#define SW3_JOY_RIGHT_PDR   PORT0.PDR.BIT.B3
#define SW3_JOY_CENTRE_PDR  PORT1.PDR.BIT.B2

#define SW2_PMR             PORT0.PMR.BIT.B5
#define SW3_JOY_UP_PMR      PORT6.PMR.BIT.B7
#define SW3_JOY_DOWN_PMR    PORTF.PMR.BIT.B5
#define SW3_JOY_LEFT_PMR    PORT1.PMR.BIT.B3
#define SW3_JOY_RIGHT_PMR   PORT0.PMR.BIT.B3
#define SW3_JOY_CENTRE_PMR  PORT1.PMR.BIT.B2

#define SW2_PFS             MPC.P05PFS.BIT.ISEL
#define SW3_JOY_UP_PFS      MPC.P67PFS.BIT.ISEL
#define SW3_JOY_DOWN_PFS    MPC.PF5PFS.BIT.ISEL
#define SW3_JOY_LEFT_PFS    MPC.P13PFS.BIT.ISEL
#define SW3_JOY_RIGHT_PFS   MPC.P03PFS.BIT.ISEL
#define SW3_JOY_CENTRE_PFS  MPC.P12PFS.BIT.ISEL

/* LEDs */
#define LED2                PORT7.PODR.BIT.B0
#define LED2_PDR            PORT7.PDR.BIT.B0

#endif /* RPBRX65N_2MB_H */

/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
* File Name     : config_tcpudp.c
* Version       : 1.00
* Device(s)     : Renesas microcomputer
* Tool-Chain    : Renesas compilers
* OS            :
* H/W Platform  :
* Description   : T4 configuration file
* Limitations   :
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 30.08.2011 1.00    First release.
*               : 30.11.2016 1.01    DHCP relation add.
******************************************************************************/

#include "r_t4_itcpip.h"

/****************************************************************************/
/**********************     TCP-related definition     **********************/
/****************************************************************************/
/* Number of LAN port, Number of Serial port */
const UB _t4_channel_num = 1;
const UB _t4_dhcp_enable = 1;
const UH _t4_dhcp_ip_reply_arp_delay = 300;       /* unit:10ms */

/***  Definition of TCP reception point (only port number needs to be set) ***/
T_TCP_CREP tcp_crep[] =
{
    /* { attribute of reception point, {local IP address, local port number}} */
    { 0, { 0, 80 }},
    { 0, { 0, 80 }},
    { 0, { 0, 80 }},
    { 0, { 0, 80 }},
    { 0, { 0, 21 }},
    { 0, { 0, 20 }},
    { 0, { 0, 21 }},
    { 0, { 0, 20 }},
    { 0, { 0, 21 }},
    { 0, { 0, 20 }},
};

/* Total number of TCP reception points */
const H __tcprepn = sizeof(tcp_crep) / sizeof(T_TCP_CREP);

/***  Definition of TCP communication end point
      (only receive window size needs to be set) ***/

extern ER http_callback(ID cepid, FN fncd , VP p_parblk);
extern ER ftp_callback(ID cepid, FN fncd , VP p_parblk);
extern ER ftp_data_callback(ID cepid, FN fncd , VP p_parblk);

const T_TCP_CCEP tcp_ccep[] =
{
    /* { attribute of TCP communication end point,
         top address of transmit window buffer, size of transmit window buffer,
         top address of receive window buffer, size of receive window buffer,
         address of callback routine }
    */
    { 0, 0, 0, 0, 1460, http_callback },	/* for httpd */
    { 0, 0, 0, 0, 1460, http_callback },	/* for httpd */
    { 0, 0, 0, 0, 1460, http_callback },	/* for httpd */
    { 0, 0, 0, 0, 1460, http_callback },	/* for httpd */
    { 0, 0, 0, 0, 1460, ftp_callback },		/* for ftpd(command) */
    { 0, 0, 0, 0, 1460, ftp_data_callback },/* for ftpd(data) */
    { 0, 0, 0, 0, 1460, ftp_callback },		/* for ftpd(command) */
    { 0, 0, 0, 0, 1460, ftp_data_callback },/* for ftpd(data) */
    { 0, 0, 0, 0, 1460, ftp_callback },		/* for ftpd(command) */
    { 0, 0, 0, 0, 1460, ftp_data_callback },/* for ftpd(data) */
};

/* Total number of TCP communication end points */
const H __tcpcepn = sizeof(tcp_ccep) / sizeof(T_TCP_CCEP);

/***  TCP MSS  ***/
const UH _tcp_mss[] =
{
    /* MAX:1460 bytes */
    1460,
    1460,
};

/***  2MSL wait time (unit:10ms)  ***/
const UH    _tcp_2msl[] =
{
//    (1 * 60 * (1000 / 10)),      /* 1 min */
//    (1 * 60 * (1000 / 10)),      /* 1 min */
    (1),      /* 10 ms */
    (1),      /* 10 ms */
};
/***  Maximum value of retransmission timeout period (unit:10ms)  ***/
const UH    _tcp_rt_tmo_rst[] =
{
    (10 * 60 * (1000 / 10)),     /* 10 min */
    (10 * 60 * (1000 / 10)),     /* 10 min */
};
/***  Transmit for delay ack (ON=1/OFF=0) ***/
const UB   _tcp_dack[] = { 1, 1 };


/****************************************************************************/
/**********************     UDP-related definition     **********************/
/****************************************************************************/
/***  Definition of UDP communication end point  ***/
ER R_TCPIP_DnsClientCallback(ID cepid, FN fncd , VP p_parblk);
ER R_TCPIP_SntpClientCallback(ID cepid, FN fncd , VP p_parblk);
const T_UDP_CCEP udp_ccep[] =
{
    /* only setting port number */
    { 0, { 0, 53 }, R_TCPIP_DnsClientCallback },
    { 0, { 0, 123 }, R_TCPIP_SntpClientCallback },
};
/* Total number of UDP communication end points */
const H __udpcepn = (sizeof(udp_ccep) / sizeof(T_UDP_CCEP));

/***  TTL for multicast transmission  ***/
const UB __multi_TTL[] = { 1, 1 };

/*** Behavior of UDP zero checksum ***/
const UB _udp_enable_zerochecksum[] = { 0, 0 }; /* 0 = disable, other = enable */

/****************************************************************************/
/**********************  SYSTEM-callback definition   ***********************/
/****************************************************************************/

extern ER R_TCPIP_UserCallback(UB channel, UW eventid, VP param);
const callback_from_system_t g_fp_user = R_TCPIP_UserCallback;

/****************************************************************************/
/**********************     IP-related definition     ***********************/
/****************************************************************************/
const UH _ip_tblcnt[] = { 3, 3 };

#define MY_IP_ADDR0     192,168,0,3            /* Local IP address  */
#define GATEWAY_ADDR0   0,0,0,0                /* Gateway address (invalid if all 0s) */
#define SUBNET_MASK0    255,255,255,0          /* Subnet mask  */

#define MY_IP_ADDR1     192,168,0,10           /* Local IP address  */
#define GATEWAY_ADDR1   0,0,0,0                /* Gateway address (invalid if all 0s) */
#define SUBNET_MASK1    255,255,255,0          /* Subnet mask  */

TCPUDP_ENV tcpudp_env[] =
{
    {{MY_IP_ADDR0}, {SUBNET_MASK0}, {GATEWAY_ADDR0}},
    {{MY_IP_ADDR1}, {SUBNET_MASK1}, {GATEWAY_ADDR1}},
};

/****************************************************************************/
/**********************     Driver-related definition     *******************/
/****************************************************************************/
/*--------------------------------------------------------------------------*/
/*    Set of Ethernet-related                                               */
/*--------------------------------------------------------------------------*/
/* Local MAC address (Set all 0s when unspecified) */
#define MY_MAC_ADDR0    0x74,0x90,0x50,0x00,0x79,0x03
#define MY_MAC_ADDR1    0x74,0x90,0x50,0x00,0x79,0x04

UB _myethaddr[][6] =
{
    {MY_MAC_ADDR0},
    {MY_MAC_ADDR1},
};

/*--------------------------------------------------------------------------*/
/*    Set of PPP-related                                                    */
/*--------------------------------------------------------------------------*/
/* Authentication-related setting */
const UH ppp_auth  = AUTH_PAP;               /* PAP,No authentication enabled */
UB user_name[6]    = "abcde";                /* user name */
UB user_passwd[6]  = "abc00";                /* password  */

/* Dial up-related setting */
const UB peer_dial[]      = "0,123";                /* Destination telephone number */
const UB at_commands[]   = "ATW2S0=2&C0&D0&S0M0S10=255X3";    /* Modem initialization command */


/* Copyright 2014, RENESAS ELECTRONICS CORPORATION */


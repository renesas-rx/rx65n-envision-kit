//--------------------------------------------------------------------------
// Project: D/AVE
// File:    dave_base_nios.h (%version: 4 %)
//          created Mon Aug 22 12:50:45 2005 by hh04027
//
// Description:
//  %date_modified: Wed Apr 18 15:42:41 2007 %  (%derived_by:  hh74040 %)
//
// Changes:
//  2005-04-12 CSe  fully implemented missing d1 driver functionality
//
//--------------------------------------------------------------------------

#ifndef __1_dave_base_rx_h_H
#define __1_dave_base_rx_h_H

#define PRCR_KEY                (0xA500)
#define PRCR_PRC1               (0x0002)
#define MSTP_C                  (2)
#define DAVE2D_MSTP_NUM         (28)

#define MSTP_0_BASE             (0x00080010UL)
#define PRCR_0_BASE             (0x000803FEUL)
#define DAVE2D_0_BASE           (0x000E3000UL)
#define IR_0_BASE               (0x00087000UL)
#define IEN_0_BASE              (0x00087200UL)
#define IPR_0_BASE              (0x00087300UL)
#define GRP_0_BASE              (0x00087600UL)
#define GRPAL1_0_BASE           (0x00087834UL)
#define GENAL1_0_BASE           (0x00087874UL)

#define RX_USE_DLIST_INDIRECT   (1)

//--------------------------------------------------------------------------
//
#define WRITE_REG( BASE, OFFSET, DATA ) \
        (((unsigned long *)(BASE))[OFFSET] = DATA)

#define READ_REG( BASE, OFFSET ) \
        (((unsigned long *)(BASE))[OFFSET])

#define INTERNAL_NOT_USED(p)    ((void)(p))
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

typedef struct _d1_device_rx
{
    volatile long *dlist_start; /* dlist start addresses */
    int dlist_indirect;
} d1_device_rx;

//---------------------------------------------------------------------------

extern int d1_initirq_intern(d1_device_rx *handle);
extern int d1_shutdownirq_intern(d1_device_rx *handle);

//---------------------------------------------------------------------------

#define D2_DLISTSTART   (50)

#endif

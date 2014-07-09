/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: port.h,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#ifndef _PORT_H
#define _PORT_H

/***************************** Include Files *********************************/
#include <assert.h>
#include <inttypes.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* stm32 includes. */
#include "stm32f10x.h"
#include "stm32f10x_it.h"

/************************** Constant Definitions *****************************/

#if defined FLOW_CONTROL
    #define COM1
    #define DE_REn_PA11
    #define PV_PA8

#elif defined MB_BASE
    #define COM3
    #define DE_REn_PB12
    #define PV_PB13

#else
    #error "At Least One Board Type Must Defined!"
#endif

#define	INLINE                      inline
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }

#define ENTER_CRITICAL_SECTION( )   portENTER_CRITICAL()
#define EXIT_CRITICAL_SECTION( )    portEXIT_CRITICAL()

/**************************** Type Definitions *******************************/

typedef uint8_t         BOOL;

typedef unsigned char   UCHAR;
typedef char            CHAR;

typedef uint16_t        USHORT;
typedef int16_t         SHORT;

typedef uint32_t        ULONG;
typedef int32_t         LONG;

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

#endif

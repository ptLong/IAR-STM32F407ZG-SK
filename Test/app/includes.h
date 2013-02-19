/***************************************************************************
 **
 **
 **    Master include file
 **
 **    Used with ARM IAR C/C++ Compiler
 **
 **    (c) Copyright IAR Systems 2007
 **
 **    $Revision: 48478 $
 **
 ***************************************************************************/

#ifndef __INCLUDES_H
#define __INCLUDES_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <intrinsics.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>

#include "arm_comm.h"
#include "iar_stm32f407zg_sk.h"

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"


#include "drv_glcd_cnfg.h"
#include "drv_glcd.h"
#include "glcd_ll.h"
#include "accl_drv.h"
#include "i2c1_drv.h"
#include "iar_logo.h"
#include "stm32_logo.h"

#include "temp_sensor_drv.h"

#include "Sin_Table.h"
#endif  // __INCLUDES_H

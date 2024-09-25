## Included Example

### CLD_Example.m

The example connects to a CLD1010 laser diode driver, changes the temperature setpoint and turns on TEC output.

The code uses the C library DLL file TL4000_64.dll

With suitable modifications, the example code can also be used for the laser diode and temperature controllers ITC40x, TED40x and LDC2x, that use the same library. 

In order to avoid error messages, comment out the “__fastcall” and “signed” calling conventions in the header file “visatype.h”
in the folder: C:\Program Files\IVI Foundation\VISA\Win64\Include

```C
/*---------------------------------------------------------------------------*/
/* Distributed by IVI Foundation Inc. */
/* */
/* Do not modify the contents of this file. */
/*---------------------------------------------------------------------------*/
/* */
/* Title : VISATYPE.H */
/* Date : 05-12-2009 */
/* Purpose : Fundamental VISA data types and macro definitions */
/* */
/*---------------------------------------------------------------------------*/
#ifndef __VISATYPE_HEADER__
#define __VISATYPE_HEADER__
#if defined(_WIN64)
#define _VI_FAR
#define _VI_FUNC                     //__fastcall
#define _VI_FUNCC                    //__fastcall
#define _VI_FUNCH                    //__fastcall
#define _VI_SIGNED                   //signed
#elif (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)) &&
!defined(_NI_mswin16_)
#define _VI_FAR
.
.
.

```

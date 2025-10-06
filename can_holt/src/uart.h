/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : uart.h

  Abstract           : Diagnostic UART interface.

  Software Structure : SDD References: 136T-2200-132000-001-D22 SWDES-???

*************************************************************************/

#ifndef UART_H
#define UART_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

/***** Literals *********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Transmit a message */
void UartTransmitString(const Char_t * const buffer);

#endif /* UART_H */

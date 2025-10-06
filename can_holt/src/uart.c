/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : uart.c

  Abstract           : Diagnostic UART interface.

  Software Structure : SDD References: 136T-2200-132000-001-D22 SWDES-???

*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_string.h"
#include "soc/uart/d_uart.h"

#include "uart.h"

/***** Constants ********************************************************/

#define UART_CHANNEL  0u
/*********************************************************************//**
  <!-- UartTransmitString -->

  Transmit a zero terminated string.
*************************************************************************/
void                           /** \return None */
UartTransmitString
(
const Char_t * const buffer    /**< [in] Pointer to string to transmit */
)
{
  (void)d_UART_Transmit(UART_CHANNEL, (const Uint8_t *)buffer, d_GEN_StringLength(buffer, 80));
  
  return;
}


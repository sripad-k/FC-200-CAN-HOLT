/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Global

  Abstract           : This module groups together any global variables and extra utility functions required by
  	  	  	  	  	  multiple other modules.

  Software Structure : SDD References: 136B-9312-134100-001-D22

\note
  CSC ID             : SWDES-XXXX-TBD [DERIVED]
*************************************************************************/


#include "ETAS_global.h"									    // Own header
#include "soc/timer/d_timer.h"								// Required for delay in debug flush function
#include "uart.h"                           	// We wrap the driver print function for application layer to add cycle count
#include "kernel/general/d_gen_string.h"		  // Used by the print wrapping function
#include "soc/timer/d_timer.h"                // Used for the delay function
#include "sru/fcu/d_fcu.h"                    // For slot number function

/***** Constants ********************************************************/
// Temporary defines to test the proposed solutions
// #define used instead of static const because BUFFLEN is used in array size definition.
#define BUFFLEN ((Uint8_t)80)


/***** Type Definitions *************************************************/

/***** Variables ********************************************************/
static GlobalVariablesStructDef_t gVars; /* Global variable structure */
/* TODO. See how close we can get to a singleton on this structure. */

/***** CSC Function Declarations ****************************************/



/***** Function Definitions *********************************************/


/*********************************************************************//**
  <!-- GLOBAL_SerialPrint_Simple -->

  Simplified function that doesn't print any float or int values.
*************************************************************************/
void /** \return None*/
GLOBAL_SerialPrint_Simple
(
	const Char_t* const pTextMessage			/* < [in] text message to print.  Max 100 chars */
)
{
	(void)GLOBAL_SerialPrint(pTextMessage,0,0.0f);
}

/*********************************************************************//**
  <!-- GLOBAL_SerialPrint -->

  Wrapper for diagnostic message. Adds the current cycle counter to the message.
  Takes one optional integer and one optional float value.
  Optional in that the format %d, %x or %f will determine if it's used,
  not optional like c++ optional variables.
*************************************************************************/
void /** \return None*/
GLOBAL_SerialPrint
(
		const Char_t* const pTextMessage,		/* < [in] text message to print.  Max 100 chars */
		const Int32_t iValue,					/* < [in] Integer value to print. (replace %d or %x in pTextMessage) */
		const Float32_t fValue					/* < [in] Floating point value to print. (replace %f in pTextMessage) */
)
{
	Char_t tempBuffer[BUFFLEN];
	Char_t tempBuffer2[BUFFLEN];
	Char_t tempBuffer3[BUFFLEN];
	Int32_t errValue = 0;  // Overloaded variable.  If >0 it is the length of the string in the buffer.
	                       //                       If <0 it is an error code.

	errValue = d_GEN_SprintfInteger(tempBuffer,BUFFLEN,"[%d]",(Int32_t)gVars.cycleCounter);
	if(errValue >=0)
	{
	  Uint32_t lenValue = d_GEN_StringLength(pTextMessage, BUFFLEN);
	  if(lenValue > BUFFLEN) lenValue = BUFFLEN;   // An error code will return -1, but because we we have it as unsigned this will be 0xFFFFFFFF
		errValue = d_GEN_StringConcatenate(tempBuffer,BUFFLEN,pTextMessage,(Uint32_t)lenValue);
	}
	if(errValue >=0)
	{
		errValue = d_GEN_SprintfFloat(tempBuffer2, BUFFLEN, tempBuffer, fValue);
	}
	if(errValue >=0)
	{
		errValue = d_GEN_SprintfInteger(tempBuffer3, BUFFLEN, tempBuffer2, iValue);
	}
	if(errValue >=0)
	{
		UartTransmitString(tempBuffer3);
	}
	if(errValue < 0)
	{

	    DO_NOTHING();
	}
}

/*********************************************************************//**
  <!-- GLOBAL_FullSerialPrintFlush -->

  Delay required for the serial data to go out on the interface
  before adding additional data to the buffer.
*************************************************************************/
void  /** \return None */
GLOBAL_FullSerialPrintFlush
(
  Bool_t longDelayFlag /**< [in]  Flag setting long or short flush delay. */
)
{
  Uint32_t delay = 0;
  if(longDelayFlag == d_TRUE)
  {
    delay = 500u;
  }
  else
  {
    delay = 5u;
  }
  d_TIMER_DelayMilliseconds(delay);
}



/*********************************************************************//**
  <!-- GLOBAL_Initialise -->

  Initialise the global variables to a known default state.
*************************************************************************/
void	/** \return None */
GLOBAL_Initialise
(
	void  /** None */
)
{
#ifndef DEVKIT_ONLY
  Uint32_t slot = d_FCU_SlotNumber();
#else
  Uint32_t slot = 0; // FOR HOME DEVKIT
#endif
  gVars.slotNum = slot;

	for(Uint8_t i=0; i<DBSTRLEN; i++)
	{
		gVars.dbgOutputStr[i] = '\0';
	}
	gVars.cycleCounter = 0;
	gVars.testStopWatchdog = d_FALSE;
} /* End of function GLOBAL_Initialise */


/*********************************************************************//**
  <!-- GLOBAL_IncrementCycleCounter -->

  This function is called once per cycle by the main scheduler to increment
  the cycle counter. This is for reference and logging purposes.
  We use this instead of a SET function, because we don't want any other
  functions to access and set this counter.
*************************************************************************/
void	/** \return None */
GLOBAL_IncrementCycleCounter
(
	void  /** None */
)
{
	gVars.cycleCounter++;
} /* End of function GLOBAL_IncrementCycleCounter */

/*********************************************************************//**
  <!-- GLOBAL_GetCycleCounter -->

  This function is called once per cycle by the main scheduler to increment
  the cycle counter. This is for reference and logging purposes.
*************************************************************************/
Uint64_t	/** \return the current value of the cycle counter */
GLOBAL_GetCycleCounter
(
	void  /** None */
)
{
	return (gVars.cycleCounter);
} /* End of function GLOBAL_GetCycleCounter */



/*********************************************************************//**
  <!-- GLOBAL_getGlobalVars -->

  Access to the global variable structure.
  gVars is declared static, so there is only one copy, with SBC_global.c scope.
  For other files to access the global variables they make use of the GLOBAL_getGlobalVars()
  function and can then keep a local pointer.

  Would making an extern declaration in the header file achieve the same without the
  hassle of another function call and pointer to keep track off?

  Review this idea. This defeats the purpose of having access functions
  to individual variables.
*************************************************************************/
GlobalVariablesStructDef_t*
GLOBAL_GetGlobalVars
(
	void  /** None */
)
{
	return &gVars;
}


/*********************************************************************//**
  <!-- GLOBAL_ClearIFStructure -->

  Initialises the structure to default values.
*************************************************************************/
void                                           /** None */
GLOBAL_ClearIFStructure
(
    GlobalInterfaceHealthDef_t *pStruct,         /**< [out] Pointer to the structure to initialise.  */
  Uint32_t                   expectedCommsTime /**< [in]  Expected maximum time between valid messages */
)
{
  pStruct->commsStartedFlag       = d_FALSE;
  pStruct->commsOKFlag            = d_TRUE;  // Actually invalid until commsStartedFlag == TRUE
  pStruct->commsLossCounter       = 0;
  pStruct->maxCommsLossCounter    = 0;
  pStruct->expectedCommsLossValue = expectedCommsTime;    // Default 1 expected message each second.  So don't start flagging errors until
}


/*********************************************************************//**
  <!-- GLOBAL_UpdateIFStatus -->

  This function does the required checks to determine if an interface is
  experiencing an error or not.

  Essentially the comms loss counter is incremented every cycle that a valid
  message was not received on this interface. Not receiving a message is not
  automatically a failure. Only if the  number of missed cycles is larger than
  the expected counter a error is flagged.

  Until the first valid input is received on this interface it is assumed that
  it is working and d_TRUE will be returned. This is to ensure that any start
  up delays are not registered as faults.

*************************************************************************/
void                                           /** None */
GLOBAL_UpdateIFStatus
(
    GlobalInterfaceHealthDef_t *pStruct,   /**< [in/out] Pointer to the structure to work on.  */
    const Bool_t msgIn             /**< [in] Input flag indicating a valid message was received this cycle  */
)
{
  //Default interfaces are started up with the commStartedFlag set to d_FALSE.
  //The first valid message changes this flag to true, after which we can start
  //monitoring interface health.
    if((pStruct->commsStartedFlag == d_FALSE) && (msgIn == d_TRUE))
    {
      pStruct->commsStartedFlag = d_TRUE;
    }

    // Internal Interface Health calculations
    if(pStruct->commsStartedFlag == d_TRUE)
    {
      // If we didn't receive a valid message this cycle increment the commsLossCounter
      if(msgIn == d_FALSE)
      {
        pStruct->commsLossCounter++;
      }
      else
      {
        pStruct->commsOKFlag      = d_TRUE;
        pStruct->commsLossCounter = 0;
      }

      // Update the maximum comms loss counter value.
      if(pStruct->commsLossCounter > pStruct->maxCommsLossCounter)
      {
        pStruct->maxCommsLossCounter = pStruct->commsLossCounter;
      }

      // Set the comms OK flag to false
      if(pStruct->commsLossCounter > pStruct->expectedCommsLossValue)
      {
        pStruct->commsOKFlag = d_FALSE;
      }
    }
}


/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Global

  Abstract           : This module groups together any global variables and extra utility functions required by
  	  	  	  	  	  multiple other modules.

  Software Structure : SDD References: None. This function contains derived helper functionality.

\note
  CSC ID             : SWDES-XXXX-TBD [DERIVED]
*************************************************************************/

/* Note on naming convention

Filenames:
	Filenames and #ifndef will have a prefix showing the logical grouping.
	ETAS_  Is used for anything in the root /src folder.
	HKP_  Is used for anything related to src/housekeeping.
	CIF_  Is used for interface files in src/comms
	CMAN_ Is used for manager files in src/comms
	LOG_  Is used for files in src/datalog
	BIT_  Is used for files in src/health.  (Note this includes mission system health things, not just MC or SBC Bit)

Global functions in each file will have a prefix showing the CSC it belongs too, but won't also have the logical grouping prefix.
I.e.  SCHED_Initialise()  NOT   HKP_SCHED_Initialise()

Local static functions will just use an applicable function name.

 */


#ifndef ETAS_GLOBAL_H
#define ETAS_GLOBAL_H

/***** Includes *********************************************************/
/* Common used definitions from driver code. Include here once instead of repeating in each module header.*/
#include "soc/defines/d_common_status.h"
#include "soc/defines/d_common_types.h"

/***** Constants ********************************************************/
#define DBSTRLEN ((Uint8_t)(200))      // Compiles
//enum { DBSTRLEN = 200};			         // Compiles
//static const Uint8_t DBSTRLEN = 200; // Does NOT compile.   DBSTRLEN is used to declare the global structure buffer size.

static const Float32_t PI = 3.14159265359;

#define CONV_RAD2DEG_f64 ((Float64_t)360.0/(2.0*PI))
#define CONV_DEG2RAD_f64 ((Float64_t)(2.0*PI)/360.0)
#define CONV_RAD2DEG_f32 ((Float32_t)360.0f/(2.0f*PI))
#define CONV_DEG2RAD_f32 ((Float32_t)(2.0f*PI)/360.0f)

/* Allow decision coverage by always including and 'else' block */
#ifdef COVERAGE
  #define DO_NOTHING() __asm__ __volatile__("NOP")
#else
  #define DO_NOTHING()
#endif

/***** Type Definitions *************************************************/
#define class struct    // Define the keyword "class" to distinguish classes from structures.

typedef struct
{
  Uint32_t slotNum;
  Bool_t   iAmMaster;             // Is this the Master FCU as per Arbitration.  Determines control of the Discrete outputs */
  Uint64_t cycleCounter;          /* SW Counter that counts the elapsed number of 20ms cycles */
  Char_t dbgOutputStr[DBSTRLEN];  /* Debug output string */
  Bool_t testStopWatchdog;       /* By default the software strobes the watchdog every cycle to prevent a reset.
                                     For testing purposes we want to deliberately stop doing this to force a reset and test behaviour. */
}GlobalVariablesStructDef_t;      /* All global variables are available inside of a single global structure. */
                                  /* TODO. Need to think about this a bit, might be a bad idea.
                                   * Can't restrict access, for example prevent setting of the global counter.
                                   */


// Structure used to track cyclic health of an interface.
typedef struct
{
  Bool_t   commsStartedFlag;         // This flag is initialised to FALSE, and set to TRUE on the first received message.
  Bool_t   commsOKFlag;              // This flag indicates whether there is any comms issues.
  Uint32_t commsLossCounter;         // This is a counter used to indicate the number of consecutive cycles without any received messages. Resets once any message is received.
  Uint32_t maxCommsLossCounter;      // This counter keeps track of the maximum comms loss length experienced since start up.
  Uint32_t expectedCommsLossValue;   // This is the expected number of cycles without comms before it is flagged as a problem. For example a system transmitting once
                                     // per second will have 1 cycle with comms and 49 cycles without. It is only a problem once we go over 50 cycles without comms.
}GlobalInterfaceHealthDef_t;


typedef enum
{
  GLOBAL_DISPLAY_NONE = 0,
  GLOBAL_DISPLAY_MIN,
  GLOBAL_DISPLAY_ALL = 255
}GlobalDisplayLevelEnum_t;


/***** Variables ********************************************************/


/***** External Function Declarations ***********************************/

void GLOBAL_Initialise(void);
void GLOBAL_SerialPrint(const Char_t* const pTextMessage,const Int32_t iValue,const Float32_t fValue);  // TODO. Consider creating a wrapper function that takes in a display flag.
void GLOBAL_SerialPrint_Simple(const Char_t* const pTextMessage);
void GLOBAL_FullSerialPrintFlush(Bool_t longDelayFlag);
void GLOBAL_ClearIFStructure(GlobalInterfaceHealthDef_t *pStruct,Uint32_t expectedCommsTime);
void GLOBAL_UpdateIFStatus(GlobalInterfaceHealthDef_t *pStruct, const Bool_t msgIn);

/***** Variable Access Functions ****************************************/
GlobalVariablesStructDef_t *GLOBAL_GetGlobalVars(void);
void GLOBAL_IncrementCycleCounter(void);
Uint64_t GLOBAL_GetCycleCounter(void);

#endif /* ETAS_GLOBAL_H */


/*******************************************************************************
Copyright (c) 2022 Incomar Group (Pty) Ltd.

All rights reserved.
The software shall not be copied, modified or distributed without the express
permission of Incomar.

********************************************************** [Incomar Group] ****/

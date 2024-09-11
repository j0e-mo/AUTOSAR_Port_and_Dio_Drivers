 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Yousouf Soliman
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H


/*******************************************************************************
 *                             Inclusions                                      *
 *******************************************************************************/
#include "Port_Types.h"
#include "Port_Cfg.h"
/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif

#include "Port_Reg.h"

#include "Common_Macros.h"

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* Invalid Port Pin ID requested */
#define PORT_E_PARAM_PIN                    (uint8)0x0A

/* Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE       (uint8)0x0B

/* API Port_Init service called with wrong parameter */
#define PORT_E_PARAM_CONFIG                 (uint8)0x0C

/* API Port_SetPinMode service called when Port Pin Mode passed not valid */
#define PORT_E_PARAM_INVALID_MODE           (uint8)0x0D

/* API Port_SetPinMode service called when mode is unchangeable */
#define PORT_E_MODE_UNCHANGEABLE            (uint8)0x0E

/* API service called without module initialization */
#define PORT_E_UNINIT                       (uint8)0x0F

/* APIs called with a Null Pointer */
#define PORT_E_PARAM_POINTER                (uint8)0x10



/* Array of Port_ConfigPin */
typedef struct
{
    Port_ConfigPin Pin[PORT_NUMBER_OF_PORT_PINS];
}Port_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/************************************************************************************
* Service Name: Port_Init
*
* Sync/Async: Synchronous
* Reentrancy: non-reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the Port Driver module.
************************************************************************************/
void Port_Init (
                 const Port_ConfigType* ConfigPtr
                )
;

#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
/************************************************************************************
* Service Name: Port_SetPinDirection
* Service ID: 0x01
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): - Pin: Port Pin ID number
                   - Direction: Port Pin Direction
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Set the port pin direction.
*
************************************************************************************/
void Port_SetPinDirection (
                            Port_PinType Pin,
                            Port_PinDirectionType Direction
                           )
;
#endif


/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Service ID: 0x02
* Sync/Async: Synchronous
* Reentrancy: non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to refresh the port direction
*
************************************************************************************/
void Port_RefreshPortDirection (
void
)
;

#if (PORT_VERSION_INFO_API == STD_ON)
/************************************************************************************
* Service Name: Port_GetVersionInfo
* Service ID: 0x03
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): versioninfo - Pointer to where to store the version information of this module.
* Return value: None
* Description: Returns the version information of this module.
************************************************************************************/

void Port_GetVersionInfo(
        Std_VersionInfoType* versioninfo
        )
;
#endif


#if (PORT_SET_PIN_MODE_API == STD_ON)
/************************************************************************************
* Service Name: Port_SetPinMode
* Service ID: 0x04
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pin - Port Pin ID number, Mode - New Port Pin mode to be set on port pin
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode.
************************************************************************************/

void Port_SetPinMode(
        Port_PinType Pin,
        Port_PinModeType Mode
        )
;
#endif


/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Post build structure used with Port_Init API */
extern const Port_ConfigType Port_PinConfig;

#endif /* PORT_H */

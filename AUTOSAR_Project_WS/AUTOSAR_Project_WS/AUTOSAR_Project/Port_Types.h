/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Types.h
 *
 * Description: Custom Port Module Types Header File for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Yousouf Soliman
 ******************************************************************************/

#ifndef PORT_TYPES_H_
#define PORT_TYPES_H_

/*******************************************************************************
 *                              Module Definitions                             *
 *******************************************************************************/

/*
 * Id for the company in the AUTOSAR
 *  */
#define PORT_VENDOR_ID    (1234U)

/* Port Module Id */
#define PORT_MODULE_ID    (124U)

/* Port Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Port Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)


/*
 * Number of modes available in Target HW
 * */
#define NUMBER_OF_MODES                  (11U)
/*******************************************************************************
 *                              Inclusions                                     *
 *******************************************************************************/
#include "Std_Types.h"
/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for Port_Init */
#define PORT_INIT_SID                       (uint8)0x00

/* Service ID for Port_SetPinDirection */
#define PORT_SET_PIN_DIR_SID                (uint8)0x01

/* Service ID for Port_RefreshPortDirection */
#define PORT_REFRESH_PORT_DIR_SID           (uint8)0x02

/* Service ID for Port_GetVersionInfo */
#define PORT_GET_VERSION_INFO_SID           (uint8)0x03

/* Service ID for Port_SetPinMode */
#define PORT_SET_PIN_MODE_SID               (uint8)0x04

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/* Description: Enum to hold PIN direction */
typedef enum
{
    PORT_PIN_IN, PORT_PIN_OUT
} Port_PinDirectionType;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF, PULL_UP, PULL_DOWN
} Port_InternalResistorType;

/* Description: Type to hold the number of a specific port
 * For the target HW there are 6 available port (A-F)
 * uint8 range (0-255)  */
typedef uint8 Port_PortType;

/* Description: Type to hold the number of a pin in a specific port
 * For the target HW the maximum number of pins in a port is 8
 * And the total number of pins is 43
 * uint8 range (0-255)  */
typedef uint8 Port_PinType;


/* Description: Port Pin Level value from Port pin list */
typedef enum
{
    PORT_PIN_LEVEL_LOW, PORT_PIN_LEVEL_HIGH
} Port_PinLevelValue;

/* Description: Type to hold the number an alternative operation mode
 * For the target HW there are 11 available modes
 * uint8 range (0-255)  */
typedef enum
{
    GPIO_MODE,
    ALT1,
    ALT2,
    ALT3,
    ALT4,
    ALT5,
    ALT6,
    ALT7,
    ALT8,
    ALT9,
    ALT10 = 14,
    ALT11 = 15,
    ANALOG
}Port_PinModeType;

/* Description: Structure to configure each individual PIN:
 *  1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
 *  2. the number of the pin in the PORT.
 *  3. the direction of pin --> INPUT or OUTPUT
 *  4. the internal resistor --> Disable, Pull up or Pull down
 *  5. initial port pin value (high, low)
 *  6. initial port pin mode  (ADC, DIO, ..)
 *  7. pin direction changeable (true, false)
 *  8. pin mode changeable (true, false)
 */
typedef struct
{
    Port_PortType port_num;
    Port_PinType pin_num;
    Port_PinDirectionType direction;
    Port_InternalResistorType resistor;
    Port_PinModeType mode;
    Port_PinLevelValue initial_value;
    uint8 pin_dir_changeable;
    uint8 pin_mode_changeable;
} Port_ConfigPin;

#endif /* PORT_TYPES_H_ */

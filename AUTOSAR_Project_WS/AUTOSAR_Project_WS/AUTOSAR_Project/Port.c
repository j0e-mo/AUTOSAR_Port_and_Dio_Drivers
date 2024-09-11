/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Yousouf Soliman
 ******************************************************************************/

/* Module Includes */
#include "Port.h"
#include "Port_Reg.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Det.h does not match the expected version"
#endif

#endif

/* Holds the status of the Port
 * options:     PORT_NOT_INITIALIZED
 *              PORT_INITIALIZED        ( set by Port_Init() )
 */
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;

/* Holds the pointer of the Port_PinConfig */
STATIC const Port_ConfigType *Port_ConfigPtr = NULL_PTR;

/* Contains the allowed modes for each pin */
Port_PinModeType Port_AllowedModes[NUMBER_OF_MODES];

/************************************************************************************************************************************************************************************************************************************************************
 *                                                                                                                                                                                                                                                          *
 *************************************************************************************************************************************************************************************************************************************************************/

/************************************************************************************
 * Service Name: Port_Init
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters (in): ConfigPtr - Pointer to configuration set
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to Initialize the Port Driver module.
 ************************************************************************************/
void Port_Init(const Port_ConfigType *ConfigPtr)
{
    /* Loop counter to check on all configured pins */
    Port_PinType counter = 0;

    /* Index counter to deal with allowed mode array Port_AllowedModes[i] */
    uint8 i = 0;

    /* Flag to indicate if a mode is allowed for a pin or not (POSITIVE LOGIC) */
    boolean b_AllowedModeMatched = FALSE;


#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the input configuration pointer is not a NULL_PTR */
    if (ConfigPtr == NULL_PTR)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
        PORT_E_PARAM_CONFIG);
    }
    else
    { /* Do Nothing */
    }
#endif

    /* When this function is called, it means that the Port is now initialized */
    Port_Status = PORT_INITIALIZED;

    /* For easier, more understandable code */
    Port_ConfigPtr = ConfigPtr;


    /* Loop to do the following to all the configured pins in Port_PBcfg.c file:
     * 1- Detect the corresponding port
     * 2- Detect the corresponding pin
     * 3- Set the Port_AllowedModes[] array to the allowed modes for this pin according to Target HW specification
     * 4- Enable clock for the corresponding port
     * 5- Configure the pin to according to Port_ConfigPtr
     *      A- Direction
     *      B- Resistance
     *      C- Mode
     *      D- Initial Value
     *  */
    for (counter = 0; counter < PORT_NUMBER_OF_PORT_PINS; counter++)
    {

        volatile uint32 *PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
        volatile uint32 delay = 0;
        /* Clear the Port_AllowedModes[i] container */
        for (i = 0; i < NUMBER_OF_MODES; i++)
        {
            Port_AllowedModes[i] = GPIO_MODE;
        }
        /* Clear the array index for next use */
        i = 0;
        switch (Port_ConfigPtr->Pin[counter].port_num) /* get the corresponding port */
        {
        case 0:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
            switch (Port_ConfigPtr->Pin[counter].pin_num) /* get the corresponding pin */
            {
            /* add the allowed mode to the Port_AllowedModes[] array */
            case 0:
            case 1:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT8;
                break;
            case 2:
            case 3:
            case 4:
            case 5:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT2;
                break;
            case 6:
            case 7:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT3;
                Port_AllowedModes[2] = ALT5;
                break;
            }
            break;
        case 1:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
            switch (Port_ConfigPtr->Pin[counter].pin_num)
            {
            case 0:
            case 1:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT7;
                Port_AllowedModes[3] = ANALOG;
                break;
            case 2:
            case 3:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT3;
                Port_AllowedModes[2] = ALT7;
                break;
            case 4:
            case 5:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT2;
                Port_AllowedModes[2] = ALT4;
                Port_AllowedModes[3] = ALT7;
                Port_AllowedModes[4] = ALT8;
                Port_AllowedModes[5] = ANALOG;
                break;
            case 6:
            case 7:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT2;
                Port_AllowedModes[2] = ALT4;
                Port_AllowedModes[3] = ALT7;
                break;
            }
            break;
        case 2:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
            switch (Port_ConfigPtr->Pin[counter].pin_num)
            {
            case 0:
            case 1:
            case 2:
            case 3:
                break;
            case 4:
            case 5:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT2;
                Port_AllowedModes[3] = ALT4;
                Port_AllowedModes[4] = ALT6;
                Port_AllowedModes[5] = ALT7;
                Port_AllowedModes[6] = ALT8;
                Port_AllowedModes[7] = ANALOG;
                break;
            case 6:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT6;
                Port_AllowedModes[3] = ALT7;
                Port_AllowedModes[4] = ALT8;
                Port_AllowedModes[5] = ANALOG;
                break;
            case 7:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT7;
                Port_AllowedModes[3] = ALT8;
                Port_AllowedModes[4] = ANALOG;
                break;
            }
            break;
        case 3:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
            switch (Port_ConfigPtr->Pin[counter].pin_num)
            {
            case 0:
            case 1:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT2;
                Port_AllowedModes[3] = ALT3;
                Port_AllowedModes[4] = ALT4;
                Port_AllowedModes[5] = ALT5;
                Port_AllowedModes[6] = ALT7;
                Port_AllowedModes[7] = ANALOG;
                break;
            case 2:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT2;
                Port_AllowedModes[3] = ALT4;
                Port_AllowedModes[4] = ALT7;
                Port_AllowedModes[5] = ALT8;
                Port_AllowedModes[6] = ANALOG;
                break;
            case 3:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT2;
                Port_AllowedModes[3] = ALT6;
                Port_AllowedModes[4] = ALT7;
                Port_AllowedModes[5] = ALT8;
                Port_AllowedModes[6] = ANALOG;
                break;
            case 4:
            case 5:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT7;
                Port_AllowedModes[3] = ANALOG;
                break;
            case 6:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT4;
                Port_AllowedModes[3] = ALT6;
                Port_AllowedModes[4] = ALT7;
                break;
            case 7:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT6;
                Port_AllowedModes[3] = ALT7;
                Port_AllowedModes[4] = ALT8;
                break;
            }
            break;
        case 4:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
            switch (Port_ConfigPtr->Pin[counter].pin_num)
            {
            case 0:
            case 1:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ANALOG;
                break;
            case 2:
            case 3:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ANALOG;
                break;
            case 4:
            case 5:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT3;
                Port_AllowedModes[3] = ALT4;
                Port_AllowedModes[4] = ALT5;
                Port_AllowedModes[5] = ALT8;
                Port_AllowedModes[6] = ANALOG;
                break;
            }
            break;
        case 5:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
            switch (Port_ConfigPtr->Pin[counter].pin_num)
            {
            case 0:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT2;
                Port_AllowedModes[3] = ALT3;
                Port_AllowedModes[4] = ALT5;
                Port_AllowedModes[5] = ALT6;
                Port_AllowedModes[6] = ALT7;
                Port_AllowedModes[7] = ALT8;
                Port_AllowedModes[8] = ALT9;
                break;
            case 1:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT1;
                Port_AllowedModes[2] = ALT2;
                Port_AllowedModes[3] = ALT5;
                Port_AllowedModes[4] = ALT6;
                Port_AllowedModes[5] = ALT7;
                Port_AllowedModes[6] = ALT9;
                Port_AllowedModes[7] = ALT10;
                break;
            case 2:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT2;
                Port_AllowedModes[2] = ALT4;
                Port_AllowedModes[3] = ALT5;
                Port_AllowedModes[4] = ALT7;
                Port_AllowedModes[5] = ALT10;
                break;
            case 3:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT2;
                Port_AllowedModes[2] = ALT3;
                Port_AllowedModes[3] = ALT5;
                Port_AllowedModes[4] = ALT7;
                Port_AllowedModes[5] = ALT10;
                break;
            case 4:
                Port_AllowedModes[0] = GPIO_MODE;
                Port_AllowedModes[1] = ALT5;
                Port_AllowedModes[2] = ALT6;
                Port_AllowedModes[3] = ALT7;
                Port_AllowedModes[4] = ALT8;
                break;
            }
            break;
        }

        /* Enable clock for PORT and allow time for clock to start*/
        SYSCTL_REGCGC2_REG |= (1 << Port_ConfigPtr->Pin[counter].port_num);
        delay = SYSCTL_REGCGC2_REG;

        if (((Port_ConfigPtr->Pin[counter].port_num == 3)
                && (Port_ConfigPtr->Pin[counter].pin_num == 7))
                || ((Port_ConfigPtr->Pin[counter].port_num == 5)
                        && (Port_ConfigPtr->Pin[counter].pin_num == 0))) /* PD7 or PF0 */
        {
            /* Unlock the GPIOCR register */
            *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                    + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;

            /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET),
                    Port_ConfigPtr->Pin[counter].pin_num);
        }
        else if ((Port_ConfigPtr->Pin[counter].port_num == 2)
                && (Port_ConfigPtr->Pin[counter].pin_num <= 3)) /* PC0 to PC3 */
        {
            /* Do Nothing ... JTAG pins */
            continue;
        }
        else
        {
            /* Do Nothing ... No need to unlock the commit register for this pin */
        }

        for (i = 0; i < NUMBER_OF_MODES; i++)
        {
            if (Port_ConfigPtr->Pin[counter].mode == Port_AllowedModes[i])
            {
                b_AllowedModeMatched = TRUE;
            }
        }

        if (b_AllowedModeMatched == FALSE)
        {
            //#error "This mode cannot be selected for this pin"
        }
        else if ((Port_ConfigPtr->Pin[counter].port_num == 2)
                && (Port_ConfigPtr->Pin[counter].pin_num <= 3)) /* PC0 to PC3 */
        {
            // #error "Cannot configure JTAG pins to a different mode"
        }
        else
        {
            if (Port_ConfigPtr->Pin[counter].mode == GPIO_MODE)
            {
                /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);

                /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);

                /* Clear the PMCx bits for this pin */
                *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                        + PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                        << (Port_ConfigPtr->Pin[counter].pin_num * 4));

                /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);
            }
            else if (Port_ConfigPtr->Pin[counter].mode == ANALOG)
            {
                /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);

                /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);

                /* Clear the PMCx bits for this pin */
                *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                        + PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                        << (Port_ConfigPtr->Pin[counter].pin_num * 4));

                /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);
            }
            else /* Another mode */
            {
                /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);

                /* Enable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);

                /* Set the PMCx bits for this pin */
                *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                        + PORT_CTL_REG_OFFSET) |=
                        (Port_ConfigPtr->Pin[counter].mode
                                & 0x0000000F
                                        << (Port_ConfigPtr->Pin[counter].pin_num
                                                * 4));

                /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);
            }

        }

        if (Port_ConfigPtr->Pin[counter].direction == PORT_PIN_OUT)
        {
            /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),
                    Port_ConfigPtr->Pin[counter].pin_num);

            if (Port_ConfigPtr->Pin[counter].initial_value
                    == PORT_PIN_LEVEL_HIGH)
            {
                /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);
            }
            else
            {
                /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);
            }
        }
        else if (Port_ConfigPtr->Pin[counter].direction == PORT_PIN_IN)
        {
            /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
            CLEAR_BIT(
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),
                    Port_ConfigPtr->Pin[counter].pin_num);

            if (Port_ConfigPtr->Pin[counter].resistor == PULL_UP)
            {
                /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);
            }
            else if (Port_ConfigPtr->Pin[counter].resistor == PULL_DOWN)
            {
                /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);
            }
            else
            {
                /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);

                /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);
            }
        }
        else
        {
            /* Do Nothing */
        }
    }
}

/************************************************************************************
 * Service Name: Port_SetPinDirection
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): Pin - Port Pin ID number , Direction - Port Pin Direction
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to Sets the port pin direction.
 ************************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if the Driver is initialized before using this function */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID,
        PORT_E_UNINIT);
    }
    else
    { /* Do Nothing */
    }

    /* check if incorrect Port Pin ID passed */
    if (Pin >= PORT_NUMBER_OF_PORT_PINS)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID,
        PORT_E_PARAM_PIN);
    }
    else
    { /* Do Nothing */
    }

    /* check if Port Pin not configured as changeable */
    if (Port_ConfigPtr->Pin[Pin].pin_dir_changeable == STD_OFF)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID,
        PORT_E_DIRECTION_UNCHANGEABLE);
    }
    else
    { /* Do Nothing */
    }
#endif

    volatile uint32 *PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */

    switch (Port_ConfigPtr->Pin[Pin].port_num)
    {
    case 0:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
        break;
    case 1:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
        break;
    case 2:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
        break;
    case 3:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
        break;
    case 4:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
        break;
    case 5:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
        break;
    }

    if ((Port_ConfigPtr->Pin[Pin].port_num == 2)
            && (Port_ConfigPtr->Pin[Pin].pin_num <= 3)) /* PC0 to PC3 */
    {
        /* Do Nothing ...  this is the JTAG pins */
        return;
    }

    if (Direction == PORT_PIN_OUT)
    {
        /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),
                Port_ConfigPtr->Pin[Pin].pin_num);
    }
    else if (Direction == PORT_PIN_IN)
    {
        /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
        CLEAR_BIT(
                *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),
                Port_ConfigPtr->Pin[Pin].pin_num);
    }
    else
    { /* Do Nothing */
    }
}
#endif

/************************************************************************************
 * Service Name: Port_RefreshPortDirection
 * Service ID: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Refreshes port direction.
 ************************************************************************************/
void Port_RefreshPortDirection(void)
{
    Port_PinType counter = 0;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if the Driver is initialized before using this function */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
        PORT_REFRESH_PORT_DIR_SID,
                        PORT_E_UNINIT);
    }
    else
    { /* Do Nothing */
    }
#endif

    for (counter = 0; counter < PORT_NUMBER_OF_PORT_PINS; counter++)
    {
        volatile uint32 *PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */

        switch (Port_ConfigPtr->Pin[counter].port_num)
        {
        case 0:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
            break;
        case 1:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
            break;
        case 2:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
            break;
        case 3:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
            break;
        case 4:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
            break;
        case 5:
            PortGpio_Ptr = (volatile uint32*) GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
            break;
        }

        if ((Port_ConfigPtr->Pin[counter].port_num == 2)
                && (Port_ConfigPtr->Pin[counter].pin_num <= 3)) /* PC0 to PC3 */
        {
            /* Do Nothing ...  JTAG pins */
            continue;
        }

        /* PORT061: The function Port_RefreshPortDirection shall exclude those port pins from
         * refreshing that are configured as pin direction changeable during runtime */
        if (Port_ConfigPtr->Pin[counter].pin_dir_changeable == STD_OFF)
        {
            if (Port_ConfigPtr->Pin[counter].direction == PORT_PIN_OUT)
            {
                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);
            }
            else if (Port_ConfigPtr->Pin[counter].direction == PORT_PIN_IN)
            {
                /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                CLEAR_BIT(
                        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),
                        Port_ConfigPtr->Pin[counter].pin_num);
            }
            else
            { /* Do Nothing */
            }
        }
        else
        { /* Do Nothing */
        }
    }
}

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
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the input configuration pointer is not a NULL_PTR */
    if (versioninfo == NULL_PTR)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
        PORT_GET_VERSION_INFO_SID,
                        PORT_E_PARAM_POINTER);
    }
    else
    { /* Do Nothing */
    }

    /* Check if the Driver is initialized before using this function */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
        PORT_GET_VERSION_INFO_SID,
                        PORT_E_UNINIT);
    }
    else
    { /* Do Nothing */
    }
#endif

    /* Copy the module Id */
    versioninfo->moduleID = (uint16) PORT_MODULE_ID;
    /* Copy the vendor Id */
    versioninfo->vendorID = (uint16) PORT_VENDOR_ID;
    /* Copy Software Major Version */
    versioninfo->sw_major_version = (uint8) PORT_SW_MAJOR_VERSION;
    /* Copy Software Minor Version */
    versioninfo->sw_minor_version = (uint8) PORT_SW_MINOR_VERSION;
    /* Copy Software Patch Version */
    versioninfo->sw_patch_version = (uint8) PORT_SW_PATCH_VERSION;
}
#endif

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
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    boolean b_AllowedModeMatched = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if the Driver is initialized before using this function */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
        PORT_E_UNINIT);
    }
    else
    {
        /* Do Nothing */
    }

    /* check if incorrect Port Pin ID passed */
    if (Pin >= PORT_NUMBER_OF_PORT_PINS)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
        PORT_E_PARAM_PIN);
    }
    else
    {
        /* Do Nothing */
    }

    /* check if the API called when the mode is unchangeable */
    if (Port_ConfigPtr->Pin[Pin].pin_mode_changeable == STD_OFF)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
        PORT_E_MODE_UNCHANGEABLE);
    }
    else
    {
        /* Do Nothing */
    }
#endif

    volatile uint32 *PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */

    /* Index to be used with Port_AllowedModes[i] */
    uint8 i = 0;


    /* Assure that the Port_AllowedModes[i] container is cleared */
    for (i = 0; i < NUMBER_OF_MODES; i++)
    {
        Port_AllowedModes[i] = GPIO_MODE;
    }
    /* Clear the array index for next use */
    i = 0;

    switch (Port_ConfigPtr->Pin[Pin].port_num) /* get the corresponding port number */
    {
    case 0:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
        switch (Port_ConfigPtr->Pin[Pin].pin_num) /* get the corresponding pin number */
        {
        /* add the allowed mode to the Port_AllowedModes[] array */
        case 0:
        case 1:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT8;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT2;
            break;
        case 6:
        case 7:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT3;
            Port_AllowedModes[2] = ALT5;
            break;
        }
        break;
    case 1:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
        switch (Port_ConfigPtr->Pin[Pin].pin_num)
        {
        case 0:
        case 1:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT7;
            Port_AllowedModes[3] = ANALOG;
            break;
        case 2:
        case 3:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT3;
            Port_AllowedModes[2] = ALT7;
            break;
        case 4:
        case 5:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT2;
            Port_AllowedModes[2] = ALT4;
            Port_AllowedModes[3] = ALT7;
            Port_AllowedModes[4] = ALT8;
            Port_AllowedModes[5] = ANALOG;
            break;
        case 6:
        case 7:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT2;
            Port_AllowedModes[2] = ALT4;
            Port_AllowedModes[3] = ALT7;
            break;
        }
        break;
    case 2:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
        switch (Port_ConfigPtr->Pin[Pin].pin_num)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            break;
        case 4:
        case 5:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT2;
            Port_AllowedModes[3] = ALT4;
            Port_AllowedModes[4] = ALT6;
            Port_AllowedModes[5] = ALT7;
            Port_AllowedModes[6] = ALT8;
            Port_AllowedModes[7] = ANALOG;
            break;
        case 6:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT6;
            Port_AllowedModes[3] = ALT7;
            Port_AllowedModes[4] = ALT8;
            Port_AllowedModes[5] = ANALOG;
            break;
        case 7:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT7;
            Port_AllowedModes[3] = ALT8;
            Port_AllowedModes[4] = ANALOG;
            break;
        }
        break;
    case 3:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
        switch (Port_ConfigPtr->Pin[Pin].pin_num)
        {
        case 0:
        case 1:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT2;
            Port_AllowedModes[3] = ALT3;
            Port_AllowedModes[4] = ALT4;
            Port_AllowedModes[5] = ALT5;
            Port_AllowedModes[6] = ALT7;
            Port_AllowedModes[7] = ANALOG;
            break;
        case 2:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT2;
            Port_AllowedModes[3] = ALT4;
            Port_AllowedModes[4] = ALT7;
            Port_AllowedModes[5] = ALT8;
            Port_AllowedModes[6] = ANALOG;
            break;
        case 3:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT2;
            Port_AllowedModes[3] = ALT6;
            Port_AllowedModes[4] = ALT7;
            Port_AllowedModes[5] = ALT8;
            Port_AllowedModes[6] = ANALOG;
            break;
        case 4:
        case 5:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT7;
            Port_AllowedModes[3] = ANALOG;
            break;
        case 6:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT4;
            Port_AllowedModes[3] = ALT6;
            Port_AllowedModes[4] = ALT7;
            break;
        case 7:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT6;
            Port_AllowedModes[3] = ALT7;
            Port_AllowedModes[4] = ALT8;
            break;
        }
        break;
    case 4:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
        switch (Port_ConfigPtr->Pin[Pin].pin_num)
        {
        case 0:
        case 1:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ANALOG;
            break;
        case 2:
        case 3:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ANALOG;
            break;
        case 4:
        case 5:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT3;
            Port_AllowedModes[3] = ALT4;
            Port_AllowedModes[4] = ALT5;
            Port_AllowedModes[5] = ALT8;
            Port_AllowedModes[6] = ANALOG;
            break;
        }
        break;
    case 5:
        PortGpio_Ptr = (volatile uint32*) GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
        switch (Port_ConfigPtr->Pin[Pin].pin_num)
        {
        case 0:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT2;
            Port_AllowedModes[3] = ALT3;
            Port_AllowedModes[4] = ALT5;
            Port_AllowedModes[5] = ALT6;
            Port_AllowedModes[6] = ALT7;
            Port_AllowedModes[7] = ALT8;
            Port_AllowedModes[8] = ALT9;
            break;
        case 1:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT1;
            Port_AllowedModes[2] = ALT2;
            Port_AllowedModes[3] = ALT5;
            Port_AllowedModes[4] = ALT6;
            Port_AllowedModes[5] = ALT7;
            Port_AllowedModes[6] = ALT9;
            Port_AllowedModes[7] = ALT10;
            break;
        case 2:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT2;
            Port_AllowedModes[2] = ALT4;
            Port_AllowedModes[3] = ALT5;
            Port_AllowedModes[4] = ALT7;
            Port_AllowedModes[5] = ALT10;
            break;
        case 3:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT2;
            Port_AllowedModes[2] = ALT3;
            Port_AllowedModes[3] = ALT5;
            Port_AllowedModes[4] = ALT7;
            Port_AllowedModes[5] = ALT10;
            break;
        case 4:
            Port_AllowedModes[0] = GPIO_MODE;
            Port_AllowedModes[1] = ALT5;
            Port_AllowedModes[2] = ALT6;
            Port_AllowedModes[3] = ALT7;
            Port_AllowedModes[4] = ALT8;
            break;
        }
        break;
    }

    if ((Port_ConfigPtr->Pin[Pin].port_num == 2)
            && (Port_ConfigPtr->Pin[Pin].pin_num <= 3)) /* PC0 to PC3 */
    {
        /* Do Nothing ...  this is the JTAG pins */
        return;
    }

    /* Check if the specified mode is applicable to the selected pin */
    for (i = 0; i < NUMBER_OF_MODES; i++)
    {
        if (Port_ConfigPtr->Pin[Pin].mode == Port_AllowedModes[i])
        {
            b_AllowedModeMatched = TRUE;
        }
    }

    if (b_AllowedModeMatched == FALSE)
    {
        /* Report a Det error if the mode is inapplicable to the specified pin */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
        PORT_E_PARAM_INVALID_MODE);
    }
    else
    {

        if (Mode == GPIO_MODE)
        {
            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),
                    Port_ConfigPtr->Pin[Pin].pin_num);

            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET),
                    Port_ConfigPtr->Pin[Pin].pin_num);

            /* Clear the PMCx bits for this pin */
            *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                    + PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                    << (Port_ConfigPtr->Pin[Pin].pin_num * 4));

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),
                    Port_ConfigPtr->Pin[Pin].pin_num);
        }
        else if (Mode == ANALOG)
        {
            /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
            CLEAR_BIT(
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),
                    Port_ConfigPtr->Pin[Pin].pin_num);

            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET),
                    Port_ConfigPtr->Pin[Pin].pin_num);

            /* Clear the PMCx bits for this pin */
            *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                    + PORT_CTL_REG_OFFSET) &= ~(0x0000000F
                    << (Port_ConfigPtr->Pin[Pin].pin_num * 4));

            /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),
                    Port_ConfigPtr->Pin[Pin].pin_num);
        }
        else /* Another mode */
        {
            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),
                    Port_ConfigPtr->Pin[Pin].pin_num);

            /* Enable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET),
                    Port_ConfigPtr->Pin[Pin].pin_num);

            /* Set the PMCx bits for this pin */
            *(volatile uint32*) ((volatile uint8*) PortGpio_Ptr
                    + PORT_CTL_REG_OFFSET) |= (Mode
                    & 0x0000000F << (Port_ConfigPtr->Pin[Pin].pin_num * 4));

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),
                    Port_ConfigPtr->Pin[Pin].pin_num);
        }
    }
}
#endif

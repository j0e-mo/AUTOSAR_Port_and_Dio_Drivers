 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Yousouf Soliman
 ******************************************************************************/
#ifndef PORT_CFG_H_
#define PORT_CFG_H_


/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/*
 * Enable/Disable DEV error detection
 */
#define PORT_DEV_ERROR_DETECT                 (STD_ON)


/*
 * Enable/Disable Setting pin direction in runtime
 */
#define PORT_SET_PIN_DIRECTION_API            (STD_ON)


/*
 * Enable/Disable Setting pin mode in runtime
 */
#define PORT_SET_PIN_MODE_API                 (STD_ON)


/*
 * Enable/Disable Getting AUTOSAR and Module version info
 */
#define PORT_VERSION_INFO_API                 (STD_ON)

/* Number of configured PINS */
#define PORT_NUMBER_OF_PORT_PINS              (43U)

/* Target HW Ports */
#define PORT_PortA                  (0U)
#define PORT_PortB                  (1U)
#define PORT_PortC                  (2U)
#define PORT_PortD                  (3U)
#define PORT_PortE                  (4U)
#define PORT_PortF                  (5U)

/* Target HW Pins */
#define PORT_Pin0                   (0U)
#define PORT_Pin1                   (1U)
#define PORT_Pin2                   (2U)
#define PORT_Pin3                   (3U)
#define PORT_Pin4                   (4U)
#define PORT_Pin5                   (5U)
#define PORT_Pin6                   (6U)
#define PORT_Pin7                   (7U)


#endif /* PORT_CFG_H_ */

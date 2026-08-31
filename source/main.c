/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the Class-B Safety test code
*              example for TCPWM block on KIT_XMC52_EVK, for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
 * $ Copyright 2026 Infineon Technologies AG $
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cybsp.h"
#include "cy_pdl.h"
#include "mtb_hal.h"
#include "cy_retarget_io.h"
#include "self_test.h"

/*******************************************************************************
* Macros
*******************************************************************************/
/* Available commands */
#define SELFTEST_CMD_TIMER          ('1')
#define SELFTEST_CMD_PWM            ('2')
#define SELFTEST_CMD_PWM_GATEKILL   ('3')

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* For the Retarget-IO (Debug UART) usage */
static cy_stc_scb_uart_context_t DEBUG_UART_context;
static mtb_hal_uart_t            DEBUG_UART_hal_obj;

/*******************************************************************************
* Function Name: main
*********************************************************************************
* Summary:
*  This is the main function. It performs Class-B safety tests for the TCPWM
*  block on KIT_XMC52_EVK. Tests are triggered by commands sent over UART:
*    '1' - Timer/Counter self-test
*    '2' - PWM self-test (no external connection needed; STATUS.LINE_OUT read directly)
*    '3' - PWM Gate Kill self-test (software-triggered, no external wiring needed)
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;
    uint32_t rx_data;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize the debug UART using PDL */
    result = Cy_SCB_UART_Init(CYBSP_DEBUG_UART_HW, &CYBSP_DEBUG_UART_config,
                               &DEBUG_UART_context);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    Cy_SCB_UART_Enable(CYBSP_DEBUG_UART_HW);

    /* Setup the HAL UART wrapper for retarget-io */
    result = mtb_hal_uart_setup(&DEBUG_UART_hal_obj, &CYBSP_DEBUG_UART_hal_config,
                                 &DEBUG_UART_context, NULL);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(&DEBUG_UART_hal_obj);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");
    printf("****************** "
           "MTB STL: TCPWM Safety Test"
           " ******************\r\n\n");

    /* Display available commands */
    printf("Available commands \r\n");
    printf("1 : Run SelfTest for Timer/Counter\r\n");
    printf("2 : Run SelfTest for PWM\r\n");
    printf("3 : Run SelfTest for PWM Gate Kill\r\n\n");

    for (;;)
    {
        /* Poll for a received character */
        rx_data = Cy_SCB_UART_Get(CYBSP_DEBUG_UART_HW);

        if (rx_data != CY_SCB_UART_RX_NO_DATA)
        {
            if (SELFTEST_CMD_TIMER == (uint8_t)rx_data)
            {
                printf("\r\n[Command] : Run SelfTest for Timer/Counter\r\n");
                timer_test();
            }
            else if (SELFTEST_CMD_PWM == (uint8_t)rx_data)
            {
                printf("\r\n[Command] : Run SelfTest for PWM\r\n");
                pwm_test();
            }
            else if (SELFTEST_CMD_PWM_GATEKILL == (uint8_t)rx_data)
            {
                printf("\r\n[Command] : Run SelfTest for PWM Gate Kill\r\n");
                pwm_gate_kill_test();
            }
            else
            {
                printf("\r\nEnter a valid command\r\n");
            }
        }
    }
}

/* [] END OF FILE */

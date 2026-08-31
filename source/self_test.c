/******************************************************************************
 * File Name:   self_test.c
 *
 * Description: This file contains the Timer/Counter, PWM, and PWM Gate Kill
 *              Class-B self-test wrapper functions for the TCPWM Safety Test
 *              on KIT_XMC52_EVK.
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
#include <stdio.h>
#include <stdbool.h>
#include "self_test.h"

/*******************************************************************************
* Function Name: timer_test
*********************************************************************************
* Summary:
* Configures the TCPWM block (group 0, counter 0) as a Timer/Counter with the
* CPU clock as input. The test verifies that the counter increments and that
* the count value falls within the expected thresholds.
*
* Parameters:
*  none
*
* Return:
*  void
*
*******************************************************************************/
void timer_test(void)
{
    SelfTest_Timer_Counter_init(CYBSP_TIMER_COUNTER_TEST_HW,
            CYBSP_TIMER_COUNTER_TEST_NUM, &CYBSP_TIMER_COUNTER_TEST_config,
            (IRQn_Type)CYBSP_TIMER_COUNTER_TEST_IRQ);

    if (OK_STATUS != SelfTest_Counter_Timer())
    {
        printf("Error: Timer Counter\r\n");
    }
    else
    {
        printf("Success: Timer Counter\r\n");
    }
}

/*******************************************************************************
* Function Name: pwm_test
*********************************************************************************
* Summary:
* Configures a 16-bit PWM (TCPWM group 0, counter 1) with 1/3 duty-ON, 2/3
* duty-OFF at a 1 ms period and starts it. The clock prescaler (DIVBY_2) is
* required because XMC5200 TCPWM group-0 counters are 16-bit; without
* prescaling, the computed period would overflow the register.
*
* On MXTCPWM_VER2 (XMC5200), SelfTest_PWM() reads STATUS.LINE_OUT directly
* via Cy_TCPWM_PWM_LineOutStatus() - no GPIO loopback jumper is needed, and
* the pin parameters passed to SelfTest_PWM() are ignored for XMC5000 devices.
*
* Parameters:
*  none
*
* Return:
*  void
*
*******************************************************************************/
void pwm_test(void)
{
    bool test_passed = false;

    if (OK_STATUS != SelfTest_PWM_init(CYBSP_PWM_HW, CYBSP_PWM_NUM, &CYBSP_PWM_config,
            (IRQn_Type)CYBSP_PWM_IRQ))
    {
        printf("Error: PWM init failed\r\n");
    }
    else
    {
        if (OK_STATUS != SelfTest_PWM(NULL, 0U))
        {
            printf("Error: PWM FAIL\r\n");
        }
        else
        {
            test_passed = true;
        }
    }

    if (test_passed)
    {
        printf("Success: PWM\r\n");
    }
}

/*******************************************************************************
* Function Name: pwm_gate_kill_test
*********************************************************************************
* Summary:
* Configures a dedicated PWM counter (TCPWM group 0, counter 2) with the Gate
* Kill feature enabled (pwmstoponkill=true) and exercises the software-
* triggered Gate Kill path used on XMC5000 devices:
*   1) Start the counter and confirm it is running (SelfTest_PWM_GateKill()
*      is expected to report ERROR_STATUS while the counter is still
*      incrementing).
*   2) Issue a software Stop/Kill trigger (Cy_TCPWM_TriggerStopOrKill_Single())
*      and confirm the counter has frozen (SelfTest_PWM_GateKill() reports
*      OK_STATUS once two successive reads of the counter match).
*
* No external wiring or ADC is required for this test - the kill condition is
* asserted entirely in software.
*
* Parameters:
*  none
*
* Return:
*  void
*
*******************************************************************************/
void pwm_gate_kill_test(void)
{
    bool test_passed = false;

    if (Cy_TCPWM_PWM_Init(CYBSP_PWM_GATEKILL_HW, CYBSP_PWM_GATEKILL_NUM,
            &CYBSP_PWM_GATEKILL_config) != CY_TCPWM_SUCCESS)
    {
        printf("Error: PWM GateKill init failed\r\n");
    }
    else
    {
        Cy_TCPWM_PWM_Enable(CYBSP_PWM_GATEKILL_HW, CYBSP_PWM_GATEKILL_NUM);
        Cy_TCPWM_TriggerReloadOrIndex_Single(CYBSP_PWM_GATEKILL_HW, CYBSP_PWM_GATEKILL_NUM);

        /* With no Kill applied, the counter is expected to still be running -
         * SelfTest_PWM_GateKill() must report ERROR_STATUS (counter changed). */
        if (ERROR_STATUS != SelfTest_PWM_GateKill(CYBSP_PWM_GATEKILL_HW, CYBSP_PWM_GATEKILL_NUM))
        {
            printf("Error: PWM GateKill (counter did not run as expected)\r\n");
        }
        else
        {
            /* Apply the software Gate Kill trigger and confirm the counter stops. */
            Cy_TCPWM_TriggerStopOrKill_Single(CYBSP_PWM_GATEKILL_HW, CYBSP_PWM_GATEKILL_NUM);

            if (OK_STATUS != SelfTest_PWM_GateKill(CYBSP_PWM_GATEKILL_HW, CYBSP_PWM_GATEKILL_NUM))
            {
                printf("Error: PWM GateKill\r\n");
            }
            else
            {
                test_passed = true;
            }
        }
    }

    if (test_passed)
    {
        printf("Success: PWM GateKill\r\n");
    }
}

/* [] END OF FILE */

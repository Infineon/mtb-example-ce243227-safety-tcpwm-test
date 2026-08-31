# Safety Test Library (STL) : TCPWM

This code example demonstrates the usage of ModusToolbox&trade; Safety Test Library (MTB STL) to test the Timer/Counter/PWM (TCPWM) peripheral of the MCU. The example configures one TCPWM counter as a Timer/Counter, another as a PWM, and a third as a PWM with Gate Kill enabled, and runs a self-test for each on demand over UART.

Name | Vanity URL
-----|-----------
**Getting started documents** |
ModusToolbox&trade; tools package installation guide| https://www.infineon.com/ModusToolboxInstallguide
ModusToolbox&trade; tools package release notes| https://www.infineon.com/ModusToolboxReleaseNotes
ModusToolbox&trade; tools package quick start guide | https://www.infineon.com/ModusToolboxQSG
ModusToolbox&trade; tools package user guide| https://www.infineon.com/ModusToolboxUserguide
**IDE documents** |
Eclipse IDE for ModusToolbox&trade; user guide | https://www.infineon.com/MTBEclipseIDEUserguide
Visual Studio Code for ModusToolbox&trade; user guide | https://www.infineon.com/MTBVSCodeUserGuide
Arm&reg; Keil&reg; µVision&reg; for ModusToolbox&trade; user guide | https://www.infineon.com/MTBuVisionUserGuide
IAR Embedded Workbench for ModusToolbox&trade; user guide | https://www.infineon.com/MTBIARUserGuide
Configurators and tools |
**General tools** |
ModusToolbox&trade; Dashboard user guide | https://www.infineon.com/ModusToolboxDashboard
ModusToolbox&trade; Project Creator user guide | https://www.infineon.com/ModusToolboxProjectCreator
ModusToolbox&trade; Library Manager user guide | https://www.infineon.com/ModusToolboxLibraryManager
ModusToolbox&trade; BSP Assistant user guide | https://www.infineon.com/ModusToolboxBSPAssistant
ModusToolbox&trade; LCS Manager CLI user guide | https://www.infineon.com/ModusToolboxLCSManager
**BSP configurators** |
ModusToolbox&trade; Device Configurator user guide| https://www.infineon.com/ModusToolboxDeviceConfig
ModusToolbox&trade; CAPSENSE&trade; Configurator user guide | https://www.infineon.com/ModusToolboxCapSenseConfig
ModusToolbox&trade; CAPSENSE&trade; Tuner user guide | https://www.infineon.com/ModusToolboxCapSenseTuner
ModusToolbox&trade; QSPI Configurator user guide | https://www.infineon.com/ModusToolboxQSPIConfig
ModusToolbox&trade; Smart I/O Configurator user guide | https://www.infineon.com/ModusToolboxSmartIOConfig
**Library configurators** |
ModusToolbox&trade; Bluetooth&reg; Configurator user guide | https://www.infineon.com/ModusToolboxBLEConfig
ModusToolbox&trade; EZ-PD&trade; Configurator user guide | https://www.infineon.com/ModusToolboxEZ-PDConfig
ModusToolbox&trade; LIN Configurator user guide | https://www.infineon.com/ModusToolboxLINConfig
ModusToolbox&trade; Secure Policy Configurator user guide | https://www.infineon.com/ModusToolboxSecurePolicyConfig
ModusToolbox&trade; Segment LCD Configurator user guide | https://www.infineon.com/ModusToolboxSegLCDConfig
ModusToolbox&trade; USB Configurator user guide | https://www.infineon.com/ModusToolboxUSBConfig
**MCU tools** |
ModusToolbox&trade; Device Firmware Update Host tool guide | https://www.infineon.com/ModusToolboxDFUHostTool
**Webpage Links** |
ModusToolbox&trade; product page | https://www.infineon.com/ModusToolbox
ModusToolbox&trade; Video Hub | https://www.infineon.com/ModusToolboxVideos
ModusToolbox&trade; Latest Community Announcement Post | https://www.infineon.com/ModusToolboxReleaseAnnouncement

[View this README on GitHub.](https://github.com/Infineon/mtb-example-ce243227-safety-tcpwm-test)

[Provide feedback on this code example.](https://yourvoice.infineon.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyNDMyMjciLCJTcGVjIE51bWJlciI6IjAwMi00MzIyNyIsIkRvYyBUaXRsZSI6IlNhZmV0eSBUZXN0IExpYnJhcnkgKFNUTCkgOiBUQ1BXTSIsInJpZCI6ImphaW4uam9zZXBoQGluZmluZW9uLmNvbSIsIkRvYyB2ZXJzaW9uIjoiMS4wLjAiLCJEb2MgTGFuZ3VhZ2UiOiJFbmdsaXNoIiwiRG9jIERpdmlzaW9uIjoiTUNEIiwiRG9jIEJVIjoiSUNXIiwiRG9jIEZhbWlseSI6IlBTT0MifQ==)

## Requirements

- [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) v3.8 or later (tested with v3.8)
- Programming language: C
- Associated parts:
    1. [XMC5000 MCU](https://www.infineon.com/products/microcontroller/32bit-industrial-arm-cortex-m/xmc5000)

## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm&reg; Embedded Compiler 14.2.1 (`GCC_ARM`) – Default value of `TOOLCHAIN`
- Arm&reg; Compiler v6.22 (`ARM`)
- IAR C/C++ Compiler v9.60.1 (`IAR`)

## Supported kits (make variable 'TARGET')

- [XMC5200 Evaluation Kit](https://www.infineon.com/evaluation-board/KIT-XMC52-EVK) (`KIT_XMC52_EVK`)


## Hardware setup

This example uses three TCPWM counters in the configurable *design.modus* file: group 0, counter 0 as a Timer/Counter, group 0, counter 1 as a PWM, and group 0, counter 2 as a PWM with Gate Kill enabled. All three counters are 16-bit on the XMC5200 TCPWM group 0 - the PWM counters use a clock prescaler (DIVBY_2) so the computed periods do not overflow the 16-bit period register.

None of the three tests require any external wiring. The Timer/Counter test verifies the counter increments within an expected range using the CPU clock as input. The PWM test reads the `STATUS.LINE_OUT` bit directly through `Cy_TCPWM_PWM_LineOutStatus()` - on XMC5000 devices this does not require a GPIO loopback jumper, and the pin parameters passed to `SelfTest_PWM()` are ignored. The PWM Gate Kill test asserts its Kill/Stop condition entirely in software (`Cy_TCPWM_TriggerStopOrKill_Single()`), which exercises the same TCPWM kill-response hardware path (counter freeze and PWM output disable) that an external Gate Kill signal would trigger - no comparator, ADC, or jumper wire is needed.

No jumper wires or other physical connections are needed; the board only needs to be connected to the PC through the KitProg3 USB connector.

## Software setup

See the [ModusToolbox tools package installation guide](https://www.infineon.com/ModusToolboxInstallguide) for information about installing and configuring the tools package.

Install a terminal emulator if you do not have one.

## Using the code example


### Create the project

The ModusToolbox&trade; tools package provides the Project Creator as both a GUI tool and a command line tool.

<details><summary><b>Use Project Creator GUI</b></summary>

1. Open the Project Creator GUI tool

   There are several ways to do this, including launching it from the dashboard or from inside the Eclipse IDE. For more details, see the [Project Creator user guide](https://www.infineon.com/ModusToolboxProjectCreator) (locally available at *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/docs/project-creator.pdf*)

2. On the **Choose Board Support Package (BSP)** page, select a kit supported by this code example. See [Supported kits](#supported-kits-make-variable-target)

   > **Note:** To use this code example for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work

3. On the **Select Application** page:

   a. Select the **Applications(s) Root Path** and the **Target IDE**

      > **Note:** Depending on how you open the Project Creator tool, these fields may be pre-selected for you

   b. Select this code example from the list by enabling its check box

      > **Note:** You can narrow the list of displayed examples by typing in the filter box

   c. (Optional) Change the suggested **New Application Name** and **New BSP Name**

   d. Click **Create** to complete the application creation process

</details>


<details><summary><b>Use Project Creator CLI</b></summary>

The 'project-creator-cli' tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the 'project-creator-cli' tool. On Windows, use the command-line 'modus-shell' program provided in the ModusToolbox&trade; installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; tools. You can access it by typing "modus-shell" in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

The following example clones the "[TCPWM test](https://github.com/Infineon/mtb-example-ce243227-safety-tcpwm-test)" application with the desired name "TCPWM_Test" configured for the *KIT_XMC52_EVK* BSP into the specified working directory *C:/mtb_projects*:

   ```
   project-creator-cli --board-id KIT_XMC52_EVK --app-id mtb-example-ce243227-safety-tcpwm-test --user-app-name TCPWM_Test --target-dir "C:/mtb_projects"
   ```

The 'project-creator-cli' tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the <id> field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the <id> field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

<br>

> **Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at {ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf).

</details>



### Open the project

After the project has been created, you can open it in your preferred development environment.


<details><summary><b>Eclipse IDE</b></summary>

If you opened the Project Creator tool from the included Eclipse IDE, the project will open in Eclipse automatically.

For more details, see the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_ide_user_guide.pdf*).

</details>


<details><summary><b>Visual Studio (VS) Code</b></summary>

Launch VS Code manually, and then open the generated *{project-name}.code-workspace* file located in the project directory.

For more details, see the [Visual Studio Code for ModusToolbox&trade; user guide](https://www.infineon.com/MTBVSCodeUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_vscode_user_guide.pdf*).

</details>


<details><summary><b>Arm&reg; Keil&reg; µVision&reg;</b></summary>

Double-click the generated *{project-name}.cprj* file to launch the Keil&reg; µVision&reg; IDE.

For more details, see the [Arm&reg; Keil&reg; µVision&reg; for ModusToolbox&trade; user guide](https://www.infineon.com/MTBuVisionUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_uvision_user_guide.pdf*).

</details>


<details><summary><b>IAR Embedded Workbench</b></summary>

Open IAR Embedded Workbench manually, and create a new project. Then select the generated *{project-name}.ipcf* file located in the project directory.

For more details, see the [IAR Embedded Workbench for ModusToolbox&trade; user guide](https://www.infineon.com/MTBIARUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_iar_user_guide.pdf*).

</details>


<details><summary><b>Command line</b></summary>

If you prefer to use the CLI, open the appropriate terminal, and navigate to the project directory. On Windows, use the command-line 'modus-shell' program; on Linux and macOS, you can use any terminal application. From there, you can run various `make` commands.

For more details, see the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>


## Operation

1. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector

2. Open a terminal program and select the KitProg3 COM port. Set the serial port parameters to 8N1 and 115200 baud

3. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE</b></summary>

      1. Select the application project in the Project Explorer

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**
   </details>


   <details><summary><b>In other IDEs</b></summary>

   Follow the instructions in your preferred IDE

   </details>


   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain is specified in the application's Makefile but you can override this value manually:
      ```
      make program TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TOOLCHAIN=GCC_ARM
      ```
   </details>

4. After programming, the application starts automatically. Confirm that **MTB STL: TCPWM Safety Test** is displayed on the UART terminal, along with the list of available commands

5. Send `1` over the terminal to run the Timer/Counter self-test, `2` to run the PWM self-test, or `3` to run the PWM Gate Kill self-test

6. The serial terminal should display the result of the selected test

   **Figure 1. Terminal output on program startup and after running the self-tests**

   ![](images/terminal-tcpwm-test.png)


## Debugging


You can debug the example to step through the code.


<details><summary><b>In Eclipse IDE</b></summary>

Use the **\<Application Name> Debug (KitProg3_MiniProg4)** configuration in the **Quick Panel**. For details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide).

</details>


<details><summary><b>In other IDEs</b></summary>

Follow the instructions in your preferred IDE.

</details>


## Design and implementation


This example uses three counters of a single TCPWM block to perform Class-B self-tests of the Timer/Counter, PWM, and PWM Gate Kill functionality. The *design.modus* file configures TCPWM group 0, counter 0 as a Timer/Counter (alias `CYBSP_TIMER_COUNTER_TEST`), group 0, counter 1 as a 16-bit PWM (alias `CYBSP_PWM`) with a `DIVBY_2` clock prescaler so the 1 ms period fits within the 16-bit period register, and group 0, counter 2 as a 16-bit PWM with Gate Kill enabled (alias `CYBSP_PWM_GATEKILL`, `pwmstoponkill = true`).

The example begins by initializing the BSP configuration and setting up retarget-io for debug prints over the dedicated debug UART. It then prints a command menu and polls the UART for a `1`, `2`, or `3` character.

Sending `1` runs the Timer/Counter self-test: `SelfTest_Timer_Counter_init()` configures the counter to run from the CPU clock, and `SelfTest_Counter_Timer()` from the mtb-stl (https://github.com/Infineon/mtb-stl) safety library verifies the counter increments and that the count value falls within the expected thresholds.

Sending `2` runs the PWM self-test: `SelfTest_PWM_init()` configures a 16-bit PWM with a 1/3 duty-ON, 2/3 duty-OFF ratio at a 1 ms period, and `SelfTest_PWM()` polls the counter's `STATUS.LINE_OUT` bit directly (via `Cy_TCPWM_PWM_LineOutStatus()`) to verify the observed duty cycle falls within the expected range. On XMC5000 devices this does not require a GPIO loopback jumper, and the pin parameters passed to `SelfTest_PWM()` are ignored.

Sending `3` runs the PWM Gate Kill self-test: the counter is started and confirmed to be running (`SelfTest_PWM_GateKill()` reports `ERROR_STATUS` while the counter is still incrementing, since no kill has been applied yet). A software Stop/Kill trigger is then issued (`Cy_TCPWM_TriggerStopOrKill_Single()`), which writes directly to the counter's `TR_CMD` register - the same trigger-command path an external Kill/Stop input would assert. `SelfTest_PWM_GateKill()` then confirms the counter has frozen (two successive counter reads, 10 ms apart, match), verifying the TCPWM kill-response hardware (counter freeze and PWM output disable) operates correctly. No external wiring, jumper, or ADC is required - the kill condition is asserted entirely in software.

The PASS or FAIL result of each self-test is printed to the debug UART console; any test can be run again at any time by re-sending its command character.

See the Application Notes listed in [Related resources](#related-resources) for more details.

### Resources and settings

**Table 1. Application resources**

 Resource  |  Alias/object     |    Purpose
 :-------- | :-------------    | :------------
 UART (HAL) | DEBUG_UART | UART HAL object used by Retarget-IO for the Debug UART port
 TCPWM (PDL) | CYBSP_TIMER_COUNTER_TEST | TCPWM group 0, counter 0 configured as a Timer/Counter under test
 TCPWM (PDL) | CYBSP_PWM | TCPWM group 0, counter 1 configured as a 16-bit PWM under test
 TCPWM (PDL) | CYBSP_PWM_GATEKILL | TCPWM group 0, counter 2 configured as a 16-bit PWM with Gate Kill enabled, under test (software-triggered, no external wiring)

<br>

## Related resources

Resources  | Links
-----------|----------------------------------
Application notes  | [AN241720](https://www.infineon.com/assets/row/public/documents/30/42/infineon-getting-started-with-xmc5000-mcu-on-modustoolbox-software-applicationnotes-en.pdf) – Getting started with XMC5000 MCU on ModusToolbox&trade; <br>
Code examples | [Using ModusToolbox&trade;](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) on GitHub
Device documentation | [XMC5000 MCU documents](https://www.infineon.com/products/microcontroller/32bit-industrial-arm-cortex-m/xmc5000#Documents) <br>
Development kits | Select your kits from the [Evaluation board finder](https://www.infineon.com/cms/en/design-support/finder-selection-tools/product-finder/evaluation-board).
Libraries on GitHub  | [mtb-pdl-cat1](https://github.com/Infineon/mtb-pdl-cat1) – Peripheral Driver Library (PDL) <br> [mtb-hal-cat1](https://github.com/Infineon/mtb-hal-cat1) – Hardware Abstraction Layer (HAL) Library
Middleware on GitHub | [mcu-middleware](https://github.com/Infineon/modustoolbox-software) – Links to all MCU middleware <br> [mtb-stl](https://github.com/Infineon/mtb-stl) - Safety Test Library (STL)
Tools  | [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) – ModusToolbox&trade; software is a collection of easy-to-use libraries and tools enabling rapid development with Infineon MCUs for applications ranging from wireless and cloud-connected systems, edge AI/ML, embedded sense and control, to wired USB connectivity using PSOC&trade; Industrial/IoT MCUs, AIROC&trade; Wi-Fi and Bluetooth&reg; connectivity devices, XMC&trade; Industrial MCUs, and EZ-USB&trade;/EZ-PD&trade; wired connectivity controllers. ModusToolbox&trade; incorporates a comprehensive set of BSPs, HAL, libraries, configuration tools, and provides support for industry-standard IDEs to fast-track your embedded application development.

<br>

## Other resources


Infineon provides a wealth of data at [www.infineon.com](https://www.infineon.com) to help you select the right device, and quickly and effectively integrate it into your design.


## Document history

Document title: *CE243227* – *Safety Test Library (STL) : TCPWM Test*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example. Includes the TCPWM PWM self-test and the PWM Gate Kill self-test (software-triggered, TCPWM group 0, counter 2)

<br>

All referenced product or service names and trademarks are the property of their respective owners.

The Bluetooth&reg; word mark and logos are registered trademarks owned by Bluetooth SIG, Inc., and any use of such marks by Infineon is under license.

PSOC&trade;, formerly known as PSoC&trade;, is a trademark of Infineon Technologies. Any references to PSoC&trade; in this document or others shall be deemed to refer to PSOC&trade;.

---------------------------------------------------------

(c) 2026, Infineon Technologies AG, or an affiliate of Infineon Technologies AG. All rights reserved.
This software, associated documentation and materials ("Software") is owned by Infineon Technologies AG or one of its affiliates ("Infineon") and is protected by and subject to worldwide patent protection, worldwide copyright laws, and international treaty provisions. Therefore, you may use this Software only as provided in the license agreement accompanying the software package from which you obtained this Software. If no license agreement applies, then any use, reproduction, modification, translation, or compilation of this Software is prohibited without the express written permission of Infineon.
<br>
Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A SPECIFIC USE/PURPOSE OR MERCHANTABILITY. Infineon reserves the right to make changes to the Software without notice. You are responsible for properly designing, programming, and testing the functionality and safety of your intended application of the Software, as well as complying with any legal requirements related to its use. Infineon does not guarantee that the Software will be free from intrusion, data theft or loss, or other breaches ("Security Breaches"), and Infineon shall have no liability arising out of any Security Breaches. Unless otherwise explicitly approved by Infineon, the Software may not be used in any application where a failure of the Product or any consequences of the use thereof can reasonably be expected to result in personal injury.

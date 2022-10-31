/+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAq-
+ACo-                   (c) SEGGER Microcontroller GmbH                  +ACo-
+ACo-                        The Embedded Experts                        +ACo-
+ACo-                           www.segger.com                           +ACo-
+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACo-

-------------------------- END-OF-HEADER -----------------------------

File    : SEGGER+AF8-SYSVIEW+AF8-Config+AF8-embOS+AF8-Ultra+AF8-CM0.c
Purpose : Sample setup configuration of SystemView with embOS
          on Cortex-M0/Cortex-M0+-/Cortex-M1 systems which do not
          have a cycle counter.
Revision: +ACQ-Rev: 25330 +ACQ-
+ACo-/
+ACM-include +ACI-RTOS.h+ACI-
+ACM-include +ACI-SEGGER+AF8-SYSVIEW.h+ACI-
+ACM-include +ACI-SEGGER+AF8-SYSVIEW+AF8-embOS.h+ACI-

/+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAq-
+ACo-
+ACo-       Defines, fixed
+ACo-
+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACo-
+ACo-/
+ACM-define SCB+AF8-ICSR  (+ACo-(volatile U32+ACo-) (0xE000ED04uL))  // Interrupt Control State Register
+ACM-define SCB+AF8-ICSR+AF8-PENDSTSET+AF8-MASK     (1UL +ADwAPA- 26)      // SysTick pending bit
+ACM-define SYST+AF8-RVR  (+ACo-(volatile U32+ACo-) (0xE000E014uL))  // SysTick Reload Value Register
+ACM-define SYST+AF8-CVR  (+ACo-(volatile U32+ACo-) (0xE000E018uL))  // SysTick Current Value Register

/+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAq-
+ACo-
+ACo-       Local functions
+ACo-
+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACo-
+ACo-/
/+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAq-
+ACo-
+ACo-       +AF8-cbSendSystemDesc()
+ACo-
+ACo-  Function description
+ACo-    Sends SystemView description strings.
+ACo-/
static void +AF8-cbSendSystemDesc(void) +AHs-
  SEGGER+AF8-SYSVIEW+AF8-SendSysDesc(+ACI-N+AD0AIg- SEGGER+AF8-SYSVIEW+AF8-APP+AF8-NAME +ACI-,O+AD0-embOS+AF8-Ultra,D+AD0AIg- SEGGER+AF8-SYSVIEW+AF8-DEVICE+AF8-NAME )+ADs-
+ACM-ifdef SEGGER+AF8-SYSVIEW+AF8-SYSDESC0
  SEGGER+AF8-SYSVIEW+AF8-SendSysDesc(SEGGER+AF8-SYSVIEW+AF8-SYSDESC0)+ADs-
+ACM-endif
+ACM-ifdef SEGGER+AF8-SYSVIEW+AF8-SYSDESC1
  SEGGER+AF8-SYSVIEW+AF8-SendSysDesc(SEGGER+AF8-SYSVIEW+AF8-SYSDESC1)+ADs-
+ACM-endif
+ACM-ifdef SEGGER+AF8-SYSVIEW+AF8-SYSDESC2
  SEGGER+AF8-SYSVIEW+AF8-SendSysDesc(SEGGER+AF8-SYSVIEW+AF8-SYSDESC2)+ADs-
+ACM-endif
+AH0-

/+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAq-
+ACo-
+ACo-       Global functions
+ACo-
+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACo-
+ACo-/
/+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAq-
+ACo-
+ACo-       SEGGER+AF8-SYSVIEW+AF8-Conf()
+ACo-
+ACo- Function description
+ACo-   Configure and initialize SystemView and register it with embOS.
+ACo-
+ACo- Additional information
+ACo-   If enabled, SEGGER+AF8-SYSVIEW+AF8-Conf() will also immediately start
+ACo-   recording events with SystemView.
+ACo-/
void SEGGER+AF8-SYSVIEW+AF8-Conf(void) +AHs-
  SEGGER+AF8-SYSVIEW+AF8-Init(SEGGER+AF8-SYSVIEW+AF8-TIMESTAMP+AF8-FREQ, SEGGER+AF8-SYSVIEW+AF8-CPU+AF8-FREQ,
                      +ACY-SYSVIEW+AF8-X+AF8-OS+AF8-TraceAPI, +AF8-cbSendSystemDesc)+ADs-
  OS+AF8-TRACE+AF8-SetAPI(+ACY-embOS+AF8-TraceAPI+AF8-SYSVIEW)+ADs-  // Configure embOS to use SYSVIEW.
+ACM-if SEGGER+AF8-SYSVIEW+AF8-START+AF8-ON+AF8-INIT
  SEGGER+AF8-SYSVIEW+AF8-Start()+ADs-                    // Start recording to catch system initialization.
+ACM-endif
+AH0-

/+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAq-
+ACo-
+ACo-       SEGGER+AF8-SYSVIEW+AF8-X+AF8-GetTimestamp()
+ACo-
+ACo- Function description
+ACo-   Returns the current timestamp in cycles using the system tick
+ACo-   count and the SysTick counter.
+ACo-   All parameters of the SysTick have to be known and are set via
+ACo-   configuration defines on top of the file.
+ACo-
+ACo- Return value
+ACo-   The current timestamp.
+ACo-
+ACo- Additional information
+ACo-   SEGGER+AF8-SYSVIEW+AF8-X+AF8-GetTimestamp is always called when interrupts are
+ACo-   disabled. Therefore locking here is not required.
+ACo-/
U32 SEGGER+AF8-SYSVIEW+AF8-X+AF8-GetTimestamp(void) +AHs-
  return BSP+AF8-OS+AF8-GetCycles()+ADs-
+AH0-

/+ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAq- End of file +ACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACoAKgAqACo-/

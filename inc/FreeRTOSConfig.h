/**************************************************************************//**
 * @file FreeRTOSConfig.h
 * @date 25.09.2015
 * @author vlunyov
 * @brief Настройки FreeRTOS
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1575 $
 * $Date: 2017-03-07 09:49:41 +0300 (Вт, 07 мар 2017) $
 *
 *
 *****************************************************************************/

/*
 * Notes:
 * Note that the configTOTAL_HEAP_SIZE setting in FreeRTOSConfig.h
 * has no effect when heap_3 is used.
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
//Set to 1 to use the preemptive RTOS scheduler,
//or 0 to use the cooperative RTOS scheduler
#define configUSE_PREEMPTION			1
//Set to 1 if you wish to use an idle hook, or 0 to omit an idle hook
#define configUSE_IDLE_HOOK				0
//Set to 1 if you wish to use an tick hook, or 0 to omit an tick hook.
#define configUSE_TICK_HOOK				1
/*
 * The malloc() failed hook function is a hook (or callback) function that,
 * if defined and configured, will be called if pvPortMalloc() ever returns NULL.
 * NULL will be returned only if there is insufficient FreeRTOS heap memory remaining
 * for the requested allocation to succeed.
 * If configUSE_MALLOC_FAILED_HOOK is set to 1 then the application must define a
 * malloc() failed hook function. If configUSE_MALLOC_FAILED_HOOK is set to 0 then
 * the malloc() failed hook function will not be called, even if one is defined.
 * Malloc() failed hook functions must have the name and prototype shown below.
 * void vApplicationMallocFailedHook( void );
 */
#define configUSE_MALLOC_FAILED_HOOK	1
/*
 * Enter the frequency in Hz at which the internal clock that drives
 * the peripheral used to generate the tick interrupt will be executing -
 * this is normally the same clock that drives the internal CPU clock.
 * This value is required in order to correctly configure timer peripherals.
 */
#define configCPU_CLOCK_HZ				( ( uint32_t ) 80000000 )
/*
 * The frequency of the RTOS tick interrupt.
 * The tick interrupt is used to measure time.
 */
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 )
/*
 * The number of priorities available to the application tasks.
 */
#define configMAX_PRIORITIES			( 4 )
// The size of the stack used by the idle task. A stack size of 50 means 200 bytes
#define configMINIMAL_STACK_SIZE		( ( uint16_t ) 50 )
//#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 24 * 1024 ) )
/*
 * The maximum permissible length of the descriptive name given to a task when
 * the task is created. The length is specified in the number of
 * characters including the NULL termination byte.
 */
#define configMAX_TASK_NAME_LEN			( 8 )
/*
 * Defining configUSE_16_BIT_TICKS as 1 causes TickType_t to be defined
 * (typedef'ed) as an unsigned 16bit type. Defining configUSE_16_BIT_TICKS
 * as 0 causes TickType_t to be defined (typedef'ed) as an unsigned 32bit type.
 */
#define configUSE_16_BIT_TICKS			0
/*
 * The preemptive scheduler is being used.
 * The application creates tasks that run at the idle priority.
 */
#define configIDLE_SHOULD_YIELD			1

/*
 * Setting configUSE_TASK_NOTIFICATIONS to 1
 * (or leaving configUSE_TASK_NOTIFICATIONS undefined)
 * will include direct to task notification functionality
 * and its associated API in the build.
 * Each task consumes 8 additional bytes of RAM when direct
 * to task notifications are included in the build.
 */

//Set to 1 to include mutex functionality in the build,
//or 0 to omit mutex functionality from the build.
#define configUSE_MUTEXES				1
#define configUSE_RECURSIVE_MUTEXES		0
#define configUSE_COUNTING_SEMAPHORES	1
#define configCHECK_FOR_STACK_OVERFLOW	2
#define configGENERATE_RUN_TIME_STATS	0
//added for vTaskGetRunTimeStats()
//#define configSUPPORT_DYNAMIC_ALLOCATION 1

#define configENABLE_BACKWARD_COMPATIBILITY		1

#if defined(DEBUG)
#define configUSE_TRACE_FACILITY				1
#define configUSE_STATS_FORMATTING_FUNCTIONS	1
#else
/*
 * Set to 1 if you wish to include additional structure members
 * and functions to assist with execution visualisation and tracing.
 */
#define configUSE_TRACE_FACILITY				1
/*
 * Set configUSE_TRACE_FACILITY and configUSE_STATS_FORMATTING_FUNCTIONS to 1
 *  to include the vTaskList() and vTaskGetRunTimeStats() functions in the build.
 *  Setting either to 0 will omit vTaskList() and vTaskGetRunTimeStates()
 *  from the build.
 */
#define configUSE_STATS_FORMATTING_FUNCTIONS	1

#endif

// Co-routine definitions.
#define configUSE_CO_ROUTINES 			0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

// Software timer definitions.
#define configUSE_TIMERS				0
#define configTIMER_TASK_PRIORITY		( 2 )
#define configTIMER_QUEUE_LENGTH		10
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )

// Set the following definitions to 1 to include the API function, or zero to exclude the API function.

#define INCLUDE_vTaskPrioritySet		0
#define INCLUDE_uxTaskPriorityGet		0
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

// Cortex-M specific definitions.
#ifdef __NVIC_PRIO_BITS
	// __BVIC_PRIO_BITS will be specified when CMSIS is being used.
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		3        // 8 priority levels
#endif

// Interrupt priority levels:
// 0 Наивысший приоритет прерываний
// 1
// 2
// 3 FreeRTOS API configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
// 4 FreeRTOS API
// 5 FreeRTOS API
// 6 FreeRTOS API
// 7 FreeRTOS API configKERNEL_INTERRUPT_PRIORITY Низший приоритет прерываний

// The lowest interrupt priority that can be used in a call to a "set priority" function.
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			( (1 << configPRIO_BITS ) - 1 )

// The highest interrupt priority that can be used by any interrupt service
// routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
// INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
// PRIORITY THAN THIS! (higher priorities are lower numeric values.
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	3

// Interrupt priorities used by the kernel port layer itself.  These are generic
// to all Cortex-M ports, and do not rely on any particular library functions.
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
// !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
// See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html.
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )


// Definitions that map the FreeRTOS port interrupt handlers to their CMSIS standard names.
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

extern void vConfigureTimerForRunTimeStats();
extern volatile unsigned long ulHighFrequencyTimerTicks;

//#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() vConfigureTimerForRunTimeStats()
//#define portGET_RUN_TIME_COUNTER_VALUE() ulHighFrequencyTimerTicks


//#define configASSERT( ( x ) )	if( ( x ) == 0) vAssertCalled(__FILE__, __LINE__)
//#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for(;;); }

#endif // FREERTOS_CONFIG_H

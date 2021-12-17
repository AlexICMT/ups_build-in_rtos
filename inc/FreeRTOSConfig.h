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
#define configCPU_CLOCK_HZ				( ( uint32_t ) 80000000 )
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES			( 4 )
#define configMINIMAL_STACK_SIZE		( ( uint16_t ) 50 )
//#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 24 * 1024 ) )
#define configMAX_TASK_NAME_LEN			( 8 )
#define configUSE_16_BIT_TICKS			0
#define configIDLE_SHOULD_YIELD			1
#define configUSE_MUTEXES				1
#define configUSE_RECURSIVE_MUTEXES		0
#define configUSE_COUNTING_SEMAPHORES	1
#define configCHECK_FOR_STACK_OVERFLOW	2
#define configGENERATE_RUN_TIME_STATS	0

#define configENABLE_BACKWARD_COMPATIBILITY		1

#if defined(DEBUG)
#define configUSE_TRACE_FACILITY				1
#define configUSE_STATS_FORMATTING_FUNCTIONS	1
#else
#define configUSE_TRACE_FACILITY				1
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

//#define configASSERT( ( x ) )	if( ( x ) == 0) vAssertCalled(__FILE__, __LINE__)
//#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for(;;); }

#endif // FREERTOS_CONFIG_H

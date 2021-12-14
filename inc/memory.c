/**************************************************************************//**
 * @file memory.c
 * @date 01.07.2013
 * @author vlunyov
 * @brief Stack memory usage.
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1449 $
 * $Date: 2017-01-10 10:11:24 +0300 (Вт, 10 янв 2017) $
 *
 * @note
 * <h2><center>&copy; Copyright KW-Systems</center></h2>
 * <h2><center><a href="https://kwsystems.ru/">KW-Systems</a></center></h2>
 *
 *****************************************************************************/

#include <sys/types.h>

// Non standard function returns the stack size
size_t stack_size(void)
{
	extern char __StackTop[];
	extern char __StackLimit[];

	return (size_t)__StackTop - (size_t)__StackLimit;
}

#define STACK_MAGIC_VALUE	0xA55AF00F

// Non standard function initialize the stack usage mechanism
void stack_used_init(void)
{
	extern char __StackTop[];
	extern char __StackLimit[];

	size_t *stack = (size_t*)__StackLimit;

	while(stack < (size_t *)__StackTop - 0x20u)
	{
		*stack++ = (size_t)STACK_MAGIC_VALUE;
	}
}

// Non standard function returns the stack usage size
size_t stack_left(void)
{
	extern char __StackLimit[];

	size_t *stack = (size_t*)__StackLimit;

	while(*stack == (size_t)STACK_MAGIC_VALUE)
		stack++;

	return (size_t)stack - (size_t)__StackLimit;
}

// Called before the main() function
void low_level_init(void)
{
	stack_used_init();
}

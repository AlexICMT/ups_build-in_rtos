/**************************************************************************//**
 * @file syscalls.c
 * @date 01.07.2013
 * @author vlunyov
 * @brief System dependences for newlib, allocate memory, etc.
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

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

void _exit(int code);
int _open(const char *name, int flags, int mode);
int _close(int file);
int _read(int file, char *ptr, int len);
int _write(int file, char *ptr, int len);
int _lseek(int file, int ptr, int dir);
int _fstat(int file, struct stat *st);
int _getpid(void);
int _isatty(int file);
int _kill(int pid, int sig);
caddr_t _sbrk(int incr);

#ifdef errno
#undef errno
#endif
extern int errno;

extern char __HeapBase[];	// Defined by the linker.
extern char __StackLimit[];	// Defined by the linker.

static caddr_t heap = (caddr_t)__HeapBase;

void _exit(int code)
{
	(void)code;
	for(;;);
}

int _open(const char *name, int flags, int mode)
{
	(void)name;
	(void)flags;
	(void)mode;
	return -1;
}

int _close(int file)
{
	(void)file;
	return -1;
}

int _read(int file, char *ptr, int len)
{
	(void)file;
	(void)ptr;
	(void)len;
	return 0;
}

int _write(int file, char *ptr, int len)
{
	(void)file;
	(void)ptr;
	(void)len;

	extern int __write(char *ptr, int len);
	if(file == 1)
		return __write(ptr, len);

	return 0;
}

int _lseek(int file, int ptr, int dir)
{
	(void)file;
	(void)ptr;
	(void)dir;
	return 0;
}

int _fstat(int file, struct stat *st)
{
	(void)file;
	st->st_mode = S_IFCHR;
	return 0;
}

int _getpid(void)
{
	return 1;
}

int _isatty(int file)
{
	(void)file;
	return 1;
}

int _kill(int pid, int sig)
{
	(void)pid;
	(void)sig;
	errno = EINVAL;
	return -1;
}

caddr_t _sbrk(int incr)
{
	caddr_t prevHeap;
	caddr_t nextHeap;

	prevHeap = heap;

	// Always return data aligned on a 8 byte boundary
	nextHeap = (caddr_t)(((unsigned int)(heap + incr) + 3) & ~3u);

	// Check enough space and there is no collision with stack coming the other way
	// if stack is above start of heap
	if(nextHeap > (caddr_t)__StackLimit)
	{
		errno = ENOMEM;

#if configUSE_MALLOC_FAILED_HOOK == 1
		extern void vApplicationMallocFailedHook(void);
		vApplicationMallocFailedHook();
#endif

		return NULL;		// error - no more memory
	}

	heap = nextHeap;

	return prevHeap;
}

// Non standard function for controlling the heap allocation size
size_t heap_allocated(void)
{
	size_t allocated;
	allocated = (size_t)heap - (size_t)__HeapBase;
	return allocated;
}

// Non standard function returns the heap size
size_t heap_size(void)
{
	return (size_t)__StackLimit - (size_t)__HeapBase;
}

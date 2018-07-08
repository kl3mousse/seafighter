// ++===================================================================++
// || mutex.c - Mutexes													||
// ++-------------------------------------------------------------------++
// || $Id: mutex.c,v 1.1 2001/04/19 14:45:34 fma Exp $					||
// ++-------------------------------------------------------------------++
// || BGM: Cowboy BeBop - Tank!											||
// ++===================================================================++

//---------------------------------------------------------------------------
#include <stdlib.h>
#include <mutex.h>
#include <task.h>
//---------------------------------------------------------------------------
static	MUTEX mutex_list[MAX_MUTEX];
//---------------------------------------------------------------------------
void __attribute__ ((constructor)) mutex_init(void)
{
	memset(mutex_list, 0, sizeof(MUTEX) * MAX_MUTEX);
}
//---------------------------------------------------------------------------
PMUTEX	mutex_create(void)
{
	register int	i;

	// Find a free slot
	for(i=0;i<MAX_MUTEX;i++)
	{
		if (!mutex_list[i])
		{
			// Indicate the used state of the mutex and return its
			// address
			mutex_list[i] = 1;
			return &mutex_list[i];
		}
	}

	return NULL;
}
//---------------------------------------------------------------------------
void	mutex_destroy(PMUTEX mutex)
{
	// Mark the mutex as free
	*mutex = 0;
}
//---------------------------------------------------------------------------
void	mutex_take(PMUTEX mutex)
{
	// First check if the mutex is allocated, and if the current task
	// doesn't already own it
	if ((*mutex == 0)||(*mutex == (DWORD)_current_task)||
		(_current_task == 0))
		return;

	// Wait for the mutex to be released...
	while(*mutex != 1)
	{
		_release_timeslice();
	}

	// ...and take it
	*mutex = (DWORD)_current_task;
}
//---------------------------------------------------------------------------
void	mutex_release(PMUTEX mutex)
{
	// First check the mutex is allocated and the task requesting its
	// release is the current owner
	if ((*mutex == 0)||(*mutex != (DWORD)_current_task))
		return;

	// release the mutex for other tasks
	*mutex = 1;
}
//---------------------------------------------------------------------------
void    mutex_clean(PTASK task)
{
	register int     i;

	// Free all mutexes owned by the current task
	for(i=0;i<MAX_MUTEX;i++)
	{
		if (mutex_list[i] == (DWORD)task)
			mutex_list[i] = 1;
	}
}


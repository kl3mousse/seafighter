// ++===================================================================++
// || task.c - "Multitasking" System									||
// ++-------------------------------------------------------------------++
// || $Id: task.c,v 1.6 2001/07/18 14:45:58 fma Exp $					||
// ++-------------------------------------------------------------------++
// || Please note that this is not REAL multitasking but merely			||
// || a fake. ^_^;														||
// ++-------------------------------------------------------------------||
// || BGM: Di Gi Charat - Only One, Number One !						||
// ++===================================================================++

//---------------------------------------------------------------------------
#include <stdlib.h>
#include <mutex.h>
//---------------------------------------------------------------------------
static const char Id[] = "libProcess Ver 1.0 Build:"__DATE__" "__TIME__;
//---------------------------------------------------------------------------
#ifdef __cplusplus
	extern "C" {
#endif
// System -> Task context switch
void _exec_task(void);
#ifdef __cplusplus
	}
#endif

// Routine to terminate a task that has reached its closing brace
static  void    task_term(void);

// Global system cpu context. Registers aren't in expected order
CPU_CONTEXT		_system_context;

// Save slot for context switching routine
DWORD			_save_reg;

// Task being currently executed. == NULL outside of any task.
PTASK			_current_task;

// First task of the list
PTASK			first_task;
//---------------------------------------------------------------------------
// Task slots
TASK	task_table[MAX_TASKS];
//---------------------------------------------------------------------------
void __CONSTRUCTOR__ task_init(void)
{
	register int	i;

	// Initialize all task slots
	for(i=0;i<MAX_TASKS;i++)
	{
		task_table[i].state = TASK_TERMINATED;
		task_table[i].mailbox.from = NULL;
	}

	// Task list is empty
	first_task = NULL;
	
	// We're not in a task
	_current_task = NULL;
}
//---------------------------------------------------------------------------
PTASK	task_create(void *task_addr, DWORD prio, DWORD tag1,
	DWORD tag2, int nb_args, ...)
{
	PTASK	task;
	int		i;

	// Look for a free slot
	for(i=0;i<MAX_TASKS;i++)
	{
		if (task_table[i].state == TASK_TERMINATED)
			break;
	}

	// If no slot available, exit
	if (i == MAX_TASKS)
		return NULL;

	// Find the place of the task in the list, according to its prio
	task = first_task;
	while( task != NULL)
	{
		if ((task->next == NULL)||
			((task->next)->prio > prio))
				break;

		task = task->next;
	}

	// *****************************
	// Initialise the task's context
	// *****************************

	// Copy arguments into task's stack
	memcpy(&task_table[i].stack[STACK_SIZE - nb_args * 4],
		((PDWORD)&nb_args)+1, 4 * nb_args);

	// Put 'myself' argument in the stack
	*((DWORD *)&task_table[i].stack[STACK_SIZE - nb_args * 4 - 4])
		= (DWORD)&task_table[i];

	// Put return address (task_term) in the task's stack
	*((DWORD *)&task_table[i].stack[STACK_SIZE - nb_args * 4 - 8])
		= (DWORD)task_term;

	// Initialize stack pointer, pc and status reg
	task_table[i].context.a7 =
		(DWORD)&task_table[i].stack[STACK_SIZE - nb_args * 4 - 8];
	task_table[i].context.pc = (DWORD)task_addr;
	task_table[i].context.sr = 0x2000;

	// Initialize task properties
	task_table[i].prio = prio;
	task_table[i].tag1 = tag1;
	task_table[i].tag2 = tag2;
	task_table[i].state = TASK_RUNNING;
	task_table[i].mailbox.from = NULL;

	// Insert the task in the list
	if (task == NULL)
	{
		task_table[i].next = NULL;
		first_task = &task_table[i];
		return first_task;
	}

	if ((task == first_task)&&(task->prio > prio))
	{
		task_table[i].next = task;
		first_task = &task_table[i];
		return &task_table[i];
	}

	task_table[i].next = task->next;
	task->next = &task_table[i];

	return &task_table[i];
}
//---------------------------------------------------------------------------
void    task_term(void)
{
	// This function is called whenever a task ends by letting the PC
	// reach its closing brace
	task_kill(_current_task);
}
//---------------------------------------------------------------------------
void    task_kill(PTASK task)
{
	PTASK   ptr;

	// Check if the task is not already dead
	if (task->state == TASK_TERMINATED)
                return;

	// Find the task BEFORE the one we want to kill
	ptr = first_task;
	while(ptr != NULL)
	{
		if (ptr->next == task)
			break;

		ptr = ptr->next;
	}

	// Signal the terminated state of the task and eject it from the list
	task->state = TASK_TERMINATED;
	if (ptr != NULL)
		ptr->next = task->next;
	else
		first_task = task->next;

	// Free all mutexes owned by the killed task
	mutex_clean(task);
	
	// If a task killed itself, its timeslice is released immediately
	if (task == _current_task)
		_release_timeslice();
}
//---------------------------------------------------------------------------
void    task_exec(void)
{
	PTASK   task;

	// Scan the task list and execute any task that has the "running" state
	task = first_task;
	while(task != NULL)
	{
		if (task->state == TASK_RUNNING)
		{
			_current_task = task;
			_exec_task();
			_current_task = NULL;
		}

		task = task->next;
	}
}
//---------------------------------------------------------------------------
void	task_suspend(PTASK task)
{
	// Suspend the task if it's running
	if (task->state == TASK_RUNNING)
		task->state = TASK_SUSPENDED;
}
//---------------------------------------------------------------------------
void	task_resume(PTASK task)
{
	// Resume execution of the task if it's suspended
	if (task->state == TASK_SUSPENDED)
		task->state = TASK_RUNNING;
}
//---------------------------------------------------------------------------
void	task_enum(DWORD what, DWORD state, DWORD tag1, DWORD tag2,
	void *user_data, TASKENUM_CALLBACK callback)
{
	PTASK   task;

	task = first_task;
	while(task != NULL)
	{
		if ((what & TASKENUM_STATE)&&(task->state != state))
			goto skip_task;

		if ((what & TASKENUM_TAG1)&&(task->tag1 != tag1))
			goto skip_task;

		if ((what & TASKENUM_TAG2)&&(task->tag2 != tag2))
			goto skip_task;
			
		if (callback(task, user_data) != TRUE)
			return;

skip_task:
		task = task->next;
	}	
}
//---------------------------------------------------------------------------
void	task_sleep(DWORD ntimes)
{
	for(;ntimes>0;ntimes--)
		_release_timeslice();
}

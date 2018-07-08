/* $Id: task_disp.c,v 1.4 2001/06/06 12:39:01 fma Exp $ */

#include <input.h>
#include <video.h>
#include "task_disp.h"

#define FIX_PALETTE	0

// Function prototype of the task_enum Callback
BOOL task_disp_enum(PTASK task, void *user_data);

// Task displayer task, useful for debug !
void task_disp(PTASK myself)
{
	DWORD	current_line;
	
	while(1)
	{
		while(!(poll_joystick(PORT1, READ_BIOS) & JOY_SELECT))
			_release_timeslice();
	
		do
		{
			// Display row header
			textout(0, 0, FIX_PALETTE, 0, "TCB    PRIO TAG1TAG2 NEXTPC");

			// Start displaying tasks just after row header
			current_line = 1;

			// Enumerate all tasks
			task_enum(TASKENUM_NONE, 0, 0, 0, &current_line, task_disp_enum);

			// Materialize the end of the list (the number of tasks may be
			// variable)
			textout(0, current_line, FIX_PALETTE, 0, "-- End of List --");

			// Relinquish control to other tasks
			_release_timeslice();
		} while(poll_joystick(PORT1, READ_BIOS) & JOY_SELECT);

		clear_fix();
	}
}

// task_enum callback, called for each task
BOOL task_disp_enum(PTASK task, void *user_data)
{
	// Display task information
	textoutf(0, *((PDWORD)user_data), FIX_PALETTE, 0, "%06X %04X %.8s %06X",
		task, task->prio, &task->tag1, task->context.pc);
		
	// Increment line counter
	*((PDWORD)user_data) += 1;
	
	return TRUE;
}

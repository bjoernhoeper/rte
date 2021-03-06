/*
*   The Framework was generated by the ACPLT/OV Builder.
*
*   Builder version: 1.0.0
*   Date of file generation:  21-Nov-2001 (10:25:25)
*/

#ifndef OV_COMPILE_LIBRARY_tasklib
#define OV_COMPILE_LIBRARY_tasklib
#endif


#include "tasklib.h"
#include "libov/ov_macros.h"
#include "libov/ov_scheduler.h"
#include "libov/ov_supervise.h"
#include "libov/ov_time.h"


OV_DLLFNCEXPORT void tasklib_SchedulerTask_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	ov_object_startup(pobj);

	tasklib_SchedulerTask_activate(Ov_StaticPtrCast(tasklib_Task, pobj));
}

OV_DLLFNCEXPORT void tasklib_SchedulerTask_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
	ov_scheduler_unregister(pobj);
	ov_object_shutdown(pobj);
}

OV_DLLFNCEXPORT void tasklib_SchedulerTask_activate(
             OV_INSTPTR_tasklib_Task          ptask
) {
	OV_TIME acttime;
	OV_INSTPTR_ov_object pobj;

	
	pobj = Ov_PtrUpCast(ov_object, ptask);
	ov_time_gettime(&acttime);
	switch (ptask->v_actimode) {
		case 0:
			ov_scheduler_unregister(pobj);
			break;
		case 1:
			ov_scheduler_register(pobj, tasklib_SchedulerTask_execute);
			ov_scheduler_setreleventtime(pobj, &ptask->v_cycletime);
			ov_time_add(&ptask->v_proctime, &acttime, &ptask->v_cycletime);
			break;
		case 2:
			ov_scheduler_register(pobj, tasklib_SchedulerTask_execute);
			ov_scheduler_setreleventtime(pobj, &ptask->v_cycletime);
			ov_time_gettime(&acttime);
			ov_time_add(&ptask->v_proctime, &acttime, &ptask->v_cycletime);
			break;
		case 3:
			ov_scheduler_register(pobj, tasklib_SchedulerTask_execute);
			ov_scheduler_setabseventtime(pobj, &ptask->v_alarmtime);
			ptask->v_proctime = ptask->v_alarmtime;
			break;
		case 4:
			ov_scheduler_register(pobj, tasklib_SchedulerTask_execute);
			ov_scheduler_setabseventtime(pobj, &ptask->v_alarmtime);
			ptask->v_proctime = ptask->v_alarmtime;
			break;
	}
}

OV_DLLFNCEXPORT void tasklib_SchedulerTask_execute(
	OV_INSTPTR_ov_object pobj
) {
	OV_INSTPTR_tasklib_Task		       	pchildtask;
	OV_TIME 				acttime;
	OV_INSTPTR_tasklib_Task 		ptask;
	OV_JUMPBUFFER				jumpbuffer;

	ptask = Ov_StaticPtrCast(tasklib_Task, pobj);
	ov_time_gettime(&acttime);

	switch (ptask->v_actimode) {
		case 0:
			ov_scheduler_unregister(pobj);
			break;
		case 1:
			ov_scheduler_setreleventtime(pobj, &ptask->v_cycletime);
			ov_time_add(&ptask->v_proctime, &acttime, &ptask->v_cycletime);
			break;
		case 2:
			if (ov_time_compare(&ptask->v_alarmtime, &acttime) <=0) {
				ptask->v_actimode = 0;
				ov_scheduler_unregister(pobj);
			}
			else {
				ov_scheduler_setreleventtime(pobj, &ptask->v_cycletime);
				ov_time_add(&ptask->v_proctime, &acttime, &ptask->v_cycletime);
			}
			break;
		case 3:
			ptask->v_actimode = 0;
			ov_scheduler_unregister(pobj);
			break;
		case 4:
			ptask->v_actimode = 1;
			ov_scheduler_setreleventtime(pobj, &ptask->v_cycletime);
			ov_time_add(&ptask->v_proctime, &acttime, &ptask->v_cycletime);
			break;
	}

	if (ptask->v_supervised) {
		if(ov_supervise_setjmp(jumpbuffer) == 0) {
			ov_supervise_start(&ptask->v_cycletime, &jumpbuffer);
			pchildtask = Ov_GetFirstChild(tasklib_tasklist, ptask);
			tasklib_Task_executechilds(ptask, acttime);
			ov_supervise_end();
		}
		else {
		     	ptask->v_actimode = 0;
		     	ptask->v_ErrState = 1;
		}
	}
	else {
	     	tasklib_Task_executechilds(ptask, acttime);
	}
}


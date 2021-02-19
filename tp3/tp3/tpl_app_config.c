/**
 * @file tpl_app_config.c
 *
 * @section desc File description
 *
 * OS data structure generated from application test
 * Automatically generated by goil on Fri Feb 19 16:18:57 2021
 * from root OIL file tp3.oil
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005-2007
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#include "tpl_app_config.h"

#include "tpl_os_internal_types.h"
#include "tpl_machine.h"
#include "tpl_os_it.h"
#include "tpl_os_it_kernel.h"
#include "tpl_os_alarm_kernel.h"
#include "tpl_os_alarm.h"
#include "tpl_os_rez_kernel.h"
#include "tpl_os_rez.h"
#include "tpl_os_event_kernel.h"
#include "tpl_os_event.h"
#include "tpl_os_action.h"
#include "tpl_os_kernel.h"
#include "tpl_os_definitions.h"


#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Declaration of resources IDs
 */

/* Resource resource_distance */
#define resource_distance_id 0
CONST(ResourceType, AUTOMATIC) resource_distance = resource_distance_id;

/*=============================================================================
 * Declaration of processes IDs
 */

/* Task Detection_contact identifier */
#define Detection_contact_id 0
CONST(TaskType, AUTOMATIC) Detection_contact = Detection_contact_id;

/* Task Detection_distance identifier */
#define Detection_distance_id 1
CONST(TaskType, AUTOMATIC) Detection_distance = Detection_distance_id;

/* Task Navigation identifier */
#define Navigation_id 2
CONST(TaskType, AUTOMATIC) Navigation = Navigation_id;

/* ISR isr_button_left identifier */
#define isr_button_left_id 3
/* ISR isr_button_right identifier */
#define isr_button_right_id 4
/* ISR isr_button_start identifier */
#define isr_button_start_id 5
/* ISR isr_button_stop identifier */
#define isr_button_stop_id 6
/*=============================================================================
 * Declaration of Alarm IDs
 */

/* Alarm alarmForContact identifier */
#define alarmForContact_id 0
CONST(AlarmType, AUTOMATIC) alarmForContact = alarmForContact_id;

/* Alarm alarmForDistance identifier */
#define alarmForDistance_id 1
CONST(AlarmType, AUTOMATIC) alarmForDistance = alarmForDistance_id;

/* Alarm alarmForNaviation identifier */
#define alarmForNaviation_id 2
CONST(AlarmType, AUTOMATIC) alarmForNaviation = alarmForNaviation_id;

#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Definition and initialization of regular Resource related structures
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*-----------------------------------------------------------------------------
 * Resource descriptor of resource resource_distance
 *
 * Tasks which use this resource :
 * Detection_contact
 * Detection_distance
 * Navigation
 */
VAR(tpl_resource, OS_VAR) resource_distance_rez_desc = {
  /* ceiling priority of the resource */  3,
  /* owner previous priority          */  0,
  /* owner of the resource            */  INVALID_PROC_ID,
#if WITH_OSAPPLICATION == YES
  /* OS Application id                */  
#endif    
  /* next resource in the list        */  NULL
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2VAR(tpl_resource, AUTOMATIC, OS_APPL_DATA)
tpl_resource_table[RESOURCE_COUNT] = {
  &resource_distance_rez_desc,
  &res_sched_rez_desc  
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Counters related defines and structures
 */
/*-----------------------------------------------------------------------------
 * Counter SystemCounter descriptor
 */

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_tick, OS_CONST) OSTICKSPERBASE = 1;
CONST(tpl_tick, OS_CONST) OSMAXALLOWEDVALUE = 2000;
CONST(tpl_tick, OS_CONST) OSMINCYCLE = 1;

#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_counter, OS_VAR) SystemCounter_counter_desc = {
  /* ticks per base       */  1,
  /* max allowed value    */  2000,
  /* minimum cycle        */  1,
  /* current tick         */  0,
  /* current date         */  0,
#if WITH_OSAPPLICATION == YES
    /* OS Application id    */  
#endif
    /* first alarm          */  NULL_PTR,
    /* next alarm to raise  */  NULL_PTR
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#include "tpl_os_kernel.h"          /* tpl_schedule */
#include "tpl_os_timeobj_kernel.h"  /* tpl_counter_tick */
#include "tpl_machine_interface.h"  /* tpl_switch_context_from_it */
#include "AT91SAM7.h" /* AT91C_AIC_ISCR */

#define OS_START_SEC_VAR_8BIT
#include "tpl_memmap.h"
VAR(u8, OS_VAR) check_buttons_period = 0;
#define OS_STOP_SEC_VAR_8BIT
#include "tpl_memmap.h"

#define OS_START_SEC_CODE
#include "tpl_memmap.h"

FUNC(void, OS_CODE) tpl_tick_it_timer1()
{
  tpl_status  need_rescheduling = NO_SPECIAL_CODE;

  need_rescheduling |= tpl_counter_tick(&SystemCounter_counter_desc);

  
  check_buttons_period++;
  if (check_buttons_period == 10)
  {
    /* Call check_buttons_status() (via interrupts)
     * which will check if buttons are pressed or not.
     */
     *AT91C_AIC_ISCR = (1 << AT91C_PERIPHERAL_ID_IRQ0);
     check_buttons_period = 0;
  }

  if (need_rescheduling == NEED_RESCHEDULING)
  {
    tpl_schedule_from_running();
#if WITH_SYSTEM_CALL == NO
    if (tpl_kern.need_switch != NO_NEED_SWITCH) {
      tpl_switch_context_from_it(
        &(tpl_kern.s_old->context),
        &(tpl_kern.s_running->context)
      );
    }
#endif
  }
}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Task related defines and structures
 */
/*-----------------------------------------------------------------------------
 * Task Detection_contact descriptor
 */
#define APP_Task_Detection_contact_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * Task Detection_contact function prototype
 */
FUNC(void, OS_APPL_CODE) Detection_contact_function(void);
#define APP_Task_Detection_contact_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task Detection_contact stack
 */
#define APP_Task_Detection_contact_START_SEC_STACK
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) Detection_contact_stack_zone[512/sizeof(tpl_stack_word)];
#define APP_Task_Detection_contact_STOP_SEC_STACK
#include "tpl_memmap.h"

#define Detection_contact_STACK {Detection_contact_stack_zone, 512}

/*
 * Task Detection_contact context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) Detection_contact_int_context;

#define Detection_contact_CONTEXT &Detection_contact_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"



#if WITH_AUTOSAR_TIMING_PROTECTION == YES
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#endif

/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*
 * Resources used by task Detection_contact
 *
 * resource_distance
 */

/*
 * Static descriptor of task Detection_contact
 */
CONST(tpl_proc_static, OS_CONST) Detection_contact_task_stat_desc = {
  /* context                  */  Detection_contact_CONTEXT,
  /* stack                    */  Detection_contact_STACK,
  /* entry point (function)   */  Detection_contact_function,
  /* internal ressource       */  NULL,
  /* task id                  */  Detection_contact_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  3,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* pointer to the timing
     protection descriptor    */  $TIMING_PROT_REF$
#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task Detection_contact
 */
VAR(tpl_proc, OS_VAR) Detection_contact_task_desc = {
  /* resources                      */  NULL,
#if WITH_MEMORY_PROTECTION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_MEMORY_PROTECTION */
  /* activate count                 */  0,
  /* task priority                  */  3,
  /* task state                     */  AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Task Detection_distance descriptor
 */
#define APP_Task_Detection_distance_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * Task Detection_distance function prototype
 */
FUNC(void, OS_APPL_CODE) Detection_distance_function(void);
#define APP_Task_Detection_distance_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task Detection_distance stack
 */
#define APP_Task_Detection_distance_START_SEC_STACK
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) Detection_distance_stack_zone[512/sizeof(tpl_stack_word)];
#define APP_Task_Detection_distance_STOP_SEC_STACK
#include "tpl_memmap.h"

#define Detection_distance_STACK {Detection_distance_stack_zone, 512}

/*
 * Task Detection_distance context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) Detection_distance_int_context;

#define Detection_distance_CONTEXT &Detection_distance_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"



#if WITH_AUTOSAR_TIMING_PROTECTION == YES
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#endif

/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*
 * Resources used by task Detection_distance
 *
 * resource_distance
 */

/*
 * Static descriptor of task Detection_distance
 */
CONST(tpl_proc_static, OS_CONST) Detection_distance_task_stat_desc = {
  /* context                  */  Detection_distance_CONTEXT,
  /* stack                    */  Detection_distance_STACK,
  /* entry point (function)   */  Detection_distance_function,
  /* internal ressource       */  NULL,
  /* task id                  */  Detection_distance_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  2,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* pointer to the timing
     protection descriptor    */  $TIMING_PROT_REF$
#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task Detection_distance
 */
VAR(tpl_proc, OS_VAR) Detection_distance_task_desc = {
  /* resources                      */  NULL,
#if WITH_MEMORY_PROTECTION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_MEMORY_PROTECTION */
  /* activate count                 */  0,
  /* task priority                  */  2,
  /* task state                     */  AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Task Navigation descriptor
 */
#define APP_Task_Navigation_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * Task Navigation function prototype
 */
FUNC(void, OS_APPL_CODE) Navigation_function(void);
#define APP_Task_Navigation_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task Navigation stack
 */
#define APP_Task_Navigation_START_SEC_STACK
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) Navigation_stack_zone[512/sizeof(tpl_stack_word)];
#define APP_Task_Navigation_STOP_SEC_STACK
#include "tpl_memmap.h"

#define Navigation_STACK {Navigation_stack_zone, 512}

/*
 * Task Navigation context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) Navigation_int_context;

#define Navigation_CONTEXT &Navigation_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"



#if WITH_AUTOSAR_TIMING_PROTECTION == YES
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#endif

/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*
 * Resources used by task Navigation
 *
 * resource_distance
 */

/*
 * Static descriptor of task Navigation
 */
CONST(tpl_proc_static, OS_CONST) Navigation_task_stat_desc = {
  /* context                  */  Navigation_CONTEXT,
  /* stack                    */  Navigation_STACK,
  /* entry point (function)   */  Navigation_function,
  /* internal ressource       */  NULL,
  /* task id                  */  Navigation_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  1,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* pointer to the timing
     protection descriptor    */  $TIMING_PROT_REF$
#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task Navigation
 */
VAR(tpl_proc, OS_VAR) Navigation_task_desc = {
  /* resources                      */  NULL,
#if WITH_MEMORY_PROTECTION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_MEMORY_PROTECTION */
  /* activate count                 */  0,
  /* task priority                  */  1,
  /* task state                     */  AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of ISR2 related defines and structures
 */
/*-----------------------------------------------------------------------------
 * ISR isr_button_left descriptor
 */
#define APP_ISR_isr_button_left_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * ISR isr_button_left function prototype
 */
FUNC(void, OS_APPL_CODE) isr_button_left_function(void);
#define APP_ISR_isr_button_left_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * ISR isr_button_left stack
 */
#define APP_ISR_isr_button_left_START_SEC_STACK
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) isr_button_left_stack_zone[500/sizeof(tpl_stack_word)];
#define APP_ISR_isr_button_left_STOP_SEC_STACK
#include "tpl_memmap.h"

#define isr_button_left_STACK {isr_button_left_stack_zone, 500}

/*
 * ISR isr_button_left context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) isr_button_left_int_context;

#define isr_button_left_CONTEXT &isr_button_left_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"



/*
  No timing protection
 */

#if WITH_AUTOSAR_TIMING_PROTECTION == YES
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#endif

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of ISR isr_button_left
 */
CONST(tpl_proc_static, OS_CONST) isr_button_left_isr_stat_desc = {
  /* context                  */  isr_button_left_CONTEXT,
  /* stack                    */  isr_button_left_STACK,
  /* entry point (function)   */  isr_button_left_function,
  /* internal ressource       */  NULL,
  /* ISR id                   */  isr_button_left_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  4,
  /* max activation count     */  1,
  /* task type                */  IS_ROUTINE,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* pointer to the timing
     protection descriptor    */  $TIMING_PROT_REF$
#endif
};

CONST(tpl_isr_static, OS_CONST) isr_button_left_isr_helper = {
  /* helper */ NULL,
  /* next */   NULL,
  /* id */     isr_button_left_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of ISR isr_button_left
 */
VAR(tpl_proc, OS_VAR) isr_button_left_isr_desc = {
  /* resources                      */  NULL,
#if WITH_MEMORY_PROTECTION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_MEMORY_PROTECTION */
  /* activate count                 */  0,
  /* ISR priority                   */  4,
  /* ISR state                      */  SUSPENDED,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* activation allowed             */  ,TRUE
#endif
};
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * ISR isr_button_right descriptor
 */
#define APP_ISR_isr_button_right_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * ISR isr_button_right function prototype
 */
FUNC(void, OS_APPL_CODE) isr_button_right_function(void);
#define APP_ISR_isr_button_right_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * ISR isr_button_right stack
 */
#define APP_ISR_isr_button_right_START_SEC_STACK
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) isr_button_right_stack_zone[500/sizeof(tpl_stack_word)];
#define APP_ISR_isr_button_right_STOP_SEC_STACK
#include "tpl_memmap.h"

#define isr_button_right_STACK {isr_button_right_stack_zone, 500}

/*
 * ISR isr_button_right context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) isr_button_right_int_context;

#define isr_button_right_CONTEXT &isr_button_right_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"



/*
  No timing protection
 */

#if WITH_AUTOSAR_TIMING_PROTECTION == YES
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#endif

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of ISR isr_button_right
 */
CONST(tpl_proc_static, OS_CONST) isr_button_right_isr_stat_desc = {
  /* context                  */  isr_button_right_CONTEXT,
  /* stack                    */  isr_button_right_STACK,
  /* entry point (function)   */  isr_button_right_function,
  /* internal ressource       */  NULL,
  /* ISR id                   */  isr_button_right_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  4,
  /* max activation count     */  1,
  /* task type                */  IS_ROUTINE,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* pointer to the timing
     protection descriptor    */  $TIMING_PROT_REF$
#endif
};

CONST(tpl_isr_static, OS_CONST) isr_button_right_isr_helper = {
  /* helper */ NULL,
  /* next */   NULL,
  /* id */     isr_button_right_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of ISR isr_button_right
 */
VAR(tpl_proc, OS_VAR) isr_button_right_isr_desc = {
  /* resources                      */  NULL,
#if WITH_MEMORY_PROTECTION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_MEMORY_PROTECTION */
  /* activate count                 */  0,
  /* ISR priority                   */  4,
  /* ISR state                      */  SUSPENDED,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* activation allowed             */  ,TRUE
#endif
};
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * ISR isr_button_start descriptor
 */
#define APP_ISR_isr_button_start_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * ISR isr_button_start function prototype
 */
FUNC(void, OS_APPL_CODE) isr_button_start_function(void);
#define APP_ISR_isr_button_start_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * ISR isr_button_start stack
 */
#define APP_ISR_isr_button_start_START_SEC_STACK
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) isr_button_start_stack_zone[500/sizeof(tpl_stack_word)];
#define APP_ISR_isr_button_start_STOP_SEC_STACK
#include "tpl_memmap.h"

#define isr_button_start_STACK {isr_button_start_stack_zone, 500}

/*
 * ISR isr_button_start context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) isr_button_start_int_context;

#define isr_button_start_CONTEXT &isr_button_start_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"



/*
  No timing protection
 */

#if WITH_AUTOSAR_TIMING_PROTECTION == YES
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#endif

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of ISR isr_button_start
 */
CONST(tpl_proc_static, OS_CONST) isr_button_start_isr_stat_desc = {
  /* context                  */  isr_button_start_CONTEXT,
  /* stack                    */  isr_button_start_STACK,
  /* entry point (function)   */  isr_button_start_function,
  /* internal ressource       */  NULL,
  /* ISR id                   */  isr_button_start_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  4,
  /* max activation count     */  1,
  /* task type                */  IS_ROUTINE,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* pointer to the timing
     protection descriptor    */  $TIMING_PROT_REF$
#endif
};

CONST(tpl_isr_static, OS_CONST) isr_button_start_isr_helper = {
  /* helper */ NULL,
  /* next */   NULL,
  /* id */     isr_button_start_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of ISR isr_button_start
 */
VAR(tpl_proc, OS_VAR) isr_button_start_isr_desc = {
  /* resources                      */  NULL,
#if WITH_MEMORY_PROTECTION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_MEMORY_PROTECTION */
  /* activate count                 */  0,
  /* ISR priority                   */  4,
  /* ISR state                      */  SUSPENDED,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* activation allowed             */  ,TRUE
#endif
};
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * ISR isr_button_stop descriptor
 */
#define APP_ISR_isr_button_stop_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * ISR isr_button_stop function prototype
 */
FUNC(void, OS_APPL_CODE) isr_button_stop_function(void);
#define APP_ISR_isr_button_stop_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * ISR isr_button_stop stack
 */
#define APP_ISR_isr_button_stop_START_SEC_STACK
#include "tpl_memmap.h"
VAR(tpl_stack_word, OS_APPL_DATA) isr_button_stop_stack_zone[500/sizeof(tpl_stack_word)];
#define APP_ISR_isr_button_stop_STOP_SEC_STACK
#include "tpl_memmap.h"

#define isr_button_stop_STACK {isr_button_stop_stack_zone, 500}

/*
 * ISR isr_button_stop context
 */
#define OS_START_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"
VAR(arm_context, OS_VAR) isr_button_stop_int_context;

#define isr_button_stop_CONTEXT &isr_button_stop_int_context

#define OS_STOP_SEC_VAR_NOINIT_32BIT
#include "tpl_memmap.h"



/*
  No timing protection
 */

#if WITH_AUTOSAR_TIMING_PROTECTION == YES
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
#endif

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of ISR isr_button_stop
 */
CONST(tpl_proc_static, OS_CONST) isr_button_stop_isr_stat_desc = {
  /* context                  */  isr_button_stop_CONTEXT,
  /* stack                    */  isr_button_stop_STACK,
  /* entry point (function)   */  isr_button_stop_function,
  /* internal ressource       */  NULL,
  /* ISR id                   */  isr_button_stop_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  4,
  /* max activation count     */  1,
  /* task type                */  IS_ROUTINE,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* pointer to the timing
     protection descriptor    */  $TIMING_PROT_REF$
#endif
};

CONST(tpl_isr_static, OS_CONST) isr_button_stop_isr_helper = {
  /* helper */ NULL,
  /* next */   NULL,
  /* id */     isr_button_stop_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of ISR isr_button_stop
 */
VAR(tpl_proc, OS_VAR) isr_button_stop_isr_desc = {
  /* resources                      */  NULL,
#if WITH_MEMORY_PROTECTION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_MEMORY_PROTECTION */
  /* activate count                 */  0,
  /* ISR priority                   */  4,
  /* ISR state                      */  SUSPENDED,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
  /* activation allowed             */  ,TRUE
#endif
};
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#include "tpl_machine.h"


/* Interrupt functions */

#define OS_START_SEC_CODE
#include "tpl_memmap.h"

FUNC(void, OS_CODE) tpl_interrupt_source_1()
{

  tpl_tick_it_timer1();

  systick_isr_C_function(); 
}

FUNC(void, OS_CODE) tpl_interrupt_source_2()
{

  nxt_motor_isr_C_function(); 
}

FUNC(void, OS_CODE) tpl_interrupt_source_5()
{

  spi_isr_C_function(); 
}

FUNC(void, OS_CODE) tpl_interrupt_source_7()
{

  bt_isr_C_function(); 
}

FUNC(void, OS_CODE) tpl_interrupt_source_9()
{

  twi_isr_C_function(); 
}

FUNC(void, OS_CODE) tpl_interrupt_source_10()
{

  systick_low_priority_C_function(); 
}

FUNC(void, OS_CODE) tpl_interrupt_source_12()
{

  i2c_timer_isr_C_function(); 
}

FUNC(void, OS_CODE) tpl_interrupt_source_15()
{

  tpl_central_interrupt_handler(5); 
}

FUNC(void, OS_CODE) tpl_interrupt_source_16()
{

  tpl_central_interrupt_handler(3); 
}

FUNC(void, OS_CODE) tpl_interrupt_source_17()
{

  tpl_central_interrupt_handler(4); 
}

FUNC(void, OS_CODE) tpl_interrupt_source_18()
{

  tpl_central_interrupt_handler(6); 
}

FUNC(void, OS_CODE) tpl_interrupt_source_30()
{

  check_buttons_status_function(); 
}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"


/* Interrupt table vector */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONST(tpl_it_vector_entry, OS_CONST) tpl_it_vector[31] = {
  { tpl_null_it, (void *)NULL },
  { tpl_interrupt_source_1, (void *)NULL },
  { tpl_interrupt_source_2, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_interrupt_source_5, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_interrupt_source_7, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_interrupt_source_9, (void *)NULL },
  { tpl_interrupt_source_10, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_interrupt_source_12, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_interrupt_source_15, (void *)NULL },
  { tpl_interrupt_source_16, (void *)NULL },
  { tpl_interrupt_source_17, (void *)NULL },
  { tpl_interrupt_source_18, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_null_it, (void *)NULL },
  { tpl_interrupt_source_30, (void *)NULL }
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
/*=============================================================================
 * Definition and initialization of process tables (tasks and isrs)
 */
CONSTP2CONST(tpl_proc_static, AUTOMATIC, OS_APPL_DATA)
tpl_stat_proc_table[TASK_COUNT+ISR_COUNT+1] = {
  &Detection_contact_task_stat_desc,
  &Detection_distance_task_stat_desc,
  &Navigation_task_stat_desc,
  &isr_button_left_isr_stat_desc,
  &isr_button_right_isr_stat_desc,
  &isr_button_start_isr_stat_desc,
  &isr_button_stop_isr_stat_desc,
  &idle_task_static
};

CONSTP2VAR(tpl_proc, AUTOMATIC, OS_APPL_DATA)
tpl_dyn_proc_table[TASK_COUNT+ISR_COUNT+1] = {
  &Detection_contact_task_desc,
  &Detection_distance_task_desc,
  &Navigation_task_desc,
  &isr_button_left_isr_desc,
  &isr_button_right_isr_desc,
  &isr_button_start_isr_desc,
  &isr_button_stop_isr_desc,
  &idle_task
};

CONSTP2CONST(tpl_isr_static, AUTOMATIC, OS_APPL_DATA)
tpl_isr_stat_table[ISR_COUNT] = {
  &isr_button_left_isr_helper,
  &isr_button_right_isr_helper,
  &isr_button_start_isr_helper,
  &isr_button_stop_isr_helper
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Alarm related structures
 */
/*-----------------------------------------------------------------------------
 * Alarm alarmForContact descriptor
 *
 * This alarm does the activation of task Detection_contact
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


CONST(tpl_task_activation_action, OS_CONST) alarmForContact_action = {
  {
    /* action function  */  tpl_action_activate_task
  },
  /* task id            */  Detection_contact_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

 
#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_alarm_static, OS_CONST) alarmForContact_static = {
  {
    /* pointer to counter           */  &SystemCounter_counter_desc,
    /* pointer to the expiration    */  tpl_raise_alarm
#if (WITH_TRACE == YES)
    /* id of the alarm for tracing  */  , alarmForContact_id
#endif
#if WITH_OSAPPLICATION == YES
    /* OS application id            */  , 
#endif
  },
  /* action of the alarm  */  (tpl_action *)&alarmForContact_action
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_time_obj, OS_VAR) alarmForContact_alarm_desc = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&alarmForContact_static,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  100,
    /* date                         */  100,
    /* State of the alarm           */  ALARM_AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Alarm alarmForDistance descriptor
 *
 * This alarm does the activation of task Detection_distance
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


CONST(tpl_task_activation_action, OS_CONST) alarmForDistance_action = {
  {
    /* action function  */  tpl_action_activate_task
  },
  /* task id            */  Detection_distance_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

 
#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_alarm_static, OS_CONST) alarmForDistance_static = {
  {
    /* pointer to counter           */  &SystemCounter_counter_desc,
    /* pointer to the expiration    */  tpl_raise_alarm
#if (WITH_TRACE == YES)
    /* id of the alarm for tracing  */  , alarmForDistance_id
#endif
#if WITH_OSAPPLICATION == YES
    /* OS application id            */  , 
#endif
  },
  /* action of the alarm  */  (tpl_action *)&alarmForDistance_action
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_time_obj, OS_VAR) alarmForDistance_alarm_desc = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&alarmForDistance_static,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  300,
    /* date                         */  300,
    /* State of the alarm           */  ALARM_AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Alarm alarmForNaviation descriptor
 *
 * This alarm does the activation of task Navigation
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


CONST(tpl_task_activation_action, OS_CONST) alarmForNaviation_action = {
  {
    /* action function  */  tpl_action_activate_task
  },
  /* task id            */  Navigation_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

 
#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_alarm_static, OS_CONST) alarmForNaviation_static = {
  {
    /* pointer to counter           */  &SystemCounter_counter_desc,
    /* pointer to the expiration    */  tpl_raise_alarm
#if (WITH_TRACE == YES)
    /* id of the alarm for tracing  */  , alarmForNaviation_id
#endif
#if WITH_OSAPPLICATION == YES
    /* OS application id            */  , 
#endif
  },
  /* action of the alarm  */  (tpl_action *)&alarmForNaviation_action
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_time_obj, OS_VAR) alarmForNaviation_alarm_desc = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&alarmForNaviation_static,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  200,
    /* date                         */  200,
    /* State of the alarm           */  ALARM_AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2VAR(tpl_time_obj, AUTOMATIC, OS_APPL_DATA)
  tpl_alarm_table[ALARM_COUNT] = {
  &alarmForContact_alarm_desc,
  &alarmForDistance_alarm_desc,
  &alarmForNaviation_alarm_desc
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Declaration of flags functions
 */
/* $FLAGSFUNCTIONS$ */


/*=============================================================================
 * Definition and initialization of Ready List structures
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_proc_id, OS_VAR) tpl_priority_0_fifo[1];
VAR(tpl_proc_id, OS_VAR) tpl_priority_1_fifo[1];
VAR(tpl_proc_id, OS_VAR) tpl_priority_2_fifo[1];
VAR(tpl_proc_id, OS_VAR) tpl_priority_3_fifo[4];
VAR(tpl_proc_id, OS_VAR) tpl_priority_4_fifo[12];

VAR(tpl_fifo_state, OS_VAR) tpl_fifo_rw[5] = {
  { 0 , 0 },
  { 0 , 0 },
  { 0 , 0 },
  { 0 , 0 },
  { 0 , 0 }
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_priority_level, OS_CONST) tpl_ready_list[5] = {
  { tpl_priority_0_fifo, 1 },
  { tpl_priority_1_fifo, 1 },
  { tpl_priority_2_fifo, 1 },
  { tpl_priority_3_fifo, 4 },
  { tpl_priority_4_fifo, 12 }
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2CONST(char, AUTOMATIC, OS_APPL_DATA) proc_name_table[TASK_COUNT + ISR_COUNT] = {
  "Detection_contact",
  "Detection_distance",
  "Navigation",
  "isr_button_left",
  "isr_button_right",
  "isr_button_start",
  "isr_button_stop"
};
#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/* End of file tpl_app_config.c */

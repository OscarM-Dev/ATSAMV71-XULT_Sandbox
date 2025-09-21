/*******************************************************************************/
/**
\file       app_scheduler.h
\brief      Task scheduler function prototypes
\author     Abraham Tezmol
\version    0.1
\date       09/09/2008
*/

#ifndef APP_SCHEDULER_H        /*prevent duplicated includes*/
#define APP_SCHEDULER_H

/*-- Includes ----------------------------------------------------------------*/

#include "compiler.h"
#include "typedefs.h"

/*****************************************************************************************************
* Declaration of module wide TYPES
*****************************************************************************************************/
typedef enum
{
    SUSPENDED,
    READY,
    RUNNING
}tTaskStates;

/* Task Scheduler Tasks IDs definitions */
typedef enum  
{   
    TASKS_1_MS,
    TASKS_2_MS_A,
    TASKS_2_MS_B,
    TASKS_10_MS,
    TASKS_50_MS,
    TASKS_100_MS,
    TASK_NULL,
} tSchedulerTimeTasks_ID;

//Prority tasks IDs.
typedef enum 
{
    TASKP_1,
    TASKP_2,
    TASKP_3,
    TASKP_4,
    TASKP_5
} tSchedulerPriorityTasks_ID;

//Control structure for priority triggered tasks.
typedef struct
{
    tSchedulerPriorityTasks_ID TaskId;
    tPtr_to_function           ptrTask;
    uint8_t                    Priority;
} tPriorityTask;

//Control structure for time triggered tasks.
typedef struct 
{
    tSchedulerTimeTasks_ID TaskId;  //Identifies and indicates task periodicity.
    tPtr_to_function       ptrTask;
    tTaskStates            enTaskState;
    uint8_t                u8Priority;  //Priority related to task periodicity.
} tSchedulingTask;

/*****************************************************************************************************
* Definition of module wide MACROS / #DEFINE-CONSTANTS 
*****************************************************************************************************/



/* Global Task Scheduler Status definitions */
#define    TASK_SCHEDULER_INIT              0x00u
#define    TASK_SCHEDULER_RUNNING           0x01u
#define    TASK_SCHEDULER_OVERLOAD_1MS      0x02u
#define    TASK_SCHEDULER_OVERLOAD_2MS_A    0x03u
#define    TASK_SCHEDULER_OVERLOAD_2MS_B    0x04u
#define    TASK_SCHEDULER_HALTED            0xAAu

#define    TASK_SCH_MAX_NUMBER_TIME_TASKS   0x06u
#define    TASK_SCH_MAX_NUMBER_PRIORITY_TASKS   5

#define    TASK_SCHEDULER_BASE_FREQ		    2000
/*****************************************************************************************************
* Definition of module wide VARIABLEs
*****************************************************************************************************/
extern tSchedulingTask TimeTriggeredTasks[TASK_SCH_MAX_NUMBER_TIME_TASKS];
extern tPriorityTask PriorityTriggeredTasks[TASK_SCH_MAX_NUMBER_PRIORITY_TASKS];

/*****************************************************************************************************
* Declaration of module wide FUNCTIONS
*****************************************************************************************************/
//Watchdog disable function.
void vfnWdtCtrl_Disable( void );

/** Scheduler Initalization (arming) */
void vfnScheduler_Init(void);

/** Scheduler kick-off function */
void vfnScheduler_Start(void);

/** Scheduler stop function */
void vfnScheduler_Stop(void);

/** Multi-thread round robin task scheduler */
void vfnTask_Scheduler(void);

// Schedule Point Function.
void vfnSchedulePoint( void );

// Event to change led1 priority
void vfnCycleLed1Priority(void);

// Gets the current priority assigned to LED1
uint8_t u8GetLed1Priority(void);

/*******************************************************************************/

#endif /* APP_SCHEDULER_H */
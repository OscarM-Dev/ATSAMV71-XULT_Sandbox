/****************************************************************************************************/
/**
\file       app_tasks.c
\brief      Multi-thread Task scheduler - list of tasks.
\author     Abraham Tezmol
\version    1.0
\date       07/09/2013
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Scheduler function prototypes definitions */
#include    "app_tasks.h"

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/
uint16_t dummy_counters_tasks[TASK_SCH_MAX_NUMBER_PRIORITY_TASKS] = { 0, 0, 0, 0, 0 };

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/

/*****************************************************************************************************
* Time triggered tasks.
*****************************************************************************************************/
/* List of tasks to be executed @ 1ms */
void TASKS_LIST_1MS( void )
{
    vfnSchedulePoint();
}

/* List of tasks to be executed @ 2ms, first group */
void TASKS_LIST_2MS_A(void)
{
    vfnSchedulePoint();
}
/* List of tasks to be executed @ 2ms, second group */
void TASKS_LIST_2MS_B( void )
{
    vfnSchedulePoint();
}
    
/* List of tasks to be executed @ 10ms */
void TASKS_LIST_10MS( void )
{
    vfnSchedulePoint();
}
/* List of tasks to be executed @ 50ms */
void TASKS_LIST_50MS( void )
{
    vfnSchedulePoint();
}
/* List of tasks to be executed @ 100ms */
void TASKS_LIST_100MS( void )
{
    vfnSchedulePoint();
}

/*****************************************************************************************************
* Priority triggered tasks.
*****************************************************************************************************/
/**
 * @brief This funcion executes priority task 1.
 * 
 */
void TASK_1( void )
{
    /* Led1 executes in the TASK_1 */
    vfnLed1Ctrl_BlinkingPattern();
    dummy_counters_tasks[TASKP_1]++;
}

/**
 * @brief This funcion executes priority task 2.
 * 
 */
void TASK_2( void )
{
    dummy_counters_tasks[TASKP_2]++;
}

/**
 * @brief This funcion executes priority task 3.
 * 
 */
void TASK_3( void )
{
    /* Led0 has a fixed task */
    vfnLed0Ctrl_BlinkingPattern();
    dummy_counters_tasks[TASKP_3]++;
}

/**
 * @brief This funcion executes priority task 4.
 * 
 */
void TASK_4( void )
{
    dummy_counters_tasks[TASKP_4]++;
}

/**
 * @brief This funcion executes priority task 5.
 * 
 */
void TASK_5( void )
{
    dummy_counters_tasks[TASKP_5]++;
}

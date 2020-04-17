#ifndef __WHEEL_TIMER__
#define __WHEEL_TIMER__
#include "LinkList.h"
#include "pthread.h"

typedef void (*app_callback)(void* arg, int sizeof_arg);

/*Gives the absolute slot no since the time WT has started*/
#define GET_WT_CURRENT_ABS_SLOT_NO(wt) ((wt->current_cycle_no * wt->wheel_size) + wt->current_clock_tick)

typedef struct wheel_timer_s {
 int clock_tick_interval;
 int wheel_size;
 int current_cycle_no;
 int current_clock_tick;
 pthread_t wheel_thread;
 ll_t* slots[0];
} wheel_timer_t;

typedef struct wheel_timer_elem_s{
 int time_interval;
 int execute_cycle_no;
 app_callback app_call_back;
 void* arg;
 int arg_size;
 char is_recurrence;
}wheel_timer_elem_t; 

//FUNCTIONS
wheel_timer_t* init_wheel_timer(int wheel_size, int clock_tick_interval);
static void* wheel_fn(void* arg);
void start_wheel_timer(wheel_timer_t* wt);
wheel_timer_elem_t* register_app_event(wheel_timer_t* wt,
                                        app_callback call_back,
                                        void* arg,
                                        int arg_size,
                                        int time_interval,
					char is_recurrence);
#endif

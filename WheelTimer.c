#include "WheelTimer.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

free_wheel_timer_element(wheel_timer_elem_t* wt_elem) {
  free(wt_elem->arg);
  free(wt_elem);
}

wheel_timer_t* init_wheel_timer(int wheel_size, int clock_tick_interval) {
  wheel_timer_t* wt = calloc(1, sizeof(wheel_timer_t) + wheel_size*sizeof(ll_t)); 
  wt->clock_tick_interval = clock_tick_interval;
  wt->wheel_size = wheel_size;
  memset(&(wt->wheel_thread), 0, sizeof(wheel_timer_t));

  for(int i=0;i<wheel_size;i++) {
    wt->slots[i] = init_singly_list();
  }
  return wt;
}

static void* wheel_fn(void* arg) {
   wheel_timer_t* wt = (wheel_timer_t*)arg;
   ll_t* slot_list = NULL;
   singly_ll_t* head = NULL, *prev_node = NULL;
   wheel_timer_elem_t* wt_elem = NULL;
   int absolute_slot_no = 0;

   while(1) {
     wt->current_clock_tick++;
     if(wt->current_clock_tick == wt->wheel_size)
         wt->current_clock_tick = 0;
     if(wt->current_clock_tick == 0)
	 wt->current_cycle_no++;

     sleep(wt->clock_tick_interval);
    
     slot_list = wt->slots[wt->current_clock_tick];
     absolute_slot_no = GET_WT_CURRENT_ABS_SLOT_NO(wt);
     printf("Wheel timer time: %d ", absolute_slot_no * wt->clock_tick_interval);
     if(isListEmpty(slot_list))
	printf("\n");

     ITERATE_LIST_BEGIN2(slot_list, head, prev_node) {
       wt_elem = (wheel_timer_elem_t*)head->data;
       /*printf("Current Clock tick: %d, Event current cycle: %d", 
 					wt->current_clock_tick, wt_elem->execute_cycle_no);*/
       if(wt->current_cycle_no == wt_elem->execute_cycle_no) {
	  wt_elem->app_call_back(wt_elem->arg, wt_elem->arg_size);

          if(wt_elem->is_recurrence) {
					
	    int next_abs_slot_no  = absolute_slot_no + (wt_elem->time_interval/wt->clock_tick_interval);
	    int next_cycle_no     = next_abs_slot_no / wt->wheel_size;
	    int next_slot_no      = next_abs_slot_no % wt->wheel_size;
	    wt_elem->execute_cycle_no 	 = next_cycle_no;

	    if(next_slot_no == wt->current_clock_tick){
		ITERATE_LIST_CONTINUE2(slot_list, head, prev_node);
	    }
	    remove_node(slot_list, head);
	    add_node(wt->slots[next_slot_no], head);
	  } 
          else {
            free_wheel_timer_element((wheel_timer_elem_t*)head->data);
	    singly_ll_delete_node(slot_list, head);
          }
       }
     } ITERATE_LIST_END2(slot_list, head, prev_node);
   } 
   return NULL;
}

void start_wheel_timer(wheel_timer_t* wt) {
  if(pthread_create(&wt->wheel_thread, NULL, wheel_fn, (void*)wt)) {
     printf("Wheel thread intialization failed...\n");
     exit(0);
  }
}

wheel_timer_elem_t* register_app_event(wheel_timer_t* wt, 
					app_callback call_back,
					void* arg, 
					int arg_size, 
					int time_interval,
					char is_recursive) {
   if (!wt || !call_back) return NULL;

   wheel_timer_elem_t* wt_elem = calloc(1, sizeof(wheel_timer_elem_t));

   wt_elem->time_interval = time_interval;
   wt_elem->app_call_back = call_back;
   wt_elem->arg = calloc(1, arg_size);
   memcpy(wt_elem->arg, arg, arg_size);
   wt_elem->arg_size = arg_size;
   wt_elem->is_recurrence = is_recursive;

   int absolute_slot = GET_WT_CURRENT_ABS_SLOT_NO(wt);
   int register_next_abs_slot = absolute_slot + (wt_elem->time_interval / wt->clock_tick_interval);
   wt_elem->execute_cycle_no = register_next_abs_slot / wt->wheel_size;
   int slot = register_next_abs_slot % wt->wheel_size;
   printf("In register_app_event() slot is: %d, time interval: %d \n", slot, wt_elem->time_interval);
   add_singly_list(wt->slots[slot], wt_elem); 
   return wt_elem;
}

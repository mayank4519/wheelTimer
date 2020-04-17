#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WheelTimer.h"

typedef struct routing_table_s {
  char address[16];	
  char gateway[16];	
  char oif[16];		
  int mask;	
}routing_table_t;

void print_data(routing_table_t* s) {
  printf("IP Address: %s/%d Gateway: %s Interface: %s\n", s->address, s->mask, s->gateway, s->oif);
}

void wrapper_func(void* arg, int arg_size) {
  routing_table_t *s = (routing_table_t*)arg;
  print_data(s);
}

void add_entry(routing_table_t *s, char* address, int mask, char* gateway, char* oif) {
  memset(s, '\0', sizeof(routing_table_t));
  memcpy(s->address, address, strlen(address));
  memcpy(s->gateway, gateway, strlen(gateway));
  s->mask = mask;
  memcpy(s->oif, oif, strlen(oif));
}

int main() {
  //create wheel timer object
  wheel_timer_t* wt = init_wheel_timer(10, 1);

  //start wheel timer in a seperate thread.
  start_wheel_timer(wt);

  routing_table_t *obj = malloc(sizeof(routing_table_t));
  routing_table_t *obj1 = malloc(sizeof(routing_table_t));
  add_entry(obj, "1.1.1.1", 32, "12.12.12.12", "EIF1");
  add_entry(obj1, "2.2.2.2", 32, "11.11.11.11", "EIF2");

  /*Now Wheel timer has started running in a separte thread. 
   *Register the events to be triggered with Wheel timer now.*/
  wheel_timer_elem_t *wt_elem = register_app_event(wt, wrapper_func, obj, 
						   sizeof(routing_table_t), 
						   3, // wrapper_func fn will be called every 3 seconds.
						   1); // Call wrapper_func fn indefintely.

  wt_elem = register_app_event(wt, wrapper_func, obj1, 
			       sizeof(routing_table_t), 
			       5, //wrapper_func fn will be called every 5 seconds.
			       0);// Call wrapper_func fn only once. 

  scanf("\n");
  return 0;
}

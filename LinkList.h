#ifndef __LINKLIST__
#define __LINKLIST__

#include <stdbool.h>

typedef struct singly_ll_s{
  void* data;
  struct singly_ll_s* next;
}singly_ll_t;

typedef struct ll_s {
  int node_count;
  singly_ll_t* head;
}ll_t;

ll_t* init_singly_list(); 
bool isListEmpty(ll_t* ll);
int add_singly_list(ll_t* list, void* data);
int add_node(ll_t* list, singly_ll_t* node);
int singly_ll_delete_node(ll_t*, singly_ll_t*);
int remove_node(ll_t* list, singly_ll_t* node);

#define GET_HEAD_SINGLY_LL(ll) (ll->head) 
#define DEC_NODE_COUNT_SINGLY_LL(ll) (ll->node_count--) 

#define ITERATE_LIST_BEGIN(list_ptr, node_ptr)                              \
    {                                                                       \
    singly_ll_t *_node_ptr = NULL;                                     \
    node_ptr = GET_HEAD_SINGLY_LL(list_ptr);                                \
    for(; node_ptr!= NULL; node_ptr = _node_ptr){                           \
        _node_ptr = node_ptr->next; 

#define ITERATE_LIST_END  }} 

 /* delete safe loop*/
#define ITERATE_LIST_BEGIN2(list_ptr, node_ptr, prev)                       \
    {                                                                       \
    singly_ll_t *_node_ptr = NULL; prev = NULL;                        \
    node_ptr = GET_HEAD_SINGLY_LL(list_ptr);                                \
    for(; node_ptr!= NULL; node_ptr = _node_ptr){                           \
        _node_ptr = node_ptr->next; 
         
#define ITERATE_LIST_CONTINUE2(list_ptr, node_ptr, prev)    \
         {if(node_ptr) prev = node_ptr;                     \
         continue;}
            
#define ITERATE_LIST_BREAK2(list_ptr, node_ptr, prev)       \
         break

#define ITERATIVE_LIST_NODE_DELETE2(list_ptr, node_ptr, prev)   \
        {if(node_ptr && prev == NULL){                          \
            list_ptr->head = node_ptr->next;                    \
        }                                                       \
        else if(node_ptr && prev){                              \
            prev->next = node_ptr->next;                        \
        }                                                       \
        free(node_ptr);                                         \
        list_ptr->node_count--;                                 \
        node_ptr = NULL;}
            
#define ITERATE_LIST_END2(list_ptr, node_ptr, prev)   \
             if(node_ptr) prev = node_ptr; }} 

#define LL_LESS_THAN(listptr, data1ptr, data2ptr)      \
    (listptr->order_comparison_fn(data1ptr, data2ptr) == -1)

#define LL_GREATER_THAN(listptr, data1ptr, data2ptr)   \
    (listptr->order_comparison_fn(data1ptr, data2ptr) == 1)

#define LL_EQUAL(listptr, data1ptr, data2ptr)          \
    (listptr->order_comparison_fn(data1ptr, data2ptr) == 0)
 
#endif

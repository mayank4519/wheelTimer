#include "LinkList.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

ll_t* init_singly_list() {
   return calloc(1, sizeof(ll_t));
}

bool isListEmpty(ll_t* ll) {
   if(ll->node_count == 0) 
     return true;
   return false;
}

int singly_ll_delete_node(ll_t* ll, singly_ll_t* node) {
  if(ll == NULL) return -1;
  if(ll->head == NULL || node == NULL) return 0;
  singly_ll_t* trav = NULL;

//case 1: If node is not the last node.
 if(node->next != NULL) {
   node->data = node->next->data;
   trav = node->next;
   node->next = node->next->next;
   free(trav);
   DEC_NODE_COUNT_SINGLY_LL(ll);
   return 0;   
 }  
//case 2: If node is the only node in ll.
 if(ll->node_count == 1 && GET_HEAD_SINGLY_LL(ll) == node) {
   free(node);
   GET_HEAD_SINGLY_LL(ll) = NULL;
   DEC_NODE_COUNT_SINGLY_LL(ll);
   return 0;
 }

//case 3: If node is the last node
  trav = GET_HEAD_SINGLY_LL(ll);
  while(trav->next != node) {
    trav = trav->next;
  }
  trav->next = NULL;
  free(node);
  DEC_NODE_COUNT_SINGLY_LL(ll);
  return 0;
}

singly_ll_t* get_node(ll_t* list, void* data) {
 singly_ll_t *temp = list->head;
 for(int i=0; i<list->node_count; i++) {
   if(temp->data == data)
     return temp;
   temp = temp->next;
 }
 return NULL;
}

singly_ll_t* create_node(void* data) {
  singly_ll_t* node = calloc(1, sizeof(singly_ll_t));
  node->data = data;
  return node;
}

int remove_node(ll_t* list, singly_ll_t* node) {
  if(list == NULL || list->head == NULL) return 0;
  if(node == NULL) {
    printf("%s(%d): Error: node is null\n", __FUNCTION__, __LINE__);
    return -1;
  }
  singly_ll_t* head = GET_HEAD_SINGLY_LL(list), *prev = NULL;
  
  if(head == node) {
    list->head = head->next;
    DEC_NODE_COUNT_SINGLY_LL(list);
    node->next = NULL;
    return 0;
  }
  prev = head;
  head = head->next;
  for(int i=1; i < list->node_count; i++) {
    if(head != node) {
      prev = head;
      head = head->next;
      continue;
    }
    prev->next = head->next;
    head->next = NULL;
    DEC_NODE_COUNT_SINGLY_LL(list);
    return 0;
  }
  printf("%s(%d): Error: node is nor found\n", __FUNCTION__, __LINE__);
  return -1;
}

int add_node(ll_t* list, singly_ll_t* node) {
  node->next = list->head;
  list->head = node;
  list->node_count++;
  return 0;
}

int add_singly_list(ll_t* list, void* data) {
 if(get_node(list, data) != NULL) {
   printf("Node is already present\n");
   return -1;
 }
 singly_ll_t* node = create_node(data);
 return add_node(list, node);
}

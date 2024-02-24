#include "../../stack.h"
#include "../../symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const size_t count = 15;
int main() {
  BTNodePtr *my_array = (BTNodePtr *) malloc(sizeof(BTNodePtr) * count);
  ptrStack *s = (ptrStack *) malloc(sizeof(ptrStack));
  if (!my_array || !s) {
    exit(666);
  }
  char a[2] = {'a','\0'};
  for (size_t i = 0; i < count; i++) {
    my_array[i] = (BTNodePtr) malloc(sizeof(struct BTNode_item));
    my_array[i]->name = (char *) calloc(2,sizeof(char));
    if (!my_array[i])
      exit(333);
      
    strncpy(my_array[i]->name,a,2);
    printf("%d ",a[0]); a[0] += 1; printf("%d\n",a[0]);
  }

  
  printf("=============== STACK TEST ===============\n");
  
  printf("Test 1) INIT\n");
  p_initS(s);
  if (s->arr == NULL && !s->cap && !s->top) {
    printf("Test 1 succesfull\n");
  }
  else
    printf("Test 1 failed\n");
    
  printf("Test 2) PUSH\n");
  bool test2 = true;
  for (size_t i = 0; i < count; i++) {
    p_pushS(s,my_array[i]);
    printf("%s must be equal to %s\n",s->arr[s->top]->name, my_array[i]->name);
    if (s->arr[s->top] != my_array[i] || s->arr[s->top]->name[0] != my_array[i]->name[0]) {
      test2 = false;
    }
  }
  if (test2) {
    printf("Test 2 succesfull\n");
  }
  else
    printf("Test 2 failed\n");
  
  printf("Test 3) POP\n");
  bool test3 = true;
  
  for (size_t i = 0; i < count; i++) {
    p_popS(s);
    if(i < count -1){
      if (s->arr[s->top] != my_array[count - i - 2] || s->arr[s->top]->name[0] != my_array[count - i - 2]->name[0]) {
        printf("%p should be %p\n",(void *)s->arr[s->top],(void *)my_array[count - i - 1]);
        test3 = false;
      }
    }
    else{
      if (s->arr != NULL) {
        test3 = false;
      }
    }
  }
  if (test3) {
    printf("%p should be (nil)\n",(void *)s->arr);
    printf("Test 3 succesfull\n");
  }
  else
    printf("Test 3 failed\n");
  if (!p_topS(s)) {
    printf("Test 4) TOP on blank stack\n");
    printf("Test 4 succesfull\n");
  }
  
  if (p_emptyS(s)) {
    printf("Test empty on empty stack succesfull\n");
  }
  else{
    printf("Test empty on empty stack failed\n");
  }
  
  bool test5 = true;
  printf("Test 5) TOPPUSH\n");
  if(p_topPushS(s,my_array[0])) test5 = false;
  
  for (size_t i = 1; i < count; i++) {
    BTNodePtr tmp = my_array[i-1];
    BTNodePtr returned = p_topPushS(s,my_array[i]);
    printf("%p must be equal to %p\n",(void *)tmp, (void *) returned);
    if (tmp != returned) {
      test5 = false;
    }
  }
  if(test5){
    printf("Test 5 succesfull\n");
  }
  else{
    printf("Test 5 failed\n");
  }
  
  
  if (!p_emptyS(s)) {
    printf("Test empty on non empty stack succesfull\n");
  }
  else{
    printf("Test empty on empty stack failed\n");
  }
  
  printf("Test 6) p_dTorS\n");
  p_dTorS(s);
  if (p_emptyS(s)) {
    printf("Test 6 succesfull\n");
  }
  else
    printf("Test 6 failed\n");
    
  p_pushS(s,my_array[0]);
  printf("Test TOPPOP\n");
  if(p_topPopS(s) == my_array[0]){
    printf("Test TOPPOP succesfull\n");
  }
  else{
    printf("Test TOPPOP failed\n");
  }
  p_pushS(s,my_array[0]);
  printf("Test TOPPOPPUSH\n");
  if(p_topPopPushS(s,my_array[1]) == my_array[0] && p_topS(s) == my_array[1]){
    printf("Test TOPPOPPUSH succesfull\n");
  }
  else{
    printf("Test TOPPOPPUSH failed\n");
  }
  
  p_dTorS(s);
  p_popS(s);
  
  printf("=============== END OF STACK TEST ===============\n");
  
  for (size_t i = 0; i < count; i++) {
    free(my_array[i]->name);
    free(my_array[i]);
  }
  free(my_array);
  free(s);
  return 0;
}
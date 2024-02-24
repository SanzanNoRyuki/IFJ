#include "../../stack.h"
#include <stdio.h>
#include <stdlib.h>

void printS(inStack *s);

int main(){
  
  inStack *stack = (inStack *) malloc(sizeof(inStack));
  if (!stack) {
    fprintf(stderr, "Allokace stacku se nezdřila:(\n");
    return -1;
  }
  initS(stack);
  //push
  pushS(stack,12);
  printS(stack);
  //pushTopS
  printf("pushTopS returned: %d\n",topPushS(stack,11));
  
  printS(stack);
  //allokace nové oblasti
  for (int i = 10; i > 0; i--) {
    pushS(stack,i);
    printS(stack);
  }
  //pop
  printf("Popping\n");
  popS(stack);
  //top
  printf("topS should return 2 and returned: %d\n",topS(stack));
  printS(stack);
  //topPop
  printf("topPopS should return 2 and returned: %d\n",topPopS(stack));
  
  printf("stack shlould look like:TOP --> {3,4,5,6,7,8,9,10,11,12} and look like: ");
  printS(stack);

  dTorS(stack);
  for (size_t i = 0; i < 50; i++) {
    pushS(stack,i);
    printS(stack);
  }
  for (size_t i = 50; i > 0; i--) {
    printS(stack);
    popS(stack);
  }
  pushS(stack,50);
  for (size_t i = 49; i > 0; i--) {
    printf("%d ", topPopPushS(stack,i));
    printS(stack);
  }
  
  
  free(stack);
  return 0;
}


void printS(inStack *s){
  if (emptyS(s)) {
    return;
  }
  printf("TOP --> {");
  for (int i = s->top; i > 0; i--) {
    printf("%d",s->arr[i]);
    if (i > 1) {
      printf(",");
    }
  }
  printf("}\n");
}
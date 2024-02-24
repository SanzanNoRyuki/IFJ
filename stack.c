/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file stack.c
 * @brief Stack implementation.
 *
 * @authors Vojtěch Ulej (xulejv00)
 * @author Daniel Andraško (xandra05)
 *
 *
 * @date 15.11.2019
 ******************************************************************************/

#include "stack.h"
#include "symtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "errors.h"
#include "symtable.h"
#include "expression_parser.h"
#include "stack.h"

void *CTor(unsigned short Cap, void *arr, size_t TYPE);  // Rozšiřuje zásobník.

bool fullS(inStack *s);

void initS(inStack *s){
  assert(s != NULL);
  s->arr = NULL;
  s->top = 0;
  s->cap = 0;
}

void pushS(inStack *s,unsigned short val){
  assert(s != NULL);

  if (emptyS(s) || fullS(s)) {                //Pokud je zásobník prázdný (není alokováno žádné volné místo), nebo plný,dojde k rozšíření kapacity
    s->arr = (unsigned short *) CTor(s->cap,s->arr, sizeof(unsigned short));
    s->cap += NEW_CAPACITY;
  }

  s->arr[++s->top] = val;   //Vložení prvku.

}

unsigned short topS(inStack *s){
  assert(s != NULL);
  if (emptyS(s)) {
    fprintf(stderr, "Stack is Empty!!!\n"); // Just for debugging delete when everything works
    return -1;
  }

  return s->arr[s->top];
}

void popS(inStack *s){
  assert(s != NULL);
  if (emptyS(s)) {
    return;
  }
  s->top--;
  if (s->top == 0) {
    dTorS(s);           //pokud je zásobník prázdný, dojde k uvolnění veškeré paměti.
    return;
  }
}

unsigned short topPushS(inStack *s,unsigned short val){
  assert(s != NULL);
  unsigned short returnVal = topS(s);
  pushS(s,val);
  return returnVal;
}

unsigned short topPopPushS(inStack *s, unsigned short val){
  assert(s != NULL);

  unsigned short returnVal = topS(s);
  popS(s);
  pushS(s,val);
  return returnVal;
}


unsigned short topPopS(inStack *s){
  assert(s != NULL);
  assert(!emptyS(s));

  unsigned short returnVal = topS(s);

  popS(s);

  return returnVal;
}


void dTorS(inStack *s){
  assert(s != NULL);

  free(s->arr);
  initS(s);
}

bool emptyS(inStack *s){
  assert(s != NULL);
  return s->cap == 0;
}
bool fullS(inStack *s){
  assert(s != NULL);
  return s->top == (s->cap - 1);
}

void *CTor(unsigned short Cap, void *arr,size_t TYPE){
  void *newPtr;

  newPtr = realloc(arr, (Cap + NEW_CAPACITY) * TYPE);
  if (!newPtr) {
    fprintf(stderr, "Internal error: Allocation of memory failed. Exiting with error code %d\n",ERR_INTERNAL);
    exit(ERR_INTERNAL);
  }
  return newPtr;
}

/**************************************/
/*        Pointer STACK_H             */
/**************************************/

bool p_fullS(ptrStack *s){
  return s->top == (s->cap - 1);
}

void p_initS(ptrStack *s){
  assert(s);
  s->arr = NULL;
  s->top = 0;
  s->cap = 0;
}

void p_pushS(ptrStack *s,BTNodePtr ptr){
  assert(s);
  if (p_emptyS(s) || p_fullS(s)) {                //Pokud je zásobník prázdný (není alokováno žádné volné místo), nebo plný,dojde k rozšíření kapacity
    s->arr = (BTNodePtr *) CTor(s->cap,s->arr, sizeof(BTNodePtr));
    s->cap += NEW_CAPACITY;
  }

  s->arr[++s->top] = ptr;   //Vložení prvku.
}

BTNodePtr p_topPushS(ptrStack *s,BTNodePtr ptr){
  assert(s);
  BTNodePtr RetVal = p_topS(s);
  p_pushS(s,ptr);
  return RetVal;
}

BTNodePtr p_topS(ptrStack *s){
  assert(s);
  if (p_emptyS(s)) {
    return 0x0;
  }
  return s->arr[s->top];
}

BTNodePtr p_topPopPushS(ptrStack *s, BTNodePtr ptr){
  assert(s);
  BTNodePtr RetVal = p_topPopS(s);
  p_pushS(s,ptr);
  return RetVal;
}

 BTNodePtr p_topPopS(ptrStack *s){
  assert(s);
  BTNodePtr RetVal = p_topS(s);
  p_popS(s);
  return RetVal;
}

void p_popS(ptrStack *s){
  assert(s);
  if (p_emptyS(s)) {
    return;
  }
  s->top--;
  if (s->top == 0) {
    p_dTorS(s);
    return;
  }
}

void p_dTorS(ptrStack *s){
  assert(s);
  free(s->arr);           //pokud je zásobník prázdný, dojde k uvolnění veškeré paměti.
  p_initS(s);
}

bool p_emptyS(ptrStack *s){
  assert(s);
  return (s->cap == 0);
}

/****************************************************/
/*            Stack for expression parser           */
/****************************************************/
bool ep_fullS(epStack *s){
 	return s->top == (s->cap - 1);
}

bool ep_emptyS(epStack *s){
	assert(s != NULL);
  	return s->cap == 0;
}

void ep_dTorS(epStack *s){
	assert(s);
	free(s->arr);           //pokud je zásobník prázdný, dojde k uvolnění veškeré paměti.
	ep_initS(s);
}


void ep_initS(epStack *s){
	assert(s);
	s->arr = NULL;
	s->top = 0;
	s->cap = 0;
}

void ep_pushS(epStack *s, const epitem *item){
	assert(s);

	if (ep_emptyS(s) || ep_fullS(s)) {                //Pokud je zásobník prázdný (není alokováno žádné volné místo), nebo plný,dojde k rozšíření kapacity
		s->arr = (epitem *) CTor(s->cap,s->arr, sizeof(epitem));
   		s->cap += NEW_CAPACITY;
	}

        s->arr[++s->top] = *item;
}

void ep_popS(epStack *s){
	assert(s);
	if(ep_emptyS(s) == true)
	  return;

	s->top--;
	if(s->top == 0)
	  ep_dTorS(s);

}

epitem *ep_topS(epStack *s){
	assert(s);
	if(ep_emptyS(s)) {
	return NULL;
	}
	return &(s->arr[s->top]);
}

epitem *ep_topPopS(epStack *s){
	assert(s);
	epitem *item = ep_topS(s);
	ep_popS(s);
	return item;
}

epitem *ep_topPopPushS(epStack *s, epitem *item){
	assert(s);
	epitem *item_ret = ep_topPopS(s);
	ep_pushS(s, item);
	return item_ret;
}

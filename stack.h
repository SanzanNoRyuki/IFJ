/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file stack.h
 * @brief Header file for the stack.
 *
 * @authors Vojtěch Ulej (xulejv00)
 * @author Daniel Andraško (xandra05)
 *
 *
 * @date 15.11.2019
 ******************************************************************************/

#ifndef STACK_H
#define STACK_H
#include <stdbool.h>
#include "symtable.h"
#include "scanner.h"
#include "expression_parser.h"
#define NEW_CAPACITY 10

typedef struct {                           /// Stack struct for indentation
	unsigned short *arr;                     /// Pointer to dynamic array
	unsigned short top;                      /// Top index
  unsigned short cap;											 /// Capacity
} inStack;


typedef struct {                           /// Stack struct for indentation
	BTNodePtr *arr;                               /// Pointer to dynamic array
	unsigned short top;                      /// Top index
  unsigned short cap;											 /// Capacity
} ptrStack;

typedef struct {		/// Stack struct for indentation
	epitem *arr;       /// Pointer to dynamic array
	unsigned short top;	/// Top index
	unsigned short cap;	/// Capacity
} epStack;

/**
  * @brief Function initialize stack.
  *
  * @param Pointer to stack.
	*/
void initS(inStack *s);

/**
  * @brief Function pushes param val on the top of stack s.
  *
  * @param Pointer to stack.
	* @param Value.
	*/
void pushS(inStack *s,unsigned short val);

/**
  * @brief Function returns top value, then pushes param val on the top of stack s.
  *
  * @param Pointer to stack.
	* @param Value.
	* @return Top value.
	*/
unsigned short topPushS(inStack *s,unsigned short val);

/**
  * @brief Returns top value.
  *
  * @param Pointer to stack.
	* @return Top value.
	*/
unsigned short topS(inStack *s);

/**
  * @brief Function returns top value, then pop it, then pushes param val on the top of stack s.
  *
  * @param Pointer to stack.
	* @param Value.
	*/
unsigned short topPopPushS(inStack *s, unsigned short val);

/**
  * @brief Function return top value from stack s, then pops it.
  *
  * @param Pointer to stack.
	* @return Top value.
	*/
unsigned short topPopS(inStack *s);

/**
  * @brief Function "pops" remove top value from stack s.
  *
  * @param Pointer to stack.
	*/
void popS(inStack *s);

/**
  * @brief Function deallocate stacks allocated array.
  *
  * @param Pointer to stack.
	* @param Value.
	*/
void dTorS(inStack *s);

/**
  * @brief Function returns true if stack is empty, false otherwise.
  *
  * @param Pointer to stack.
	* @return True when stack is empty.
	* @return False when stack isn't empty.
	*/
bool emptyS(inStack *s);


/****************************************************/
/*            Pointer stack for symtable            */
/****************************************************/
void p_initS(ptrStack *s);

/**
  * @brief Function pushes param val on the top of stack s.
  *
  * @param Pointer to stack.
	* @param Value.
	*/
void p_pushS(ptrStack *s,BTNodePtr ptr);

/**
  * @brief Function returns top value, then pushes param val on the top of stack s.
  *
  * @param Pointer to stack.
	* @param Value.
	* @return Top value.
	*/
BTNodePtr p_topPushS(ptrStack *s,BTNodePtr ptr);

/**
  * @brief Returns top value.
  *
  * @param Pointer to stack.
	* @return Top value.
	*/
BTNodePtr p_topS(ptrStack *s);

/**
  * @brief Function returns top value, then pop it, then pushes param val on the top of stack s.
  *
  * @param Pointer to stack.
	* @param Value.
	*/
BTNodePtr p_topPopPushS(ptrStack *s, BTNodePtr ptr);

/**
  * @brief Function return top value from stack s, then pops it.
  *
  * @param Pointer to stack.
	* @return Top value.
	*/
BTNodePtr p_topPopS(ptrStack *s);

/**
  * @brief Function "pops" remove top value from stack s.
  *
  * @param Pointer to stack.
	*/
void p_popS(ptrStack *s);

/**
  * @brief Function deallocate stacks allocated array.
  *
  * @param Pointer to stack.
	* @param Value.
	*/
void p_dTorS(ptrStack *s);

/**
  * @brief Function returns true if stack is empty, false otherwise.
  *
  * @param Pointer to stack.
	* @return True when stack is empty.
	* @return False when stack isn't empty.
	*/
bool p_emptyS(ptrStack *s);

 /****************************************************/
 /*            Stack for expression parser           */
 /****************************************************/

/**
  *@brief Function determines wether the stack is full or not.
  *
  *@param Pointer to the stack.
  *
  *@return Returns true if the stack is full.
  *@return Returns false if the stack is not full.
  */
bool ep_fullS(epStack *s);

/**
  *@brief Function determines wether the stack is empty or not.
  *
  *@param Pointer to the stack.
  *
  *@return Returns true if the stack is empty.
  *@return Returns false if the stack is not empty.
  */
bool ep_emptyS(epStack *s);

/**
  *@brief Function deallocates memmory of array from the stack.
  *
  *@param Pointer to the stack.
  */
void ep_dTorS(epStack *s);

/**
  *@brief Function initialize stack.
  *
  *@param Pointer to the stack.
  */
void ep_initS(epStack *s);

/**
  *@brief Function pushes param to the top of the stack
  *
  *@param Pointer to the stack
  *@param Pointer to the param
*/
void ep_pushS(epStack *s, const epitem *item);
/**
  *@brief Function removes the top value from stack
  *
  *@param Pointer to the stack
  */
void ep_popS(epStack *s);

/**
  *@brief Function return item at the top of the stack.
  *
  *@param Pointer to the stack.
  *
  *@return Returns item at the top of the stack, if there is any.
  */
epitem *ep_topS(epStack *s);

/**
  *@brief Function returns item at the top of the stack and removes it.
  *
  *@param Pointer to the stack.
  *
  *@param Returns item at the top of the stack, if there is any.
  */
epitem *ep_topPopS(epStack *s);

/**
  *@brief Function returns item at the top of the stack and pushes a new item.
  *
  *@param Pointer to the stack.
  *@param New item.
  *
  *@return Returns item at the top of the stack, if there is any.
  */
epitem *ep_topPopPushS(epStack *s, epitem *item);
#endif

/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file symtable.c
 * @brief Implementation of symtable.
 *
 * @authors Vojtěch Ulej (xulejv00)
 * @author Daniel Andraško (xandra05)
 *
 *
 * @date 22.10.2019
 ******************************************************************************/

#include "symtable.h"
#include "stack.h"

void BTNode_init(BTNodePtr *NodePtr){
	*NodePtr = NULL;
}


struct BTNode *BTNode_search(BTNodePtr *NodePtr, char *name){

	/* Auxiliary variables */
	BTNodePtr node_tmp = *NodePtr;

	while(node_tmp != NULL)
	{	// Searching tree
		if(strcmp(name, node_tmp->name) < 0)
		{
			node_tmp = node_tmp->LPtr;
		}
		else if(strcmp(name, node_tmp->name) > 0)
		{
			node_tmp = node_tmp->RPtr;
		}
		else
		{	// We found the node with identifier name
			return node_tmp;
		}
	}
	return NULL;
}

void BTNode_insert(BTNodePtr *NodePtr, char *name, BTData *data){

	/* Auxiliray variables */
	BTNodePtr node_parent;
	BTNodePtr node_tmp = *NodePtr;

	/* Searching for the node with given identifier */
	while(node_tmp != NULL)
	{
		node_parent = node_tmp;	// To define the parent of the new node

		if (strcmp(name, node_tmp->name) < 0)
		{	// Given identifier is lower than actual
			node_tmp = node_tmp->LPtr;
		}
		else if(strcmp(name, node_tmp->name) > 0)
		{	// Given identifier is higher than actual
			node_tmp = node_tmp->RPtr;
		}
		else
		{	// Given identifier is already in the tree
			node_tmp->data = data;
			return;
		}
	}	// There was none found

	/* Allocating memmory for new node */
	if((node_tmp = (BTNodePtr) malloc(sizeof(struct BTNode))) == NULL)
	{	// If allocating went wrong
		exit(ERR_INTERNAL);
	}

	/* Assigning data to the new node */
	node_tmp->name = name;
	node_tmp->data = data;
	node_tmp->LPtr = NULL;
	node_tmp->RPtr = NULL;

	/* Assigning child node */
	if(*NodePtr == NULL)
	{	// If the tree has only root
		*NodePtr = node_tmp;
	}
	else
	{	// If the has more than one node
		if(strcmp(node_tmp->name, node_parent->name) < 0)
		{
			node_parent->LPtr = node_tmp;
		}
		else if(strcmp(node_tmp->name, node_parent->name) > 0)
		{
			node_parent->RPtr = node_tmp;
		}
	}
}
 void Delete_left_null(BTNodePtr *NodePtr, BTNodePtr *NodeParent, char direction)
{
	BTNodePtr node_tmp = *NodePtr;
	BTNodePtr node_parent = *NodeParent;

	if(direction == 'L')
		node_parent->LPtr = node_tmp->RPtr;
	else if(direction == 'R')
		node_parent->RPtr = node_tmp->RPtr;
	else
		printf("Direction err\n");
	free(node_tmp->data);
	free(node_tmp);
	node_tmp = NULL;
}

void Delete_right_null(BTNodePtr *NodePtr, BTNodePtr *NodeParent, char direction)
{
	BTNodePtr node_tmp = *NodePtr;
	BTNodePtr node_parent = *NodeParent;

	if(direction == 'L')
		node_parent->LPtr = node_tmp->LPtr;
	else if(direction == 'R')
		node_parent->RPtr = node_tmp->LPtr;
	else
		printf("Direction err \n");
	free(node_tmp->data);
	free(node_tmp);
	node_tmp = NULL;
}

void ReplaceRightMostNode(BTNodePtr NodeReplaced, BTNodePtr *NodePtr){
	if(NodeReplaced != NULL && *NodePtr != NULL)
	{	// Control

		BTNodePtr node_tmp = *NodePtr;
		BTNodePtr node_parent;

		while(node_tmp->RPtr != NULL)
		{
			node_parent = node_tmp;
			node_tmp = node_tmp->RPtr;
		}// We are at the last right node

		/* Assigning data to the node which is replaced */
		NodeReplaced->name = node_tmp->name;
		NodeReplaced->data = node_tmp->data;

		node_parent->RPtr = node_tmp->LPtr;

		free(node_tmp);
		node_tmp = NULL;
	}
	else
	{
		return;
	}
}

char direction = ' ';

void BTNode_delete_direction(BTNodePtr *RootPtr, char *name, char direction){
        /* Auxiliray variables */
        BTNodePtr root_tmp = *RootPtr;
       BTNodePtr node_parent = root_tmp;

        if(root_tmp == NULL)
                return;

        if(strcmp(name, root_tmp->name) > 0)
       {// Moving to the right node
                node_parent = root_tmp;
               direction = 'R'; // On the right
                BTNode_delete_direction(&root_tmp->RPtr, name, direction);
                return;
        }
        else if(strcmp(name, root_tmp->name) < 0)
        {// Moving to the left node
                node_parent = root_tmp;
                direction = 'L'; // On the left
                BTNode_delete_direction(&root_tmp->LPtr, name, direction);
                return;
        }
        else
	{
		if(root_tmp->LPtr == NULL)
                {
                        Delete_left_null(&root_tmp, &node_parent, direction);
                }
                else if(root_tmp->RPtr == NULL)
                {
                        Delete_right_null(&root_tmp, &node_parent, direction);
                }
                else
                {// Node has both child nodes
                       ReplaceRightMostNode(root_tmp, &root_tmp->LPtr);
               }
        }
}



void BTNode_delete(BTNodePtr *RootPtr, char *name){
	/* Auxiliray variables */
	BTNodePtr root_tmp = *RootPtr;
	BTNodePtr node_parent = root_tmp;

	if(root_tmp == NULL)
		return;

	if(strcmp(name, root_tmp->name) > 0)
	{// Moving to the right node
		node_parent = root_tmp;
		direction = 'R'; // On the right
		BTNode_delete_direction(&root_tmp->RPtr, name, direction);
		return;
	}
	else if(strcmp(name, root_tmp->name) < 0)
	{// Moving to the left node
		node_parent = root_tmp;
		direction = 'L'; // On the left
		BTNode_delete_direction(&root_tmp->LPtr, name, direction);
		return;
	}
	else
	{// We have found the node
		if(root_tmp->LPtr == NULL)
		{
			Delete_left_null(&root_tmp, &node_parent, direction);
		}
		else if(root_tmp->RPtr == NULL)
		{
			Delete_right_null(&root_tmp, &node_parent, direction);
		}
		else
		{// Node has both child nodes
			ReplaceRightMostNode(root_tmp, &root_tmp->LPtr);
		}
	}
}


void BTNode_delete_all(BTNodePtr *NodePtr){

	ptrStack stack;
	p_initS(&stack);	// Stack with pointers

	do{
		if(*NodePtr == NULL)
		{	// We take a node from stack
			if(!p_emptyS(&stack))
			{
				*NodePtr = p_topPopS(&stack);
			}
		}
		else
		{
			if((*NodePtr)->RPtr != NULL)
			{	// We push right child node to the stack
				p_pushS(&stack, (*NodePtr)->RPtr);
			}

			BTNodePtr node_tmp = *NodePtr;
			*NodePtr = (*NodePtr)->LPtr;	// Go on the left
			free(node_tmp);	// deletes actual node
		}
	}while((*NodePtr != NULL) || (!p_emptyS(&stack)));
}

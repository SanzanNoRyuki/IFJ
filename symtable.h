/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file symtable.h
 * @brief Symtable for the IFJ project with language IFJ19
 *
 * @authors Vojtěch Ulej (xulejv00)
 * @author Daniel Andraško (xandra05)
 *
 *
 * @date 22.10.2019
 ******************************************************************************/

#ifndef SYMT_H
#define SYMT_H 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "errors.h"

/**
 * @enum Different types of data
 */
 typedef enum {
	type_int,	// Data type of integer
	type_float,	// Data type of float
	type_string,	// Data type of string
  type_multilinestr, // Data type of multiline string
	type_none	// Data type none
} Data_type;

/**
 * @enum Different types of identifier
 */
typedef enum {
    var_id,       /// name of variable
    func_id,   /// name of function
    param_id
} ID_type;

/**
 * @struct Data for function;
 */
typedef struct {
  bool defined;         /// Is the function already defined??
  char *label;          /// Lable of function
  int count_of_params;  /// Count of parameters... Obviously... LoL
  void *local_table;
} func_data;

/**
 * @union Data for ID.
 */
typedef union {
  char *str;
  double flt_val;
  int int_val;
} var_data;

typedef struct {
  int index;
  Data_type type;
  var_data data;
} param_data;

/**
 * @union Just data... 😀
 */
typedef union {
  func_data data_func;
  var_data data_var;
  param_data data_param;
} data;

/**
 * @struct The structure of data
 */
typedef struct
{
  ID_type   id_type;    /// Type of identifier
  Data_type data_type;	/// Data type
  data dat;             /// The real data.
} BTData;

/**
 * @struct The structure of tree node
 */
typedef struct BTNode
{
	char *name;	/// Identifier
	BTData *data; 	/// Usefull data

	struct BTNode *LPtr;	/// Pointer to the left child node
 	struct BTNode *RPtr;	/// Pointer to the right child node

} *BTNodePtr;

/**
  * @brief Function initialize a new node.
  *
  * @param Pointer to the root.
  */
void BTNode_init(BTNodePtr *NodePtr);

/**
  * @brief Function search for node.
  *
  * @param Pointer to the root.
  * @param Identifier of node we search for.
  *
  * @return Returns data structure of the node.
  */
struct BTNode *BTNode_search(BTNodePtr *NodePtr, char *name);

/**
  * @brief Function adds a new node with data Content.
  *
  * @param Pointer to the root.
  * @param Identifier of the node.
  * @param Content we want to add.
  */
void BTNode_insert(BTNodePtr *NodePtr, char *name,  BTData *data);

/**
  * @brief Auxiliary function for node delete
  *
  * @param Pointer to the root
  * @param Node name
  * @param Direction of the parent-child node
  */
void BTNode_delete_direction(BTNodePtr *RootPtr, char *name, char direction);

/**
  * @brief Function deletes node
  *
  * @param Pointer to the node
  * @param Pointer to the parent node
  * @param Direction of parent-child node
  */
void Delete_left_null(BTNodePtr *node_ptr, BTNodePtr *node_parent, char direction);

/**
  * @brief Function deletes node
  *
  * @param Pointer to the node
  * @param Pointer to the parent node
  * @param Direction of parent-child node
  */
void Delete_right_null(BTNodePtr *node_ptr, BTNodePtr *node_parent, char direction);

/**
 * @brief Auxiliary function which finds the node on the most right
 *
 * @param Pointer to the node we want to be replaced.
 * @param Pointer to the root of the tree.
 */
void ReplaceRightMostNode(BTNodePtr NodeReplaced, BTNodePtr *RootPtr);

/**
  * @brief Function deletes a node.
  *
  * @param Pointer to the root.
  * @param Key which identifies the node we want to delete.
  */
void BTNode_delete(BTNodePtr *NodePtr, char *name);

/**
  * @brief Function deletes the entire tree.
  *
  * @param Pointer to the root.
  */
void BTNode_delete_all(BTNodePtr *NodePtr);
#endif

/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file expression_parser.c
 * @brief Parser of expressions.
 *
 * @authors Roman Fulla (xfulla00)
 *
 *
 * @date 11.12.2019
 ******************************************************************************/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code_gen.h"
#include "errors.h"
#include "expression_parser.h"
#include "parser.h"
#include "scanner.h"
#include "stack.h"

#define DOLLAR 14                                                               // $ symbol value in the precedence table

int EP_ERR = ALL_OK;                                                            // Expression parser return when an error has occured
int EP_RET = EP_RET_END;                                                        // Expression parser return when successful

const char ep_table[15][15] = {                                                 // Precedence table
/* stack/scan| + | - | * | / | // | < | <= | > | >= | == | != | ( | ) | ID | $ |*/
/* | +  | */ {'>','>','<','<','<' ,'>','>' ,'>','>' ,'>' ,'>' ,'<','>','<' ,'>'},
/* | -  | */ {'>','>','<','<','<' ,'>','>' ,'>','>' ,'>' ,'>' ,'<','>','<' ,'>'},
/* | *  | */ {'>','>','>','>','>' ,'>','>' ,'>','>' ,'>' ,'>' ,'<','>','<' ,'>'},
/* | /  | */ {'>','>','>','>','>' ,'>','>' ,'>','>' ,'>' ,'>' ,'<','>','<' ,'>'},
/* | // | */ {'>','>','>','>','>' ,'>','>' ,'>','>' ,'>' ,'>' ,'<','>','<' ,'>'},
/* | <  | */ {'>','>','>','>','>' ,'>','>' ,'>','>' ,'>' ,'>' ,'<','>','<' ,'>'},
/* | <= | */ {'>','>','>','>','>' ,'>','>' ,'>','>' ,'>' ,'>' ,'<','>','<' ,'>'},
/* | >  | */ {'>','>','>','>','>' ,'>','>' ,'>','>' ,'>' ,'>' ,'<','>','<' ,'>'},
/* | >= | */ {'>','>','>','>','>' ,'>','>' ,'>','>' ,'>' ,'>' ,'<','>','<' ,'>'},
/* | == | */ {'>','>','>','>','>' ,'>','>' ,'>','>' ,'>' ,'>' ,'<','>','<' ,'>'},
/* | != | */ {'>','>','>','>','>' ,'>','>' ,'>','>' ,'>' ,'>' ,'<','>','<' ,'>'},
/* | (  | */ {'<','<','<','<','<' ,'<','<' ,'<','<' ,'<' ,'<' ,'<','=','<' ,'X'},
/* | )  | */ {'>','>','>','>','>' ,'>','>' ,'>','>' ,'>' ,'>' ,'X','>','X' ,'>'},
/* | ID | */ {'>','>','>','>','>' ,'>','>' ,'>','>' ,'>' ,'>' ,'X','>','X' ,'>'},
/* | $  | */ {'<','<','<','<','<' ,'<','<' ,'<','<' ,'<' ,'<' ,'<','X','<' ,'X'},
};

void token2stack(Token *token, epStack *stack, epitem *item) {                  // Transforms token to the stack item
  assert(token != NULL);
  assert(stack != NULL);
  assert(item != NULL);

  item->term  = true;
  item->prev  = ep_topS(stack);
  item->token = *token;

  if (item->prev == 0x0) {
    fprintf(stderr,"Precedence stack is empty.\n");
    exit(ERR_INTERNAL);
  }

  ep_pushS(stack,item);
}

int token2index(Token *token) {                                                 // Gets a table index from the token
  assert(token != NULL);

  if (EP_ERR != ALL_OK) return EP_ERR;                                          // Error return change protection

  switch (token->type) {
    case ADD:           return  0;                                              // +
    case SUB:           return  1;                                              // -
    case MUL:           return  2;                                              // *
    case DIV:           return  3;                                              // /
    case INT_DIV:       return  4;                                              // //

    case LESS:          return  5;                                              // <
    case LESS_EQ:       return  6;                                              // <=
    case MORE:          return  7;                                              // >
    case MORE_EQ:       return  8;                                              // >=
    case EQUAL:         return  9;                                              // ==
    case NOT_EQ:        return 10;                                              // !=

    case L_PAR:         return 11;                                              // (
    case R_PAR:         return 12;                                              // )

    case INT:                                                                   // Integer
    case DOUBLE:                                                                // Double
    case STR:                                                                   // String
    case MULTILINE_STR:                                                         // Multiline string
    case ID:            return 13;                                              // ID

    case KEY_WORD: if (token->attr.key_W == NONE) {
                        return 13;                                              // None
                   }
                   else {
                     fprintf(stderr,"Invalid token type.\n");
                     EP_ERR = ERR_SYNTAX;
                        return -1;
                   }

    case COLON:    EP_RET = EP_RET_COLON;                                       // Sets expression parser return to :
    case EOL:                                                                   // 14
    case _EOF:          return DOLLAR;                                          // $

    default:       fprintf(stderr,"Invalid token type.\n");
                   EP_ERR = ERR_SYNTAX;
                        return -1;
  }
}

epitem top_term(epStack *stack) {                                              // Gets the terminal closest to the top of the stack
  assert(stack != NULL);

  epitem *top_term = ep_topS(stack);
  if (top_term == 0x0) {
    fprintf(stderr,"Precedence stack is empty.\n");
    exit(ERR_INTERNAL);
  }

  for (int i = stack->top; i > 0; --i) {
    top_term = &(stack->arr[i]);
    if (top_term->term == true) break;
  }
  return *top_term;
}

void operand_check(int op2, int operation, int op1) {                           // Checks if operands are compatible with the operation
  switch (operation) {
    case ADD     :
    case LESS    :
    case LESS_EQ :
    case MORE    :
    case MORE_EQ : if (op2 == KEY_WORD || op1 == KEY_WORD) EP_ERR = ERR_SEM_TYPE;
                   else if (op2 == STR || op2 == MULTILINE_STR) {
                     if (op1 != STR && op1 != MULTILINE_STR && op1 != ID) {
                       EP_ERR = ERR_SEM_TYPE;
                     }
                   }
                   else if (op1 == STR || op1 == MULTILINE_STR) {
                     if (op2 != STR && op2 != MULTILINE_STR && op2 != ID) {
                       EP_ERR = ERR_SEM_TYPE;
                     }
                   }
                   break;

    case SUB     :
    case MUL     :
    case DIV     :
                   if (op2 == KEY_WORD || op1 == KEY_WORD) EP_ERR = ERR_SEM_TYPE;
                   else if (op2 == STR || op2 == MULTILINE_STR) EP_ERR = ERR_SEM_TYPE;
                   else if (op1 == STR || op1 == MULTILINE_STR) EP_ERR = ERR_SEM_TYPE;
                   break;

    case INT_DIV :
                   if (op2 != ID && op2 != INT) EP_ERR = ERR_SEM_TYPE;
                   if (op1 != ID && op1 != INT) EP_ERR = ERR_SEM_TYPE;
                   break;
  }

  if (EP_ERR != ALL_OK) fprintf(stderr,"Incorrect operation input.\n");
}

void process_rule(epStack *stack, int *op_idx) {                                // Finds appropriate rule and reduces accordingly
  assert(stack != NULL);

  epitem *op1 = ep_topS(stack);                                                 // Last operand
  if (op1 == 0x0) {
    fprintf(stderr,"Precedence stack is empty.\n");
    exit(ERR_INTERNAL);
  }

  if (op1->term == true) {                                                      // Last operand is terminal
    if      (op1->token.type == INT           ||
             op1->token.type == DOUBLE        ||
             op1->token.type == STR           ||
             op1->token.type == MULTILINE_STR ||
             op1->token.type == ID            ||
             op1->token.type == KEY_WORD        ) {                             // E -> ID
               op1->term  = false;
               op1->index = ++*op_idx;
               EP_ERR = code_gen_operand(op1);
    }
    else if (op1->token.type == R_PAR) {                                        // E -> (E)
      epitem *inside_par = op1->prev;

      if (inside_par->term == true) {
        fprintf(stderr,"Unexpected terminal on the expression stack.\n");
        EP_ERR = ERR_SYNTAX;
        return;
      }
      if (inside_par->prev->term != true) {
        fprintf(stderr,"Unexpected nonterminal on the expression stack.\n");
        EP_ERR = ERR_SYNTAX;
        return;
      }
      if (inside_par->prev->token.type != L_PAR) {
        fprintf(stderr,"Unexpected terminal on the expression stack.\n");
        EP_ERR = ERR_SYNTAX;
        return;
      }

      ep_popS(stack);
      ep_popS(stack);
      ep_popS(stack);
      ep_pushS(stack,inside_par);
    }
    else {
      fprintf(stderr,"Unexpected terminal on the expression stack.\n");
      EP_ERR = ERR_SYNTAX;
      return;
    }
  }
  else {                                                                        // Last operand is nonterminal
    epitem *operation = op1->prev;                                              // Operation
    if (operation->term != true) {
      fprintf(stderr,"Unexpected nonterminal on the expression stack.\n");
      EP_ERR = ERR_SYNTAX;
      return;
    }

    epitem *op2 = operation->prev;                                              // First operand
    if (op2->term == true) {
      fprintf(stderr,"Unexpected terminal on the expression stack.\n");
      EP_ERR = ERR_SYNTAX;
      return;
    }

    epitem result_val, *result = &result_val;                                   // Result -> op2 operation op1
    result->token.type = ASSIGN;
    result->term = false;
    result->index = ++*op_idx;
    result->prev = op2->prev;

    EP_ERR = code_gen_operand(result);                                          // Generate result code
    if (EP_ERR != ALL_OK) return;

    code_gen_type_init();
    bool are_str = false;                                                       // Switch for constant string addition

    switch (operation->token.type) {                                            // Operation type
      case ADD     :                                                            // E -> E + E
                     operand_check(op2->token.type,operation->token.type,op1->token.type);
                     if (EP_ERR != ALL_OK) return;

                     if (op2->token.type == STR || op2->token.type == MULTILINE_STR) {
                       if (op1->token.type == STR || op1->token.type == MULTILINE_STR) {
                         are_str = true;
                       }
                     }
                     else if (op2->token.type == DOUBLE && op1->token.type == INT) {
                       code_gen_int2double(op1);
                     }
                     else if (op1->token.type == DOUBLE && op2->token.type == INT) {
                       code_gen_int2double(op2);
                     }

                     if (op2->token.type == ID     || op1->token.type == ID     ||
                         op2->token.type == ASSIGN || op1->token.type == ASSIGN   ) {
                           code_gen_type(op2,op1);
                           code_gen_var_add(op2,op1,result);
                     }
                     else  code_gen_add(op2,op1,result,are_str);
                     break;
      case SUB     :                                                            // E -> E - E
                     operand_check(op2->token.type,operation->token.type,op1->token.type);
                     if (EP_ERR != ALL_OK) return;

                     if (op2->token.type == DOUBLE && op1->token.type == INT) {
                       code_gen_int2double(op1);
                     }
                     else if (op1->token.type == DOUBLE && op2->token.type == INT) {
                       code_gen_int2double(op2);
                     }

                     if (op2->token.type == ID     || op1->token.type == ID     ||
                         op2->token.type == ASSIGN || op1->token.type == ASSIGN   ) {
                           code_gen_type(op2,op1);
                           code_gen_var_sub(op2,op1,result);
                     }
                     else  code_gen_sub(op2,op1,result);
                     break;
      case MUL     :                                                            // E -> E * E
                     operand_check(op2->token.type,operation->token.type,op1->token.type);
                     if (EP_ERR != ALL_OK) return;

                     if (op2->token.type == DOUBLE && op1->token.type == INT) {
                       code_gen_int2double(op1);
                     }
                     else if (op1->token.type == DOUBLE && op2->token.type == INT) {
                       code_gen_int2double(op2);
                     }

                     if (op2->token.type == ID     || op1->token.type == ID     ||
                         op2->token.type == ASSIGN || op1->token.type == ASSIGN   ) {
                           code_gen_type(op2,op1);
                           code_gen_var_mul(op2,op1,result);
                     }
                     else  code_gen_mul(op2,op1,result);
                     break;
      case DIV     :                                                            // E -> E / E
                     operand_check(op2->token.type,operation->token.type,op1->token.type);
                     if (EP_ERR != ALL_OK) return;

                     if (op2->token.type == INT) code_gen_int2double(op2);
                     if (op1->token.type == INT) code_gen_int2double(op1);

                     if (op1->token.type == INT) {                              // Division by 0
                       if (op1->token.attr.int_num == 0) {
                         fprintf(stderr,"Division by zero!\n");
                         EP_ERR = ERR_ZERO_DIV;
                         return;
                       }
                     }
                     if (op1->token.type == DOUBLE) {
                       if (op1->token.attr.decimal_num == 0.0) {
                         fprintf(stderr,"Division by zero!\n");
                         EP_ERR = ERR_ZERO_DIV;
                         return;
                       }
                     }

                     if (op2->token.type == ID     || op1->token.type == ID     ||
                         op2->token.type == ASSIGN || op1->token.type == ASSIGN   ) {
                           code_gen_type(op2,op1);
                           code_gen_var_div(op2,op1,result);
                     }
                     else  code_gen_div(op2,op1,result);
                     break;
      case INT_DIV :                                                            // E -> E // E
                     operand_check(op2->token.type,operation->token.type,op1->token.type);
                     if (EP_ERR != ALL_OK) return;

                     if (op1->token.type == INT) {                              // Division by 0
                       if (op1->token.attr.int_num == 0) {
                         fprintf(stderr,"Division by zero!\n");
                         EP_ERR = ERR_ZERO_DIV;
                         return;
                       }
                     }

                     if (op2->token.type == ID     || op1->token.type == ID     ||
                         op2->token.type == ASSIGN || op1->token.type == ASSIGN   ) {
                           code_gen_type(op2,op1);
                           code_gen_var_int_div(op2,op1,result);
                     }
                     else  code_gen_int_div(op2,op1,result);
                     break;
      case LESS    :                                                            // E -> E < E
                     operand_check(op2->token.type,operation->token.type,op1->token.type);
                     if (EP_ERR != ALL_OK) return;

                     if (op2->token.type == DOUBLE && op1->token.type == INT) {
                       code_gen_int2double(op1);
                     }
                     else if (op1->token.type == DOUBLE && op2->token.type == INT) {
                       code_gen_int2double(op2);
                     }

                     if (op2->token.type == ID     || op1->token.type == ID     ||
                         op2->token.type == ASSIGN || op1->token.type == ASSIGN   ) {
                           code_gen_type(op2,op1);
                           code_gen_var_less(op2,op1,result);
                     }
                     else  code_gen_less(op2,op1,result);
                     break;
      case LESS_EQ :                                                            // E -> E <= E
                     operand_check(op2->token.type,operation->token.type,op1->token.type);
                     if (EP_ERR != ALL_OK) return;;

                     if (op2->token.type == DOUBLE && op1->token.type == INT) {
                       code_gen_int2double(op1);
                     }
                     else if (op1->token.type == DOUBLE && op2->token.type == INT) {
                       code_gen_int2double(op2);
                     }

                     if (op2->token.type == ID     || op1->token.type == ID     ||
                         op2->token.type == ASSIGN || op1->token.type == ASSIGN   ) {
                           code_gen_type(op2,op1);
                           code_gen_var_less_eq(op2,op1,result);
                     }
                     else  code_gen_less_eq(op2,op1,result);
                     break;
      case MORE    :                                                            // E -> E > E
                     operand_check(op2->token.type,operation->token.type,op1->token.type);
                     if (EP_ERR != ALL_OK) return;

                     if (op2->token.type == DOUBLE && op1->token.type == INT) {
                       code_gen_int2double(op1);
                     }
                     else if (op1->token.type == DOUBLE && op2->token.type == INT) {
                       code_gen_int2double(op2);
                     }

                     if (op2->token.type == ID     || op1->token.type == ID     ||
                         op2->token.type == ASSIGN || op1->token.type == ASSIGN   ) {
                           code_gen_type(op2,op1);
                           code_gen_var_more(op2,op1,result);
                     }
                     else  code_gen_more(op2,op1,result);
                     break;
      case MORE_EQ :                                                            // E -> E >= E
                     operand_check(op2->token.type,operation->token.type,op1->token.type);
                     if (EP_ERR != ALL_OK) return;

                     if (op2->token.type == DOUBLE && op1->token.type == INT) {
                       code_gen_int2double(op1);
                     }
                     else if (op1->token.type == DOUBLE && op2->token.type == INT) {
                       code_gen_int2double(op2);
                     }

                     if (op2->token.type == ID     || op1->token.type == ID     ||
                         op2->token.type == ASSIGN || op1->token.type == ASSIGN   ) {
                           code_gen_type(op2,op1);
                           code_gen_var_more_eq(op2,op1,result);
                     }
                     else  code_gen_more_eq(op2,op1,result);
                     break;
      case EQUAL   :                                                            // E -> E == E
                     if (op2->token.type == DOUBLE && op1->token.type == INT) {
                       code_gen_int2double(op1);
                     }
                     else if (op1->token.type == DOUBLE && op2->token.type == INT) {
                       code_gen_int2double(op2);
                     }

                     if (op2->token.type == ID     || op1->token.type == ID     ||
                         op2->token.type == ASSIGN || op1->token.type == ASSIGN   ) {
                           code_gen_type(op2,op1);
                           code_gen_var_equal(op2,op1,result);
                     }
                     else  code_gen_equal(op2,op1,result);
                     break;
      case NOT_EQ  :                                                            // E -> E != E
                     if (op2->token.type == DOUBLE && op1->token.type == INT) {
                       code_gen_int2double(op1);
                     }
                     else if (op1->token.type == DOUBLE && op2->token.type == INT) {
                       code_gen_int2double(op2);
                     }

                     if (op2->token.type == ID     || op1->token.type == ID     ||
                         op2->token.type == ASSIGN || op1->token.type == ASSIGN   ) {
                           code_gen_type(op2,op1);
                           code_gen_var_not_eq(op2,op1,result);
                     }
                     else  code_gen_not_eq(op2,op1,result);
                     break;
      default      : fprintf(stderr,"Unexpected nonoperation on the expression stack.\n");
                     EP_ERR = ERR_SYNTAX;
                     return;
    }

    ep_popS(stack);
    ep_popS(stack);
    ep_popS(stack);
    ep_pushS(stack,result);
  }
}

int expression_parser(FILE *file, Token *par_tkn_1, Token *par_tkn_2) {         // Function called by main parser
  assert(file != NULL);

  EP_RET = 0;                                                                   // Initialization of the return

  int idx1, idx2, op_idx = 0;
  Token   token_val, *token = &token_val;
  epitem  item_val,  *item = &item_val;
  epStack stack_val, *stack = &stack_val;
  const epitem *permanent_item = item;
  ep_initS(stack);

  item->term       = true;
  item->prev       = NULL;
  item->token.type = _EOF;
  ep_pushS(stack,item);                                                         // Pushes $ to the stack as the first item

  if      (par_tkn_1 != NULL) token  = par_tkn_1;                               // Acquisition of the first token
  else if (par_tkn_2 != NULL) token  = par_tkn_2;
  else                        EP_ERR = gimme_Token(file,token);

  idx1 = token2index(token);
  if (idx1 == 12 || idx1 == DOLLAR) {                                           // First token is ) or $
    fprintf(stderr,"Unexpected first token.\n");
    EP_ERR = ERR_SYNTAX;
  }
  if (EP_ERR != ALL_OK) return EP_ERR;

  token2stack(token,stack,item);                                                // Pushes first real item to the stack
  item = (epitem*) permanent_item;                                              // TODO REMOVE

  if (par_tkn_1 != NULL && par_tkn_2 != NULL) {                                 // Acquisition of the second token
       token = par_tkn_2;
  }
  else EP_ERR = gimme_Token(file,token);

  idx2 = token2index(token);
  if (EP_ERR != ALL_OK) return EP_ERR;

  do {                                                                          // Expression processing according to precedence table and rules
    switch (ep_table[idx1][idx2]) {
      case '=' :
      case '<' : token2stack(token,stack,item);
                 item = (epitem*) permanent_item;                               // TODO REMOVE
                 EP_ERR = gimme_Token(file,token);
                 break;
      case '>' : process_rule(stack,&op_idx);
                 break;
      default  : fprintf(stderr,"Illegal precedence table move.\n");
                 EP_ERR = ERR_SYNTAX;
                 break;
    }
    if (EP_ERR != ALL_OK) return EP_ERR;
    epitem tmp = top_term(stack);
    idx1 = token2index(&(tmp.token));
    idx2 = token2index(token);
    if (EP_ERR != ALL_OK) return EP_ERR;
  } while (idx1 != DOLLAR || idx2 != DOLLAR);

  code_gen_result(ep_topS(stack)->index);                                       // Assigns the result
  ep_popS(stack);
  ep_popS(stack);
  return EP_RET;                                                                // Returns what was used to terminate the expression
}

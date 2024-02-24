/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file expression_parser.h
 * @brief Header file for the expression parser.
 *
 * @authors Roman Fulla (xfulla00)
 *
 *
 * @date 9.12.2019
 ******************************************************************************/

#ifndef EXP_PAR_H
#define EXP_PAR_H


#include <stdbool.h>
#include <stdio.h>

#include "scanner.h"

#define EP_RET_END    0        /// Expression parser return if the expression ends with EOL/EOF.
#define EP_RET_COLON 10        /// Expression parser return if the expression ends with :.

typedef struct epitem {        /// Stack item.
  bool           term;         /// Terminal / Nonterminal.
  struct epitem *prev;         /// Pointer to the item lower on the stack.
  unsigned       index;        /// Operand index.
  Token          token;        /// Token.
} epitem;

/**
  * @brief Function processes expressions.
  *
  * @param Input file.
  * @param Token from the parser.
  * @param Second token from the parser.
  *
  * @return  0 if the expression is terminated by EOL or EOF.
  * @return 10 if the expression is terminated by colon.
  * @return Appropriate error code if necessary.
  */
int expression_parser(FILE *file, Token *par_tkn_1, Token *par_tkn_2);

#endif

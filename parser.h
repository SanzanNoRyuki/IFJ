/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file parser.h
 * @brief Header file for parser.
 *
 * @authors Vojtěch Ulej (xulejv00)
 *
 *
 * @date 16.11.2019
 ******************************************************************************/
#ifndef PARSER_H
#define PARSER_H

#include "code_gen.h"
#include "scanner.h"
#include "symtable.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  BTNodePtr GlobalTable;
  BTNodePtr LocalTable;

  long long int  label_gen;              /// Variable for generating unique labels
  bool defining_function;
  bool if_or_while;

} ParserData;

extern ParserData GlobalParserData;
extern code_gen *code;

/** @brief This function runs syntactic analysis.
  *
  * @returns Error code, or 0 in case everything is fine.
  *
  */
int Analysis(FILE *f);



#endif /* PARSER_H */
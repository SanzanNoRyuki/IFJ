/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file scanner.c
 * @brief Header file of lexical analysis.
 *
 * @authors Pavol Szepsi (xszeps00)
 *
 *
 * @date 15.11.2019
 ******************************************************************************/

#ifndef SCANNER_H
#define SCANNER_H

#include <stdlib.h>
#include <stdio.h>
#include "errors.h"
#include "dynamic_string.h"

typedef enum {
	DEF,
	ELSE,
	IF,
	NONE,
	PASS,
	RETURN,
	WHILE
}K_Word;

typedef enum {
	/* Specials */
	INDENT,
	DEDENT,
	_EOF,
	EOL,

	/* Identifier */
	ID,             /// Identifier
	KEY_WORD,
	/* Data types */
	INT,            /// Integer
	DOUBLE,
	STR,            /// string
	MULTILINE_STR,  /// multiline string might be comment

	/* Symbols, operators */
	ADD,            /// symbol +
	SUB,            /// symbol -
	DIV,            /// symbol /
	INT_DIV,        /// symbol //
	MUL,            /// symbol *
	LESS,           /// symbol <
	MORE,           /// symbol >
	EQUAL,          /// symbol ==
	LESS_EQ,        /// symbol <=
	MORE_EQ,        /// symbol >=
	NOT_EQ,         /// symbol !=
	L_PAR,          /// symbol (
	R_PAR,          /// symbol )
	COMMA,          /// symbol ,
	ASSIGN,         /// symbol =
	COLON,          /// symbol :

} T_type;

typedef union {
	char *str;                /// String
	K_Word key_W;             /// Keyword enum
	int int_num;              /// int value
	double decimal_num;       /// double value
} T_data;

typedef struct {
	T_type type;
	T_data attr;
} Token;

/**
  * @brief Main function of scanner.
  * This function scans the output and generate Token.
  *
  * @param src Sourcefile - AKA input
  * @param *token pointer to a Token
  *
  * @return error code (e.g. 0 - ALL_OK, 99 - ERR_INTERNAL, etc.)
  */
int gimme_Token(FILE *src, Token *token);


#endif


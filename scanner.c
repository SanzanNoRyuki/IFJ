/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file scanner.c
 * @brief Source file of lexical analysis.
 *
 * @authors Pavol Szepsi (xszeps00)
 *
 *
 * @date 15.11.2019
 ******************************************************************************/
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "scanner.h"
#include "stack.h"

FILE *src;

typedef enum {
	START,
	IDENTIFIER,
	NUMBER,
	NUMBER_POINT,
	NUMBER_POINT_FLOAT,
	NUMBER_EXPONENT,
	NUMBER_EXPONENT_SIGN,
	NUMBER_EXPONENT_END,
	ZERO,
	STRING,
	STRING_ESCAPE,
	STRING_ESCAPE_HEXA,
	STRING_ESCAPE_HEXA2,
	DOCUMENT_STRING1,
	DOCUMENT_STRING2,
	DOCUMENT_STRING3,
  DOCUMENT_STRING_ESCAPE,
	DOCUMENT_STRING_END1,
	DOCUMENT_STRING_END2,
	COMMENT,
	LESS_THAN,
	MORE_THAN,
	ASSIGN_TO,
	NOT,
	DIVISION,
	INDENTATION,
} state;

state s;

dm_str *str;
inStack st;
inStack *stack = &st;
bool first_token_call = true;

unsigned short indent_level = 0;
bool dedent = false;    // sluzi na zistenie, ci indent hodnota bola na zasobniku

int is_it_Keyword(dm_str* str, Token *token) {

	if (dm_str_cmp(str, "def") == 0) {
		token->type = KEY_WORD;
		token->attr.key_W = DEF;
	}
	else if (dm_str_cmp(str, "else") == 0) {
		token->type = KEY_WORD;
		token->attr.key_W = ELSE;
  }
	else if (dm_str_cmp(str, "if") == 0) {
		token->type = KEY_WORD;
		token->attr.key_W = IF;
	}
	else if (dm_str_cmp(str, "None") == 0) {
		token->type = KEY_WORD;
		token->attr.key_W = NONE;
	}
	else if (dm_str_cmp(str, "pass") == 0) {
		token->type = KEY_WORD;
		token->attr.key_W = PASS;
	}
	else if (dm_str_cmp(str, "return") == 0) {
		token->type = KEY_WORD;
		token->attr.key_W = RETURN;
	}
	else if (dm_str_cmp(str, "while") == 0) {
		token->type = KEY_WORD;
		token->attr.key_W = WHILE;
	}
	else {
		token->type = ID;
    token->attr.str = dm_str_first(str);
    return ALL_OK;
  }

	dm_str_free(str);
	return ALL_OK;
}

int handle_number_int(dm_str* str, Token *token) {

	token->type = INT;
	token->attr.int_num = (int) strtol(dm_str_first(str), NULL, 10);

	dm_str_free(str);
	return ALL_OK;
}

int handle_number_double(dm_str* str, Token *token) {

	token->type = DOUBLE;
	token->attr.decimal_num = strtod(dm_str_first(str), NULL);

	dm_str_free(str);
	return ALL_OK;
}

int gimme_Token(FILE *src, Token *token) {

  if (src == NULL) {
    return ERR_INTERNAL;
  }

  dm_str dm;
  str = &dm;
  dm_str_init(str);

  if (first_token_call) {
    initS(stack);
    pushS(stack, 0);
    first_token_call = false;
  }

	int ch = getc(src);
	ungetc(ch, src);
	if (ch == EOF) {
		if (topS(stack) != 0) {
			popS(stack);
			token->type = DEDENT;
		}
		else {
			token->type = _EOF;
		}
		dm_str_free(str);
		return ALL_OK;
	}

	if (s != INDENTATION) {
		s = START;
	}

  char hexa1;

	while (true) {

    char c = (char) getc(src);

		switch (s)
		{

		case START:
			if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_') {
				s = IDENTIFIER;
				dm_str_add_c(str, c);
			}
			else if (c >= '1' && c <= '9') {
				s = NUMBER;
				dm_str_add_c(str, c);
			}
			else if (c == '0') {
				s = ZERO;
				dm_str_add_c(str, c);
			}
			else if ((int)c == 39) {
				s = STRING;
			}
			else if (c == '"') {
				s = DOCUMENT_STRING1;
			}
			else if (c == '#') {
				s = COMMENT;
			}
			else if (c == '+') {
				token->type = ADD;
				dm_str_free(str);
				return ALL_OK;
			}
			else if (c == '-') {
				token->type = SUB;
				dm_str_free(str);
				return ALL_OK;
			}
      else if (c == '*') {
        token->type = MUL;
        dm_str_free(str);
        return ALL_OK;
      }
			else if (c == '/') {
				s = DIVISION;
			}
			else if (c == '<') {
				s = LESS_THAN;
			}
			else if (c == '>') {
				s = MORE_THAN;
			}
			else if (c == '=') {
				s = ASSIGN_TO;
			}
			else if (c == '!') {
				s = NOT;
			}
			else if (c == '(') {
				token->type = L_PAR;
				dm_str_free(str);
				return ALL_OK;
			}
			else if (c == ')') {
				token->type = R_PAR;
				dm_str_free(str);
				return ALL_OK;
			}
			else if (c == ',') {
				token->type = COMMA;
				dm_str_free(str);
				return ALL_OK;
			}
      else if (c == ':') {
        token->type = COLON;
        dm_str_free(str);
        return ALL_OK;
      }
			else if (isspace(c) && c != '\n') {
				s = START;
			}
			else if (c == '\n') {
				token->type = EOL;
				s = INDENTATION;
				dm_str_free(str);
				return ALL_OK;
			}
			else {
				ungetc(c, src);
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			break;

		case IDENTIFIER:
			if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || (c >= '0' && c <= '9')) {
				dm_str_add_c(str, c);
			}
			else {
				ungetc(c, src);
				s = START;
				return is_it_Keyword(str, token);
			}
			break;

		case NUMBER:
			if (c >= '0' && c <= '9') {
				dm_str_add_c(str, c);
			}
			else if (c == '.') {
				s = NUMBER_POINT;
				dm_str_add_c(str, c);
			}
			else if (c == 'E' || c == 'e') {
				s = NUMBER_EXPONENT;
				dm_str_add_c(str, c);
			}
			else {
				ungetc(c, src);
				s = START;
				return handle_number_int(str, token);
			}
			break;

		case ZERO:
			if (c == '0') {
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			else if (c == '.') {
				s = NUMBER_POINT;
				dm_str_add_c(str, c);
			}
			else if (c == 'E' || c == 'e') {
				s = NUMBER_EXPONENT;
				dm_str_add_c(str, c);
			}
			else {
				ungetc(c, src);
				s = START;
				return handle_number_int(str, token);
			}
			break;

		case NUMBER_POINT:
			if (c >= '0' && c <= '9') {
				s = NUMBER_POINT_FLOAT;
				dm_str_add_c(str, c);
			}
			else {
				ungetc(c, src);
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			break;

		case NUMBER_POINT_FLOAT:
			if (c >= '0' && c <= '9') {
				dm_str_add_c(str, c);
			}
			else if (c == 'E' || c == 'e') {
				s = NUMBER_EXPONENT;
				dm_str_add_c(str, c);
			}
			else {
				ungetc(c, src);
				s = START;
				return handle_number_double(str, token);
			}
			break;

		case NUMBER_EXPONENT:
			if (c == '+' || c == '-') {
				s = NUMBER_EXPONENT_SIGN;
				dm_str_add_c(str, c);
			}
			else if (c >= '0' && c <= '9') {
				s = NUMBER_EXPONENT_END;
				dm_str_add_c(str, c);
			}
			else {
				ungetc(c, src);
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			break;

		case NUMBER_EXPONENT_SIGN:
			if (c >= '0' && c <= '9') {
				s = NUMBER_EXPONENT_END;
				dm_str_add_c(str, c);
			}
			else {
				ungetc(c, src);
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			break;

		case NUMBER_EXPONENT_END:
			if (c >= '0' && c <= '9') {
				dm_str_add_c(str, c);
			}
			else {
				ungetc(c, src);
				s = START;
				return handle_number_double(str, token);
			}
			break;

		case STRING:
			if ((int)c > 31 && (int)c != 39 && (int)c != 92) {
				dm_str_add_c(str, c);
			}
			else if ((int)c == 39) {
        token->attr.str = dm_str_first(str);
				token->type = STR;
				return ALL_OK;
			}
			else if ((int)c == 92) {
				s = STRING_ESCAPE;
			}
			else {
				ungetc(c, src);
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			break;

		case STRING_ESCAPE:
			if (c == '"') {
				s = STRING;
				dm_str_add_c(str, '\"');
			}
			else if ((int)c == 39) {
				s = STRING;
				dm_str_add_c(str, '\'');
			}
			else if (c == 'n') {
				s = STRING;
				dm_str_add_c(str, '\n');
			}
			else if (c == 't') {
				s = STRING;
				dm_str_add_c(str, '\t');
			}
			else if ((int)c == 92) {
				s = STRING;
				dm_str_add_c(str, '\\');
			}
			else if (c == 'x') {
				s = STRING_ESCAPE_HEXA;
			}
			else if ((int)c > 31) {
				dm_str_add_c(str, 92);
				dm_str_add_c(str, c);
				s = STRING;
			}
			else {
				ungetc(c, src);
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			break;

		case STRING_ESCAPE_HEXA:
			if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
				hexa1 = c;
				s = STRING_ESCAPE_HEXA2;
			}
			else if ((int)c == 39) {
				dm_str_add_c(str, 92);
				dm_str_add_c(str, 'x');
        token->attr.str = dm_str_first(str);
				token->type = STR;
				return ALL_OK;
			}
			else if ((int)c == 92) {
				dm_str_add_c(str, 92);
				dm_str_add_c(str, 'x');
				s = STRING_ESCAPE;
			}
			else {
				ungetc(c, src);
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			break;

		case STRING_ESCAPE_HEXA2:
			if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
				s = STRING;
				dm_str_add_c(str, 92);
				dm_str_add_c(str, 'x');
				dm_str_add_c(str, hexa1);
				dm_str_add_c(str, c);
			}
			else if ((int)c == 39) {
				dm_str_add_c(str, 92);
			  dm_str_add_c(str, 'x');
				dm_str_add_c(str, hexa1);
        token->attr.str = dm_str_first(str);
				token->type = STR;
				return ALL_OK;
			}
			else if ((int)c == 92) {
				dm_str_add_c(str, 92);
				dm_str_add_c(str, 'x');
				dm_str_add_c(str, hexa1);
				s = STRING_ESCAPE;
			}
			else {
				ungetc(c, src);
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			break;

		case COMMENT:
			if (c == '\n' || c == EOF) {
				ungetc(c, src);
				s = START;
			}
			break;

		case DOCUMENT_STRING1:
			if (c == '"') {
				s = DOCUMENT_STRING2;
			}
			else {
				ungetc(c, src);
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			break;

		case DOCUMENT_STRING2:
			if (c == '"') {
				s = DOCUMENT_STRING3;
			}
			else {
				ungetc(c, src);
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			break;

		case DOCUMENT_STRING3:
      if ((int)c == 92) {
        s = DOCUMENT_STRING_ESCAPE;
      }
			else if (c == '"') {
				s = DOCUMENT_STRING_END1;
			}
			else if (c == EOF) {
				ungetc(c, src);
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			else {
				dm_str_add_c(str, c);
			}
			break;

    case DOCUMENT_STRING_ESCAPE:
      if (c == '"') {
        dm_str_add_c(str, '\"');
        s = DOCUMENT_STRING3;
      }
      else if ((int)c == 92) {
        dm_str_add_c(str, 92);
      }
      else if (c == EOF) {
        ungetc(c, src);
        s = START;
        dm_str_free(str);
        return ERR_LEX;
      }
      else {
        dm_str_add_c(str, 92);
        dm_str_add_c(str, c);
        s = DOCUMENT_STRING3;
      }
      break;

		case DOCUMENT_STRING_END1:
      if ((int)c == 92) {
        s = DOCUMENT_STRING_ESCAPE;
        dm_str_add_c(str, '"');
      }
			else if (c == '"') {
				s = DOCUMENT_STRING_END2;
			}
      else if (c == EOF) {
        ungetc(c, src);
        s = START;
        dm_str_free(str);
        return ERR_LEX;
      }
      else {
        s = DOCUMENT_STRING3;
        dm_str_add_c(str, '"');
        dm_str_add_c(str, c);
      }
			break;

		case DOCUMENT_STRING_END2:
      if ((int)c == 92) {
        s = DOCUMENT_STRING_ESCAPE;
        dm_str_add_c(str, '"');
        dm_str_add_c(str, '"');
      }
			else if (c == '"') {
        token->attr.str = dm_str_first(str);
				token->type = MULTILINE_STR;
				return ALL_OK;
			}
			else if (c == EOF) {
        ungetc(c, src);
        s = START;
        dm_str_free(str);
        return ERR_LEX;
      }
      else {
        s = DOCUMENT_STRING3;
        dm_str_add_c(str, '"');
        dm_str_add_c(str, '"');
        dm_str_add_c(str, c);
      }
			break;

		case LESS_THAN:
			if (c == '=') {
				s = START;
				token->type = LESS_EQ;
			}
			else {
				ungetc(c, src);
				s = START;
				token->type = LESS;
			}
			dm_str_free(str);
			return ALL_OK;

		case MORE_THAN:
			if (c == '=') {
				s = START;
				token->type = MORE_EQ;
			}
			else {
				ungetc(c, src);
				s = START;
				token->type = MORE;
			}
			dm_str_free(str);
			return ALL_OK;

		case ASSIGN_TO:
			if (c == '=') {
				s = START;
				token->type = EQUAL;
			}
			else {
				ungetc(c, src);
				s = START;
				token->type = ASSIGN;
			}
			dm_str_free(str);
			return ALL_OK;

		case NOT:
			if (c == '=') {
				s = START;
				token->type = NOT_EQ;
				dm_str_free(str);
				return ALL_OK;
			}
			else {
				ungetc(c, src);
				s = START;
				dm_str_free(str);
				return ERR_LEX;
			}
			break;

		case DIVISION:
			if (c == '/') {
				s = START;
				token->type = INT_DIV;
			}
			else {
				ungetc(c, src);
				s = START;
				token->type = DIV;
			}
			dm_str_free(str);
			return ALL_OK;

		case INDENTATION:
			if (c == ' ') {
				indent_level++;
			}
      else if (c == '#' || c == '"') {
        ungetc(c, src);
        s = START;
        indent_level = 0;
      }
			else {
				ungetc(c, src);

				if (indent_level > topS(stack)) {
					if (dedent) {
						return ERR_LEX;   // hodnota nebola na zasobniku
					}
					s = START;
					pushS(stack, indent_level);
					token->type = INDENT;
					indent_level = 0;
					dm_str_free(str);
					return ALL_OK;
				}
				else if (indent_level < topS(stack)) {
					popS(stack);
					token->type = DEDENT;
					dedent = true;
					dm_str_free(str);
					return ALL_OK;
				}
				else {
					dedent = false;
					s = START;
					indent_level = 0;
				}
			}
			break;
		}
	}
}

/* Testing scanner */

#include <stdio.h>

#include "../../scanner.h"

const char *TokenTypeToString(T_type type);

char token2str[][26] = {
  "INDENT",
  "DEDENT",
  "EOF",
  "EOL",
  "ID",
  "KEY_WORD",
  "INT",
  "DOUBLE",
  "STR",
  "MULTILINE_STR",
  "+",
  "-",
  "/",
  "//",
  "*",
  "<",
  ">",
  "==",
  "<=",
  ">=",
  "!=",
  "(",
  ")",
  ",",
  "=",
  ":",
};

/* a = 5
   b = 10
   c = a + b */

int main()
{
	FILE *file;
	file = fopen("../test_source_codes/basic_test.ifj19", "r");

	/* Token 1  -> a */
	Token *tokenptr;
	if((tokenptr = (Token *) malloc(sizeof(Token))) == NULL)
	{
		printf("Zlyhala alokacia tokenu.\n");
		return 1;
	}
	gimme_Token(file, tokenptr);
	if(tokenptr->type == ID)
		printf("%s je ID /%d/\n", tokenptr->attr.str, tokenptr->type);
	else
		printf("%s nieje ID /%d/\n", tokenptr->attr.str, tokenptr->type);

	/* Token 2  -> = */
	Token *tokenptr2;
	if((tokenptr2 = (Token *) malloc(sizeof(Token))) == NULL)
	{
		printf("Zlyhala alokacia tokenu.\n");
		return 1;
	}
	gimme_Token(file, tokenptr2);
	if(tokenptr2->type == ASSIGN)
        	printf("%s je ASSIGN /%d/\n", TokenTypeToString(tokenptr2->type), tokenptr2->type);
        else
                printf("%s nieje ASSIGN /%d/\n", TokenTypeToString(tokenptr2->type), tokenptr2->type);

	/* Token 3 -> 5 */
	Token *tokenptr3;
        if((tokenptr3 = (Token *) malloc(sizeof(Token))) == NULL)
        {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr3);
        if(tokenptr3->type == INT)
                printf("%d je INT /%d/\n", tokenptr3->attr.int_num, tokenptr3->type);
        else
                printf("%d nieje INT /%d/\n", tokenptr3->attr.int_num, tokenptr3->type);

	/* Token 4 -> EOL */
	Token *tokenptr4;
        if((tokenptr4 = (Token *) malloc(sizeof(Token))) == NULL)
        {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr4);
        if(tokenptr4->type == EOL)
                printf("%s je EOL /%d/\n",TokenTypeToString(tokenptr4->type), tokenptr4->type);
        else
                printf("%s nieje EOL /%d/\n", TokenTypeToString(tokenptr4->type), tokenptr4->type);

	/* Token 5 -> b */
	Token *tokenptr5;
        if((tokenptr5 = (Token *) malloc(sizeof(Token))) == NULL)
        {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr5);
        if(tokenptr5->type == ID)
                printf("%s je ID /%d/\n", tokenptr5->attr.str, tokenptr5->type);
        else
                printf("%s nieje ID /%d/\n", tokenptr5->attr.str, tokenptr5->type);

	/* Token 6 -> = */
	Token *tokenptr6;
        if((tokenptr6 = (Token *) malloc(sizeof(Token))) == NULL)
        {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr6);
        if(tokenptr6->type == ASSIGN)
                printf("%s je ASSIGN /%d/\n", TokenTypeToString(tokenptr6->type), tokenptr6->type);
        else
                printf("%s nieje ASSIGN /%d/\n", TokenTypeToString(tokenptr6->type), tokenptr6->type);

	/* Token 7 -> 10 */
	Token *tokenptr7;
        if((tokenptr7 = (Token *) malloc(sizeof(Token))) == NULL)
        {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr7);
        if(tokenptr7->type == INT)
                printf("%d je INT /%d/\n", tokenptr7->attr.int_num, tokenptr7->type);
        else
                printf("%d nieje INT /%d/\n", tokenptr7->attr.int_num, tokenptr7->type);

	/* Token 8 -> EOL */
	Token *tokenptr8;
        if((tokenptr8 = (Token *) malloc(sizeof(Token))) == NULL)
        {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr8);
        if(tokenptr8->type == EOL)
                printf("%s je EOL /%d/\n",TokenTypeToString(tokenptr8->type), tokenptr8->type);
        else
                printf("%s nieje EOL /%d/\n", TokenTypeToString(tokenptr8->type), tokenptr8->type);

	/* Token 9 -> c */
	Token *tokenptr9;
        if((tokenptr9 = (Token *) malloc(sizeof(Token))) == NULL)
        {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr9);
        if(tokenptr9->type == ID)
                printf("%s je ID /%d/\n", tokenptr9->attr.str, tokenptr9->type);
        else
                printf("%s nieje ID /%d/\n", tokenptr9->attr.str, tokenptr9->type);

	/* Token 10 - > = */
	Token *tokenptr10;
        if((tokenptr10 = (Token *) malloc(sizeof(Token))) == NULL)
        {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr10);
        if(tokenptr10->type == ASSIGN)
                printf("%s je ASSIGN /%d/\n", TokenTypeToString(tokenptr10->type), tokenptr10->type);
        else
                printf("%s nieje ASSIGN /%d/\n", TokenTypeToString(tokenptr10->type), tokenptr10->type);


	/* Token 11 -> a */
	Token *tokenptr11;
        if((tokenptr11 = (Token *) malloc(sizeof(Token))) == NULL)
        {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr11);
        if(tokenptr11->type == ID)
                printf("%s je ID /%d/\n", tokenptr11->attr.str, tokenptr11->type);
        else
                printf("%s nieje ID /%d/\n", tokenptr11->attr.str, tokenptr11->type);

	/* Token 12 -> + */
	Token *tokenptr12;
        if((tokenptr12 = (Token *) malloc(sizeof(Token))) == NULL)
        {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr12);
        if(tokenptr12->type == ADD)
                printf("%s je ADD /%d/\n", TokenTypeToString(tokenptr12->type), tokenptr12->type);
        else
                printf("%s nieje ADD /%d/\n", TokenTypeToString(tokenptr12->type), tokenptr12->type);

	/* Token 13 -> b */
	Token *tokenptr13;
        if((tokenptr13 = (Token *) malloc(sizeof(Token))) == NULL)
         {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr13);
        if(tokenptr13->type == ID)
                printf("%s je ID /%d/\n", tokenptr13->attr.str, tokenptr13->type);
        else
                printf("%s nieje ID /%d/\n", tokenptr13->attr.str, tokenptr13->type);

	/* Token 14 -> EOL */
	Token *tokenptr14;
        if((tokenptr14 = (Token *) malloc(sizeof(Token))) == NULL)
        {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr14);
        if(tokenptr14->type == EOL)
                printf("%s je EOL /%d/\n", TokenTypeToString(tokenptr14->type), tokenptr14->type);
        else
                printf("%s nieje EOL /%d/\n", TokenTypeToString(tokenptr14->type), tokenptr14->type);
	
	/* Token 15 -> _EOF */
        Token *tokenptr15;
        if((tokenptr15 = (Token *) malloc(sizeof(Token))) == NULL)
        {
                printf("Zlyhala alokacia tokenu.\n");
                return 1;
        }
        gimme_Token(file, tokenptr15);
        if(tokenptr15->type == _EOF)
               printf("%s je _EOF /%d/\n", TokenTypeToString(tokenptr15->type), tokenptr15->type);
        else
               printf("%s nieje _EOF /%d/\n", TokenTypeToString(tokenptr15->type), tokenptr15->type);


	return 0;
}


const char *TokenTypeToString(T_type type){

  switch (type) {
    case INDENT:
      return token2str[0];
    case DEDENT:
      return token2str[1];
    case _EOF:
      return token2str[2];
    case EOL:
      return token2str[3];
    case ID:
      return token2str[4];
    case KEY_WORD:
      return token2str[5];
    case INT:
      return token2str[6];
    case DOUBLE:
      return token2str[7];
    case STR:
      return token2str[8];
    case MULTILINE_STR:
      return token2str[9];
    case ADD:
      return token2str[10];
    case SUB:
      return token2str[11];
    case DIV:
      return token2str[12];
    case INT_DIV:
      return token2str[13];
    case MUL:
      return token2str[14];
    case LESS:
      return token2str[15];
    case MORE:
      return token2str[16];
    case EQUAL:
      return token2str[17];
    case LESS_EQ:
      return token2str[18];
    case MORE_EQ:
      return token2str[19];
    case NOT_EQ:
      return token2str[20];
    case L_PAR:
      return token2str[21];
    case R_PAR:
      return token2str[22];
    case COMMA:
      return token2str[23];
    case ASSIGN:
      return token2str[24];
    case COLON:
      return token2str[25];

  }
  return *token2str;
}

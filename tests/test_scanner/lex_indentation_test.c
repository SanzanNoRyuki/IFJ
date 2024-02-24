#include <stdio.h>
#include <string.h>

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

int main(){

	FILE *file;
	file = fopen("../test_source_codes/lex_indentation_test.ifj19", "r");
	
	/* Token 03  -> EOL */
	Token *token03;
	if((token03 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token03);
	if(token03->type != EOL)
		printf("%s nieje EOL /%d/token03\n", TokenTypeToString(token03->type), token03->type);

	/* Token 04  -> EOL */
	Token *token04;
	if((token04 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token04);
	if(token04->type != EOL)
		printf("%s nieje EOL /%d/token04\n", TokenTypeToString(token04->type), token04->type);

	/* Token 05  -> EOL */
	Token *token05;
	if((token05 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token05);
	if(token05->type != EOL)
		printf("%s nieje EOL /%d/token05\n", TokenTypeToString(token05->type), token05->type);

	/* Token 01  -> comment */
	Token *token01;
	if((token01 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token01);
	if(token01->type != MULTILINE_STR)
		printf("%s nieje multistr /%d/token01\n", token01->attr.str, token01->type);

	/* Token eol1  -> EOL */
	Token *tokeneol1;
	if((tokeneol1 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, tokeneol1);
	if(tokeneol1->type != EOL)
		printf("%s nieje EOL /%d/token eol1\n", TokenTypeToString(tokeneol1->type), tokeneol1->type);
	
	/* Token eol2  -> EOL */
	Token *tokeneol2;
	if((tokeneol2 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, tokeneol2);
	if(tokeneol2->type != EOL)
		printf("%s nieje EOL /%d/token eol2\n", TokenTypeToString(tokeneol2->type), tokeneol2->type);

	/* Token 02  -> comment */
	Token *token02;
	if((token02 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token02);
	if(token02->type != MULTILINE_STR)
		printf("%s nieje multistr /%d/token02\n", token02->attr.str, token02->type);	

	/* Token eol3  -> EOL */
	Token *tokeneol3;
	if((tokeneol3 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, tokeneol3);
	if(tokeneol3->type != EOL)
		printf("%s nieje EOL /%d/token eol3\n", TokenTypeToString(tokeneol3->type), tokeneol3->type);


	/* Token 1  -> s1 */
	Token *token1;
	if((token1 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token1);
	if(token1->type != ID)
		printf("%s nieje ID /%d/ token1\n", token1->attr.str, token1->type);
	
	/* Token 2  -> = */
	Token *token2;
	if((token2 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token2);
	if(token2->type != ASSIGN)
		printf("%s nieje ASSIGN /%d/token2\n", TokenTypeToString(token2->type), token2->type);
	
	/* Token 3  -> string */
	Token *token3;
	if((token3 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token3);
	if(token3->type != STR)
		printf("%s nieje STR /%d/token3\n", token3->attr.str, token3->type);
	if((strcmp(token3->attr.str, "Some text, so you have something to do with your life.")) != 0)
		printf("%s nieje spravny string! token3\n", token3->attr.str);

	/* Token eol4  -> EOL */
	Token *tokeneol4;
	if((tokeneol4 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, tokeneol4);
	if(tokeneol4->type != EOL)
		printf("%s nieje EOL /%d/token eol4\n", TokenTypeToString(tokeneol4->type), tokeneol4->type);


	/* Token 4  -> s2 */
	Token *token4;
	if((token4 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token4);
	if(token4->type != ID)
		printf("%s nieje ID /%d/token4\n", token4->attr.str, token4->type);

	/* Token 5  -> = */
	Token *token5;
	if((token5 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token5);
	if(token5->type != ASSIGN)
		printf("%s nieje ASSIGN /%d/token5\n", TokenTypeToString(token5->type), token5->type);

	/* Token 6  -> string */
	Token *token6;
	if((token6 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token6);
	if(token6->type != STR)
		printf("%s nieje STR /%d/token6\n", token6->attr.str, token6->type);
	if((strcmp(token6->attr.str, "Thank you, sir.")) != 0)
		printf("%s nieje spravy string!token6\n", token6->attr.str);
	
	/* Token eol5  -> EOL */
	Token *tokeneol5;
	if((tokeneol5 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, tokeneol5);
	if(tokeneol5->type != EOL)
		printf("%s nieje EOL /%d/token eol5\n", TokenTypeToString(tokeneol5->type), tokeneol5->type);


	/* Token 7  -> Key_word IF */
	Token *token7;
	if((token7 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token7);
	if(token7->type != KEY_WORD)
		printf("nieje KEY_WORD /%d/token7\n", token7->type);

	/* Token 8  -> s1 */
	Token *token8;
	if((token8 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token8);
	if(token8->type != ID)
		printf("%s nieje ID /%d/token8\n", token8->attr.str, token8->type);

	/* Token 9  -> != */
	Token *token9;
	if((token9 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token9);
	if(token9->type != NOT_EQ)
		printf("%s nieje NOT_EQ /%d/token9\n", TokenTypeToString(token9->type), token9->type);
	
	/* Token 10  -> s2 */
	Token *token10;
	if((token10 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token10);
	if(token10->type != ID)
		printf("%s nieje ID /%d/token10\n", token10->attr.str, token10->type);

	/* Token 10b  -> : */
	Token *token10b;
	if((token10b = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token10b);
	if(token10b->type != COLON)
		printf("%s nieje COLON /%d/\n", TokenTypeToString(token10b->type), token10b->type);


	/* Token 11  -> _EOL */
	Token *token11;
	if((token11 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token11);
	if(token11->type != EOL)
		printf("%s nieje EOL /%d/token11\n", TokenTypeToString(token11->type), token11->type);

	/* Token 12  -> s3 */
	Token *token12;
	if((token12 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token12);
	if(token12->type != ID)
		printf("%s nieje ID /%d/token12\n", token12->attr.str, token12->type);


	/* Token 13  -> = */
	Token *token13;
	if((token13 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token13);
	if(token13->type != ASSIGN)
		printf("%s nieje ASSIGN /%d/token13\n", TokenTypeToString(token13->type), token13->type);


	/* Token 14  -> string */
	Token *token14;
	if((token14 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token14);
	if(token14->type != STR)
		printf("%s nieje STR /%d/token14\n", token14->attr.str, token14->type);


	/* Token 15  -> EOL */
	Token *token15;
	if((token15 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token15);
	if(token15->type != EOL)
		printf("%s nieje EOL /%d/token15\n", TokenTypeToString(token15->type), token15->type);


	/* Token 16  -> Key_word */
	Token *token16;
	if((token16 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token16);
	if(token16->type != KEY_WORD)
		printf("nieje key_word /%d/token16\n",  token16->type);


	/* Token 17  -> : */
	Token *token17;
	if((token17 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token17);
	if(token17->type != COLON)
		printf("%s nieje COLON /%d/token17\n", TokenTypeToString(token17->type), token17->type);


	/* Token 18  -> EOL */
	Token *token18;
	if((token18 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token18);
	if(token18->type != EOL)
		printf("%s nieje EOL /%d/token18\n", TokenTypeToString(token18->type), token18->type);
	
	/* Token dd1  -> dedent */
	Token *tokendd1;
	if((tokendd1 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, tokendd1);
	if(tokendd1->type != DEDENT)
		printf("%s nieje DEDENT /token%d/tokendd1\n", TokenTypeToString(tokendd1->type), tokendd1->type);

	/* Token 19  -> s3 */
	Token *token19;
	if((token19 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token19);
	if(token19->type != ID)
		printf("%s nieje ID /%d/token19\n", token19->attr.str, token19->type);


	/* Token 20  -> = */
	Token *token20;
	if((token20 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token20);
	if(token20->type != ASSIGN)
		printf("%s nieje ASSIGN /%d/token20\n", TokenTypeToString(token20->type), token20->type);

	/* Token 21  -> str */
	Token *token21;
	if((token21 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token21);
	if(token21->type != STR)
		printf("%s nieje STR /token22%d/\n", token21->attr.str, token21->type);

	/* Token 22  -> EOL */
	Token *token22;
	if((token22 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token22);
	if(token22->type != EOL)
		printf("%s nieje EOL /token22%d/\n", TokenTypeToString(token22->type), token22->type);

	/* Token 23  -> EOL */
	Token *token23;
	if((token23 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token23);
	if(token23->type != EOL)
		printf("%s nieje EOL /token23%d/\n", TokenTypeToString(token23->type), token23->type);

	/* Token 24  -> comment */
	Token *token24;
	if((token24 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token24);
	if(token24->type != MULTILINE_STR)
		printf("nieje multistr /token24%d/\n", token24->type);
	
	/* Token indent  -> indent */
	Token *tokenid1;
	if((tokenid1 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, tokenid1);
	if(tokenid1->type != INDENT)
		printf("%s nieje ID /%d/token\n", TokenTypeToString(tokenid1->type), tokenid1->type);	

	/* Token 25  -> EOL */
	Token *token25;
	if((token25 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token25);
	if(token25->type != EOL)
		printf("%s nieje EOL /token25%d/\n", TokenTypeToString(token25->type), token25->type);

	/* Token 26  -> a */
	Token *token26;
	if((token26 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token26);
	if(token26->type != ID)
		printf("%s nieje ID /token%d/\n", token26->attr.str, token26->type);

	/* Token 27  -> = */
	Token *token27;
	if((token27 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token27);
	if(token27->type != ASSIGN)
		printf("%s nieje ASSIGN /token27%d/\n", TokenTypeToString(token27->type), token27->type);

	/* Token 28  -> INT */
	Token *token28;
	if((token28 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token28);
	if(token28->type != INT)
		printf("%d nieje INT /token28%d/\n", token28->attr.int_num, token28->type);

	/* Token 29  -> EOL */
	Token *token29;
	if((token29 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token29);
	if(token29->type != EOL)
		printf("%s nieje EOL /token29%d/\n", TokenTypeToString(token29->type), token29->type);

	/* Token 30  -> key_word */
	Token *token30;
	if((token30 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token30);
	if(token30->type != KEY_WORD)
		printf("%s nieje keyword /%d/token30\n", TokenTypeToString(token30->type), token30->type);
	
	/* Token 31  -> a */
	Token *token31;
	if((token31 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token31);
	if(token31->type != ID)
		printf("%s nieje ID /%d/token31\n", token31->attr.str, token31->type);

	/* Token 32  -> != */
	Token *token32;
	if((token32 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token32);
	if(token32->type != NOT_EQ)
		printf("%s nieje not_eq /%d/token32\n", TokenTypeToString(token32->type), token32->type);

	/* Token 33  -> 0 */
	Token *token33;
	if((token33 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token33);
	if(token33->type != INT)
		printf("%d nieje INT /%d/token33\n", token33->attr.int_num, token33->type);

	/* Token 34  -> : */
	Token *token34;
	if((token34 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token34);
	if(token34->type != COLON)
		printf("%s nieje COLON /%d/token34\n", TokenTypeToString(token34->type), token34->type);

	/* Token 35  -> EOL */
	Token *token35;
	if((token35 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token35);
	if(token35->type != EOL)
		printf("%s nieje EOL /%d/token35\n", TokenTypeToString(token35->type), token35->type);

	/* Token 36  ->  */
	Token *token36;
	if((token36 = (Token *) malloc(sizeof(Token))) == NULL)
		printf("Zlyhala alokacia tokenu.\n");
		
	gimme_Token(file, token36);
	if(token36->type != ID)
		printf("%s nieje ID /%d/token\n", token36->attr.str, token36->type);

	fclose(file);
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

/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file parser.c
 * @brief Source file of syntax analysis.
 *
 * @authors Vojtěch Ulej (xulejv00)
 *
 *
 * @date 22.10.2019
 ******************************************************************************/

 #include <stdbool.h>
 #include <stdio.h>
 #include "scanner.h"
 #include "errors.h"
 #include "symtable.h"
 #include "parser.h"
 #include "expression_parser.h"
 #include "code_gen.h"


/**
  * For printing errors
  */
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
   ":"
 };

 #define CTtoken(token)		\
 	token = (Token *) malloc(sizeof(Token));      \
 	if (! token) exit(ERR_INTERNAL);

 #define CHECK_LEX_NEXT_TOKEN(token, err)   \
  err = gimme_Token(input_file,token); if(err) return err;

 #define IS_VALUE(token) \
 token.type == INT || \
 token.type == STR || \
 token.type == DOUBLE || \
 token.type == MULTILINE_STR || \
 (token.type == KEY_WORD && token.attr.key_W == NONE)

 #define ANSWER_TO_LIFE 42

 #define UNEXP_T(token) fprintf(stderr, "Unexpected token: %s line:%d\n",TokenTypeToString(token.type),__LINE__);

ParserData GlobalParserData;
FILE *input_file;
code_gen *code;


BTData *BTDataCtor();
const char *TokenTypeToString(T_type type);
void add_build_in_functions(BTNodePtr *GlobalTable);
bool isOperator(T_type type);
Data_type data_Type(T_type type);


int INPUT();
// INPUT      -> STMTS  _eof
int STMTS(Token *act_Token);
//STMTS      -> ε
//STMTS      -> STMT STMTS
//STMT       -> id = ASSIGN
int _IF(); //STMT       -> if EXPRESION : eol indent STMTS dedent else : eol indent STMTS dedent
int _WHILE();//STMT       -> while EXPRESION : eol indent STMTS dedent
int _DEF();//STMT       -> FUNC_DEF eol
//STMT       -> pass eol
//STMT       -> eol
//STMT       -> CALL_FUNC eol
//STMT       -> return RET_VAL eol
int _ASSIGN(BTData *data);
//ASSIGN     -> EXPRESION
//ASSIGN     -> TERM eol
//ASSIGN     -> CALL_FUNC eol
//VALUE      -> int
//VALUE      -> double
//VALUE      -> str
//VALUE      -> multiline_str
//VALUE      -> none
//TERM       -> id
//TERM       -> VALUE
int CALL_FUNC(BTNodePtr func);
//CALL_FUNC  -> func_id ( ARGS
//CALL_FUNC  -> inputs ( )
//CALL_FUNC  -> inputi ( )
//CALL_FUNC  -> inputf ( )
//CALL_FUNC  -> len ( TERM )
//CALL_FUNC  -> substr ( TERM , TERM , TERM )
//CALL_FUNC
//CALL_FUNC  -> ord ( TERM )
//CALL_FUNC  -> chr ( TERM )  -> print ( ARGS

//FUNC_DEF   -> def func_id ( ARGS : eol indent FUNC_BODY dedent
int RETVAL();
//RET_VAL    -> TERM
//RET_VAL    -> EXPRESION
int ARGS(int * count_of_params, void *local);
//ARGS       -> )
//ARGS       -> TERM NEXT_ARG
//NEXT_ARG   -> , TERM NEXT_ARG
//NEXT_ARG   -> )

int Analysis(FILE *in){

  if (in == NULL) {
    return ERR_INTERNAL;
  }
  GlobalParserData.label_gen = ANSWER_TO_LIFE;
  static code_gen code_gen;
  code = &code_gen;

  code_gen_init(code);

  BTNode_init(&GlobalParserData.GlobalTable);
  add_build_in_functions(&GlobalParserData.GlobalTable);
  input_file = in;
  int err;
  err = INPUT();

  return err;
}

int INPUT(){
  Token t;
  int err;
  //bool nonemptyflag = false;
  for (err = gimme_Token(input_file,&t);t.type != _EOF && err == ALL_OK;err = gimme_Token(input_file,&t)) {
    /* INPUT      -> STMTS  _eof */
    //if (nonemptyflag || (t.type != EOL && t.type != _EOF)) nonemptyflag = true; // Hlavní tělo programu je NEPRÁZDNÁ množina příkazů
    err = STMTS(&t);
    if (err) break;
  }

  //return nonemptyflag ? err : ERR_SYNTAX;       // Pokud se v těle programu neobjevil žádný příkaz vrať ERR_SYNTAX
  return err;
}

int STMTS(Token *act_Token){
  int err = ALL_OK;
  Token next_T1;
  //This switch handle which rule should be used
  switch (act_Token->type) {
    case EOL:                 // return ALL_OK the STMTS() will be recalled by INPUT()
      break;
    case ID:                  // might be assignment or just ID eol (also possible if ID is defined)
      CHECK_LEX_NEXT_TOKEN(&next_T1,err);
      if (next_T1.type == EOL) {
        if (!BTNode_search(&GlobalParserData.GlobalTable,act_Token->attr.str)) {
          return ERR_SEM_DEF;
        }
        break;
      }
      else if (next_T1.type == ASSIGN) {
        if (!BTNode_search(&GlobalParserData.GlobalTable,act_Token->attr.str)) { //If var was not defined before define it now
            ADD_INS("DEFVAR GF@");     // Defining new variable
            ADD_INS(act_Token->attr.str);
            GEN_LF;
        }
        BTData *data = BTDataCtor();
        ADD_INS("CREATEFRAME\nDEFVAR TF@%%assign%\n");
        err = _ASSIGN(data);
        ADD_INS("MOVE GF@");
        ADD_INS(act_Token->attr.str);
        ADD_INS(" ");
        ADD_INS( "TF@%%assign%\n");
        BTNode_insert(&GlobalParserData.GlobalTable,act_Token->attr.str,data);
      }
      else if(next_T1.type == L_PAR) {           // Might be declaration of function so don't forget to add the id as new func_id to global symtable
        BTNodePtr id_func;
        if (!(id_func = BTNode_search(&GlobalParserData.GlobalTable,act_Token->attr.str))) {
          BTData *data = BTDataCtor();
          data->id_type = func_id;
          data->dat.data_func.label = LabelGen(act_Token->attr.str);
          data->dat.data_func.defined = false;
          BTNode_insert(&GlobalParserData.GlobalTable,act_Token->attr.str,data);
          id_func = BTNode_search(&GlobalParserData.GlobalTable,act_Token->attr.str);
        }
        ADD_INS( "CREATEFRAME\nDEFVAR TF@%%assign%\n");
        err = CALL_FUNC(id_func);
      }
      break;
    case INT:
    case DOUBLE:
    case STR:
    case MULTILINE_STR:
      CHECK_LEX_NEXT_TOKEN(&next_T1,err); // Ask for next token
      if (next_T1.type == EOL)                // If next token == EOL then everything is OK
        break;
      else if(isOperator(next_T1.type)){     // Expression

        ADD_INS("CREATEFRAME\nDEFVAR TF@%%assign%\n");
        err = expression_parser(input_file,act_Token,&next_T1);
        if (err == 10) err = ERR_SYNTAX;
        break;
      }
      else{
        return ERR_SYNTAX;
      }
      case KEY_WORD:
      switch (act_Token->attr.key_W) {          // This switch handles keywords
        case PASS:                                      //pass
          ADD_INS("#pass\n");
          CHECK_LEX_NEXT_TOKEN(&next_T1,err);
          if (next_T1.type == EOL) break;   // After pass there must be eol
          else {
            err = ERR_SYNTAX;
            break;
          }
        case NONE:          // None eol is possible
          CHECK_LEX_NEXT_TOKEN(&next_T1,err)
          if (next_T1.type == EOL) break;
          else if(isOperator(next_T1.type)){       // this is expression
            err = expression_parser(input_file,act_Token,&next_T1);
            if (err == 10) return ERR_SYNTAX;  // 10 means ':'
            else if(err) return err;
            break;
          }
          else {
            err = ERR_SYNTAX;
            break;
          }
        case IF:
          if (!GlobalParserData.if_or_while) {
          GlobalParserData.if_or_while = true;
          err = _IF();
          GlobalParserData.if_or_while = false;
          } else err = _IF();
          break;
        case DEF:
          if (!GlobalParserData.defining_function) return ERR_SYNTAX;
          else if (GlobalParserData.if_or_while) return ERR_SYNTAX;
          GlobalParserData.defining_function = true;
          err = _DEF();
          GlobalParserData.defining_function = false;
          break;
        case WHILE:
          if (!GlobalParserData.if_or_while) {
          GlobalParserData.if_or_while = true;
          err = _WHILE();
          GlobalParserData.if_or_while = false;
          } else err = _WHILE();
          break;
        case RETURN:
          if (GlobalParserData.defining_function){
            err = RETVAL();
          }
          else{
            err = ERR_SYNTAX;
          }
          break;
        default:
          err = ERR_SYNTAX;
          break;
      }
      break;
    default:
      err = ERR_SYNTAX;
      break;
  }
  return err;
}

/** @brief Function parse assignment
 *
 *  @returns 0 if ALL_OK or another error code (in case of error)
 * */
int _ASSIGN(BTData *data){
  int err = ALL_OK;
  Token t0;
  Token t1;
  CHECK_LEX_NEXT_TOKEN(&t0,err);
  if (IS_VALUE(t0)){        // ID = VALUE
    CHECK_LEX_NEXT_TOKEN(&t1,err);
    if (t1.type == EOL || t1.type == _EOF) {
      data->id_type = var_id;
      ADD_INS("MOVE TF@%%assign%"); ADD_INS(" ");
        if (t0.type == STR ||t0.type == MULTILINE_STR)  {
            ADD_INS( "string@");
            ADD_INS( string2format(t0.attr.str));
        }
        else if (t0.type == INT) {
            ADD_INS("int@");
            ADD_INS(int2str(t0.attr.int_num));
        } else if (t0.type == DOUBLE){

            ADD_INS("float@");
            ADD_INS(flt2format(t0.attr.decimal_num));
        } else ADD_INS("nil@nil");
        GEN_LF;
        return ALL_OK;
    }
    else if (isOperator(t1.type)){

      err = expression_parser(input_file,&t0,&t1);
      if (err){
        if (err == 10) err = ERR_SYNTAX;
        return err;
      }
    }
    else{
      err = ERR_SYNTAX;
    }

  } //endif ID = VALUE | ID = expression (VALUE operator)
  else if (t0.type == ID) { // ID = ID | ID = expression (ID operator)
    BTNodePtr var = BTNode_search(&GlobalParserData.GlobalTable, t0.attr.str);
    if (!var)
      return ERR_SEM_DEF;
    CHECK_LEX_NEXT_TOKEN(&t1, err);
    if (t1.type == EOL || t1.type == _EOF) {
      data->dat.data_var = var->data->dat.data_var;
      ADD_INS("MOVE TF@%%assign% ");
      ADD_INS("GF@");
      code_gen_add_instruction(code, (GlobalParserData.defining_function)? Function:Global, t0.attr.str);
      code_gen_add_instruction(code, (GlobalParserData.defining_function)? Function:Global, "\n");
      return err;
    } else if (t1.type == L_PAR) {
      /// TODO call function (might be declaration if id isn't found in GlobalTable!!!)
      BTNodePtr func;
      if (!(func = BTNode_search(&GlobalParserData.GlobalTable, t0.attr.str))) { // function is not declared
        BTData *data = BTDataCtor();
        data->id_type = func_id;
        data->dat.data_func.label = LabelGen(t0.attr.str);
        data->dat.data_func.defined = false;
        BTNode_insert(&GlobalParserData.GlobalTable,t0.attr.str,data);
        func = BTNode_search(&GlobalParserData.GlobalTable,t0.attr.str);
      }
      err = CALL_FUNC(func);
    } else if (isOperator(t1.type)) {
      err = expression_parser(input_file, &t0, &t1);
      if (err) {
        if (err == 10)
          err = ERR_SYNTAX;
        return err;
      }
    } // endif ID = ID | ID = expression (ID operator)
    return err;
  }
  else if (isOperator(t0.type)){
    err = expression_parser(input_file,&t0,NULL);
    if (err){
      if (err == 10) err = ERR_SYNTAX;
      return err;
    }
  }
  return err;
}

int _WHILE(){
  int err = ALL_OK;
  char *label_while, *label_end, *label_cond;
  label_while = LabelGen("while");
  label_cond = LabelGen("while_cond");
  label_end = LabelGen("end_while");
  Token t0;

  ADD_INS("#WHILE_CONDITION\nLABEL "); ADD_INS(label_cond); GEN_LF;
  ADD_INS("CREATEFRAME\n");
  ADD_INS("DEFVAR TF@%%assign%\n");
  err = expression_parser(input_file, NULL, NULL);  // Condition expresion code will be generated by expression parser
  if(!err) return ERR_SYNTAX;
  else if (err == 10) {
    err = ALL_OK;
  } else return err;
  /* V %%assign% bude uložen výsledek expression vygenerujeme podmínku skoku end_while
   * Skok end_while vystoupí z cyklu while pokud je !!znegovaná!! podmínka pravdivá
   * if (!condition) goto end_while
   * provede se typová kontrola výsledku, pokud je výsledek číslo porovná se z 0, pokud string, dojde k porovnání s prázdným stringem
                "DEFVAR TF@%type\n\
                TYPE TF@%type TF@%%assign%\n\
                JUMPIFNEQ int TF@%type string@bool\n\*/char *label_cond_int = LabelGen("int");/*
                JUMPIFNEQ end_while TF@%%assign% bool@true\n\
                JUMP while\n\
                LABEL int\n\
                JUMPIFNEQ double TF@%type string@int\n\*/char *label_cond_double = LabelGen("double");/*
                JUMPIFNEQ end_while TF@%%assign% int@0\n\
                JUMP while\n\
                LABEL double\n\
                JUMPIFNEQ string TF@%type string@float\n\*/char *label_cond_str = LabelGen("string");/*
                JUMPIFNEQ end_while TF@%%assign% float@0x0p+0\n\
                JUMP while\n\
                LABEL string\n\
                JUMPIFNEQ nil TF@%type string@string\n\*/char *label_cond_none = LabelGen("nil");/*
                JUMPIFNEQ end_while TF@%%assign% string@\n\
                JUMP while\n\
                LABEL nil\n\
                JUMPIFEQ end_while TF@%%assign% nil@nil"\n\
   * */
  /* Generating condition code */
  ADD_INS("DEFVAR TF@%type\nTYPE TF@%type TF@%%assign%\nJUMPIFNEQ "); ADD_INS(label_cond_int);
  ADD_INS(" TF@%type string@bool\nJUMPIFNEQ ");
  ADD_INS(label_end);ADD_INS(" TF@%%assign% bool@true\nJUMP ");
  ADD_INS(label_while); GEN_LF;
  //Label int
  ADD_INS("\nLABEL  ");ADD_INS(label_cond_int); GEN_LF;
  ADD_INS("JUMPIFNEQ "); ADD_INS(label_cond_double);
  ADD_INS(" TF@%type string@int\nJUMPIFEQ ");
  ADD_INS(label_end);
  ADD_INS(" TF@%%assign% int@0\n");
  ADD_INS("JUMP ");  ADD_INS(label_while);GEN_LF;
  //Label double
  ADD_INS("\nLABEL  ");ADD_INS(label_cond_double); GEN_LF;
  ADD_INS("JUMPIFNEQ "); ADD_INS(label_cond_str);
  ADD_INS(" TF@%type string@float\nJUMPIFEQ ");
  ADD_INS(label_end);
  ADD_INS(" TF@%%assign% float@0x0p+0\n");
  ADD_INS("JUMP ");  ADD_INS(label_while);GEN_LF;
  //Label string
  ADD_INS("\nLABEL  ");ADD_INS(label_cond_str); GEN_LF;
  ADD_INS("JUMPIFNEQ "); ADD_INS(label_cond_none);
  ADD_INS(" TF@%type string@string\nJUMPIFEQ ");
  ADD_INS(label_end);
  ADD_INS(" TF@%%assign% string@\n");
  ADD_INS("JUMP ");  ADD_INS(label_while);GEN_LF;
  //Label none
  ADD_INS("\nLABEL  ");ADD_INS(label_cond_none); GEN_LF;
  ADD_INS("\nJUMPIFEQ  "); ADD_INS(label_end);
  ADD_INS(" TF@%%assign% nil@nil\n");
  /* End of generating condition code */

  for(;;){                              // Take all comments and eols
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == EOL) continue;
    else if (t0.type == INDENT) break;
    else if (t0.type == MULTILINE_STR) continue;
    else return ERR_SYNTAX;
  }
  bool nonemptyflag = false;
  ADD_INS("\n#WHILE_BODY\nLABEL  "); ADD_INS(label_while);GEN_LF;
  for (err = gimme_Token(input_file,&t0);t0.type != _EOF && t0.type != DEDENT && err == ALL_OK;err = gimme_Token(input_file,&t0)) {
    if(err) return err;
    if (nonemptyflag || (t0.type != EOL && t0.type != _EOF)) nonemptyflag = true; // Hlavní tělo programu je NEPRÁZDNÁ množina příkazů
    err = STMTS(&t0);
    if (err) return err;
  }
  if (!nonemptyflag)
    fprintf(stderr,"The while body mustn't be empty!");

  ADD_INS("JUMP "); ADD_INS(label_cond); GEN_LF;
  ADD_INS("LABEL  "); ADD_INS(label_end);ADD_INS("\n#END_WHILE\n\n");
  return nonemptyflag ? err : ERR_SYNTAX;
}

int _IF(){
  int err = ALL_OK;
  Token t0;
  char *label_if, *label_else, *label_end_if, *label_cond_int, *label_cond_double, *label_cond_str, *label_cond_none;
  label_if = LabelGen("if");
  label_else = LabelGen("else");
  label_end_if = LabelGen("end_if");
  label_cond_int = LabelGen("int");
  label_cond_double = LabelGen("double");
  label_cond_str = LabelGen("string");
  label_cond_none = LabelGen("nil");

  ADD_INS("\n#IF_CONDITION\n");
  ADD_INS("CREATEFRAME\n");
  ADD_INS("DEFVAR TF@%%assign%\n");
  err = expression_parser(input_file,NULL,NULL);
  if (err != 10) {    // 10 means expression ended with ':'
    if (err == ALL_OK) err = ERR_SYNTAX;    // ALL_OK means EOL or _EOF
    return err;
  }

  /*  Generating condition code  */
  ADD_INS("DEFVAR TF@%type\nTYPE TF@%type TF@%%assign%\nJUMPIFNEQ "); ADD_INS(label_cond_int);
  ADD_INS(" TF@%type string@bool\nJUMPIFNEQ ");
  ADD_INS(label_else);ADD_INS(" TF@%%assign% bool@true\nJUMP ");
  ADD_INS(label_if); GEN_LF;
  //Label int
  ADD_INS("\nLABEL  ");ADD_INS(label_cond_int); GEN_LF;
  ADD_INS("JUMPIFNEQ "); ADD_INS(label_cond_double);
  ADD_INS(" TF@%type string@int\nJUMPIFEQ ");
  ADD_INS(label_else);
  ADD_INS(" TF@%%assign% int@0\n");
  ADD_INS("JUMP ");  ADD_INS(label_if);GEN_LF;
  //Label double
  ADD_INS("\nLABEL  ");ADD_INS(label_cond_double); GEN_LF;
  ADD_INS("JUMPIFNEQ "); ADD_INS(label_cond_str);
  ADD_INS(" TF@%type string@float\nJUMPIFEQ ");
  ADD_INS(label_else);
  ADD_INS(" TF@%%assign% float@0x0p+0\n");
  ADD_INS("JUMP ");  ADD_INS(label_if);GEN_LF;
  //Label string
  ADD_INS("\nLABEL  ");ADD_INS(label_cond_str); GEN_LF;
  ADD_INS("JUMPIFNEQ "); ADD_INS(label_cond_none);
  ADD_INS(" TF@%type string@string\nJUMPIFEQ ");
  ADD_INS(label_else);
  ADD_INS(" TF@%%assign% string@\n");
  ADD_INS("JUMP ");  ADD_INS(label_if);GEN_LF;
  //Label none
  ADD_INS("\nLABEL  ");ADD_INS(label_cond_none); GEN_LF;
  ADD_INS("JUMPIFEQ  "); ADD_INS(label_else);
  ADD_INS(" TF@%%assign% nil@nil\n");
  /*  End of generating condition code  */

  for(;;){                              // Take all comments and eols
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == EOL) continue;
    else if (t0.type == INDENT) break;
    else if (t0.type == MULTILINE_STR) continue;
    else return ERR_SYNTAX;
  }

  bool nonemptyflag = false;
  ADD_INS("\n#IF BODY\nLABEL  "); ADD_INS(label_if);GEN_LF;
  for (err = gimme_Token(input_file,&t0);t0.type != DEDENT && err == ALL_OK;err = gimme_Token(input_file,&t0)) {
    if(err) return err;
    if (nonemptyflag || (t0.type != EOL )) nonemptyflag = true; // Hlavní tělo programu je NEPRÁZDNÁ množina příkazů
    err = STMTS(&t0);
    if (err) return err;
  }
  if (!nonemptyflag) {
    fprintf(stderr, "The if body mustn't be empty!");
    return ERR_SYNTAX;
  }
  ADD_INS("JUMP "); ADD_INS(label_end_if);GEN_LF;

  /* else */
  for(;;){                              // Take all comments and eols
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == EOL) continue;
    else if (t0.type == KEY_WORD && t0.attr.key_W == ELSE) break;
    else if (t0.type == MULTILINE_STR) continue;
    else return ERR_SYNTAX;
  }
  CHECK_LEX_NEXT_TOKEN(&t0,err);
  if (t0.type != COLON) {
    return ERR_SYNTAX;
  }
  for(;;){                              // Take all comments and eols
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == EOL) continue;
    else if (t0.type == INDENT) break;
    else if (t0.type == MULTILINE_STR) continue;
    else return ERR_SYNTAX;
  }

  ADD_INS("\n#ELSE\n");ADD_INS("LABEL  "); ADD_INS(label_else);GEN_LF;
  for (err = gimme_Token(input_file,&t0);t0.type != _EOF && t0.type != DEDENT && err == ALL_OK;err = gimme_Token(input_file,&t0)) {
    if(err) return err;
    if (nonemptyflag || (t0.type != EOL && t0.type != _EOF)) nonemptyflag = true; // Hlavní tělo programu je NEPRÁZDNÁ množina příkazů
    err = STMTS(&t0);
    if (err) return err;
  }
  if (!nonemptyflag)
    fprintf(stderr,"The while body mustn't be empty!");

  ADD_INS("LABEL  "); ADD_INS(label_end_if);ADD_INS("\n#END_IF\n\n");

  return nonemptyflag ? err : ERR_SYNTAX;
}


/** @brief FUNC_DEF rule.
  * The function is called after function STMTS recieved Keyword token def
  *
  */
int _DEF(){
  /* Setting flags and defining variables */
  GlobalParserData.defining_function = true;
  int err = ALL_OK;
  Token t1;
  CHECK_LEX_NEXT_TOKEN(&t1,err)
  if (t1.type != ID){
    UNEXP_T(t1);
    return ERR_SYNTAX;                // After def token expect token ID
  }

  BTNodePtr id = BTNode_search(&GlobalParserData.GlobalTable,t1.attr.str);
  bool declared = false;
  BTData *data;
  if(!id){
                                    // not declared nor defined
    // Allocation for data
    data = BTDataCtor();
    // Setting the data
    data->id_type = func_id;
    data->dat.data_func.label = LabelGen(t1.attr.str);
    data->dat.data_func.defined = true;
    // Creating local table
    BTNodePtr Local_table;
    BTNode_init(&Local_table);
    GlobalParserData.LocalTable = Local_table;
    data->dat.data_func.local_table = (void *) Local_table;
    //BTNode_insert(&GlobalParserData.GlobalTable, t1.attr.str,data);  //Ještě nevkládat
  }
  else {
    declared = true;
    data = id->data;
    if (id->data->id_type == var_id) {
      fprintf(stderr, "ID: %s is already used as variable ID\n",t1.attr.str);
      return ERR_SEM_DEF;
    }
    else if (id->data->dat.data_func.defined) {
      fprintf(stderr, "Function %s is already defined.\n", t1.attr.str);
      return ERR_SEM_DEF;
    }
    else {
      id->data->dat.data_func.defined = true;
    }
  }
  CHECK_LEX_NEXT_TOKEN(&t1,err);

  if (t1.type != L_PAR){
    UNEXP_T(t1);
    return ERR_SYNTAX;
  }

  int n_of_args = 0;
  err = ARGS(&n_of_args, data->dat.data_func.local_table);
  if(err) return err;
  if (declared){ // if the function already been declared
    if (n_of_args != id->data->dat.data_func.count_of_params) {
      fprintf(stderr, "Function %s was called declared with %d parameters, but defined %d parameters.\n", t1.attr.str,id->data->dat.data_func.count_of_params, n_of_args);
      return ERR_SEM_PAR;
    }
  }
  else{
    data->dat.data_func.count_of_params = n_of_args;
    BTNode_insert(&GlobalParserData.GlobalTable, t1.attr.str,data);
  }

  CHECK_LEX_NEXT_TOKEN(&t1,err);
  if(t1.type != COLON){            // Expecting ':'
    UNEXP_T(t1);
    return ERR_SYNTAX;
  }

  CHECK_LEX_NEXT_TOKEN(&t1,err);
  if(t1.type != EOL){              // Expecting end of line
    UNEXP_T(t1);
    return ERR_SYNTAX;
  }

  CHECK_LEX_NEXT_TOKEN(&t1,err);
  if(t1.type != INDENT){              // Expecting end of line
    UNEXP_T(t1);
    return ERR_SYNTAX;
  }

  CHECK_LEX_NEXT_TOKEN(&t1,err);
  for(;t1.type != DEDENT && t1.type != _EOF; err = gimme_Token(input_file,&t1)){
    if (err) return err;

  }

  GlobalParserData.LocalTable = NULL;
  return err;
}

/** @brief Function parse parameters
  * In count_of_params will be stored count of parameters parsed by the function.
  * The symbols will be stored in local table
  * @returns Error code
  */
int ARGS(int *count_of_params, void *local){       // Parse the arguments until ')'
  int err = ALL_OK;
  BTNodePtr l_table = (BTNodePtr) local;

  Token t;
  bool firstflag = true;
  for (;;) {

    CHECK_LEX_NEXT_TOKEN(&t,err);
    if (t.type == R_PAR) {
      if (firstflag) {
        return err;
      }
      else {
        fprintf(stderr, "Unexpected token: %s\n", TokenTypeToString(t.type));
        return ERR_SYNTAX;
      }
    }
    else if (GlobalParserData.defining_function) {
      if (t.type != ID) {
        fprintf(stderr, "Unexpected token: %s\n", TokenTypeToString(t.type));
        return ERR_SYNTAX;
      }
      BTData *data = BTDataCtor();
      data->id_type = param_id;

      (*count_of_params)++;

      BTNode_insert(&l_table,t.attr.str,data);
    }
    else{
      /// Probably calling function
    }
    CHECK_LEX_NEXT_TOKEN(&t,err);
    if (t.type == R_PAR) {
      return err;
    }
    else if (t.type != COMMA) {
      return ERR_SYNTAX;
    }
    else{
      firstflag = false;
    }

  }

  return err;
}

int CALL_FUNC(BTNodePtr func){
  int err = ALL_OK;
  Token t0;
  if (!strcmp(func->name,"inputf")){
      ADD_INS("READ TF@%%assign% float\n");
      CHECK_LEX_NEXT_TOKEN(&t0,err);
      if(t0.type != R_PAR) err = ERR_SEM_PAR;
  } else if (!strcmp(func->name,"inputs")) {
      ADD_INS("READ TF@%%assign% string\n");
      CHECK_LEX_NEXT_TOKEN(&t0,err);
      if(t0.type != R_PAR) err = ERR_SEM_PAR;
  } else if (!strcmp(func->name,"inputi")) {
      ADD_INS("READ TF@%%assign% int\n");
      CHECK_LEX_NEXT_TOKEN(&t0,err);
      if(t0.type != R_PAR) err = ERR_SEM_PAR;
  } else if (!strcmp(func->name,"print")) {
      Token term;
      ADD_INS("DEFVAR TF@%ARG0\n");
      for(;;){
        CHECK_LEX_NEXT_TOKEN(&term,err);
        if (term.type == ID) {
          BTNodePtr id = NULL;
          ADD_INS("MOVE TF@%ARG0 ");
          if (GlobalParserData.defining_function) {
            id = BTNode_search(&GlobalParserData.LocalTable,term.attr.str);
            if (id) ADD_INS("LF@");
          }
          if (!id) {
            if (!(id = BTNode_search(&GlobalParserData.GlobalTable,term.attr.str)) ) {
              return ERR_SEM_DEF;
            }
            ADD_INS("GF@");
          }
          ADD_INS(term.attr.str); GEN_LF;
        } else if (term.type == STR || term.type == MULTILINE_STR){
          ADD_INS("MOVE TF@%ARG0 string@");
          ADD_INS(string2format(term.attr.str)); GEN_LF;
        } else if (term.type == INT) {
          ADD_INS("MOVE TF@%ARG0 int@");
          ADD_INS(int2str(term.attr.int_num)); GEN_LF;
        } else if (term.type == DOUBLE){
          ADD_INS("MOVE TF@%ARG0 float@");
          ADD_INS(flt2format(term.attr.decimal_num)); GEN_LF;
        } else if (term.type == KEY_WORD && term.attr.key_W == NONE) ADD_INS("MOVE TF@%ARG0 nil@nil\n");
        else return ERR_SYNTAX;
        CHECK_LEX_NEXT_TOKEN(&term,err);
        if (term.type == COMMA) {
          ADD_INS("CALL print\n");
          ADD_INS("WRITE string@\\032\n");
          continue;
        } else if (term.type == R_PAR) {
          ADD_INS("CALL print\n");
          break;
        } else return ERR_SYNTAX;

      }
      ADD_INS("WRITE string@\\010\n");
  }
  else if (!strcmp(func->name,"len")) {
    ADD_INS("DEFVAR TF@%ARG0\n");
    ADD_INS("MOVE TF@%ARG0 ");
    CHECK_LEX_NEXT_TOKEN(&t0,err); if (t0.type == ID) {
      BTNodePtr id = NULL;
      if (GlobalParserData.defining_function) {
        id = BTNode_search(&GlobalParserData.LocalTable,t0.attr.str);
        if (id) {
          ADD_INS("LF@");
        }
      }
      if (!id) {
        id = BTNode_search(&GlobalParserData.GlobalTable,t0.attr.str);
        if (id) {
          ADD_INS("GF@");
        }
      }
      if (!id) return ERR_SEM_DEF;
      ADD_INS(t0.attr.str); GEN_LF;
    } else if (t0.type == STR || t0.type == MULTILINE_STR) {
      ADD_INS("string@"); ADD_INS(string2format(t0.attr.str)); GEN_LF;
    } else return ERR_SEM_TYPE;
    ADD_INS("CALL len\n");
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == COMMA) return ERR_SEM_PAR;
    else if (t0.type == R_PAR) return ALL_OK;
    else return ERR_SYNTAX;
  }
  else if (!strcmp(func->name,"ord")) {
    ADD_INS("DEFVAR TF@%ARG0\n");
    ADD_INS("DEFVAR TF@%ARG1\n");
    ADD_INS("MOVE TF@%ARG0 ");
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == ID) {
      BTNodePtr id = NULL;
      if (GlobalParserData.defining_function) {
        id = BTNode_search(&GlobalParserData.LocalTable,t0.attr.str);
        if (id) {
          ADD_INS("LF@");
        }
      }
      if (!id) {
        id = BTNode_search(&GlobalParserData.GlobalTable,t0.attr.str);
        if (id) {
          ADD_INS("GF@");
        }
      }
      if (!id) return ERR_SEM_DEF;
      ADD_INS(t0.attr.str); GEN_LF;
    }
    else if (t0.type == STR || t0.type == MULTILINE_STR) {
      ADD_INS("string@"); ADD_INS(string2format(t0.attr.str)); GEN_LF;
    }
    else if (t0.type == INT || t0.type == DOUBLE || (t0.type == KEY_WORD && t0.attr.key_W == NONE)) return ERR_SEM_TYPE;
    else return ERR_SYNTAX;
    ADD_INS("MOVE TF@%ARG1 ");
    CHECK_LEX_NEXT_TOKEN(&t0,err); if (t0.type != COMMA) return ERR_SYNTAX;
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == ID) {
      BTNodePtr id = NULL;
      if (GlobalParserData.defining_function) {
        id = BTNode_search(&GlobalParserData.LocalTable,t0.attr.str);
        if (id) {
          ADD_INS("LF@");
        }
      }
      if (!id) {
        id = BTNode_search(&GlobalParserData.GlobalTable,t0.attr.str);
        if (id) {
          ADD_INS("GF@");
        }
      }
      if (!id) return ERR_SEM_DEF;
      ADD_INS(t0.attr.str); GEN_LF;
    }
    else if (t0.type == INT) {
      ADD_INS("int@"); ADD_INS(int2str(t0.attr.int_num)); GEN_LF;
    }
    else if (t0.type == MULTILINE_STR || t0.type == STR || t0.type == DOUBLE || (t0.type == KEY_WORD && t0.attr.key_W == NONE)) return ERR_SEM_TYPE;
    else return ERR_SYNTAX;
    ADD_INS("CALL ord\n");
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == COLON) return ERR_SEM_PAR;
    else if (t0.type == R_PAR) return ALL_OK;
    else return ERR_SYNTAX;
  }
  else if (!strcmp(func->name,"substr")) {
    ADD_INS("DEFVAR TF@%ARG0\n");
    ADD_INS("DEFVAR TF@%ARG1\n");
    ADD_INS("DEFVAR TF@%ARG2\n");
    ADD_INS("MOVE TF@%ARG0 ");
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == ID) {
      BTNodePtr id = NULL;
      if (GlobalParserData.defining_function) {
        id = BTNode_search(&GlobalParserData.LocalTable,t0.attr.str);
        if (id) {
          ADD_INS("LF@");
        }
      }
      if (!id) {
        id = BTNode_search(&GlobalParserData.GlobalTable,t0.attr.str);
        if (id) {
          ADD_INS("GF@");
        }
      }
      if (!id) return ERR_SEM_DEF;
      ADD_INS(t0.attr.str); GEN_LF;
    }
    else if (t0.type == STR || t0.type == MULTILINE_STR) {
      ADD_INS("string@"); ADD_INS(string2format(t0.attr.str)); GEN_LF;
    }
    else if (t0.type == INT || t0.type == DOUBLE || (t0.type == KEY_WORD && t0.attr.key_W == NONE)) return ERR_SEM_TYPE;
    else return ERR_SYNTAX;
    CHECK_LEX_NEXT_TOKEN(&t0,err); if (t0.type != COMMA) return ERR_SYNTAX;
    ADD_INS("MOVE TF@%ARG1 ");
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == ID) {
      BTNodePtr id = NULL;
      if (GlobalParserData.defining_function) {
        id = BTNode_search(&GlobalParserData.LocalTable,t0.attr.str);
        if (id) {
          ADD_INS("LF@");
        }
      }
      if (!id) {
        id = BTNode_search(&GlobalParserData.GlobalTable,t0.attr.str);
        if (id) {
          ADD_INS("GF@");
        }
      }
      if (!id) return ERR_SEM_DEF;
      ADD_INS(t0.attr.str); GEN_LF;
    } else if (t0.type == INT) {
      ADD_INS("int@"); ADD_INS(int2str(t0.attr.int_num)); GEN_LF;
    }
    else if (t0.type == STR || t0.type == MULTILINE_STR || t0.type == DOUBLE || (t0.type == KEY_WORD && t0.attr.key_W == NONE)) return ERR_SEM_TYPE;
    else return ERR_SYNTAX;
    CHECK_LEX_NEXT_TOKEN(&t0,err); if (t0.type != COMMA) return ERR_SYNTAX;
    ADD_INS("MOVE TF@%ARG2 ");
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == ID) {
      BTNodePtr id = NULL;
      if (GlobalParserData.defining_function) {
        id = BTNode_search(&GlobalParserData.LocalTable,t0.attr.str);
        if (id) {
          ADD_INS("LF@");
        }
      }
      if (!id) {
        id = BTNode_search(&GlobalParserData.GlobalTable,t0.attr.str);
        if (id) {
          ADD_INS("GF@");
        }
      }
      if (!id) return ERR_SEM_DEF;
      ADD_INS(t0.attr.str); GEN_LF;
    }
    else if (t0.type == INT) {
      ADD_INS("int@"); ADD_INS(int2str(t0.attr.int_num)); GEN_LF;
    }
    else if (t0.type == MULTILINE_STR || t0.type == STR || t0.type == DOUBLE || (t0.type == KEY_WORD && t0.attr.key_W == NONE) ) return ERR_SEM_TYPE;
    else return ERR_SYNTAX;
    ADD_INS("CALL substr\n");
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == COLON) return ERR_SEM_PAR;
    else if (t0.type == R_PAR) return ALL_OK;
    else return ERR_SYNTAX;
  }
  else if (!strcmp(func->name,"chr")) {
    ADD_INS("DEFVAR TF@%ARG0\n");
    ADD_INS("MOVE TF@%ARG0 ");
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == ID) {
      BTNodePtr id = NULL;
      if (GlobalParserData.defining_function) {
        id = BTNode_search(&GlobalParserData.LocalTable,t0.attr.str);
        if (id) {
          ADD_INS("LF@");
        }
      }
      if (!id) {
        id = BTNode_search(&GlobalParserData.GlobalTable,t0.attr.str);
        if (id) {
          ADD_INS("GF@");
        }
      }
      if (!id) return ERR_SEM_DEF;
      ADD_INS(t0.attr.str); GEN_LF;
    } else if (t0.type == INT) {
      ADD_INS("int@"); ADD_INS(int2str(t0.attr.int_num)); GEN_LF;
    } else if (t0.type == MULTILINE_STR || t0.type == STR || t0.type == DOUBLE ) return ERR_SEM_TYPE; else return ERR_SYNTAX;
    ADD_INS("CALL chr\n");
    CHECK_LEX_NEXT_TOKEN(&t0,err);
    if (t0.type == COLON) return ERR_SEM_PAR;
    else if (t0.type == R_PAR) return ALL_OK;
  }
  else {
    ///TODO user defined function call
  }
  return err;
}

int RETVAL(){
  int err = ALL_OK;
  Token t0,t1;
  CHECK_LEX_NEXT_TOKEN(&t0,err);
  if (IS_VALUE(t0)) {
    CHECK_LEX_NEXT_TOKEN(&t1,err);
    if (t1.type == EOL) { // return VAL
      ///TODO
      return ALL_OK;
    }
    else if (isOperator(t1.type)) {     // return EXPRESSION
      ///TODO
      err = expression_parser(input_file, &t0, &t1);
      if (err) {
        if (err == 10) err = ERR_SYNTAX;
        return err;
      }
    }
    else return ERR_SYNTAX;
  }
  else if (t0.type == ID) {
    CHECK_LEX_NEXT_TOKEN(&t1,err);
    if (t1.type == EOL) { // return ID
      ///TODO
      return ALL_OK;
    }
    else if (isOperator(t1.type)) {     // return EXPRESSION
      ///TODO
      err = expression_parser(input_file, &t0, &t1);
      if (err) {
        if (err == 10) err = ERR_SYNTAX;
        return err;
      }
    }
  }

  return err;
}

/** Auxiliary functions **/
BTData *BTDataCtor(){
  BTData *new_ptr = (BTData *) calloc(1,sizeof(BTData));
  if (!new_ptr) {
    exit(ERR_INTERNAL);
  }
  return new_ptr;
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
    default:
      return *token2str;
  }
}
bool isOperator(T_type type){
  switch (type){
    case ADD:
    case SUB:
    case DIV:
    case INT_DIV:
    case MUL:
    case LESS:
    case MORE:
    case EQUAL:
    case LESS_EQ:
    case MORE_EQ:
    case NOT_EQ:
    case L_PAR:
    case R_PAR:
      return true;
    default:
      return false;
  };
}

Data_type data_Type(T_type type){  // USE ONLY AFTER if IS_VALUE() macro
  switch (type){
    case INT:
      return type_int;
    case DOUBLE:
      return type_float;
    case STR:
      return type_string;
    case MULTILINE_STR:
      return type_multilinestr;
    default:
      return type_none;
  }
}


void add_build_in_functions(BTNodePtr *GlobalTable){
  /*    Print     */
  BTData *data = BTDataCtor();
  data->id_type = func_id;
  static char printlbl[] = "$$PRINT$$";
  static char print[] = "print";
  data->dat.data_func.label = printlbl;
  data->dat.data_func.defined = true;
  BTNode_insert(GlobalTable,print,data);
  /*    inputf    */
  data = BTDataCtor();
  data->id_type = func_id;
  static char inputflbl[] = "$$INPUTF$$";
  static char inputf[] = "inputf";
  data->dat.data_func.label = inputflbl;
  data->dat.data_func.defined = true;
  data->dat.data_func.count_of_params = 0;
  BTNode_insert(GlobalTable,inputf,data);
  /*    inputs    */
  data = BTDataCtor();
  data->id_type = func_id;
  static char inputslbl[] = "$$INPUTS$$";
  static char inputs[] = "inputs";
  data->dat.data_func.label = inputslbl;
  data->dat.data_func.defined = true;
  data->dat.data_func.count_of_params = 0;
  BTNode_insert(GlobalTable,inputs,data);
  /*    inputi    */
  data = BTDataCtor();
  data->id_type = func_id;
  static char inputilbl[] = "$$INPUTI$$";
  static char inputi[] = "inputi";
  data->dat.data_func.label = inputilbl;
  data->dat.data_func.defined = true;
  data->dat.data_func.count_of_params = 0;
  BTNode_insert(GlobalTable,inputi,data);
  /*    len    */
  data = BTDataCtor();
  data->id_type = func_id;
  static char lenlbl[] = "$$LEN$$";
  static char len[] = "len";
  data->dat.data_func.label = lenlbl;
  data->dat.data_func.defined = true;
  data->dat.data_func.count_of_params = 1;
  BTNode_insert(GlobalTable,len,data);
  /*   substr   */
  data = BTDataCtor();
  data->id_type = func_id;
  static char substrlbl[] = "$$SUBSTR$$";
  static char substr[] = "substr";
  data->dat.data_func.label = substrlbl;
  data->dat.data_func.defined = true;
  data->dat.data_func.count_of_params = 3;
  BTNode_insert(GlobalTable,substr,data);
  /*    ord   */
  data = BTDataCtor();
  data->id_type = func_id;
  static char ordlbl[] = "$$ORD$$";
  static char ord[] = "ord";
  data->dat.data_func.label = ordlbl;
  data->dat.data_func.defined = true;
  data->dat.data_func.count_of_params = 3;
  BTNode_insert(GlobalTable,ord,data);
  /*    chr   */
  data = BTDataCtor();
  data->id_type = func_id;
  static char chrlbl[] = "$$CHR$$";
  static char chr[] = "chr";
  data->dat.data_func.label = chrlbl;
  data->dat.data_func.defined = true;
  data->dat.data_func.count_of_params = 3;
  BTNode_insert(GlobalTable,chr,data);
}

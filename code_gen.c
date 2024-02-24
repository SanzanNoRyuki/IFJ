/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file code_gen.c
 * @brief Generator of the output code.
 *
 * @authors Vojtěch Ulej (xulejv00)
 * @authors Roman Fulla (xfulla00)
 *
 *
 * @date 11.12.2019
 ******************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code_gen.h"
#include "dynamic_string.h"
#include "errors.h"
#include "expression_parser.h"
#include "parser.h"
#include "scanner.h"
#include "symtable.h"

#define NUMBER_OF_FUNCTIONS 5

const char build_in[][3000] ={ "\nLABEL print\n\
        JUMPIFNEQ writestr TF@%ARG0 nil@nil\n\
        WRITE string@None\n\
        JUMP afterstr\n\
        LABEL writestr\n\
        WRITE TF@%ARG0\n\
        LABEL afterstr\n\
        MOVE TF@%%assign% nil@nil\n\
        RETURN\n",
        "\nLABEL len\n\
        DEFVAR TF@$type\n\
        TYPE TF@$type TF@%ARG0\n\
        JUMPIFNEQ $wrong_type0 TF@$type string@string\n\
        STRLEN TF@%%assign% TF@%ARG0\n\
        RETURN\n\
        LABEL $wrong_type0\n\
          EXIT int@4\n",
        "\nLABEL ord\n\
        PUSHFRAME\n\
        DEFVAR LF@$type\n\
        TYPE LF@$type LF@%ARG0\n\
        JUMPIFNEQ $wrong_type1 LF@$type string@string\n\
        TYPE LF@$type LF@%ARG1\n\
        JUMPIFNEQ $wrong_type1 LF@$type string@int\n\
        DEFVAR LF@$not_in_range\n\
        LT LF@$not_in_range LF@%ARG1 int@0\n\
        JUMPIFEQ $out_of_range2 LF@$not_in_range bool@true\n\
        DEFVAR LF@$string_length\n\
        CREATEFRAME\n\
        DEFVAR TF@%%assign%\n\
        DEFVAR TF@%ARG0 \n\
        MOVE TF@%ARG0 LF@%ARG0\n\
        CALL len\n\
        MOVE LF@$string_length TF@%%assign%\n\
        SUB LF@$string_length LF@$string_length int@1\n\
        GT LF@$not_in_range LF@%ARG1 LF@$string_length\n\
        JUMPIFEQ $out_of_range2 LF@$not_in_range bool@true\n\
        STRI2INT LF@%%assign% LF@%ARG0 LF@%ARG1\n\
        POPFRAME\n\
        RETURN\n\
        LABEL $out_of_range2\n\
        MOVE LF@%%assign% nil@nil\n\
        POPFRAME\n\
        RETURN\n\
        LABEL $wrong_type1\n\
        POPFRAME\n\
        EXIT int@4\n"
,
        "\nLABEL substr\n\
        PUSHFRAME\n\
        DEFVAR LF@$char\n\
        DEFVAR LF@$index\n\
        DEFVAR LF@$type\n\
        DEFVAR LF@$end\n\
        MOVE LF@$index LF@%ARG1\n\
        MOVE LF@%%assign% string@\n\
        TYPE LF@$type LF@%ARG0\n\
        JUMPIFNEQ $wrong_type5 LF@$type string@string\n\
        TYPE LF@$type LF@%ARG1\n\
        JUMPIFNEQ $wrong_type5 LF@$type string@int\n\
        TYPE LF@$type LF@%ARG2\n\
        JUMPIFNEQ $wrong_type5 LF@$type string@int\n\
        DEFVAR LF@$not_in_range\n\
        LT LF@$not_in_range LF@%ARG1 int@0\n\
        JUMPIFEQ $out_of_range3 LF@$not_in_range bool@true\n\
        DEFVAR LF@$string_length\n\
        CREATEFRAME\n\
        DEFVAR TF@%%assign%\n\
        DEFVAR TF@%ARG0\n\
        MOVE TF@%ARG0 LF@%ARG0\n\
        CALL len\n\
        MOVE LF@$string_length TF@%%assign%\n\
        GT LF@$not_in_range LF@%ARG1 LF@$string_length\n\
        JUMPIFEQ $out_of_range3 LF@$not_in_range bool@true\n\
        LT LF@$not_in_range LF@%ARG2 int@0\n\
        JUMPIFEQ $out_of_range3 LF@$not_in_range bool@true\n\
        SUB LF@$string_length LF@$string_length LF@%ARG1\n\
        GT LF@$not_in_range LF@%ARG2 LF@$string_length\n\
        ADD LF@$string_length LF@$string_length LF@%ARG1\n\
        SUB LF@$string_length LF@$string_length int@1\n\
        JUMPIFEQ $return_rest_of_s LF@$not_in_range bool@true\n\
        LABEL $return\n\
        GETCHAR LF@$char LF@%ARG0 LF@$index\n\
        CONCAT LF@%%assign% LF@%%assign% LF@$char\n\
        ADD LF@$index LF@$index int@1\n\
        SUB LF@%ARG2 LF@%ARG2 int@1\n\
        GT LF@$end LF@%ARG2 int@0\n\
        JUMPIFEQ $return LF@$end bool@true\n\
        POPFRAME\n\
        RETURN\n\
        LABEL $return_rest_of_s\n\
        GETCHAR LF@$char LF@%ARG0 LF@$index\n\
        CONCAT LF@%%assign% LF@%%assign% LF@$char\n\
        ADD LF@$index LF@$index int@1\n\
        GT LF@$end LF@$index LF@$string_length\n\
        JUMPIFEQ $return_rest_of_s LF@$end bool@false\n\
        POPFRAME\n\
        RETURN\n\
        LABEL $out_of_range3\n\
          MOVE LF@%%assign% nil@nil\n\
          POPFRAME\n\
          RETURN\n\
        LABEL $wrong_type5\n\
          POPFRAME\n\
          EXIT int@4\n",
        "\nLABEL chr\n\
        DEFVAR TF@$type\n\
        TYPE TF@$type TF@%ARG0\n\
        JUMPIFNEQ $error10 TF@$type string@int\n\
        DEFVAR TF@$not_in_range\n\
        LT TF@$not_in_range TF@%ARG0 int@0\n\
        JUMPIFEQ $error10 TF@$not_in_range bool@true\n\
        GT TF@$not_in_range TF@%ARG0 int@255\n\
        JUMPIFEQ $error10 TF@$not_in_range bool@true\n\
        INT2CHAR TF@%%assign% TF@%ARG0\n\
        RETURN\n\
        LABEL $error10\n\
        EXIT int@4\n"
};

/******************************************************************************/
/*                              Basic functions                               */
/******************************************************************************/

void code_gen_init(code_gen *cg) {
  assert(cg);

  static dm_str Global;
  static dm_str funcs;

  cg->global_body = &Global;
  cg->functions   = &funcs;

  dm_str_init(cg->global_body);
  dm_str_init(cg->functions);
}

void code_gen_clear(code_gen *cg) {
  dm_str_free(cg->global_body);
  dm_str_free(cg->functions);
}

void code_gen_return(code_gen *cg, FILE *output_file) {
  assert(output_file);
  assert(cg);

  fprintf(output_file,".IFJcode19\nJUMP $$MAIN$$\n");

  /*  build-in functions  */
  for (int i = 0; i < NUMBER_OF_FUNCTIONS; ++i) fprintf(output_file,"\n%s\n",build_in[i]);

  /*  user defined functions  */
  fprintf(output_file,"%s\n",dm_str_first(cg->functions));      //Print is definition of print function

  /*  "Main" function  */
  fprintf(output_file,"LABEL $$MAIN$$\n"); // Unique label for "main" function;
  fprintf(output_file,"%s\n",dm_str_first(cg->global_body));
  code_gen_clear(cg);
}

void code_gen_add_instruction(code_gen *cg,cg_type type,const char *instruction){
    assert(cg);
    if (type == Global) {
        dm_str_add_str(cg->global_body,instruction);
    }
    else if (type == Function) {
        dm_str_add_str(cg->global_body,instruction);
    }
}

char *LabelGen(const char *func_id){    // Should generate unique labels for functions
  GlobalParserData.label_gen++;
  dm_str label;
  dm_str_init(&label);
  dm_str_add_str(&label,"%label_");
#define MAX_NUMBER_OF_INT_CHARACTERS 13
  char lbln[MAX_NUMBER_OF_INT_CHARACTERS];
#undef MAX_NUMBER_OF_INT_CHARACTERS
  sprintf(lbln,"%lld",GlobalParserData.label_gen);
  dm_str_add_str(&label,lbln);
  dm_str_add_str(&label,func_id);
  dm_str_add_c(&label,'%');
  return dm_str_first(&label);
}


const char *string2format(const char *str){
  int len = strlen(str);
  dm_str new_str;
  dm_str_init(&new_str);

  for (int i = 0; i < len; ++i) {
    if ((str[i] >= 0 && str[i] <= 32) || str[i] == 35 || str[i] == 92) {
      char news[5];
      if (str[i] >= 10)
        sprintf(news,"\\0%d",str[i]);
      else
        sprintf(news,"\\00%d",str[i]);
      dm_str_add_str(&new_str,news);
      continue;
    }
    dm_str_add_c(&new_str,str[i]);

  }
  return dm_str_first(&new_str);
}

const char *int2str(const int i){
  static char strint[13];
  sprintf(strint,"%d",i);
  return strint;
}

const char *flt2format(const double d){
  static char strflt[40];
  sprintf(strflt,"%a",d);
  return strflt;
}

/******************************************************************************/
/*                              Parser functions                              */
/******************************************************************************/

// TODO

/******************************************************************************/
/*                          Generator of expressions                          */
/******************************************************************************/

void code_gen_type_init() {
  ADD_INS("DEFVAR TF@%TYPE2");
  GEN_LF;
  ADD_INS("DEFVAR TF@%TYPE1");
  GEN_LF;
}

void code_gen_type(epitem *op2, epitem *op1) {
  ADD_INS("TYPE TF@%TYPE2 TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  GEN_LF;

  ADD_INS("TYPE TF@%TYPE1 TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;
}

int code_gen_operand(epitem *operand) {
  ADD_INS("DEFVAR TF@%OPERAND");
  ADD_INS(int2str(operand->index));
  GEN_LF;

  if (operand->token.type == ASSIGN) return ALL_OK;                             // Result

  ADD_INS("MOVE TF@%OPERAND");
  ADD_INS(int2str(operand->index));
  ADD_INS(" ");

  if (operand->token.type == ID) {                                              // ID
    BTNodePtr id = NULL;
    if (GlobalParserData.defining_function) {
      id = BTNode_search(&GlobalParserData.LocalTable,operand->token.attr.str);
      if (id != NULL) ADD_INS("LF@");                                           // Local Frame
    }
    if (!id) {
      if (!(id = BTNode_search(&GlobalParserData.GlobalTable,operand->token.attr.str)) ) {
        return ERR_SEM_DEF;
      }
      ADD_INS("GF@");                                                           // Global Frame
    }
    ADD_INS(operand->token.attr.str);
    GEN_LF;
  }
  else if (operand->token.type == STR || operand->token.type == MULTILINE_STR) {// STR
    ADD_INS("string@");
    ADD_INS(string2format(operand->token.attr.str));
    GEN_LF;
  }
  else if (operand->token.type == DOUBLE) {                                     // DOUBLE
    ADD_INS("float@");
    ADD_INS(flt2format(operand->token.attr.decimal_num));
    GEN_LF;
  }
  else if (operand->token.type == INT) {                                        // INT
    ADD_INS("int@");
    ADD_INS(int2str(operand->token.attr.int_num));
    GEN_LF;
  }
  else if (operand->token.type == KEY_WORD) {                                   // NONE
    ADD_INS("nil@nil");
    GEN_LF;
  }

  return ALL_OK;
}

void code_gen_int2double(epitem *operand) {
  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(operand->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(operand->index));
  GEN_LF;
}

void code_gen_parameters(epitem *op2, epitem *op1, epitem *result) {
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;
}

void code_gen_not(epitem *operand) {
  ADD_INS("NOT");
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(operand->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(operand->index));
  GEN_LF;
}

void code_gen_add(epitem *op2, epitem *op1, epitem *result, bool are_str) {
  if (are_str == false) ADD_INS("ADD");
  else                  ADD_INS("CONCAT");
  code_gen_parameters(op2,op1,result);
}
void code_gen_sub(epitem *op2, epitem *op1, epitem *result) {
  ADD_INS("SUB");
  code_gen_parameters(op2,op1,result);
}
void code_gen_mul(epitem *op2, epitem *op1, epitem *result) {
  ADD_INS("MUL");
  code_gen_parameters(op2,op1,result);
}
void code_gen_div(epitem *op2, epitem *op1, epitem *result) {
  ADD_INS("DIV");
    code_gen_parameters(op2,op1,result);
}
void code_gen_int_div(epitem *op2, epitem *op1, epitem *result) {
  ADD_INS("IDIV");
  code_gen_parameters(op2,op1,result);
}

void code_gen_less(epitem *op2, epitem *op1, epitem *result) {
  ADD_INS("LT");
  code_gen_parameters(op2,op1,result);
}
void code_gen_less_eq(epitem *op2, epitem *op1, epitem *result) {
  ADD_INS("GT");
  code_gen_parameters(op2,op1,result);
  code_gen_not(result);
}
void code_gen_more(epitem *op2, epitem *op1, epitem *result) {
  ADD_INS("GT");
  code_gen_parameters(op2,op1,result);
}
void code_gen_more_eq(epitem *op2, epitem *op1, epitem *result) {
  ADD_INS("LT");
  code_gen_parameters(op2,op1,result);
  code_gen_not(result);
}
void code_gen_equal(epitem *op2, epitem *op1, epitem *result) {
  ADD_INS("EQ");
  code_gen_parameters(op2,op1,result);
}
void code_gen_not_eq(epitem *op2, epitem *op1, epitem *result) {
  ADD_INS("EQ");
  code_gen_parameters(op2,op1,result);
  code_gen_not(result);
}

/**/
void code_gen_var_add(epitem *op2, epitem *op1, epitem *result) {
  char *op1int = LabelGen("OP1INT");
  char *err = LabelGen("ERR");
  char *end = LabelGen("END");
  char *same = LabelGen("SAME");
  char *string = LabelGen("STRING");
  char *addition = LabelGen("ADDITION");


  ADD_INS("JUMPIFEQ ");
  ADD_INS(same);
  ADD_INS(" TF@%TYPE1 TF@%TYPE2");
  GEN_LF;
  ADD_INS("JUMPIFEQ ");

  ADD_INS(err);
  ADD_INS(" TF@%TYPE1 string@string");
  GEN_LF;
  ADD_INS("JUMPIFEQ ");

  ADD_INS(err );
  ADD_INS(" TF@%TYPE2 string@string");
  GEN_LF;
  ADD_INS("JUMPIFEQ ");

  ADD_INS(err );
  ADD_INS(" TF@%TYPE1 string@nil");
  GEN_LF;
  ADD_INS("JUMPIFEQ ");

  ADD_INS(err );
  ADD_INS(" TF@%TYPE2 string@nil");
  GEN_LF;
  ADD_INS("JUMPIFEQ ");

  ADD_INS(op1int);
  ADD_INS(" TF@%TYPE1 string@int");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(addition);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(op1int);
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(addition);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(same);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(string);
  ADD_INS(" TF@%TYPE1 string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(addition);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(addition);
  ADD_INS(" TF@%TYPE1 string@int");
  GEN_LF;


  ADD_INS("LABEL ");
  ADD_INS(err);
  GEN_LF;

  ADD_INS("EXIT int@4");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(string);
  GEN_LF;

  ADD_INS("CONCAT TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(end);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(addition);
  GEN_LF;
  ADD_INS("ADD TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(end);
  GEN_LF;
}

void code_gen_var_sub(epitem *op2, epitem *op1, epitem *result) {
  char *same = LabelGen("SAME");
  char *err = LabelGen("ERR");
  char *op1int = LabelGen("OP1INT");
  char *operation = LabelGen("OPERATION");

  ADD_INS("JUMPIFEQ ");
  ADD_INS(same);
  ADD_INS(" TF@%TYPE1 TF@%TYPE2");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1 string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2 string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1 string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2 string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(op1int);
  ADD_INS(" TF@%TYPE1 string@int");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(op1int);
  GEN_LF;


  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(same);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 string@int");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(err);
  GEN_LF;

  ADD_INS("EXIT int@4");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("SUB TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;
}

void code_gen_var_mul(epitem *op2, epitem *op1, epitem *result) {
  char *same = LabelGen("SAME");
  char *err = LabelGen("ERR");
  char *op1int = LabelGen("OP1INT");
  char *operation = LabelGen("OPERATION");

  ADD_INS("JUMPIFEQ ");
  ADD_INS(same);
  ADD_INS(" TF@%TYPE1 TF@%TYPE2");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1 string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2 string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1 string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2 string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(op1int);
  ADD_INS(" TF@%TYPE1 string@int");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(op1int);
  GEN_LF;


  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(same);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 string@int");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(err);
  GEN_LF;

  ADD_INS("EXIT int@4");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("MUL TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;
}

void code_gen_var_div(epitem *op2, epitem *op1, epitem *result) {
  char *same = LabelGen("SAME");
  char *err = LabelGen("ERR");
  char *op1int = LabelGen("OP1INT");
  char *operation = LabelGen("OPERATION");
  char *convert = LabelGen("CONVERT");
  char *zero = LabelGen("ZERO");

  ADD_INS("JUMPIFEQ ");
  ADD_INS(same);
  ADD_INS(" TF@%TYPE1 TF@%TYPE2");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1 string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2 string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1 string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2 string@nil");
  GEN_LF;


  ADD_INS("LABEL ");
  ADD_INS(convert);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(op1int);
  ADD_INS(" TF@%TYPE2 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(op1int);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE2 string@float");
  GEN_LF;


  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(same);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(convert);
  ADD_INS(" TF@%TYPE1 string@int");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(err);
  GEN_LF;

  ADD_INS("EXIT int@4");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(zero);
  GEN_LF;

  ADD_INS("EXIT int@9");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(zero);
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  ADD_INS(" float@0x0p+0");
  GEN_LF;

  ADD_INS("DIV TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;
}

void code_gen_var_int_div(epitem *op2, epitem *op1, epitem *result) {
  char *same = LabelGen("SAME");
  char *err = LabelGen("ERR");
  char *operation = LabelGen("OPERATION");
  char *zero = LabelGen("ZERO");

  ADD_INS("JUMPIFEQ ");
  ADD_INS(same);
  ADD_INS(" TF@%TYPE1 TF@%TYPE2");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1 string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2 string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1 string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2 string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2 string@float");
  GEN_LF;


  ADD_INS("LABEL ");
  ADD_INS(same);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 string@int");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(err);
  GEN_LF;

  ADD_INS("EXIT int@4");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(zero);
  GEN_LF;

  ADD_INS("EXIT int@9");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(zero);
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  ADD_INS(" int@0");
  GEN_LF;

  ADD_INS("IDIV TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;
}

void code_gen_var_less(epitem *op2, epitem *op1, epitem *result) {
  char *err = LabelGen("ERR");
  char *op1int = LabelGen("OP1INT");
  char *operation = LabelGen("OPERATION");

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1  string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2  string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1  string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2  string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 TF@%TYPE2");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(op1int);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(op1int);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(err);
  GEN_LF;

  ADD_INS("EXIT int@4");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LT TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;
}

void code_gen_var_less_eq(epitem *op2, epitem *op1, epitem *result) {
  char *err = LabelGen("ERR");
  char *op1int = LabelGen("OP1INT");
  char *operation = LabelGen("OPERATION");

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1  string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2  string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1  string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2  string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 TF@%TYPE2");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(op1int);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(op1int);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(err);
  GEN_LF;

  ADD_INS("EXIT int@4");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("GT TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  code_gen_not(result);
}

void code_gen_var_more(epitem *op2, epitem *op1, epitem *result) {
  char *err = LabelGen("ERR");
  char *op1int = LabelGen("OP1INT");
  char *operation = LabelGen("OPERATION");

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1  string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2  string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1  string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2  string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 TF@%TYPE2");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(op1int);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(op1int);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(err);
  GEN_LF;

  ADD_INS("EXIT int@4");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("GT TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;
}

void code_gen_var_more_eq(epitem *op2, epitem *op1, epitem *result) {
  char *err = LabelGen("ERR");
  char *op1int = LabelGen("OP1INT");
  char *operation = LabelGen("OPERATION");

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1  string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2  string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1  string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2  string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 TF@%TYPE2");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(op1int);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(op1int);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(err);
  GEN_LF;

  ADD_INS("EXIT int@4");
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LT TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  code_gen_not(result);
}

void code_gen_var_equal(epitem *op2, epitem *op1, epitem *result) {
  char *err = LabelGen("ERR");
  char *op1int = LabelGen("OP1INT");
  char *operation = LabelGen("OPERATION");
  char *end = LabelGen("END");

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1  string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2  string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1  string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2  string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 TF@%TYPE2");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(op1int);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(op1int);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(err);
  GEN_LF;

  ADD_INS("MOVE TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" bool@false");
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(end);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("EQ TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(end);
  GEN_LF;
}

void code_gen_var_not_eq(epitem *op2, epitem *op1, epitem *result) {
  char *err = LabelGen("ERR");
  char *op1int = LabelGen("OP1INT");
  char *operation = LabelGen("OPERATION");
  char *end = LabelGen("END");

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1  string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2  string@nil");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE1  string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(err);
  ADD_INS(" TF@%TYPE2  string@string");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 TF@%TYPE2");
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(op1int);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(op1int);
  GEN_LF;

  ADD_INS("JUMPIFEQ ");
  ADD_INS(operation);
  ADD_INS(" TF@%TYPE1 string@float");
  GEN_LF;

  ADD_INS("INT2FLOAT TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(err);
  GEN_LF;

  ADD_INS("MOVE TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" bool@false");
  GEN_LF;

  ADD_INS("JUMP ");
  ADD_INS(end);
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(operation);
  GEN_LF;

  ADD_INS("EQ TF@%OPERAND");
  ADD_INS(int2str(result->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op2->index));
  ADD_INS(" TF@%OPERAND");
  ADD_INS(int2str(op1->index));
  GEN_LF;

  ADD_INS("LABEL ");
  ADD_INS(end);
  GEN_LF;

  code_gen_not(result);
}

void code_gen_result(int result_index) {
  ADD_INS("MOVE TF@%%assign% TF@%OPERAND");
  ADD_INS(int2str(result_index));
  GEN_LF;
}
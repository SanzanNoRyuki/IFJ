/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file code_gen.h
 * @brief Header file for the code generator.
 *
 * @authors Vojtěch Ulej (xulejv00)
 * @authors Roman Fulla (xfulla00)
 *
 *
 * @date 10.12.2019
 ******************************************************************************/

#ifndef CODE_GEN_H
#define CODE_GEN_H


#include <stdio.h>

#include "dynamic_string.h"
#include "expression_parser.h"

#define ADD_INS(ins)\
code_gen_add_instruction(\
code,GlobalParserData.defining_function ? Global : Function,ins)

#define GEN_LF                                                                 \
  code_gen_add_instruction(                                                    \
      code, GlobalParserData.defining_function ? Global : Function, "\n")

typedef struct {
    dm_str *global_body;
    dm_str *functions;
} code_gen;

typedef enum {
    Global,
    Function
} cg_type;

/******************************************************************************/
/*                              Basic functions                               */
/******************************************************************************/

/**
  * @brief Function initializes code generator.
  *
  * @param Pointer to the code generator.
  */
void code_gen_init(code_gen *cg);

/**
  * @brief Function deallocates memory used by code generator.
  *
  * @param Pointer to the code generator.
  */
void code_gen_clear(code_gen *cg);

/**
  * @brief Function prints generated code into the output file.
  *
  * @param Pointer to the code generator.
  * @param Output file.
  */
void code_gen_return(code_gen *cg,FILE *output_file);

/**
  * @brief Function generates unique labels.
  *
  * @param String used to generate unique label.
  *
  * @return Unique label.
  */
char *LabelGen(const char *func_id);

/**
  * @brief Adds instruction to the generated code.
  *
  * @param Pointer to the code generator.
  * @param Code generator type.
  * @param Added Instruction.
  */
void code_gen_add_instruction(code_gen *cg,cg_type type,const char *instruction);

/**
  * @brief Function generates integer to string tranformation.
  *
  * @param Integer to transform.
  */
const char *int2str(const int i);

/**
  * @brief Function generates floating point number to string tranformation.
  *
  * @param Float to transform.
  */
const char *flt2format(const double d);

/**
  * @brief Function generates string to formatted string tranformation.
  *
  * @param String to transform.
  */
const char *string2format(const char *str);

/******************************************************************************/
/*                              Parser functions                              */
/******************************************************************************/

// TODO

/******************************************************************************/
/*                          Generator of expressions                          */
/******************************************************************************/

/**
  * @brief Function initializes type operands.
  */
void code_gen_type_init();

/**
  * @brief Function generates type declaration of the operands.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  */
void code_gen_type(epitem *op2, epitem *op1);

/**
  * @brief Function generates operand declaration.
  *
  * @param Pointer to the operand.
  */
int code_gen_operand(epitem *operand);

/**
  * @brief Function generates integer to double tranformation.
  *
  * @param Pointer to the operand.
  */
void code_gen_int2double(epitem *operand);

/**
  * @brief Function generates addition code.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  * @param Switch changes if operands are strings / numbers.
  */
void code_gen_add(epitem *op2, epitem *op1, epitem *result, bool is_str);

/**
  * @brief Function generates subtraction code.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_sub(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates multiplication code.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_mul(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates division code.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_div(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates integer division code.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_int_div(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates less comparison code.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_less(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates less or equal comparison code.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_less_eq(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates more comparison code.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_more(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates more or equal comparison code.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_more_eq(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates equal comparison code.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_equal(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates not equal comparison code.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_not_eq(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates variable addition operation.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_var_add(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates variable subtraction operation.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_var_sub(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates variable multiplication operation.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_var_mul(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates variable division operation.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_var_div(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates variable integer division operation.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_var_int_div(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates variable comparison operation.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_var_less(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates variable comparison operation.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_var_less_eq(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates variable comparison operation.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_var_more(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates variable comparison operation.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_var_more_eq(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates variable comparison operation.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_var_equal(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function generates variable comparison operation.
  *
  * @param Pointer to the first operand.
  * @param Pointer to the second operand.
  * @param Pointer to the result.
  */
void code_gen_var_not_eq(epitem *op2, epitem *op1, epitem *result);

/**
  * @brief Function assigns the result of the expression.
  *
  * @param Index of result operand.
  */
void code_gen_result(int result_index);

#endif

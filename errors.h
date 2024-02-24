/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file errors.h
 * @brief Return values in case of an error.
 *
 * @authors Roman Fulla (xfulla00)
 *
 *
 * @date 02.11.2019
 ******************************************************************************/

#ifndef ERRORS_H
#define ERRORS_H


#define ALL_OK        0   /// No reported error (Everything is OK)
#define ERR_LEX       1   /// Lexical error
#define ERR_SYNTAX    2   /// Syntax error
#define ERR_SEM_DEF   3   /// Semantic definition error (e.g. undefined variable)
#define ERR_SEM_TYPE  4   /// Semantic type error (e.g. uncompatible types)
#define ERR_SEM_PAR   5   /// Semantic parameter number error (e.g. too many)
#define ERR_SEM_OTHER 6   /// Other semantic errors
#define ERR_ZERO_DIV  9   /// Division by zero
#define ERR_INTERNAL  99  /// Internal error (e.g. memory allocation error)

#endif
/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file dynamic_string.h
 * @brief Header file for the dynamic string manipulation.
 *
 * @authors Roman Fulla (xfulla00)
 *
 *
 * @date 9.12.2019
 ******************************************************************************/

#ifndef DM_STR_H
#define DM_STR_H


#define DEFAULT_ALLOC_BUFFER 8

typedef struct {               /// Dynamic string.
  char *array;                 /// Pointer to an array of characters.
  unsigned length;             /// String length.
  unsigned size;               /// Allocated memory in bytes.
} dm_str;

/**
  * @brief Function initializes a dynamic string to an empty string.
  *
  * @param Pointer to the dynamic string.
  */
void dm_str_init(dm_str *str);

/**
  * @brief Function appends one character to a dynamic string.
  *
  * @param Pointer to the dynamic string.
  * @param Character to append.
  */
void dm_str_add_c(dm_str *str, char c);

/**
  * @brief Function appends a constant string to a dynamic string.
  *
  * @param Pointer to the dynamic string.
  * @param Pointer to the constant string.
  */
void dm_str_add_str(dm_str *str, const char *ct_str);

/**
  * @brief Function returns pointer to a first character of a dynamic string.
  *
  * @param Pointer to the dynamic string.
  */
char *dm_str_first(dm_str *str);

/**
  * @brief Function returns pointer to a last character of a dynamic string.
  *
  * @param Pointer to the dynamic string.
  */
char *dm_str_last(dm_str *str);

/**
  * @brief Function compares a dynamic string with a constant string.
  *
  * @param Pointer to the dynamic string.
  * @param Pointer to the constant string.
  *
  * @return -1 if the dynamic string is greater.
  * @return  0 if the strings are equal.
  * @return  1 if the constant string is greater.
  */
int dm_str_cmp(dm_str *str, const char *ct_str);

/**
  * @brief Function copies a dynamic string.
  *
  * @param Pointer to the source string.
  * @param Pointer to the destination string.
  */
void dm_str_cpy(dm_str *src,dm_str *dest);

/**
  * @brief Function deallocates memory of a dynamic string.
  *
  * @param Pointer to the dynamic string.
  */
void dm_str_free(dm_str *str);

#endif

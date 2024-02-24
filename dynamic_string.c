/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file dynamic_string.c
 * @brief Manipulation with dynamic strings.
 *
 * @authors Roman Fulla (xfulla00)
 *
 *
 * @date 9.12.2019
 ******************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_string.h"
#include "errors.h"

int ALLOC_BUFFER = DEFAULT_ALLOC_BUFFER;

void dm_str_alloc(dm_str *str, unsigned inc) {                                  // Allocation of memory for a dynamic string
  str->size = str->size + inc;
  char *tmp_ptr = (char*) realloc(str->array,str->size);

  if (tmp_ptr != NULL) str->array = tmp_ptr;
  else {
    dm_str_free(str);
    fprintf(stderr,"Dynamic string reallocation has failed -"
                   " dynamic string was wiped out.\n");
    exit(ERR_INTERNAL);
  }
}

void dm_str_init(dm_str *str) {
  assert(str != NULL);

  str->array = (char*) calloc(1,sizeof(char));                                  // Initiating empty string - "\0"
  str->length = 0;
  str->size = 1;

  if (str->array == NULL) {
    fprintf(stderr,"Dynamic string initiation has failed.\n");
    exit(ERR_INTERNAL);
  }
}

void dm_str_add_c(dm_str *str, char c) {
  assert(str != NULL);

  if (str->array == NULL) dm_str_init(str);                                     // NULL string is transformed to initialized string

  if (c == '\0') return;                                                        // Double '\0' protection

  if (str->size <= str->length + 1) {
    dm_str_alloc(str,1 + ALLOC_BUFFER);
  }

  str->array[str->length] = c;
  str->length = str->length + 1;

  str->array[str->length] = '\0';
}

void dm_str_add_str(dm_str *str, const char *ct_str) {
  assert(str != NULL);
  assert(ct_str != NULL);

  if (str->array == NULL) dm_str_init(str);                                     // NULL string is transformed to initialized string

  unsigned short ct_str_len = strlen(ct_str);

  if (str->size <= str->length + ct_str_len) {
    dm_str_alloc(str,ct_str_len + ALLOC_BUFFER);
  }

  str->array = strcat(str->array,ct_str);
  str->length = str->length + ct_str_len;
}

char *dm_str_first(dm_str *str) {
  assert(str != NULL);

  if (str->array == NULL) dm_str_init(str);                                     // NULL string is transformed to initialized string

  return str->array;
}

char *dm_str_last(dm_str *str) {
  assert(str != NULL);

  if (str->array == NULL) {
    dm_str_init(str);                                                           // NULL string is transformed to initialized string
    return str->array;
  }

  return &(str->array[str->length - 1]);
}

int dm_str_cmp(dm_str *str, const char *ct_str) {
  assert(str != NULL);
  assert(ct_str != NULL);

  if (str->array == NULL) dm_str_init(str);                                     // NULL string is transformed to initialized string

  return strcmp(str->array,ct_str);
}

void dm_str_cpy(dm_str *src, dm_str *dest) {
  assert(src != NULL);
  assert(dest != NULL);

  if (src->array == NULL) dm_str_init(src);                                     // NULL string is transformed to initialized string
  if (dest->array == NULL) dm_str_init(dest);                                   // NULL string is transformed to initialized string

  if (dest->size <= src->length) {
    dm_str_alloc(dest,src->length + 1 - dest->size + ALLOC_BUFFER);
  }

  strcpy(dest->array,src->array);
  dest->length = src->length;
}

void dm_str_free(dm_str *str) {
  if (str == NULL) return;

  if (str->array != NULL) free(str->array);

  str->array = NULL;
  str->length = 0;
  str->size = 0;
}

/******************************************************************************
 * @project IFJ19 Compiler
 *
 * @file main.c
 * @brief Main function of the IFJ19Compiler.
 *
 * @authors Vojtěch Ulej (xulejv00)
 *
 *
 * @date 22.10.2019
 ******************************************************************************/


#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "code_gen.h" // Headfile for code generator
#include "parser.h"

void parse_Args(int argc,FILE **in,FILE **out,const char **argv);

int main(int argc, char const *argv[]){

  FILE *in = stdin;
  FILE *out = stdout;

  parse_Args(argc,&in,&out,argv);

  int error;
  error = Analysis(in);  // Zavolání syntaktické analýzy
  if (error) {
    code_gen_clear(code);
    fprintf(stderr, "An error has occured during code analysis. Parser returned %d\n",error);
    return error;
  }
  code_gen_return(code,out);
  return ALL_OK;
}


void parse_Args(int argc,FILE **in,FILE **out,const char **argv){

    if (argc > 1) {
      if (argc == 3) {
        if (!strcmp(argv[1],"-i") || !strcmp(argv[1],"--input")) {
            *in = fopen(argv[2],"r");
            if(!*in){
              fprintf(stderr, "Cannot open input file: %s\n",argv[2]);
              *in = stdin;
            }
        }
        else if (!strcmp(argv[1],"-o") || !strcmp(argv[1],"--output")) {
          *out = fopen(argv[2],"w");
          if(!*out){
            fprintf(stderr, "Cannot open output file: %s\n",argv[2]);
            *out = stdout;
          }
        }
      }
      else if (argc == 5){
        for (size_t i = 1; i < 5; i++) {
          if (!strcmp(argv[i],"-i") || !strcmp(argv[i],"--input")) {
            *in = fopen(argv[++i],"r");
            if(!*in){
              fprintf(stderr, "Cannot open input file: %s\n",argv[i-1]);
              *in = stdin;
            }
          }
          else if (!strcmp(argv[i],"-o") || !strcmp(argv[i],"--output")) {
            *out = fopen(argv[++i],"w");
            if(!*out){
              fprintf(stderr, "Cannot open output file: %s\n",argv[i-1]);
              *out = stdout;
            }
        }
      }
    }
      else{
        fprintf(stderr, "Error invalid number of arguments %d\n",argc);
        fprintf(stderr, "Using stdin as input file and stdout as output file\n");
      }
    }

}

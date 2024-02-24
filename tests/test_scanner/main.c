#include<stdio.h>
#include"../../scanner.h"



int main(){
  Token t;
  gimme_Token(stdin,&t);

  if(t.type == INT){
    printf("INT");	
  }
  if(t.type == ID || t.type == STR || t.type == MULTILINE_STR){
    printf("STR: %s\n",t.attr.str);
  }
 


}

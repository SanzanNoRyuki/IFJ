#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../dynamic_string.h"

int main() {
  dm_str     *dm_string = (dm_str*) malloc(sizeof(dm_str));
  dm_str     *dm_copy   = (dm_str*) malloc(sizeof(dm_str));
  const char *ct_str    = "Hope My Death Makes More Cents Than My Life.";

  if (dm_string == NULL || dm_copy == NULL) {
    fprintf(stderr, "Dynamic string declaration has failed.\n");
    return 999;
  }

  /*--------------------------------------------------------------------------*/
  /*------------------------------ BASIC  TESTS ------------------------------*/
  /*--------------------------------------------------------------------------*/

  printf("_____________________________\n");
  printf("00000000 BASIC TESTS 00000000\n");

  printf("######## INIT TEST ########\n");
  dm_str_init(dm_string);
  if (dm_str_cmp(dm_string,"") == 0) printf("Success!\n");
  else                               printf("Failure!\n");

  printf("##### FREE TEST EMPTY #####\n");
  dm_str_free(dm_string);
  if (dm_string->array == NULL) printf("Success!\n");
  else                          printf("Failure!\n");

  printf("######## ADD CHAR ########\n");
  dm_str_add_c(dm_string,'A');
  if (strcmp(dm_string->array,"A") == 0) printf("Success!\n");
  else                                   printf("Failure!\n");

  printf("##### ADD MORE CHARS #####\n");
  dm_str_add_c(dm_string,'B');
  dm_str_add_c(dm_string,'C');
  if (strcmp(dm_string->array,"ABC") == 0) printf("Success!\n");
  else                                     printf("Failure!\n");

  printf("#### ADD STR NOT EMPTY ####\n");
  dm_str_add_str(dm_string," - MID or FEED");
  if (strcmp(dm_string->array,"ABC - MID or FEED") == 0) printf("Success!\n");
  else                                                   printf("Failure!\n");

  printf("########## FIRST ##########\n");
  if (*dm_str_first(dm_string) == 'A') printf("Success!\n");
  else                                 printf("Failure!\n");

  printf("########## LAST ##########\n");
  if (*dm_str_last(dm_string) == 'D') printf("Success!\n");
  else                                printf("Failure!\n");

  printf("####### CMP NOT EQ #######\n");
  if (dm_str_cmp(dm_string,ct_str) != 0) printf("Success!\n");
  else                                   printf("Failure!\n");

  dm_str_init(dm_copy);
  dm_str_add_str(dm_copy,"Hope My Death Makes More Cents Than My Life.");
  printf("########## COPY ##########\n");
  dm_str_cpy(dm_copy,dm_string);
  printf("Depends on the next test. ~v\n");

  printf("######### CMP EQ #########\n");
  if (dm_str_cmp(dm_string,ct_str) == 0) printf("Success!\n");
  else                                   printf("Failure!\n");

  printf("### FREE TEST NOT EMPTY ###\n");
  dm_str_free(dm_string);
  dm_str_free(dm_copy);
  if (dm_string->array  == NULL &&
      dm_copy->array    == NULL &&

      dm_string->length == 0    &&
      dm_copy->length   == 0    &&

      dm_string->size   == 0    &&
      dm_copy->size     == 0      ) printf("Success!\n");
  else                              printf("Failure!\n");

  printf("###### ADD STR EMPTY ######\n");
  dm_str_add_str(dm_string,"Boh si to ziada!");
  if (strcmp(dm_string->array,"Boh si to ziada!") == 0) printf("Success!\n");
  else                                                  printf("Failure!\n");

  dm_str_free(dm_string);

  /*--------------------------------------------------------------------------*/
  /*----------------------------- ADVANCED TESTS -----------------------------*/
  /*--------------------------------------------------------------------------*/

  printf("_____________________________\n");
  printf("000000 ADVANCED TESTS 000000\n");

  printf("######## ADD '\\0' ########\n");
  dm_str_add_c(dm_string,'\0');
  if (strcmp(dm_string->array,"") == 0) printf("Success!\n");
  else                                  printf("Failure!\n");

  printf("###### ADD LONG STR ######\n");
  dm_str_add_str(dm_string,
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur nec mauris eget nibh vestibulum dictum vitae at ante. Aenean non pellentesque velit. Donec nec iaculis leo, ut vulputate lacus. Integer sit amet lorem vel nibh bibendum vestibulum sed id tellus. Sed rhoncus, quam non pellentesque fringilla, sapien elit vehicula lacus, vitae ultricies magna orci eleifend arcu. Morbi nulla quam, blandit id ligula id, molestie dignissim nisl. Maecenas luctus non nisl a bibendum. Cras quis elit posuere metus dictum porttitor. Nulla congue lectus eget purus rhoncus, sed porta odio pulvinar. Donec a magna non ipsum ultricies fringilla. Vivamus dolor nulla, egestas ut porta id, consequat sed nulla. Vestibulum ullamcorper, purus efficitur mollis varius, risus eros aliquet mauris, vel fermentum magna libero in magna. Vestibulum enim libero, efficitur vitae pulvinar quis, porttitor ut mi. Aenean elementum egestas suscipit. Nam egestas ornare consectetur. Integer vestibulum commodo ex in lacinia. Suspendisse eget ornare magna. Vestibulum rhoncus enim semper ipsum dapibus lacinia. Nam vitae semper nisl, non venenatis nibh. Suspendisse felis nisl, consequat ut nibh et, accumsan pretium metus. Nam quis lacus turpis. Aenean non lacus tristique, ultricies est eget, posuere diam. Sed in turpis et enim ornare feugiat ut a tellus. Duis volutpat ligula volutpat, lacinia tellus sed, fermentum metus. Nam ullamcorper pulvinar est, et tincidunt neque aliquam eu. Ut malesuada egestas blandit. Praesent arcu neque, fermentum in viverra nec, condimentum sit amet lacus. Mauris id elit nisl. Proin imperdiet hendrerit tellus in fringilla. Donec id elit non nisl vulputate mollis. Mauris eu blandit diam, elementum placerat velit. Aliquam interdum, odio vel eleifend congue, tortor dolor luctus metus, eget eleifend sem leo elementum metus. Integer orci enim, pretium quis mi ut, sodales varius nisi. Ut vestibulum lorem magna, vel semper libero rutrum eu. Fusce vestibulum lacus arcu, porta porttitor risus hendrerit vitae. Nullam a luctus velit. Vivamus viverra, sem ut aliquet consequat, justo risus volutpat libero, eget scelerisque orci arcu vel metus. Vestibulum sodales elit ut ultrices porttitor. Suspendisse gravida nibh ut urna vulputate, eu finibus nisl bibendum. Curabitur erat turpis, eleifend tempor faucibus eget, consequat quis odio. Quisque facilisis blandit enim, et semper libero feugiat sed. Cras sed congue dui, quis euismod sapien. Integer finibus efficitur urna, ut ornare neque efficitur non. Curabitur aliquet lobortis metus et iaculis. Vestibulum ullamcorper consectetur nibh nec elementum. Nunc faucibus, neque sed dapibus vulputate, justo nibh tempor odio, vitae blandit leo tortor non lacus. Nam mollis mauris sed est commodo sodales. In vel egestas tellus. Sed ut commodo metus. Maecenas nec quam sed nunc cursus molestie id a diam. Cras tincidunt, lacus sed malesuada pharetra, ipsum augue pretium odio, elementum tempor nunc tellus ac felis. Pellentesque mollis diam vitae massa scelerisque mollis. Etiam id nibh velit. Maecenas eu dignissim purus. Suspendisse sed maximus libero. Praesent sed nulla in sem rutrum ultrices. Nulla porttitor id odio sed rutrum. Nunc venenatis, ex eu ornare fringilla, ligula ipsum dignissim orci, eget sodales augue justo ac enim. Fusce sit amet rutrum sem, eu mollis sapien. Nunc pharetra leo velit, sodales hendrerit ligula pellentesque vel. Suspendisse sodales, ante a suscipit rutrum, ante justo blandit mi, dignissim congue libero justo et enim. Ut ante lacus, malesuada vel gravida porttitor, viverra mattis urna. Duis lacinia imperdiet sem, sed fringilla velit tempus ac. Nunc diam erat, consectetur ac malesuada vel, eleifend sit amet nunc. Maecenas euismod, justo eu rutrum convallis, ligula sapien lacinia mi, a consectetur nisi odio eu lorem. Etiam cursus neque quam, eget auctor ex eleifend quis. Curabitur erat elit, maximus quis rutrum vitae, pretium at lorem. Nulla egestas diam at quam rhoncus sollicitudin. Nunc at elementum massa. Donec eget porta libero, nec molestie dui. Nullam quis sapien purus. Phasellus eu lorem dui. Vivamus varius ultricies commodo. Nunc eu luctus enim. Cras elementum felis nec turpis sagittis gravida. Integer nec ullamcorper enim. Morbi facilisis posuere lorem, ut accumsan felis vestibulum nec. Aliquam ut tellus vel massa feugiat porttitor a ac tellus. Curabitur posuere consectetur nisi, volutpat pretium dui auctor ullamcorper. Vivamus at mauris eget ex egestas rutrum. Ut et leo fringilla, auctor mauris sed, gravida nibh. Phasellus fringilla, ipsum sit amet vestibulum molestie, velit urna feugiat ex, sit amet maximus nibh dui in sem. Sed ornare massa et lorem molestie tempus. In risus nisi, dapibus luctus viverra et, euismod vitae nibh. Cras pretium ut erat vehicula mattis. Integer mi nisl, tempor quis mattis id, facilisis et nulla. Nunc sit amet lacinia ligula. Maecenas varius aliquet mauris vitae dapibus. Nulla volutpat efficitur leo, nec imperdiet ipsum fringilla laoreet. Donec a est eget leo congue sodales vitae eget dolor. Quisque vitae aliquam ante. Proin pulvinar, ipsum vitae sagittis efficitur, ligula purus scelerisque sapien, vitae lacinia tellus est id orci. Fusce sed velit ut sapien mollis sagittis vitae eu ipsum. Nullam eu aliquet lorem, sed ornare quam. Aenean est tellus, dictum at elit nec, bibendum pellentesque felis. Pellentesque mollis dui a urna egestas, a tincidunt turpis commodo. Nulla rhoncus volutpat velit vel efficitur. Nulla facilisi. Sed hendrerit tristique lectus non iaculis. Etiam a tellus blandit, ultrices felis ac, tincidunt sapien. Maecenas sagittis nec enim aliquet commodo. Pellentesque cursus nulla non eleifend feugiat. Curabitur mattis convallis turpis, nec eleifend urna ornare sed. Nam eu tristique mauris. Donec tempor facilisis lacus, ac vulputate magna bibendum quis. Praesent maximus, orci congue commodo commodo, sapien nunc porttitor odio, eget ullamcorper ante dolor id diam. Cras vel lacus imperdiet, ultricies augue in, elementum tortor. Vivamus at nunc et risus convallis maximus id eu tellus. Vivamus eu suscipit lorem, ac porta purus. Suspendisse facilisis porta ipsum, quis efficitur leo posuere non. Vivamus vehicula sapien est, ac auctor velit mollis et. Curabitur vel ligula venenatis, mattis elit non, vulputate magna. Sed vestibulum nibh urna, nec consectetur augue varius eu. Maecenas tristique lacus libero, in dignissim augue sodales a. Maecenas rhoncus dolor sapien, at viverra justo accumsan blandit. Nunc leo diam, cursus eu condimentum vitae, bibendum in ipsum. Fusce rhoncus et ipsum et varius. Donec at enim quis risus efficitur vulputate. In feugiat eros orci, eget rutrum neque tristique id. Nullam ac porttitor augue."
  );

  if (strcmp(dm_string->array,
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur nec mauris eget nibh vestibulum dictum vitae at ante. Aenean non pellentesque velit. Donec nec iaculis leo, ut vulputate lacus. Integer sit amet lorem vel nibh bibendum vestibulum sed id tellus. Sed rhoncus, quam non pellentesque fringilla, sapien elit vehicula lacus, vitae ultricies magna orci eleifend arcu. Morbi nulla quam, blandit id ligula id, molestie dignissim nisl. Maecenas luctus non nisl a bibendum. Cras quis elit posuere metus dictum porttitor. Nulla congue lectus eget purus rhoncus, sed porta odio pulvinar. Donec a magna non ipsum ultricies fringilla. Vivamus dolor nulla, egestas ut porta id, consequat sed nulla. Vestibulum ullamcorper, purus efficitur mollis varius, risus eros aliquet mauris, vel fermentum magna libero in magna. Vestibulum enim libero, efficitur vitae pulvinar quis, porttitor ut mi. Aenean elementum egestas suscipit. Nam egestas ornare consectetur. Integer vestibulum commodo ex in lacinia. Suspendisse eget ornare magna. Vestibulum rhoncus enim semper ipsum dapibus lacinia. Nam vitae semper nisl, non venenatis nibh. Suspendisse felis nisl, consequat ut nibh et, accumsan pretium metus. Nam quis lacus turpis. Aenean non lacus tristique, ultricies est eget, posuere diam. Sed in turpis et enim ornare feugiat ut a tellus. Duis volutpat ligula volutpat, lacinia tellus sed, fermentum metus. Nam ullamcorper pulvinar est, et tincidunt neque aliquam eu. Ut malesuada egestas blandit. Praesent arcu neque, fermentum in viverra nec, condimentum sit amet lacus. Mauris id elit nisl. Proin imperdiet hendrerit tellus in fringilla. Donec id elit non nisl vulputate mollis. Mauris eu blandit diam, elementum placerat velit. Aliquam interdum, odio vel eleifend congue, tortor dolor luctus metus, eget eleifend sem leo elementum metus. Integer orci enim, pretium quis mi ut, sodales varius nisi. Ut vestibulum lorem magna, vel semper libero rutrum eu. Fusce vestibulum lacus arcu, porta porttitor risus hendrerit vitae. Nullam a luctus velit. Vivamus viverra, sem ut aliquet consequat, justo risus volutpat libero, eget scelerisque orci arcu vel metus. Vestibulum sodales elit ut ultrices porttitor. Suspendisse gravida nibh ut urna vulputate, eu finibus nisl bibendum. Curabitur erat turpis, eleifend tempor faucibus eget, consequat quis odio. Quisque facilisis blandit enim, et semper libero feugiat sed. Cras sed congue dui, quis euismod sapien. Integer finibus efficitur urna, ut ornare neque efficitur non. Curabitur aliquet lobortis metus et iaculis. Vestibulum ullamcorper consectetur nibh nec elementum. Nunc faucibus, neque sed dapibus vulputate, justo nibh tempor odio, vitae blandit leo tortor non lacus. Nam mollis mauris sed est commodo sodales. In vel egestas tellus. Sed ut commodo metus. Maecenas nec quam sed nunc cursus molestie id a diam. Cras tincidunt, lacus sed malesuada pharetra, ipsum augue pretium odio, elementum tempor nunc tellus ac felis. Pellentesque mollis diam vitae massa scelerisque mollis. Etiam id nibh velit. Maecenas eu dignissim purus. Suspendisse sed maximus libero. Praesent sed nulla in sem rutrum ultrices. Nulla porttitor id odio sed rutrum. Nunc venenatis, ex eu ornare fringilla, ligula ipsum dignissim orci, eget sodales augue justo ac enim. Fusce sit amet rutrum sem, eu mollis sapien. Nunc pharetra leo velit, sodales hendrerit ligula pellentesque vel. Suspendisse sodales, ante a suscipit rutrum, ante justo blandit mi, dignissim congue libero justo et enim. Ut ante lacus, malesuada vel gravida porttitor, viverra mattis urna. Duis lacinia imperdiet sem, sed fringilla velit tempus ac. Nunc diam erat, consectetur ac malesuada vel, eleifend sit amet nunc. Maecenas euismod, justo eu rutrum convallis, ligula sapien lacinia mi, a consectetur nisi odio eu lorem. Etiam cursus neque quam, eget auctor ex eleifend quis. Curabitur erat elit, maximus quis rutrum vitae, pretium at lorem. Nulla egestas diam at quam rhoncus sollicitudin. Nunc at elementum massa. Donec eget porta libero, nec molestie dui. Nullam quis sapien purus. Phasellus eu lorem dui. Vivamus varius ultricies commodo. Nunc eu luctus enim. Cras elementum felis nec turpis sagittis gravida. Integer nec ullamcorper enim. Morbi facilisis posuere lorem, ut accumsan felis vestibulum nec. Aliquam ut tellus vel massa feugiat porttitor a ac tellus. Curabitur posuere consectetur nisi, volutpat pretium dui auctor ullamcorper. Vivamus at mauris eget ex egestas rutrum. Ut et leo fringilla, auctor mauris sed, gravida nibh. Phasellus fringilla, ipsum sit amet vestibulum molestie, velit urna feugiat ex, sit amet maximus nibh dui in sem. Sed ornare massa et lorem molestie tempus. In risus nisi, dapibus luctus viverra et, euismod vitae nibh. Cras pretium ut erat vehicula mattis. Integer mi nisl, tempor quis mattis id, facilisis et nulla. Nunc sit amet lacinia ligula. Maecenas varius aliquet mauris vitae dapibus. Nulla volutpat efficitur leo, nec imperdiet ipsum fringilla laoreet. Donec a est eget leo congue sodales vitae eget dolor. Quisque vitae aliquam ante. Proin pulvinar, ipsum vitae sagittis efficitur, ligula purus scelerisque sapien, vitae lacinia tellus est id orci. Fusce sed velit ut sapien mollis sagittis vitae eu ipsum. Nullam eu aliquet lorem, sed ornare quam. Aenean est tellus, dictum at elit nec, bibendum pellentesque felis. Pellentesque mollis dui a urna egestas, a tincidunt turpis commodo. Nulla rhoncus volutpat velit vel efficitur. Nulla facilisi. Sed hendrerit tristique lectus non iaculis. Etiam a tellus blandit, ultrices felis ac, tincidunt sapien. Maecenas sagittis nec enim aliquet commodo. Pellentesque cursus nulla non eleifend feugiat. Curabitur mattis convallis turpis, nec eleifend urna ornare sed. Nam eu tristique mauris. Donec tempor facilisis lacus, ac vulputate magna bibendum quis. Praesent maximus, orci congue commodo commodo, sapien nunc porttitor odio, eget ullamcorper ante dolor id diam. Cras vel lacus imperdiet, ultricies augue in, elementum tortor. Vivamus at nunc et risus convallis maximus id eu tellus. Vivamus eu suscipit lorem, ac porta purus. Suspendisse facilisis porta ipsum, quis efficitur leo posuere non. Vivamus vehicula sapien est, ac auctor velit mollis et. Curabitur vel ligula venenatis, mattis elit non, vulputate magna. Sed vestibulum nibh urna, nec consectetur augue varius eu. Maecenas tristique lacus libero, in dignissim augue sodales a. Maecenas rhoncus dolor sapien, at viverra justo accumsan blandit. Nunc leo diam, cursus eu condimentum vitae, bibendum in ipsum. Fusce rhoncus et ipsum et varius. Donec at enim quis risus efficitur vulputate. In feugiat eros orci, eget rutrum neque tristique id. Nullam ac porttitor augue."
  ) == 0) printf("Success!\n");
  else    printf("Failure!\n");

  dm_str_free(dm_string);

  free(dm_string);
  free(dm_copy);

  //////////////////////////////////////////////////////////////////////////////
  //////////////////////* ##### FREE TEST VALGRIND ##### *//////////////////////
  //////////////////////////////////////////////////////////////////////////////

  return 0;
}

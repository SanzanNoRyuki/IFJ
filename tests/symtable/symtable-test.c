#include "../../symtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void Print_tree2(BTNodePtr TempTree, char* sufix, char fromdir)
/* vykresli sktrukturu binarniho stromu */

{
     if (TempTree != NULL)
     {
	char* suf2 = (char*) malloc(strlen(sufix) + 4);
	strcpy(suf2, sufix);
        if (fromdir == 'L')
	{
	   suf2 = strcat(suf2, "  |");
	   printf("%s\n", suf2);
	}
	else
	   suf2 = strcat(suf2, "   ");
	Print_tree2(TempTree->RPtr, suf2, 'R');
        printf("%s  +-[%s,%d,%d]\n", sufix, TempTree->name, TempTree->data->dat.data_var.int_val, TempTree->data->data_type);
	strcpy(suf2, sufix);
        if (fromdir == 'R')
	   suf2 = strcat(suf2, "  |");	
	else
	   suf2 = strcat(suf2, "   ");
	Print_tree2(TempTree->LPtr, suf2, 'L');
	if (fromdir == 'R') printf("%s\n", suf2);
	free(suf2);
    }
}

void Print_tree(BTNodePtr TempTree)
{
  printf("Struktura binarniho stromu:\n");
  printf("\n");
  if (TempTree != NULL)
     Print_tree2(TempTree, "", 'X');
  else
     printf("strom je prazdny\n");
  printf("\n");
  printf("=================================================\n");
}


int main(){
	
	BTNodePtr Tree;
	BTNodePtr Tree_result;
	char *name;

	/********TEST 1********/
	printf("TEST 1\n");
	printf("Inicializacia stromu\n");
	BTNode_init(&Tree);
	Print_tree(Tree);

	/********TEST 2********/
	printf("************************\n");
	printf("TEST 2\n");
	printf("Search in the empty tree\n");
	name = "50";
	Tree_result = BTNode_search(&Tree, name);
	if(Tree_result == NULL)
		printf("Node was not found!\n");
	else
		printf("Node was found! name = %s, int_val = %d\n", Tree_result->name, Tree_result->data->dat.data_var.int_val); 
	printf("\n");

	/*********TEST 3*******/
	printf("************************\n");
	printf("TEST 3\n");
	printf("Insert\n");
	
	name = "50";
	BTData *data1 = (BTData *) calloc(1, sizeof(BTData));
	if(data1 == NULL)
		fprintf(stderr, "Allocating FAILed!");
	data1->dat.data_var.int_val = 1;
	data1->data_type = type_int;
	BTNode_insert(&Tree, name, data1);
	if(Tree->data->data_type != type_int)
		printf("Insert data->data_type failed.\n");
	else
		printf("Insert data->data_type succeded.\n");
	if(Tree->data->dat.data_var.int_val != 1)
		printf("Insert data.dat.data_var failed.\n");
	else
		printf("Insert data.dat.data_var succeded.\n");


	name = "45";
	BTData *data2 = (BTData *) calloc(1, sizeof(BTData));
        if(data2 == NULL)
                 fprintf(stderr, "Allocating FAILed!");
        data2->data_type = type_float;
	data2->dat.data_var.int_val = 2;
        BTNode_insert(&Tree, name, data2);
	if(Tree->data->data_type != type_float)
                printf("Insert data->data_type failed.\n");
        else
                printf("Insert data->data_type succeded.\n");
        if(Tree->data->dat.data_var.int_val != 2)
                printf("Insert data.dat.data_var failed.\n");
        else
                printf("Insert data.dat.data_var succeded.\n");

	name = "55";
	BTData *data3 = (BTData *) calloc(1, sizeof(BTData));
        if(data3 == NULL)
                 fprintf(stderr, "Allocating FAILed!");
	data3->dat.data_var.int_val = 3;
        data3->data_type = type_float;
        BTNode_insert(&Tree, name, data3);
	if(Tree->data->data_type != type_float)
                printf("Insert data->data_type failed.\n");
        else
                printf("Insert data->data_type succeded.\n");
        if(Tree->data->dat.data_var.int_val != 3)
                printf("Insert data.dat.data_var failed.\n");
        else
                printf("Insert data.dat.data_var succeded.\n");

	name = "40";
	BTData *data4 = (BTData *) calloc(1, sizeof(BTData));
        if(data4 == NULL)
                 fprintf(stderr, "Allocating FAILed!");
	data4->dat.data_var.int_val = 4;
        data4->data_type = type_string;
        BTNode_insert(&Tree, name, data4);
	if(Tree->data->data_type != type_string)
                printf("Insert data->data_type failed.\n");
        else
                printf("Insert data->data_type succeded.\n");
        if(Tree->data->dat.data_var.int_val != 4)
                printf("Insert data.dat.data_var failed.\n");
        else
                printf("Insert data.dat.data_var succeded.\n");

	name = "35";
	BTData *data5 = (BTData *) calloc(1, sizeof(BTData));
        if(data5 == NULL)
                 fprintf(stderr, "Allocating FAILed!");
	data5->dat.data_var.int_val = 5;
	data5->data_type = type_int;
	BTNode_insert(&Tree, name, data5);
	if(Tree->data->data_type != type_int)
                printf("Insert data->data_type failed.\n");
        else
                printf("Insert data->data_type succeded.\n");
        if(Tree->data->dat.data_var.int_val != 5)
                printf("Insert data.dat.data_var failed.\n");
        else
                printf("Insert data.dat.data_var succeded.\n");

	name = "43";
	BTData *data6 = (BTData *) calloc(1, sizeof(BTData));
        if(data6 == NULL)
                 fprintf(stderr, "Allocating FAILed!");
	data6->dat.data_var.int_val = 6;
	data6->data_type = type_none;
	BTNode_insert(&Tree, name, data6);
	if(Tree->data->data_type != type_none)
        	printf("Insert data->data_type failed\n");
	else
                printf("Insert data->data_type succeded.\n");
        if(Tree->data->dat.data_var.int_val != 6)
                printf("Insert data.dat.data_var failed.\n");
        else
                printf("Insert data.dat.data_var succeded.\n");

	name = "47";
	BTData *data7 = (BTData *) calloc(1, sizeof(BTData));
        if(data7 == NULL)
                 fprintf(stderr, "Allocating FAILed!");
	data7->dat.data_var.int_val = 7;
        data7->data_type = type_multilinestr;
        BTNode_insert(&Tree, name, data7);
	if(Tree->data->data_type != type_multilinestr)
                printf("Insert data->data_type failed.\n");
        else
                printf("Insert data->data_type succeded.\n");
        if(Tree->data->dat.data_var.int_val != 7)
                printf("Insert data.dat.data_var failed.\n");
        else
                printf("Insert data.dat.data_var succeded.\n");

	name = "53";
	BTData *data11 = (BTData *) calloc(1, sizeof(BTData));
        if(data7 == NULL)
                 fprintf(stderr, "Allocating FAILed!");

	data11->dat.data_var.int_val = 8;
        data11->data_type = type_string;
        BTNode_insert(&Tree, name, data11);
	if(Tree->data->data_type != type_string)
                printf("Insert data->data_type failed.\n");
        else
                printf("Insert data->data_type succeded.\n");
        if(Tree->data->dat.data_var.int_val != 8)
                printf("Insert data.dat.data_var failed.\n");
        else
                printf("Insert data.dat.data_var succeded.\n");

	name = "60";
	BTData *data8 = (BTData *) calloc(1, sizeof(BTData));
        if(data8 == NULL)
                 fprintf(stderr, "Allocating FAILed!");
	data8->dat.data_var.int_val = 9;
        data8->data_type = type_multilinestr;
        BTNode_insert(&Tree, name, data8);
	if(Tree->data->data_type != type_multilinestr)
        	printf("Insert data->data)type failed.\n");
        else
                printf("Insert data->data_type succeded.\n");
        if(Tree->data->dat.data_var.int_val != 9)
                printf("Insert data.dat.data_var failed.\n");
        else
                printf("Insert data.dat.data_var succeded.\n");

	name = "57";
	BTData *data9 = (BTData *) calloc(1, sizeof(BTData));
        if(data9 == NULL)
                 fprintf(stderr, "Allocating FAILed!");
	data9->dat.data_var.int_val = 10;
        data9->data_type = type_int;
        BTNode_insert(&Tree, name, data9);
	if(Tree->data->data_type != type_int)
                printf("Insert data->data_type failed.\n");
        else
                printf("Insert data->data_type succeded.\n");
        if(Tree->data->dat.data_var.int_val != 10)
                printf("Insert data.dat.data_var failed.\n");
        else
                printf("Insert data.dat.data_var succeded.\n");

	name = "65";
	BTData *data10 = (BTData *) calloc(1, sizeof(BTData));
        if(data10 == NULL)
                 fprintf(stderr, "Allocating FAILed!");
	data10->dat.data_var.int_val = 11;
        data10->data_type = type_none;
        BTNode_insert(&Tree, name, data10);
	if(Tree->data->data_type != type_none)
                printf("Insert data->data_type failed.\n");
        else
                printf("Insert data->data_type succeded.\n");
        if(Tree->data->dat.data_var.int_val != 11)
                printf("Insert data.dat.data_var failed.\n");
        else
                printf("Insert data.dat.data_var succeded.\n");

	Print_tree(Tree);

	printf("\n");
	/*******TEST 4********/
	printf("*************************\n");
	printf("TEST 4\n");
        printf("Search\n");
	printf("Searching 50 - should be found\n");
        name = "50";
        Tree_result = BTNode_search(&Tree, name);
        if(Tree_result == NULL)
                printf("Node was not found!\n");
        else
                printf("Node was found! name = %s, int_val = %d\n", Tree_result->name, Tree_result->data->dat.data_var.int_val);
	
	printf("Searching 43 - should be found\n");
	name = "43";
        Tree_result = BTNode_search(&Tree, name);
        if(Tree_result == NULL)
                printf("Node was not found!\n");
        else
                printf("Node was found! name = %s, int_val = %d\n", Tree_result->name, Tree_result->data->dat.data_var.int_val);
	
	printf("Searching 100 - should not be found\n");
	name = "100";
        Tree_result = BTNode_search(&Tree, name);
        if(Tree_result == NULL)
                printf("Node was not found!\n");
        else
                printf("Node was found! name = %s, int_val = %d\n", Tree_result->name, Tree_result->data->dat.data_var.int_val);
	
	printf("Searching 91 - should not be found\n");
	name = "91";
	Tree_result = BTNode_search(&Tree, name);
        if(Tree_result == NULL)
                printf("Node was not found!\n");
        else
                printf("Node was found! name = %s, int_val = %d\n", Tree_result->name, Tree_result->data->dat.data_var.int_val);

	
	printf("\n");
	/****** TEST 5******/
	printf("*******************************\n");
	printf("TEST 5\n");
	printf("Insert - to override\n");
	
	printf("Inserting with name 50, int_val 20, data_type none\n");
	name = "50";
	BTData *data12 = (BTData *) calloc(1, sizeof(BTData));
         if(data12 == NULL)
                 fprintf(stderr, "Allocating FAILed!");
        data12->dat.data_var.int_val = 20;
        data12->data_type = type_none;
        BTNode_insert(&Tree, name, data12);
        if(Tree->data->data_type != type_none)
                printf("Insert data->data_type failed.\n");
        else
                printf("Insert data->data_type succeded.\n");
        if(Tree->data->dat.data_var.int_val != 20)
                printf("Insert data.dat.data_var failed.\n");
        else
                printf("Insert data.dat.data_var succeded.\n");
	Print_tree(Tree);
	printf("\n");
	

	/******* TEST 6 ********/
	printf("***********************************\n");
	printf("TEST 6\n");
	printf("Does insert change every node data?\n");
	name = "50";
	Tree_result = BTNode_search(&Tree, name);
	if(Tree_result->data->dat.data_var.int_val != 20)
	{	
		printf("Searching name 50, should have int_val 20 but has %d.\n", Tree_result->data->dat.data_var.int_val);
	}
	else
	{
		printf("Searching name 50, should have int_val 20, has %d\n", Tree_result->data->dat.data_var.int_val);
	}

	name = "53";
	Tree_result = BTNode_search(&Tree, name);
	if(Tree_result->data->dat.data_var.int_val != 8)
	{
		printf("Searching name 53, should have int_val 8 but has %d.\n", Tree_result->data->dat.data_var.int_val);
	}
	else
	{
	        printf("Searching name 53, should have int_val 8, has %d\n", Tree_result->data->dat.data_var.int_val);
	}
	
	
	/******** TEST 7 *********/
	printf("\n TEST 7 \n");
	printf("Delete function.Deleting 50\n");
	BTNode_delete(&Tree, "50");
	Print_tree(Tree);
	printf("Deleting 57 and 43\n");
	BTNode_delete(&Tree, "57");
	BTNode_delete(&Tree, "43");
	Print_tree(Tree);
	printf("Deleting 45.\n");
	BTNode_delete(&Tree, "45");
	Print_tree(Tree);


	/********* TEST 7 *********/
	printf("**********************\n");
	printf("TEST 7\n");
	printf("Dispose tree\n");
	BTNode_delete_all(&Tree);
	Print_tree(Tree);
	
	/********* TEST 8 **********/
	printf("***************\n");
	printf("TEST 8\n");
	printf("Testing more different data.\n");
		
	BTData *nodeptr1 = (BTData *) calloc(1, sizeof(BTData));
	if(nodeptr1 == NULL)
              fprintf(stderr, "Allocating FAILed!");
	/* Assigning data */
	name = "M";
	nodeptr1->id_type = var_id;
	nodeptr1->data_type = type_int;
	nodeptr1->dat.data_func.defined = true;
	nodeptr1->dat.data_func.count_of_params = 5;
	BTNode_insert(&Tree, name, nodeptr1);	
	/* Testing */
	if(nodeptr1->id_type != var_id)
		printf("nodeptr1 id_type failed!\n");
	if(nodeptr1->data_type != type_int)
		printf("nodeptr1 data_type failed!\n");	
	if(nodeptr1->dat.data_func.defined != true)
		printf("nodeptr1 defined failed!\n");
	if(nodeptr1->dat.data_func.count_of_params != 5)
		printf("nodeptr1 count of params failed!\n");
	Print_tree(Tree);

	printf("som tu\n");
	
	/* Second node */
	BTData *nodeptr2 = (BTData *) calloc(1, sizeof(BTData));
	if(nodeptr2 == NULL)
              fprintf(stderr, "Allocating FAILed!");
	/* Assigning data */
	name = "N";
	nodeptr2->id_type = func_id;
	nodeptr2->data_type = type_float;
	nodeptr2->dat.data_var.str = "string";
	nodeptr2->dat.data_var.flt_val = 5.5;
	nodeptr2->dat.data_var.int_val = 42;
	printf("tuas\n");
	BTNode_insert(&Tree, name, nodeptr2);	
	/* Testing */
	printf("testtu\n");
	if(nodeptr2->id_type != func_id)
		printf("nodeptr2 id_type failed!\n");
	printf("1tst\n");
	if(nodeptr2->data_type != type_float)
		printf("nodeptr2 data_type failed!\n");	
	printf("3test\n");
	if(nodeptr2->dat.data_var.flt_val != 5.5)
		printf("nodeptr2 flt_val failed!\n");
	printf("4test\n");
	if(nodeptr2->dat.data_var.int_val != 42)
		printf("nodeptr2 int_val failed!\n");

		
}


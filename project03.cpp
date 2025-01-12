#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/socket.h>
#define SOCKETNAME "MySocket"
#define NUM_CLIENTS 5
#define NUM_PRODUCTS 20
#define MAX_ORDERS 10
#define <fuctionsbodies03.cpp>
#define <functionsdeclarations03.cpp>

typedef struct products{
	char description[50];
	int price;
	int item_count;
}products;

products catalog[NUM_PRODUCTS];


int main(void){
	initialize_catalog();
	initialize_variables();
	client_working();
	server_working();
	return 0;
	
}

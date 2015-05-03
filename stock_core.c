#include <stdio.h>
#include "curl/curl.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "stock_core.h"

/********************************************************************
 * Function:
 *  		f_concat
 * Description:
 *		contatinates two character strings
 * Formula:
 *              -
 * Parameter:
 *             	1.)	Pointer String 1
 *             	2.)	Pointer String 2
 * Return Value:
 *		Returns new String Pointer to concatinated value of P1 and P2	
 *
 *******************************************************************/
char* f_concat(const char* str1, const char* str2)
{
    	char* result;
    	asprintf(&result, "%s%s", str1, str2);
    	return result;
}

/********************************************************************
 * Function:
 * 		f_get_stock_price 
 * Description:
 *		Executes Curl on Commandline to get Price for
 *		specified Stock
 * Formula:
 *              -
 * Parameter:
 *              1.)    Pointer to Structure of Type Stock 
 * Return Value:
 *              0       Success
 *              1       Error
 *
 *******************************************************************/
int f_get_stock_price(struct stock *stk){

	char *stock_url_string;
	FILE *fp;
	char result_string[1024];
	int  c;

	c = 0;

	stock_url_string = f_concat(STOCK_EXCHANGE_URL, "aktien/");
	stock_url_string = f_concat(stock_url_string, stk->name);
	stock_url_string = f_concat("/usr/bin/curl -sS ", stock_url_string);
	stock_url_string = f_concat(stock_url_string, "|grep 'var prev'");
	stock_url_string = f_concat(stock_url_string, "|cut -d'=' -f2");
	stock_url_string = f_concat(stock_url_string, "|cut -d' ' -f2");
	stock_url_string = f_concat(stock_url_string, "|cut -d';' -f1");

/*	printf("%s\n",stock_url_string);*/
	

	fp = popen(stock_url_string, "r");
	
	if(fp == NULL) {
		printf("Failed to run curl command!\n");
		return 1;
	}

	while (fgets(result_string, sizeof(result_string)-1, fp) != NULL) {
    		if (c == 0) {
  			stk->new_bid_price = strtod(result_string,NULL);
		}
		if (c == 1){
  			stk->new_ask_price = strtod(result_string,NULL);
		}
		c++;
  	}

  	pclose(fp);
	free(stock_url_string);

	return 0;
}

/********************************************************************
 * Function:
 *              f_get_index_value 
 * Description:
 *              Executes Curl on Commandline to get Price for
 *              specified Index 
 * Formula:
 *              -
 * Parameter:
 *              1.)    Pointer to Structure of Type Index
 * Return Value:
 *              0       Success
 *              1       Error
 *
 *******************************************************************/
int f_get_index_value(struct index *idx){

        char *index_url_string;
        FILE *fp;
        char result_string[1024];
        int  c;

        c = 0;

        index_url_string = f_concat(STOCK_EXCHANGE_URL, "aktien/indezes/");
        index_url_string = f_concat(index_url_string, idx->name);
        index_url_string = f_concat("/usr/bin/curl -sS ", index_url_string);
        index_url_string = f_concat(index_url_string, "|grep --regexp=\"\\....\\,\"|grep -v span|grep -v google");
	index_url_string = f_concat(index_url_string, "|head -1|sed \"s/\\ //g\"|sed \"s/\\t//g\"");

        /*printf("%s\n",index_url_string);*/

        fp = popen(index_url_string, "r");

        if(fp == NULL) {
                printf("Failed to run curl command!\n");
                return 1;
        }

        while (fgets(result_string, sizeof(result_string)-1, fp) != NULL) {
                if (c == 0) {
                        idx->new_value = strtod(result_string,NULL);
                }
                c++;
        }

	printf("%f\n",idx->new_value);

        pclose(fp);
        free(index_url_string);

        return 0;
}

/********************************************************************
 * Function:
 *  		f_stock_price_update
 * Description:
 *		Updates the current Price of Stock with
 *		new Price if Price is not equal to 0
 * Formula:
 *              -
 * Parameter:
 *              1.)     Pointer to Structure of Type Stock
 * Return Value:
 *              0       Success
 *              1       Error
 *
 *******************************************************************/
int f_stock_price_update(struct stock *stk){

	if(stk->new_bid_price != 0)
	stk->current_bid_price = stk->new_bid_price;
	if(stk->new_ask_price != 0)
	stk->current_ask_price = stk->new_ask_price;

	return 0;
}

/********************************************************************
 * Function:
 *  		f_price_change
 * Description:
 *		Compares the to Input Values and returns
 *		specific value
 * Formula:
 *              -
 * Parameter:
 *              1.)     Input Double 
 *              2.)     Input Double
 * Return Value:
 *		STOCK_PRICE_EQ 	0	1 == 2
 *		STOCK_PRICE_LT 	1	1  > 2
 *		STOCK_PRICE_GT 	2	1  < 2
 *		default: 0
 *
 *******************************************************************/
int f_price_change(double x1, double x2){

	if(x1 == x2) return STOCK_PRICE_EQ;
	if(x1 < x2) return STOCK_PRICE_GT;
	if(x1 > x2) return STOCK_PRICE_LT;

	return 0;
}

/********************************************************************
 * Function:
 * 		f_print_stock 
 * Description:
 *		Prints the Values of the Struct Stock in a specified
 *		colored format
 * Formula:
 *              -
 * Parameter:
 *              1.)     Pointer to Structure to Type Stock
 * Return Value:
 *              0       Success
 *              1       Error
 *
 *******************************************************************/
int f_print_stock(struct stock *stk){

	int rs;

	printf("%c[%d;%dm%-28s%c[%dm",27,0,33,stk->name,27,0);

	rs = f_price_change(stk->current_bid_price,stk->new_bid_price);

	if(rs == STOCK_PRICE_EQ) printf("%c[%d;%dm%07.3f%c[%dm",27,0,36,stk->new_bid_price,27,0);
	if(rs == STOCK_PRICE_LT) printf("%c[%d;%dm%07.3f%c[%dm",27,0,31,stk->new_bid_price,27,0);
	if(rs == STOCK_PRICE_GT) printf("%c[%d;%dm%07.3f%c[%dm",27,1,32,stk->new_bid_price,27,0);

	rs = f_price_change(stk->current_ask_price,stk->new_ask_price);

	printf(" / ");

	if(rs == STOCK_PRICE_EQ) printf("%c[%d;%dm%07.3f%c[%dm",27,0,36,stk->new_ask_price,27,0);
	if(rs == STOCK_PRICE_LT) printf("%c[%d;%dm%07.3f%c[%dm",27,1,31,stk->new_ask_price,27,0);
	if(rs == STOCK_PRICE_GT) printf("%c[%d;%dm%07.3f%c[%dm",27,0,32,stk->new_ask_price,27,0);

	printf("	|");

	return 0;
}

/********************************************************************
 * Function:
 *  		f_stock_refresh
 * Description:
 *		Calls the functions f_stock_price_update and
 *		f_get_stock_price
 * Formula:
 *              -
 * Parameter:
 *              1.)     Pointer to Structure of Type Stock
 * Return Value:
 *              0       Success
 *              1       Error
 *
 *******************************************************************/
int f_stock_refresh(struct stock *stk){
	
	f_stock_price_update(stk);
	f_get_stock_price(stk);

	return 0;
}

/********************************************************************
 * Function:
 *  		f_read_config_file
 * Description:
 *		Reads the passed configfile and copies the Stock
 *		Names to the Stock Structure Array
 * Formula:
 *              -
 * Parameter:
 *              1.)     FileName of Stocks Config File
 * Return Value:
 *          	Number of Elements in the File
 *
 *******************************************************************/
int f_read_config_file(char *filename){

	FILE *fp;
	char buff[255];
	int c;

	c = 0;

	fp = fopen(filename, "r");

	while(fgets(buff, 255, (FILE*)fp) != NULL){

		stks[c].name = malloc(sizeof(char)*(int)strlen(buff));

		memcpy ( stks[c].name, buff, strlen(buff)-1);

		c++;

	}

	fclose(fp);

	return c;
}

/********************************************************************
 * Function:
 *  		f_stock_thread_loop
 * Description:
 *		Endless Loop function which
 *		Queries Data for a single stock
 *		Should be execute by thread
 * Formula:
 *              -
 * Parameter:
 *              1.)    	Pointer to Structure of Type Stock 
 * Return Value:
 *
 *******************************************************************/
void *f_stock_thread_loop(void *ch){

	struct stock *stk;
	stk = (struct stock*)ch;

	while(1){
		f_stock_refresh(stk);
		sleep((rand() % DATA_MAX_REFRESH_TIME) + DATA_MIN_REFRESH_TIME);
	}
	
}

/********************************************************************
 * Function:
 *  		f_stock_thread_start
 * Description:
 *		Starts an thread with function f_stock_thread_loop for
 *		the passed Stock
 * Formula:
 *              -
 * Parameter:
 *              1.)     Pointer to Structure of Type Stock
 *              2.)     Index where the stock is saved in the Stocks array
 *			Should be same number as position in stks_threads array
 * Return Value:
 *              0       Success
 *              1       Error
 *
 *******************************************************************/
int f_stock_thread_start(struct stock *stk, int index){

	pthread_create(&stks_threads[index], NULL, f_stock_thread_loop, stk);

	return 0;
}

/********************************************************************
 * Function:
 *  		f_stock_thread_stop
 * Description:
 *		Stocks the thread on passed position in stks_threads array
 * Formula:
 *              -
 * Parameter:
 *              1.)     Index of Thread which should be Stopped
 * Return Value:
 *              0       Success
 *              1       Error
 *
 *******************************************************************/
int f_stock_thread_stop(int index){

	pthread_join(stks_threads[index], NULL);

	return 0;
}

int main(){

	struct index idx;
	int i;
	idx.name = "dax+DE0008469008";

	stock_cnt = f_read_config_file(STOCK_CONFIG_FILE);

	system("clear");

	for(i = 0; i < stock_cnt; i++){

		f_stock_thread_start(&stks[i],i);

	}

	while (1) {

	system("clear");

	for(i = 0; i < stock_cnt; i++){

		f_print_stock(&stks[i]);
		if ( (i % 3) == 0 && i > 0 ) printf("\n");

	}

	printf("\n");
	f_get_index_value(&idx);
	sleep(SCREEN_REFRESH_TIME);

	}

        for(i = 0; i < stock_cnt; i++){

                f_stock_thread_stop(i);

        }


	return 0;
}

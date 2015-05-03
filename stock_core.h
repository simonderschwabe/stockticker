#ifndef STOCK_CORE_H
#define STOCK_CORE_H

#define STOCK_EXCHANGE_URL "http://www.boerse-frankfurt.de/de/"
#define STOCK_CONFIG_FILE "aktien.txt"
#define INDEX_CONFIG_FILE "indezes.txt"
#define STOCK_PRICE_EQ 0
#define STOCK_PRICE_LT 1
#define STOCK_PRICE_GT 2
#define MAX_STOCKS 126
#define DATA_MIN_REFRESH_TIME 10
#define DATA_MAX_REFRESH_TIME 20
#define SCREEN_REFRESH_TIME 5

int stock_cnt;

struct stock {
        char *name;
        double current_bid_price;
        double current_ask_price;
        double new_bid_price;
        double new_ask_price;
};

struct index {
        char *name;
        double current_value;
        double new_value;
};

struct stock stks[MAX_STOCKS];
pthread_t stks_threads[MAX_STOCKS];

#endif

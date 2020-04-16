// PROJECT IDENTIFIER 0E04A31E0D60C01986ACB20081C9D8722A1899B6
//  market.h
//  Project 2
//
//  Created by Michelle Figueroa on 2/9/20.
//  Copyright © 2020 Michelle Figueroa. All rights reserved.
//

#ifndef market_h
#define market_h

#include <string>
#include <vector>
#include <queue>

    //a struct of orders
    class order {
    public:
        
        int place_in_line;
        size_t trader_id;
        int price;
        int quantity;
        int order_timestamp;
        size_t stock_id;
        std::string type;
        
    };
    //buying comparator
    struct buyComp {
        bool operator()(const order &l, const order &r) const {
            if (l.price != r.price) {
                return l.price < r.price;
            }
            else {
                return l.place_in_line > r.place_in_line;
            }
        }
    };

    struct sellComp {
        bool operator()(const order &l, const order &r) const {
            if (l.price != r.price) {
                return l.price > r.price;
            }
            else {
                return l.place_in_line > r.place_in_line;
            }
        }
    };

    // struct for time_travler, if given
    struct time_travler {
        bool had_sell = false;
        int buying_price = 0;
        int temp_buying_price = 0;
        int buying_time = 0;
        int temp_buying_time = 0;
        int selling_price = 0;
        int temp_selling_price = 0;
        int temp_selling_time = 0;
        int selling_time = 0;
    };
    // class stock
    class stock_class {
    public:
        
        //PQ's for actual exchanges
        std::priority_queue<order, std::vector<order>, buyComp>buy_PQ;
        std::priority_queue<order, std::vector<order>, sellComp>sell_PQ;
        
        //PQ's for median command
        std::priority_queue<int, std::vector<int>, std::greater<int>> top;
        std::priority_queue<int, std::vector<int>, std::less<int>> bottom;
        time_travler explore;
        
        /*trying somehthing, cerasing the median class and
         putting it in the stocks class to make function acess easier */
        
        // "start" of median
        void find_median(size_t  stock, int price);
        size_t print_median(size_t stock);
        void print_median_pt2(size_t stock_num, int time);
    };


    // struct for trade_list mode
    struct trader_info {
    public:
        size_t trader_id;
        size_t amount_bought;
        size_t amount_sold;
        int net;
    };

class Market {
    public:

        bool verbose = false;
        bool median = false;
        bool trader_info = false;
        bool time_travel = false;
        unsigned int num_traders;
        unsigned int num_stocks;
        std::string mode;
        bool buy_order = false;
        bool sell_order = false;
    
        // variables for pseudorandom input
        unsigned int random_seed;
        unsigned int num_orders;
        unsigned int arrival_rate;
    
    
        // command line function
        void get_options(int argc, char* argv[]);
        // reading in inital unput
        void read_inital_input();
    
        // main function, reading in orders
        void read_all_input();
    
        // function for verbose
        void verbose_function(size_t stock_id, size_t seller_id, size_t buyer_id, int quan, int price);
    
    
        // output for trader_info
        void trader_output();
    
        // function for end report
        void end_report();
    
        //find travelers
        void find_traveler(order &x);
    
        // print out time traveler if given
        void time_traveler_report();
    
    

};
#endif /* market_h */

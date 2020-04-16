// PROJECT IDENTIFIER 0E04A31E0D60C01986ACB20081C9D8722A1899B6
//  market.cpp
//  Project 2
//
//  Created by Michelle Figueroa on 2/9/20.
//  Copyright © 2020 Michelle Figueroa. All rights reserved.
//

#include <stdio.h>
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include "market.h"
#include "P2random.h"
#include "xcode_redirect.hpp"

using namespace std;

vector<stock_class>stocks;
vector<trader_info>traders;
//vector<time_traveler>time_travel_vec;
vector<stock_class>stock_median;
int num_transactions = 0;

int main (int argc, char* argv[]) {
    
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    Market m1;
    m1.get_options(argc, argv);
    m1.read_inital_input();
    m1.read_all_input();
    m1.end_report();
    m1.trader_output();
    m1.time_traveler_report();
    
    return 0;
    
    
    
}

//command line function
void Market::get_options(int argc, char* argv[]) {
    int option_index = 0, option = 0;
    
    // Don't display getopt error messages about options
    opterr = false;
    
    // use getopt to find command line options
    struct option longOpts[] = {
        { "verbose", no_argument, nullptr, 'v' },
        { "median", no_argument, nullptr, 'm'},
        { "trader_info", no_argument, nullptr, 'i'},
        { "time_travelers", no_argument, nullptr, 't'},
        { nullptr, 0, nullptr, '\0' }};
    
    while ((option = getopt_long(argc, argv, "vmit", longOpts, &option_index)) != -1) {
        switch (option) {
            case 'v':
                verbose = true;
                break;
            
            case 'm':
                median = true;
                break;
            
            case 'i':
                trader_info = true;
                break;
            
            case 't':
                time_travel = true;
                break;
                
            default:
                cerr << "Invalid commnand line. Abort\n";
                exit(1);
        } // switch
    } // while loop
} // get_options end
void Market::read_inital_input() {
    string comment;
    string stuff;
    cin >> stuff >> comment;
    getline(cin, stuff);
    cin >> stuff >> mode;
    cin >> stuff >> num_traders;
    cin >> stuff >> num_stocks;
}
void Market::read_all_input() {
    int time_it_came_in;
    std::string type;
    std::string current_stock;
    std::string current_trader;
    std::string stock_price;
    std::string stock_quantity;
    stringstream ss;
    string extras;
    unsigned int seed;
    int timestamp = 0;
    int place = 0;
    int total_sold_stocks = 0;
    Market m1;
    stock_class s1;
 //   int junk_char;
    
    // taking in input for PR
    if (mode == "PR") {
        cin >> extras >> random_seed;
        cin >> extras >> num_orders;
        cin >> extras >> arrival_rate;
        seed = random_seed;
        P2random::PR_init(ss, seed, num_traders, num_stocks, num_orders, arrival_rate);
    }
//    else if (mode == "TL") {
//        cin >> extras >> num_traders >> extras >> num_stocks;
//    }
    istream & inputS = (mode == "PR") ? ss : std::cin;
    
    // have to resize the vectors for num_traders and stocks
    traders.resize(num_traders);
    stocks.resize(num_stocks);
    stock_median.resize(num_stocks);
    
    // reading in all of the orders from the user input
    cout << "Processing orders...\n";
    while (inputS >> time_it_came_in >> type >>
           current_trader >> current_stock >> stock_price >> stock_quantity) {
        place++;
        // error checking
        if (time_it_came_in < 0) {
            cerr << "Invalid timestamp\n";
            exit(1);
        }
        
        unsigned int trader_num = (unsigned int)stoi(current_trader.substr(1,current_trader.length()));
        size_t stock_num = (size_t)stoi(current_stock.substr(1, current_stock.length()));
        int price = stoi(stock_price.substr(1, stock_price.length() - 1));
        int quantity = stoi(stock_quantity.substr(1, stock_quantity.length() - 1));
        
        if ((int)trader_num < 0 || (trader_num > num_traders - 1)) {
            cerr << "Invalid trader ID number\n";
            exit(1);
        }
        if ((int)stock_num < 0 || stock_num > (num_stocks - 1)) {
            cerr << "Invalid stock number\n";
            exit(1);
        }
        if (price <= 0 || quantity <= 0) {
            cerr << "Abort. Stock price or quantity is negative\n";
            exit(1);
        }
        if (time_it_came_in < timestamp) {
            cerr << "'Timestamps in decreasing order\n";
            exit(1);
        }
        // end of error checking
        
        // beginning of algorithm
        
        order current_order;
        current_order.order_timestamp = time_it_came_in;
        current_order.place_in_line = place;
        current_order.price = price;
        current_order.quantity = quantity;
        current_order.type = type;
        current_order.trader_id = (size_t)trader_num;
        current_order.stock_id = (size_t)stock_num;
        
//        if (current_order.trader_id == 10) {
//            cout << "hello";
//        }
        
        if (timestamp != time_it_came_in) {
            if (median) {
               s1.print_median_pt2(num_stocks, timestamp);
            }
            timestamp = time_it_came_in;
        }
        if (time_travel) {
            find_traveler(current_order);
        }
        
        while (current_order.quantity != 0) {
        // atually starting to trade
        if (current_order.type == "SELL") {
            
            // if there are no buyers to sell to
            if (stock_median[current_order.stock_id].buy_PQ.empty()) {
            stock_median[current_order.stock_id].sell_PQ.push(current_order);
                current_order.quantity = 0;
                continue;
            }
            
            /* if the price that the seller wants to sell if less than a
             buyer's price (THE SELLER WANTS THIS) */
            else if (current_order.price <= stock_median[current_order.stock_id].buy_PQ.top().price) {
                // when the quantity being sold is greater than wanting to be bought
                if (current_order.quantity > stock_median[current_order.stock_id].buy_PQ.top().quantity){
                    
                 // update median
                    if (median) {
                        s1.find_median(current_order.stock_id, stock_median[current_order.stock_id].buy_PQ.top().price);
                    }
                 // a transaction is made
                    if(verbose) {
                        m1.verbose_function(current_order.stock_id, current_order.trader_id, stock_median[current_order.stock_id].buy_PQ.top().trader_id, stock_median[current_order.stock_id].buy_PQ.top().quantity, stock_median[current_order.stock_id].buy_PQ.top().price);
                    }
                    num_transactions++;
                    total_sold_stocks += stock_median[current_order.stock_id].buy_PQ.top().quantity;
                    
                    int newQunatity = current_order.quantity - stock_median[current_order.stock_id].buy_PQ.top().quantity;
                    current_order.quantity = newQunatity;
//                    order NewOrder;
//                    NewOrder = current_order;
//                    NewOrder.quantity = newQunatity;
                //    stock_median[current_order.stock_id].sell_PQ.push(NewOrder);
                    
                    
                    // updating the seller net
              //      size_t running_quantity = traders[current_order.trader_id].amount_sold;
             //       int running_net = traders[current_order.trader_id].net;
                
                    
                    traders[current_order.trader_id].amount_sold += (size_t)stock_median[current_order.stock_id].buy_PQ.top().quantity;
                    
                    traders[current_order.trader_id].net += ((int)stock_median[current_order.stock_id].buy_PQ.top().quantity * (int)stock_median[current_order.stock_id].buy_PQ.top().price);
                    
                    // updating the buyer net
                    size_t buyer_id = stock_median[current_order.stock_id].buy_PQ.top().trader_id;
                  //  size_t buyer_running = traders[buyer_id].amount_bought;
                 //   int buyer_net = traders[buyer_id].net;
                    
                    traders[buyer_id].amount_bought += (size_t)stock_median[current_order.stock_id].buy_PQ.top().quantity;
                    
                    traders[buyer_id].net -= ((int)stock_median[current_order.stock_id].buy_PQ.top().price * (int)stock_median[current_order.stock_id].buy_PQ.top().quantity);
                    
                    // need to pop the buy order, becuase it's now unneeded
                    stock_median[current_order.stock_id].buy_PQ.pop();
                    continue;
                }
                // if the quantity being sold is less than requested
            else if (current_order.quantity < stock_median[current_order.stock_id].buy_PQ.top().quantity) {
                
                // if median is present
                if (median) {
                    s1.find_median(current_order.stock_id, stock_median[current_order.stock_id].buy_PQ.top().price);
                }
                //a transaction is complete, still selling buy not all that we have
                if (verbose) {
                    m1.verbose_function(current_order.stock_id, current_order.trader_id, stock_median[current_order.stock_id].buy_PQ.top().trader_id, current_order.quantity, stock_median[current_order.stock_id].buy_PQ.top().price);
                }
                num_transactions++;
                total_sold_stocks += current_order.quantity;
                
                /* don't have to pop the sell order becasue it was never pushed back
                 but the buyer still wants more shares
                 */
                int new_quantity = stock_median[current_order.stock_id].buy_PQ.top().quantity - current_order.quantity;
                order NewOrder;
                NewOrder = stock_median[current_order.stock_id].buy_PQ.top();
                NewOrder.quantity = new_quantity;
               // stock_median[current_order.stock_id].buy_PQ.pop();
              //  stock_median[current_order.stock_id].buy_PQ.push(NewOrder);
                
                
                // updating the seller net
            //    size_t running_quantity = traders[current_order.trader_id].amount_sold;
             //   int running_net = traders[current_order.trader_id].net;
                traders[current_order.trader_id].amount_sold += (size_t)current_order.quantity;
                
                traders[current_order.trader_id].net += ((int)current_order.quantity * (int)stock_median[current_order.stock_id].buy_PQ.top().price);
                
                // updating the buyer net
                size_t buyer_id = stock_median[current_order.stock_id].buy_PQ.top().trader_id;
             //   size_t buyer_running = traders[buyer_id].amount_bought;
               // int buyer_net = traders[buyer_id].net;
            
                traders[buyer_id].amount_bought += (size_t)current_order.quantity;
                traders[buyer_id].net -= ((int)stock_median[current_order.stock_id].buy_PQ.top().price * (int)current_order.quantity);
                
                stock_median[current_order.stock_id].buy_PQ.pop();
                stock_median[current_order.stock_id].buy_PQ.push(NewOrder);
                current_order.quantity = 0;
                continue;
            }
            // if the quantity that i'm selling and buying is the same
            else {
                // update median
                if (median) {
                    s1.find_median(current_order.stock_id, stock_median[current_order.stock_id].buy_PQ.top().price);
                }
                // still a transations
                if (verbose) {
                    m1.verbose_function(current_order.stock_id, current_order.trader_id, stock_median[current_order.stock_id].buy_PQ.top().trader_id, current_order.quantity, stock_median[current_order.stock_id].buy_PQ.top().price);
                }
                num_transactions++;
                total_sold_stocks += current_order.quantity;
                
                // updating the seller net
              //  size_t running_quantity = traders[current_order.trader_id].amount_sold;
             //   int running_net = traders[current_order.trader_id].net;
                traders[current_order.trader_id].amount_sold += (size_t)current_order.quantity;
                
                traders[current_order.trader_id].net +=((int)current_order.quantity * (int)stock_median[current_order.stock_id].buy_PQ.top().price);
                
                // updating the buyer net
                 size_t buyer_id = stock_median[current_order.stock_id].buy_PQ.top().trader_id;
             //   size_t buyer_running = traders[buyer_id].amount_bought;
            //    int buyer_net = traders[buyer_id].net;
                
                traders[buyer_id].amount_bought += (size_t)current_order.quantity;
                traders[buyer_id].net -= ((int)stock_median[current_order.stock_id].buy_PQ.top().price * (int)current_order.quantity);
                
                stock_median[current_order.stock_id].buy_PQ.pop();
                current_order.quantity = 0;
                continue;
            }
          }
            else {
                stock_median[current_order.stock_id].sell_PQ.push(current_order);
                current_order.quantity = 0;
                continue;
            }
        }
        if (current_order.type == "BUY") {
            if (stock_median[current_order.stock_id].sell_PQ.empty()) {
                stock_median[current_order.stock_id].buy_PQ.push(current_order);
                current_order.quantity = 0;
                continue;
            }
            else if (current_order.price >= stock_median[current_order.stock_id].sell_PQ.top().price) {
                if (current_order.quantity < stock_median[current_order.stock_id].sell_PQ.top().quantity) {
                    // update median
                    if (median) {
                        s1.find_median(current_order.stock_id, stock_median[current_order.stock_id].sell_PQ.top().price);
                    }
                    // still a transations
                    if (verbose) {
                        verbose_function(current_order.stock_id, stock_median[current_order.stock_id].sell_PQ.top().trader_id, current_order.trader_id, current_order.quantity, stock_median[current_order.stock_id].sell_PQ.top().price);
                    }
                    num_transactions++;
                    total_sold_stocks += current_order.quantity;
                    
                    // updating the buyer info
                 //   size_t buyer_quantity = traders[current_order.trader_id].amount_bought;
              //      size_t buyer_net = (size_t)traders[current_order.trader_id].net;
                    int net_helper = (stock_median[current_order.stock_id].sell_PQ.top().price * current_order.quantity);
                    
                    traders[current_order.trader_id].amount_bought += (size_t)current_order.quantity;
                    traders[current_order.trader_id].net -= (int)net_helper;
                    
                    // updating the seller info
                    size_t seller_id = stock_median[current_order.stock_id].sell_PQ.top().trader_id;
               //     size_t seller_quantity = traders[seller_id].amount_sold;
                  //  int seller_net = traders[seller_id].net;
                    
                    traders[seller_id].amount_sold += (size_t)current_order.quantity;
                    traders[seller_id].net += ((int)current_order.quantity * (int)stock_median[current_order.stock_id].sell_PQ.top().price);
                    
                    // new order for the seller
                    
                    int new_quantity = stock_median[current_order.stock_id].sell_PQ.top().quantity - current_order.quantity;
                    order NewOrder;
                    NewOrder = stock_median[current_order.stock_id].sell_PQ.top();
                    stock_median[current_order.stock_id].sell_PQ.pop();
                    NewOrder.quantity = new_quantity;
                    stock_median[current_order.stock_id].sell_PQ.push(NewOrder);
                    current_order.quantity = 0;
                    
                    
                    
                } // if seller quantity is greater
                else if (current_order.quantity > stock_median[current_order.stock_id].sell_PQ.top().quantity) {
                    // update median
                    if (median) {
                        s1.find_median(current_order.stock_id, stock_median[current_order.stock_id].sell_PQ.top().price);
                    }
                    // still a transations
                    if (verbose) {
                        verbose_function(current_order.stock_id, stock_median[current_order.stock_id].sell_PQ.top().trader_id, current_order.trader_id, stock_median[current_order.stock_id].sell_PQ.top().quantity, stock_median[current_order.stock_id].sell_PQ.top().price);
                    }
                    num_transactions++;
                    total_sold_stocks += current_order.quantity;
                    
                    // updating the buyer info
                 //   size_t buyer_quantity = traders[current_order.trader_id].amount_bought;
                //    int buyer_net = traders[current_order.trader_id].net;
                    
                    traders[current_order.trader_id].amount_bought += (size_t)stock_median[current_order.stock_id].sell_PQ.top().quantity;
                    traders[current_order.trader_id].net -= ((int)stock_median[current_order.stock_id].sell_PQ.top().price * (int)stock_median[current_order.stock_id].sell_PQ.top().quantity);
                    
                    // updating the seller info
                    size_t seller_id = stock_median[current_order.stock_id].sell_PQ.top().trader_id;
                   // size_t seller_quantity = traders[seller_id].amount_sold;
                 //   int seller_net = traders[seller_id].net;
                    
                    traders[seller_id].amount_sold += (size_t)stock_median[current_order.stock_id].sell_PQ.top().quantity;
                    traders[seller_id].net += ((int)stock_median[current_order.stock_id].sell_PQ.top().quantity * (int)stock_median[current_order.stock_id].sell_PQ.top().price);
                    
                    int new_quantity = (int)current_order.quantity - (int)stock_median[current_order.stock_id].sell_PQ.top().quantity;
//                    order NewOrder;
//                    NewOrder = current_order;
//                    NewOrder.quantity = new_quantity;
//                    stock_median[current_order.stock_id].buy_PQ.push(NewOrder);
                    current_order.quantity = new_quantity;
                    
                    
                    stock_median[current_order.stock_id].sell_PQ.pop();
                    
                } // if buying quantity is greater
                else {
                    // update median
                    if (median) {
                        s1.find_median(current_order.stock_id, stock_median[current_order.stock_id].sell_PQ.top().price);
                    }
                    // still a transations
                    if (verbose) {
                        verbose_function(current_order.stock_id, stock_median[current_order.stock_id].sell_PQ.top().trader_id, current_order.trader_id, current_order.quantity, stock_median[current_order.stock_id].sell_PQ.top().price);
                    }
                    num_transactions++;
                    total_sold_stocks += current_order.quantity;
                    
                   // updating the buyer info
                  //  size_t buyer_quantity = traders[current_order.trader_id].amount_bought;
               //     int buyer_net = traders[current_order.trader_id].net;
                    
                    traders[current_order.trader_id].amount_bought += (size_t)current_order.quantity;
                    traders[current_order.trader_id].net -= ((int)stock_median[current_order.stock_id].sell_PQ.top().price * (int)current_order.quantity);
                    
                    // updating the seller info
                    size_t seller_id = stock_median[current_order.stock_id].sell_PQ.top().trader_id;
                  //  size_t seller_quantity = traders[seller_id].amount_sold;
                   // int seller_net = traders[seller_id].net;
                    
                    traders[seller_id].amount_sold+= (size_t)current_order.quantity;
                    traders[seller_id].net += ((int)current_order.quantity * (int)stock_median[current_order.stock_id].sell_PQ.top().price);
                    
                    stock_median[current_order.stock_id].sell_PQ.pop();
                    current_order.quantity = 0;
                    
                    
                    
                } // end if quantities are equal
            } // end of price
            else {
                stock_median[current_order.stock_id].buy_PQ.push(current_order);
                current_order.quantity = 0;
                continue;
            }
        }
        }
    }
    if (median) {
    s1.print_median_pt2(num_stocks, timestamp);
    }
}

void stock_class::find_median(size_t stock, int price) {
    // default if they are both empty
    if (stock_median[stock].bottom.empty()) {
        stock_median[stock].bottom.push(price);
        return;
    }
    else if (stock_median[stock].top.empty()) {
        stock_median[stock].top.push(price);
        return;
    }
    
    int temp_top = stock_median[stock].top.top();
    int temp_bottom = stock_median[stock].bottom.top();
    
    // if they are both the same size
    if (stock_median[stock].top.size() == stock_median[stock].bottom.size()) {
        if (temp_top < temp_bottom) {
            stock_median[stock].top.pop();
            stock_median[stock].top.push(temp_bottom);
            stock_median[stock].bottom.pop();
            stock_median[stock].bottom.push(temp_top);
        }
    }
    
    temp_top = stock_median[stock].top.top();
    temp_bottom = stock_median[stock].bottom.top();
    
    /* checking to see if the top of both arrays are less than the price to see where to
    place it */
    if (price <= temp_bottom) {
        stock_median[stock].bottom.push(price);
    }
    else {
        stock_median[stock].top.push(price);
    }
    temp_top = stock_median[stock].top.top();
    temp_bottom = stock_median[stock].bottom.top();
    
    size_t top_size = stock_median[stock].top.size();
    size_t bottom_size = stock_median[stock].bottom.size();
        if ((int)(top_size - bottom_size) > 1) {
            stock_median[stock].top.pop();
            stock_median[stock].bottom.push(temp_top);
    }
        else if ((int)(bottom_size - top_size) > 1) {
            stock_median[stock].bottom.pop();
            stock_median[stock].top.push(temp_bottom);
        }
}
size_t stock_class::print_median(size_t stock) {
    size_t temp_top = 0;
    size_t temp_bottom = 0;
    size_t top_size = stock_median[stock].top.size();
    size_t bottom_size = (size_t)stock_median[stock].bottom.size();
    if (top_size != 0) {
     temp_top = (size_t)stock_median[stock].top.top();
    }
    if (bottom_size != 0) {
    temp_bottom = (size_t)stock_median[stock].bottom.top();
    }
    if (top_size == bottom_size) {
        return ((temp_top + temp_bottom)/2);
    }
    else if (top_size > bottom_size) {
        return temp_top;
    }
    else {
        return temp_bottom;
    }
}
void stock_class::print_median_pt2(size_t stock_num, int time) {
    for (size_t i = 0; i < stock_num; i++) {
        if ((stock_median[i].top.size() != 0) || (stock_median[i].bottom.size() != 0)) {
            cout << "Median match price of Stock " << i << " at time " << time << " is $" << print_median(i) << "\n";
        }
    }
}

void Market::verbose_function(size_t stock_id, size_t seller_id, size_t buyer_id, int quan, int price) {
    cout << "Trader " << buyer_id << " purchased " << quan << " shares of Stock " << stock_id << " from Trader " << seller_id << " for $" << price << "/share\n";
}

void Market::end_report() {
    cout << "---End of Day---\n";
    cout << "Orders Processed: " << num_transactions << "\n";
}

void Market::trader_output() {
    if (trader_info) {
        cout << "---Trader Info---\n";
        for (size_t i = 0; i < num_traders; i++) {
            cout << "Trader " << i << " bought " << traders[i].amount_bought << " and sold " << traders[i].amount_sold << " for a net transfer of $" << traders[i].net << "\n";
        }
    }
    else {
        return;
    }
}
void Market::find_traveler(order &x) {
    if (x.type == "SELL" && !stock_median[x.stock_id].explore.had_sell) {
            stock_median[x.stock_id].explore.had_sell = true;
            stock_median[x.stock_id].explore.buying_price = x.price;
            stock_median[x.stock_id].explore.buying_time = x.order_timestamp;
            stock_median[x.stock_id].explore.temp_buying_time = x.order_timestamp;
            stock_median[x.stock_id].explore.temp_buying_price = x.price;
    }
    else if (x.type == "SELL") {
        if (stock_median[x.stock_id].explore.had_sell) {
            if (stock_median[x.stock_id].explore.buying_price > x.price) {
                if (stock_median[x.stock_id].explore.temp_buying_price > x.price) {
                    stock_median[x.stock_id].explore.temp_buying_price = x.price;
                    stock_median[x.stock_id].explore.temp_buying_time = x.order_timestamp;
                    stock_median[x.stock_id].explore.temp_selling_time = 0;
                    stock_median[x.stock_id].explore.temp_selling_price = 0;
                }
            }
        }
    }
    int temp_price_helper = x.price - stock_median[x.stock_id].explore.temp_buying_price;
    int temp_price_helper2 = stock_median[x.stock_id].explore.temp_selling_price - stock_median[x.stock_id].explore.temp_buying_price;
    int temp_price_helper3 = stock_median[x.stock_id].explore.selling_price - stock_median[x.stock_id].explore.buying_price;
    
    if (x.type == "BUY") {
        if (stock_median[x.stock_id].explore.had_sell) {
            if (temp_price_helper > temp_price_helper2) {
                stock_median[x.stock_id].explore.temp_selling_time = x.order_timestamp;
                stock_median[x.stock_id].explore.temp_selling_price = x.price;
            }
        }
    }
    
    // final check
    if (temp_price_helper2 > temp_price_helper3) {
        stock_median[x.stock_id].explore.buying_price = stock_median[x.stock_id].explore.temp_buying_price;
        stock_median[x.stock_id].explore.buying_time = stock_median[x.stock_id].explore.temp_buying_time;
        stock_median[x.stock_id].explore.selling_time = stock_median[x.stock_id].explore.temp_selling_time;
        stock_median[x.stock_id].explore.selling_price = stock_median[x.stock_id].explore.temp_selling_price;
    }
}
void Market::time_traveler_report() {
    if (time_travel) {
        cout << "---Time Travelers---\n";
        for (unsigned int i = 0; i < num_stocks; i++) {
            if (stock_median[i].explore.selling_price == 0) {
                cout << "A time traveler could not make a profit on shares of Stock " << i << "\n";
            }
            else {
                cout << "A time traveler would buy shares of Stock " << i << " at time " << stock_median[i].explore.buying_time << " for $" << stock_median[i].explore.buying_price << " and sell these shares at time " << stock_median[i].explore.selling_time << " for $" << stock_median[i].explore.selling_price << "\n";
            }
        }
    }
    else {
        return;
    }
}

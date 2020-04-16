// PROJECT IDENTIFIER 1CAEF3A0FEDD0DEC26BA9808C69D4D22A9962768
//  main.cpp
//  Lab 2
//
//  Created by Michelle Figueroa on 2/4/20.
//  Copyright © 2020 Michelle Figueroa. All rights reserved.
//

#include <iostream>
#include <iterator>
#include <stack>
#include "xcode_redirect.hpp"

using namespace std;

bool checkBalance();
stack<char>balanced;

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    if (checkBalance()) {
        cout << "Balanced\n";
    }
    else {
        cout << "Not balanced\n";
    }
    return 0;
}

bool checkBalance() {
    char input;
    while (cin >> input) {
        switch(input) {
            case '(':
                balanced.push(input);
                break;
                
            case '{':
                balanced.push(input);
                break;
                
            case '[':
                balanced.push(input);
                break;
                
            case ')':
                if (balanced.empty()) {
                    return false;
                }
                if (balanced.top() == '(') {
                    balanced.pop();
                }
                else {
                    return false;
                }
                break;
                
            case '}':
                if (balanced.empty()) {
                    return false;
                }
                if (balanced.top() == '{') {
                    balanced.pop();
                }
                else {
                    return false;
                }
                break;
                
            case ']':
                if (balanced.empty()) {
                    return false;
                }
                if (balanced.top() == '[') {
                    balanced.pop();
                }
                else {
                    return false;
                }
                break;
                
            default:
                break;
        }
    }
    if (balanced.empty()) {
        return true;
    }
    return false;
}

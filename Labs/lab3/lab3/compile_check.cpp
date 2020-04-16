// LAB 3 COMPILE CHECK
// Project Identifer: 5AE7C079A8BF493DDDB6EF76D42136D183D8D7A8
#include "String.h"
#include <cstring>
#include <stdio.h> /* printf */

int main() {
    // THIS FILE IS NOT COMPREHENSIVE - YOU SHOULD WRITE YOUR OWN TESTS
    String myString = "hello world";
    printf("Front is %c\n", myString.front());
    printf("Back is %c\n", myString.back());
    String mySubstring = myString.substr(0, 5);
    printf("My substring is %s\n", mySubstring.c_str());
    size_t index = myString.find("worl", 2);
    myString[index] = 'l';
    myString.erase(9, 1);
    printf("%s\n", myString.c_str());
    return 0;
}

// EXPECTED OUTPUT:
/* 
Front is h
Back is d
My substring is hello
hello lord
*/

/**
 *  change.cpp
 *  
 *  This file contains a program using an elementary greedy algorithm finding the
 *  the minimum number of coins needed to change the input value into coins with
 *  denominations of 1, 5, 10.
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date: January 29, 2018
 *
 **/

#include <iostream>

/**
 *  Function get_change
 *
 *  Finds the minimum number of coins to change the input value.
 *
 *  Arguments:
 *      int m - The integer value of the amount to change.
 *
 *  Return:
 *      int - The minimum number of coins.
 **/

int get_change(int m) {
    int n = 0;
    int v = 0;
    
    // dimes
    int dimes = (m - v) / 10;
    n += dimes;
    v += dimes * 10;
    
    if (v == m) { return n; }
    
    // nickles
    int nickels = (m - v) / 5;
    n += nickels;
    v += nickels * 5;
    
    if (v == m) { return n; }
    
    // pennies - just the difference left
    n += m - v;
    
    return n;
}

int main(int argc, const char * argv[]) {
    int m;
    std::cin >> m;
    std::cout << get_change(m) << std::endl;
    return 0;
}

/**
 *  change_dp.cpp
 *
 *  This file implements a dynammic programming algorithm to sfind the minimum number
 *  of coins needed to change a monetary value.
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   February 28, 2018
 *
 **/

#include <iostream>

struct coin_denominations {
    int numCoins = 3;
    int coins[] {1, 3, 4};
} denominations;

int get_change(int m) {
    
    int min_num_coins [m + 1];
    min_num_coins[0] = 0;
    
    for (int i = 1; i <= m; ++i) {
        min_num_coins[i] = 1001;
        
        for (int j = 0; j < denominations.numCoins; ++j) {
            int coin = denominations.coins[j];
            
            if (i >= coin) {
                int num_coins = min_num_coins[i - coin] + 1;
                
                if (num_coins < min_num_coins[i]) {
                    min_num_coins[i] = num_coins;
                }
            }
        }
    }
    
    return min_num_coins[m];
}

int main() {
    int m;
    std::cin >> m;
    std::cout << get_change(m) << '\n';
    return 0;
}

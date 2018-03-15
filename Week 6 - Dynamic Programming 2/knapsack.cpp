/**
 *  knapsack.cpp
 *
 *  This file implements implements a dynamic programming algorithm to solve
 *  a variant of the 'knapsack' problem. Given a maximum weight for the
 *  knapsack and the weights of available bars of gold, the maximum weight
 *  that can be carried is returned.
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   March 15, 2018
 **/

#include <iostream>
#include <vector>

using std::vector;


/**
 *  Function optimal_weight
 *
 *  Finds the maximum weight that can be carried given the maximum weight
 *  the knapsack can handle and the weights of the available bars of gold.
 **/
int optimal_weight(int W, const vector<int> &weights) {
  int n = weights.size();
  vector<vector<int>> current_weight(W + 1, vector<int>(n + 1, 0));
  for (size_t i = 1; i <= n; ++i) {
      for (size_t w = 1; w <= W; ++w) {
          current_weight[w][i] = current_weight[w][i - 1];
          int item_weight = weights[i - 1];
          if (item_weight <= w) {
              int num = current_weight[w - item_weight][i - 1] + item_weight;
              if (current_weight[w][i] < num) {
                  current_weight[w][i] = num;
              }
          }
      }
  }
  return current_weight[W][n];
}

int main() {
  int n, W;
  std::cin >> W >> n;
  vector<int> w(n);
  for (int i = 0; i < n; i++) {
    std::cin >> w[i];
  }
  std::cout << optimal_weight(W, w) << '\n';
}

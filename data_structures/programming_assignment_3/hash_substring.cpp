/**
 *  hash_substrings.cpp
 *  Data Structures: Programming Assignment 3
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   May 9, 2018
 **/
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using std::string;
using std::vector;
typedef unsigned long long ull;

struct Data {
    string pattern, text;
};

Data read_input() {
    Data data;
    std::cin >> data.pattern >> data.text;
    return data;
}

void print_occurrences(const std::vector<size_t>& output) {
    for (size_t i = 0; i < output.size(); ++i)
        std::cout << output[i] << " ";
    std::cout << "\n";
}

ull poly_hash(const string& s, ull p, ull x) {
    ull hash = 0;
    for (int i = static_cast<int> (s.size()) - 1; i >= 0; --i) {
        hash = ((hash * x + s[i]) % p + p) % p;
    }
    return hash;
}

vector<ull> precomputeHashes(const string& text, size_t pattern_length,
                             ull prime, ull x) {
    vector<ull> hashes(text.length() - pattern_length + 1);
    string s = text.substr(text.length() - pattern_length);
    hashes[text.length() - pattern_length] = poly_hash(s, prime, x);
    ull y = 1;
    for (size_t i = 0; i < pattern_length; i++) {
        y = ((y * x) % prime + prime) % prime;
    }
    for (int i = static_cast<int>(text.length() - pattern_length) - 1; i >= 0; --i) {
        hashes[i] = (
                     (
                      (x * hashes[i + 1]) % prime
                      + text[i] % prime
                      - (y * text[i + pattern_length]) % prime
                      ) + prime
                     ) % prime;
    }
    return hashes;
}

vector<size_t> get_occurrences(const Data& input) {
    // Implements Rabin-Karp algorithm
    std::vector<size_t> res;
    srand((unsigned int) time(NULL));
    ull prime = 1000000007;
    ull x = (rand() + 1) % (prime - 1);
    ull hash = poly_hash(input.pattern, prime, x);
    size_t pattern_length = input.pattern.length();
    size_t text_length = input.text.length();
    vector<ull> hashes = precomputeHashes(input.text, pattern_length,
                                          prime, x);
    for (size_t i = 0; i <= text_length - pattern_length; i++) {
        if (hash == hashes[i] &&
            input.text.substr(i, pattern_length) == input.pattern) {
            res.push_back(i);
        }
    }
    return res;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    print_occurrences(get_occurrences(read_input()));
    return 0;
}

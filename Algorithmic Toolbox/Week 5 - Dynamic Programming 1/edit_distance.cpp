#include <iostream>
#include <string>

using std::string;

int min(int x, int y, int z) {
    int res = (x <= y ? x : y);
    return (z < res ? z : res);
}

int edit_distance(string &str1, string &str2) {

    int n = str1.length();
    int m = str2.length();

    //prepend whitespace to strings to avoid offset indeces.
    str1.insert(0, 1, ' ');
    str2.insert(0, 1, ' ');

    int dp_table[m + 1][n + 1];
    dp_table[0][0] = 0;

    for (int i = 1; i <= m; ++i) {
        dp_table[i][0] = i;
    }

    for (int j = 1; j <= n; ++j) {
        dp_table[0][j] = j;
    }

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {

            int ins = dp_table[i][j - 1] + 1;
            int del = dp_table[i - 1][j] + 1;
            int mch = dp_table[i - 1][j - 1];
            int mis = dp_table[i - 1][j - 1] + 1;

            dp_table[i][j] = (str1[j] == str2[i] ?
                                min(ins, del, mch) :
                                min(ins, del, mis));

        }
    }

    return dp_table[m][n];
}

int main() {
    string str1;
    string str2;
    std::cin >> str1 >> str2;
    std::cout << edit_distance(str1, str2) << std::endl;
    return 0;
}

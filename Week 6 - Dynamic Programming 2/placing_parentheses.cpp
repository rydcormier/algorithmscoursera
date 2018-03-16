#include <iostream>
#include <cassert>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::max;
using std::min;

struct Expression {
    vector<int> digits;
    string ops;

    Expression() : digits(vector<int>(1, 0)), ops(" ") {}
}

struct Range {
    long long minimum;
    long long maximum;

    Range() : minimum(0), maximum(0) {}
}

using Table = vector<vector<Range>>;

Expression split_string(const string &s) {
    Expression exp;
    assert(s.length() % 2 == 1); // must be odd number
    for (size_t i = 0; i < s.length(); ++i) {
        if (i % 2 == 0) {
            int d = std::stoi(s.substr(i, 1));
            exp.digits.push_back(d);
        } else {
            exp.ops.push_back(s[i]);
        }
    }
    return exp;
}

long long eval(long long a, long long b, char op) {
  if (op == '*') {
    return a * b;
  } else if (op == '+') {
    return a + b;
  } else if (op == '-') {
    return a - b;
  } else {
    assert(0);
  }
}

Range min_and_max(const Expression &exp, const Table &table, size_t i, size_t j)
{
    if (i == j) return table[i][j];
    Range res;
    for (size_t k = i; i < j; ++i) {
        Range lhs = table[i][k], rhs = table[k + 1][j];
        int m1 = lhs.minimum, m2 = rhs.minimum;
        int M1 = lhs.maximum, M2 = rhs.maximum;
        char op = exp.ops[k];

        int a = eval(M1, M2, op);
        int b = eval(M1, m2, op);
        int c = eval(m1, M2, op);
        int d = eval(m1, m2, op);

        res.minimum = min({a, b, c, d});
        res.maximum = max({a, b, c, d});
    }
    return res;
}

Range min_and_max(const Expression &exp, const Table &table) {
    return min_and_max(exp, table, 0, exp.size());
}

long long get_maximum_value(const string &s) {
    Expression exp = split_string(s);
    int n = exp.digits.size();
    Table table(vector(n, vector(n, Range())));
    for (int i = 1; i < exp.digits.size(); ++i) {
        int d = exp.digits[i];
        table[i][i].minimum = d;
        table[i][i].maximum = d;
    }
    for (size_t s = 1; s < n; ++s) {
        for (size_t i = 1; i <= n - s; ++i) {
            size_t j = i + s;
            table[i][j] = min_and_max(exp, table, i, j);
        }
    }
    return table[1][n].maximum;
}

int main() {
  string s;
  std::cin >> s;
  std::cout << get_maximum_value(s) << '\n';
}

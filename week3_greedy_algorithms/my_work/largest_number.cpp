#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

bool is_greater_or_equal(string a, string b) {
    int lhs = std::stoi(a + b), rhs = std::stoi(b + a);
    return (lhs >= rhs);
}

bool is_less_than(string a, string b) {
    return (!is_greater_or_equal(a, b));
}

class IntString {
    string str;
    int n;
    
public:
    IntString (int a) : str(std::to_string(a)), n(a) {};
    IntString (string s) : str(s), n(std::stoi(s)) {};
    string as_string() const { return this->str; }
    int as_int() const { return this->n; }
    bool operator< (const IntString &rhs) const { return is_less_than(this->str, rhs.as_string()); }
};

string concatenate_vector(vector<string> a) {
    string result = "";
    for (size_t i = 0; i < a.size(); ++i) {
        result += a[i];
    }
    return result;
}

string largest_number(vector<string> &a) {
    vector<IntString> ais;
    for (size_t i = 0; i < a.size(); ++i) {
        ais.push_back(IntString(a[i]));
    }
    std::sort(ais.begin(), ais.end());
    std::reverse(ais.begin(), ais.end());
    // reuse a
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] = ais[i].as_string();
    }
    return concatenate_vector(a);
}

int main() {
    int n;
    std::cin >> n;
    vector<string> a(n);
    for (size_t i = 0; i < a.size(); i++) {
        std::cin >> a[i];
    }
    std::cout << largest_number(a);
}

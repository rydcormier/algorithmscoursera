/**
 *  hash_chains.cpp
 *  Data Structures: Programming Assignment 3
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   May 9, 2018
 **/
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

using std::string;
using std::vector;
using std::list;
using std::cin;

struct Query {
    string type, s;
    size_t ind;
};

struct Chain {
    list<string> elems;
    
    string to_string() const {
        string res = "";
        for (list<string>::const_iterator it = elems.begin(); it != elems.end(); it++) {
            if (! res.empty()) {
                res += " ";
            }
            res += *it;
        }
        return res;
    }
    
    void add(const string& str) {
        bool exists = false;
        for (list<string>::iterator it = elems.begin(); it != elems.end(); it++) {
            if (str == *it) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            elems.push_front(str);
        }
    }
    
    void erase(const string& str) {
        for (list<string>::iterator it = elems.begin(); it != elems.end(); it++) {
            if (str == *it) {
                elems.erase(it);
                break;
            }
        }
    }
    
    string find(const string& str) const{
        for (list<string>::const_iterator it = elems.begin(); it != elems.end(); it++) {
            if (str == *it) {
                return "yes";
            }
        }
        return "no";
    }
    
};

class QueryProcessor {
    int bucket_count;
    // implement chains;
    vector<Chain> elems;
    
    size_t hash_func(const string& s) const {
        static const size_t multiplier = 263;
        static const size_t prime = 1000000007;
        unsigned long long hash = 0;
        for (int i = static_cast<int> (s.size()) - 1; i >= 0; --i)
            hash = (hash * multiplier + s[i]) % prime;
        return hash % bucket_count;
    }
    
public:
    explicit QueryProcessor(int bucket_count) : bucket_count(bucket_count),
    elems(vector<Chain>(bucket_count)) {}
    
    Query readQuery() const {
        Query query;
        cin >> query.type;
        if (query.type != "check")
            cin >> query.s;
        else
            cin >> query.ind;
        return query;
    }
    
    void writeSearchResult(bool was_found) const {
        std::cout << (was_found ? "yes\n" : "no\n");
    }
    
    void processQuery(const Query& query) {
        if (query.type == "check") {
            std::cout << elems[query.ind].to_string() << std::endl;
        } else {
            Chain* chain = &elems[hash_func(query.s)];
            if (query.type == "add") {
                chain->add(query.s);
            } else if (query.type == "del") {
                chain->erase(query.s);
            } else if (query.type == "find") {
                std::cout << chain->find(query.s) << std::endl;
            }
        }
    }
    
    void processQueries() {
        int n;
        cin >> n;
        for (int i = 0; i < n; ++i)
            processQuery(readQuery());
    }
};

class QueryProcessorNaive {
    int bucket_count;
    // store all strings in one vector
    vector<string> elems;
    
    size_t hash_func(const string& s) const {
        static const size_t multiplier = 263;
        static const size_t prime = 1000000007;
        unsigned long long hash = 0;
        for (int i = static_cast<int> (s.size()) - 1; i >= 0; --i)
            hash = (hash * multiplier + s[i]) % prime;
        return hash % bucket_count;
    }
    
public:
    explicit QueryProcessorNaive(int bucket_count) : bucket_count(bucket_count) {}
    
    Query readQuery() const {
        Query query;
        cin >> query.type;
        if (query.type != "check")
            cin >> query.s;
        else
            cin >> query.ind;
        return query;
    }
    
    void writeSearchResult(bool was_found) const {
        std::cout << (was_found ? "yes\n" : "no\n");
    }
    
    void processQuery(const Query& query) {
        if (query.type == "check") {
            // use reverse order, because we append strings to the end
            for (int i = static_cast<int>(elems.size()) - 1; i >= 0; --i)
                if (hash_func(elems[i]) == query.ind)
                    std::cout << elems[i] << " ";
            std::cout << "\n";
        } else {
            vector<string>::iterator it = std::find(elems.begin(), elems.end(), query.s);
            if (query.type == "find")
                writeSearchResult(it != elems.end());
            else if (query.type == "add") {
                if (it == elems.end())
                    elems.push_back(query.s);
            } else if (query.type == "del") {
                if (it != elems.end())
                    elems.erase(it);
            }
        }
    }
    
    void processQueries() {
        int n;
        cin >> n;
        for (int i = 0; i < n; ++i)
            processQuery(readQuery());
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    int bucket_count;
    cin >> bucket_count;
    QueryProcessor proc(bucket_count);
    proc.processQueries();
    return 0;
}


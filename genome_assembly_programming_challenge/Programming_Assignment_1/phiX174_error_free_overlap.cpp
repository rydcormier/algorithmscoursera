//
//  phiX174_error_free_overlap.cpp
//
//  Created by Ryan Cormier on 5/22/20.
//

#include <cassert>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

#define NUM_READS 1168
#define READ_SIZE 100


bool share_substring(const string&, const string&, size_t);

size_t overlap(const string&, const string&);

int main()
{
    vector<string> reads(NUM_READS);
    
    for (int i = 0; i < NUM_READS; i++)
        cin >> reads[i];
    
//    vector< vector<size_t>> adj(NUM_READS);
//    
//    for (int i = 0; i < NUM_READS - 1; i++)
//    {
//        for (int j = i + 1; j < NUM_READS; j++)
//        {
//            string a(reads[i]);
//            string b(reads[j]);
//            
//            if (share_substring(a, b, 12))
//            {
//                adj[i].push_back(j);
//                adj[j].push_back(i);
//            }
//        }
//    }
    
    vector<bool> visited(NUM_READS, 0);
    string res(reads[0]);
    size_t next_read;
    size_t weight;
    size_t cnt = 0;
    size_t read = 0;
    while (cnt < NUM_READS)
    {
        cnt++;
        visited[read] = 1;
        weight = 0;
        
        for (int v = 0; v < NUM_READS); v++)
        {
            
            if (! visited[v])
            {
                size_t d = overlap(reads[read], reads[v]);
                
                if (d > weight)
                {
                    next_read = v;
                    weight = d;
                }
            }
        }
        
        res += reads[next_read].substr(weight);
        read = next_read;
        
    }
    
    size_t pos = res.length() - overlap(reads[read], reads[0]);
    res.erase(pos);
    
    cout << res << endl;
    
    return 0;
}


bool share_substring(const string& a, const string& b, size_t k)
{
    for (size_t pos = 0; pos < READ_SIZE - k; pos++)
    {
        if (b.find(a.substr(pos, k)) != string::npos)
            return true;
    }
    
    return false;
}

size_t overlap(const string& a, const string& b)
{
    for (size_t pos = 0; pos < READ_SIZE; pos++)
    {
        if (a.substr(pos) == b.substr(0, READ_SIZE - pos))
            return READ_SIZE - pos;
    }
    
    return 0;
}















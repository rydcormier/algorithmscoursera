//
//  suffix_array_matching.cpp
//  Algorithms on Strings - Programming Assignment 3
//
//  Created by Ryan Cormier on 5/22/19.
//

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::string;
using std::vector;

const size_t NUM_CHARS = 5;

size_t get_char_index(char c)
{
    switch (c)
    {
        case 'A':
            return 1;
            break;

        case 'C':
            return 2;
            break;

        case 'G':
            return 3;
            break;

        case 'T':
            return 4;
            break;

        default:
            return 0;
            break;
    }
}

// A stable sort returning indeces of text lexicographically ordered.
vector<size_t> sort_characters(const string& text)
{
    vector<size_t> order(text.length());
    vector<size_t> count(NUM_CHARS);

    for (size_t i = 0; i < text.length(); ++i)
    {
        count[get_char_index(text[i])] += 1;
    }

    for (size_t j = 1; j < NUM_CHARS; ++j)
    {
        count[j] = count[j] + count[j - 1];
    }

    size_t i = text.length();

    while (i > 0)
    {
        --i;
        size_t c = get_char_index(text[i]);
        count[c] -= 1;
        order[count[c]] = i;
    }

    return order;
}

// Returns the equivalence classes of the characters of text.
vector<size_t> compute_character_classes(const string& text,
                                         const vector<size_t>& order)
{
    vector<size_t> classes(text.length());
    classes[order[0]] = 0;
    size_t current_class = 0;

    for (size_t i = 1; i < text.length(); ++i)
    {
        if (text[order[i]] != text[order[i - 1]])
        {
            ++current_class;
        }

        classes[order[i]] =  current_class;
    }

    return classes;
}

// Given ordered partial cyclic shifts, sort shifts of doubled length.
vector<size_t> sort_doubled(const string& text,
                            size_t length,
                            const vector<size_t>& order,
                            const vector<size_t>& classes)
{
    size_t s = text.length();
    vector<size_t> new_order(s);
    vector<size_t> count(s, 0);

    for (size_t i = 0; i < s; ++i)
    {
        count[classes[i]] += 1;
    }

    for (size_t j = 1; j < s; ++j)
    {
        count[j] += count[j - 1];
    }

    size_t i = s;

    while (i > 0)
    {
        --i;
        size_t start = (order[i] - length + s) % s;
        size_t cl = classes[start];
        count[cl] -= 1;
        new_order[count[cl]] = start;
    }

    return new_order;
}

// Update the equivalence classes of ordered cyclic shifts.
vector<size_t> update_classes(const vector<size_t>& order,
                              const vector<size_t> classes, size_t length)
{
    size_t n = order.size();
    vector<size_t> new_classes(n);
    new_classes[order[0]] = 0;

    for (size_t i = 1; i < n; ++i)
    {
        size_t cur = order[i];
        size_t prev = order[i - 1];
        size_t mid = (cur + length) % n;
        size_t mid_prev = (prev + length) % n;

        if (classes[cur] != classes[prev] || classes[mid] != classes[mid_prev])
        {
            new_classes[cur] = new_classes[prev] + 1;
        }
        else
        {
            new_classes[cur] = new_classes[prev];
        }
    }

    return new_classes;
}

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<size_t> BuildSuffixArray(const string& text)
{
    vector<size_t> order = sort_characters(text);
    vector<size_t> classes = compute_character_classes(text, order);
    size_t l = 1;

    while (l < text.length())
    {
        order = sort_doubled(text, l, order, classes);
        classes = update_classes(order, classes, l);
        l = 2 * l;
    }

    return order;
}

vector<size_t> FindOccurrences(const string& pattern, const string& text,
                               const vector<size_t>& suffix_array)
{
    vector<size_t> result;
    size_t min_index = 0;
    size_t max_index = text.length();

    while (min_index < max_index)
    {
        size_t mid_index = (min_index + max_index) / 2;

        if (pattern > text.substr(suffix_array[mid_index]))
        {
            min_index = mid_index + 1;
        }
        else
        {
            max_index = mid_index;
        }
    }

    size_t start = min_index;
    max_index = text.length();

    while (min_index < max_index)
    {
        size_t mid_index = (min_index + max_index) / 2;

        if (pattern < text.substr(suffix_array[mid_index]))
        {
            max_index = mid_index;
        }
        else
        {
            min_index = mid_index + 1;
        }
    }

    size_t end = max_index;

    for (size_t i = start; i <= end; ++i)
    {
        result.push_back(i);
    }

    return result;
}

int main()
{
    char buffer[100001];
    scanf("%s", buffer);

    string text = buffer;
    text += '$';
    vector<size_t> suffix_array = BuildSuffixArray(text);
    int pattern_count;

    scanf("%d", &pattern_count);

    vector<bool> occurs(text.length(), false);

    for (int pattern_index = 0; pattern_index < pattern_count; ++pattern_index)
    {
        scanf("%s", buffer);
        string pattern = buffer;
        vector<size_t> occurrences = FindOccurrences(pattern, text, suffix_array);

        for (int j = 0; j < occurrences.size(); ++j)
        {
            occurs[occurrences[j]] = true;
        }
    }

    for (int i = 0; i < occurs.size(); ++i)
    {
        if (occurs[i]) {
            printf("%d ", i);
        }
    }

    printf("\n");
    return 0;
}

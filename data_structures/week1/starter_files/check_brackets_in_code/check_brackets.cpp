#include <iostream>
#include <stack>
#include <string>

struct Bracket {
    Bracket(char type, int position):
        type(type),
        position(position)
    {}

    bool Matchc(char c) {
        if (type == '[' && c == ']')
            return true;
        if (type == '{' && c == '}')
            return true;
        if (type == '(' && c == ')')
            return true;
        return false;
    }

    char type;
    int position;
};

int main() {
    std::string text;
    getline(std::cin, text);
    Bracket *open_bracket(NULL);
    std::stack <Bracket> opening_brackets_stack;
    bool balanced = true;
    for (int position = 0; position < text.length(); ++position) {
        char next = text[position];

        if (next == '(' || next == '[' || next == '{') {
            // create Bracket and add to stack
            open_bracket = new Bracket(next, position);
            opening_brackets_stack.push(*open_bracket);

            continue;
        }

        if (next == ')' || next == ']' || next == '}') {
            // empty stack means unbalanced
            if (opening_brackets_stack.empty()) {
                std::cout << position + 1 << std::endl;
                return;
            }

            // Get last open bracket from stack and see if it is a match
            open_bracket = &opening_brackets_stack.top();
            if (! open_bracket->Matchc(next)) {
                balanced = false;
                break;
            }
            opening_brackets_stack.pop();
        }
    }

    // Printing answer, write your code here
    std::cout << "Success" << std::endl;

    return 0;
}

#include <iostream>
#include <stack>
#include <string>

bool DEBUG = false;

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

void print_stack(std::stack<Bracket> s) {
    bool started(false);
    while (!s.empty()) {
        if (started) {
            std::cout << ", ";
            started = true;
        } else {
            std::cout << "{ ";
        }
        std::cout << "'" << s.top().type << "'";
        s.pop();
    }
    std::cout << " }" << std::endl;
}

int main() {
    std::string text;
    getline(std::cin, text);
    Bracket *open_bracket(NULL);
    std::stack <Bracket> opening_brackets_stack;
    std::string res("Success");
    if (DEBUG) std::cout << text << std::endl;
    for (int position = 0; position < text.length(); ++position) {
        char next = text[position];
        if (DEBUG) {
            std::cout << "pos: " << position << "   next: " << next << '\n';
            print_stack(opening_brackets_stack);
        }
        if (next == '(' || next == '[' || next == '{') {
            // create Bracket and add to stack
            opening_brackets_stack.emplace(next, position + 1);
            if (DEBUG) print_stack(opening_brackets_stack);
            continue;
        }

        if (next == ')' || next == ']' || next == '}') {
            // empty stack means unbalanced - extra closing
            if (opening_brackets_stack.empty()) {
                res = std::to_string(position + 1);
                if (DEBUG) print_stack(opening_brackets_stack);
                break;
            }

            // Get last open bracket from stack and see if it is a match
            open_bracket = &opening_brackets_stack.top();
            if (DEBUG) std::cout << "Top of stack: " << open_bracket->type << '\n';
            if (! open_bracket->Matchc(next)) {  // extra closing
                res = std::to_string(position + 1);
                if (DEBUG) print_stack(opening_brackets_stack);
                break;
            }
            opening_brackets_stack.pop();
            if (DEBUG) print_stack(opening_brackets_stack);
        }
    }
    
    // If stack is non-empty, there's an extra opening
    if (res == "Success" && !opening_brackets_stack.empty()) {
        res = std::to_string(opening_brackets_stack.top().position);
    }
    
    // Printing answer, write your code here
    std::cout << res << std::endl;

    return 0;
}

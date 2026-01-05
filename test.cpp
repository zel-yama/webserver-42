#include <cctype>
#include <iostream>

int main() {
    char c = '\v';

    if (isspace(c))
        std::cout << "Whitespace\n";
    else
        std::cout << "Not whitespace\n";
}

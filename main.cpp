#include <iostream>
#include <stdexcept>
#include <string>

#include <cstdlib>

#include <readline/history.h>
#include <readline/readline.h>

#include "parser.hpp"

auto readline_string(char const * const prompt) -> std::string const;

auto readline_string(char const * const prompt) -> std::string const {
    char * const input = readline(prompt);
    if (input == nullptr) throw std::runtime_error("Got EOF");
    auto const result = std::string(input) + '\n';
    add_history(input);
    std::free(input);
    return result;
}

int main(void) {
    clumsy_parser<std::string::const_iterator> parser;
    std::string input;
    decltype(parser)::result_type result;

    while (true) {
        try {
            input.append(readline_string(input.size() == 0 ? ">>> " : "... "));
        } catch (std::runtime_error e) {
            break;
        }

        auto const feeded = parser.parse(input.begin(), input.end(), result);
        if (feeded != input.cend()) continue;
        std::cout << result << std::endl;

        input.erase(input.cbegin(), input.cend());
        result.erase(result.begin(), result.end());
    }
}

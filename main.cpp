#include <iostream>
#include <stdexcept>
#include <string>

#include <cstdlib>

#include <boost/optional.hpp>
#include <readline/history.h>
#include <readline/readline.h>

#include "parser.hpp"
#include "eval.hpp"

auto readline_string(char const * const prompt) -> boost::optional<std::string> const;

auto readline_string(char const * const prompt) -> boost::optional<std::string> const {
    auto const input = readline(prompt);
    if (input == nullptr) return boost::none;
    auto result = std::string(input);
    add_history(input);
    std::free(input);
    return result;
}

int main(void) {
    clumsy_parser<std::string::const_iterator> parser;
    std::string input;

    while (true) {
        auto const line = readline_string(input.size() == 0 ? ">>> " : "... ");
        if (!line) break;
        input.append(line.get() + '\n');

        auto const result = parser.parse(input.cbegin(), input.cend());
        if (!result) continue;
        std::cout << result.get() << std::endl;
        input.erase(input.begin(), input.end());
    }
}

// vim: set ts=4 sw=4 et:

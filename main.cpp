#include <iostream>
#include <stdexcept>
#include <string>

#include <cstdlib>

#include <boost/optional.hpp>
#include <readline/history.h>
#include <readline/readline.h>

#include "parser.hpp"

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
    decltype(parser)::result_type result;

    while (true) {
        auto const line = readline_string(input.size() == 0 ? ">>> " : "... ");
        if (!line) break;
        input.append(line.get() + '\n');

        auto const feeded = parser.parse(input.begin(), input.cend(), result);
        if (feeded != input.cend()) continue;
        std::cout << result << std::endl;

        input.erase(input.cbegin(), input.cend());
        result.erase(result.begin(), result.end());
    }
}

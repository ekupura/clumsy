#ifndef PARSER_HPP
#define PARSER_HPP

#include <stdexcept>

#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;

class syntax_error : public std::runtime_error { };

template <typename Iterator>
struct skipper_grammar : qi::grammar<Iterator> {
    skipper_grammar() : skipper_grammar::base_type(start) {
        start = qi::ascii::space | comment;
        comment = ';' >> *qi::char_ >> qi::eol;
    }

    qi::rule<Iterator> start;
    qi::rule<Iterator> comment;
};

template <typename Iterator, typename Skipper = skipper_grammar<Iterator>>
struct clumsy_grammar : qi::grammar<Iterator, Skipper> {
    clumsy_grammar() : clumsy_grammar::base_type(start) {
        start      = *expression;
        expression = identifier | ( '(' >> ( lambda | define | apply | identifier ) >> ')' );
        identifier = qi::lexeme[qi::alpha >> *qi::alnum];
        lambda     = "lambda" >> identifier >> expression;
        define     = "define" >> identifier >> expression;
        apply      = expression >> expression;
    }

    qi::rule<Iterator, Skipper> start;
    qi::rule<Iterator, Skipper> expression;
    qi::rule<Iterator, Skipper> identifier;
    qi::rule<Iterator, Skipper> lambda;
    qi::rule<Iterator, Skipper> define;
    qi::rule<Iterator, Skipper> apply;
};


template <typename Iterator>
struct clumsy_parser {
    auto parse(Iterator begin, Iterator end) {
        Iterator const former_begin = begin;

        auto const succeed = qi::phrase_parse(begin, end, grammar, skipper);
        if (!succeed) return former_begin;
        return begin;
    }

    clumsy_grammar<Iterator> grammar;
    skipper_grammar<Iterator> skipper;
};


#endif // PARSER_HPP

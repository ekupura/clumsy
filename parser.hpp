#ifndef PARSER_HPP
#define PARSER_HPP

#include <stdexcept>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_utree.hpp>

namespace spirit = boost::spirit;
namespace qi = spirit::qi;

template <typename Iterator>
struct skipper_grammar : qi::grammar<Iterator> {
    skipper_grammar() : skipper_grammar::base_type(start) {
        start = qi::ascii::space | comment;
        comment = ';' >> *(qi::char_ - qi::eol) >> qi::eol;
    }

    qi::rule<Iterator> start;
    qi::rule<Iterator> comment;
};

template <typename Iterator, typename Result = spirit::utree::list_type(), typename Skipper = skipper_grammar<Iterator>>
struct clumsy_grammar : qi::grammar<Iterator, Result, Skipper> {
    clumsy_grammar() : clumsy_grammar::base_type(start) {
        start      = *expression;
        expression = identifier | ( '(' >> ( lambda | define | apply | identifier ) >> ')' );
        identifier = qi::lexeme[qi::alpha >> *qi::alnum];
        lambda     = "lambda" >> identifier >> expression;
        define     = "define" >> identifier >> expression;
        apply      = expression >> expression;
    }

    qi::rule<Iterator, Result, Skipper> start;
    qi::rule<Iterator, Result, Skipper> expression;
    qi::rule<Iterator, Result, Skipper> identifier;
    qi::rule<Iterator, Result, Skipper> lambda;
    qi::rule<Iterator, Result, Skipper> define;
    qi::rule<Iterator, Result, Skipper> apply;
};


template <typename Iterator, typename Result>
struct clumsy_parser {
    auto parse(Iterator begin, Iterator end, Result & result) {
        Iterator const former_begin = begin;

        auto const succeed = qi::phrase_parse(begin, end, grammar, skipper, result);
        if (!succeed) return former_begin;
        return begin;
    }

    clumsy_grammar<Iterator> grammar;
    skipper_grammar<Iterator> skipper;
};

#endif  // PARSER_HPP

#ifndef PARSER_HPP
#define PARSER_HPP

#include <boost/optional.hpp>
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
    using result_type = Result;

    clumsy_grammar() : clumsy_grammar::base_type(start) {
        // program is a list of definitions or expressions
        start      = *( define | expression );

        // identifier is a continuous sequence of alphabet or number
        identifier = qi::as_string[qi::lexeme[+qi::alnum]];

        // expression is an identifier, function expression or function application, or a parenthesized expression
        expression = identifier | lambda | apply | '(' >> expression >> ')';

        // definition is a parenthesized pair of identifier and expression following 'define' keyword
        define     = '(' >> qi::lit("define") >> identifier >> expression >> ')';

        // function expression is a parenthesized pair of patameters and expression following 'lambda' keyword
        lambda     = '(' >> qi::lit("lambda") >> parameters >> expression >> ')';

        // function application is a parenthesized pair of expression and arguments
        apply      = '(' >>                      expression >> arguments  >> ')';

        // parameters is an identifier or parenthesized list of identifiers
        parameters = identifier | '(' >> +identifier >> ')';

        // arguments is list of expression
        arguments  = +expression;
    }

    qi::rule<Iterator, result_type, Skipper> start;
    qi::rule<Iterator, spirit::utree(), Skipper> identifier;
    qi::rule<Iterator, spirit::utree(), Skipper> expression;
    qi::rule<Iterator, spirit::utree::list_type(), Skipper> define;
    qi::rule<Iterator, spirit::utree::list_type(), Skipper> lambda;
    qi::rule<Iterator, spirit::utree::list_type(), Skipper> apply;
    qi::rule<Iterator, spirit::utree::list_type(), Skipper> parameters;
    qi::rule<Iterator, spirit::utree::list_type(), Skipper> arguments;
};


template <typename Iterator>
struct clumsy_parser {
    using result_type = spirit::utree;

    auto parse(Iterator begin, Iterator const & end) -> boost::optional<result_type> const {
        result_type result;

        auto const succeed = qi::phrase_parse(begin, end, grammar, skipper, result);
        if (!succeed || begin != end) return boost::none;

        return result;
    }

    clumsy_grammar<Iterator> grammar;
    skipper_grammar<Iterator> skipper;
};

#endif  // PARSER_HPP
// vim: set ts=4 sw=4 et:

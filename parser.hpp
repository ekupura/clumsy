#ifndef PARSER_HPP
#define PARSER_HPP

#include <stdexcept>

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
    using identifier_type = spirit::utree();

    clumsy_grammar() : clumsy_grammar::base_type(start) {
        start      = *list;
        list       = '(' >> +(list | identifier) >> ')';
        identifier = qi::as_string[+qi::alnum];
    }

    qi::rule<Iterator, result_type, Skipper> start;
    qi::rule<Iterator, result_type, Skipper> list;
    qi::rule<Iterator, identifier_type> identifier;
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

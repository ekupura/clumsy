#ifndef EVAL_HPP
#define EVAL_HPP

#include <boost/spirit/include/support_utree.hpp>

namespace spirit = boost::spirit;

struct env_type { };

struct evaluator {
    auto eval(spirit::utree::list_type const & tree, env_type & env) {
    }

    template <typename T>
    auto eval(T const & tree) {
        return eval(tree, global_env);
    }

    env_type global_env;
};

#endif  // EVAL_HPP
// vim: set ts=4 sw=4 et:

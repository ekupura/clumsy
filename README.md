# clumsy
A tiny and clumsy Lisp-flavored programming language.

## Examples

    (\x x)                       # => (\x x)
    ((\x x) a)                   # => a
    ((\x x) (\y y))              # => (\y y)
    (((\x (\y (x y)) (\z z)) a)  # => a

## Syntax

- Expressions
    - Function: `(` `\` [parameter] [expression] `)`
    - FunctionApplication: `(` [function] [expression] `)`
    - Variable or Symbol: other characters
        - A Variable that is not defined in any outer scope is assumed as a Symbol.

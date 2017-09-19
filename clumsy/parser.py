class FunctionExpression:
    def __init__(self, parameter, body):
        self.parameter = parameter
        self.body = body

    def __repr__(self):
        return fr'(\{self.parameter} {self.body})'


class FunctionApplication:
    def __init__(self, function, argument):
        self.function = function
        self.argument = argument

    def __repr__(self):
        return fr'({self.function} {self.argument})'


def parse(tokens):
    return read_from_tokens(tokens)


def read_from_tokens(tokens):
    if len(tokens) == 0:
        raise SyntaxError('unexpected EOF')

    token = tokens.pop(0)
    if token == '(':
        result = []
        while tokens[0] != ')':
            result.append(read_from_tokens(tokens))
        tokens.pop(0)

        if result[0] == '\\':
            return FunctionExpression(*result[1:])
        else:
            return FunctionApplication(*result)
    elif token == ')':
        raise SyntaxError('unexpected )')
    else:
        return token

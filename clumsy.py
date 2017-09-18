class Scope(dict):
    def __init__(self, parameter=None, argument=None, outer=None):
        if parameter is not None:
            self[parameter] = argument
        self.outer = outer

    def find(self, variable):
        if variable in self:
            return self
        elif self.outer is None:
            return None
        else:
            return self.outer.find(variable)


class Function:
    def __init__(self, parameter, body, scope):
        self.parameter, self.body, self.scope = parameter, body, scope

    def call(self, argument):
        return evaluate(self.body, Scope(self.parameter, argument, self.scope))

    def __repr__(self):
        return fr'(\{self.parameter} {self.body})'


global_scope = Scope()


def parse(program):
    return read_from_tokens(tokenize(program))


def tokenize(chars):
    return (chars.replace('\\', ' \\ ')
                 .replace('(', ' ( ')
                 .replace(')', ' ) ')
                 .split())


def read_from_tokens(tokens):
    if len(tokens) == 0:
        raise SyntaxError('unexpected EOF')

    token = tokens.pop(0)
    if token == '(':
        result = []
        while tokens[0] != ')':
            result.append(read_from_tokens(tokens))
        tokens.pop(0)
        return result
    elif token == ')':
        raise SyntaxError('unexpected )')
    else:
        return token


def evaluate(x, scope=global_scope):
    if isinstance(x, str):
        found = scope.find(x)
        if found is None:
            return x
        else:
            return found[x]
    elif x[0] == '\\':
        _, parameter, body = x
        return Function(parameter, body, scope)
    else:
        function = evaluate(x[0], scope)
        argument = evaluate(x[1], scope)
        return function.call(argument)

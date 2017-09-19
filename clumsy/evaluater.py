from . import parser


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


def evaluate(ast, scope=global_scope):
    if isinstance(ast, str):
        found = scope.find(ast)
        if found is None:
            return ast
        else:
            return found[ast]

    elif isinstance(ast, parser.FunctionExpression):
        return Function(ast.parameter, ast.body, scope)

    elif isinstance(ast, parser.FunctionApplication):
        function = evaluate(ast.function, scope)
        argument = evaluate(ast.argument, scope)
        return function.call(argument)

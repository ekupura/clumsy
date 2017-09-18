#!/usr/bin/env python
import cmd
import sys
from clumsy import evaluate, parse


class Cmd(cmd.Cmd):
    prompt = '> '

    def cmdloop(self):
        while True:
            try:
                super().cmdloop()
            except KeyboardInterrupt:
                print()

    def default(self, line):
        print(evaluate(parse(line)))

    def do_help(self, arg):
        self.default('help ' + arg)

    def do_EOF(self, arg):
        sys.exit()

    def emptyline(self):
        pass


if __name__ == '__main__':
    Cmd().cmdloop()

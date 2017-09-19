def tokenize(chars):
    return (chars.replace('\\', ' \\ ')
                 .replace('(', ' ( ')
                 .replace(')', ' ) ')
                 .split())

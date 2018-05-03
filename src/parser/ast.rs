#[derive(Debug, PartialEq)]
pub enum Expression {
    Abstraction {
        parameter: Variable,
        expression: Box<Expression>,
    },
    Application {
        callee: Box<Expression>,
        argument: Box<Expression>,
    },
    Variable(Variable),
}

#[derive(Debug, PartialEq, Eq, Hash)]
pub struct Variable(pub String);

impl Variable {
    pub fn new(variable: &str) -> Variable {
        Variable(variable.to_owned())
    }
}
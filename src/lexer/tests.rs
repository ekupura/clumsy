use super::*;

#[test]
fn it_accepts_valid_lambda_expression() {
    let result = Lexer::new("((\\x y. x y) (\\z. z)) a").collect::<Vec<_>>();
    assert_eq!(
        result,
        vec![
            Token::LeftBracket,
            Token::LeftBracket,
            Token::Lambda,
            Token::Variable("x".to_string()),
            Token::Variable("y".to_string()),
            Token::Dot,
            Token::Variable("x".to_string()),
            Token::Variable("y".to_string()),
            Token::RightBracket,
            Token::LeftBracket,
            Token::Lambda,
            Token::Variable("z".to_string()),
            Token::Dot,
            Token::Variable("z".to_string()),
            Token::RightBracket,
            Token::RightBracket,
            Token::Variable("a".to_string()),
        ]
    );
}

#[test]
fn it_rejects_invalid_characters() {
    let result = Lexer::new("this = !valid [I think].").collect::<Vec<_>>();
    assert_eq!(
        result,
        vec![
            Token::Variable("this".to_string()),
            Token::InvalidCharacter('='),
            Token::InvalidCharacter('!'),
            Token::Variable("valid".to_string()),
            Token::InvalidCharacter('['),
            Token::Variable("I".to_string()),
            Token::Variable("think".to_string()),
            Token::InvalidCharacter(']'),
            Token::Dot,
        ]
    );
}

#[test]
fn it_accepts_and_ignores_very_long_white_spaces() {
    let mut source = String::new();
    for _ in 0..100_000 {
        source.push(' ');
    }
    source.push_str("\\x. x");
    Lexer::new(&source).count();
}

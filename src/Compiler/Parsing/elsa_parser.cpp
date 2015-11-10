#include "elsa_parser.h"

namespace elsa {
	namespace compiler {

		ElsaParser::ElsaParser(Lexer* lexer)
			:
			lexer_(std::unique_ptr<Lexer>(lexer)),
			current_function_(nullptr)
		{
			initialize_grammar();
			next_token();
		}

		std::unique_ptr<Program> ElsaParser::parse()
		{
			auto program = std::make_unique<Program>(&function_table_, &struct_table_);

			while (current_token_->get_type() != TokenType::END)
			{
				program->add_statement(parse_statement());
			}

			return program;
		}

		std::unique_ptr<Expression> ElsaParser::parse_statement()
		{
			auto parser = get_statement_parser(current_token_->get_type());

			if (parser == nullptr)
				throw ParsingException("Invalid token");

			return parser->parse(this);
		}

		std::unique_ptr<Expression> ElsaParser::parse_expression()
		{
			return parse_expression(0);
		}

		std::unique_ptr<Expression> ElsaParser::parse_expression(int precedence)
		{
			auto parser = get_expression_parser(current_token_->get_type());

			if (parser == nullptr)
				throw ParsingException("Invalid token");

			auto left = std::unique_ptr<Expression>(parser->parse(this));

			while (precedence < get_precedence())
			{
				auto infix_parser = get_infix_parser(current_token()->get_type());
				left = std::unique_ptr<Expression>(infix_parser->parse(this, std::move(left)));
			}

			return left;
		}

		FunctionSymbol* ElsaParser::current_function()
		{
			return current_function_;
		}

		void ElsaParser::set_current_function(FunctionSymbol* function_symbol)
		{
			current_function_ = function_symbol;
		}

		StructTable& ElsaParser::struct_table()
		{
			return struct_table_;
		}

		FunctionTable& ElsaParser::function_table()
		{
			return function_table_;
		}

		void ElsaParser::consume(TokenType type)
		{
			if(current_token()->get_type() != type)
				throw ParsingException("Unexpected token");

			next_token();
		}

		void ElsaParser::consume()
		{
			next_token();
		}

		Token* ElsaParser::current_token()
		{
			return current_token_.get();
		}

		void ElsaParser::next_token()
		{
			current_token_ = std::unique_ptr<Token>(lexer_->next_token());
		}

		Parser* ElsaParser::get_expression_parser(TokenType type)
		{
			auto it = expression_parsers_.find(type);
			if (it != expression_parsers_.end())
			{
				return it->second.get();
			}

			return nullptr;
		}

		Parser* ElsaParser::get_statement_parser(TokenType type)
		{
			auto it = statement_parsers_.find(type);
			if (it != statement_parsers_.end())
			{
				return it->second.get();
			}

			return nullptr;
		}

		InfixParser* ElsaParser::get_infix_parser(TokenType type)
		{
			auto it = infix_parsers_.find(type);
			if (it != infix_parsers_.end())
			{
				return it->second.get();
			}

			return nullptr;
		}

		int ElsaParser::get_precedence()
		{
			auto infix_parser = get_infix_parser(current_token()->get_type());
			if (infix_parser != nullptr)
				return infix_parser->precedence();

			return 0;
		}

		void ElsaParser::register_expression_parser(TokenType type, Parser* parser)
		{
			expression_parsers_.insert(std::pair<TokenType, std::unique_ptr<Parser>>(type, std::unique_ptr<Parser>(parser)));
		}

		void ElsaParser::register_statement_parser(TokenType type, Parser* parser)
		{
			statement_parsers_.insert(std::pair<TokenType, std::unique_ptr<Parser>>(type, std::unique_ptr<Parser>(parser)));
		}

		void ElsaParser::register_infix_parser(TokenType type, InfixParser* parser)
		{
			infix_parsers_.insert(std::pair<TokenType, std::unique_ptr<InfixParser>>(type, std::unique_ptr<InfixParser>(parser)));
		}

		void ElsaParser::register_prefix_parser(TokenType type)
		{
			register_expression_parser(type, new PrefixOperatorParser());
		}

		void ElsaParser::initialize_grammar()
		{
			// Statements
			register_statement_parser(TokenType::Func, new FuncDeclarationParser());
			register_statement_parser(TokenType::Struct, new StructDeclarationParser());

			// Expressions
			register_expression_parser(TokenType::Identifier, new IdentifierParser());
			register_expression_parser(TokenType::Var, new VariableDeclarationParser());
			register_expression_parser(TokenType::IntegerLiteral, new LiteralParser());
			register_expression_parser(TokenType::FloatLiteral, new LiteralParser());
			register_expression_parser(TokenType::CharLiteral, new LiteralParser());
			register_expression_parser(TokenType::BoolLiteral, new LiteralParser());
			register_expression_parser(TokenType::StringLiteral, new LiteralParser());
			register_expression_parser(TokenType::Elsa, new ElsaInvokeParser());
			register_expression_parser(TokenType::New, new CreateStructParser());

			// Prefix
			register_prefix_parser(TokenType::Exclamation);

			// Infix
			register_infix_parser(TokenType::Plus, new BinaryOperatorParser(Precedence::Sum));
			register_infix_parser(TokenType::Minus, new BinaryOperatorParser(Precedence::Sum));
			register_infix_parser(TokenType::Slash, new BinaryOperatorParser(Precedence::Product));
			register_infix_parser(TokenType::Asterix, new BinaryOperatorParser(Precedence::Product));
		}
	}
}

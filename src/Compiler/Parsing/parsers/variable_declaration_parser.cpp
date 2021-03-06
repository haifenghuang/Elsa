#include "variable_declaration_parser.h"

namespace elsa {
	namespace compiler {

		std::unique_ptr<Expression> VariableDeclarationParser::parse(ElsaParser* parser)
		{
			std::unique_ptr<ElsaType> explicit_type;
			if (is_explicit(parser))
			{
				explicit_type = get_explicit_type(parser);
			}
			else
			{
				parser->consume(TokenType::Var);
			}

			auto name = parser->current_token()->get_value();

			if(parser->current_scope()->has_local(name))
				throw ParsingException(L"A local variable or parameter with the name '" + name + L"' has already been declared in the current scope", parser->current_token());

			if (parser->current_scope()->any_scope_has_local(name))
				throw ParsingException(L"A local variable or parameter with the name '" + name + L"' has already been declared in a parent scope", parser->current_token());

			if (parser->current_struct() != nullptr)
			{
				for (const auto& field : parser->current_struct()->get_fields())
				{
					if(field->get_name() == name)
						throw ParsingException(L"A field with the the name '" + name + L"' has already been declared", parser->current_token());
				}
			}

			parser->consume(TokenType::Identifier);
			parser->consume(TokenType::Equals);

			auto expression = std::unique_ptr<Expression>(parser->parse_expression());
			auto expression_type = std::unique_ptr<ElsaType>(parser->type_checker().get_expression_type(expression.get()));

			if (explicit_type.get() != nullptr)
			{
				if (!is_valid_declaration(explicit_type.get(), expression_type.get(), parser) && !TypeConverter::try_convert(expression, explicit_type.get(), &parser->type_checker()))
				{
					throw ParsingException(L"Both sides of a variable declaration expression must be of the same type.", parser->current_token());
				}
			}

			auto struct_expression = dynamic_cast<CreateStructExpression*>(expression.get());
			auto array_expression = dynamic_cast<ArrayDeclarationExpression*>(expression.get());
			if (struct_expression != nullptr || array_expression != nullptr)
			{
				auto entry_name = struct_expression != nullptr ? struct_expression->get_struct_name() : array_expression->get_type()->get_name();
				if (!parser->struct_table().has_entry(entry_name))
					throw ParsingException(L"No struct with the name '" + name + L"' is defined", parser->current_token());

				// Struct
				auto si = parser->struct_table().get(entry_name);
				parser->current_scope()->add_local(name, *expression_type, si->get_expression());
			}
			else
			{
				// Other types
				parser->current_scope()->add_local(name, *expression_type);
			}

			if(parser->current_token()->get_type() == TokenType::Semicolon)
				parser->consume(TokenType::Semicolon);

			return std::make_unique<VariableDeclarationExpression>(name, expression_type.release(), std::move(expression));
		}

		bool VariableDeclarationParser::is_explicit(ElsaParser* parser)
		{
			auto current_token_type = parser->current_token()->get_type();
			return current_token_type == TokenType::Int || 
				current_token_type == TokenType::Float || 
				current_token_type == TokenType::Char || 
				current_token_type == TokenType::Bool ||
				current_token_type == TokenType::Byte ||
				current_token_type == TokenType::Identifier;
		}

		std::unique_ptr<ElsaType> VariableDeclarationParser::get_explicit_type(ElsaParser* parser)
		{
			auto type = std::unique_ptr<ElsaType>(parser->type_checker().get_type_from_token(parser->current_token()));

			if (parser->current_token()->get_type() == TokenType::LSBracket)
			{
				auto array_struct = parser->struct_table().get(L"Array")->get_expression();
				auto array_type = array_struct->create_generic(std::move(type), parser);

				parser->consume(TokenType::LSBracket);
				parser->consume(TokenType::RSBracket);

				return std::make_unique<ElsaType>(array_type);
			}

			return type;
		}

		bool VariableDeclarationParser::is_valid_declaration(const ElsaType* expected, const ElsaType* actual, ElsaParser* parser)
		{
			if (expected->get_type() != actual->get_type())
				return false;

			if (expected->get_type() == ObjectType::GCOPtr)
			{
				if (actual->get_type() != ObjectType::GCOPtr)
					return false;
				else
				{
					if (expected->get_struct_declaration_expression()->get_name() != actual->get_struct_declaration_expression()->get_name())
						return false;
				}
			}

			return true;
		}

	}
}

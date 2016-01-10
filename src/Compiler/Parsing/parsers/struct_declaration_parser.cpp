#include "struct_declaration_parser.h"

namespace elsa {
	namespace compiler {

		std::unique_ptr<Expression> StructDeclarationParser::parse(ElsaParser* parser)
		{
			parser->consume(TokenType::Struct);

			auto struct_exp = std::make_unique<StructDeclarationExpression>();

			auto name = parser->current_token()->get_value();

			if (parser->struct_table().has_entry(name))
				throw ParsingException(L"A struct with the same name has already been declared", parser->current_token());

			struct_exp->set_name(name);

			parser->consume(TokenType::Identifier);
			parser->consume(TokenType::LBracket);

			while (parser->current_token()->get_type() != TokenType::RBracket)
			{
				struct_exp->add_field_expression(parse_field_expression(parser));
			}

			parser->consume(TokenType::RBracket);
			parser->consume(TokenType::Semicolon);

			parser->struct_table().add_struct(name, struct_exp.get());

			return std::move(struct_exp);
		}

		std::unique_ptr<FieldExpression> StructDeclarationParser::parse_field_expression(ElsaParser* parser)
		{
			auto field_type = std::unique_ptr<ElsaType>(parser->type_checker().get_type_from_token(parser->current_token()));

			parser->consume();

			auto field_expression = std::make_unique<FieldExpression>();

			if (parser->current_token()->get_type() == TokenType::LSBracket)
			{
				parser->consume(TokenType::LSBracket);
				parser->consume(TokenType::RSBracket);
				field_expression->set_is_array(true);

				auto array_struct = parser->struct_table().get(L"Array")->get_expression();
				auto array_type = array_struct->create_generic(std::move(field_type), parser);

				field_expression->set_type(new ElsaType(array_type));
			}
			else
			{
				field_expression->set_type(field_type.release());
			}

			auto field_name = parser->current_token()->get_value();
			parser->consume(TokenType::Identifier);

			field_expression->set_name(field_name);

			parser->consume(TokenType::Semicolon);

			return std::move(field_expression);
		}

	}
}
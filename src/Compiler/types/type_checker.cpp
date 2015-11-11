#include "type_checker.h"

#include "../parsing/elsa_parser.h"

namespace elsa {
	namespace compiler {
		TypeChecker::TypeChecker(ElsaParser* parser)
			:
			parser_(parser)
		{
		}

		ElsaType* TypeChecker::get_expression_type(Expression* expression)
		{
			if (is_of_type<BinaryOperatorExpression>(expression))
			{
				auto be = static_cast<BinaryOperatorExpression*>(expression);
				auto left_type = std::unique_ptr<ElsaType>(get_expression_type(be->get_left()));
				auto right_type = std::unique_ptr<ElsaType>(get_expression_type(be->get_right()));

				if (left_type->get_type() == OType::Int && right_type->get_type() == OType::Int)
					return new ElsaType(OType::Int);

				if (left_type->get_type() == OType::Float && right_type->get_type() == OType::Float)
					return new ElsaType(OType::Float);

				if (left_type->get_type() == OType::Bool && right_type->get_type() == OType::Bool)
					return new ElsaType(OType::Bool);

				throw ParsingException("Type mismatch");
			}
			if (is_of_type<IdentifierExpression>(expression))
			{
				auto id = static_cast<IdentifierExpression*>(expression);
				auto local = parser_->current_scope()->get_local(id->get_name());

				if (local == nullptr)
					throw ParsingException("Unkown identifier");

				if (local->get_struct_expression() != nullptr)
				{
					return new ElsaType(local->get_struct_expression());
				}
				else
				{
					return new ElsaType(local->get_type());
				}
			}
			if (is_of_type<PrefixOperatorExpression>(expression))
			{
				auto poe = static_cast<PrefixOperatorExpression*>(expression);
				return get_expression_type(poe->get_right());
			}
			if (is_of_type<IntegerLiteralExpression>(expression))
			{
				return new ElsaType(OType::Int);
			}
			if (is_of_type<FloatLiteralExpression>(expression))
			{
				return new ElsaType(OType::Float);
			}
			if (is_of_type<CharLiteralExpression>(expression))
			{
				return new ElsaType(OType::Char);
			}
			if (is_of_type<BoolLiteralExpression>(expression))
			{
				return new ElsaType(OType::Bool);
			}
			if (is_of_type<CreateStructExpression>(expression))
			{
				return new ElsaType(OType::UserDefined);
			}

			throw ParsingException("Unkown expression type.");
		}

		ElsaType* TypeChecker::get_type_from_token(TokenType type)
		{
			switch (type)
			{
			case TokenType::Void: {
				return new ElsaType(OType::Void);
			}
			case TokenType::Int: {
				return new ElsaType(OType::Int);
			}
			case TokenType::Float: {
				return new ElsaType(OType::Float);
			}
			case TokenType::Char: {
				return new ElsaType(OType::Char);
			}
			case TokenType::Bool: {
				return new ElsaType(OType::Bool);
			}
			case TokenType::IntegerLiteral: {
				return new ElsaType(OType::Int);
			}
			case TokenType::FloatLiteral: {
				return new ElsaType(OType::Float);
			}
			case TokenType::CharLiteral: {
				return new ElsaType(OType::Char);
			}
			case TokenType::BoolLiteral: {
				return new ElsaType(OType::Bool);
			}
			default:
				throw ParsingException("Invalid type.");
			}
		}

		void TypeChecker::assert_is_same_type(OType t1, OType t2)
		{
			if (t1 != t2)	
				throw ParsingException("Type mismatch");
		}

		ElsaType* TypeChecker::get_field_type(const StructDeclarationExpression* struct_expression, const IdentifierExpression * field)
		{
			if(struct_expression == nullptr)
				throw ParsingException("The StructDeclarationExpression can not be a nullptr");

			for (const auto& field : struct_expression->get_fields())
			{
				if (field->get_name() == field->get_name())
					return new ElsaType(field->get_type());
			}

			throw ParsingException("Invalid struct field");
		}

		ElsaType* TypeChecker::get_struct_type(const std::wstring & name)
		{
			if (parser_->struct_table().has_entry(name))
			{
				auto si = parser_->struct_table().get(name);
				return new ElsaType(si->get_expression());
			}

			throw ParsingException("Invalid struct name");
		}

		template<typename TExpression>
		bool TypeChecker::is_of_type(Expression* exp)
		{
			return dynamic_cast<TExpression*>(exp) != nullptr;
		}
	}
}

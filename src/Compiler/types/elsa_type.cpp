#include "elsa_type.h"

#include "../ast/struct_declaration_expression.h"
#include "../ast/func_declaration_expression.h"
#include "../ast/enum_declaration_expression.h"

namespace elsa {
	namespace compiler {

		ElsaType::ElsaType(const ElsaType* type)
			:
			type_(type->get_type()),
			struct_declaration_expression_(type->get_struct_declaration_expression()),
			func_declaration_expression_(type->get_func_declaration_expression()),
			enum_declaration_expression_(type->get_enum_declaration_expression()),
			is_array_(type->get_is_array())
		{
			assert_is_valid();
		}

		ElsaType::ElsaType(const ElsaType* type, bool is_array)
			:
			type_(type->get_type()),
			struct_declaration_expression_(type->get_struct_declaration_expression()),
			func_declaration_expression_(type->get_func_declaration_expression()),
			enum_declaration_expression_(type->get_enum_declaration_expression()),
			is_array_(is_array)
		{
			assert_is_valid();
		}

		ElsaType::ElsaType(ObjectType type)
			:
			type_(type),
			struct_declaration_expression_(nullptr),
			func_declaration_expression_(nullptr),
			enum_declaration_expression_(nullptr),
			is_array_(false)

		{
			assert_is_valid();
		}

		ElsaType::ElsaType(const StructDeclarationExpression* struct_declaration_expression)
			:
			type_(ObjectType::GCOPtr),
			struct_declaration_expression_(struct_declaration_expression),
			func_declaration_expression_(nullptr),
			is_array_(false)
		{
			assert_is_valid();
		}

		ElsaType::ElsaType(const FuncDeclarationExpression* func_declaration_expression)
			:
			type_(ObjectType::Function),
			func_declaration_expression_(func_declaration_expression),
			struct_declaration_expression_(nullptr),
			is_array_(false)
		{
			assert_is_valid();
		}

		ElsaType::ElsaType(const EnumDeclarationExpression* enum_declaration_expression)
			:
			type_(ObjectType::Enum),
			func_declaration_expression_(nullptr),
			struct_declaration_expression_(nullptr),
			enum_declaration_expression_(enum_declaration_expression),
			is_array_(false)
		{
			assert_is_valid();
		}

		ObjectType ElsaType::get_type() const
		{
			return type_;
		}

		VMType ElsaType::get_vm_type() const
		{
			switch (type_)
			{
			case ObjectType::Bool:
			case ObjectType::Int:
			case ObjectType::Enum: {
				return VMType::Int;
			}
			case ObjectType::Float: {
				return VMType::Float;
			}
			case ObjectType::Char: {
				return VMType::Char;
			}
			case ObjectType::Byte: {
				return VMType::Byte;
			}
			case ObjectType::GCOPtr: {
				return VMType::GCOPtr;
			}
			case ObjectType::Function: {
				return VMType::Function;
			}
			default:
				throw ParsingException("Unsupported vm type.");
			}
		}

		std::wstring ElsaType::get_name() const
		{
			switch (type_)
			{
			case ObjectType::Void: {
				return L"void";
			}
			case ObjectType::Bool: {
				return L"bool";
			}
			case ObjectType::Int: {
				return L"int";
			}
			case ObjectType::Float: {
				return L"float";
			}
			case ObjectType::Char: {
				return L"char";
			}
			case ObjectType::Byte: {
				return L"byte";
			}
			case ObjectType::GCOPtr: {
				return struct_declaration_expression_->get_name();
			}
			case ObjectType::Function: {
				return func_declaration_expression_->get_type_name();
			}
			case ObjectType::Enum: {
				return enum_declaration_expression_->get_name();
			}
			default:
				throw ParsingException("ElsaType::get_name: Can not get the type name.");
			}
		}

		const StructDeclarationExpression* ElsaType::get_struct_declaration_expression() const
		{
			return struct_declaration_expression_;
		}

		const FuncDeclarationExpression* ElsaType::get_func_declaration_expression() const
		{
			return func_declaration_expression_;
		}

		const EnumDeclarationExpression* ElsaType::get_enum_declaration_expression() const
		{
			return enum_declaration_expression_;
		}

		void ElsaType::set_is_array(bool is_array)
		{
			is_array_ = is_array;
		}

		bool ElsaType::get_is_array() const
		{
			return is_array_;
		}

		bool ElsaType::are_equal(const ElsaType* other) const
		{
			if (this->get_type() == ObjectType::GCOPtr && other->get_type() == ObjectType::GCOPtr)
				return this->get_struct_declaration_expression()->get_name() == other->get_struct_declaration_expression()->get_name();

			if (this->get_type() == ObjectType::Function && other->get_type() == ObjectType::Function)
				return this->get_func_declaration_expression()->are_equal(other->get_func_declaration_expression());

			if (this->get_type() == ObjectType::Enum && other->get_type() == ObjectType::Enum)
				return this->enum_declaration_expression_->get_name() == other->enum_declaration_expression_->get_name();

			if (this->get_type() == ObjectType::Enum && other->get_type() == ObjectType::Int || other->get_type() == ObjectType::Enum && this->get_type() == ObjectType::Int)
				return true;

			return this->get_type() == other->get_type();
		}

		void ElsaType::assert_is_valid()
		{
			if (type_ == ObjectType::GCOPtr && struct_declaration_expression_ == nullptr)
				throw ElsaException("GCOPtr types must have a StructDeclarationExpression");

			if (type_ == ObjectType::Function && func_declaration_expression_ == nullptr)
				throw ElsaException("Function types must have a FuncDeclarationExpression");

			if (type_ == ObjectType::Enum && enum_declaration_expression_ == nullptr)
				throw ElsaException("Enum types must have a EnumDeclarationExpression");
		}
	}
}

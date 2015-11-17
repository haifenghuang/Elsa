#include "literal_expression_builder.h"

#include "../visitors/vm_expression_visitor.h"

namespace elsa {
	namespace compiler {

		void LiteralExpressionBuilder::build(VMProgram* program, IntegerLiteralExpression* expression)
		{
			program->emit(OpCode::iconst);
			program->emit(expression->get_value());
		}

		void LiteralExpressionBuilder::build(VMProgram* program, FloatLiteralExpression* expression)
		{
			program->emit(OpCode::fconst);
			auto index = program->add_float(std::make_unique<FloatInfo>(expression->get_value()));
			program->emit(static_cast<int>(index));
		}

		void LiteralExpressionBuilder::build(VMProgram* program, CharLiteralExpression* expression)
		{
			program->emit(OpCode::cconst);
			auto index = program->add_char(std::make_unique<CharInfo>(expression->get_value()));
			program->emit(static_cast<int>(index));
		}

		void LiteralExpressionBuilder::build(VMProgram* program, BoolLiteralExpression* expression)
		{
			program->emit(OpCode::bconst);
			auto value = expression->get_value() == true ? 1 : 0;
			program->emit(static_cast<int>(value));
		}

	}
}


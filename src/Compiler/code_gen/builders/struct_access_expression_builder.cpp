#include "struct_access_expression_builder.h"

#include "../../ast/struct_declaration_expression.h"

namespace elsa {
	namespace compiler {

		void StructAccessExpressionBuilder::build(VMProgram* program, VMExpressionVisitor* visitor, StructAccessExpression* expression)
		{
			IdentifierExpressionBuilder::build(program, visitor, expression->get_base());

			auto current_type = expression->get_base()->get_type()->get_struct_declaration_expression();
			for (const auto& field_exp : expression->get_expressions())
			{
				for (const auto& field : current_type->get_fields())
				{
					if (field->get_name() == field_exp->get_name())
					{
						if (field->get_type()->get_type() == OType::GCOPtr)
							current_type = field->get_type()->get_struct_declaration_expression();

						program->emit(OpCode::l_field);
						program->emit(static_cast<int>(field->get_index()));
						break;
					}
				}
			}
		}

	}
}

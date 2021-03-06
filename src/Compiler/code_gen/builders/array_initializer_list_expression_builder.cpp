#include "array_initializer_list_expression_builder.h"

#include "../vm_expression_visitor.h"

namespace elsa {
	namespace compiler {

		void ArrayInitializerListExpressionBuilder::build(VMProgram* program, VMExpressionVisitor* visitor, ArrayInitializerListExpression* expression)
		{
			program->emit(OpCode::iconst);

			auto size = expression->get_values().size();
			program->emit(static_cast<int>(size));

			program->emit(OpCode::new_arr);

			auto type = expression->get_type()->get_struct_declaration_expression()->get_generic_type();
			program->emit(static_cast<int>(type->get_vm_type()));

			auto local_index = visitor->current_scope()->create_new_local();

			program->emit(OpCode::s_local);
			program->emit(static_cast<int>(local_index));

			for (const auto& exp : expression->get_values())
			{
				program->emit(OpCode::l_local);
				program->emit(static_cast<int>(local_index));
				exp->accept(visitor);
				program->emit(OpCode::a_ele);
			}

			program->emit(OpCode::l_local);
			program->emit(static_cast<int>(local_index));
		}

	}
}

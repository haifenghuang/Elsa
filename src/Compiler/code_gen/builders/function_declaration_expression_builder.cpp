#include "function_declaration_expression_builder.h"

#include "../visitors/vm_expression_visitor.h"

namespace elsa {
	namespace compiler {

		void FunctionDeclarationExpressionBuilder::build(VMProgram* program, VMExpressionVisitor* visitor, FuncDeclarationExpression* expression)
		{
			auto fi = std::make_unique<FunctionInfo>(expression->get_name());

			if (expression->get_name() == L"main")
			{
				if (program->get_entry_point() != -1)
					throw CodeGenException("An entry point has alredy been defined");

				program->set_entry_point(static_cast<int>(program->get_next_instruction_index()));
			}

			fi->set_addr(static_cast<int>(program->get_next_instruction_index()));

			visitor->set_current_function(expression->get_name());
			visitor->reset_scope_nesting();

			for (auto& exp : expression->get_body())
			{
				exp->accept(visitor);
			}

			program->add_func(std::move(fi));

			visitor->reset_current_function();
		}

	}
}

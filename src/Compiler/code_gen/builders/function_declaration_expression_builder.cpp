#include "function_declaration_expression_builder.h"

#include "../visitors/vm_expression_visitor.h"

namespace elsa {
	namespace compiler {

		void FunctionDeclarationExpressionBuilder::build(VMProgram* program, VMExpressionVisitor* visitor, FuncDeclarationExpression* expression)
		{
			build_internal(program, visitor, expression);
		}

		const FunctionInfo* FunctionDeclarationExpressionBuilder::build_member(VMProgram* program, VMExpressionVisitor* visitor, FuncDeclarationExpression* expression)
		{
			return build_internal(program, visitor, expression);
		}

		const FunctionInfo* FunctionDeclarationExpressionBuilder::build_internal(VMProgram* program, VMExpressionVisitor* visitor, FuncDeclarationExpression* expression)
		{
			if (expression->built())
			{
				return program->get_func(expression->get_name());
			}

			for (auto nested_function_expression : expression->get_nested_functions())
			{
				build_internal(program, visitor, nested_function_expression);
			}

			auto fi = std::make_unique<FunctionInfo>(expression->get_name());
			fi->set_num_args(expression->get_num_args());
			auto is_main = expression->get_name() == L"main";

			if (is_main)
			{
				if (program->get_entry_point() != -1)
					throw CodeGenException("An entry point has alredy been defined");

				program->set_entry_point(static_cast<int>(program->get_next_instruction_index()));
			}

			fi->set_addr(static_cast<int>(program->get_next_instruction_index()));

			auto parent_scope = visitor->current_scope();
			visitor->set_current_scope(expression);

			for (auto& exp : expression->get_body())
			{
				exp->accept(visitor);
			}

			if (is_main)
			{
				program->emit(OpCode::halt);
			}
			else
			{
				program->emit(OpCode::ret);
			}

			fi->set_num_locals(expression->get_num_locals());

			auto fi_ptr = fi.get();
			program->add_func(std::move(fi));

			visitor->set_current_scope(parent_scope);
			expression->set_built(true);

			return fi_ptr;
		}

	}
}

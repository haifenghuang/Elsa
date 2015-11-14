#pragma once

#include <memory>
#include <utility>

#include "exceptions\codegen_exception.h"
#include "../../ast/func_call_expression.h"
#include "constants\function_info.h"
#include "vm_program.h"

namespace elsa {
	namespace compiler {

		class VMExpressionVisitor;

		class FuncCallExpressionBuilder
		{
		public:
			static void build(VMProgram* program, VMExpressionVisitor* visitor, FuncCallExpression* expression);
		};

	}
}
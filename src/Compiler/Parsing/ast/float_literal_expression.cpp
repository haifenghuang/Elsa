#include "float_literal_expression.h"

namespace elsa {
	namespace compiler {

		FloatLiteralExpression::FloatLiteralExpression(float value)
			:
			value_(value)
		{}

		float FloatLiteralExpression::get_value() const
		{
			return value_;
		}

	}
}


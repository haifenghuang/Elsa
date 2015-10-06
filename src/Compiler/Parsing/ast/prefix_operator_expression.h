#pragma once

#include <memory>

#include "expression.h"
#include "../../lexing/token.h"

namespace elsa {
	namespace compiler {

		class PrefixOperatorExpression : public Expression
		{
		public:
			PrefixOperatorExpression(TokenType op, Expression* right);

			TokenType get_operator() const;
			Expression* get_right() const;

		private:
			TokenType op_;
			std::unique_ptr<Expression> right_;
		};

	}
}

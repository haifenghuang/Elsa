#pragma once

#include <memory>

#include "../ast/expression.h"
#include "../ast/char_literal_expression.h"
#include "../ast/integer_literal_expression.h"
#include "../ast/float_literal_expression.h"
#include "../ast/bool_literal_expression.h"
#include "../ast/identifier_expression.h"
#include "../ast/variable_declaration_expression.h"
#include "../ast/binary_operator_expression.h"
#include "../ast/prefix_operator_expression.h"
#include "elsa_type.h"
#include "../token.h"
#include "exceptions\parsing_exception.h"
#include "../ast/create_struct_expression.h"

namespace elsa {
	namespace compiler {

		class ElsaParser;

		class TypeChecker
		{
		public:
			TypeChecker(ElsaParser* parser);

			ElsaType* get_expression_type(Expression* expression);
			ElsaType* get_type_from_token(TokenType type);
			void assert_is_same_type(OType t1, OType t2);

		private:
			template<typename TExpression>
			static bool is_of_type(Expression* exp);
			ElsaParser* parser_;
		};

	}
}

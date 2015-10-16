#include "argument_expression.h"

namespace elsa {
	namespace compiler {

		void ArgumentExpression::set_name(const std::wstring& name)
		{
			name_ = name;
		}

		void ArgumentExpression::set_type(ElsaType* type)
		{
			type_ = std::unique_ptr<ElsaType>(type);
		}

		const std::wstring& ArgumentExpression::get_name() const
		{
			return name_;
		}

		const ElsaType* ArgumentExpression::get_type() const
		{
			return type_.get();
		}
	}
}
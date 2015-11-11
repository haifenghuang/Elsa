#pragma once

#include <string>
#include <memory>
#include <cstddef>

#include "expression.h"
#include "../types/elsa_type.h"

namespace elsa {
	namespace compiler {

		class FieldExpression : public Expression
		{
		public:
			void set_name(const std::wstring& name);
			void set_type(ElsaType* type);
			void set_index(std::size_t index);

			const std::wstring& get_name() const;
			const ElsaType* get_type() const;
			std::size_t get_index() const;

			void accept(ExpressionVisitor* visitor) override;

		private:
			std::wstring name_;
			std::unique_ptr<ElsaType> type_;
			std::size_t index_;
		};
	}
}

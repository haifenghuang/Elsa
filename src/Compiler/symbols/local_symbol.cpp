#include "local_symbol.h"

namespace elsa {
	namespace compiler {

		LocalSymbol::LocalSymbol(const std::wstring& name, std::size_t index, const ElsaType& type)
			:
			name_(name),
			index_(index),
			type_(type)
		{}

		const std::wstring& LocalSymbol::get_name() const
		{
			return name_;
		}

		std::size_t LocalSymbol::get_index() const
		{
			return index_;
		}

		const ElsaType& LocalSymbol::get_type() const
		{
			return type_;
		}

	}
}
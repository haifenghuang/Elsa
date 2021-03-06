#include "scoped_expression.h"

#include "func_declaration_expression.h"

namespace elsa {
	namespace compiler {

		ScopedExpression::ScopedExpression(ScopedExpression* parent)
			:
			parent_(parent)
		{}

		LocalTable& ScopedExpression::locals()
		{
			return locals_;
		}

		void ScopedExpression::add_arg(const std::wstring& name, const ElsaType& type)
		{
			add(name, type, nullptr, LocalType::Arg);
			get_root_scope()->increment_num_args();
		}

		void ScopedExpression::add_arg(const std::wstring& name, const ElsaType& type, const StructDeclarationExpression* struct_expression)
		{
			add(name, type, struct_expression, LocalType::Arg);
			get_root_scope()->increment_num_args();
		}

		void ScopedExpression::add_local(const std::wstring& name, const ElsaType& type)
		{
			add(name, type, nullptr, LocalType::Local);
			get_root_scope()->increment_num_locals();
		}

		void ScopedExpression::add_local(const std::wstring& name, const ElsaType& type, const StructDeclarationExpression* struct_expression)
		{
			add(name, type, struct_expression, LocalType::Local);
			get_root_scope()->increment_num_locals();
		}

		const LocalSymbol* ScopedExpression::get_local(const std::wstring& name, bool include_closure)
		{
			const auto local = locals_.get(name);

			if (local != nullptr)
				return local;

			if (!include_closure)
			{
				// If we hit a nested function and no local is found the local must be defined in an outer function scope so we return null
				if (auto fde = dynamic_cast<FuncDeclarationExpression*>(this) && parent_ != nullptr)
					return nullptr;
			}

			if (local == nullptr && parent_ != nullptr)
				return parent_->get_local(name, include_closure);

			return nullptr;
		}

		const LocalSymbol* ScopedExpression::get_local_from_closure(const std::wstring& name)
		{
			return get_local(name, true);
		}

		bool ScopedExpression::any_scope_has_local(const std::wstring & name)
		{
			return get_local(name) != nullptr;
		}

		bool ScopedExpression::has_local(const std::wstring & name)
		{
			return locals_.has_entry(name);
		}

		std::size_t ScopedExpression::create_new_local()
		{
			auto index = add(L"0", ElsaType(ObjectType::Object), nullptr, LocalType::Local);
			get_root_scope()->increment_num_locals();
			return index;
		}

		ScopedExpression* ScopedExpression::parent()
		{
			return parent_;
		}

		void ScopedExpression::set_parent_scope(ScopedExpression* parent)
		{
			if (parent_ != nullptr)
				throw ElsaException(L"ScopedExpression::set_parent -> A parent scoped is already defined");

			parent_ = parent;
		}

		void ScopedExpression::accept(ExpressionVisitor * visitor)
		{
			throw ElsaException("Not implemented: ScopedExpression::accept");
		}

		FuncDeclarationExpression* ScopedExpression::get_root_scope(bool stop_at_function)
		{
			if (stop_at_function)
			{
				if (auto fde = dynamic_cast<FuncDeclarationExpression*>(this))
					return fde;
			}

			if (parent_ == nullptr)
				return dynamic_cast<FuncDeclarationExpression*>(this);

			return parent_->get_root_scope();
		}

		std::size_t ScopedExpression::add(const std::wstring& name, const ElsaType& type, const StructDeclarationExpression* struct_expression, LocalType local_type)
		{
			std::size_t index = 0;
			if (local_type == LocalType::Local)
				index = get_root_scope()->get_num_locals();
			else
				index = get_root_scope()->get_num_args();

			locals_.add(name, new LocalSymbol(name, index, type, struct_expression, local_type));
			return index;
		}

	}
}

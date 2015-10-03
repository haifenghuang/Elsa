#pragma once

#include <cstddef>
#include <string>

namespace elsa {

		enum FunctionType
		{
			Static,
			Member
		};

		class FunctionInfo
		{
		public:
			FunctionInfo(const std::string& name, std::size_t num_args, std::size_t num_locals, std::size_t addr, FunctionType type);
			~FunctionInfo();

			std::string get_name() const;
			std::size_t get_num_args() const;
			std::size_t get_num_locals() const;
			std::size_t get_addr() const;
			FunctionType get_type() const;

		private:
			std::string name_;
			std::size_t num_args_;
			std::size_t num_locals_;
			std::size_t addr_;
			FunctionType type_;
		};

}
#include "native_calls.h"

namespace elsa {
	namespace vm {

		NativeCalls::NativeCalls()
		{
			initialize();
		}
		
		void NativeCalls::invoke(std::size_t index, StackFrame* frame)
		{
			functions_[index](frame);
		}
		
		void NativeCalls::initialize()
		{
			functions_.push_back(print);
			functions_.push_back(print_ln);
		}

		void NativeCalls::print(StackFrame* frame)
		{
			print_internal(frame);
		}

		void NativeCalls::print_ln(StackFrame* frame)
		{
			print_internal(frame);
			std::cout << std::endl;
		}

		void NativeCalls::print_internal(StackFrame * frame)
		{
			auto object = frame->pop();

			if (object.get_type() == OType::Int)
				std::cout << object.i();
			else if (object.get_type() == OType::Float)
				std::cout << object.f();
			else if (object.get_type() == OType::Char)
				std::wcout << object.c();
			else if (object.get_type() == OType::Bool)
				std::cout << object.b();
			else
				throw RuntimeException("Unsupported type: print_ln");
		}

	}
}
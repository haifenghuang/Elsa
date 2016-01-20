#pragma once

#include <cstddef>
#include <vector>
#include <iostream>
#include <memory>

#include "opcodes\opcodes.h"
#include "object_types\vm_type.h"
#include "types\object.h"
#include "call_stack.h"
#include "memory\heap.h"
#include "vm_program.h"
#include "constants\function_info.h"
#include "exceptions\runtime_exception.h"
#include "GC\gc.h"
#include "interop\native_calls.h"

namespace elsa {
	namespace vm {

		class  VM
		{
		public:
			VM(VMProgram& program);
			~VM();

			void execute();
			void execute_one();
			void skip_one();
			Object eval_stack_top() const;
			std::size_t get_pc() const;
			GCResult gc_collect();
			void dump_stack_trace();

		private:
			int get_instruction(std::size_t pc);
			void cycle();
			void push_main();
			void next_opcode();

			OpCode oc_;
			std::size_t code_length_;
			std::size_t pc_;
			CallStack call_stack_;
			StackFrame* current_frame_;
			VMProgram& program_;
			Heap heap_;
			GC gc_;
			NativeCalls native_calls_;
		};

	}
}

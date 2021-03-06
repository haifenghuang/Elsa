#pragma once

#include <Windows.h>
#include <string>
#include <string.h>
#include <vector>
#include <cstddef>
#include <functional>
#include <iostream>

#include "../stack_frame.h"
#include "object_types/vm_type.h"
#include "../memory/heap.h"

namespace elsa {
	namespace vm {

		class Window;
		class FileHandle;
		class VM;

		class NativeCalls
		{
		public:
			NativeCalls(VM* vm);
			
			void invoke(std::size_t index, StackFrame* frame, Heap* heap);

		private:
			void initialize();
			static void print(StackFrame* frame, Heap* heap);
			static void print_ln(StackFrame* frame, Heap* heap);
			static void print_internal(StackFrame* frame, Heap* heap);
			static void are_eq(StackFrame* frame, Heap* heap);
			static bool are_eq_internal(StackFrame* frame);
			static void assert_eq(StackFrame* frame, Heap* heap);
			static void ref_eq(StackFrame* frame, Heap* heap);
			static bool ref_eq_internal(const Object& first, const Object& second);
			static void create_window(StackFrame* frame, Heap* heap);
			static void open_window(StackFrame* frame, Heap* heap);
			static void close_window(StackFrame* frame, Heap* heap);
			static void update_window(StackFrame* frame, Heap* heap);
			static void fill_rect(StackFrame* frame, Heap* heap);
			static void fill_circle(StackFrame* frame, Heap* heap);
			static void sleep(StackFrame* frame, Heap* heap);
			static void get_ticks(StackFrame* frame, Heap* heap);
			static void key_down(StackFrame* frame, Heap* heap);
			static void render_text(StackFrame* frame, Heap* heap);
			static void window_blt(StackFrame* frame, Heap* heap);
			static void file_open(StackFrame* frame, Heap* heap);
			static void file_close(StackFrame* frame, Heap* heap);
			static void file_read(StackFrame* frame, Heap* heap);
			static void file_write(StackFrame* frame, Heap* heap);

			static std::wstring read_string(Object& object, Heap* heap);
			static bool is_string(Object& object);
			static Window* get_window_handle(Object& object);
			static FileHandle* get_file_handle(Object& object);

			std::vector<std::function<void(StackFrame*, Heap*)>> functions_;
			Heap* heap_;
		};

	}
}
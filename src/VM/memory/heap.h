#pragma once

#include <cstddef>
#include <cstdint>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>

#include "object_types\vm_type.h"
#include "../types/object.h"
#include "constants/struct_info.h"
#include "exceptions/runtime_exception.h"
#include "../types/gcobject.h"

namespace elsa {
	namespace vm {
		class GC;
		typedef int8_t byte;
		class ResourceHandle;

		class Heap
		{
			friend class GC;
		public:
			Heap();
			~Heap();

			Object alloc_struct(const StructInfo* si);
			Object alloc_array(elsa::VMType type, std::size_t size);

			Object load_field(const Object& instance, FieldInfo* fi);
			Object load_field(const Object& instance, std::size_t field_index);
			void store_field(const Object& instance, const Object& value, FieldInfo* fi);
			void store_field(const Object& instance, const Object& value, std::size_t field_index);
			std::wstring load_string(const Object& instance);
			Object load_element(const Object& instance, int element_index);
			void store_element(const Object& instance, const Object& value, int element_index);
			void add_element(Object& instance, const Object& value);
			Object pop_element(Object& instance);
			void remove_element(Object& instance, int element_index);
			void copy_array(const Object& source, Object& target, int index_to_exclude = -1);
			int array_length(const Object& instance);
			std::size_t get_num_objects() const;
			Object allocate_resource_handle(ResourceHandle* handle);

		private:
			void assert_is_not_null(const Object& instance);
			void assert_is_struct(const Object& instance);
			void assert_is_array(const Object& instance);
			void assert_arrays_are_same_type(const Object& first, const Object& second);
			void assert_array_index_in_range(const Object& instance, int element_index);
			byte* get_field_ptr(void* s_ptr, FieldInfo* f);
			std::size_t get_size_of_type(VMType type);
			void init_struct(const Object& instance);
			void init_array(const Object& instance, std::size_t start_index = 0);
			void resize_array(Object& instance, std::size_t new_size);
			Object get_default_value(VMType type);
			void link_new_object(GCObject* obj);
			void increment_num_objects();

			std::size_t num_objects_;
			GCObject* base_;
		};

	}
}
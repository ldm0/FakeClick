#pragma once

#include"string.h"

namespace FakeClick {
	class IO {
	public:
		static IO& get();
		bool file2str(const String file_path, String &des);
		bool str2file(const String file_path, const String &res);
		bool file2data(const String file_path, void *data, size_t size);
		bool data2file(const String file_path, const void* data, size_t size);
	};
}
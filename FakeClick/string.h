#pragma once

#include<iostream>

namespace FakeClick{
	class String {
		int m_capacity;
		int m_top;
		char *m_data;
	public:
		String(const char *data = nullptr, const int length = -1);

		String(const int length, const char *data = nullptr);

		String(const String &string);

		~String();

		//let the capacity to be the minimal exp of 2
		void Shrink();

		String& operator=(const String &string);

		String& operator=(const char *c_string);

		String& operator+=(const String &string);

		String operator+(const String &string) const;

		//string length no more than INT_MAX
		String& operator+(const char *c_string);

		const char *c_str() const;
	};

	std::ostream& operator<<(std::ostream &os, const String &string);
}

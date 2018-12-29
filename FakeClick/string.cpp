#include"string.h"
#include<iostream>

using namespace FakeClick;

namespace Utils{
	int log2_int(int input)
	{
		union { int i; float f; } tmp;
		tmp.f = (float)input;
		return ((tmp.i >> 23) - 127);
	}
	void strncpy(char *des, const char *source, int length)
	{
		for (int i = 0; i < length; ++i)
			des[i] = source[i];
	}
	void memset(void *mem, int val, size_t size)
	{
		for (size_t i = 0; i < size; ++i) {
			((int *)mem)[i] = val;
		}
	}
}

String::String(const int length, const char *data)
{
	m_top = 0;

	// Important
	if (data != nullptr)
		for (; data[m_top] != '\0' && m_top < length; ++m_top);

	m_capacity = 1 << (Utils::log2_int(length) + 1);
	size_t size = (m_capacity + 1) * sizeof(char);
	m_data = (char *)operator new(size);
	Utils::memset(m_data, 0, size);
	if (data != nullptr)
		Utils::strncpy(m_data, data, m_top);
}

String::String(const char *data, const int length)
{
	if (length == -1) {
		if (data != nullptr) {
			for (m_top = 0; data[m_top] != '\0'; ++m_top);
			m_capacity = 1 << (Utils::log2_int(m_top) + 1);
			size_t size = (m_capacity + 1) * sizeof(char);
			m_data = (char *)operator new(size);
			Utils::memset(m_data, 0, size);
			Utils::strncpy(m_data, data, m_top);
			return;
		}
		m_top = 0;
		m_capacity = 0;
		size_t size = (m_capacity + 1) * sizeof(char);
		m_data = (char *)operator new(size);
		Utils::memset(m_data, 0, size);
		return;
	}

	m_top = 0;

	// Important
	if (data != nullptr)
		for (; data[m_top] != '\0' && m_top < length; ++m_top);

	m_capacity = 1 << (Utils::log2_int(length) + 1);
	size_t size = (m_capacity + 1) * sizeof(char);
	m_data = (char *)operator new(size);
	Utils::memset(m_data, 0, size);
	if (data != nullptr)
		Utils::strncpy(m_data, data, m_top);

}

String::String(const String &string)
{
	m_capacity = string.m_capacity;
	m_top = string.m_top;
	size_t size = (m_capacity + 1) * sizeof(char);
	m_data = (char *)operator new(size);
	Utils::memset(m_data, 0, size);
	Utils::strncpy(m_data, string.m_data, m_top);
}

String::~String()
{
	delete(m_data);
}

void String::Shrink()
{
	int tmp = 1 << (Utils::log2_int(m_top) + 1);
	if (m_capacity == tmp)
		return;
	m_capacity = tmp;
	size_t size = (m_capacity + 1) * sizeof(char);
	char *tmp_data = (char *)operator new(size);
	Utils::memset(tmp_data, 0, size);
	Utils::strncpy(tmp_data, m_data, m_top);
	delete(m_data);
	m_data = tmp_data;
}

String& String::operator=(const String &string)
{
	if (this == &string)
		return *this;
	size_t size = (string.m_capacity + 1) * sizeof(char);
	char *tmp = (char *)operator new(size);
	Utils::memset(m_data, 0, size);
	Utils::strncpy(tmp, string.m_data, string.m_top);
	delete(m_data);
	m_data = tmp;
	return *this;
}

String& String::operator=(const char *c_string)
{
	int length;
	for (length = 0; c_string[length] != '\0'; ++length);

	m_top = length;
	if (m_top >= m_capacity) {
		char *tmp_data = m_data;
		m_capacity = (1 << (Utils::log2_int(m_top) + 1)) + 1;
		m_data = (char *)operator new((m_capacity + 1) * sizeof(char));
	}
	Utils::memset(m_data, 0, (m_capacity + 1) * sizeof(char));
	Utils::strncpy(m_data, c_string, length);
	return *this;
}

String& String::operator+=(const String &string)
{
	char *tmp = m_data;
	int tmp_top = m_top;

	m_top += string.m_top;
	m_capacity = 1 << (Utils::log2_int(m_top) + 1);
	size_t size = (m_capacity + 1) * sizeof(char);
	m_data = (char *)operator new(size);
	Utils::memset(m_data, 0, size);

	Utils::strncpy(m_data, tmp, tmp_top);
	Utils::strncpy(m_data + tmp_top, string.m_data, string.m_top);

	delete(tmp);
	return *this;
}

String String::operator+(const String &string) const
{
	String result = String(m_top + string.m_top);
	Utils::strncpy(result.m_data, m_data, m_top);
	Utils::strncpy(result.m_data + m_top, string.m_data, string.m_top);
	result.m_top = m_top + string.m_top;
	return result;
}

String& String::operator+(const char *c_string)
{
	size_t size = 0;

	//Get length
	int length;
	for (length = 0; c_string[length] != '\0'; ++length);

	char *tmp_data = m_data;
	////Can be optimized by using memcpy m_top - length later in the memcpy
	//int tmp_top = m_top; 

	m_top += length;

	if (m_top >= m_capacity) {
		//If larger, realloc the m_data to fit the two string
		m_capacity = 1 << (Utils::log2_int(m_top) + 1);
		size = (m_capacity + 1) * sizeof(char);
		m_data = (char *)operator new(size);
		Utils::memset(m_data, 0, size);
		Utils::strncpy(m_data, tmp_data, m_top - length);
		delete(tmp_data);
	}
	Utils::strncpy(m_data + m_top - length, c_string, length);
	return *this;
}

const char *String::c_str() const
{
	return m_data;
}

std::ostream& FakeClick::operator<<(std::ostream &os, const String &string)
{
	os << string.c_str();
	return os;
}

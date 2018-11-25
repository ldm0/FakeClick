#include"string.h"
//#include<string.h> //for strncpy(), memset()

using namespace FakeClick;

int log2_int(int input)
{
	union {
		int i;
		float f;
	} tmp;
	tmp.f = (float)input;
	return ((tmp.i >> 23) - 127);
}

char *strncpy(char *dest, const char *src, int n)
{
	for (int i = 0; i < n; ++i) {
		dest[i] = src[i];
	}
	return dest;
}

String::String(const int length, const char *data)
{
	m_top = 0;

	// Important
	if (data != nullptr)
		for (; data[m_top] != '\0' && m_top < length; ++m_top);

	m_capacity = 1 << (log2_int(length) + 1);
	size_t size = (m_capacity + 1) * sizeof(char);
	m_data = (char *)operator new(size);
	memset(m_data, 0, size);
	if (data != nullptr)
		strncpy(m_data, data, m_top);
}

String::String(const char *data, const int length)
{
	if (length == -1) {
		if (data != nullptr) {
			for (m_top = 0; data[m_top] != '\0'; ++m_top);
			m_capacity = 1 << (log2_int(m_top) + 1);
			size_t size = (m_capacity + 1) * sizeof(char);
			m_data = (char *)operator new(size);
			memset(m_data, 0, size);
			strncpy(m_data, data, m_top);
			return;
		}
		m_top = 0;
		m_capacity = 0;
		size_t size = (m_capacity + 1) * sizeof(char);
		m_data = (char *)operator new(size);
		memset(m_data, 0, size);
		return;
	}

	m_top = 0;

	// Important
	if (data != nullptr)
		for (; data[m_top] != '\0' && m_top < length; ++m_top);

	m_capacity = 1 << (log2_int(length) + 1);
	size_t size = (m_capacity + 1) * sizeof(char);
	m_data = (char *)operator new(size);
	memset(m_data, 0, size);
	if (data != nullptr)
		strncpy(m_data, data, m_top);

}

String::String(const String &string)
{
	m_capacity = string.m_capacity;
	m_top = string.m_top;
	size_t size = (m_capacity + 1) * sizeof(char);
	m_data = (char *)operator new(size);
	memset(m_data, 0, size);
	strncpy(m_data, string.m_data, m_top);
}

String::~String()
{
	delete(m_data);
}

void String::Shrink()
{
	int tmp = 1 << (log2_int(m_top) + 1);
	if (m_capacity == tmp)
		return;
	m_capacity = tmp;
	size_t size = (m_capacity + 1) * sizeof(char);
	char *tmp_data = (char *)operator new(size);
	memset(tmp_data, 0, size);
	strncpy(tmp_data, m_data, m_top);
	delete(m_data);
	m_data = tmp_data;
}

String& String::operator=(const String &string)
{
	if (this == &string)
		return *this;
	size_t size = (string.m_capacity + 1) * sizeof(char);
	char *tmp = (char *)operator new(size);
	memset(m_data, 0, size);
	strncpy(tmp, string.m_data, string.m_top);
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
		m_capacity = (1 << (log2_int(m_top) + 1)) + 1;
		m_data = (char *)operator new((m_capacity + 1) * sizeof(char));
	}
	memset(m_data, 0, (m_capacity + 1) * sizeof(char));
	strncpy(m_data, c_string, length);
	return *this;
}

String& String::operator+=(const String &string)
{
	char *tmp = m_data;
	int tmp_top = m_top;

	m_top += string.m_top;
	m_capacity = 1 << (log2_int(m_top) + 1);
	size_t size = (m_capacity + 1) * sizeof(char);
	m_data = (char *)operator new(size);
	memset(m_data, 0, size);

	strncpy(m_data, tmp, tmp_top);
	strncpy(m_data + tmp_top, string.m_data, string.m_top);

	delete(tmp);
	return *this;
}

String& String::operator+=(const char *c_string)
{
	size_t size = 0;

	//Get length
	int length;
	for (length = 0; c_string[length] != '\0'; ++length);

	char *tmp_data = m_data;
	////Can be optimized by useing memcpy m_top - length later in the memcpy
	//int tmp_top = m_top; 

	m_top += length;

	if (m_top >= m_capacity) {
		//If larger, realloc the m_data to fit the two string
		m_capacity = 1 << (log2_int(m_top) + 1);
		size = (m_capacity + 1) * sizeof(char);
		m_data = (char *)operator new(size);
		memset(m_data, 0, size);
		strncpy(m_data, tmp_data, m_top - length);
		delete(tmp_data);
	}
	strncpy(m_data + m_top - length, c_string, length);
	return *this;
}

String String::operator+(const String &string) const
{
	String result = String(m_top + string.m_top);
	strncpy(result.m_data, m_data, m_top);
	strncpy(result.m_data + m_top, string.m_data, string.m_top);
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
	////Can be optimized by useing memcpy m_top - length later in the memcpy
	//int tmp_top = m_top; 

	m_top += length;

	if (m_top >= m_capacity) {
		//If larger, realloc the m_data to fit the two string
		m_capacity = 1 << (log2_int(m_top) + 1);
		size = (m_capacity + 1) * sizeof(char);
		m_data = (char *)operator new(size);
		memset(m_data, 0, size);
		strncpy(m_data, tmp_data, m_top - length);
		delete(tmp_data);
	}
	strncpy(m_data + m_top - length, c_string, length);
	return *this;
}

const char *String::c_str() const
{
	return m_data;
}

std::ostream & FakeClick::operator<<(std::ostream & os, const String & string)
{
	os << string.c_str();
	return os;
}

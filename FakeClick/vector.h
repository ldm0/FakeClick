#pragma once

namespace FakeClick {
	inline int log2_int(int input)
	{
		union { int i; float f; } tmp;
		tmp.f = (float)input;
		return ((tmp.i >> 23) - 127);
	}

	inline int get_capacity(int top)
	{
		return 1 << (log2_int(top) + 1);
	}

	template<typename T>
	class Vector {
		int m_top;
		int m_capacity;
		T *m_data;
	public:
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef T*			iterator;
		typedef const T*	const_iterator;

		// When exceeds the capacity will double the size
		Vector(int capacity = 1024, int length = 0, const T *data = nullptr);
		Vector(const Vector &vector);
		~Vector();

		Vector& operator=(const Vector &vector);
		Vector& operator+(const Vector &vector);
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;
		void shrink_to_fit();
		void clear();
		void push_back(const T &value);
		void pop_back();

		//template<class... Args>
		//void emplace_back(Args&&... args);

		void swap(Vector &other);
		bool empty() const;
		int size() const;
		int capacity() const;
		int max_size() const;
	};

	template<typename T>
	Vector<T>::Vector(int capacity, int length, const T *data)
	{
		m_top = length;
		m_capacity = get_capacity(capacity);
		m_data = (T *)operator new (m_capacity * sizeof(T));
		if (m_data == nullptr)
			__debugbreak();

		if (data != nullptr)
			// With OpenMP can be better. XD
			for (int i = 0; i < length; ++i)
				m_data[i] = data[i];
	}

	template<typename T>
	Vector<T>::Vector(const Vector &vector)
	{
		m_top = vector.m_top;
		m_capacity = vector.m_capacity;
		m_data = (T*)operator new(m_capacity * sizeof(T));
		if (m_data == nullptr)
			__debugbreak();
		for (int i = 0; i < m_top; ++i)
			m_data[i] = vector.m_data[i];
	}

	template<typename T>
	Vector<T>::~Vector()
	{
		operator delete(m_data);
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector & vector)
	{
		if (this == &vector)
			return *this;
		operator delete(m_data);
		m_top = vector.m_top;
		m_capacity = vector.m_capacity;
		m_data = (T *)operator new(m_capacity * sizeof(T));
		if (m_data == nullptr)
			__debugbreak();
		for (int i = 0; i < m_top; ++i)
			m_data[i] = vector.m_data[i];
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator+(const Vector &vector)
	{
		if (vector.m_top + m_top > m_capacity) {
			m_capacity = get_capacity(vector.m_top + m_top);
			T *tmp = (T *)operator new(m_capacity * sizeof(T));
			if (tmp == nullptr)
				__debugbreak();
			for (int i = 0; i < m_top; ++i)
				tmp[i] = m_data[i];
			operator delete(m_data);
			m_data = tmp;
		}

		for (int i = m_top; i < m_top + vector.m_top; ++i)
			m_data[i] = vector.m_data[i];
		m_top += vector.m_top;
		return *this;
	}

	template<typename T>
	typename Vector<T>::reference Vector<T>::front()
	{
		if (m_top <= 0)
			__debugbreak();
		return m_data[0];
	}

	template<typename T>
	typename Vector<T>::reference Vector<T>::back()
	{
		if (m_top <= 0)
			__debugbreak();
		return m_data[m_top - 1];
	}

	template<typename T>
	typename Vector<T>::const_reference Vector<T>::front() const
	{
		if (m_top <= 0)
			__debugbreak();
		return m_data[0];
	}

	template<typename T>
	typename Vector<T>::const_reference Vector<T>::back() const
	{
		if (m_top <= 0)
			__debugbreak();
		return m_data[m_top - 1];
	}

	template<typename T>
	typename Vector<T>::iterator Vector<T>::begin()
	{
		return m_data;
	}

	template<typename T>
	typename Vector<T>::const_iterator Vector<T>::begin() const
	{
		return m_data;
	}

	template<typename T>
	typename Vector<T>::iterator Vector<T>::end()
	{
		return (m_data + m_top);
	}

	template<typename T>
	typename Vector<T>::const_iterator Vector<T>::end() const
	{
		return (m_data + m_top);
	}

	template<typename T>
	void Vector<T>::shrink_to_fit(void)
	{
		int tmp = 1 << ((log2_int(m_top) + 1) + 1);
		if (m_capacity == tmp)
			return;
		m_capacity = tmp;
		T *tmp_data = (T *)operator new (m_capacity * sizeof(T));
		if (tmp_data == nullptr)
			__debugbreak();
		for (int i = 0; i < m_top; ++i)
			tmp_data[i] = m_data[i];
		delete m_data;
		m_data = tmp_data;
	}

	template<typename T>
	inline void Vector<T>::clear()
	{
		for (; m_top >= 0; --m_top)
			m_data[m_top].~T();
	}

	template<typename T>
	void Vector<T>::push_back(const T &value)
	{
		if (m_top + 1 > m_capacity) {
			m_capacity *= 2;
			T *tmp = (T *)operator new(m_capacity * sizeof(T));
			if (tmp == nullptr)
				__debugbreak();
			for (int i = 0; i < m_top; ++i)
				tmp[i] = m_data[i];
			operator delete(m_data);
			m_data = tmp;
		}
		m_data[m_top] = value;
		++m_top;
	}

	template<typename T>
	inline void Vector<T>::pop_back()
	{
		m_data[m_top].~T();
		--m_top;
	}

	template<typename T>
	inline void Vector<T>::swap(Vector &other)
	{
		m_top ^= other.m_top;
		other.m_top ^= m_top;
		m_top ^= other.m_top;

		m_capacity ^= other.m_capacity;
		other.m_capacity ^= m_capacity;
		m_capacity ^= other.m_capacity;

		T *tmp = m_data;
		m_data = other.m_data;
		other.m_data = tmp;
	}

	template<typename T>
	inline int Vector<T>::size() const
	{
		return m_top;
	}

	template<typename T>
	inline int Vector<T>::capacity() const
	{
		return m_capacity;
	}

	template<typename T>
	inline int Vector<T>::max_size() const
	{
		return 0x7fffffff;
	}

	template<typename T>
	inline bool Vector<T>::empty() const
	{
		return (m_top == 0);
	}
}
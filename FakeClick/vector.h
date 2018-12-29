#pragma once

namespace FakeClick{
	template<typename T>
	class Vector {
	public:
		typedef T			value_type;
		typedef int			size_type;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T*			iterator;
		typedef const T*	const_iterator;
	private:
		size_type m_top;
		size_type m_capacity;
		T *m_data;
		inline int log2_int(int input);
		inline int get_capacity(int top);
	public:
		// When exceeds the capacity will double the size
		Vector();
		Vector(const Vector &vector);
		~Vector();
		Vector& operator=(const Vector &vector);
		void assign(size_type count, const T& val);
		reference at(size_type pos);
		const_reference at(size_type pos) const;
		reference operator [](size_type pos);
		const_reference operator [](size_type pos) const;
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
		T* data();
		const T* data() const;
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;
		bool empty() const;
		int size() const;
		int max_size() const;
		void reserve(size_type new_cap);
		int capacity() const;
		void shrink_to_fit();

		void clear();
		iterator insert(const_iterator pos, const T& val);
		iterator insert(const_iterator pos, size_type count, const T& val);
		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);
		void push_back(const T &value);
		//template<class... Args>
		//void emplace_back(Args&&... args);
		void pop_back();
		void resize(size_type count);
		void resize(size_type count, const value_type& value);
		void swap(Vector &other);
	};

	template<typename T>
	inline int Vector<T>::log2_int(int input)
	{
		union { int i; float f; } tmp;
		tmp.f = (float)input;
		return ((tmp.i >> 23) - 127);
	}

	template<typename T>
	inline int Vector<T>::get_capacity(int top)
	{
		return 1 << (log2_int(top) + 1);
	}

	template<typename T>
	Vector<T>::Vector()
	{
		m_top = 0;
		m_capacity = 1024;
		m_data = (T *)operator new (m_capacity * sizeof(T));
	}

	template<typename T>
	Vector<T>::Vector(const Vector &vector)
	{
		m_top = vector.m_top;
		m_capacity = vector.m_capacity;
		m_data = (T*)operator new(m_capacity * sizeof(T));
		for (int i = 0; i < m_top; ++i)
			new(m_data + i) T(vector.m_data[i]);
	}

	template<typename T>
	Vector<T>::~Vector()
	{
		for (size_type i = 0; i < m_top; ++i)
			m_data[i].~T();
		operator delete(m_data);
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector & vector)
	{
		if (this == &vector)
			return *this;
		for (size_type i = 0; i < m_top; ++i)
			m_data[i].~T();
		operator delete(m_data);
		new(this) Vector<T>(vector);
		return *this;
	}

	template<typename T>
	void Vector<T>::assign(size_type count, const T & val)
	{
		for (size_type i = 0; i < m_top; ++i)
			m_data[i].~T();
		if (count > m_capacity) {
			operator delete(m_data);
			m_capacity = get_capacity(count);
			m_data = (T *)operator new(m_capacity * sizeof(T));
		}
		for (size_type i = 0; i < count; ++i)
			new(m_data + i) T(val);
		m_top = count;
	}

	template<typename T>
	typename Vector<T>::reference Vector<T>::at(size_type pos)
	{
		return m_data[pos];
	}

	template<typename T>
	typename Vector<T>::const_reference Vector<T>::at(size_type pos) const
	{
		return m_data[pos];
	}

	//template<typename T>
	//typename Vector<T>::reference Vector<T>::operaotr[](size_type pos)
	template<typename T>
	typename Vector<T>::reference Vector<T>::operator[](size_type pos)
	{
		return m_data[pos];
	}

	template<typename T>
	typename Vector<T>::const_reference Vector<T>::operator[](size_type pos) const
	{
		return m_data[pos];
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
	inline T * Vector<T>::data()
	{
		return m_data;
	}

	template<typename T>
	inline const T * Vector<T>::data() const
	{
		return m_data;
	}

	template<typename T>
	void Vector<T>::clear()
	{
		for (int i = 0; i < m_top; ++i)
			m_data[i].~T();
		m_top = 0;
	}

	template<typename T>
	typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, const T& val)
	{
		if (pos < m_data)
			return;
		size_type pos_index = (pos - m_data) / sizeof(T);
		if (m_top + 1 >= m_capacity) {
			m_capacity *= 2;
			T *new_data = (T*)operator new(m_capacity * sizeof(T));
			for (size_type i = 0; i < pos_index; ++i) {
				new(new_data + i) T(m_data[i]);
				m_data[i].~T();
			}
			new(new_data + pos_index) T(val);
			for (size_type i = pos_index; i < m_top; ++i) {
				new(new_data + i + 1) T(m_data[i]);
				m_data[i].~T();
			}
			m_data = new_data;
		} else {
			for (size_type i = m_top - 1; i > pos_index - 1; --i) {
				new(m_data + i + 1) T(m_data[i]);
				m_data[i].~T();
			}
			new(m_data + pos_index) T(val);
		}
		return m_data[pos_index];
	}

	template<typename T>
	typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, size_type count, const T & val)
	{
		if (pos < m_data)
			return;
		size_type pos_index = (pos - m_data) / sizeof(T);
		if (m_top + 1 >= m_capacity) {
			m_capacity *= 2;
			T *new_data = (T*)operator new(m_capacity * sizeof(T));
			for (size_type i = 0; i < pos_index; ++i) {
				new(new_data + i) T(m_data[i]);
				m_data[i].~T();
			}
			for (size_type i = 0; i < count; ++i) {
				new(new_data + pos_index + i) T(val);
			}
			for (size_type i = pos_index; i < m_top; ++i) {
				new(new_data + i + count) T(m_data[i]);
				m_data[i].~T();
			}
			m_data = new_data;
		} else {
			for (size_type i = m_top - 1; i > pos_index - 1; --i) {
				new(m_data + i + count) T(m_data[i]);
				m_data[i].~T();
			}
			for (size_type i = 0; i < count; ++i) {
				new(m_data + pos_index + i) T(val);
			}
		}
		return m_data[pos_index];
	}

	template<typename T>
	typename Vector<T>::iterator Vector<T>::erase(const_iterator pos)
	{
		// The end() iterator is just the m_end_guard
		size_type pos_index = (pos - m_data) / sizeof(T);
		for (size_type i = pos; i < m_top - 1; ++i) {
			m_data[i].~T();
			new(m_data + i) T(m_data[i + 1]);
		}
		m_data[m_top - 1].~T();
		--m_top;
	}

	template<typename T>
	typename Vector<T>::iterator Vector<T>::erase(const_iterator first, const_iterator last)
	{
		// The end() iterator is just the m_end_guard
		size_type pos_index = (first - m_data) / sizeof(T);
		size_type remove_length = (last - first) / sizeof(T);
		for (size_type i = pos_index; i < m_top - remove_length; ++i) {
			m_data[i].~T();
			new(m_data + i) T(m_data[i + remove_length]);
		}
		for (size_type i = 0; i < remove_length; ++i)
			m_data[m_top - i].~T();
		m_top -= remove_length;
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
	inline void Vector<T>::resize(size_type count)
	{
		if (count <= m_top) {
			for (int i = count; i < m_top; ++i)
				m_data[i].~T();
			m_top = count;
		} else {
			if (count > m_capacity) {
				m_capacity = get_capacity(count);
				T *new_data = (T *)operator new(m_capacity * sizeof(T));
				for (size_type i = 0; i < m_top; ++i) {
					new(new_data + i) T(m_data[i]);
					m_data[i].~T();
				}
				operator delete(m_data);
				m_data = new_data;
			}
			for (size_type i = m_top; i < count; ++i)
				new(m_data + i) T();
		}
	}

	template<typename T>
	inline void Vector<T>::resize(size_type count, const value_type &val)
	{
		if (count <= m_top) {
			for (int i = count; i < m_top; ++i)
				m_data[i].~T();
			m_top = count;
		} else {
			if (count > m_capacity) {
				m_capacity = get_capacity(count);
				T *new_data = (T *)operator new(m_capacity * sizeof(T));
				for (size_type i = 0; i < m_top; ++i) {
					new(new_data + i) T(m_data[i]);
					m_data[i].~T();
				}
				operator delete(m_data);
				m_data = new_data;
			}
			for (size_type i = m_top; i < count; ++i)
				new(m_data + i) T(val);
		}
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
	void Vector<T>::reserve(size_type new_cap)
	{
		size_type new_cap = (size_type)get_capacity(new_cap);
		if (new_cap <= m_capacity)
			return;
		T* new_data = (T *)operator new(new_cap * sizeof(T));
		for (size_type i = 0; i < m_top; ++i) {
			new(new_data + i) T(m_data[i]);
			m_data[i].~T();
		}
		operator delete(m_data);
		m_data = new_data;
	}

	template<typename T>
	void Vector<T>::shrink_to_fit()
	{
		size_type new_cap = (size_type)get_capacity(m_top);
		if (new_cap <= m_capacity)
			return;
		T* new_data = (T *)operator new(new_cap * sizeof(T));
		for (size_type i = 0; i < m_top; ++i) {
			new(new_data + i) T(m_data[i]);
			m_data[i].~T();
		}
		operator delete(m_data);
		m_data = new_data;
	}

	template<typename T>
	inline bool Vector<T>::empty() const
	{
		return (m_top == 0);
	}
}
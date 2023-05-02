#pragma once
template <typename T>
class DynamicArray
{
	size_t	m_size;
	T*	m_arr;

public:
	DynamicArray(size_t size)
		: m_size(size)
		, m_arr (new T[size])
	{
		std::cout << "Array Constructor\n";
	}

	~DynamicArray()
	{
		delete [] m_arr;
		std::cout << "Array Destructor\n";
	}

	T get(size_t index) const
	{
		return m_arr[index];
	}

	void set(size_t index, T val)
	{
		m_arr[index] = val;
	}

	void print() const
	{
		for (size_t i = 0; i < m_size; i++)
		{
			std::cout << i << " " << m_arr[i] << "\n";
		}
	}

	// allow you to get the value at index of Dynamic array with [] (eg std::cout << myArr[2] << "\n";)
	const T & operator [](size_t index) const
	{
		return m_arr[index];
	}

	// allow you to set the DynamicArray with [] operator (eg myArr[2] = 2;)
	T & operator [](size_t index)
	{
		return m_arr[index];
	}
};
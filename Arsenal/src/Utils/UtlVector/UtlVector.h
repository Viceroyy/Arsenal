#pragma once
#include <cstddef>
#include <new>

template <class T>
class UtlVector
{
public:
	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
	int m_nSize;
	T* m_pElements;

public:
	UtlVector()
		: m_pMemory(nullptr), m_nAllocationCount(0), m_nGrowSize(0), m_nSize(0), m_pElements(nullptr)
	{
	}

	UtlVector(int nGrowSize, int nInitSize = 0)
		: m_pMemory(nullptr), m_nAllocationCount(0), m_nGrowSize(nGrowSize), m_nSize(0), m_pElements(nullptr)
	{
	}

	~UtlVector()
	{
		Purge();
	}

	T& operator[](int i) const
	{
		return m_pMemory[i];
	}

	T& Element(int i) const
	{
		return m_pMemory[i];
	}

	T* Base()
	{
		return m_pMemory;
	}

	const T* Base() const
	{
		return m_pMemory;
	}

	int Count() const
	{
		return m_nSize;
	}

	bool IsEmpty() const
	{
		return m_nSize == 0;
	}

	int NumAllocated() const
	{
		return m_nAllocationCount;
	}

	void Purge()
	{
		if (m_pMemory)
		{
			for (int i = m_nSize; --i >= 0; )
				m_pMemory[i].~T();
			operator delete(m_pMemory);
		}
		m_pMemory = nullptr;
		m_nAllocationCount = 0;
		m_nSize = 0;
		m_pElements = nullptr;
	}

	void RemoveAll()
	{
		for (int i = m_nSize; --i >= 0; )
			m_pMemory[i].~T();
		m_nSize = 0;
		m_pElements = nullptr;
	}

	void GrowVector(int nCount = 1)
	{
		if (m_nSize + nCount > m_nAllocationCount)
		{
			int nNewAllocationCount = (m_nSize + nCount > m_nAllocationCount + m_nGrowSize) ? m_nSize + nCount : m_nAllocationCount + m_nGrowSize;
			T* pNewMemory = static_cast<T*>(operator new(nNewAllocationCount * sizeof(T)));

			if (m_pMemory)
			{
				for (int i = 0; i < m_nSize; i++)
				{
					new (pNewMemory + i) T(std::move(m_pMemory[i]));
					m_pMemory[i].~T();
				}
				operator delete(m_pMemory);
			}

			m_pMemory = pNewMemory;
			m_nAllocationCount = nNewAllocationCount;
		}
		m_nSize += nCount;
		m_pElements = m_pMemory + m_nSize;
	}

	int AddToTail(const T& src)
	{
		const int nIndex = m_nSize;
		GrowVector();
		new (m_pMemory + nIndex) T(src);
		return nIndex;
	}

	int AddToTail()
	{
		const int nIndex = m_nSize;
		GrowVector();
		new (m_pMemory + nIndex) T;
		return nIndex;
	}

	int InsertBefore(int nIndex, const T& src)
	{
		GrowVector();
		for (int i = m_nSize - 1; i > nIndex; --i)
		{
			new (m_pMemory + i) T(std::move(m_pMemory[i - 1]));
			m_pMemory[i - 1].~T();
		}
		new (m_pMemory + nIndex) T(src);
		return nIndex;
	}

	void Remove(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_nSize)
			return;

		m_pMemory[nIndex].~T();
		if (nIndex < m_nSize - 1)
		{
			for (int i = nIndex; i < m_nSize - 1; i++)
			{
				new (m_pMemory + i) T(std::move(m_pMemory[i + 1]));
				m_pMemory[i + 1].~T();
			}
		}
		--m_nSize;
		m_pElements = m_pMemory + m_nSize;
	}

	bool FindAndRemove(const T& src)
	{
		for (int i = 0; i < m_nSize; i++)
		{
			if (m_pMemory[i] == src)
			{
				Remove(i);
				return true;
			}
		}
		return false;
	}

	int Find(const T& src) const
	{
		for (int i = 0; i < m_nSize; i++)
		{
			if (m_pMemory[i] == src)
				return i;
		}
		return -1;
	}

	void CopyArray(const T* pArray, int nCount)
	{
		EnsureCapacity(nCount);
		m_nSize = nCount;
		for (int i = 0; i < nCount; i++)
			new (m_pMemory + i) T(pArray[i]);
		m_pElements = m_pMemory + m_nSize;
	}

	void EnsureCapacity(int nCount)
	{
		if (nCount > m_nAllocationCount)
		{
			if (m_pMemory)
			{
				for (int i = m_nSize; --i >= 0; )
					m_pMemory[i].~T();
				operator delete(m_pMemory);
			}
			m_pMemory = static_cast<T*>(operator new(nCount * sizeof(T)));
			m_nAllocationCount = nCount;
			m_nSize = 0;
			m_pElements = nullptr;
		}
	}
};

#pragma once

#include "Core.h"

namespace Frostic {

	template<typename FEVector>
	class FEVectorIterator
	{
	public:
		using ValueType = typename FEVector::ValueType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;
	public:
		FEVectorIterator(PointerType ptr)
			: m_Ptr(ptr) {}

		FEVectorIterator& operator++()
		{
			m_Ptr++;
			return *this;
		}

		FEVectorIterator& operator++(int)
		{
			FEVectorIterator iterator = *this;
			++(*this);
			return iterator;
		}

		FEVectorIterator& operator--()
		{
			m_Ptr--;
			return *this;
		}

		FEVectorIterator& operator--(int)
		{
			FEVectorIterator iterator = *this;
			--(*this);
			return iterator;
		}

		ReferenceType operator[](size_t index)
		{
			return *(m_Ptr + index);
		}

		PointerType operator->()
		{
			return	m_Ptr;
		}

		ReferenceType operator*()
		{
			return *m_Ptr;
		}

		bool operator==(const FEVectorIterator& other)
		{
			return m_Ptr == other.m_Ptr;
		}

		bool operator!=(const FEVectorIterator& other)
		{
			return !(*this == other);
		}
	private:
		PointerType m_Ptr;
	};


	template<typename T>
	class FEVector
	{
	public:
		using ValueType = T;
		using Iterator = FEVectorIterator<FEVector<ValueType>>;
	public:
		FEVector()
		{
			ReAllocate(2);
		}

		~FEVector()
		{
			Clear();
			::operator delete(m_Data, m_Capacity * sizeof(ValueType));
		}

		void PushBack(const ValueType& value)
		{
			if (m_Size >= m_Capacity)
				ReAllocate(m_Capacity + m_Capacity / 2);

			m_Data[m_Size++] = other;
		}

		void PushBack(ValueType&& value)
		{
			if (m_Size >= m_Capacity)
				ReAllocate(m_Capacity + m_Capacity / 2);

			m_Data[m_Size++] = std::move(value);
		}

		template<typename... Args>
		ValueType& EmplaceBack(Args&&... args)
		{
			if (m_Size >= m_Capacity)
				ReAllocate(m_Capacity + m_Capacity / 2);

			new(&m_Data[m_Size]) ValueType(std::forward<Args>(args)...);
			return m_Data[m_Size++];
		}

		void PopBack()
		{
			if (m_Size != 0)
			{
				m_Size--;
				m_Data[m_Size].~ValueType();
			}
		}

		void Clear()
		{
			for (size_t i = 0; i < m_Size; i++)
			{
				m_Data[i].~ValueType();
			}

			m_Size = 0;
		}

		ValueType& operator[](size_t index) { FE_CORE_ASSERT(index < m_Size, ""); return m_Data[index]; }
		const ValueType& operator[](size_t index) const { FE_CORE_ASSERT(index < m_Size, ""); return m_Data[index]; }

		size_t Size() const { return m_Size; }
		ValueType* Data() const { return m_Data; }

		Iterator begin() { return m_Data; }
		Iterator end() { return m_Data + m_Size; }

		std::string ToString() const
		{
			std::stringstream ss;
			for (size_t i = 0; i < m_Size; i++)
				ss << m_Data[i] << " ";
			return ss.str();
		}
	private:
		void ReAllocate(size_t newCapacity)
		{
			ValueType* newBlock = (ValueType*)::operator new(newCapacity * sizeof(ValueType));

			if (newCapacity < m_Size)
				m_Size = newCapacity;

			for (size_t i = 0; i < m_Size; i++)
				newBlock[i] = std::move(m_Data[i]);

			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~ValueType();

			::operator delete(m_Data, m_Capacity * sizeof(ValueType));
			m_Data = newBlock;
			m_Capacity = newCapacity;
		}
	private:
		ValueType* m_Data = nullptr;
		size_t m_Size = 0;
		size_t m_Capacity = 0;
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const FEVector<T>& vector)
	{
		return os << vector.ToString();
	}

}
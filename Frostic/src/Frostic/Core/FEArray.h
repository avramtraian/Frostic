#pragma once

#include "Core.h"

namespace Frostic {

	template<typename FEArray>
	class FEArrayIterator
	{
	public:
		using ValueType = typename FEArray::ValueType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;
	public:
		FEArrayIterator(PointerType ptr)
			: m_Ptr(ptr) {}

		FEArrayIterator& operator++()
		{
			m_Ptr++;
			return *this;
		}

		FEArrayIterator& operator++(int)
		{
			FEArrayIterator iterator = *this;
			++(*this);
			return iterator;
		}

		FEArrayIterator& operator--()
		{
			m_Ptr--;
			return *this;
		}

		FEArrayIterator& operator--(int)
		{
			FEArrayIterator iterator = *this;
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

		bool operator==(const FEArrayIterator& other)
		{
			return m_Ptr == other.m_Ptr;
		}

		bool operator!=(const FEArrayIterator& other)
		{
			return !(*this == other);
		}
	private:
		PointerType m_Ptr;
	};

	template<typename T, size_t S>
	class FEArray
	{
	public:
		using ValueType = T;
		using Iterator = FEArrayIterator<FEArray<ValueType, S>>;
	public:
		FEArray() = default;

		ValueType& operator[](size_t index) { FE_CORE_ASSERT(index < S, "");  return m_Data[index]; }
		const ValueType& operator[](size_t index) const { FE_CORE_ASSERT(index < S, "");  return m_Data[index]; }

		ValueType* Data() { return m_Data; }
		size_t Size() const { return S; }

		const std::string ToString() const
		{
			std::stringstream ss;
			for (size_t i = 0; i < S; i++)
				ss << m_Data[i] << " ";
			return ss.str();
		}

		Iterator begin() { return m_Data; }
		Iterator end() { return m_Data + S; }
	private:
		ValueType m_Data[S];
	};

	template<typename T, size_t S>
	inline std::ostream& operator<<(std::ostream& os, const FEArray<T, S>& array)
	{
		return os << array.ToString();
	}

}
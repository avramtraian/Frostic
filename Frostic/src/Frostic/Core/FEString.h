#pragma once

namespace Frostic {

	class FEString
	{
	public:
		FEString()
		{
			ReAllocate(1);
			m_Data[0] = 0;
		}
		~FEString()
		{
			::operator delete(m_Data, m_Size * sizeof(char));
		}

		FEString(const char charArray[])
		{
			size_t newSize = strlen(charArray) + 1;
			ReAllocate(newSize);
			memcpy(m_Data, charArray, m_Size);
			m_Data[m_Size - 1] = 0;
		}

		FEString(const FEString& other)
		{
			ReAllocate(other.Size());
			memcpy(m_Data, other.Data(), m_Size);
			m_Data[m_Size - 1] = 0;
		}

		// DEPRECATED
		FEString operator+(const char other[])
		{
			size_t otherSize = strlen(other);
			FEString newString;
			newString.ReAllocate(m_Size + otherSize);
			memcpy(newString.Data(), m_Data, m_Size - 1);
			memcpy(newString.Data() + m_Size - 1, other, otherSize);
			newString.Data()[newString.Size() - 1] = 0;
			return newString;
		}

		// DEPRECATED
		FEString operator+(const FEString& other)
		{
			FEString newString;
			newString.ReAllocate(m_Size + other.Size() - 1);
			memcpy(newString.Data(), m_Data, m_Size - 1);
			memcpy(newString.Data() + m_Size - 1, other.Data(), other.Size());
			newString.Data()[newString.Size() - 1] = 0;
			return newString;
		}

		FEString& Append(const char other[])
		{
			size_t otherSize = strlen(other);
			ReAllocate(m_Size + otherSize);
			memcpy(m_Data + m_Size - otherSize - 1, other, otherSize);
			m_Data[m_Size - 1] = 0;
			return *this;
		}

		FEString& Append(const FEString& other)
		{
			ReAllocate(m_Size + other.Size() - 1);
			memcpy(m_Data + m_Size - other.Size(), other.Data(), other.Size());
			m_Data[m_Size - 1] = 0;
			return *this;
		}

		bool Contains(const FEString& toFind)
		{
			if (m_Size >= toFind.Size())
			{
				for (size_t offset = 0; offset < m_Size - toFind.Size() + 1; offset++)
				{
					bool find = true;
					for (size_t i = 0; i < toFind.Size() - 1; i++)
					{
						if (m_Data[i + offset] != toFind.m_Data[i])
						{
							find = false;
							break;
						}
					}
					if (find)
						return true;
				}
			}
			return false;
		}

		bool Equals(const FEString& other)
		{
			if (m_Size == other.Size())
			{
				for (size_t i = 0; i < m_Size; i++)
				{
					if (m_Data[i] != other.m_Data[i]) return false;
				}
				return true;
			}
			return false;
		}

		void Clear()
		{
			ReAllocate(1);
			m_Data[0] = 0;
		}

		char& operator[](size_t index) { FE_CORE_ASSERT(index < m_Size, ""); return m_Data[index]; }
		const char& operator[](size_t index) const { FE_CORE_ASSERT(index < m_Size, ""); return m_Data[index]; }

		size_t Size() const { return m_Size; }
		char* Data() const { return m_Data; }
	private:
		void ReAllocate(size_t newSize)
		{
			char* newBlock = new char[newSize];
			memset(newBlock, 0, newSize * sizeof(char));

			size_t oldSize = m_Size;
			if (m_Size > newSize)
				m_Size = newSize;

			// for (size_t i = 0; i < m_Size; i++)
			// 	newBlock[i] = m_Data[i];
			memcpy(newBlock, m_Data, m_Size);

			// newBlock[m_Size - 1] = 0;

			::operator delete(m_Data, oldSize * sizeof(char));
			m_Data = newBlock;
			m_Size = newSize;
		}
	private:
		char* m_Data = nullptr;
		size_t m_Size = 0;
	};

	inline std::ostream& operator<<(std::ostream& os, const FEString& string)
	{
		return os << string.Data();
	}

}
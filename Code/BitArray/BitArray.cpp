#include "BitArray.h"

#include <cstring>

#include "../DebugFunctions/DebugFunctions.h"

BitArray::BitArray(const size_t i_numBits, const bool i_isStartClear) : m_numBits(i_numBits), m_arraySize(0), m_bits(nullptr)
{
	assert(i_numBits > 0);

	const size_t bitsPerByte = 8;
	const size_t bitwidth = sizeof(size_t) * bitsPerByte;

	m_arraySize = (m_numBits / bitwidth);

	if ((m_numBits % bitwidth) != 0)
	{
		m_arraySize += 1;
	}

	m_bits = new size_t[m_arraySize];
	memset(m_bits, i_isStartClear ? 0 : 1, sizeof(size_t));

	DEBUG_PRINT("Created Bit array with %d bits and %d bytes", m_numBits, m_arraySize);
}

BitArray::~BitArray()
{
	delete m_bits;

	DEBUG_PRINT("Deleted Bit array with %d bits and %d bytes", m_numBits, m_arraySize);
}

void BitArray::ClearAll()
{
	memset(m_bits, 0, m_arraySize);

	DEBUG_PRINT("Cleared all the %d bytes", m_arraySize);
}

void BitArray::SetAll()
{
	memset(m_bits, 1, m_arraySize);

	DEBUG_PRINT("Set all the %d bytes", m_arraySize);
}

bool BitArray::AreAllClear()
{
	for (size_t i = 0; i < m_arraySize; i++)
	{
		if (m_bits[i] != 0x00)
		{
			DEBUG_PRINT("%d byte is not clear", i);
			
			return false;
		}
	}

	DEBUG_PRINT("All the %d bits are clear", m_numBits);

	return true;
}

bool BitArray::AreAllSet()
{	
	for (size_t i = 0; i < m_arraySize; i++)
	{
		if (m_bits[i] != 0xFF)
		{
			DEBUG_PRINT("%d byte is not set", i);
			
			return false;
		}
	}

	DEBUG_PRINT("All the %d bits are set", m_numBits);

	return true;
}

bool BitArray::IsBitSet(const size_t i_bitNumber) const
{
	assert(i_bitNumber <= m_numBits);

	const size_t bitsPerByte = 8;
	const size_t bitwidth = sizeof(size_t) * bitsPerByte;
	const size_t bitpos = i_bitNumber % bitwidth;
	size_t bitArrayIndex = i_bitNumber / bitwidth;

	const size_t one = 1;
	if (m_bits[bitArrayIndex] & (one << (bitwidth - bitpos)))
	{
		DEBUG_PRINT("%d bit is set", i_bitNumber);
		
		return true;
	}
	else
	{
		DEBUG_PRINT("%d bit is not set", i_bitNumber);
		
		return false;
	}
}

bool BitArray::IsBitClear(const size_t i_bitNumber) const
{
	assert(i_bitNumber <= m_numBits);

	const size_t bitsPerByte = 8;
	const size_t bitwidth = sizeof(size_t) * bitsPerByte;
	const size_t bitpos = i_bitNumber % bitwidth;
	size_t bitArrayIndex = i_bitNumber / bitwidth;

	const size_t one = 1;
	if (m_bits[bitArrayIndex] & (one << (bitwidth - bitpos)))
	{
		DEBUG_PRINT("%d bit is not clear", i_bitNumber);
		
		return false;
	}
	else
	{
		DEBUG_PRINT("%d bit is clear", i_bitNumber);
		
		return true;
	}
}

void BitArray::SetBit(const size_t i_bitNumber)
{
	assert(i_bitNumber <= m_numBits);

	const size_t bitsPerByte = 8;
	const size_t bitwidth = sizeof(size_t) * bitsPerByte;
	const size_t bitpos = i_bitNumber % bitwidth;
	size_t bitArrayIndex = i_bitNumber / bitwidth;

	const size_t one = 1;
	m_bits[bitArrayIndex] = m_bits[bitArrayIndex] | (one << (bitwidth - bitpos));

	DEBUG_PRINT("Set %d bit", i_bitNumber);
}

void BitArray::ClearBit(const size_t i_bitNumber)
{
	assert(i_bitNumber <= m_numBits);

	const size_t bitsPerByte = 8;
	const size_t bitwidth = sizeof(size_t) * bitsPerByte;
	const size_t bitpos = i_bitNumber % bitwidth;
	size_t bitArrayIndex = i_bitNumber / bitwidth;
	
	const size_t one = 1;
	m_bits[bitArrayIndex] = m_bits[bitArrayIndex] & (~(one << (bitwidth - bitpos)));

	DEBUG_PRINT("Clear %d bit", i_bitNumber);
}

bool BitArray::GetFirstSetBit(size_t& o_bitNumber) const
{
	const size_t bitsPerByte = 8;
	const size_t bitwidth = sizeof(size_t) * bitsPerByte;

	for (size_t i = 0; i < m_arraySize; i++)
	{
		for (size_t j = (bitwidth - m_numBits - 1); j < bitwidth; j++)
		{
			const size_t one = 1;
			if (m_bits[i] & (one << j))
			{
				o_bitNumber = (i * bitwidth) + j;

				DEBUG_PRINT("First set bit is at %d", o_bitNumber);

				return true;
			}
		}
	}

	DEBUG_PRINT("No bits are set");

	return false;
}

bool BitArray::GetFirstClearbit(size_t& o_bitNumber) const
{
	const size_t bitsPerByte = 8;
	const size_t bitwidth = sizeof(size_t) * bitsPerByte;

	for (size_t i = 0; i < m_arraySize; i++)
	{
		for (size_t j = (bitwidth - 1); j >= (bitwidth - m_numBits); j--)
		{
			const size_t one = 1;
			if ((m_bits[i] | (~(one << j))) != ~(one - 1))
			{
				o_bitNumber = (i * bitwidth) + (bitwidth - j);

				DEBUG_PRINT("First clear bit is at %d", o_bitNumber);

				return true;
			}
		}
	}

	DEBUG_PRINT("No bits are clear");

	return false;
}
#ifndef BITARRAY_H
#define BITARRAY_H

#include <stdint.h>

class BitArray
{
public:
	BitArray(const size_t i_numBits, const bool i_isStartClear);
	~BitArray();

	void ClearAll();
	void SetAll();

	bool AreAllClear();
	bool AreAllSet();

	bool IsBitSet(const size_t i_bitNumber) const;
	bool IsBitClear(const size_t i_bitNumber) const;

	void SetBit(const size_t i_bitNumber);
	void ClearBit(const size_t i_bitNumber);

	bool GetFirstSetBit(size_t& o_bitNumber) const;
	bool GetFirstClearbit(size_t& o_bitNumber) const;

private:
	BitArray(const BitArray& i_bitArray);
	BitArray& operator= (const BitArray& i_bitArray);

	const size_t m_numBits;
	size_t m_arraySize;
	size_t* m_bits;
};

#endif
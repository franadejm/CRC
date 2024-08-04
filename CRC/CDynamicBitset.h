#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <climits>
#include <bitset>
#include <algorithm>


using ull = unsigned long long;

class CDynamicBitset
{
private:
	std::vector<ull> data;
	static const ull bitsInUll = sizeof(ull) * 8;
	void findBit(ull& position, ull& bitIdx) const;
	void deleteLeadingZeros();
	const std::vector<ull>& getData() const;
	static inline int HammingWeight(ull item);

public:
	CDynamicBitset();
	CDynamicBitset(ull val);
	CDynamicBitset(int val);
	CDynamicBitset(std::string& s);
	CDynamicBitset(const char* s);
	CDynamicBitset(const CDynamicBitset& dbs);
	CDynamicBitset(CDynamicBitset&& dbs);
	size_t size() const;
	CDynamicBitset& setBit(ull position, ull value=1);
	CDynamicBitset& flipBit(ull position);
	int at(ull position) const;
	bool empty() const;
	ull mostSignificantBit() const;
	int HammingWeight() const;
	std::string dump() const;
	CDynamicBitset& operator = (const CDynamicBitset& dbs);
	CDynamicBitset& operator = (CDynamicBitset&& dbs);
	bool operator == (const CDynamicBitset& dbs) const;
	CDynamicBitset operator >> (const ull shift) const;
	CDynamicBitset operator << (const ull shift) const;
	CDynamicBitset& operator <<= (ull shift);
	CDynamicBitset& operator >>= (ull shift);
	CDynamicBitset operator & (const CDynamicBitset& dbs) const;
	CDynamicBitset operator | (const CDynamicBitset& dbs) const;
	CDynamicBitset operator ^ (const CDynamicBitset& dbs) const;
	CDynamicBitset& operator &= (const CDynamicBitset& dbs);
	CDynamicBitset& operator |= (const CDynamicBitset& dbs);
	CDynamicBitset& operator ^= (const CDynamicBitset& dbs);
	friend std::ostream& operator << (std::ostream& os, const CDynamicBitset dbs);
};


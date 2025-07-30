#include "CDynamicBitset.h"
#include <iomanip>

CDynamicBitset::CDynamicBitset() : data({0})
{
}

CDynamicBitset::CDynamicBitset(ull val) : data ({ val })
{
}

CDynamicBitset::CDynamicBitset(int val) : data({ (ull)val })
{
}

CDynamicBitset::CDynamicBitset(std::string& s) 
{
	ull charsToCopy = s.size();
	ull charsInUll = sizeof(ull) / sizeof(char);
	
	// (a + b - 1) / b == ceil(a / b)
	ull requiredVectorSize = (charsToCopy + charsInUll - 1) / charsInUll;  

	data.resize(requiredVectorSize, 0);
	std::memcpy(data.data(), s.data(), charsToCopy);
}

CDynamicBitset::CDynamicBitset(const char* s)
{
	std::string tmp(s);
	*this = CDynamicBitset(tmp);
}

CDynamicBitset::CDynamicBitset(const CDynamicBitset& dbs) : data(dbs.data){}

CDynamicBitset::CDynamicBitset(CDynamicBitset&& dbs) : data(std::move(dbs.data))
{
}

void CDynamicBitset::findBit(ull& position, ull& bitIdx) const
{
	if (position >= size())
		throw std::out_of_range("Position out of range");

	bitIdx = position % bitsInUll;
	position /= bitsInUll;
	position = data.size() - position - 1;
}

void CDynamicBitset::deleteLeadingZeros()
{
	ull num = 0;
	for (auto it = data.begin(); it != data.end() - 1 && *it == 0; it++)
		num++;

	data.erase(data.begin(), data.begin() + num);
}

const std::vector<ull>& CDynamicBitset::getData() const { return data; }

inline int CDynamicBitset::HammingWeight(ull item)
{
	int HammingWeight = 0;
	ull bitmask = 1;
	while (item)
	{
		HammingWeight += item & bitmask;
		item >>= 1;
	}
	return HammingWeight;
}

size_t CDynamicBitset::size() const { return this->data.size() * bitsInUll; }

CDynamicBitset& CDynamicBitset::setBit(ull position, ull value)
{
	if (value != 1 && value != 0)
		throw std::invalid_argument("Value must be 0 or 1");

	ull bitIdx;

	if (position >= size())
	{
		ull availableBits = size();
		ull bitsToAdd = position - availableBits + 1;
		ull ullToAdd = (bitsToAdd + bitsInUll - 1) / bitsInUll;
		data.insert(data.begin(), ullToAdd, 0);
	}
	findBit(position, bitIdx);
	
	ull bitmask = 1;
	bitmask <<= bitIdx;

	if (value == 1)
		data.at(position) |= bitmask;

	else
	{
		bitmask = ~bitmask;
		data.at(position) &= bitmask;
	}

	this->deleteLeadingZeros();
	return *this;
}

CDynamicBitset& CDynamicBitset::flipBit(ull position)
{
	if (at(position))
		setBit(position, 0);
	else setBit(position);
	return *this;
}

int CDynamicBitset::at(ull position) const
{
	if (position >= size())
		return 0;

	ull bitIdx, bitmask = 1;
	findBit(position, bitIdx);
	bitmask <<= bitIdx;
	if (data.at(position) & bitmask)
		return 1;
	return 0;
}

bool CDynamicBitset::empty() const
{
	for (const ull& item : getData())
		if (item != 0) return false;
	return true;
}

ull CDynamicBitset::mostSignificantBit() const
{
	if (*this == 0) return -1;

	ull position = 0;
	ull tmp = *getData().begin();
	
	while (tmp >>= 1)
		position++;
	
	position += size() - bitsInUll;
	return position;
}

int CDynamicBitset::HammingWeight() const
{
	int HammingWeight = 0;
	for (ull item : getData())
		HammingWeight += CDynamicBitset::HammingWeight(item);

	return HammingWeight;
}

std::string CDynamicBitset::dump() const
{
	// # ull to dump * bytes in ull
	size_t bytes = data.size() * sizeof(ull); 
	char* tmp = new char[bytes+1];
	std::memcpy((void*)tmp, (const void*)getData().data(), bytes);
	tmp[bytes] = 0;
	std::string dmp = std::move(tmp);
	return dmp;
}

CDynamicBitset& CDynamicBitset::operator=( const CDynamicBitset& dbs)
{
	data = dbs.data;
	return *this;
}

CDynamicBitset& CDynamicBitset::operator=(CDynamicBitset&& dbs)
{
	data = std::move(dbs.data);
	return *this;
}

bool CDynamicBitset::operator==(const CDynamicBitset& dbs) const { return (data == dbs.data); }



/*

general idea of >> operator:
	
	vector<ull> data = { first, second, ... , secondLast, last }

	if some ulls fully underflow 
		erase several elements from the end
	
	last  >>= shift

	last += secondLast underflow
	secondLast >>= shift
	.
	.
	.
	second += first underflow
	first >>= shift

	erase leading zeros


general idea << operator:

	vector<ull> data = { first, second, ... , secondLast, last }
	
	if some ulls fully overflow
		insert elements at the end

	if (first underflows)
		insert first underflow
	
	first <<= shift

	first += second overlow
	second <<= shift

	.
	.
	.
	secondLast += last underflow
	last <<= shift
	
	
*/


CDynamicBitset CDynamicBitset::operator >> (const ull shft) const
{
	CDynamicBitset res(*this);

	ull shift = shft;

	ull fullUllUnderflows = shift / bitsInUll;
	shift %= bitsInUll;

	if (fullUllUnderflows >= res.getData().size()) {
		res.data.resize(1, 0);
		return res;
	}
	
	res.data.erase(res.data.end() - fullUllUnderflows, res.data.end());

	if (!shift)
		return res;

	ull underflow;
	ull bitmask = std::numeric_limits<ull>::max();
	bitmask >>= bitsInUll - shift;


	for (auto it = res.data.rbegin(); it != res.data.rend(); it++)
	{
		underflow = *it & bitmask;
		underflow <<= bitsInUll - shift;
		*it >>= shift;
		if (it != res.data.rbegin())
			*(it - 1) += underflow;
	}

	res.deleteLeadingZeros();
	return res;
}

CDynamicBitset CDynamicBitset::operator << (const ull shft) const
{
	CDynamicBitset res(*this);
	
	ull shift = shft;

	ull fullOverflows = shift / bitsInUll;
	shift %= bitsInUll;

	if (fullOverflows)
		res.data.insert(res.data.end(), fullOverflows, 0);

	if (!shift)
		return res;

	ull overflow;
	ull bitmask = std::numeric_limits<ull>::max();
	bitmask <<= (bitsInUll - shift);

	for (auto it = res.data.begin(); it != res.data.end(); it++)
	{
		overflow = bitmask & *it;
		overflow >>= (bitsInUll - shift);
		*it <<= shift;

		if (overflow)
		{
			if (it == res.data.begin())
			{														
				res.data.insert(res.data.begin(), overflow);   // { first, ... } -> { overflow, first, ... } , it undefined
				it = res.data.begin() + 1;//						      it -^
			}
			else *(it - 1) += overflow;   // { ...., *(it - 1), *it, ... } -> { ..., *(it -1) + overflow, *it ... } 
		}
	}
	return res;
}


CDynamicBitset& CDynamicBitset::operator <<= (ull shift)
{
	*this = std::move(*this << shift);
	return *this;
}

CDynamicBitset& CDynamicBitset::operator >>= (ull shift)
{
	*this = std::move(*this >> shift);
	return *this;
}

CDynamicBitset CDynamicBitset::operator & (const CDynamicBitset& dbs) const
{
	ull len1 = data.size(), len2 = dbs.data.size();
	CDynamicBitset res;
	const CDynamicBitset* other;
	
	if (len1 >= len2)
	{
		res = *this;
		other = &dbs;
	}
	else
	{
		res = dbs;
		other = this;
	}
	std::cout  << *other << std::endl;

	auto it2 = other->data.rbegin();
	for (auto it = res.data.rbegin(); it != res.data.rend() && it2 != other->data.rend(); it++, it2++)
	{				// res = { item01, item02, item03, item04 }
		*it &= *it2;	        //*other =               { item05, item06 }
	}				// res = { item01, item02, item03 & item05, item04 & item06 }

	res.deleteLeadingZeros();

	return res;
}

CDynamicBitset CDynamicBitset::operator | (const CDynamicBitset& dbs) const
{
	ull len1 = data.size(), len2 = dbs.data.size();
	CDynamicBitset res;
	const CDynamicBitset* other;

	if (len1 >= len2)
	{
		res = *this;
		other = &dbs;
	}
	else
	{
		res = dbs;
		other = this;
	}
	std::cout << *other << std::endl;

	auto it2 = other->data.rbegin();
	for (auto it = res.data.rbegin(); it != res.data.rend() && it2 != other->data.rend(); it++, it2++)
	{				// res = { item01, item02, item03, item04 }
		*it |= *it2;		//*other =               { item05, item06 }
	}				// res = { item01, item02, item03 | item05, item04 | item06 }

	return res;
}

CDynamicBitset CDynamicBitset::operator^(const CDynamicBitset& dbs) const
{
	ull len1 = data.size(), len2 = dbs.data.size();
	CDynamicBitset res;
	const CDynamicBitset* other;

	if (len1 >= len2)
	{
		res = *this;
		other = &dbs;
	}
	else
	{
		res = dbs;
		other = this;
	}
	
	auto it2 = other->data.rbegin();
	for (auto it = res.data.rbegin(); it != res.data.rend() && it2 != other->data.rend(); it++, it2++)
	{			         // res = { item01, item02, item03, item04 }
		*it ^= *it2;		 //*other =               { item05, item06 }
	}				 // res = { item01, item02, item03 ^ item05, item04 ^ item06 }

	res.deleteLeadingZeros();
	return res;

}

CDynamicBitset& CDynamicBitset::operator&=(const CDynamicBitset& dbs)
{
	*this = *this & dbs;
	return *this;
}

CDynamicBitset& CDynamicBitset::operator|=(const CDynamicBitset& dbs)
{
	*this = *this | dbs;
	return *this;
}

CDynamicBitset& CDynamicBitset::operator^=(const CDynamicBitset& dbs)
{
	*this = *this ^ dbs;
	return *this;
}

std::ostream& operator << (std::ostream& os, const CDynamicBitset dbs)
{
	os << std::hex;
	for (const ull& item : dbs.getData())
		os << std::setfill('0') << std::setw(16) << item;
	return os;
}


#pragma once
#include "CBinaryPolynomial.h"
#include <random>

using cbp = CBinaryPolynomial;
using ull = unsigned long long;

class CRC
{
  private:
	cbp message;
	bool isValid(const cbp& generator) const;

  public:
	CRC() = default;
	CRC(cbp& cbp);
	CRC(cbp&& cbp);
	CRC(const CRC& crc);
	CRC(CRC&& crc);
	CRC& operator = (CRC& crc);
	CRC& operator = (CRC&& crc);
	friend std::ostream& operator << (std::ostream& os, CRC& crc);
	CRC& encode(const cbp& generator=CRC::CRC_8());
	CRC& decode(const cbp& generator=CRC::CRC_8());
	void createErrors(unsigned int numOfErrors);
	std::string read() const;
	static cbp CRC_8();
	static cbp CRC_16();
	static cbp CRC_32C();
	static cbp CRC_64_ISO();
};


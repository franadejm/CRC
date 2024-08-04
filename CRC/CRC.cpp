#include "CRC.h"

CRC::CRC(cbp& cbp) : message(cbp)
{
}

CRC::CRC(cbp&& cbp) : message(std::move(cbp))
{
}

CRC::CRC(const CRC& crc) : message(crc.message)
{
}

CRC::CRC(CRC&& crc) : message(std::move(crc.message))
{
}

CRC& CRC::operator=(CRC& crc)
{
	this->message = crc.message;
	return *this;
}

CRC& CRC::operator=(CRC&& crc)
{
	message = std::move(crc.message);
	return *this;
}

std::ostream& operator<<(std::ostream& os, CRC& crc)
{
	os << crc.message;
	return os;
}

CRC& CRC::encode(const cbp& generator)
{
	message *= cbp::xToThe(generator.degree()); // multiply by x^(highest possible degree of remainder)
//	std::cout << message << std::endl;
	message += (message % generator);		// add remainder after division by generator
//	std::cout << message << std::endl << std::endl;
	return *this;
}

CRC& CRC::decode(const cbp& generator)
{
	if (!isValid(generator))
		throw std::invalid_argument("    [LOG] Errors detected");

	message /= (1 << (generator.degree()));
	return *this;
}

void CRC::createErrors(unsigned int numOfErrors)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, message.getCoefficients().size()-1);

	for (unsigned int i = 0; i < numOfErrors; i++)
	{
		cbp noise = cbp::xToThe(dist(rng));
		message += noise;
	}
}

std::string CRC::read() const
{
	return std::move(message.getCoefficients().dump());
}

cbp CRC::CRC_8()
{
	return 0x107;
}

cbp CRC::CRC_16()
{
	return 0x11021;
}

cbp CRC::CRC_32C()
{
	return (ull) 0x11edc6f41;
}

cbp CRC::CRC_64_ISO()
{
	return ((ull)1 << 63) + 0x1b;
}

bool CRC::isValid(const cbp& generator) const
{
	return (message % generator) == 0;
}



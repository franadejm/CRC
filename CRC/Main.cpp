#include <iostream>
#include "CRC.h"
#include <assert.h>
#include <bitset>

using uint = unsigned int;

void printUInt(uint i)
{
	int bitmask = 0xFF;
	while (i != 0)
	{
		char ctrl = i & bitmask;
		std::cout << ctrl;
		i >>= (sizeof(char) * 8);
	}
}

void dumpStr(std::string str)
{
	size_t len = str.length();
	for (size_t i = 0; i < len; i++)
		std::cout << std::bitset<sizeof(char) * 8>(str.data()[i]);
	std::cout << std::endl;
}

void strInfo(std::string s)
{
	std::cout << "  Adress          Char     ASCII" << std::endl;
	for (const char* c = s.data(); *c != 0; c++)
	std::cout << std::hex << (ull)c << "        " << *c << "        " << std::bitset<sizeof(char) * 8>(*c) << std::endl;
}


void test()
{
	cbp a = 4;
	cbp b = 1;

	assert(a * b == 4);
	assert(a + b == 5);
	assert(a + a + b == b);
	assert((b *= 2) == 2);
	assert((b += 1 )== 3);
	assert(a * b == 0b1100);
	
	a = 4;
	b = 1;
	assert(a / b == 4);
	b = 2;
	assert(a / b == 2);
	a = 11;
	b = 3;
	assert(a / b == 0b110);
	assert(a % b == 1);
	a = "Hello";
	b = CRC::CRC_8();
	assert(a / b == 0b1101110011001110101100011101111);
	assert(a % b == 0b11000101);

	assert(cbp::xToThe(4) == 16);
	assert(cbp::xToThe(2) * a ==(ull) 0b11011110110110001101100011001010100100000);
	assert(cbp::xToThe(3) * a == (ull)0b110111101101100011011000110010101001000000);
	assert(cbp::xToThe(4) * a == (ull)0b1101111011011000110110001100101010010000000);
	assert(cbp::xToThe(5) * a == (ull)0b11011110110110001101100011001010100100000000);
	assert(cbp::xToThe(6) * a == (ull)0b110111101101100011011000110010101001000000000);
	assert(cbp::xToThe(7) * a == (ull)0b1101111011011000110110001100101010010000000000);	
	assert(a * cbp::xToThe(2) == (ull)0b11011110110110001101100011001010100100000);
	assert(a * cbp::xToThe(3) == (ull)0b110111101101100011011000110010101001000000);
	assert(a * cbp::xToThe(4) == (ull)0b1101111011011000110110001100101010010000000);
	assert(a * cbp::xToThe(5) == (ull)0b11011110110110001101100011001010100100000000);
	assert(a * cbp::xToThe(6) == (ull)0b110111101101100011011000110010101001000000000);
	assert(a * cbp::xToThe(7) == (ull)0b1101111011011000110110001100101010010000000000);

	a = "12345678";
	//assert(a*cbp::xToThe(2) == (ull)0b0011100000110111001101100011010100110100001100110011001000110001)

}

void interface()
{
	std::cout << "Enter message to send: ";
	std::string mes;
	std::getline(std::cin, mes);
	CRC a(mes);
	std::cout << "    [LOG] Encoding message" << std::endl;
	a.encode();
	std::cout << "Enter number of transmission errors: ";
	int err;
	std::cin >> err;
	std::cout << "    [LOG] Simulating errors" << std::endl;
	a.createErrors(err);
	std::cout << "    [LOG] Decoding" << std::endl;
	try
	{
		a.decode();
		std::cout << "Recieved message: \"" << a.read() << "\"" << std::endl;
	}
	catch (std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
	}

}

int main()
{
	interface();
	//test();
	return 0;
}
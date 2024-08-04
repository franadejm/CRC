#include "CBinaryPolynomial.h"

CBinaryPolynomial CBinaryPolynomial::xToThe(unsigned int power)
{
	cdb res = 1;
	return res << power;
}

CBinaryPolynomial::CBinaryPolynomial(int val) : coefficients(val)
{
}

CBinaryPolynomial::CBinaryPolynomial(ull val) : coefficients(val)
{
}

CBinaryPolynomial::CBinaryPolynomial(std::string str) : coefficients(str)
{
}

CBinaryPolynomial::CBinaryPolynomial(const char* str) : coefficients(str)
{
}

CBinaryPolynomial::CBinaryPolynomial(const CDynamicBitset& cdb) : coefficients(cdb)
{
}

CBinaryPolynomial::CBinaryPolynomial(CDynamicBitset&& cdb) : coefficients(std::move(cdb))
{
}

CBinaryPolynomial::CBinaryPolynomial(const CBinaryPolynomial& cbp) : coefficients(cbp.coefficients)
{
}

CBinaryPolynomial::CBinaryPolynomial(CBinaryPolynomial&& cbp) : coefficients(std::move(cbp.coefficients))
{
}

CBinaryPolynomial& CBinaryPolynomial::operator=(const CBinaryPolynomial& cbp)
{
	coefficients = cbp.coefficients;
	return *this;
}

CBinaryPolynomial& CBinaryPolynomial::operator=(CBinaryPolynomial&& cbp)
{
	if (this != &cbp)
		coefficients = std::move(cbp.coefficients);
	
	return *this;
}

bool CBinaryPolynomial::operator==(const CBinaryPolynomial& cbp) const
{
	return coefficients == cbp.coefficients;
}

CBinaryPolynomial CBinaryPolynomial::operator+(const CBinaryPolynomial& cbp) const
{
	CBinaryPolynomial res = *this;
	res.coefficients ^= cbp.coefficients;
	return res;
}

CBinaryPolynomial CBinaryPolynomial::operator-(const CBinaryPolynomial& cbp) const
{
	return *this + cbp;
}

CBinaryPolynomial& CBinaryPolynomial::operator+=(const CBinaryPolynomial& cbp)
{
	*this = (*this + cbp);
	return *this;
}

CBinaryPolynomial& CBinaryPolynomial::operator-=(const CBinaryPolynomial& cbp)
{
	return (*this += cbp);
}

CBinaryPolynomial CBinaryPolynomial::operator*(const CBinaryPolynomial& cbp) const
{
	CBinaryPolynomial res = 0;
	ull degree = this->degree();
	for (ull idx = 0; idx <= degree; idx++)
	{
		if (coefficients.at(idx))
		{
			CBinaryPolynomial tmp = (cbp.coefficients << idx);
	//		std::cout << "  " << cbp.coefficients << "<< " << idx << std::endl << "= " << tmp << std::endl << std::endl;
	//		std::cout << "  " << res << std::endl << " +" << tmp << std::endl << "  ";
	//		for (int i = 0; i < 64; i++)
	//			std::cout << "-";
			
			res += tmp;
	//		std::cout << std::endl << "  " << res << std::endl << std::endl;
		}
	}
	return res;
}

CBinaryPolynomial& CBinaryPolynomial::operator*=(const CBinaryPolynomial& cbp)
{
	*this = (*this * cbp);
	return *this;
}

CBinaryPolynomial CBinaryPolynomial::operator/(const CBinaryPolynomial& quotient) const
{
	if (quotient == 0)
		throw std::invalid_argument("Division by zero");

	CBinaryPolynomial numerator = *this;
	CBinaryPolynomial res(0);
	int quotientDegree = quotient.degree();
	int numeratorDegree = numerator.degree();

	while (numeratorDegree >= quotientDegree)
	{
		res.coefficients.setBit(numeratorDegree - quotientDegree);
		numerator -= quotient * xToThe(numeratorDegree - quotientDegree);
		numeratorDegree = numerator.degree();
	}
	return res;
}

CBinaryPolynomial& CBinaryPolynomial::operator/=(const CBinaryPolynomial& cbp)
{
	*this = (*this / cbp);
	return *this;
}

CBinaryPolynomial CBinaryPolynomial::operator%(const CBinaryPolynomial& quotient) const
{
	if (quotient == 0)
		throw std::invalid_argument("Division by zero");

	CBinaryPolynomial numerator = *this;
	int quotientDegree = quotient.degree();
	int numeratorDegree = numerator.degree();

	while (numeratorDegree >= quotientDegree)
	{
		numerator -= quotient * xToThe(numeratorDegree - quotientDegree);
		numeratorDegree = numerator.degree();
	}
	return numerator;
}

CBinaryPolynomial& CBinaryPolynomial::operator%=(const CBinaryPolynomial& cbp)
{
	*this = (*this % cbp);
	return *this;
}

int CBinaryPolynomial::valueAt(int x) const
{
	if (x != 0 && x != 1)
		throw std::invalid_argument("X must be 0 or 1");
	
	if (x == 1) return coefficients.HammingWeight();
	else return coefficients.at(0);
}

bool CBinaryPolynomial::isRoot(int x) const
{
	if (x != 0 && x != 1)
		return false;

	return valueAt(x) == 0;
}

bool CBinaryPolynomial::isZero() const
{
	return coefficients.empty();
}

const cdb& CBinaryPolynomial::getCoefficients() const
{
	return coefficients;
}

int CBinaryPolynomial::degree() const
{
	return coefficients.mostSignificantBit(); 
}

std::ostream& operator<<(std::ostream& os, CBinaryPolynomial cbp)
{
	os << cbp.coefficients;
	return os;
}

#pragma once
#include "CDynamicBitset.h"


using cdb = CDynamicBitset;

class CBinaryPolynomial
{
  private:
	cdb coefficients = 0;
	
public:
	CBinaryPolynomial() = default;
	CBinaryPolynomial(int val);
	CBinaryPolynomial(ull val);
	CBinaryPolynomial(std::string str);
	CBinaryPolynomial(const char* str);
	CBinaryPolynomial(const CDynamicBitset& cdb);
	CBinaryPolynomial(CDynamicBitset&& cdb);
	CBinaryPolynomial(const CBinaryPolynomial& cbp);
	CBinaryPolynomial(CBinaryPolynomial&& cbp);
	static CBinaryPolynomial xToThe(unsigned int power);
	CBinaryPolynomial& operator = (const CBinaryPolynomial& cbp);
	CBinaryPolynomial& operator = (CBinaryPolynomial&& cbp);
	bool operator == (const CBinaryPolynomial& cbp) const;
	CBinaryPolynomial  operator +  (const CBinaryPolynomial& cbp) const;
	CBinaryPolynomial& operator += (const CBinaryPolynomial& cbp);
	CBinaryPolynomial  operator -  (const CBinaryPolynomial& cbp) const;
	CBinaryPolynomial& operator -= (const CBinaryPolynomial& cbp);
	CBinaryPolynomial  operator *  (const CBinaryPolynomial& cbp) const;
	CBinaryPolynomial& operator *= (const CBinaryPolynomial& cbp);
	CBinaryPolynomial  operator /  (const CBinaryPolynomial& cbp) const;
	CBinaryPolynomial& operator /= (const CBinaryPolynomial& cbp);
	CBinaryPolynomial  operator %  (const CBinaryPolynomial& cbp) const;
	CBinaryPolynomial& operator %= (const CBinaryPolynomial& cbp);
	friend std::ostream& operator << (std::ostream& os, CBinaryPolynomial cbp);
	int valueAt(int x) const;
	bool isRoot(int x) const;
	bool isZero() const;
	const cdb& getCoefficients() const;
	int degree() const;
	
};
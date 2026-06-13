#ifndef CLASSDROBI_H
#define CLASSDROBI_H

#include <iostream>
#include "biginteger.hpp"


class Fraction {

private:
	BigInteger nominator_;
	BigInteger denominator_;

	static BigInteger gcd(BigInteger a, BigInteger b) {
		a = a.abs();
		b = b.abs();
		while (b != 0) {
			BigInteger temp = b;
			b = a % b;
			a = temp;
		}
		return a;
	}

	void normalize() {

		if (denominator_ == 0) {
			throw std::runtime_error("Denominator can not be a zero value");
		}

		if (denominator_ < 0) {
			nominator_ = -nominator_;
			denominator_ = -denominator_;

		}
		//сокращение дроби
		BigInteger divisor = gcd(nominator_, denominator_);
		nominator_ = nominator_ / divisor;
		denominator_ = denominator_ / divisor;
	}	

public:

	Fraction(BigInteger nominator, BigInteger denominator) : nominator_(std::move(nominator)), denominator_(std::move(denominator)) {
		normalize();
	}

	explicit Fraction(BigInteger nominator) : nominator_(std::move(nominator)), denominator_(1) {
		normalize();
	}

	Fraction(const Fraction& other) {
		nominator_ = other.nominator_;
		denominator_ = other.denominator_;
	}


	Fraction(int nominator) : nominator_(BigInteger(nominator)), denominator_(BigInteger(1)) {
		normalize();
	}

	Fraction() : nominator_(1), denominator_(1) {
		normalize();
	}

	Fraction(Fraction&& other) noexcept = default;
	Fraction& operator=(const Fraction& other) = default;
	Fraction& operator=(Fraction&& other) noexcept = default;

	Fraction operator+() const { return *this; }
	Fraction operator-() const { return Fraction(-nominator_, denominator_); }
	
	~Fraction() = default;
	Fraction& operator+=(const Fraction& rhs);

	Fraction& operator-=(const Fraction& rhs);

	Fraction& operator*=(const Fraction& rhs);

	Fraction& operator/=(const Fraction& rhs);

	friend bool operator==(const Fraction& lhs, const Fraction& rhs);
	friend bool operator!=(const Fraction& lhs, const Fraction& rhs);
	friend bool operator<(const Fraction& lhs, const Fraction& rhs);
	friend bool operator>(const Fraction& lhs, const Fraction& rhs);

	friend std::ostream& operator<<(std::ostream& os, const Fraction& f);
	friend std::istream& operator>>(std::istream& is, Fraction& f);

	BigInteger getNominator() const {
		return nominator_;
	}
	
	BigInteger getDenominator() const {
		return denominator_;
	}
};

Fraction operator+(Fraction lhs, const Fraction& rhs);

Fraction operator-(Fraction lhs, const Fraction& rhs);

Fraction operator*(Fraction lhs, const Fraction& rhs);

Fraction operator/(Fraction lhs, const Fraction& rhs);

#endif



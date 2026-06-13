#include "classDrobi.h"

Fraction& Fraction::operator+=(const Fraction& rhs) {
	nominator_ = (nominator_ * rhs.denominator_) + (rhs.nominator_ * denominator_);
	denominator_ = (denominator_ * rhs.denominator_);
	normalize();
	return *this;
}

Fraction& Fraction::operator-=(const Fraction& rhs) {
	nominator_ = (nominator_ * rhs.denominator_) - (rhs.nominator_ * denominator_);
	denominator_ = (denominator_ * rhs.denominator_);
	normalize();
	return *this;
}

Fraction& Fraction::operator*=(const Fraction& rhs) {
	nominator_ = nominator_ * rhs.nominator_;
	denominator_ = denominator_ * rhs.denominator_;
	normalize();
	return *this;
}

Fraction& Fraction::operator/=(const Fraction& rhs) {
	nominator_ = nominator_ * rhs.denominator_;
	denominator_ = denominator_ * rhs.nominator_;
	normalize();
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Fraction& f) {
	os << to_string(f.nominator_);
	if (f.denominator_ != BigInteger(1)) {
		os << '/' << to_string(f.denominator_);
	}
	return os;
}

std::istream& operator>>(std::istream& is, Fraction& f) {
	int num = 0;
	int denom = 1;
	char slash = 0;

	if (is >> num) {
		if (is.peek() == '/') {
			is >> slash >> denom;
		}
		f = Fraction(num, denom);
	}
	return is;
}

Fraction operator+(Fraction lhs, const Fraction& rhs) {
	lhs += rhs;
	return lhs;
}

Fraction operator-(Fraction lhs, const Fraction& rhs) {
	lhs -= rhs;
	return lhs;
}

Fraction operator*(Fraction lhs, const Fraction& rhs) {
	lhs *= rhs;
	return lhs;
}

Fraction operator/(Fraction lhs, const Fraction& rhs) {
	lhs /= rhs;
	return lhs;
}

bool operator==(const Fraction& lhs, const Fraction& rhs) {
	return (lhs.nominator_ == rhs.nominator_ && lhs.denominator_ == rhs.denominator_);
}

bool operator!=(const Fraction& lhs, const Fraction& rhs) {
	return !(lhs == rhs);
}

bool operator<(const Fraction& lhs, const Fraction& rhs) {
	BigInteger left = lhs.nominator_ * rhs.denominator_;
	BigInteger right = rhs.nominator_ * lhs.denominator_;
	return left < right;
}

bool operator>(const Fraction& lhs, const Fraction& rhs) {
	return rhs < lhs;
}




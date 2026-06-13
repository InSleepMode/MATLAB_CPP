#include "biginteger.hpp"
#include <cstring>
#include <cstdint>
#include <string>
#include <limits>


//Увеличение размера массива
void BigInteger::enhance(size_t new_size) {

    uint32_t* new_num = new uint32_t[new_size]();

    for (size_t i = 0; i < size; ++i) {
        new_num[i] = num[i];
    }

    delete[] num;
    num = new_num;
    size = new_size;
}

void BigInteger::removeZeros() {

    size_t new_size = size;
    while (new_size > 1 && num[new_size - 1] == 0) {
        new_size--;
    }

    if (new_size != size) {
        uint32_t* new_num = new uint32_t[new_size];
        for (size_t i = 0; i < new_size; ++i) {
            new_num[i] = num[i];
        }
        delete[] num;
        num = new_num;
        size = new_size;
    }

    if (size == 1 && num[0] == 0) {
        sign = false;
    }
}

//конструктор по умолчанию
BigInteger::BigInteger() : size(1), num(new uint32_t[1]()), sign(false)
{
    num[0] = 0;
}

// Оператор присваивания копированием
BigInteger& BigInteger::operator=(const BigInteger& other) {
    if (this != &other) {
        delete[] num;
        sign = other.sign;
        size = other.size;

        num = new uint32_t[size];
        for (size_t i = 0; i < size; ++i) {
            num[i] = other.num[i];
        }
    }
    return *this;
}

// Оператор присваивания перемещением
BigInteger& BigInteger::operator=(BigInteger&& other) noexcept {
    if (this != &other) {
        delete[] num;
        num = other.num;
        sign = other.sign;
        size = other.size;
        other.num = nullptr;
        other.size = 0;
        other.sign = false;
    }
    return *this;
}

// Операторы сравнения
bool operator==(const BigInteger& left, const BigInteger& right) {

    if (left.sign != right.sign || left.size != right.size) {
        return false;
    }

    for (size_t i = 0; i < left.size; ++i) {
        if (left.num[i] != right.num[i]) {
            return false;
        }
    }



    return true;
}

bool operator!=(const BigInteger& left, const BigInteger& right) {
    return !(left == right);
}

bool operator<(const BigInteger& left, const BigInteger& right) {
    if (left.sign != right.sign) {
        return left.sign;
    }

    bool twoNegativeNums = left.sign;

    if (left.size != right.size) {
        if (twoNegativeNums) {
            return left.size > right.size;
        }

        return left.size < right.size;

    }
    for (size_t i = left.size; i > 0; --i) {
        size_t index = i - 1;
        if (left.num[index] != right.num[index]) {
            if (twoNegativeNums) {
                return left.num[index] > right.num[index];
            }
            return left.num[index] < right.num[index];
        }
    }
    return false;
}

bool operator>(const BigInteger& left, const BigInteger& right) {
    return right < left;
}

bool operator>=(const BigInteger& left, const BigInteger& right) {
    return !(right > left);
}

bool operator<=(const BigInteger& left, const BigInteger& right) {
    return !(right < left);
}

//конструкторы от числовых типов
//------------------------------------------
BigInteger::BigInteger(int number) {
    sign = number < 0;
    size = 1;
    uint32_t unsignedNumber = std::abs(number);

    num = new uint32_t[size];
    num[0] = unsignedNumber;

}

BigInteger::BigInteger(unsigned int number) : sign(false) {
    size = 1;
    num = new uint32_t[size];
    num[0] = number;

}

BigInteger::BigInteger(long number) {
    sign = number < 0;

    uint64_t unsignedNumber = std::abs(number);

    if ((unsignedNumber >> 32) != 0U) {
        size = 2;
    }
    else {
        size = 1;
    }

    num = new uint32_t[size];
    num[0] = unsignedNumber;

    if (size == 2) {
        num[1] = unsignedNumber >> 32;
    }

    if (num[0] == 0) {
        size = 1;
        sign = false;
    }

    removeZeros();
}

BigInteger::BigInteger(unsigned long number) : sign(false) {

    uint64_t unsignedNumber = number;

    if ((unsignedNumber >> 32) != 0U) {
        size = 2;
    }
    else {
        size = 1;
    }

    num = new uint32_t[size];

    num[0] = unsignedNumber;
    if (size == 2) {
        num[1] = unsignedNumber >> 32;
    }
}

BigInteger::BigInteger(long long number) {
    sign = number < 0;
    uint64_t unsignedNumber = std::abs(number);

    if ((unsignedNumber >> 32) != 0U) {
        size = 2;
    }
    else {
        size = 1;
    }

    num = new uint32_t[size];
    num[0] = unsignedNumber;

    if (size == 2) {
        num[1] = unsignedNumber >> 32;
    }

    if (num[0] == 0) {
        size = 1;
        sign = false;
    }


    removeZeros();
}

BigInteger::BigInteger(unsigned long long number) : sign(false) {
    uint64_t unsignedNumber = number;
    if ((unsignedNumber >> 32) != 0U) {
        size = 2;
    }
    else {
        size = 1;
    }

    num = new uint32_t[size];
    num[0] = unsignedNumber;
    if (size == 2) {
        num[1] = unsignedNumber >> 32;
    }
}

//------------------------------------------

BigInteger BigInteger::abs() const {
    BigInteger result(*this);
    result.sign = false;
    return result;
}

// Конструктор перемещения
BigInteger::BigInteger(BigInteger&& other) noexcept : num(other.num), sign(other.sign), size(other.size) {
    other.num = nullptr;
    other.size = 0;
    other.sign = false;
}

// Конструктор копирования
BigInteger::BigInteger(const BigInteger& other) : sign(other.sign), size(other.size) {

    num = new uint32_t[size];
    for (size_t i = 0; i < size; ++i) {
        num[i] = other.num[i];
    }

}

// Унарный минус
BigInteger BigInteger::operator-() const {
    BigInteger result(*this);
    if (result.size == 1 && result.num[0] == 0) {
        result.sign = false;
    }
    else {
        result.sign = !result.sign;
    }
    return result;
}
// Унарный плюс
BigInteger BigInteger::operator+() const {
    return *this;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {

    if (sign != other.sign) {
        *this -= (-other);
        return *this;
    }

    size_t maxSize = std::max(size, other.size) + 1;
    enhance(maxSize);

    uint64_t carry = 0;
    size_t i;

    for (i = 0; i < maxSize || (carry != 0U); ++i) {

        uint64_t leftDigit;
        if (i < size) {
            leftDigit = num[i];
        }
        else {
            leftDigit = 0;
        }
        uint64_t rightDigit;
        if (i < other.size) {
            rightDigit = other.num[i];
        }
        else {
            rightDigit = 0;
        }

        const uint64_t sum = leftDigit + rightDigit + carry;
        if (i < maxSize) {
            num[i] = static_cast<uint32_t>(sum);
        }
        carry = sum >> 32;
    }

    size = std::min(i, maxSize);
    removeZeros();
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {

    if (sign != other.sign) {
        
        BigInteger other_abs = other.abs();

        size_t maxSize = std::max(size, other_abs.size) + 1;
        enhance(maxSize);

        uint64_t carry = 0;
        size_t i;

        for (i = 0; i < maxSize || (carry != 0U); ++i) {
            uint64_t leftDigit = (i < size) ? num[i] : 0;
            uint64_t rightDigit = (i < other_abs.size) ? other_abs.num[i] : 0;

            const uint64_t sum = leftDigit + rightDigit + carry;
            if (i < maxSize) {
                num[i] = static_cast<uint32_t>(sum);
            }
            carry = sum >> 32;
        }
        
        size = std::min(i, maxSize);
        removeZeros();
        return *this;
    }


    if (this->abs() < other.abs()) {
        *this = other - *this;
        this->sign = !this->sign;
        return *this;
    }

    uint64_t borrow = 0;
    for (size_t i = 0; i < size; ++i) {
        uint64_t thisDigit = num[i];
        uint64_t OtherDigit;

        if (i < other.size) {
            OtherDigit = other.num[i];
        }
        else {
            OtherDigit = 0;
        }

        uint64_t diff;
        if (thisDigit >= OtherDigit + borrow) {
            diff = thisDigit - OtherDigit - borrow;
            borrow = 0;
        }
        else {
            diff = thisDigit - OtherDigit - borrow;
            borrow = 1;
        }

        num[i] = static_cast<uint32_t>(diff);
    }

    removeZeros();
    return *this;
}

BigInteger operator+(BigInteger left, const BigInteger& right) {
    left += right;
    return left;
}

BigInteger operator-(BigInteger left, const BigInteger& right) {
    left -= right;
    return left;
}

BigInteger& BigInteger::operator*=(const BigInteger& right) {

    size_t sizeOfRes = size + right.size;
    uint32_t* result = new uint32_t[sizeOfRes]{ 0 };

    for (size_t i = 0; i < size; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < right.size; ++j) {
            uint64_t tmp = static_cast<uint64_t>(num[i]) * right.num[j];
            tmp += result[i + j] + carry;
            result[i + j] = static_cast<uint32_t>(tmp);
            carry = tmp >> 32;
        }

        size_t carryPos = i + right.size;
        while (carry > 0 && carryPos < sizeOfRes) {
            uint64_t sum = result[carryPos] + carry;
            result[carryPos] = static_cast<uint32_t>(sum);
            carry = sum >> 32;
            ++carryPos;
        }
    }

    delete[] num;
    num = result;
    size = sizeOfRes;
    sign = (sign != right.sign);

    removeZeros();
    return *this;
}

BigInteger operator*(BigInteger left, const BigInteger& right) {
    left *= right;
    return left;
}

void BigInteger::moveLeft32(size_t bits) {

    size_t fullBits = bits / 32;
    size_t remainedBits = bits % 32;

    if (remainedBits > 0) {
        fullBits += 1;
    }

    size_t newSize = size + fullBits;
    uint32_t* res = new uint32_t[newSize]();

    for (size_t i = 0; i < size; ++i) {
        res[i + fullBits] = static_cast<uint32_t>(num[i]);
    }

    delete[] num;
    num = res;
    size = newSize;

    removeZeros();
}

//-----------------------------------------------------------------------------------------

//prefix
BigInteger& BigInteger::operator++() {
    *this += BigInteger(1);
    return *this;
}

BigInteger& BigInteger::operator--() {
    *this -= BigInteger(1);
    return *this;
}
//postfix
BigInteger BigInteger::operator++(int number) {
    BigInteger copy = *this;
    *this += BigInteger(1);
    return copy;

}

BigInteger BigInteger::operator--(int number) {
    BigInteger copy = *this;
    *this -= BigInteger(1);
    return copy;
}

BigInteger& BigInteger::operator/=(const BigInteger& right) {

    if (right == 0) {
        throw std::invalid_argument("ERROR: division by zero");
    }
    bool result_sign = (sign != right.sign);
    BigInteger dividend = abs();
    const BigInteger& divisor = right.abs();

    BigInteger resOfDivision(0);
    BigInteger remainder(0); //остаток

    for (size_t i = dividend.size; i > 0; --i) {

        remainder.moveLeft32(32);
        remainder += dividend.num[i - 1];

        // Бинарный поиск
        uint32_t foundDigit = 0;
        uint32_t low = 0;
        uint32_t high = std::numeric_limits<uint32_t>::max();

        while (low <= high) {
            uint32_t mid = low + (high - low) / 2;
            BigInteger product = divisor * BigInteger(mid);
            if (product <= remainder) {
                foundDigit = mid;
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }

        remainder -= divisor * BigInteger(foundDigit);
        resOfDivision.moveLeft32(32);
        resOfDivision += foundDigit;
    }

    resOfDivision.sign = result_sign;
    resOfDivision.removeZeros();
    *this = resOfDivision;

    return *this;
}

BigInteger operator/(BigInteger left, const BigInteger& right) {
    left /= right;
    return left;
}

BigInteger& BigInteger::operator%=(const BigInteger& divisor_param) {
    if (divisor_param == 0) {
        throw std::invalid_argument("ERROR: division by zero");
    }
    BigInteger dividend = abs();
    const BigInteger& divisor = divisor_param.abs();

    BigInteger resOfDivision(0);
    BigInteger remainder(0); //остаток

    for (size_t i = dividend.size; i > 0; --i) {
        remainder.moveLeft32(32);
        remainder += dividend.num[i - 1];
        remainder.removeZeros();

        if (remainder < divisor) {
            resOfDivision.moveLeft32(32);
        }

        // Бинарный поиск
        uint32_t foundDigit = 0;
        uint32_t low = 0;
        uint32_t high = std::numeric_limits<uint32_t>::max();

        while (low <= high) {
            uint32_t mid = low + (high - low) / 2;
            BigInteger product = divisor * BigInteger(mid);
            if (product <= remainder) {
                foundDigit = mid;
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }

        remainder -= divisor * BigInteger(foundDigit);
        resOfDivision.moveLeft32(32);
        resOfDivision += foundDigit;
    }

    if (sign) {
        remainder = -remainder;
    }
    *this = remainder;

    return *this;
}

BigInteger operator%(BigInteger left, const BigInteger& right) {
    left %= right;
    return left;
}

std::string to_string(const BigInteger& number) {

    if (number == BigInteger(0)) {
        return "0";
    }
    std::string numStr;
    BigInteger tmp = number;

    while (tmp != 0) {
        BigInteger remainder = tmp % 10;
        char digit = static_cast<char>('0' + remainder.num[0]);
        tmp = tmp / 10;
        numStr += digit;
    }

    if (number.sign) {
        numStr += '-';
    }

    std::reverse(numStr.begin(), numStr.end());
    return numStr;
}

std::string to_hex(const BigInteger& number) {

    if (number == BigInteger(0)) {
        return "0";
    }
    std::string numStr;

    BigInteger tmp = number.abs();

    char symbols[] = "0123456789ABCDEF";

    while (tmp != 0) {
        BigInteger remainder = tmp % 16;
        uint32_t symbol = remainder.num[0];
        tmp = tmp / 16;
        numStr += symbols[symbol];

    }

    if (number.sign) {
        numStr += '-';
    }

    std::reverse(numStr.begin(), numStr.end());

    return numStr;
}

BigInteger::BigInteger(std::string strNumber, biginteger_base base) {

    sign = false;
    size = 1;
    num = new uint32_t[size]{ 0 };

    try {
        if (strNumber[0] == '-') {
            sign = true;
            strNumber.erase(0, 1);
        }

        if (strNumber[0] == '+') {
            sign = false;
            strNumber.erase(0, 1);
        }

        if (strNumber.empty()) {

            throw std::invalid_argument("ERROR: empty string");
        }

        if (base == biginteger_base::dec) {
            decConvert(strNumber);
        }
        else {
            hexConvert(strNumber);
        }
    }
    catch (...) {
        delete[] num;
        throw;
    }
}

void BigInteger::decConvert(std::string& strNumber) {

    if (strNumber.empty()) {
        sign = false;
        num[0] = 0;
        return;
    }

    for (char ch : strNumber) {
        if (ch < '0' || ch > '9') {
            throw std::invalid_argument("ERROR: non-decimal number");
        }
        int digit = ch - '0';
        uint64_t carry = digit;
        for (size_t i = 0; i < size; ++i) {
            carry += static_cast<uint64_t>(num[i]) * 10;
            num[i] = static_cast<uint32_t>(carry);
            carry >>= 32;
        }
        if (carry > 0) {
            enhance(size + 1);
            num[size - 1] = static_cast<uint32_t>(carry);
        }
    }
    removeZeros();

}

void BigInteger::hexConvert(std::string& strNumber) {

    for (char ch : strNumber) {
        int digit = static_cast<unsigned char>(ch);
        if (ch >= '0' && ch <= '9') {
            digit = ch - '0';
        }
        else if (ch >= 'A' && ch <= 'F') {
            digit = ch - 'A' + 10;
        }
        else if (ch >= 'a' && ch <= 'f') {
            digit = ch - 'a' + 10;
        }

        uint64_t carry = digit;
        for (size_t i = 0; i < size; ++i) {
            carry += static_cast<uint64_t>(num[i]) * 16;
            num[i] = static_cast<uint32_t>(carry);
            carry >>= 32;
        }

        if (carry > 0) {
            enhance(size + 1);
            num[size - 1] = static_cast<uint32_t>(carry);
        }
    }

    removeZeros();

}

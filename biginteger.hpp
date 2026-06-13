#ifndef BIG_INTEGER_HPP
#define BIG_INTEGER_HPP

#include <string>
#include <cstring>
#include <stdexcept>
#include <cstdint>

enum class biginteger_base { dec = 10, hex = 16 };

class BigInteger {
private:

    uint32_t* num;
    size_t size;
    bool sign;

    void enhance(size_t new_size);

    void removeZeros();

public:

    //конструктор по умолчанию
    BigInteger();

    ~BigInteger() {
        delete[] num;
    }

    // Оператор присваивания копированием
    BigInteger& operator=(const BigInteger& other);

    // Оператор присваивания перемещением
    BigInteger& operator=(BigInteger&& other) noexcept;

    // Операторы сравнения
    friend bool operator==(const BigInteger& left, const BigInteger& right);

    friend bool operator!=(const BigInteger& left, const BigInteger& right);

    friend bool operator<(const BigInteger& left, const BigInteger& right);

    friend bool operator>(const BigInteger& left, const BigInteger& right);

    friend bool operator>=(const BigInteger& left, const BigInteger& right);

    friend bool operator<=(const BigInteger& left, const BigInteger& right);

    //конструкторы от числовых типов
    //------------------------------------------
    BigInteger(int number);

    BigInteger(unsigned int number);

    BigInteger(long number);

    BigInteger(unsigned long number);

    BigInteger(long long number);

    BigInteger(unsigned long long number);

    //------------------------------------------

    BigInteger abs() const;


    // Конструктор перемещения
    BigInteger(BigInteger&& other) noexcept;

    // Конструктор копирования
    BigInteger(const BigInteger& other);

    // Унарный минус
    BigInteger operator-() const;

    // Унарный плюс
    BigInteger operator+() const;

    BigInteger& operator+=(const BigInteger& other);

    friend BigInteger operator+(BigInteger left, const BigInteger& right);

    BigInteger& operator-=(const BigInteger& other);

    friend BigInteger operator-(BigInteger left, const BigInteger& right);

    BigInteger& operator*=(const BigInteger& right);

    friend BigInteger operator*(BigInteger left, const BigInteger& right);

    void moveLeft32(size_t bits);

    BigInteger& operator/=(const BigInteger& right);

    friend BigInteger operator/(BigInteger left, const BigInteger& right);

    BigInteger& operator%=(const BigInteger& divisor_param);

    friend BigInteger operator%(BigInteger left, const BigInteger& right);


    //явный конструктор от строки
    BigInteger(std::string strNumber, biginteger_base base = biginteger_base::dec);

    void decConvert(std::string& strNumber);

    void hexConvert(std::string& strNumber);

    friend std::string to_string(const BigInteger& number);

    friend std::string to_hex(const BigInteger& number);

    BigInteger& operator++();

    BigInteger& operator--();

    BigInteger operator++(int number);

    BigInteger operator--(int number);

};


#endif

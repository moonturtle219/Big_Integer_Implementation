#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iomanip>

class BigInt
{
public:
    // constructors
    BigInt();                                          // default constructor, creates BigInt with value 0
    BigInt(int64_t val);                               // create BigInt from an int64_t value
    BigInt(const std::string& str, uint8_t base = 10); // create BigInt from a string in given base, default to base 10
    explicit operator uint64_t() const;

    // copy constructor
    BigInt(const BigInt& other);

    // move constructor
    BigInt(BigInt&& other);

    // destructor
    ~BigInt();

    // copy assignment operator
    BigInt& operator=(const BigInt& other);

    // move assignment operator
    BigInt& operator=(BigInt&& other);

    // arithmetic operators
    BigInt operator+(const BigInt& other) const; // addition
    BigInt operator-(const BigInt& other) const; // subtraction
    BigInt operator*(const BigInt& other) const; // multiplication
    BigInt operator%(const BigInt& other) const; // modulo
    BigInt& operator+=(const BigInt& other);
    BigInt& operator-=(const BigInt& other);
    BigInt& operator*=(const BigInt& other);
    BigInt& operator/=(const BigInt& other);

    // comparison operators
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    // output stream operator
    friend std::ostream& operator<<(std::ostream& os, const BigInt& bigInt);

    size_t size() const
    {
        return size_;
    }

private:
    uint64_t* data_; // array to store binary data
    size_t size_;    // size of data_ array in uint64_t
    bool negative_;  // true if BigInt is negative

    // helper functions
    void divide(const BigInt& other, BigInt& quotient, BigInt& remainder) const;
    void decode(const std::string& str, uint8_t base = 10); // decode a string in given base and store as binary data
    void complement();                                      // take 2's complement of data_
    void trim();                                            // remove leading zeros in data_
    void resize(size_t new_size);                           // resize data_ array to new size
};
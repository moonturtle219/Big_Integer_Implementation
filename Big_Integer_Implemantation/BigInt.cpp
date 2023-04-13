#include "BigInt.h"

// default constructor
BigInt::BigInt()
{
    // initialize data_ to nullptr
    data_ = nullptr;

    // initialize size_ to 0
    size_ = 0;

    // initialize negative_ to false
    negative_ = false;
}

BigInt::BigInt(int64_t val)
{
    // determine whether val is negative
    if (val < 0)
    {
        negative_ = true;
        val = -val;
    }
    else
    {
        negative_ = false;
    }

    // allocate memory for the data array
    data_ = new uint64_t[1];
    size_ = 1;

    // initialize data array with 2's complement representation of val
    data_[0] = static_cast<uint64_t>(val);
    if (negative_)
    {
        complement();
    }

    // remove any leading zeros from the data array
    trim();
}

BigInt::BigInt(const std::string& str, uint8_t base)
{
    size_ = 0;
    data_ = nullptr;
    negative_ = false;

    // Determine the sign of the number
    size_t start = 0;
    if (str[0] == '-')
    {
        negative_ = true;
        start = 1;
    }

    BigInt bbase(base);
    for (size_t i = start; i < str.size(); ++i)
    {
        *this *= bbase;

        // Convert character to digit
        uint8_t digit = 0;
        if (str[i] >= '0' && str[i] <= '9')
        {
            digit = str[i] - '0';
        }
        else if (str[i] >= 'A' && str[i] <= 'Z')
        {
            digit = str[i] - 'A' + 10;
        }
        else if (str[i] >= 'a' && str[i] <= 'z')
        {
            digit = str[i] - 'a' + 10;
        }
        else
        {
            // Invalid character
            throw std::invalid_argument("Invalid character in input string");
        }

        // Check if digit is valid in given base
        if (digit >= base)
        {
            throw std::invalid_argument("Invalid digit in input string");
        }

        *this += BigInt(digit);
    }

    // Take 2's complement of data_ if the number is negative
    if (negative_)
    {
        complement();
    }
}

BigInt::operator uint64_t() const
{
    if (size_ == 1)
    {
        return data_[0];
    }
    else
    {
        // handle error case
    }
}

// destructor
BigInt::~BigInt()
{
    delete[] data_;
}

// Copy constructor
BigInt::BigInt(const BigInt& other)
{
    size_ = other.size_;
    data_ = new uint64_t[size_];
    std::copy(other.data_, other.data_ + size_, data_);
}

BigInt& BigInt::operator=(const BigInt& other)
{
    if (this != &other)
    {
        delete[] data_;
        size_ = other.size_;
        negative_ = other.negative_;
        data_ = new uint64_t[size_];
        std::copy(other.data_, other.data_ + size_, data_);
    }
    return *this;
}

// Move assignment operator
BigInt& BigInt::operator=(BigInt&& other)
{
    if (this != &other)
    {
        delete[] data_;
        size_ = other.size_;
        data_ = other.data_;
        other.size_ = 0;
        other.data_ = nullptr;
    }
    return *this;
}

BigInt BigInt::operator+(const BigInt& other) const
{
    // determine the size of the result
    size_t max_size = std::max(size_, other.size_);
    BigInt result;
    result.resize(max_size + 1);

    // add data_ and other.data_ arrays
    bool carry = 0;
    for (size_t i = 0; i < max_size; ++i)
    {
        uint64_t a = (i < size_) ? data_[i] : 0;
        uint64_t b = (i < other.size_) ? other.data_[i] : 0;
        uint64_t sum = a + b + carry;
        if (sum < a || sum < b)
        {
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        result.data_[i] = sum;
    }

    // check if a carry is generated in last iteration
    if (carry)
    {
        result.data_[max_size] = 1;
    }
    else
    {
        // result.resize(max_size);
        // No need to resize because the void resize will be recreate a data_ and copy from old to new
        result.size_--;
    }

    // set sign of result
    result.negative_ = negative_;

    // remove leading zeros in result
    result.trim();

    return result;
}

BigInt BigInt::operator-(const BigInt& other) const
{
    BigInt result(*this);

    // If signs of operands are different, negate other operand and add to this operand
    if (negative_ != other.negative_)
    {
        BigInt neg_other = other;
        neg_other.complement(); // take 2's complement of other
        result += neg_other;
        return result;
    }

    // If signs of operands are the same, take 2's complement of other operand and add to this operand
    BigInt neg_other = other;
    neg_other.complement(); // take 2's complement of other
    result += neg_other;
    result.negative_ = !result.negative_; // flip the sign of result

    result.trim();
    return result;
}

BigInt BigInt::operator*(const BigInt& other) const
{
    // determine sign of result
    bool negative = negative_ != other.negative_;

    // create result object and resize data array
    BigInt result;
    result.resize(size_ + other.size_);

    // perform long multiplication
    for (size_t i = 0; i < size_; i++)
    {
        uint64_t carry = 0;
        for (size_t j = 0; j < other.size_; j++)
        {
            __uint128_t prod = (__uint128_t)data_[i] * other.data_[j] + carry + result.data_[i + j];
            result.data_[i + j] = (uint64_t)(prod & 0xffffffffffffffff);
            carry = (uint64_t)(prod >> 64);
        }
        result.data_[i + other.size_] += carry;
    }

    // set sign and trim leading zeros
    result.negative_ = negative;
    result.trim();

    return result;
}

BigInt BigInt::operator%(const BigInt& other) const
{
    // BigInt quotient, remainder;

    // // Calculate quotient and remainder using division
    // divide(other, quotient, remainder);

    // // If dividend is negative and remainder is non-zero, add divisor to remainder
    // if (negative_ && !remainder.is_zero())
    // {
    //     remainder += other;
    // }

    // // Set sign of remainder
    // remainder.negative_ = negative_;

    // // Remove leading zeros from remainder
    // remainder.trim();

    // return remainder;
}

void BigInt::divide(const BigInt& other, BigInt& quotient, BigInt& remainder) const
{

}

BigInt& BigInt::operator+=(const BigInt& other)
{
    *this = *this + other;
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& other)
{
    *this = *this - other;
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& other)
{
    *this = *this * other;
    return *this;
}

BigInt& BigInt::operator/=(const BigInt& other)
{
    BigInt quotient;
    BigInt remainder = *this;
    while (remainder >= other)
    {
        remainder -= other;
        quotient += 1;
    }
    *this = quotient;
    return *this;
}

bool BigInt::operator>(const BigInt& other) const
{
    if (negative_ != other.negative_)
    {
        return !negative_;
    }
    if (size_ != other.size_)
    {
        return size_ > other.size_;
    }
    for (int i = size_ - 1; i >= 0; --i)
    {
        if (data_[i] != other.data_[i])
        {
            return data_[i] > other.data_[i];
        }
    }
    return false;
}

bool BigInt::operator>=(const BigInt& other) const
{
    // check signs
    if (negative_ && !other.negative_)
        return false;
    if (!negative_ && other.negative_)
        return true;

    // compare sizes
    if (size_ > other.size_)
        return !negative_;
    if (size_ < other.size_)
        return negative_;

    // compare data arrays
    for (int i = size_ - 1; i >= 0; --i)
        if (data_[i] != other.data_[i])
            return (data_[i] > other.data_[i]) != negative_;

    // numbers are equal
    return true;
}

bool BigInt::operator==(const BigInt& other) const
{
    // check signs and sizes
    if (negative_ != other.negative_ || size_ != other.size_)
        return false;

    // compare data arrays
    for (size_t i = 0; i < size_; ++i)
        if (data_[i] != other.data_[i])
            return false;

    // numbers are equal
    return true;
}

bool BigInt::operator!=(const BigInt& other) const
{
    return !(*this == other);
}

void BigInt::complement()
{
    // take 1's complement
    for (size_t i = 0; i < size_; ++i)
    {
        data_[i] = ~data_[i];
    }

    // add 1 to get 2's complement
    bool carry = 1;
    for (size_t i = 0; i < size_; ++i)
    {
        uint64_t sum = data_[i] + carry;
        if (sum < data_[i])
        {
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        data_[i] = sum;
        if (!carry)
        {
            break;
        }
    }

    // set negative flag to true
    negative_ = true;
}

void BigInt::trim()
{
    while (size_ > 1 && data_[size_ - 1] == 0)
    {
        --size_;
    }
    resize(size_);
}

// std::ostream &operator<<(std::ostream &os, const BigInt &bigInt)
// {
//     if (bigInt.negative_)
//     {
//         os << "-";
//     }
//     size_t n = bigInt.size_;
//     uint64_t *data = bigInt.data_;
//     os << data[n - 1];
//     for (int i = n - 2; i >= 0; i--)
//     {
//         os << std::setfill('0') << std::setw(19) << data[i]; // print each uint64_t with leading zeros
//     }
//     return os;
// }

std::ostream& operator<<(std::ostream& os, const BigInt& bigInt)
{
    for (size_t i = bigInt.size_; i-- > 0;)
    {
        os << std::bitset<64>(bigInt.data_[i]);
    }
    return os;
}

// std::ostream &operator<<(std::ostream &os, const BigInt &bigInt)
// {
//     if (bigInt.size_ == 0)
//     {
//         os << '0';
//         return os;
//     }

//     BigInt temp(bigInt);
//     std::vector<uint8_t> digits;
//     while (temp.size_ > 0)
//     {
//         uint64_t remainder = static_cast<uint64_t>(temp % 10);
//         temp /= 10;
//         digits.push_back(remainder);
//     }

//     if (bigInt.negative_)
//     {
//         os << '-';
//     }
//     for (size_t i = digits.size(); i-- > 0;)
//     {
//         os << static_cast<char>('0' + digits[i]);
//     }
//     return os;
// }

void BigInt::resize(size_t new_size)
{
    uint64_t* new_data = new uint64_t[new_size];
    std::memset(new_data, 0, new_size * sizeof(uint64_t));
    if (data_ != nullptr)
    {
        std::memcpy(new_data, data_, size_ * sizeof(uint64_t));
        delete[] data_;
    }
    data_ = new_data;
    size_ = new_size;
}

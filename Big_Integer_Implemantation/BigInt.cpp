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
	// determine if val is negative
	negative_ = (val < 0);

	// take absolute value of val
	uint64_t abs_val = std::abs(val);

	// allocate memory for data_
	size_ = 1;
	data_ = new uint64_t[size_];

	// store abs_val in data_
	data_[0] = abs_val;
}

BigInt::BigInt(const std::string& str, uint8_t base) {
	size_ = 0;
	data_ = nullptr;
	BigInt bbase(base);
	for (char c : str) {
		*this *= bbase;
		*this += BigInt(c - '0');
	}
}

// destructor
BigInt::~BigInt()
{
	delete[] data_;
}

// Copy constructor
BigInt::BigInt(const BigInt& other) {
	size_ = other.size_;
	data_ = new uint64_t[size_];
	std::copy(other.data_, other.data_ + size_, data_);
}

// Move assignment operator
BigInt& BigInt::operator=(BigInt&& other) {
	if (this != &other) {
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

BigInt BigInt::operator*(const BigInt& other) const {
	// determine sign of result
	bool negative = negative_ != other.negative_;

	// create result object and resize data array
	size_t new_size = size_ + other.size_;
	BigInt result;
	result.resize(new_size);

	// perform long multiplication
	memset(result.data_, 0, new_size * sizeof(uint64_t));
	result.negative_ = negative_ ^ other.negative_;
	for (size_t i = 0; i < size_; i++) {
		uint64_t carry = 0;
		for (size_t j = 0; j < other.size_; j++) {
			uint64_t tmp = data_[i] * other.data_[j] + result.data_[i + j] + carry;
			result.data_[i + j] = tmp & 0xFFFFFFFFFFFFFFFF;
			carry = tmp >> 64;
		}
		size_t k = i + other.size_;
		while (carry != 0) {
			uint64_t tmp = result.data_[k] + carry;
			result.data_[k] = tmp & 0xFFFFFFFFFFFFFFFF;
			carry = tmp >> 64;
			k++;
		}
	}
	result.trim();
	return result;
}

BigInt& BigInt::operator+=(const BigInt& other)
{
	*this = *this + other;
	return *this;
}

BigInt& BigInt::operator*=(const BigInt& other)
{
	*this = *this * other;
	return *this;
}

void BigInt::trim()
{
	// Determine the index of the most significant non-zero digit
	size_t i;
	for (i = size_ - 1; i > 0; i--)
	{
		if (data_[i] != 0)
		{
			break;
		}
	}

	// Adjust the size of the array to remove any leading zero digits
	size_ = i + 1;
}

std::ostream& operator<<(std::ostream& os, const BigInt& bigInt)
{
	if (bigInt.negative_)
	{
		os << "-";
	}
	size_t n = bigInt.size_;
	uint64_t* data = bigInt.data_;
	os << data[n - 1];
	for (int i = n - 2; i >= 0; i--)
	{
		os << std::setfill('0') << std::setw(19) << data[i]; // print each uint64_t with leading zeros
	}
	return os;
}

void BigInt::resize(size_t new_size)
{
	if (new_size == size_)
		return;

	uint64_t* new_data = new uint64_t[new_size];
	size_t copy_size = std::min(size_, new_size);
	std::copy(data_, data_ + copy_size, new_data);
	std::fill(new_data + copy_size, new_data + new_size, 0);

	delete[] data_;
	data_ = new_data;
	size_ = new_size;
}
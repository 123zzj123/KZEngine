#pragma once
#ifndef KZ_FIX32

#define KZ_FIX32
#include<iostream>
#include<assert.h>

namespace KZMath {

	/*
	*	including fix32 definition and operation,高20位整数，低12位小数
	*/
	class KZFix32
	{
	public:
		inline KZFix32() : value_(0) {}
		inline KZFix32(int32_t value) {
			assert(value >= MIN_INT_VALUE && value <= MAX_INT_VALUE);
			value_ = value << DECIMAL_BITS;
		}
		inline KZFix32(int64_t value) {
			assert(value >= MIN_INT_VALUE && value <= MAX_INT_VALUE);
			value_ = static_cast<int32_t>((value + 2048) >> DECIMAL_BITS);
		}
		inline KZFix32(float value) {
			assert(value >= MIN_FLOAT_VALUE && value <= MAX_FLOAT_VALUE);
			int a = (1 << DECIMAL_BITS);
			value_ = static_cast<int32_t>(value * (1 << DECIMAL_BITS) + 0.5f);
		}
		inline KZFix32(const KZFix32& other) {
			value_ = other.value_;
		}
		inline void SetValue(int32_t value) {
			assert(value >= MIN_INT_VALUE && value <= MAX_INT_VALUE);
			value_ = value << DECIMAL_BITS;
		}
		inline void SetValue(float value) {
			assert(value >= MIN_FLOAT_VALUE && value <= MAX_FLOAT_VALUE);
			value_ = static_cast<int32_t>(value * (1 << DECIMAL_BITS) + 0.5f);
		}
		inline KZFix32 operator +(const KZFix32& other) const {
			KZFix32 result;
			result.value_ = value_ + other.value_;
			return result;
		}
		inline KZFix32 operator -(const KZFix32& other) const {
			KZFix32 result;
			result.value_ = value_ - other.value_;
			return result;
		}
		inline KZFix32 operator -() const {
			KZFix32 result;
			result.value_ = -value_;
			return result;
		}
		inline KZFix32 operator *(const KZFix32& other) const {
			if (other.value_ == 0 || value_ == 0) {
				return ZERO;
			}
			if (other.value_ == 1) {
				return *this;
			}
			if (value_ == 1) {
				return other.value_;
			}
			int64_t value = value_;
			value *= other.value_;
			value >>= DECIMAL_BITS;
			KZFix32 result;
			result.value_ = (int32_t)value;
			return result;
		}
		inline KZFix32 operator /(const KZFix32& other) const {
			if (value_ == 0) {
				return ZERO;
			}
			if (other.value_ == 1) {
				return *this;
			}
			if (other.value_ == 0) {
				return ZERO;
			}
			int64_t value = value_;
			value <<= DECIMAL_BITS;
			value /= other.value_;

			KZFix32 result;
			result.value_ = (int32_t)value;
			return result;
		}
		inline KZFix32 operator +=(const KZFix32& other) {
			return *this = *this + other;
		}
		inline KZFix32 operator -=(const KZFix32& other) {
			return *this = *this - other;
		}
		inline KZFix32 operator *=(const KZFix32& other) {
			return *this = *this * other;
		}
		inline KZFix32 operator /=(const KZFix32& other) {
			return *this = *this / other;
		}
		inline bool operator < (const KZFix32& other) {
			return value_ < other.value_;
		}
		inline bool operator > (const KZFix32& other) {
			return value_ > other.value_;
		}
		inline bool operator <= (const KZFix32& other) {
			return value_ <= other.value_;
		}
		inline bool operator >= (const KZFix32& other) {
			return value_ >= other.value_;
		}
		inline bool operator == (const KZFix32& other) {
			return value_ == other.value_;
		}
		inline bool operator != (const KZFix32& other) {
			return value_ != other.value_;
		}
		inline int32_t GetIntValue()
		{
			return value_ >> DECIMAL_BITS;
		}
		inline float GetFloatValue()
		{
			return ((float)value_ / 4096.0f);
		}
	public:
		static const int32_t MAX_INT_VALUE;
		static const int32_t MIN_INT_VALUE;
		static const float MAX_FLOAT_VALUE;
		static const float MIN_FLOAT_VALUE; 
		static const int32_t INTEGER_BITS;
		static const int32_t DECIMAL_BITS;
		static const KZFix32 ZERO;
		static const KZFix32 One;
		static const KZFix32 MaxColorValue;
	private:
		int32_t value_;
	};
}

#endif // !KZ_FIX32

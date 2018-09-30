#include"fix32.h"
using namespace KZMath;

const int32_t KZFix32::INTEGER_BITS = 20;
const int32_t KZFix32::DECIMAL_BITS = 12;
const int32_t KZFix32::MAX_INT_VALUE  = 524287;
const int32_t KZFix32::MIN_INT_VALUE = -524288;
const float KZFix32::MAX_FLOAT_VALUE = 524287.0f;
const float KZFix32::MIN_FLOAT_VALUE = -524288.0f;
const KZFix32 KZFix32::ZERO = KZFix32(0);
const KZFix32 KZFix32::One = KZFix32(1);
const KZFix32 KZFix32::MaxColorValue = KZFix32(255);
#pragma once
#ifndef _MATH_HPP_
#define _MATH_HPP_

#include <cmath>

float RoundSmallToZero(float number) {
	if (abs(number) < 1.f / 1000.f) number = 0;
	return number;
}

#endif //_MATH_HPP_
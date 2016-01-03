#pragma once
#include "Interpolator.h"

template <class T>
class LinearInterpolator : public Interpolator<T>
{
public:
	LinearInterpolator();
	~LinearInterpolator();

	virtual const T& interpolate(const std::vector<T> &points, int basePoint, float value);
};


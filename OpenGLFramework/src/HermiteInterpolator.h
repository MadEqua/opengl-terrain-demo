#pragma once

#include "Interpolator.h"

template <class T>
class HermiteInterpolator : public Interpolator<T>
{
public:
	HermiteInterpolator();
	~HermiteInterpolator();

	virtual const T& interpolate(const std::vector<T> &points, int basePoint, float value);
};


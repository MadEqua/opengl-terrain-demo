#pragma once

#include "Interpolator.h"

//Catmull-Rom

template <class T>
class CubicInterpolator : public Interpolator<T>
{
public:
	CubicInterpolator();
	~CubicInterpolator();

	virtual const T& interpolate(const std::vector<T> &points, int basePoint, float value);
};


#pragma once

#include <vector>

template <class T>
class Interpolator
{
public:
	Interpolator();
	~Interpolator();

	virtual const T& interpolate(const std::vector<T> &points, int basePoint, float value) = 0;

protected:
	T temp;

	const T& getPointLooped(const std::vector<T> &points, int index);
};
#pragma once

#include "Interpolator.h"
#include <vector>

template <class T>
class Automation
{
public:
	Automation(float duration);
	~Automation();

	inline void addCheckPoint(const T &v) { cps.push_back(v); }
	inline void setDuration(float d) { duration = d; }

	const T& getValue(float deltaTime);

private:
	std::vector<T> cps;
	Interpolator<T> *interpolator;

	float progress; //seconds
	float duration; //seconds
};


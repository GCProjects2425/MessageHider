#pragma once

#include "framework.h"
#include <gdiplus.h>

class Filter
{
public:
	enum Filters {
		BLACKWHITE_FILTER = 0,
		INVERT_FILTER,
		BLUR_FILTER,
		SATURE_FILTER
	};

	void Apply(Bitmap* bitmap, Filters filter);
	//virtual void Apply(Gdiplus::Bitmap* image) = 0;
	virtual ~Filter() = default;

private:
	void BlackWhiteFilter(Bitmap* image);
	void InvertFilter(Bitmap* image);
	void BlurFilter(Bitmap* image);
};


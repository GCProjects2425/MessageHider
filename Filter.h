#pragma once

#include "framework.h"
#include <gdiplus.h>

class Filter
{
public:
	virtual void Apply(Gdiplus::Bitmap* image) = 0;
	virtual ~Filter() = default;
};


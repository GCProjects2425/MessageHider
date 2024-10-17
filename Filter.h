#pragma once

#include "ImageHandler.h"


class Filter
{
public:
	virtual void Apply(Bitmap& image) = 0;
	virtual ~Filter() = default;
};


#pragma once
#include "Filter.h"


class BlackWhiteFilter : public Filter
{
public :
	void Apply(Bitmap* image) override;
};


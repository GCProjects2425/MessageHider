#pragma once
#include "Field.h"
class ImageField : public Field
{
public:
	ImageField(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text)
		: Field(hParentWnd, x, y, width, height, id, text) {
		m_type = L"STATIC";
	};
	~ImageField() {};


protected:
};


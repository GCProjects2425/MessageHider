#pragma once
#include "Field.h"

class TextField : public Field
{
public:
	TextField(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text)
		: Field(hParentWnd, x, y, width, height, id, text) {m_type = L"EDIT";};
	~TextField() {};


protected:
};


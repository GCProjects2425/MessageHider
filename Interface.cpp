#include "Interface.h"


Interface::~Interface(){}


void Interface::CreateInterface()
{
    buttons.push_back(Button());
    buttons.back().CreateButton(parentWindow, 10, 100, 150, 30, 2, L"Importer Image");
}

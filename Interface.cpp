﻿#include "Interface.h"
#include "ElementsHeaders.h"



Interface::~Interface(){}


void Interface::CreateInterface()
{
    UIElement* loadImageButton = new BrowseButton(parentWindow, 70, 80, 250, 40, 1, L"Load Image");
    elements.push_back(loadImageButton);
    UIElement* filePathField = new TextField(parentWindow, 360, 80, 620, 40, 2, L"File path");
    elements.push_back(filePathField);
    UIElement* textField = new TextField(parentWindow, 70, 760, 1260, 280, 3, L"");
    elements.push_back(textField);
    UIElement* previewField = new ImageField(parentWindow, 70, 140, 910, 600, 4, L"");
    elements.push_back(previewField);
    UIElement* filtersField = new ImageField(parentWindow, 1000, 140, 330, 600, 5, L"");
    elements.push_back(filtersField);
    UIElement* propertiesField = new ImageField(parentWindow, 1350, 140, 520, 780, 6, L"");
    elements.push_back(propertiesField);
    UIElement* exportButton = new BrowseButton(parentWindow, 1630, 970, 250, 40, 7, L"Export");
    elements.push_back(exportButton);
    UIElement* decodeButton = new ActionButton(parentWindow, 1350, 970, 250, 40, 8, L"Decode");
    elements.push_back(decodeButton);


    for (auto element : elements) 
    {
        element->CreateElement();
    }
}


void Interface::HandleCommands(WPARAM wParam)
{
    for (auto element : elements)
        element->HandleCommand(wParam);
}
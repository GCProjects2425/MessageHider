#include "Interface.h"
#include "ElementsHeaders.h"



Interface::~Interface(){}


void Interface::CreateInterface()
{
    UIElement* loadImageButton = new BrowseButton(parentWindow, 10, 10, 250, 40, 1, L"Load Image");
    elements.push_back(loadImageButton);
    UIElement* filePathField = new TextField(parentWindow, 360, 10, 620, 40, 2, L"File path");
    elements.push_back(filePathField);
    UIElement* textField = new TextField(parentWindow, 10, 760, 1260, 280, 3, L"");
    elements.push_back(textField);
    UIElement* previewField = new ImageField(parentWindow, 10, 60, 910, 600, 4, L"");
    elements.push_back(previewField);
    UIElement* filtersField = new ImageField(parentWindow, 930, 60, 330, 600, 5, L"");
    elements.push_back(filtersField);
    UIElement* propertiesField = new ImageField(parentWindow, 1270, 60, 520, 780, 6, L"");
    elements.push_back(propertiesField);
    UIElement* encryptButton = new ActionButton(parentWindow, 1630, 940, 250, 40, 7, L"Encrypt");
    elements.push_back(encryptButton);
    UIElement* decryptButton = new ActionButton(parentWindow, 1350, 940, 250, 40, 8, L"Decrypt");
    elements.push_back(decryptButton);
    UIElement* exportButton = new BrowseButton(parentWindow, 1350, 1000, 520, 40, 9, L"Export");
    elements.push_back(exportButton);


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
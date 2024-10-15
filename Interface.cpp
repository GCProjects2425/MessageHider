#include "Interface.h"
#include "ElementsHeaders.h"
#include "framework.h"


Interface::~Interface(){}


void Interface::CreateInterface()
{
    UIElement* loadImageButton = new BrowseButton(parentWindow, 20, 20, 300, 40, IDM_OPEN_FILE, L"Load Image");
    elements.push_back(loadImageButton);
    UIElement* filePathField = new Field(parentWindow, 360, 20, 620, 40, 2, L"File path");
    elements.push_back(filePathField);
    UIElement* textField = new TextField(parentWindow, 20, 800, 1340, 200, 69, L"");
    elements.push_back(textField);
    UIElement* previewField = new ImageField(parentWindow, 20, 80, 1000, 700, 4, L"oui");
    elements.push_back(previewField);
    UIElement* filtersField = new Field(parentWindow, 1040, 80, 320, 700, 5, L"yo");
    elements.push_back(filtersField);
    UIElement* propertiesField = new Field(parentWindow, 1380, 80, 500, 800, 6, L"yo");
    elements.push_back(propertiesField);
    UIElement* encryptButton = new ActionButton(parentWindow, 1640, 900, 240, 40, 7, L"Encrypt");
    elements.push_back(encryptButton);
    UIElement* decryptButton = new ActionButton(parentWindow, 1380, 900, 240, 40, 8, L"Decrypt");
    elements.push_back(decryptButton);
    UIElement* exportButton = new BrowseButton(parentWindow, 1380, 960, 500, 40, IDM_SAVE_FILE, L"Export");
    elements.push_back(exportButton);


    for (UIElement* element : elements) 
    {
        element->CreateElement();
    }
}


void Interface::HandleCommands(UINT message)
{
    for (auto element : elements)
        element->HandleCommand(message);
}
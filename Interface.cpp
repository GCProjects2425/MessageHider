#include "Interface.h"
#include "ElementsHeaders.h"



Interface::~Interface(){}


void Interface::CreateInterface()
{
    UIElement* loadImageButton = new BrowseButton(parentWindow, 70, 80, 250, 40, 1, L"Load Image");
    elements.push_back(loadImageButton);
    UIElement* filePathField = new TextField(parentWindow, 360, 80, 620, 40, 3, L"File path");
    elements.push_back(filePathField);
    UIElement* textField = new TextField(parentWindow, 70, 760, 1260, 280, 3, L"");
    elements.push_back(textField);
    UIElement* previewField = new ImageField(parentWindow, 70, 140, 910, 600, 3, L"");
    elements.push_back(previewField);
    UIElement* filtersField = new ImageField(parentWindow, 1000, 140, 330, 600, 3, L"");
    elements.push_back(filtersField);
    UIElement* propertiesField = new ImageField(parentWindow, 1350, 140, 520, 780, 3, L"");
    elements.push_back(propertiesField);
    UIElement* quitButton = new ActionButton(parentWindow, 1630, 970, 250, 40, 2, L"Quit");
    elements.push_back(quitButton);
    UIElement* exportButton = new ActionButton(parentWindow, 1350, 970, 250, 40, 4, L"Export");
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
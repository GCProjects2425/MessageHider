// header.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
// Fichiers d'en-tête Windows
#include <windows.h>
// Fichiers d'en-tête C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>

#include <commdlg.h>
#include <functional>

#include "Resource.h"

#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

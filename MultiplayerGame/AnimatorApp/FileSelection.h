#pragma once
#include "windows.h"
#include <commdlg.h>
#include <string>
#include <iostream>

#pragma comment(lib, "Comdlg32.lib")

std::string OpenFileDialog(bool isAnim = true);

std::string SaveFileDialog();
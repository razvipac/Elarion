#include "FileSelection.h"

std::string OpenFileDialog(bool isAnim) {
	OPENFILENAMEW ofn;       // Note the 'W' at the end for wide char version
	WCHAR szFile[260];       // buffer for file name, using wide char
	HWND hwnd = NULL;        // owner window
	HANDLE hf;               // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);
	if (isAnim)
		ofn.lpstrFilter = L"Animation Files\0*.ANIM\0";
	else
		ofn.lpstrFilter = L"Animator Files\0*.ANIMATOR\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box 
	if (GetOpenFileNameW(&ofn) == TRUE) { // Note the 'W' at the end for wide char version
		hf = CreateFileW(ofn.lpstrFile, // Note the 'W' at the end for wide char version
			GENERIC_READ,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);
		if (hf != INVALID_HANDLE_VALUE) {
			CloseHandle(hf);
			// Convert from wide char to narrow char array
			char narrowFilePath[260];
			WideCharToMultiByte(CP_UTF8, 0, ofn.lpstrFile, -1, narrowFilePath, 260, NULL, NULL);
			return std::string(narrowFilePath);
		}
	}
	return "";
}
std::string SaveFileDialog() {
	OPENFILENAMEW ofn;       // common dialog box structure
	WCHAR szFile[260];       // buffer for file name, using wide char
	HWND hwnd = NULL;        // owner window

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetSaveFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);
	ofn.lpstrFilter = L"Animator Files\0*.ANIMATOR\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	// Display the Save As dialog box 
	if (GetSaveFileNameW(&ofn) == TRUE) {
		// Convert from wide char to narrow char array
		char narrowFilePath[260];
		WideCharToMultiByte(CP_UTF8, 0, ofn.lpstrFile, -1, narrowFilePath, 260, NULL, NULL);
		return std::string(narrowFilePath);
	}
	return "";
}
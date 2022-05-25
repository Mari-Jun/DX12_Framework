#include "stdafx.h"
#include <commdlg.h>

#include "client/util/file_dialog.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	std::string FileDialog::OpenFile(const wchar_t* filter)
	{
		OPENFILENAMEW ofn;

		TCHAR file_path_name[256] = L"";
		TCHAR current_directory[256] = L"";

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = Render::GetWindowHWND();
		ofn.lpstrFile = file_path_name;
		ofn.nMaxFile = sizeof(file_path_name);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		if (GetCurrentDirectoryW(256, current_directory))
			ofn.lpstrInitialDir = current_directory;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameW(&ofn) == TRUE)
			return utf8_encode(ofn.lpstrFile);

		return std::string();
	}
}

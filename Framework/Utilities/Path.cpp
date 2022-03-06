#include"Framework.h"
#include"Path.h"
#include <string.h>
#include <shlwapi.h>
#include "String.h"
#pragma comment(lib, "shlwapi.lib")

bool Path::ExistFile(string path)
{
	return ExistFile(String::ToWString(path));
}

bool Path::ExistFile(wstring path)
{
	return false;
}

bool Path::ExistDirectory(string path)
{
	return false;
}

bool Path::ExistDirectory(wstring path)
{
	return false;
}

string Path::Combine(string path1, string path2)
{
	return string();
}

wstring Path::Combine(wstring path1, wstring path2)
{
	return wstring();
}

string Path::Combine(vector<string> paths)
{
	return string();
}

string Path::GetDirectoryName(string path)
{
	return string();
}

wstring Path::GetDirectoryName(wstring path)
{
	return wstring();
}

string Path::GetExtension(string path)
{
	return string();
}

wstring Path::GetExtension(wstring path)
{
	return wstring();
}

string Path::GetFileName(string path)
{
	return string();
}

wstring Path::GetFileName(wstring path)
{
	return wstring();
}

string Path::GetFileNameWithoutExtension(string path)
{
	return string();
}

wstring Path::GetFileNameWithoutExtension(wstring path)
{
	return wstring();
}

void Path::OpenFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func, HWND hwnd)
{
}

void Path::SaveFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func, HWND hwnd)
{
}

void Path::GetFiles(vector<string>* files, string path, string filter, bool bFindSubFolder)
{
}

void Path::GetFiles(vector<wstring>* files, wstring path, wstring filter, bool bFindSubFolder)
{
}

void Path::CreateFolder(string path)
{
}

void Path::CreateFolder(wstring path)
{
}

void Path::CreateFolders(string path)
{
}

void Path::CreateFolders(wstring path)
{
}

bool Path::IsRelativePath(string path)
{
	return false;
}

bool Path::IsRelativePath(wstring path)
{
	return false;
}

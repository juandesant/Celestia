// winutil.h
// 
// Copyright (C) 2002, Chris Laurel <claurel@shatters.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include "winutil.h"

void SetMouseCursor(LPCTSTR lpCursor)
{
	HCURSOR hNewCrsr;
	
	if (hNewCrsr = LoadCursor(NULL, lpCursor))
	    SetCursor(hNewCrsr);
}

void CenterWindow(HWND hParent, HWND hWnd)
{
    //Center window with hWnd handle relative to hParent.
    if (hParent && hWnd)
    {
        RECT or, ir;
        if (GetWindowRect(hParent, &or))
        {
            if (GetWindowRect(hWnd, &ir))
            {
                int x, y;

                x = or.left + (or.right - or.left - (ir.right - ir.left)) / 2;
                y = or.top + (or.bottom - or.top - (ir.bottom - ir.top)) / 2;;
                SetWindowPos(hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            }
        }
    }
}

void RemoveButtonDefaultStyle(HWND hWnd)
{
    SetWindowLong(hWnd, GWL_STYLE,
		::GetWindowLong(hWnd, GWL_STYLE) & ~BS_DEFPUSHBUTTON);
	InvalidateRect(hWnd, NULL, TRUE);
}

void AddButtonDefaultStyle(HWND hWnd)
{
    SetWindowLong(hWnd, GWL_STYLE,
        ::GetWindowLong(hWnd, GWL_STYLE) | BS_DEFPUSHBUTTON);
	InvalidateRect(hWnd, NULL, TRUE);
}

const char* CurrentCP()
{
    static bool set = false;
    static char cp[20] = "CP";
    if (!set) {
        GetLocaleInfo(GetThreadLocale(), LOCALE_IDEFAULTANSICODEPAGE, cp+2, 18);
        set = true;
    }
    return cp;
}

string UTF8ToCurrentCP(const string& str)
{
    return WideToCurrentCP(UTF8ToWide(str));
}

string CurrentCPToUTF8(const string& str)
{
    return WideToUTF8(CurrentCPToWide(str));
}

string WStringToString(UINT codePage, const wstring& ws)
{
    if (ws.empty())
        return {};
    // get a converted string length
    auto len = WideCharToMultiByte(codePage, 0, ws.c_str(), ws.length(), NULL, 0, nullptr, nullptr);
    string out(len, 0);
    WideCharToMultiByte(codePage, 0, ws.c_str(), ws.length(), &out[0], len, nullptr, nullptr);
    return out;
}

wstring StringToWString(UINT codePage, const string& s)
{
    if (s.empty())
        return {};
    // get a converted string length
    auto len = MultiByteToWideChar(codePage, 0, s.c_str(), s.length(), nullptr, 0);
    wstring out(len, 0);
    MultiByteToWideChar(codePage, 0, s.c_str(), s.length(), &out[0], len);
    return out;
}

string WideToCurrentCP(const wstring& ws)
{
    return WStringToString(CP_ACP, ws);
}

wstring CurrentCPToWide(const string& s)
{
    return StringToWString(CP_ACP, s);
}

string WideToUTF8(const wstring& ws)
{
    return WStringToString(CP_UTF8, ws);
}

string UTF8ToCurrentOEMCP(const string& str)
{
    string localeStr;
    LPWSTR wout = new wchar_t[str.length() + 1];
    LPSTR out = new char[str.length() + 1];
    int wlength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wout, str.length() + 1);
    WideCharToMultiByte(CP_OEMCP, 0, wout, -1, out, str.length() + 1, NULL, NULL);
    localeStr = out;
    delete [] wout;
    delete [] out;
    return localeStr;
}

#include "WindowResizer.h"
#include <iostream>

#define vector std::vector
#define endl std::endl
#define cout std::cout
#define uint unsigned int

vector<HWND> windowHandles;
vector<std::string> windowNames;
vector<RECT> monitors;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) //get all windows
{
	if (IsWindowVisible(hwnd) && (GetWindowLong(hwnd, GWL_STYLE) & WS_EX_APPWINDOW)) {
		windowHandles.push_back(hwnd);
		char temp[100];
		GetWindowText(hwnd, temp, 100);
		windowNames.push_back(std::string(temp));
	}
	return 1;
}

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)//get all displays
{
	MONITORINFO info;
	info.cbSize = sizeof(info);
	if (GetMonitorInfo(hMonitor, &info))
		monitors.push_back(info.rcWork);
	return 1;
}

vector<RECT> parts() {
	vector<RECT> res;
	for (RECT curr : monitors) {
		int top = curr.top;
		int bottom = curr.bottom;
		int left = curr.left;
		int right = curr.right;
		int halfH = (top + bottom) / 2;
		int halfW = std::abs((left + right) / 2);
		for (int i = 1; i <= 2; i++) {
			for (int j = 1; j <= 2; j++) {
				RECT temp = {};
				temp.top = top;
				temp.bottom = halfH;
				temp.left = left;
				temp.right =  halfW;
				res.push_back(temp);
				left = (left + halfW);
			}
			left = curr.left;
			top = (halfH + top);
		}
	}
	return res;
}

WindowResizer::WindowResizer(BOOL debug) {
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, NULL);
	EnumWindows(EnumWindowsProc, NULL);
	vector<RECT> part = parts();
	int x = 0;
	for (HWND handle : windowHandles) {
		if (x >= part.size()) x = 0;
		if (debug) {
			cout << "LEFT= "<< part.at(x).left << " RIGHT= " << part.at(x).right << " TOP= " << part.at(x).top << "  BOTTOM= " << part.at(x).bottom << endl;
			char temp[100];
			GetWindowText(handle, temp, 100);
			cout << temp << endl << endl;
		}
		SetWindowPos(handle, NULL, part.at(x).left, part.at(x).top, part.at(x).right, part.at(x).bottom, NULL);
		x++;
	}
}

int main()
{
	BOOL dBug = 1;
	WindowResizer wR = WindowResizer(dBug);
	if (dBug) {
		cout << "debug boolean is true";
		cout << endl;
		for (std::string s : windowNames) {
			cout << s << endl;
		}
		cout << endl;
		for (RECT part : monitors) {
			cout << part.left << "  " << part.right << "  " << part.top << "  " << part.bottom << endl;
		}
	}
	//MessageBoxA(NULL, "yeet", "warningbox yea bro", MB_ICONWARNING | MB_OK);
	char a;
	std::cin >> a;
}


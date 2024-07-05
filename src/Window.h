#pragma once
#include "pch.h"

class Window{
    public:
        Window(HINSTANCE hInstance);
        Window(HINSTANCE hInstance, int width, int height);

        HWND RegisterWindowClass(HINSTANCE hInstance);
        int _width {0};
        int _height {0};
        int _newWidth {0};
        int _newHeight {0};    
        HWND GetHandle() {return _handle;};
        void Show(int nCmdShow) {ShowWindow(_handle, nCmdShow);};
    private:
        HWND _handle;
};
#include "pch.h"
#include "image.h"




image::image(char* path, HINSTANCE hInst)
{/////////////////////////////////////////////////////////
    ImageInit = 0;
    init(path, hInst);
}

void image::init(const char* path, HINSTANCE hInst)
{/////////////////////////////////////////////////////////
    if (ImageInit) DeleteObject(hBitmap);

    ImageInit = Bitmap.bmWidth = Bitmap.bmHeight = 0;

    hBitmap = LoadBitmap(hInst, path);
    if (!hBitmap)
    {
        hBitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (!hBitmap) return;
    }
    if (!GetObject(hBitmap, sizeof(BITMAP), &Bitmap))
    {
        DeleteObject((HGDIOBJ)hBitmap);
        return;
    }

    ImageInit = 1;
}


image::~image(void)
{///////////////////////////////////////////////////////////////////////////////////
    if (ImageInit) DeleteObject((HGDIOBJ)hBitmap);
}

void image::clear(void)
{///////////////////////////////////////////////////////////////////////////////////
    if (ImageInit) DeleteObject(hBitmap);

    ImageInit = Bitmap.bmWidth = Bitmap.bmHeight = 0;
}

void image::put(HWND hwnd, LONG x, LONG y, DWORD dwRop)
{//////////////////////////////////////////////////////////////////////////////////
    if (!ImageInit) return;
    HDC hdc = GetDC(hwnd);
    HDC hCDC = CreateCompatibleDC(hdc);
    if (!hCDC) return;
    SelectObject(hCDC, hBitmap);
    BitBlt(hdc, x, y, Bitmap.bmWidth, Bitmap.bmHeight, hCDC, 0, 0, dwRop);
    DeleteDC(hCDC);
    ReleaseDC(hwnd, hdc);
}

void image::put(HDC hdc, LONG x, LONG y, DWORD dwRop)
{//////////////////////////////////////////////////////////////////////////////////
    if (!ImageInit) return;
    HDC hCDC = CreateCompatibleDC(hdc);
    if (!hCDC) return;
    SelectObject(hCDC, hBitmap);
    BitBlt(hdc, x, y, Bitmap.bmWidth, Bitmap.bmHeight, hCDC, 0, 0, dwRop);
    DeleteDC(hCDC);
}

void image::put(HDC hdc, LONG x, LONG y, LONG w, LONG h, DWORD dwRop)
{//////////////////////////////////////////////////////////////////////////////////
    if (!ImageInit) return;
    HDC hCDC = CreateCompatibleDC(hdc);
    if (!hCDC) return;
    SelectObject(hCDC, hBitmap);
    StretchBlt(hdc, x, y, w, h, hCDC, 1, 1, Bitmap.bmWidth, Bitmap.bmHeight, dwRop);
    DeleteDC(hCDC);
}


void image::get(HWND hwnd, LONG Xo, LONG Yo, LONG width, LONG height, DWORD dwRop)
{//////////////////////////////////////////////////////////////////////////////////
    if (ImageInit) DeleteObject(hBitmap);

    //ImageInit=Bitmap.bmWidth=Bitmap.bmHeight=0;
    ImageInit = 0;

    HDC hdc = GetDC(hwnd);
    HDC hCDC = CreateCompatibleDC(hdc);
    if (!hCDC) return;
    hBitmap = CreateCompatibleBitmap(hdc, width, height);
    if (hBitmap == 0) return;
    SelectObject(hCDC, hBitmap);
    SetBitmapDimensionEx(hBitmap, width, height, NULL);
    if (!GetObject(hBitmap, sizeof(BITMAP), &Bitmap)) return;
    BitBlt(hCDC, 0, 0, width, height, hdc, Xo, Yo, dwRop);
    DeleteDC(hCDC);
    ReleaseDC(hwnd, hdc);

    ImageInit = 1;
}


void image::get(HDC hdc, LONG Xo, LONG Yo, LONG width, LONG height, DWORD dwRop)
{//////////////////////////////////////////////////////////////////////////////////
    if (ImageInit) DeleteObject(hBitmap);

    ImageInit = Bitmap.bmWidth = Bitmap.bmHeight = 0;

    HDC hCDC = CreateCompatibleDC(hdc);
    if (!hCDC) return;
    hBitmap = CreateCompatibleBitmap(hdc, width, height);
    if (!hBitmap) return;
    SelectObject(hCDC, hBitmap);
    SetBitmapDimensionEx(hBitmap, width, height, NULL);
    if (!GetObject(hBitmap, sizeof(BITMAP), &Bitmap)) return;
    BitBlt(hCDC, 0, 0, width, height, hdc, Xo, Yo, dwRop);
    DeleteDC(hCDC);

    ImageInit = 1;
}
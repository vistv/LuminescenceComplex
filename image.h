#ifndef __IMAGE_H
#define __IMAGE_H
#include <afxwin.h>
// ����������� ������ ������ image (��������� - ���� � ����� *.bmp
// (���������� ��������� �� �����������), 
// ���� ������������� BITMAPa-������� � HINSTANCE ���������), ����� � �������
// .put() �������� ��������� �� �����, � � ������� .get() - ����������� �
// ������

// ���� ���������� �� �������, ��������� ������� ���������� ���������
// bitmap �� ��������, ����� �� .bmp-����� � ��������� ������

class image
{
    BITMAP Bitmap;
    HBITMAP hBitmap;
    int ImageInit;

public:
    image(char*, HINSTANCE); // ����������� � ��� ����� � ��� �������
    image(void) { ImageInit = 0; }
    ~image(void);

    void init(const char*, HINSTANCE hInst = 0);
    void clear(void);

    int error(void) { return (!ImageInit); } // ���� ���������� ����, �������� ���������

    // ����� �� �����
    void put(HWND hwnd, LONG x = 0, LONG y = 0, DWORD dwRop = SRCCOPY);
    void put(HDC hdc, LONG x = 0, LONG y = 0, DWORD dwRop = SRCCOPY);

    // ����� �� ����� � �����������/�������:
    void put(HDC hdc, LONG x, LONG y, LONG w, LONG h, DWORD dwRop);

    // "�����" �������� � ������
    void get(HWND hwnd, LONG Xo, LONG Yo, LONG width, LONG height, DWORD dwRop = SRCCOPY);
    void get(HDC hdc, LONG Xo, LONG Yo, LONG width, LONG height, DWORD dwRop = SRCCOPY);

    LONG width(void) { return Bitmap.bmWidth; } // ������ ��������
    LONG height(void) { return Bitmap.bmHeight; } // ������ ��������

    HBITMAP hBit(void) { return hBitmap; }
    HBITMAP operator* (void) { return hBitmap; };
};

#endif
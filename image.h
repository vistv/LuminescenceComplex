#ifndef __IMAGE_H
#define __IMAGE_H
#include <afxwin.h>
// Объявляется объект класса image (параметры - путь к файлу *.bmp
// (расширение указывать не обязательно), 
// либо идентификатор BITMAPa-ресурса и HINSTANCE программы), затем с помощью
// .put() картинка выводится на экран, а с помощью .get() - считывается с
// экрана

// Если расширение не указано, программа сначала попытается загрузить
// bitmap из ресурсов, затем из .bmp-файла с указанным именем

class image
{
    BITMAP Bitmap;
    HBITMAP hBitmap;
    int ImageInit;

public:
    image(char*, HINSTANCE); // Конструктор и для файла и для ресурса
    image(void) { ImageInit = 0; }
    ~image(void);

    void init(const char*, HINSTANCE hInst = 0);
    void clear(void);

    int error(void) { return (!ImageInit); } // Если возвращает ноль, картинка загружена

    // Вывод на экран
    void put(HWND hwnd, LONG x = 0, LONG y = 0, DWORD dwRop = SRCCOPY);
    void put(HDC hdc, LONG x = 0, LONG y = 0, DWORD dwRop = SRCCOPY);

    // Вывод на экран с растяжением/сжатием:
    void put(HDC hdc, LONG x, LONG y, LONG w, LONG h, DWORD dwRop);

    // "Взять" картинку с экрана
    void get(HWND hwnd, LONG Xo, LONG Yo, LONG width, LONG height, DWORD dwRop = SRCCOPY);
    void get(HDC hdc, LONG Xo, LONG Yo, LONG width, LONG height, DWORD dwRop = SRCCOPY);

    LONG width(void) { return Bitmap.bmWidth; } // Ширина картинки
    LONG height(void) { return Bitmap.bmHeight; } // Высота картинки

    HBITMAP hBit(void) { return hBitmap; }
    HBITMAP operator* (void) { return hBitmap; };
};

#endif
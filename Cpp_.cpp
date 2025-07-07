#include <windows.h>
#include <iostream>
#include <chrono>

#define Bhind_Window // закоментить если нужена консоль

inline void presstranslat() noexcept{
    keybd_event(VK_MENU, 0, 0, 0);      // Нажатие Alt
    keybd_event(VK_LSHIFT, 0, 0, 0);        // Нажатие Shift

    Sleep(50); // Короткая задержка

    keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);   // Отпустить Shift
    keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0); // Отпустить Alt
}


inline void trackDoubleAlt(int maxDelayMs = 500) noexcept {

    bool t1 = true;
    short cunt = 1;

#ifdef Bhind_Window // для фонового режима 
    TCHAR consTitle[MAX_PATH];
    GetConsoleTitle(consTitle, MAX_PATH);
    HWND hWnd = FindWindow(0, consTitle);

    SetWindowLong(hWnd, 0, (LONG)::GetDesktopWindow());
    ShowWindow(hWnd, SW_HIDE); // скрываем окно 

 #endif
    bool firstPressDetected = false;
    auto firstPressTime = std::chrono::steady_clock::now();

    while (true) {

        // VK_MENU — это Alt
        if (GetAsyncKeyState(VK_MENU) & 0x8000) {

            if (!firstPressDetected) {
                firstPressDetected = true;
                firstPressTime = std::chrono::steady_clock::now();
               
                Sleep(150); // Задержка от дребезга
            }

            else {
                // Обработка второго нажатия 
                auto now = std::chrono::steady_clock::now();
                auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - firstPressTime).count();

                if (elapsedMs <= maxDelayMs) {
                    presstranslat();
                }

                firstPressDetected = false;
                Sleep(200); // Защита от повторного дребезга
            }
        }

        // Выход по ESC
        if (GetAsyncKeyState(VK_END) & 0x8000) { // exit если нажмёшь END

            ShowWindow(hWnd, SW_SHOW); // открываем окно или закрываем если нет define
            Sleep(500);
            break;
        }

        Sleep(20); // Минимальная задержка чтобы не было нагрузки на ЦП
    }
}


int main() noexcept{
    setlocale(LC_ALL, "RU");

    trackDoubleAlt();
       
    return 0;
}

#include <Windows.h>
#include "bot.h"
void makeOverlayWindowTransparent(HWND hwnd) {
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TOPMOST);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY); // Make black transparent
}
int GetTrayTopWhenVisible(unsigned height) {
    HWND hShellTrayWnd = FindWindow(L"Shell_TrayWnd", nullptr);
    if (!hShellTrayWnd) return height;

    HWND hTrayNotifyWnd = FindWindowEx(hShellTrayWnd, nullptr, L"TrayNotifyWnd", nullptr);
    if (!hTrayNotifyWnd) return height;

    RECT trayRect;
    if (GetWindowRect(hTrayNotifyWnd, &trayRect)) {
        return trayRect.top;
    }

    return height;
}

int main() {
    #pragma region windowSetup
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned width = desktop.size.x;
    unsigned height = desktop.size.y - 1;
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u({ width, height })), "Overlay", sf::Style::None);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    makeOverlayWindowTransparent(window.getNativeHandle());
    #pragma endregion
    #pragma region class
    const sf::Texture texture;
    sf::Sprite temp(texture);
    bot ph(window, temp, texture);
    ph.PageSize = { width, GetTrayTopWhenVisible(height) };
    if (window.isOpen()) {
        ph.SetBaseVariables();
    }
    ph.SetBaseVariables();
#pragma endregion
    while (window.isOpen()) {
        while (std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())window.close();
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && ph.MouseCollision()) {
            ph.pickup();
        }
        if (steady_clock::now() >= ph.prev + ph.interval) {
            ph.prev = steady_clock::now();
            ph.Fall();
            ph.BotBehaviour();
        }
        ph.UpdatePage();
    }
    return 0;
}
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <chrono>
void makeOverlayWindowTransparent(HWND hwnd) {
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TOPMOST);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY); // Make black transparent
}
using namespace std::chrono;

int main() {
    #pragma region windowSetup
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned width = desktop.size.x;
    unsigned height = desktop.size.y - 1;
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u({ width, height })), "Overlay", sf::Style::None);
    window.setVerticalSyncEnabled(true);
    makeOverlayWindowTransparent(window.getNativeHandle());
    #pragma endregion
    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::Green);
    rect.setSize({100, 200});
    rect.setPosition({ 300, 400 });

    steady_clock::time_point prev = steady_clock::now();
    milliseconds interval = milliseconds(20);
    while (window.isOpen()) {
        while (std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())window.close();
        }
        while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            rect.setPosition(sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y));
            window.clear(sf::Color::Black);
            window.draw(rect);
            window.display();
        }
        float x = rect.getPosition().x;
        float y = rect.getPosition().y;
        float yaxis = rect.getSize().y;
        if (steady_clock::now() >= prev + interval && y + yaxis + 5.f < height) {
            prev = steady_clock::now();
            rect.setPosition({x, y + 5.f});
        }
        window.clear(sf::Color::Black); 
        window.draw(rect);
        window.display();
    }

    return 0;
}
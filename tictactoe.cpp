#include <array>
#include <iostream>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "window.hpp"
void centerTOrigin(sf::Text &text, const sf::Vector2f& size){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin((textRect.left + textRect.width /2.0f), (textRect.top + textRect.height / 2.0f));
    text.setPosition(size.x / 2, size.y / 2);
}
void printTicTacToe(std::array<std::array<std::string, 3>, 3>& ttt){
    for (std::array<std::string, 3> arr: ttt){
        std::cout << arr[0] << "|" << arr[1] << "|" << arr[2] << std::endl;
    }
}       
int main(){
    std::array<std::array<std::string, 3>, 3> ttt;
    for (std::array<std::string, 3> arr: ttt){
        for (int i = 0; i < arr.size(); ++i){
            arr[i] = (" ");
        }
    }

    sf::RenderWindow TTTWindow(sf::VideoMode(600, 700), "Tic Tac Toe", sf::Style::Titlebar | sf::Style::Close);
    sf::Font font;
    font.loadFromFile("files/font.ttf");
    sf::Text enterInfo("Enter row, column, symbol: ", font, 14);
    enterInfo.setColor(sf::Color::Black);
    enterInfo.setStyle(sf::Text::Underlined | sf::Text::Italic);
    enterInfo.setPosition(50, 600);
    // centerTOrigin(enterInfo, static_cast<sf::Vector2f>(TTTWindow.getSize()));
    std::cout << enterInfo.getPosition().x << std::endl;
    while (TTTWindow.isOpen()){
        sf::Event e;
        while (TTTWindow.pollEvent(e)){
            switch (e.type){
                case sf::Event::Closed:
                    TTTWindow.close();
            }
        }
        TTTWindow.clear(sf::Color::Cyan);
        TTTWindow.draw(enterInfo);
        TTTWindow.display();


    }


    printTicTacToe(ttt);
    return 0;
}
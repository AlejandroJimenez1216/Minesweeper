#include <SFML/Graphics.hpp>
#include <iostream>
#include <cctype>
#include "window.hpp"
void centerTOrigin(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin((textRect.left + textRect.width /2.0f), (textRect.top + textRect.height / 2));
    text.setPosition(sf::Vector2f(x, y));
}
std::string runWelcome(sf::RenderWindow& window){
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")){
        std::cerr << "Could not load file" << "\n";
    }
    sf::Text title("WELCOME TO MINESWEEPER!", font, 24);
    sf::Text enterName("Enter Your Name:", font, 20);
    sf::Text name("|", font, 18);
    sf::String nameStr;
    sf::Vector2u wSize = window.getSize();
    float wMidX = wSize.x / 2;
    float wMidY = wSize.y / 2;
    centerTOrigin(title, wMidX, wMidY - 150);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    centerTOrigin(enterName, wMidX, wMidY - 75);
    enterName.setFillColor(sf::Color::White);
    enterName.setStyle(sf::Text::Bold);
    centerTOrigin(name, wMidX, wMidY - 45);
    name.setFillColor(sf::Color::Yellow);
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            switch(event.type){
                case sf::Event::Closed:
                    nameStr = "";
                    window.close();
                    break;
                case sf::Event::TextEntered:
                    if (event.text.unicode < 128){
                        if (event.text.unicode == 8){
                            if (nameStr.getSize() != 0){                        
                                nameStr.erase(nameStr.getSize() - 1);
                                centerTOrigin(name, wMidX, wMidY - 45);
                                name.setString(nameStr + "|");
                            }
                        }else if (event.text.unicode == 13 && !nameStr.isEmpty()){
                            window.close();                   
                            break;
                        }else if (std::string(nameStr).size() >= 10 || !std::isalpha(event.text.unicode)){
                            break;
                        }else{
                            if(std::string(nameStr).size() == 0){
                                char upper = std::toupper(static_cast<char>(event.text.unicode));
                                nameStr += sf::String(upper);
                            }else{
                                char lower = std::tolower(static_cast<char>(event.text.unicode));
                                nameStr += sf::String(lower);
                            }
                            centerTOrigin(name, wMidX, wMidY - 45);
                            name.setString(nameStr + "|");
                            centerTOrigin(name, wMidX, wMidY - 45);                           
                        }
                    }
                    std::cout << std::endl;
            }
        }
        window.clear(sf::Color::Blue);
        window.draw(enterName);
        window.draw(title);
        window.draw(name);
        window.display();
    }
    return std::string(nameStr);
}
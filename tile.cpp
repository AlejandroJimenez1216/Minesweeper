#include "window.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include<map>
#include<string>
#include<memory>
void spriteMaker(sf::Sprite& s, sf::Texture& t, sf::Vector2f pos){
    s.setTexture(t);
    s.setPosition(pos);
}
bool within(sf::Vector2i& mouse, sf::Vector2f& pos, sf::Sprite& s){
    return mouse.x > pos.x && mouse.x < (pos.x + s.getLocalBounds().width) && mouse.y > pos.y && mouse.y < (pos.y + s.getLocalBounds().height);
}
Tile::Tile(){
    x = 0;
    y = 0;
    path = "tile_revealed.png";
    neighbors.reserve(8);
}
Tile::Tile(float xCord, float yCord, std::string stringy): x(xCord), y(yCord), path(stringy){
    neighbors.reserve(8);
}
void Tile::reset(){
    x = 0;
    y = 0;
    path = "tile_revealed.png";
    isMine = false;
    isFlag = false;
    clickedOn = false;
    isEmpty = false;
}



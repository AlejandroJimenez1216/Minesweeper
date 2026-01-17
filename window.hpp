#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <string>
void centerTOrigin(sf::Text &text, float x, float y);
std::string runWelcome(sf::RenderWindow& window);
// struct spriteStruct{
//     sf::Texture texture;
//     sf::Sprite sprite;
// };
void runLeaderboard(int& colNum, int& rowNum, bool& gameWon);
bool within(sf::Vector2i& mouse, sf::Vector2f& pos, sf::Sprite& s);
void lbwrite(std::multimap<int, std::string>& mapp, std::multimap<int, std::string>::iterator currIter);
void insertTime(std::multimap<int, std::string>& mapp, int& num, std::string& str);
void spriteMaker(sf::Sprite& s, sf::Texture& t, sf::Vector2f pos = sf::Vector2f(0,0));
class Tile{
    public:
    float x;
    float y;
    bool isMine = false;
    bool isFlag = false;
    bool clickedOn = false;
    bool isEmpty = false;
    std::vector<Tile*> neighbors;
    std::string path;
    Tile();
    Tile(float x, float y, std::string stringy);
    void reset();
};
class Board{
    public:
    int allTilesRev;
    int colNum;
    int rowNum;
    int mineNum;
    bool gameOver;
    bool debugMode;
    bool paused;
    bool gameWon;
    bool lbPressed;
    int yesFlag;
    int mineCount;
    Tile* tileArr;
    Board();
    Board(int colNumb, int rowNumb, int mineNumb);
    Board(const Board& other);
    Board& operator=(const Board& other);
    Board(Board&& other) noexcept;
    Board& operator=(Board&& other) noexcept;
    void pushV(int& idx);
    void neighborSetter();
    void boardMaker();
    void exposeTiles(Tile* tile);
    void reset();

    ~Board();
};

#endif
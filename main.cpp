#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <random>
#include <utility>
#include "window.hpp"
#include <map>
#include <chrono>
int main(){
    std::ifstream configFile("files/config.cfg");
    int colNum;
    int rowNum;
    int mineNum;
    if(configFile.is_open()){
        configFile >> colNum >> rowNum >> mineNum;
        configFile.close();
    }else{std::cerr << "ConfigFile did not open" << std::endl;}
    sf::RenderWindow gameWindow(sf::VideoMode(colNum * 32, rowNum * 32 + 100), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
    std::string nameStr = runWelcome(gameWindow);
    gameWindow.create(sf::VideoMode(colNum * 32, rowNum * 32 + 100), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
    auto beginning  = std::chrono::high_resolution_clock::now();
    Board gameB(colNum, rowNum, mineNum);
    gameB.boardMaker();
    std::map<std::string, std::shared_ptr<sf::Texture>> txMap{{"debug.png", std::make_shared<sf::Texture>()}
    ,{"digits.png", std::make_shared<sf::Texture>()} 
    ,{"face_happy.png",std::make_shared<sf::Texture>()}
    ,{"face_lose.png",std::make_shared<sf::Texture>()}
    ,{"face_win.png",std::make_shared<sf::Texture>()}
    ,{"flag.png", std::make_shared<sf::Texture>()}
    ,{"leaderboard.png",std::make_shared<sf::Texture>()}
    ,{"mine.png",std::make_shared<sf::Texture>()}
    ,{"number_1.png",std::make_shared<sf::Texture>()}
    ,{"number_2.png",std::make_shared<sf::Texture>()}
    ,{"number_3.png",std::make_shared<sf::Texture>()}
    ,{"number_4.png",std::make_shared<sf::Texture>()}
    ,{"number_5.png",std::make_shared<sf::Texture>()}
    ,{"number_6.png",std::make_shared<sf::Texture>()}
    ,{"number_7.png",std::make_shared<sf::Texture>()}
    ,{"number_8.png",std::make_shared<sf::Texture>()}
    ,{"pause.png",std::make_shared<sf::Texture>()}
    ,{"play.png",std::make_shared<sf::Texture>()}
    ,{"tile_revealed.png",std::make_shared<sf::Texture>()}
    ,{"tile_hidden.png",std::make_shared<sf::Texture>()}};
    for (auto iter = txMap.begin(); iter != txMap.end(); iter++){
        if (!iter->second->loadFromFile("files/images/" + iter->first)) std::cerr << iter->first << " not loaded" << std::endl;
    }
    float commonYPos = 32 * (rowNum + 0.5);
    float commonXPos = colNum * 32;
    sf::Vector2f debugPos(commonXPos - 304, commonYPos);
    sf::Vector2f facePos((commonXPos / 2) - 32, commonYPos);
    sf::Vector2f pausePlayPos(commonXPos - 240, commonYPos);
    sf::Vector2f leaderboardPos(commonXPos - 176, commonYPos);
    sf::Vector2f counterPos(33, commonYPos + 16);
    sf::Vector2f negPos(12, commonYPos + 16);
    sf::Sprite debug, tcounter,scounter,fcounter, neg, fsec, ssec, fmin, smin, face ,lb ,pausePlay, tileHidden, tileRevealed, symbols, flag;
    
    
    spriteMaker(neg, *txMap["digits.png"], sf::Vector2f(12, commonYPos + 16));
    neg.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));
    spriteMaker(tcounter, *txMap["digits.png"], sf::Vector2f(33, commonYPos + 16));
    spriteMaker(scounter, *txMap["digits.png"], sf::Vector2f(33 + 21, commonYPos + 16));
    spriteMaker(fcounter, *txMap["digits.png"], sf::Vector2f(33 + 42, commonYPos + 16));
    spriteMaker(fmin, *txMap["digits.png"], sf::Vector2f((commonXPos - 97), commonYPos + 16));
    spriteMaker(smin, *txMap["digits.png"], sf::Vector2f((commonXPos - 97 + 21), commonYPos + 16));
    spriteMaker(fsec, *txMap["digits.png"], sf::Vector2f((commonXPos - 54), commonYPos + 16));
    spriteMaker(ssec, *txMap["digits.png"], sf::Vector2f((commonXPos - 54 + 21), commonYPos + 16));
    spriteMaker(flag, *txMap["flag.png"]);
    spriteMaker(debug, *txMap["debug.png"], debugPos);
    spriteMaker(face,  *txMap["face_happy.png"], facePos);
    spriteMaker(lb, *txMap["leaderboard.png"],  leaderboardPos);
    spriteMaker(pausePlay, *txMap["play.png"], pausePlayPos);
    spriteMaker(tileHidden, *txMap["tile_hidden.png"]);
    spriteMaker(tileRevealed, *txMap["tile_revealed.png"]);
    


    std::vector<sf::Sprite> tileSprites(rowNum * colNum);
     for (int i = 0; i < rowNum * colNum; ++i) {
        tileSprites[i].setTexture(*txMap[gameB.tileArr[i].path]);
        tileSprites[i].setPosition(gameB.tileArr[i].x, gameB.tileArr[i].y);
    }

    
    std::multimap<int, std::string> lbMap;
    std::ifstream lbExtract("files/leaderboard.txt");
    if(lbExtract.is_open()){
        std::string line;
        while(std::getline(lbExtract, line)){
            std::string name = line.substr(6);
            int seconds = (std::stoi(line.substr(0,2)) * 60) + std::stoi(line.substr(3,2));
            lbMap.insert({seconds,name});
        }
        lbExtract.close();
    }else{std::cerr << "leaderboard.txt did not open" << std::endl;}
    int seconds = 0;
    int minutes = 0;
    int tensSecs = 0;
    int secs = 0;
    int mins = 0;
    int tensMins = 0;
    bool isNeg = false;
    auto haltedTime = std::chrono::high_resolution_clock::duration::zero();
    std::chrono::high_resolution_clock::time_point end;
    while (gameWindow.isOpen() && nameStr != ""){
        gameB.mineCount < 0 ? isNeg = true: isNeg = false;
        tcounter.setTextureRect(sf::IntRect(21 * (gameB.mineCount >= 0 ? (gameB.mineCount >= 100 ?  gameB.mineCount / 100:  0) : (gameB.mineCount * -1 >= 100 ?  (-1 * gameB.mineCount) / 100:  0)), 0, 21, 32));
        scounter.setTextureRect(sf::IntRect(21 * (gameB.mineCount >= 0 ? (gameB.mineCount >= 100 ?  (gameB.mineCount % 10) / 10:  gameB.mineCount / 10) : (gameB.mineCount *-1 >= 100 ?  ((-1 * gameB.mineCount) % 10) / 10:  (-1 * gameB.mineCount) / 10) ), 0, 21, 32));
        fcounter.setTextureRect(sf::IntRect(21 * ( gameB.mineCount >= 0 ? (gameB.mineCount >= 100 ?  (gameB.mineCount % 10) % 10:  gameB.mineCount % 10) : (gameB.mineCount *-1 >= 100 ?  ((-1 * gameB.mineCount) % 10) % 10:  (-1 * gameB.mineCount) % 10)), 0, 21, 32));
        
        auto currTime = std::chrono::high_resolution_clock::now();
        int arrIdx = 0;
        sf::Event e;
        if(gameB.lbPressed){
            if (!gameB.gameWon && !gameB.gameOver && !gameB.paused){
                haltedTime += currTime - beginning;
                gameB.paused = true;
                runLeaderboard(gameB.colNum, gameB.rowNum, gameB.gameWon);
                currTime = std::chrono::high_resolution_clock::now();
                gameB.paused = false;
                beginning = currTime;
            }else{
                runLeaderboard(gameB.colNum, gameB.rowNum, gameB.gameWon);
            }
        }
        gameB.lbPressed = false;
        while (gameWindow.pollEvent(e)){
            switch(e.type){
                case sf::Event::Closed:
                    gameWindow.close();
                    break;
                case sf::Event::MouseButtonPressed: 
                    sf::Vector2i mPos = sf::Mouse::getPosition(gameWindow);
                    if (mPos.y <= rowNum * 32 && !gameB.paused){
                        int index = (mPos.x/32) + (colNum * (mPos.y/32));
                        if(e.mouseButton.button == (sf::Mouse::Left)){
                            if (!gameB.tileArr[index].isFlag) gameB.exposeTiles(gameB.tileArr + index);
                            if(gameB.allTilesRev == (gameB.rowNum * gameB.colNum) - gameB.mineNum){
                                face.setTexture(*txMap["face_win.png"]);
                                insertTime(lbMap, seconds, nameStr);
                                haltedTime += currTime - beginning;
                                gameB.paused = true;
                                gameB.gameWon = true;
                            }
                            if (gameB.tileArr[index].isMine && !gameB.tileArr[index].isFlag){
                                gameB.paused = true;
                                gameB.gameOver = true;
                                haltedTime += currTime - beginning;
                                face.setTexture(*txMap["face_lose.png"]);
                            }
                        }else if(e.mouseButton.button == sf::Mouse::Right){
                            if (gameB.tileArr[index].isFlag){
                                gameB.tileArr[index].isFlag = false;
                                if(gameB.tileArr[index].isMine) --gameB.yesFlag;
                                ++gameB.mineCount;
                            }else if(!gameB.tileArr[index].isFlag && !gameB.tileArr[index].clickedOn){
                                gameB.tileArr[index].isFlag = true;
                                --gameB.mineCount;
                                if(gameB.tileArr[index].isMine){
                                    ++gameB.yesFlag;
                                    if(gameB.yesFlag == mineNum && gameB.allTilesRev == (gameB.rowNum * gameB.colNum) - gameB.mineNum){
                                        face.setTexture(*txMap["face_win.png"]);
                                        insertTime(lbMap, seconds, nameStr);
                                        haltedTime += currTime - beginning;
                                        gameB.paused = true;
                                        gameB.gameWon = true;
                                        //runLeaderboard(colNum, rowNum);
                                    }
                                }
                            }
                            fcounter.setTextureRect(sf::IntRect(21 * (gameB.mineCount / 10), 0, 21, 32));
                            scounter.setTextureRect(sf::IntRect(21 * (gameB.mineCount % 10), 0, 21, 32));
                        }
                    }
                    if (within(mPos, facePos, face)){
                        beginning = std::chrono::high_resolution_clock::now();
                        haltedTime = std::chrono::high_resolution_clock::duration::zero();
                        gameB.reset();
                        if (gameB.paused){
                            pausePlay.setTexture(*txMap["play.png"]);
                        }
                        face.setTexture(*txMap["face_happy.png"]);
                        gameB.boardMaker();
                        tileSprites.clear();
                        for (int i = 0; i < rowNum * colNum; ++i) {
                            tileSprites[i].setTexture(*txMap[gameB.tileArr[i].path]);
                            tileSprites[i].setPosition(gameB.tileArr[i].x, gameB.tileArr[i].y);
                        }
                    }
                    if (within(mPos, debugPos, debug) && !gameB.paused){
                        gameB.debugMode ? gameB.debugMode = false: gameB.debugMode = true;
                    }
                    if (within(mPos, pausePlayPos, pausePlay) && !gameB.gameWon && !gameB.gameOver){
                        if(!gameB.paused){
                            haltedTime += currTime - beginning;
                            gameB.paused = true;
                            pausePlay.setTexture(*txMap["pause.png"]);
                        }else{
                            beginning = currTime;
                            gameB.paused = false;
                            pausePlay.setTexture(*txMap["play.png"]);
                        }
                    }
                    if(within(mPos, leaderboardPos, lb)){
                        if (!gameB.lbPressed) gameB.lbPressed = true;
                    }
                    break;
                }
            }
        seconds = std::chrono::duration_cast<std::chrono::seconds>(gameB.paused? haltedTime : haltedTime + (currTime - beginning)).count();
        minutes = std::chrono::duration_cast<std::chrono::minutes>(gameB.paused? haltedTime : haltedTime + (currTime - beginning)).count();
        tensSecs = (seconds % 60) % 10;
        secs = (seconds % 60) / 10;
        mins = minutes / 10;
        tensMins = minutes % 10;
        fmin.setTextureRect(sf::IntRect(21 * mins, 0, 21, 32));
        smin.setTextureRect(sf::IntRect(21 * tensMins, 0, 21, 32));
        fsec.setTextureRect(sf::IntRect(21 * secs, 0, 21, 32));
        ssec.setTextureRect(sf::IntRect(21 * tensSecs, 0, 21, 32));
        gameWindow.clear(sf::Color::White);
        for (int i = 0; i < rowNum * colNum ; i++){
            tileRevealed.setPosition(gameB.tileArr[i].x, gameB.tileArr[i].y);
            gameWindow.draw(tileRevealed);
            if(!gameB.lbPressed){
                if((!gameB.tileArr[i].clickedOn || gameB.tileArr[i].isFlag)){
                    tileHidden.setPosition(gameB.tileArr[i].x, gameB.tileArr[i].y);
                    gameWindow.draw(tileHidden);
                }
                if((gameB.tileArr[i].clickedOn && !gameB.tileArr[i].isFlag && !gameB.debugMode)){
                    gameWindow.draw(tileSprites[i]);
                }
                if(gameB.debugMode || gameB.gameOver){ //&& gameB.tileArr[i].isMine){
                    if (!gameB.tileArr[i].isFlag) 
                        gameWindow.draw(tileRevealed);
                    gameWindow.draw(tileSprites[i]);
                }
                if((!gameB.debugMode && gameB.tileArr[i].isFlag) || (!gameB.tileArr[i].clickedOn && gameB.gameWon)){
                    flag.setPosition(gameB.tileArr[i].x, gameB.tileArr[i].y);
                    gameWindow.draw(flag);
                }
            }

        }
        gameWindow.draw(lb);
        gameWindow.draw(debug);
        gameWindow.draw(face);
        gameWindow.draw(pausePlay);
        gameWindow.draw(fmin);
        gameWindow.draw(smin);
        gameWindow.draw(ssec);
        gameWindow.draw(fsec);
        gameWindow.draw(fcounter);
        gameWindow.draw(scounter);
        gameWindow.draw(tcounter);
        if(isNeg) gameWindow.draw(neg);
        gameWindow.display();
    }
    return 0;
}
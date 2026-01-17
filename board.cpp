#include "window.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include<map>
#include<string>
#include <random>

Board::Board(){
    colNum = rowNum = mineNum = yesFlag = mineCount = allTilesRev =  0;
    gameOver = debugMode = paused = gameWon = lbPressed = false;
    tileArr = new Tile[rowNum * colNum];
}
Board::Board(int colNumb, int rowNumb, int mineNumb): colNum(colNumb), rowNum(rowNumb), mineNum(mineNumb), mineCount(mineNumb){
    gameOver = debugMode = paused = gameWon = lbPressed = false;
    yesFlag = allTilesRev = 0;
    tileArr = new Tile[rowNum * colNum];
}
// Board::Board(const Board& other){
//     tileArr = new Tile[other.colNum * other.rowNum];
//     for (int i = 0; i < other.rowNum * other.colNum; i++){
//         tileArr[i] = other.tileArr[i];
//     }
//     colNum = other.colNum;
//     rowNum = other.rowNum;
//     mineNum = other.mineNum;
//     yesFlag = other.yesFlag;
//     mineCount = other.mineCount;
//     gameOver = other.gameOver;
//     debugMode = other.debugMode;
//     paused = other.paused;
//     gameWon = other.gameWon;
//     lbPressed = other.lbPressed;
//     allTilesRev =  other.allTilesRev;
// }
// Board& Board::operator=(const Board& other){
//     if (this == &other)return *this;
//     Tile* newTileArr = new Tile[other.colNum * other.rowNum];
//     delete[] tileArr;
//     tileArr = newTileArr;
//     for (int i = 0; i < other.rowNum * other.colNum; i++){
//         tileArr[i] = other.tileArr[i];
//     }
//     colNum = other.colNum;
//     rowNum = other.rowNum;
//     mineNum = other.mineNum;
//     yesFlag = other.yesFlag;
//     mineCount = other.mineCount;
//     gameOver = other.gameOver;
//     debugMode = other.debugMode;
//     paused = other.paused;
//     gameWon = other.gameWon;
//     lbPressed = other.lbPressed;
//     allTilesRev =  other.allTilesRev;
//     return *this;
// }
// Board::Board(Board&& other) noexcept{
//     tileArr = other.tileArr;
//     colNum = other.colNum;
//     rowNum = other.rowNum;
//     mineNum = other.mineNum;
//     yesFlag = other.yesFlag;
//     mineCount = other.mineCount;
//     gameOver = other.gameOver;
//     debugMode = other.debugMode;
//     paused = other.paused;
//     gameWon = other.gameWon;
//     lbPressed = other.lbPressed;
//     allTilesRev = other.allTilesRev;
//     other.colNum = other.rowNum = other.mineNum = other.mineCount = other.yesFlag = 0;
//     other.gameOver = other.debugMode = other.paused = false;
//     other.tileArr = nullptr;

// }
// Board& Board::operator=(Board&& other) noexcept{
//     if(this == &other) return *this;
//     tileArr = other.tileArr;
//     colNum = other.colNum;
//     rowNum = other.rowNum;
//     mineNum = other.mineNum;
//     yesFlag = other.yesFlag;
//     mineCount = other.mineCount;
//     gameOver = other.gameOver;
//     debugMode = other.debugMode;
//     paused = other.paused;
//     gameWon = other.gameWon;
//     lbPressed = other.lbPressed;
//     allTilesRev = other.allTilesRev;
//     other.colNum = other.rowNum = other.mineNum = other.mineCount = other.yesFlag = 0;
//     other.gameOver = other.debugMode = other.paused = false;
//     other.tileArr = nullptr;
//     return *this;
// }
void Board::pushV(int& idx){
    Tile * curr = tileArr + idx;
    //if(tile.y - 32 < 0 || tile.y + 32 > rowNum * 32 || tile.x - 32 < 0 || tile.x + 32 > colNum * 32){
        if(tileArr[idx].x + 32 < colNum * 32) tileArr[idx].neighbors.push_back(curr + 1);
        if(tileArr[idx].x - 32 >= 0) tileArr[idx].neighbors.push_back(curr - 1);
    if (tileArr[idx].y - 32 >= 0){
        tileArr[idx].neighbors.push_back(curr - 25);
        if(tileArr[idx].x - 32 >= 0) tileArr[idx].neighbors.push_back(curr - 26);
        if(tileArr[idx].x + 32 < colNum * 32) tileArr[idx].neighbors.push_back(curr - 24);
    }
    if(tileArr[idx].y + 32 < rowNum * 32){
        tileArr[idx].neighbors.push_back(curr + 25);
        if(tileArr[idx].x + 32 < colNum * 32) tileArr[idx].neighbors.push_back(curr + 26);
        if(tileArr[idx].x - 32 >= 0) tileArr[idx].neighbors.push_back(curr + 24);
    }
    
}
void Board::neighborSetter(){
    int arrIdx = 0;
    while(arrIdx < rowNum * colNum){
        if (!tileArr[arrIdx].isMine){
            int count = 0;
            for (std::size_t j = 0; j < tileArr[arrIdx].neighbors.size(); j++){
                if (tileArr[arrIdx].neighbors[j]->isMine){
                    count++;
                }
            }
            if(count > 0){
                tileArr[arrIdx].path = "number_" + std::to_string(count) + ".png";
            }else tileArr[arrIdx].isEmpty = true;
        }
        arrIdx++;
    }
}
void Board::boardMaker(){	
	std::mt19937 rng(time(nullptr));
	std::uniform_int_distribution<int> dist(0, (rowNum * colNum) - 1);
	int mineCounter = 0;
	int arrIdx = 0;
    for (int i = 0; i < rowNum * colNum; i++){
        //std::cout << "It's here" << 51 << std::endl;
        tileArr[i].reset();
        //std::cout << "It's here" << 53 << std::endl;
        if (!tileArr[i].neighbors.empty()){
            tileArr[i].neighbors.clear();
            //tileArr[i].neighbors.reserve(8);
        }
        //std::cout << "It's here" << std::endl;
    }
	for (float i = 0; i < rowNum; i++){
		for (float j = 0; j < colNum; j++){
			tileArr[arrIdx].x = j*32;
			tileArr[arrIdx].y = i*32;
			pushV(arrIdx);
			int rand = dist(rng);
			if (!tileArr[rand].isMine && mineCounter < mineNum){
                //std::cout << rand << std::endl;
				tileArr[rand].isMine = true;
				tileArr[rand].path = "mine.png";
				mineCounter++;
			}
            arrIdx++;
		}
	}
	neighborSetter();
}

void Board::exposeTiles(Tile* tile){
    //implement flag removal
    if (tile->isFlag){
        return;
    }
    tile->clickedOn = true;
    ++allTilesRev;
    if(tile->isEmpty){
        for (std::size_t i = 0; i < tile->neighbors.size(); i++){
            if (!tile->neighbors[i]->clickedOn) exposeTiles(tile->neighbors[i]); 
        }
    }else return;
}

void Board::reset(){
    yesFlag = allTilesRev = 0;
    mineCount = mineNum;
    gameOver = debugMode = paused = gameWon = lbPressed = false;
}

Board::~Board(){
    colNum = rowNum = mineNum = yesFlag = mineCount = allTilesRev =  0;
    gameOver = debugMode = paused = gameWon = lbPressed = false;
    delete[] tileArr;
    tileArr = nullptr;
}
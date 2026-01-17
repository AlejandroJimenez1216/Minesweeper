#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <vector>
#include "window.hpp"
int simpleRank = 0;
void runLeaderboard(int& colNum, int& rowNum, bool& gameWon){
    sf::RenderWindow leaderboard(sf::VideoMode(colNum * 16, rowNum * 16 + 50), "Leaderboard", sf::Style::Titlebar | sf::Style::Close);
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")){
        std::cerr << "Could not load file" << "\n";
    }
    std::string entireLB;
    std::ifstream lbExtract("files/leaderboard.txt");
    if(lbExtract.is_open()){
        std::string line;
        int count = 1;
        while(std::getline(lbExtract, line)){
            entireLB += std::to_string(count) + ".\t" + line.substr(0,5) + "\t" + line.substr(6);
            if (count == simpleRank && gameWon){entireLB += "*";}
            entireLB +=  "\n\n";
            ++count;
        }
    }else{std::cerr << "lbSave did not open" << std::endl;}
    sf::Text title("LEADERBOARD", font, 20);
    sf::Text lb (entireLB, font, 18);
    sf::Vector2u wSize = leaderboard.getSize();
    float wMidX = wSize.x / 2;
    float wMidY = wSize.y / 2;
    centerTOrigin(title, wMidX, wMidY - 120);
    centerTOrigin(lb, wMidX, wMidY + 20);
    lb.setFillColor(sf::Color::White);
    lb.setStyle(sf::Text::Bold);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    while (leaderboard.isOpen()){
        sf::Event event;
        while (leaderboard.pollEvent(event)){
            //leaderboard.setPosition(sf::Vector2i(10,50));
            //leaderboard.setSize(sf::Vector2u(600,300));
            //leaderboard.setTitle("Super Cool leaderboard");
            switch(event.type){
                case sf::Event::Closed:
                leaderboard.close();
                    break;
                
            }
        }
        leaderboard.clear(sf::Color::Blue);
        leaderboard.draw(title);
        leaderboard.draw(lb);
        leaderboard.display();
    }
}
void lbwrite(std::multimap<int, std::string>& mapp, std::multimap<int, std::string>::iterator currIter){
	std::ofstream lbWrite("files/leaderboard.txt");
    
    int count = 1;
    int currRank = 0;
	if(!lbWrite.is_open()) std::cerr << "Could not open" << std::endl;
	else{
		for (auto iter = mapp.begin(); iter != mapp.end(); ++iter){
			int minute = iter->first / 60;
			int second = iter->first % 60;
			std::string minString = minute < 10 ? "0" + std::to_string(minute): std::to_string(minute);
			std::string secString = second < 10 ? "0" + std::to_string(second): std::to_string(second);
			//lbWrite << std::to_string(count) << "." << '\t' << minString << ":" << secString << '\t' << iter->second;
            lbWrite << minString << ':' << secString << ',' << iter->second << '\n';
            if (iter == currIter) currRank = count;
            // else lbWrite << '\n';
            count++;
		}
		lbWrite.close();
	}
    simpleRank = currRank;
    // std::ofstream lbSave("lbSave.txt");
    // if(!lbSave.is_open()) std::cerr << "Could not open" << std::endl;
	// else{
	// 	for (auto iter = mapp.begin(); iter != mapp.end(); ++iter){
	// 		lbSave << iter->first << " " << iter->second << '\n';
	// 	}
	// 	lbSave.close();
	// }
}
void insertTime(std::multimap<int,std::string>& mapp, int& num, std::string& str){
	int lastSecond = 0;
    auto iterate = mapp.begin();
    if(mapp.size() < 5){
        mapp.insert({num,str});
        for (auto iter = mapp.begin(); iter != mapp.end(); ++iter){
            if (iter->first == num && iter->second == str) iterate = iter;
        }
        lbwrite(mapp, iterate);
    }else{
        auto iter = std::prev(mapp.end());
        lastSecond = iter->first;
        if (lastSecond > num && mapp.size() == 5){
            mapp.erase(iter);
            mapp.insert({num, str});
            
            for (auto iter = mapp.begin(); iter != mapp.end(); ++iter){
                if (iter->first == num && iter->second == str) iterate = iter;
            }
            lbwrite(mapp, iterate);
        }
    }
}
#pragma once
#include <iostream>
#include <vector>

class ThreeInRowGame
{
public:
    inline static ThreeInRowGame& getInstance( )
    {
        static ThreeInRowGame instance;
        return instance;
    }

    void startGame( );

private:
    ThreeInRowGame( );
    void paintWwindow( );
    void runCycle( );
    int parseCommand( );
    void randomBuf(  );
    void changeObjects( );
    // осталось прописать првоерку на "3 в ряд" при создании окна и при каждой итерации цикла

private:
    std::vector<std::string> screenBuf;
    std::string command;
    bool gameIsRunning;

    int rowNum;
    int colNum;
    char moveDirection;

    unsigned int score;

};

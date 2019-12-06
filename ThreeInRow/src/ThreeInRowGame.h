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
    int checkSide( char side );
    bool checkThreeInRow( );
    void deleteRow( char side, int num1, int num2 = 0 );

private:
    std::vector<std::string> screenBuf;
    std::string command;

    int fieldSize;

    bool gameIsRunning;

    int prevRowNum;
    int prevColNum;
    int rowNum;
    int colNum;
    char moveDirection;

    unsigned int score;

};

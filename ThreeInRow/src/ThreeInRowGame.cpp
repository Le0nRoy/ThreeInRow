#include "ThreeInRowGame.h"
#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#define PAUSE Sleep( 1000 );
std::string clearConsole( "cls" );
std::string clickNewKey;
#else
#include <unistd.h>
#define PAUSE usleep( 1000000 );
std::string clearConsole( "clear" );
std::string clickNewKey( "Для продолжения нажмите любую клавишу!" );
#endif

ThreeInRowGame::ThreeInRowGame( ) : 
    gameIsRunning( false ),
    rowNum( 0 ),
    colNum( 0 ),
    moveDirection( 0 ),
    score( 0 )
{
    screenBuf.resize( 10 );
    /*std::for_each( screenBuf.begin( ), screenBuf.end( ), []( std::string val )
        {
            val.resize( 10 );
        } );*/
    screenBuf.at( 0 ) = "AFIAAAAAAA";
    screenBuf.at( 1 ) = "DBGAAAAAAA";
    screenBuf.at( 2 ) = "ECHAAAAAAA";
    screenBuf.at( 3 ) = "AAAAAAAAAA";
    screenBuf.at( 4 ) = "AAAAAAAAAA";
    screenBuf.at( 5 ) = "AAAAAAAAAA";
    screenBuf.at( 6 ) = "AAAAAAAAAA";
    screenBuf.at( 7 ) = "AAAAAAAAAA";
    screenBuf.at( 8 ) = "AAAAAAAAAA";
    screenBuf.at( 9 ) = "AAAAAAAAAA";
    randomBuf( );
}

void ThreeInRowGame::startGame( )
{
    gameIsRunning = true;
    runCycle( );
}

void ThreeInRowGame::paintWwindow( )
{
    system( clearConsole.c_str( ) );
    std::cout << " Приветствую! Вы играете в игру 3 в ряд." << std::endl;
    std::cout << " Правила игры. Вам нужно совместить 3 одинаковых буквы в один ряд." << std::endl;
    std::cout << "Данный блок удалится. Вам необходимо соединять таким образом блоки" << std::endl;
    std::cout << "до тех пор, пока не будет очищено все поле." << std::endl;
    std::cout << " Правила игры.";
    std::cout << "Для перемещения элемента необходимо ввести команду типа <MRCD>,";
    std::cout << "где R - номер ряда, C - номер столбца, D - направление перемещения.";
    std::cout << "Для направления перемещения указывайте одну из следующих букв:";
    std::cout << "l - влево, r - вправо, u - вверх, d - вниз." << std::endl;
    std::cout << "Удачи!!!" << std::endl;
    std::cout << std::endl;

    std::cout << "                        Your score: " + std::to_string( score ) + "\n";
    std::cout << "    0 1 2 3 4 5 6 7 8 9" << std::endl;
    std::cout << "   --------------------" << std::endl;
    size_t pos = 0;
    std::for_each( screenBuf.begin( ), screenBuf.end( ), [ &pos ]( std::string val )
        {
            std::cout << std::to_string( pos++ ) << " | " ;
            std::for_each( val.begin( ), val.end( ), []( char c )
                {
                    std::cout << c << " ";
                } );
            std::cout << std::endl;
        } );
}

int ThreeInRowGame::parseCommand( )
{
    if ( command == "stop" )
    {
        gameIsRunning = false;
        return 1;
    }
    if ( command.size( ) != 4 )
    {
        return -1;
    }

    const char move = 'm';
    if ( ( command.at( 0 ) != move ) )
    {
        return -1;
    }

    const char left = 'l';
    const char right = 'r';
    const char up = 'u';
    const char down = 'd';

    char row = command.at( 1 );
    char col = command.at( 2 );
    char direction = command.at( 3 );

    if (
        ( row >= '0' && row <= '9' ) &&
        ( col >= '0' && col <= '9' ) &&
        ( direction == left || direction == right || direction == down || direction == up )
        )
    {
        rowNum = row - '0';
        colNum = col - '0';
        moveDirection = direction;
        if ( rowNum == 0 && moveDirection == 'u' ||
            rowNum == 9 && moveDirection == 'd' ||
            colNum == 0 && moveDirection == 'l' ||
            colNum == 9 && moveDirection == 'r' )
        {
            return -1;
        }
        return 0;
    }
    return -1;
}

void ThreeInRowGame::changeObjects( )
{
    char secRow = 0;
    char secCol = 0;
    if ( moveDirection == 'u' )
    {
        secRow = rowNum - 1;
    }
    else if ( moveDirection == 'd' )
    {
        secRow = rowNum + 1;
    }
    else
    {
        secRow = rowNum;
    }

    if ( moveDirection == 'l' )
    {
        secCol = colNum - 1;
    }
    else if ( moveDirection == 'r' )
    {
        secCol = colNum + 1;
    }
    else
    {
        secCol = colNum;
    }
    char &firstObject = screenBuf[ rowNum ][ colNum ];
    char &secondObject = screenBuf[ secRow ][ secCol ];

    char buf = firstObject;
    firstObject = secondObject;
    secondObject = buf;
}

void ThreeInRowGame::randomBuf(  )
{
    srand( time( NULL ) );
    std::for_each( screenBuf.begin( ), screenBuf.end( ), []( std::string &val )
        {
            std::for_each( val.begin( ), val.end( ), []( char &c )
                {
                    char addable = static_cast<char>( rand( ) % 6 );
                    c = 'A' + addable;
                } );
        } );
}

void ThreeInRowGame::runCycle( )
{
    while ( gameIsRunning )
    {
        paintWwindow( );
        std::cin >> command;
        int parseResult = parseCommand( );
        if ( parseResult == -1 )
        {
            std::cout << "Неверная команда! Попробуйте снова!" << std::endl;
            std::cout << clickNewKey << std::endl;
            PAUSE;
            continue;
        }
        else if ( parseResult == 1 )
        {
            std::cout << "До новых встреч! " << clickNewKey << std::endl;
            system( "pause" );
            continue;
        }

        changeObjects( );
        paintWwindow( );
        PAUSE;

        system( clearConsole.c_str( ) );
    }
}

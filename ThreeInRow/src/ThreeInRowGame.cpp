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
    fieldSize( 10 ),
    gameIsRunning( false ),
    rowNum( 0 ),
    colNum( 0 ),
    moveDirection( 0 ),
    score( 0 )
{
    screenBuf.resize( fieldSize );
    for ( int i = 0; i < fieldSize; i++ )
    {
        screenBuf.at( i ).assign( fieldSize, ' ' );
    }

    randomBuf( );
}

void ThreeInRowGame::randomBuf( )
{
    srand( time( NULL ) );
    std::for_each( screenBuf.begin( ), screenBuf.end( ), []( std::string &val )
        {
            std::for_each( val.begin( ), val.end( ), []( char &c )
                {
                    if ( c == ' ' )
                    {
                        char addable = static_cast<char>( rand( ) % 6 );
                        c = 'A' + addable;
                    }
                } );
        } );
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
    std::cout << "до тех пор, пока вы не наберете 100 очков." << std::endl;
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
        prevRowNum = row - '0';
        prevColNum = col - '0';
        if ( prevRowNum == 0 && moveDirection == 'u' ||
            prevRowNum == 9 && moveDirection == 'd' ||
            prevColNum == 0 && moveDirection == 'l' ||
            prevColNum == 9 && moveDirection == 'r' )
        {
            return -1;
        }
        moveDirection = direction;

        switch ( moveDirection )
        {
        case 'l':
        {
            colNum = prevColNum - 1;
            rowNum = prevRowNum;
            break;
        }
        case 'r':
        {
            colNum = prevColNum + 1;
            rowNum = prevRowNum;
            break;
        }
        case 'u':
        {
            rowNum = prevRowNum - 1;
            colNum = prevColNum;
            break;
        }
        case 'd':
        {
            rowNum = prevRowNum + 1;
            colNum = prevColNum;
            break;
        }
        default:
            break;
        }
        return 0;
    }
    return -1;
}

void ThreeInRowGame::changeObjects( )
{
    char &firstObject = screenBuf[ prevRowNum ][ prevColNum ];
    char &secondObject = screenBuf[ rowNum ][ colNum ];

    char buf = firstObject;
    firstObject = secondObject;
    secondObject = buf;
}

bool ThreeInRowGame::checkThreeInRow( )
{
    switch ( moveDirection )
    {
    case 'l' :
    {
        int up = checkSide( 'u' );
        int down = checkSide( 'd' );
        int left = checkSide( 'l' );
        if ( up + down > 2 || ( up + down == 2 && left < 2 ) )
        {
            deleteRow( 'u', up, down );
        }
        else if ( left == 2 )
        {
            deleteRow( 'l', left, 0 );
        }
        else
        {
            return false;
        }
        return true;
        break;
    }
    case 'r':
    {
        int up = checkSide( 'u' );
        int down = checkSide( 'd' );
        int right = checkSide( 'r' );
        if ( up + down > 2 || ( up + down == 2 && right < 2 ) )
        {
            deleteRow( 'u', up, down );
        }
        else if ( right == 2 )
        {
            deleteRow( 'r', right, 0 );
        }
        else
        {
            return false;
        }
        return true;
        break;
    }
    case 'u':
    {
        int up = checkSide( 'u' );
        int right = checkSide( 'r' );
        int left = checkSide( 'l' );
        if ( right + left > 2 || ( right + left == 2 && up < 2 ) )
        {
            deleteRow( 'r', right, left );
        }
        else if ( up == 2 )
        {
            deleteRow( 'u', up, 0 );
        }
        else
        {
            return false;
        }
        return true;
        break;
    }
    case 'd':
    {
        int right = checkSide( 'r' );
        int down = checkSide( 'd' );
        int left = checkSide( 'l' );
        if ( right + left > 2 || ( right + left == 2 && down < 2 ) )
        {
            deleteRow( 'r', right, left );
        }
        if ( down == 2 )
        {
            deleteRow( 'd', down, 0 );
        }
        else
        {
            return false;
        }
        return true;
        break;
    }
    default:
        break;
    }
    return false;
}

int ThreeInRowGame::checkSide( char side )
{
    char curChar = screenBuf[ rowNum ][ colNum ];
    int counter = 0;
    switch ( side )
    {
    case 'l':
        if ( colNum != 0 && curChar == screenBuf[ rowNum ][ colNum - 1 ] )
        {
            counter = 1;
            if ( colNum != 1 && curChar == screenBuf[ rowNum ][ colNum - 2 ] )
            {
                counter = 2;
            }
        }
        return counter;
        break;
    case 'r':
        if ( colNum != fieldSize - 1 && curChar == screenBuf[ rowNum ][ colNum + 1 ] )
        {
            counter = 1;
            if ( colNum != fieldSize - 2 && curChar == screenBuf[ rowNum ][ colNum + 2 ] )
            {
                counter = 2;
            }
        }
        return counter;
        break;
    case 'u':
        if ( rowNum != 0 && curChar == screenBuf[ rowNum - 1 ][ colNum ] )
        {
            counter = 1;
            if ( colNum != 1 && curChar == screenBuf[ rowNum - 2 ][ colNum ] )
            {
                counter = 2;
            }
        }
        return counter;
        break;
    case 'd':
        if ( rowNum != fieldSize - 1 && curChar == screenBuf[ rowNum + 1 ][ colNum ] )
        {
            counter = 1;
            if ( rowNum != fieldSize - 2 && curChar == screenBuf[ rowNum + 2 ][ colNum ] )
            {
                counter = 2;
            }
        }
        return counter;
        break;
    default:
        return -1;
        break;
    }
    return 0;
}

void ThreeInRowGame::deleteRow( char side, int num1, int num2 )
{
    switch ( side )
    {
    case 'l':
    {
        screenBuf.at( rowNum ).at( colNum ) = ' ';
        for ( int i = 0; i <= num1; i++ )
        {
            screenBuf.at( rowNum ).at( colNum - i ) = ' ';
        }
        for ( int i = 0; i <= num2; i++ )
        {
            screenBuf.at( rowNum ).at( colNum + i ) = ' ';
        }
        break;
    }
    case 'r':
    {
        screenBuf.at( rowNum ).at( colNum ) = ' ';
        for ( int i = 0; i <= num1; i++ )
        {
            screenBuf.at( rowNum ).at( colNum + i ) = ' ';
        }
        for ( int i = 0; i <= num2; i++ )
        {
            screenBuf.at( rowNum ).at( colNum - i ) = ' ';
        }
        break;
    }
    case 'u':
    {
        screenBuf.at( rowNum ).at( colNum ) = ' ';
        for ( int i = 0; i <= num1; i++ )
        {
            screenBuf.at( rowNum - i ).at( colNum ) = ' ';
        }
        for ( int i = 0; i <= num2; i++ )
        {
            screenBuf.at( rowNum + i ).at( colNum ) = ' ';
        }
        break;
    }
    case 'd':
    {
        screenBuf.at( rowNum ).at( colNum ) = ' ';
        for ( int i = 0; i <= num1; i++ )
        {
            screenBuf.at( rowNum + i ).at( colNum ) = ' ';
        }
        for ( int i = 0; i <= num2; i++ )
        {
            screenBuf.at( rowNum - i ).at( colNum ) = ' ';
        }
        break;
    }
    }

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
        if ( checkThreeInRow( ) )
        {
            //randomBuf( );
        }

        system( clearConsole.c_str( ) );
    }
}

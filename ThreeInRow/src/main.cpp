/*
* 
*/
#include "ThreeInRowGame.h"

int main( int argc, char **argv )
{
    setlocale( LC_ALL, "Russian" );
    ThreeInRowGame game = ThreeInRowGame::getInstance( );
    game.startGame( );

    return 0;
}

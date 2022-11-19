#include <SFML/Graphics.hpp>
#include <sfml/audio.hpp>
#include <vector>
#include <list>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <cstring>
#include <random>
#include <chrono>

#include "various.h"
#include "entity.h"
#include "game.h"

int main()
{
    Rnd rnd;

    //the game class
    Game game;

    //create the main window
    game.init(465,400,400,200,"Henway2");

    sf::Clock clock;
    const sf::Time timePerFrame = sf::seconds(1.f/60.f); //60fps
    sf::Time elapsed = sf::Time::Zero;

    while(game.app.isOpen())
    {
        elapsed += clock.restart();

        game.input();

        while( elapsed >= timePerFrame )
        {
            game.update(elapsed);
            elapsed -= timePerFrame;
        }

        game.draw();
    }

    return EXIT_SUCCESS;
}

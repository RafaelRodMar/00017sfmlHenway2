class Game{
public:
    int screenwidth = -1;
    int screenheight = -1;
    sf::RenderWindow app;

    enum game_states {MENU, GAME, END_GAME};
    int state = MENU;
    int lives = -1, score = -1;

    //keyboard handling
    // The keyboard's state in the current- and the previous frame
    bool CurrentKeyState[sf::Keyboard::KeyCount];
    bool PreviousKeyState[sf::Keyboard::KeyCount];

    //music
    sf::Music gamemusic;

    //sound buffers
    sf::SoundBuffer BokBuffer;
    sf::SoundBuffer CarHorn1Buffer;
    sf::SoundBuffer CarHorn2Buffer;
    sf::SoundBuffer CelebrateBuffer;
    sf::SoundBuffer GameOverBuffer;
    sf::SoundBuffer SquishBuffer;

    //sound play
    sf::Sound Bok;
    sf::Sound CarHorn1;
    sf::Sound CarHorn2;
    sf::Sound Celebrate;
    sf::Sound GameOver;
    sf::Sound Squish;

    //fonts
    sf::Font font;

    //textures
    sf::Texture tcar1,tcar2,tcar3,tcar4,tchicken,tchickenhead,thighway;

    //background data
    sf::Sprite background;

    //sprites
    sf::Sprite heads;
    sf::Sprite spChicken;
    sf::Sprite spCar1;
    sf::Sprite spCar2;
    sf::Sprite spCar3;
    sf::Sprite spCar4;

    //player and cars
    std::list<Entity*> entities;

    //the player
    player *p;

    void init(int pscrw, int pscrh, int pposx, int pposy, std::string pname);
    void input();
    void update(sf::Time delta);
    void draw();

    //keyboard functions
    bool KeyPressed(sf::Keyboard::Key Key)
        { return (CurrentKeyState[Key] && !PreviousKeyState[Key]); }

    bool KeyReleased(sf::Keyboard::Key Key)
        { return (!CurrentKeyState[Key] && PreviousKeyState[Key]); }

    bool KeyHeld(sf::Keyboard::Key Key)
        { return CurrentKeyState[Key]; }
};

void Game::init(int pscrw, int pscrh, int pposx, int pposy, std::string pname)
{
    screenwidth = pscrw;
    screenheight = pscrh;
    app.create(sf::VideoMode(pscrw,pscrh),pname);
    app.setPosition(sf::Vector2i(pposx,pposy));

    //load music.
    gamemusic.openFromFile("assets/mus/music.ogg");
    gamemusic.setLoop(true);

    //load textures
    tcar1.loadFromFile("assets/img/car1.png");
    tcar2.loadFromFile("assets/img/car2.png");
    tcar3.loadFromFile("assets/img/car3.png");
    tcar4.loadFromFile("assets/img/car4.png");
    tchicken.loadFromFile("assets/img/chicken.png");
    tchickenhead.loadFromFile("assets/img/chickenhead.png");
    thighway.loadFromFile("assets/img/highway.png");

    //prepare background
    background.setTexture(thighway);

    //prepare the sprites
    heads.setTexture(tchickenhead);
    spChicken.setTexture(tchicken);
    spCar1.setTexture(tcar1);
    spCar2.setTexture(tcar2);
    spCar3.setTexture(tcar3);
    spCar4.setTexture(tcar4);

    // Load the text font
    font.loadFromFile("assets/fnt/sansation.ttf");

    // Load sounds
    BokBuffer.loadFromFile("assets/snd/bok.wav");
    Bok.setBuffer(BokBuffer);
    CarHorn1Buffer.loadFromFile("assets/snd/carhorn1.wav");
    CarHorn1.setBuffer(CarHorn1Buffer);
    CarHorn2Buffer.loadFromFile("assets/snd/carhorn2.wav");
    CarHorn2.setBuffer(CarHorn2Buffer);
    CelebrateBuffer.loadFromFile("assets/snd/celebrate.wav");
    Celebrate.setBuffer(CelebrateBuffer);
    GameOverBuffer.loadFromFile("assets/snd/gameover.wav");
    GameOver.setBuffer(GameOverBuffer);
    SquishBuffer.loadFromFile("assets/snd/squish.wav");
    Squish.setBuffer(SquishBuffer);

    //keyboard buffers initialization
    memset(CurrentKeyState,     false, sizeof(CurrentKeyState));
    memset(PreviousKeyState,    false, sizeof(PreviousKeyState));

    //player creation
    p = new player();
    p->settings(spChicken,4,175,0,0);
    entities.push_back(p);

    //cars creation
    car *c1,*c2,*c3,*c4;
    c1 = new car();
    c2 = new car();
    c3 = new car();
    c4 = new car();
    c1->settings(spCar1,70,0,0,7);
    entities.push_back(c1);
    c2->settings(spCar2,160,0,0,3);
    entities.push_back(c2);
    c3->settings(spCar3,239,400,0,-5);
    entities.push_back(c3);
    c4->settings(spCar4,329,400,0,-10);
    entities.push_back(c4);
}

void Game::input()
{
    switch(state)
        {
        case MENU:
            {
            sf::Event event;
            while (app.pollEvent(event))
            {
                if ((event.type == sf::Event::Closed) ||
                    ((event.type == sf::Event::KeyPressed)
                     && (event.key.code == sf::Keyboard::Escape)))
                    app.close();

                // S key pressed: change state to GAME
                if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S))
                {
                    state=GAME;
                    lives = 3;
                    score = 0;
                    gamemusic.play();
                }
            }
            break;
            }
        case GAME:
            {
            sf::Event event;
            while (app.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    app.close();
            }

            // Save the state of each keyboard key (must be done before any Key* function is executed)
            for(unsigned int i = 0; i < sf::Keyboard::KeyCount; ++i)
            {
                // Save the keyboard's state from the previous frame
                PreviousKeyState[i] = CurrentKeyState[i];

                // And save the keyboard's state in the current frame
                CurrentKeyState[i] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i);
            }

            if(KeyPressed(sf::Keyboard::Escape))
                app.close();

            if (KeyHeld(sf::Keyboard::Right)) p->dx = 20;
            if (KeyHeld(sf::Keyboard::Left))  p->dx = -20;
            if (KeyHeld(sf::Keyboard::Up)) p->dy = -20;
            if (KeyHeld(sf::Keyboard::Down)) p->dy = 20;
            break;
            }
        case END_GAME:
            {
            gamemusic.stop();
            sf::Event event;
            while (app.pollEvent(event))
            {
                if ((event.type == sf::Event::Closed) ||
                    ((event.type == sf::Event::KeyPressed)
                     && (event.key.code == sf::Keyboard::Escape)))
                    app.close();

                // Any key pressed: change state to MENU
                if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::M))
                {
                    state=MENU;
                }
            }
            break;
            }
        default:
            break;
        }
}

void Game::update(sf::Time delta)
{
    //Game_cycle();
    if(state==GAME)
    {

     //play some random car horns
     if( rnd.getRndInt(0,100) == 0 )
     {
         if( rnd.getRndInt(0,1) == 0 )
             CarHorn1.play();
         else
             CarHorn2.play();
     }

      // See if the chicken made it across
      if (p->x > 400)
      {
        // Play a sound for the chicken making it safely across
        Celebrate.play();

        // Move the chicken back to the start and add to the score
        p->x = 4; p->y = 175;
        score += 150;
      }

     for(auto a:entities)
     {
        for(auto b:entities)
        {
          if (a->name=="player" && b->name=="car")
           if ( isCollide(a,b) )
            {
                lives--;
                if(lives<=0)
                {
                    GameOver.play();
                    state=END_GAME;
                }
                else
                {
                    Squish.play();
                }

                //relocate the chicken
                p->x = 4; p->y = 175;
                p->dx = 0;
                p->dy = 0;
            }
        }
     }

        for(auto i=entities.begin();i!=entities.end();i++)
        {
          Entity *e = *i;

          e->update(delta);
        }
    }
}

void Game::draw()
{
    app.clear();
    app.draw(background);

    switch(state)
    {
    case MENU:
        {
        //Show title
        std::string histr="  Henway 2  \n";
        histr += "PRESS S TO START";
        Text(app,histr,150.f,20.f,sf::Color::Cyan,24,font);
        break;
        }
    case GAME:
        {
            for(auto i:entities)
                i->draw(app);

            //draw the lives
            for(int i=0;i<lives;i++)
            {
                heads.setPosition(sf::Vector2f(406 + heads.getGlobalBounds().width * i, 382));
                app.draw(heads);
            }
        }
        break;
    case END_GAME:
        Text(app,"GAME OVER",150.f,30.f,sf::Color::Cyan,50,font);
        Text(app,"PRESS M", 150.f, 100.f, sf::Color::Cyan,25,font);
        break;
    default:
        break;
    }
    app.display();
}

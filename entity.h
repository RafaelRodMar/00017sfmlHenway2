class Entity
{
    public:
        sf::Sprite Sprite;
        float x,y,dx,dy;
        std::string name;
        int scrw=465,scrh=400;

    void settings(sf::Sprite Sp,int X,int Y,int DX, int DY)
    {
        Sprite = Sp;
        x=X; y=Y;
        dx = DX; dy = DY;
    }

    virtual void update(sf::Time dt){};

    void draw(sf::RenderWindow &app)
    {
      Sprite.setPosition( sf::Vector2f(x,y) );
      app.draw(Sprite);
    }
};

class car: public Entity
{
public:
    car()
    {
        name="car";
    }

    void  update(sf::Time dt)
    {
        y += dy * dt.asSeconds();

        if (y < 0)
        {
            y = scrh;
        }
        else
        {
            if( y > scrh )
                y = 0;
        }
    }
};

class player: public Entity
{
public:

   player()
   {
     name="player";
   }

   void update(sf::Time dt)
   {
    x+=dx * dt.asSeconds();
    y+=dy * dt.asSeconds();

    dx=dy=0.0;
    if (x>scrw) x=scrw;
    if (x<0) x=0;
    if (y>scrh) y=scrh;
    if (y<0) y=0;
   }

};

bool isCollide(Entity *a,Entity *b)
{
  /*return (b->x - a->x)*(b->x - a->x)+
         (b->y - a->y)*(b->y - a->y)<
         (a->R + b->R)*(a->R + b->R);*/
  //if ( sf::FloatRect(x+3,y+3,6,6).intersects(block[i].getGlobalBounds()) )
  return a->Sprite.getGlobalBounds().intersects( b->Sprite.getGlobalBounds() );
}

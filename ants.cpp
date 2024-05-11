#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>
using namespace std;

const int WorldSize = 25;
const int Initial_Ants = 100;
const int Initial_doodlebugs = 50;
const int doodlebug = 1;
const int ant = 2;
const int Ant_Breed =3;
const int Doodle_Breed = 8;
const int Doodle_Starve = 3;


class Creatures;
class Ant;
class DoodleBug;


class World
{
friend class Creatures;
friend class DoodleBug;
friend class Ant;


public:

World();
~ World();

Creatures* get_pos(int x, int y);

void set_pos(int x, int y, Creatures* org);
void Show();
void SimulateOneStep();


Creatures* grid[WorldSize][WorldSize];


};


class Creatures
{

friend class World;

public:

Creatures();
Creatures(World* world, int x, int y);
~Creatures();
virtual void breed() = 0;
virtual void move() = 0;
virtual int getType() = 0;
virtual bool starve() = 0;



protected:

int x,y;
bool moved;
int breedTicks;
World* world;

};


World::World()
{
 for (int i = 0; i<WorldSize; i++)
 {
    for (int j = 0; j<WorldSize; j++)
    {
        grid[i][j]=NULL;
    }

 }
}

World::~World()
{
 for (int i = 0; i<WorldSize; i++)
 {
    for (int j = 0; j<WorldSize; j++)
    {
        if (grid[i][j]!=NULL)
        delete (grid[i][j]);
    }
 }
}

Creatures* World::get_pos(int x, int y)
{
    if ((x>=0)&& (x<WorldSize)&&(y>=0)&& (y<WorldSize))
        return grid[x][y];
    return NULL;
}

void World::set_pos(int x, int y, Creatures* org)
{
    if ((x>=0)&& (x<WorldSize)&&(y>=0)&& (y<WorldSize))
        grid[x][y] = org;

}

void World::Show()
{
    for (int j = 0; j<WorldSize; j++)
    {
      for (int i = 0; i<WorldSize; i++)
      {
        if (grid[i][j]==NULL)
            cout<<".";
        else if (grid[i][j]-> getType()==ant)
            cout<<"A";
        else 
            cout<<"X";
      }
      cout<<endl;
    }

}

void World::SimulateOneStep()
{
    int i,j;
    for (int i=0; i<WorldSize;i++)
    {
        for (int j = 0; j<WorldSize; j++)
        {
          if((grid[i][j]!=NULL)&&(grid[i][j]->getType()==doodlebug))
          {
            if(grid[i][j]->moved == false)
            {
             grid[i][j]->moved =true;
             grid[i][j]->move();
             
            }
          }
        }
    }

    for (int i = 0; i<WorldSize;i++)
    {
        for (int j = 0; j<WorldSize; j++)
        {
          if((grid[i][j]!=NULL)&&(grid[i][j]->getType() == doodlebug))
          {
            if (grid[i][j]->starve())
            {
             delete(grid[i][j]);
             grid[i][j]=NULL;
            }


          }
        }
    }

    for (int i =0; i<WorldSize; i++)
    {
        for (int j = 0; j<WorldSize; j++)
        {
         if((grid[i][j]!=NULL)&&(grid[i][j]->moved == true))
            {
                grid[i][j]->breed();
            }
        }

    }

}




Creatures::Creatures()
{
  world = NULL;
  moved = false;
  breedTicks = 0;
  x=0;
  y=0;
}

Creatures::Creatures(World* world_1, int x, int y)
{
    this->world = world_1;
    moved = false;
    breedTicks = 0;
    this->x = x;
    this->y = y;
    world_1->set_pos(x,y,this);
}

Creatures::~Creatures()
{
}

class Ant : public Creatures
{
 friend class World;

 public:
 Ant();
 Ant(World* world, int x, int y);
 void breed();
 void move();
 int getType();
 bool starve()
 {
    return false;
 }

};

Ant::Ant() : Creatures()
{
}


Ant::Ant(World* world, int x, int y): Creatures(world, x, y)
{}
void Ant::move()
{
    int direction = rand()%4;
    
    if (direction == 0)
    {
     if((y>0)&&(world->get_pos(x,y-1) == NULL))
     {
        world->set_pos(x,y-1, world->get_pos(x,y));
        world->set_pos(x,y,NULL);
        y--;
     }
    }
    else if (direction == 1)
    {
     if((y<WorldSize-1)&&(world->get_pos(x,y+1) == NULL))
     {
        world->set_pos(x,y+1, world->get_pos(x,y));
        world->set_pos(x,y,NULL);
        y++;
     }
    }
    else if (direction == 2)
    {
     if((x>0)&&(world->get_pos(x-1,y) == NULL))
     {
        world->set_pos(x-1,y, world->get_pos(x,y));
        world->set_pos(x,y,NULL);
        x--;
     }
    }
    else 
    {
     if((x<WorldSize-1)&&(world->get_pos(x+1,y) == NULL))
     {
        world->set_pos(x+1,y, world->get_pos(x,y));
        world->set_pos(x,y,NULL);
        x++;
     }
    }
}



int Ant::getType()
{ return ant; }

void Ant:: breed()
{
    breedTicks++;
    if ( breedTicks == Ant_Breed )
    {
        breedTicks = 0;
        if ((y>0)&&(world->get_pos(x,y-1)==NULL))
        {
            Ant* newAnt = new Ant(world,x,y-1);
        }
        else if ((y<WorldSize-1)&&(world->get_pos(x,y+1)==NULL))
        {
            Ant* newAnt = new Ant(world,x,y+1);
        }
        
        
        if ((x>0)&&(world->get_pos(x-1,y)==NULL))
        {
            Ant* newAnt = new Ant(world,x-1,y);
        }
        else if ((x<WorldSize-1)&&(world->get_pos(x+1,y)==NULL))
        {
            Ant* newAnt = new Ant(world,x+1,y);
        }
    }
}




class Doodlebug : public Creatures
{
    friend class World;

    public:

    Doodlebug();
    Doodlebug(World* world, int x, int y);

    void breed();
    void move();
    int getType();
    bool starve();

    private:
    int starveTicks;
};


Doodlebug :: Doodlebug() : Creatures()
{
    starveTicks =0;
}

Doodlebug::Doodlebug(World* world, int x, int y): Creatures(world, x,y)
{
    starveTicks = 0;
}

void Doodlebug::move()
{
    if ((y>0)&&(world->get_pos(x,y-1) != NULL)&&(world->get_pos(x,y-1)->getType()==ant))
    {
        delete(world->grid[x][y-1]);
        world->grid[x][y-1]=this;
        world->set_pos(x,y,NULL);
        starveTicks = 0;
        y--;
        return;
    }
    else if ((y<WorldSize-1)&&(world->get_pos(x,y+1)!=NULL)&&(world->get_pos(x,y+1)->getType()==ant))
    {
        delete (world->grid[x][y+1]);
        world->grid[x][y+1]=this;
        world->set_pos(x,y,NULL);
        starveTicks=0;
        y++;
        return;
    }

    else if ((x>0)&&(world->get_pos(x-1,y) != NULL)&&(world->get_pos(x-1,y)->getType()==ant))
    {
        delete(world->grid[x-1][y]);
        world->grid[x-1][y]=this;
        world->set_pos(x,y,NULL);
        starveTicks = 0;
        x--;
        return;
    }

    else if ((x<WorldSize-1)&&(world->get_pos(x+1,y)!=NULL)&&(world->get_pos(x+1,y)->getType()==ant))
    {
        delete (world->grid[x+1][y]);
        world->grid[x+1][y]=this;
        world->set_pos(x,y,NULL);
        starveTicks=0;
        x++;
        return;
    }

int direction = rand()%4;
if (direction==0)
{
  if ((y>0)&&(world->get_pos(x,y-1)==NULL))
  {
    world->set_pos(x,y-1,world->get_pos(x,y));
    world->set_pos(x,y,NULL);
    y--;
  }
}
  else if(direction==1)
  {
   if ((y<WorldSize-1)&&(world->get_pos(x,y+1)==NULL))
   {
    world->set_pos(x,y+1,world->get_pos(x,y));
    world->set_pos(x,y,NULL);
    y++;
   } 
  }

  else if(direction==2)
  {
    if ((x>0)&&(world->get_pos(x-1,y)==NULL))
  {
    world->set_pos(x-1,y,world->get_pos(x,y));
    world->set_pos(x,y,NULL);
    x--;
  } 
  }

  else if(direction==3)
  {
    if ((x<WorldSize-1)&&(world->get_pos(x+1,y)==NULL))
    {
    world->set_pos(x+1,y,world->get_pos(x,y));
    world->set_pos(x,y,NULL);
    x++;
    } 
  }

    starveTicks++;
}

int Doodlebug::getType()
{
    return doodlebug;
}

void Doodlebug::breed()
{
    breedTicks++;
    if(breedTicks == Doodle_Breed)
    {
        breedTicks = 0;
        if((y>0)&&(world->get_pos(x,y-1) == NULL))
        {
            Doodlebug* newDoodle = new Doodlebug(world,x,y-1);
        }
        else if ((y<WorldSize-1)&&(world->get_pos(x,y+1)==NULL))
        {
            Doodlebug* newDoodle= new Doodlebug(world, x, y+1);
        }
        else if((x>0)&&(world->get_pos(x-1,y) == NULL))
        {
            Doodlebug* newDoodle = new Doodlebug(world,x-1,y);
        }
        else if ((x<WorldSize-1)&&(world->get_pos(x+1,y)==NULL))
        {
            Doodlebug* newDoodle= new Doodlebug(world, x+1, y);
        }
    }
}

bool Doodlebug::starve()
{
    if(starveTicks>Doodle_Starve)
    {return true;}
    else
    {return false;}
}


int main()
{
string str;
srand(time(NULL));
World w;
int antcountValue = 0;

while(antcountValue<Initial_Ants)
{
    int x = rand()% WorldSize;
    int y = rand()% WorldSize;

    if(w.get_pos(x,y)==NULL)
    {
     antcountValue++;
     Ant *a1 = new Ant(&w,x,y);
    }
}

int doodlecountValue = 0;

while(doodlecountValue<Initial_doodlebugs)
{
 int x = rand()%WorldSize;
 int y = rand()%WorldSize;
 if (w.get_pos(x,y)==NULL)
 {
    doodlecountValue++;
    Doodlebug *d1 = new Doodlebug(&w,x,y);
 }
}

while(true)
{
 w.Show();
 w.SimulateOneStep();
 cout<<endl<<"Press enter for next step"<<endl;
 getline(cin,str);
}
return 0;

}

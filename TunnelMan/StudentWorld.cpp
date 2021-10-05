#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir); 
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

square::square(int x, int y)
{
    m_x = x;
    m_y = y;
}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 60; j++)
        {
            EarthPtr[i][j] = nullptr;
        }
    }
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            field[i][j] = -1;  // haven't search is labeled -1
        }
    }
    sonarTicks = 0;
    waterTicks = 0;
    m_barrels = 0;
    ticksLastProtestor = 0;
    ProtestorsInGameRn = 0;
}

// NAME: init
// DESCRIPTION: Responsible for creating the current level’s oil field and populating
// it with Earth, Boulders, Barrels of Oil, and Gold Nuggets, and constructing a virtual
// representation of the current level in your StudentWorld class,
// using one or more data structures that you come up with.
// NOTE: you don't call this function by yourself!
// Things to do:
// 1. Initialize the data structures used to keep track of your game’s virtual world
// 2. Construct a new oil field that meets the requirements stated in the section below
// 3. Allocate and insert a valid TunnelMan object into the game world at the proper
// location
int StudentWorld::init()
{
    // Need to create Protestor at the start of every game/level
    ticksLastProtestor = 0;
    ProtestorsInGameRn = 0;
    
    // create TunnelMan that starts at (30, 60)
    m_TunnelMan = new TunnelMan(this, 30, 60);
    m_barrels = 0;
    
    // fill row 0-59 with Earth object
    // tunnel at column 30-33
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 60; j++)
        {
            if (i >= 30 && i <= 33 && j >= 4 && j <= 59)
                EarthPtr[i][j] = nullptr;
            else
                EarthPtr[i][j] = new Earth(this, i, j);
        }
    }

    // sonar ticks update
    int level = getLevel();
    sonarTicks = max(100, 300 - 10*level);
    waterTicks = sonarTicks;

    // create Boulders
    int B = min(level / 2 + 2, 9);
    for (int i = 0; i < B; i++)
    {
        int randX, randY;
        randX = rand() % 61;  // x is between 0-60
        randY = rand() % 37 + 20;  // y is between 20 - 56
        // if the generated x and y is bad, we generate a new one
        while ((randX > 26 && randX <= 33) || !clearCircle(randX, randY, 6.0))
        {
            randX = rand() % 61;  // x is between 0-60
            randY = rand() % 37 + 20;  // y is between 20 - 56
        }
        // now the randX and randY are good
        // remove earth blocks around the area
        for (int i = randX; i < randX+4; i++)
        {
            for (int j = randY; j < randY+4; j++)
            {
                EarthPtr[i][j]->setDead(true);
                EarthPtr[i][j]->setVisible(false);
            }
        }
        Boulder* temp = new Boulder(this, randX, randY);
        m_actors.push_back(temp);
    }

    // create gold nuggets
    int G = max(5 - level / 2, 2);
    for (int i = 0; i < G; i++)
    {
        int randX, randY;
        randX = rand() % 61;  // x is between 0 - 60
        randY = rand() % 57;  // y is between 0 - 56
        // if the generated x and y is bad, we generate a new one
        while ((randX > 26 && randX < 33) || !clearCircle(randX, randY, 6.0))
        {
            randX = rand() % 61;  // x is between 0 - 60
            randY = rand() % 57;  // y is between 0 - 56
        }
        // now the randX and randY are good
        GoldNuggets* temp = new GoldNuggets(this, randX, randY);
        m_actors.push_back(temp);
    }

    // create Barrels
    int L = min(2 + level, 21);
    for (int i = 0; i < L; i++)
    {
        int randX, randY;
        randX = rand() % 61;  // x is between 0 - 60
        randY = rand() % 57;  // y is between 0 - 56
        // if the generated x and y is bad, we generate a new one
        while ((randX > 26 && randX < 33) || !clearCircle(randX, randY, 6.0))
        {
            randX = rand() % 61;  // x is between 0 - 60
            randY = rand() % 57;  // y is between 0 - 56
        }
        // now the randX and randY are good
        OilBarrels* temp = new OilBarrels(this, randX, randY);
        m_actors.push_back(temp);
    }

    return GWSTATUS_CONTINUE_GAME;
}

// NAME: move
// DESCRIPTION: Responsible for asking each of the game’s actors to do something.
int StudentWorld::move()
{
    int level = getLevel();
    // update game status line
    // L is the number of Barrels in this level
    int L = min(2 + level, 21);
    
    // display game stats
    // has error!
    string text = formatText(level, getLives(), getTunnelMan()->getHP()*10, getTunnelMan()->getWater(), getTunnelMan()->getGold(), getTunnelMan()->getSonar(), getScore(), L - m_barrels);
    setGameStatText(text);
    
    // Let Actors do something
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (!m_actors[i]->isDead())
            m_actors[i]->doSomething();
    }
    
    // TunnelMan do something if it is not dead
    if (!m_TunnelMan->isDead())
    {
        m_TunnelMan->doSomething();
        if (m_squirt)
        {
            int squirtX = m_TunnelMan->getX();
            int squirtY = m_TunnelMan->getY();
            if (m_TunnelMan->getDirection() == 1) // up
                squirtY = squirtY + 4;
            else if (m_TunnelMan->getDirection() == 2) // down
                squirtY = squirtY - 4;
            else if (m_TunnelMan->getDirection() == 3) // left
                squirtX = squirtX - 4;
            else if (m_TunnelMan->getDirection() == 4) // right
                squirtX = squirtX + 4;
            Actor* temp = new SquirtWater(this, squirtX, squirtY, m_TunnelMan->getDirection());
            m_actors.push_back(temp);
            m_squirt = false;
        }
        // L is the number of total OilBarrels in this level
        if (m_barrels == L)
        {
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    // let TunnelMan die if it is dead
    if (m_TunnelMan->isDead())
    {
        playSound(SOUND_PLAYER_GIVE_UP);
        decLives();
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
        return GWSTATUS_PLAYER_DIED;
    }
    
    // create new actors
    // 1/G is the probability of adding a goodie
    int addGoodie = rand() % (level * 25 + 300) + 1; // addGoodie is a number from 1 - G
    int addWaterOrSonar = rand() % 5 + 1; // addWaterOrSonar is a number from 1 - 5
    if (addGoodie == 1)
    {
        // we add a Goodie
        if (addWaterOrSonar == 1)
        {
            // we add Sonar
            SonarKit* temp = new SonarKit(this, 0, 60);
            m_actors.push_back(temp);
        }
        else
        {
            // we add WaterPool
            int randX, randY;
            do
            {
                // generate a good (x, y)
                randX = rand() % 61;
                randY = rand() % 61;
            } while (!has4x4Square(randX, randY));
            // the (x, y) is now good to use
            WaterPool* temp = new WaterPool(this, randX, randY);
            m_actors.push_back(temp);
        }
    }
    
    // add Protestor
    int ticksToAddNextProtestor = max(25, 200 - level);
    int targetProtestors = min(15.0, 2 + level * 1.5);
    int probabilityOfHardcore = min(90, level * 10 + 30);
    // need to add new Protestor!
    if (ticksLastProtestor == 0 && ProtestorsInGameRn < targetProtestors)
    {
        int pick = rand() % probabilityOfHardcore;
        if (pick == 0)
        {
            // make HardcoreProtestor
            HardcoreProtestor* temp = new HardcoreProtestor(this, 60, 60);
            m_actors.push_back(temp);
        }
        else
        {
            // Regular Protestor
            RegularProtestor* temp = new RegularProtestor(this, 60, 60);
            m_actors.push_back(temp);
        }
        ProtestorsInGameRn++;
        ticksLastProtestor = ticksToAddNextProtestor;
    }
    else
        ticksLastProtestor--;
    
    return GWSTATUS_CONTINUE_GAME;
}

// NAME: cleanUp
// DESCRIPTION: Responsible for freeing all actors that are currently active in the game.
// Is called by our framework when the player loses a life, or the player
// successfully completes the current level.
void StudentWorld::cleanUp()
{
    // clean up Actors
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        delete *it;
    }
    // delete everything in the vector
    m_actors.clear();
    
    // clean up Earth
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 60; j++)
        {
            if (EarthPtr[i][j] != nullptr)
            {
                delete EarthPtr[i][j];
            }
        }
    }
    delete m_TunnelMan;
}

StudentWorld::~StudentWorld()
{
    // clean up Actors
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        delete *it;
    }
    m_actors.clear();
    
    // clean up Earth
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 60; j++)
        {
            if (EarthPtr[i][j] != nullptr)
            {
                delete EarthPtr[i][j];
            }
        }
    }
    delete m_TunnelMan;
}


// Accessor to access EarthPtr
Earth* StudentWorld::getEarth(int x, int y) { return EarthPtr[x][y]; }
// Accessor to access TunnelMan
TunnelMan* StudentWorld::getTunnelMan() { return m_TunnelMan; }

// change squirt state
void StudentWorld::setSquirt(bool squirt) { m_squirt = squirt; }
// Accessor to get the number of ticks required for sonar
int StudentWorld::getSonarTicks() { return sonarTicks; }
int StudentWorld::getWaterTicks() { return waterTicks; }

bool StudentWorld::overlap(int TunnelManX, int TunnelManY)
{
    // check if the earth at that position is not nullptr and is currently active
    if (earthAt(TunnelManX, TunnelManY) && !EarthPtr[TunnelManX][TunnelManY]->isDead())
    {
        EarthPtr[TunnelManX][TunnelManY]->setVisible(false);
        EarthPtr[TunnelManX][TunnelManY]->setDead(true);
        return true;
    }
    return false;
}


// returns true if there is Earth at x, y
// returns false if there is no Earth at x, y
bool StudentWorld::earthAt(int x, int y)
{
    if (x < 0 || x > 63 || y < 0 || y > 59)
        return false;
    if (EarthPtr[x][y] == nullptr)
        return false;
    if (EarthPtr[x][y]->isDead() && !EarthPtr[x][y]->isVisible())
        return false;
    return true;
}


double StudentWorld::getDistance (int x1, int y1, int x2, int y2)
{
    int x_diff = x1 - x2;
    int y_diff = y1 - y2;
    double num = sqrt(x_diff*x_diff + y_diff*y_diff);
    return num;
}


bool StudentWorld::clearCircle(int x, int y, double r)
{
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if (getDistance((*it)->getX(), (*it)->getY(), x, y) < r)
            return false;
    }
    return true;
}


bool StudentWorld::has4x4Square(int x, int y)
{
    for (int i = x; i < x+4; i++)
    {
        for (int j = y; j < y+4; j++)
        {
            if (!(EarthPtr[i][j] == nullptr || EarthPtr[i][j]->isDead() || EarthPtr[i][j]->isBoulderHere()))
                return false;
        }
    }
    return true;
}

void StudentWorld::addOilBarrels() { m_barrels++; }

void StudentWorld::dropGold(int x, int y)
{
    GoldNuggets* temp = new GoldNuggets(this, x, y);
    temp->setDroppedByTM(true);
    temp->setVisible(true);
    m_actors.push_back(temp);
}

string StudentWorld::formatText(int level, int lives, int health, int water, int gold, int sonar, int score, int oilLeft)
{
    ostringstream oss;
    string finalText;
    oss << "Lvl: " << setw(2) << level << "  Lives: " << setw(1) << lives << "  Hlth: " << setw(3) << health << "%  Wtr: " << setw(2) << water << "  Gld: " << setw(2) << gold << "  Oil Left: " << setw(2) << oilLeft << "  Sonar: " << setw(2) << sonar << "  Scr: ";
    oss.fill('0');
    oss << setw(6) << score;
    finalText = oss.str();
    return finalText;
}

bool StudentWorld::BoulderHere(int x, int y, int r)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->amIBoulder())
        {
            if (getDistance(x, y, m_actors[i]->getX(), m_actors[i]->getY()) <= r && !m_actors[i]->isDead())
                return true;
        }
    }
    return false;
}

bool StudentWorld::ProtestorHere(int x, int y, int r)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->amIProtestor())
        {
            if (getDistance(x, y, m_actors[i]->getX(), m_actors[i]->getY()) < r && !m_actors[i]->getActorState())
            {
                m_actors[i]->annoyProtestor(2);
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::GoldHereAndFromTM(int x, int y, int r)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->amIGold())
        {
            if (m_actors[i]->droppedByTM() && (getDistance(x, y, m_actors[i]->getX(), m_actors[i]->getY()) < r) && !m_actors[i]->isDead())
            {
                m_actors[i]->setDead(true);
                m_actors[i]->setVisible(false);
                return true;
            }
        }
    }
    return false;
}

std::vector<Actor::Direction> StudentWorld::atIntersection(int x, int y, Actor::Direction ProtestorDir, std::vector<Actor::Direction> &temp)
{
    if (ProtestorDir == Actor::up || ProtestorDir == Actor::down)
    {
        if (has4x4Square(x-1, y))
        {
            temp.push_back(Actor::left);
        }
        if (has4x4Square(x+1, y))
        {
            temp.push_back(Actor::right);
        }
    }
    else if (ProtestorDir == Actor::left || ProtestorDir == Actor::right)
    {
        if (has4x4Square(x, y-1))
        {
            temp.push_back(Actor::down);
        }
        if (has4x4Square(x, y+1))
        {
            temp.push_back(Actor::up);
        }
    }
    return temp;
}

void StudentWorld::useSonar(int x, int y)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->amIGold() || m_actors[i]->amIOilBarrel())
        {
            if (getDistance(m_actors[i]->getX(), m_actors[i]->getY(), x, y) < 12 && !m_actors[i]->isDead())
                m_actors[i]->setVisible(true);
        }
    }
}

int StudentWorld::searchExit(Protestor* exitPro, int endX, int endY)
{
    // reset the field
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            field[i][j] = -1;  // haven't search is labeled -1
        }
    }
    int x, y;
    int ProX = exitPro->getX();
    int ProY = exitPro->getY();
    std::queue<square> store;
    field[endX][endY] = 0; // our exit
    store.push(square(endX, endY));
    while (!store.empty())
    {
        x = store.front().m_x;
        y = store.front().m_y;
        store.pop();
        if (ProX == x && ProY == y)
        {
            // we have found the protestor
            break;
        }
        if (y >= 0 && !earthAt(x, y-1) && !earthAt(x+1, y-1) && !earthAt(x+2, y-1) && !earthAt(x+3, y-1) && field[x][y-1] == -1 && !BoulderHere(x, y-1, 4))
        {
            field[x][y-1] = field[x][y]+1;
            store.push(square(x, y-1));
        }
        if (x >= 0 && !earthAt(x-1, y) && !earthAt(x-1, y+1) && !earthAt(x-1, y+2) && !earthAt(x-1, y+3) && field[x-1][y] == -1 && !BoulderHere(x-1, y, 4))
        {
            field[x-1][y] = field[x][y]+1;
            store.push(square(x-1, y));
        }
        if (y < 64 && !earthAt(x, y+4) && !earthAt(x+1, y+4) && !earthAt(x+2, y+4) && !earthAt(x+3, y+4) && field[x][y+1] == -1 && !BoulderHere(x, y+4, 4))
        {
            field[x][y+1] = field[x][y]+1;
            store.push(square(x, y+1));
        }
        if (x < 64 && !earthAt(x+4, y) && !earthAt(x+4, y+1) && !earthAt(x+4, y+2) && !earthAt(x+4, y+3) && field[x+1][y] == -1 && !BoulderHere(x+4, y, 4))
        {
            field[x+1][y] = field[x][y]+1;
            store.push(square(x+1, y));
        }
    }
    return field[ProX][ProY];
}

void StudentWorld::moveToExit(Protestor* exitPro)
{
    int ProX = exitPro->getX();
    int ProY = exitPro->getY();
    // move Protestor one step closer to the desired position
    // move right
    if (field[ProX+1][ProY] < field[ProX][ProY] && field[ProX+1][ProY] != -1)
    {
        exitPro->setDirection(Actor::right);
        exitPro->moveTo(ProX+1, ProY);
    }
    // move left
    else if (field[ProX-1][ProY] < field[ProX][ProY] && field[ProX-1][ProY] != -1)
    {
        exitPro->setDirection(Actor::left);
        exitPro->moveTo(ProX-1, ProY);
    }
    // move up
    else if (field[ProX][ProY+1] < field[ProX][ProY] && field[ProX][ProY+1] != -1)
    {
        exitPro->setDirection(Actor::up);
        exitPro->moveTo(ProX, ProY+1);
    }
    // move down
    else if (field[ProX][ProY-1] < field[ProX][ProY] && field[ProX][ProY-1] != -1)
    {
        exitPro->setDirection(Actor::down);
        exitPro->moveTo(ProX, ProY-1);
    }
}

void StudentWorld::decProtestorsInGameRn()
{
    ProtestorsInGameRn--;
}

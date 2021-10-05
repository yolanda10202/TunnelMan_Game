#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// Here's GraphObject:
/*
 // You must not redefine any of these methods in your derived classes since
 // they are not defined as virtual in our base class.
 GraphObject(int imageID, int startX, int startY, DIRECTION startDirection,
             float size = 1.0, unsigned int depth = 0);
 void setVisible(bool shouldIDisplay);
 void getX() const;
 void getY() const;
 void moveTo(int x, int y);
 DIRECTION getDirection() const; // Directions: up, down, left, right
 void setDirection(DIRECTION d); // Directions: up, down, left, right
*/

//----------------------------------
// Actor class
// constructor
Actor::Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
: GraphObject(imageID, startX, startY, startDirection, size, depth)
{
    m_isDead = false;
    setVisible(true);
    m_studentWorld = sw;
}

// destructor
 Actor::~Actor()
{
    setVisible(false);
}

bool Actor::isDead()
{
    return m_isDead;
}

StudentWorld* Actor::getStudentWorld()
{
    return m_studentWorld;
}

void Actor::setDead(bool deadOrNot)
{
    m_isDead = deadOrNot;
}

bool Actor::amIBoulder() { return false; }
bool Actor::amIProtestor() { return false; }
bool Actor::amIGold() { return false; }
bool Actor::amIOilBarrel() { return false; }
bool Actor::droppedByTM() { return false; }
bool Actor::getActorState() { return false; }
void Actor::annoyProtestor(int points) {}





//----------------------------------
// Earth class
// constructor
Earth::Earth(StudentWorld* sw, int x, int y)
: Actor(sw, TID_EARTH, x, y, right, 0.25, 3)
{
    hasBoulder = false;
}

Earth::~Earth() {}
void Earth::doSomething() {}
// Earth Accessors
bool Earth::isBoulderHere() { return hasBoulder; }
// Earth Mutators
void Earth::setHasBoulder(bool thereIsBoulder) { hasBoulder = thereIsBoulder; }





//----------------------------------
// TunnelMan class
// constructor
TunnelMan::TunnelMan(StudentWorld* sw, int x, int y)
: Actor(sw, TID_PLAYER, x, y, right, 1, 0)
{
    m_goldAmount = 0;
    m_waterAmount = 5;
    m_sonarAmount = 1;
    m_hitPoints = 10;
}

// destructor
TunnelMan::~TunnelMan() {}

// TunnelMan Accessors
int TunnelMan::getHP() { return m_hitPoints; }
int TunnelMan::getWater() { return m_waterAmount; }
int TunnelMan::getGold() { return m_goldAmount; }
int TunnelMan::getSonar() {return m_sonarAmount; }


// TunnelMan Mutators
void TunnelMan::addWater() { m_waterAmount = m_waterAmount + 5; }
void TunnelMan::addGold() { m_goldAmount++; }
void TunnelMan::addSonar() { m_sonarAmount++; }
void TunnelMan::decreaseHP(int points) { m_hitPoints = m_hitPoints - points; }

void TunnelMan::doSomething()
{
    // make sure TunnelMan is able to doSomething
    if (getHP() <= 0)
    {
        setDead(true);
        return;
    }
    
    // TunnelMan is not dead and can doSomething
    
    int character;
    if (getStudentWorld()->getKey(character))
    {
        switch (character)
        {
            case KEY_PRESS_LEFT:
                if (getDirection() != left)
                    setDirection(left);
                else if (getX()-1 >= 0 && !getStudentWorld()->BoulderHere(getX()-1, getY(), 3))
                {
                    moveTo(getX()-1, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() != right)
                    setDirection(right);
                else if (getX()+1 <= 60 && !getStudentWorld()->BoulderHere(getX()+1, getY(), 3))
                {
                    moveTo(getX()+1, getY());
                }
                break;
            case KEY_PRESS_UP:
                if (getDirection() != up)
                    setDirection(up);
                else if (getY()+1 <= 60 && !getStudentWorld()->BoulderHere(getX(), getY()+1, 3))
                {
                    moveTo(getX(), getY()+1);
                }
                break;
            case KEY_PRESS_DOWN:
                if (getDirection() != down)
                    setDirection(down);
                else if (getY()-1 >= 0 && !getStudentWorld()->BoulderHere(getX(), getY()-1, 3))
                {
                    moveTo(getX(), getY()-1);
                }
                break;
            case KEY_PRESS_TAB:  // drop gold
                if (m_goldAmount > 0)
                {
                    // there's enough gold to drop
                    m_goldAmount--;
                    getStudentWorld()->dropGold(getX(), getY());
                    
                }
                break;
            case KEY_PRESS_SPACE:  // squirt water
                if (m_waterAmount > 0)
                {
                    getStudentWorld()->setSquirt(true);
                    getStudentWorld()->playSound(SOUND_PLAYER_SQUIRT);
                    m_waterAmount--;
                }
                break;
            case KEY_PRESS_ESCAPE:  // quit game
                setDead(true);
                break;
            case 'Z':  // sonar
            case 'z':
                if (m_sonarAmount > 0)
                {
                    getStudentWorld()->playSound(SOUND_SONAR);
                    m_sonarAmount--;
                    // find stuff
                    getStudentWorld()->useSonar(getX(), getY());
                }
                break;
        }
    }
    // we need to delete Earth if TunnelMan is digging the Earth
    bool isDigging = false;
    for (int i = getX(); i < getX()+4; i++)
    {
        for (int j = getY(); j < getY()+4; j++)
        {
            if (getStudentWorld()->overlap(i, j))
                isDigging = true;
        }
    }
    if (isDigging)
        getStudentWorld()->playSound(SOUND_DIG);
}





//----------------------------------
// Boulder class
Boulder::Boulder(StudentWorld* sw, int x, int y)
: Actor(sw, TID_BOULDER, x, y, down, 1, 1)
{
    setVisible(true);
    m_state = stable;
    tickCount = 0;
}

Boulder::~Boulder() {}

Boulder::BoulderState Boulder::getState() { return m_state; }

void Boulder::doSomething()
{
    if (isDead())
        return;
    if (getState() == stable)
    {
        // check if the Earth underneath it is still there
        if (!(getStudentWorld()->earthAt(getX(), getY()-1) ||
              getStudentWorld()->earthAt(getX()+1, getY()-1) ||
              getStudentWorld()->earthAt(getX()+2, getY()-1) ||
              getStudentWorld()->earthAt(getX()+3, getY()-1)))
        {
            // all these positions have no Earth underneath
            setState(waiting);
        }
    }
    else if (getState() == waiting)
    {
        if (tickCount >= 30)
        {
            setState(falling);
            getStudentWorld()->playSound(SOUND_FALLING_ROCK);
        }
        else
            tickCount++;
    }
    else if (getState() == falling)
    {
        // continuously falling
        getStudentWorld()->getEarth(getX(), getY())->setHasBoulder(false);
        fallingBoulder(getX(), getY());
    }
}

void Boulder::setState(BoulderState curState) { m_state = curState; }

void Boulder::fallingBoulder(int x, int y)
{
    if (canMove(x, y))
    {
        moveTo(x, y-1);
    }
    else
    {
        setDead(true);
        setVisible(false);
    }
}

bool Boulder::canMove(int x, int y)
{
    // reach beyond bottom
    if (y < 0)
        return false;
    // hit existing earth
    if (getStudentWorld()->earthAt(x, y-1) ||
        getStudentWorld()->earthAt(x+1, y-1) ||
        getStudentWorld()->earthAt(x+2, y-1) ||
        getStudentWorld()->earthAt(x+3, y-1))
        return false;
    if (getStudentWorld()->getEarth(x, y-1)->isBoulderHere() || getStudentWorld()->getEarth(x+1, y-1)->isBoulderHere() || getStudentWorld()->getEarth(x+2, y-1)->isBoulderHere() || getStudentWorld()->getEarth(x+3, y-1)->isBoulderHere())
        return false;
    // hit TunnelMan
    for (int i = getStudentWorld()->getTunnelMan()->getX(); i < getStudentWorld()->getTunnelMan()->getX()+4; i++)
    {
        for (int j = getStudentWorld()->getTunnelMan()->getY(); j < getStudentWorld()->getTunnelMan()->getY()+4; j++)
        {
            if ((i == x && j == y-1) || (i == x+1 && j == y-1) || (i == x+2 && j == y-1) || (i == x+3 && j == y-1))
            {
                // hit TunnelMan
                getStudentWorld()->getTunnelMan()->setDead(true);
                return false;
            }
        }
    }
    return true;
}





//----------------------------------
// SquirtWater class
SquirtWater::SquirtWater(StudentWorld* sw, int TunnelManX, int TunnelManY, Direction TunnelManDir)
: Actor(sw, TID_WATER_SPURT, TunnelManX, TunnelManY, TunnelManDir, 1, 1)
{
    moved = 0;
    setVisible(false);
}

void SquirtWater::doSomething()
{
    if (isDead())
        return;
    if (getStudentWorld()->ProtestorHere(getX(), getY(), 4))
    {
        // we have annoyed Protestor
        setDead(true);
        setVisible(false);
        return;
    }
    // traveled full distance
    if (moved == 5)
    {
        setDead(true);
        setVisible(false);
        return;
    }
    // check if it can move one square in its current facing direction
    if (canMove(getX(), getY(), getDirection()))
    {
        setVisible(true);
        if (getDirection() == left)
            moveTo(getX()-1, getY());
        else if (getDirection() == right)
            moveTo(getX()+1, getY());
        else if (getDirection() == up)
            moveTo(getX(), getY()+1);
        else
            moveTo(getX(), getY()-1);
        moved++;
    }
    else
    {
        setVisible(false);
        setDead(true);
    }
}

bool SquirtWater::canMove(int squirtX, int squirtY, Direction squirtDir)
{
    if (squirtDir == left)
        squirtX = squirtX - 1;
    else if (squirtDir == right)
        squirtX = squirtX + 4;
    else if (squirtDir == up)
        squirtY = squirtY + 4;
    else if (squirtDir == down)
        squirtY = squirtY - 1;
    bool meetEarth = getStudentWorld()->earthAt(squirtX, squirtY);
    bool outOfX = squirtX < 0 || squirtX > 63;
    bool outOfY = squirtY < 0 || squirtY > 60;
    if (outOfX || outOfY || meetEarth)
        return false;
    return true;
}





//----------------------------------
// Items class
Items::Items(StudentWorld* sw, int imageID, int startX, int startY)
: Actor(sw, imageID, startX, startY, right, 1, 2)
{
    m_ticks = 0;
}

Items::~Items() {}
int Items::getTicks() { return m_ticks; }
void Items::increaseTicks() { m_ticks++; }


//----------------------------------
// GoldNugget class
GoldNuggets::GoldNuggets(StudentWorld* sw, int x, int y)
: Items(sw, TID_GOLD, x, y)
{
    setVisible(false);
    isFromTM = false;
}

GoldNuggets::~GoldNuggets() {}

void GoldNuggets:: doSomething()
{
    if (isDead())
        return;
    int TMX = getStudentWorld()->getTunnelMan()->getX();
    int TMY = getStudentWorld()->getTunnelMan()->getY();
    // TunnelMan is near GoldNugget
    if (!isVisible() && getStudentWorld()->getDistance(TMX, TMY, getX(), getY()) <= 4)
    {
        setVisible(true);
        return;
    }
    // TunnelMan picks it up
    else if (getStudentWorld()->getDistance(TMX, TMY, getX(), getY()) <= 3 && !droppedByTM())
    {
        setDead(true);
        setVisible(false);
        getStudentWorld()->playSound(SOUND_GOT_GOODIE);
        getStudentWorld()->increaseScore(10);
        getStudentWorld()->getTunnelMan()->addGold();
        return;
    }
    // currently at a temporary state
    else if (droppedByTM() && getTicks() >= 100)
    {
        setDead(true);
        setVisible(false);
        return;
    }
    else if (droppedByTM() && getTicks() < 100)
    {
        increaseTicks();
    }
}

bool GoldNuggets::droppedByTM()
{
    return isFromTM;
}

void GoldNuggets::setDroppedByTM(bool TMDropGold)
{
    isFromTM = TMDropGold;
}

bool GoldNuggets::amIGold() { return true; }





//----------------------------------
// OilBarrel class
OilBarrels::OilBarrels(StudentWorld* sw, int x, int y)
: Items(sw, TID_BARREL, x, y)
{
    setVisible(false);
}

OilBarrels::~OilBarrels() {}

void OilBarrels::doSomething()
{
    if (isDead())
        return;
    int TMX = getStudentWorld()->getTunnelMan()->getX();
    int TMY = getStudentWorld()->getTunnelMan()->getY();
    if (!isVisible() && getStudentWorld()->getDistance(TMX, TMY, getX(), getY()) <= 4)
    {
        setVisible(true);
        return;
    }
    else if (getStudentWorld()->getDistance(TMX, TMY, getX(), getY()) <= 3)
    {
        setDead(true);
        setVisible(false);
        getStudentWorld()->playSound(SOUND_FOUND_OIL);
        getStudentWorld()->increaseScore(1000);
        getStudentWorld()->addOilBarrels();
    }
}

bool OilBarrels::amIOilBarrel() { return true; }





//----------------------------------
// SonarKit class
SonarKit::SonarKit(StudentWorld* sw, int x, int y)
: Items(sw, TID_SONAR, x, y)
{
    setVisible(true);
}

SonarKit::~SonarKit() {}

void SonarKit::doSomething()
{
    // ask itself if it is still alive
    if (isDead())
        return;
    int TunnelManX = getStudentWorld()->getTunnelMan()->getX();
    int TunnelManY = getStudentWorld()->getTunnelMan()->getY();
    // TunnelMan overlaps SonarKit: picks it up
    if (getStudentWorld()->getDistance(TunnelManX, TunnelManY, getX(), getY()) <= 3)
    {
        setDead(true);
        setVisible(false);
        getStudentWorld()->playSound(SOUND_GOT_GOODIE);
        getStudentWorld()->getTunnelMan()->addSonar();
        getStudentWorld()->increaseScore(75);
    }
    // check if Sonar should be gone
    if (getTicks() >= getStudentWorld()->getSonarTicks())
    {
        setDead(true);
        setVisible(false);
    }
    increaseTicks();
}





//----------------------------------
// WaterPool class
WaterPool::WaterPool(StudentWorld* sw, int x, int y)
: Items(sw, TID_WATER_POOL, x, y)
{
    setVisible(true);
}

WaterPool::~WaterPool() {}

void WaterPool::doSomething()
{
    if (isDead())
        return;
    int TunnelManX = getStudentWorld()->getTunnelMan()->getX();
    int TunnelManY = getStudentWorld()->getTunnelMan()->getY();
    // TunnelMan overlaps SonarKit: picks it up
    if (getStudentWorld()->getDistance(TunnelManX, TunnelManY, getX(), getY()) <= 3)
    {
        setDead(true);
        setVisible(false);
        getStudentWorld()->playSound(SOUND_GOT_GOODIE);
        getStudentWorld()->getTunnelMan()->addWater();
        getStudentWorld()->increaseScore(100);
    }
    if (getTicks() >= getStudentWorld()->getWaterTicks())
    {
        setDead(true);
        setVisible(false);
    }
    increaseTicks();
}





//----------------------------------
// Protestor class
Protestor::Protestor(StudentWorld* sw, int x, int y, int protestorImage)
: Actor(sw, protestorImage, x, y, left, 1, 0)
{
    numSquaresToMoveInCurrentDirection = rand() % 61 + 8;
    setVisible(true);
    ticksPassed = 0;
    m_ticksSinceShouted = 0;
    ticksAfterStunned = 0;
    ticksSinceTurned = 200;
    isStunned = false;
    leaveTheOilField = false;
    diedFromBoulder = false;
    m_hp = 10;
}

Protestor::~Protestor() {}

void Protestor::setHP(int hp) { m_hp = hp; }

// Accessors
int Protestor::getTicksPassed() { return ticksPassed; }

// Mutator
void Protestor::decNumSquaresToMoveInCurrentDir() { numSquaresToMoveInCurrentDirection--; }
int Protestor::getTicksSinceTurned() { return ticksSinceTurned; }
void Protestor::resetTicksSinceTurned() { ticksSinceTurned = 0; }
void Protestor:: increaseTicksSinceTurned() { ticksSinceTurned++; }

bool Protestor::ProtestorCanMoveHere(int x, int y)
{
    if (x < 0 || x > 60 || y < 0 || y > 60)
        return false;
    if (getStudentWorld()->BoulderHere(x, y, 3))
        return false;
    // cannot walk onto Earth
    for (int i = x; i < x+4; i++)
    {
        for (int j = y; j < y+4; j++)
            if (getStudentWorld()->earthAt(i, j))
                return false;
    }
    return true;
}

bool Protestor::amIProtestor() { return true; }
bool Protestor::getDiedFromBoulder() { return diedFromBoulder; }
void Protestor::setDiedFromBoulder() { diedFromBoulder = true; }

int Protestor::getNumSquaresToMoveInCurrentDir()
{
    return numSquaresToMoveInCurrentDirection;
}

void Protestor::protestorYells()
{
    // Protestor yells
    getStudentWorld()->playSound(SOUND_PROTESTER_YELL);
    // decreases TunnelMan's hp
    getStudentWorld()->getTunnelMan()->decreaseHP(2);
    // reset ticksSinceShouted
    m_ticksSinceShouted = 0;
}

void Protestor::increaseShoutTicks()
{
        m_ticksSinceShouted++;
}

void Protestor::increaseTicks() { ticksPassed++; }
int Protestor::countTicksToWaitBetweenMoves()
{
    int curLevel = Actor::getStudentWorld()->getLevel();
    int ticks = std::max(0, 3 - curLevel / 4);
    return ticks;
}
void Protestor::resetTicksPassed() { ticksPassed = 0; }

bool Protestor::noShoutForAWhile()
{
    if (m_ticksSinceShouted >= 15)
        return true;
    return false;
}

bool Protestor::canSeeTunnelMan()
{
    int TMX = getStudentWorld()->getTunnelMan()->getX();
    int TMY = getStudentWorld()->getTunnelMan()->getY();
    // Not even at the same line
    if (TMX != getX() && TMY != getY())
        return false;
    if (TMX == getX() && TMY != getY())
    {
        // need to check if there is any earth in between
        if (TMY < getY())
        {
            for (int i = getX(); i < getX()+4; i++)
            {
                for (int j = getY(); j >= TMY; j--)
                {
                    if (getStudentWorld()->earthAt(i, j))
                        return false;
                }
            }
        }
        else if (TMY > getY())
        {
            for (int i = getX(); i < getX()+4; i++)
            {
                for (int j = getY(); j <= TMY; j++)
                {
                    if (getStudentWorld()->earthAt(i, j))
                        return false;
                }
            }
        }
    }
    else if (TMX != getX() && TMY == getY())
    {
        // need to check if there is any earth in between
        if (TMX < getX())
        {
            for (int i = getY(); i < getY()+4; i++)
            {
                for (int j = getX(); j >= TMX; j--)
                {
                    if (getStudentWorld()->earthAt(j, i))
                        return false;
                }
            }
        }
        else if (TMX > getX())
        {
            for (int i = getY(); i < getY()+4; i++)
            {
                for (int j = getX(); j <= TMX; j++)
                {
                    if (getStudentWorld()->earthAt(j, i))
                        return false;
                }
            }
        }
    }
    return true;
}

bool Protestor::canDirectlySeeTM(Direction ProtestorDir, int TMX, int TMY)
{
    if (ProtestorDir == up && TMY > getY())
    {
        return true;
    }
    else if (ProtestorDir == down && TMY < getY())
    {
        return true;
    }
    else if (ProtestorDir == right && TMX > getX())
    {
        return true;
    }
    else if (ProtestorDir == left && TMX < getX())
    {
        return true;
    }
    return false;
}

void Protestor::annoyProtestor(int points) {}

void Protestor::randomGenerateMoveInCurrentDir()
{
    numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
}
void Protestor::resetNumSquaresToMoveInCurrentDir() { numSquaresToMoveInCurrentDirection = 0; }
bool Protestor::getActorState() { return leaveTheOilField; }
void Protestor::setProtestorState(bool state) { leaveTheOilField = state; }
int Protestor::getHP() { return m_hp; }
void Protestor::decreaseHP(int points) { m_hp = m_hp - points; }
bool Protestor::isProtestorStunned() { return isStunned; }
void Protestor::setStunned(bool stunned) { isStunned = stunned; }

void Protestor::increaseStunnedTicks()
{
    ticksAfterStunned++;
}

int Protestor::getStunnedTicks()
{
    return ticksAfterStunned;
}

void Protestor::resetStunnedTicks()
{
    ticksAfterStunned = 0;
}

int Protestor::howLongToWaitWhenStunned()
{
    int level = getStudentWorld()->getLevel();
    int waitingTicks = std::max(50, 100 - level * 10);
    return waitingTicks;
}





//----------------------------------
// RegularProtestor class
RegularProtestor::RegularProtestor(StudentWorld* sw, int x, int y)
: Protestor(sw, x, y, TID_PROTESTER)
{
    
}

RegularProtestor::~RegularProtestor() {}

void RegularProtestor::protestorPicksGold()
{
    getStudentWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getStudentWorld()->increaseScore(25);
    setProtestorState(true);
}

void RegularProtestor::doSomething()
{
    int TMX = Actor::getStudentWorld()->getTunnelMan()->getX();
    int TMY = Actor::getStudentWorld()->getTunnelMan()->getY();
    
    // Protestor is dead, shouldn't be moving
    if (isDead())
        return;
    // Protestor is in a rest state, shouldn't be moving
    if (getTicksPassed() < countTicksToWaitBetweenMoves())
    {
        increaseTicks();
        return;
    }
    // Protestor is stunned by water
    if (isProtestorStunned())
    {
        if (getStunnedTicks() < howLongToWaitWhenStunned())
        {
            // still needs to be stunned
            increaseStunnedTicks();
            return;
        }
        else
        {
            resetStunnedTicks();
            setStunned(false);
        }
    }
    
    // Protestor has hp = 0 and should leave the oil field
    if (getActorState())
    {
        if (getX() == 60 && getY() == 60)
        {
            setDead(true);
            setVisible(false);
            getStudentWorld()->decProtestorsInGameRn();
            return;
        }
        else
        {
            // go towards the exit
            getStudentWorld()->searchExit(this, 60, 60);
            getStudentWorld()->moveToExit(this);
            resetTicksPassed();
        }
    }
    // Protestor should keep wandering around the oil field
    else
    {
        resetTicksPassed();
        increaseShoutTicks();
        increaseTicksSinceTurned();
        // Protestor picks gold
        if (getStudentWorld()->GoldHereAndFromTM(getX(), getY(), 4))
        {
            protestorPicksGold();
            return;
        }
        // check if Protestor is hit by Boulder at the moment
        if (getStudentWorld()->BoulderHere(getX(), getY(), 3))
        {
            // protestor is bonked by Boulder
            // Protestor needs to leave the field
            setHP(0);
            annoyProtestor(0);
            setDiedFromBoulder();
            return;
        }
        
        // check if Protestor is near TunnelMan and can shout
        // TunnelMan is within the radius and Protestor hasn't shouted for a while
        if (getStudentWorld()->getDistance(getX(), getY(), TMX, TMY) <= 4)
        {
            if (canDirectlySeeTM(getDirection(), TMX, TMY) && noShoutForAWhile())
                protestorYells();
            return;
        }
        // check if Protestor is in a straight horizontal or vertical line of sight to the TunnelMan
        else if (canSeeTunnelMan())
        {
            resetNumSquaresToMoveInCurrentDir();
            // Protestor should run towards TunnelMan
            if (TMX < getX())
                setDirection(left);
            else if (TMX > getX())
                setDirection(right);
            else if (TMY < getY())
                setDirection(down);
            else if (TMY > getY())
                setDirection(up);
            
            // move towards TunnelMan
            if (getDirection() == left && ProtestorCanMoveHere(getX()-1, getY()))
                moveTo(getX()-1, getY());
            else if (getDirection() == right && ProtestorCanMoveHere(getX()+1, getY()))
                moveTo(getX()+1, getY());
            else if (getDirection() == up && ProtestorCanMoveHere(getX(), getY()+1))
                moveTo(getX(), getY()+1);
            else if (getDirection() == down && ProtestorCanMoveHere(getX(), getY()-1))
                moveTo(getX(), getY()-1);
            return;
        }
        
        // Protestor at an intersection
        if (getTicksSinceTurned() >= 200)
        {
            std::vector<Actor::Direction> perpPath;
            getStudentWorld()->atIntersection(getX(), getY(), getDirection(), perpPath);
            // if there is more than one possible direction, we randomly choose one direction
            if (perpPath.size() > 0)
            {
                int chooseDir = rand() % perpPath.size();
                setDirection(perpPath[chooseDir]);
                resetTicksSinceTurned();
            }
            // if no other Direction, we don't need to turn
        }
        
        // Protester has finished walking numSquaresToMoveInCurrentDirection steps in its
        // currently-selected direction
        if (getNumSquaresToMoveInCurrentDir() <= 0)
        {
            bool successDir = true;
            while (successDir)
            {
                // Protester will pick a random new direction to move
                int randDir = rand() % 4;
                if (randDir == 0)
                {
                    if (!ProtestorCanMoveHere(getX()-1, getY()))
                        successDir = false;
                    else
                        setDirection(left);
                }
                else if (randDir == 1)
                {
                    if (!ProtestorCanMoveHere(getX()+1, getY()))
                        successDir = false;
                    else
                        setDirection(right);
                }
                else if (randDir == 2)
                {
                    if (!ProtestorCanMoveHere(getX(), getY()+1))
                        successDir = false;
                    else
                        setDirection(up);
                }
                else if (randDir == 3)
                {
                    if (!ProtestorCanMoveHere(getX(), getY()-1))
                        successDir = false;
                    else
                        setDirection(down);
                }
                // pick a new numSquaresToMoveInCurrentDirection that is between 8~60
                randomGenerateMoveInCurrentDir();
            }
        }
        else
        {
            // the Protester will then attempt to take one step in its currently
            // facing direction
            if (getDirection() == left && ProtestorCanMoveHere(getX()-1, getY()))
            {
                moveTo(getX()-1, getY());
                decNumSquaresToMoveInCurrentDir();
            }
            else if (getDirection() == right && ProtestorCanMoveHere(getX()+1, getY()))
            {
                moveTo(getX()+1, getY());
                decNumSquaresToMoveInCurrentDir();
            }
            else if (getDirection() == up && ProtestorCanMoveHere(getX(), getY()+1))
            {
                moveTo(getX(), getY()+1);
                decNumSquaresToMoveInCurrentDir();
            }
            else if (getDirection() == down && ProtestorCanMoveHere(getX(), getY()-1))
            {
                moveTo(getX(), getY()-1);
                decNumSquaresToMoveInCurrentDir();
            }
            else
            {
                resetNumSquaresToMoveInCurrentDir();
            }
        }
    }
}


void RegularProtestor::annoyProtestor(int points)
{
    decreaseHP(points);
    if (getHP() <= 0 && getDiedFromBoulder())
    {
        // Protestor died from falling Boulder
        getStudentWorld()->increaseScore(500);
        setProtestorState(true);
        getStudentWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        return;
    }
    else if (getHP() <= 0 && !getDiedFromBoulder())
    {
        // Protestor died from squirt water
        getStudentWorld()->increaseScore(100);
        // Protestor needs to leave the field
        setProtestorState(true);
        getStudentWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        return;
    }
    else
    {
        // Protestor not dead yet
        getStudentWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        setStunned(true);
    }
}





//----------------------------------
// HardcoreProtestor class
HardcoreProtestor::HardcoreProtestor(StudentWorld* sw, int x, int y)
: Protestor(sw, x, y, TID_HARD_CORE_PROTESTER)
{
    setHP(20);
}

HardcoreProtestor::~HardcoreProtestor() {}

void HardcoreProtestor::protestorPicksGold()
{
    getStudentWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getStudentWorld()->increaseScore(50);
    setStunned(true);
}

void HardcoreProtestor::doSomething()
{
    int TMX = Actor::getStudentWorld()->getTunnelMan()->getX();
    int TMY = Actor::getStudentWorld()->getTunnelMan()->getY();
    
    // Protestor is dead, shouldn't be moving
    if (isDead())
        return;
    // Protestor is in a rest state, shouldn't be moving
    if (getTicksPassed() < countTicksToWaitBetweenMoves())
    {
        increaseTicks();
        return;
    }
    // Protestor is stunned by water
    if (isProtestorStunned())
    {
        if (getStunnedTicks() < howLongToWaitWhenStunned())
        {
            // still needs to be stunned
            increaseStunnedTicks();
            return;
        }
        else
        {
            resetStunnedTicks();
            setStunned(false);
        }
    }
    
    // Protestor has hp = 0 and should leave the oil field
    if (getActorState())
    {
        if (getX() == 60 && getY() == 60)
        {
            setDead(true);
            setVisible(false);
            getStudentWorld()->decProtestorsInGameRn();
            return;
        }
        else
        {
            // go towards the exit
            getStudentWorld()->searchExit(this, 60, 60);
            getStudentWorld()->moveToExit(this);
            resetTicksPassed();
        }
    }
    // Protestor should keep wandering around the oil field
    else
    {
        resetTicksPassed();
        increaseShoutTicks();
        increaseTicksSinceTurned();
        // Protestor picks gold
        if (getStudentWorld()->GoldHereAndFromTM(getX(), getY(), 4))
        {
            protestorPicksGold();
            return;
        }
        // check if Protestor is hit by Boulder at the moment
        if (getStudentWorld()->BoulderHere(getX(), getY(), 3))
        {
            // protestor is bonked by Boulder
            // Protestor needs to leave the field
            setHP(0);
            annoyProtestor(0);
            setDiedFromBoulder();
            return;
        }
        
        // check if Protestor is near TunnelMan and can shout
        // TunnelMan is within the radius and Protestor hasn't shouted for a while
        if (getStudentWorld()->getDistance(getX(), getY(), TMX, TMY) <= 4)
        {
            if (canDirectlySeeTM(getDirection(), TMX, TMY) && noShoutForAWhile())
                protestorYells();
            return;
        }
        // check if Protestor is in a straight horizontal or vertical line of sight to the TunnelMan
        else if (canSeeTunnelMan())
        {
            resetNumSquaresToMoveInCurrentDir();
            // Protestor should run towards TunnelMan
            if (TMX < getX())
                setDirection(left);
            else if (TMX > getX())
                setDirection(right);
            else if (TMY < getY())
                setDirection(down);
            else if (TMY > getY())
                setDirection(up);
            
            // move towards TunnelMan
            if (getDirection() == left && ProtestorCanMoveHere(getX()-1, getY()))
                moveTo(getX()-1, getY());
            else if (getDirection() == right && ProtestorCanMoveHere(getX()+1, getY()))
                moveTo(getX()+1, getY());
            else if (getDirection() == up && ProtestorCanMoveHere(getX(), getY()+1))
                moveTo(getX(), getY()+1);
            else if (getDirection() == down && ProtestorCanMoveHere(getX(), getY()-1))
                moveTo(getX(), getY()-1);
            return;
        }
        // Can't directly see TunnelMan
        else if (getStudentWorld()->getDistance(TMX, TMY, getX(), getY()) > 4)
        {
            int M = 16 + getStudentWorld()->getLevel() * 2;
            if (getStudentWorld()->searchExit(this, TMX, TMY) < M)
            {
                getStudentWorld()->moveToExit(this);
                resetTicksPassed();
            }
        }
        
        // Protestor at an intersection
        if (getTicksSinceTurned() >= 200)
        {
            std::vector<Actor::Direction> perpPath;
            getStudentWorld()->atIntersection(getX(), getY(), getDirection(), perpPath);
            // if there is more than one possible direction, we randomly choose one direction
            if (perpPath.size() > 0)
            {
                int chooseDir = rand() % perpPath.size();
                setDirection(perpPath[chooseDir]);
                resetTicksSinceTurned();
            }
            // if no other Direction, we don't need to turn
        }
        
        // Protester has finished walking numSquaresToMoveInCurrentDirection steps in its
        // currently-selected direction
        if (getNumSquaresToMoveInCurrentDir() <= 0)
        {
            bool successDir = true;
            while (successDir)
            {
                // Protester will pick a random new direction to move
                int randDir = rand() % 4;
                if (randDir == 0)
                {
                    if (!ProtestorCanMoveHere(getX()-1, getY()))
                        successDir = false;
                    else
                        setDirection(left);
                }
                else if (randDir == 1)
                {
                    if (!ProtestorCanMoveHere(getX()+1, getY()))
                        successDir = false;
                    else
                        setDirection(right);
                }
                else if (randDir == 2)
                {
                    if (!ProtestorCanMoveHere(getX(), getY()+1))
                        successDir = false;
                    else
                        setDirection(up);
                }
                else if (randDir == 3)
                {
                    if (!ProtestorCanMoveHere(getX(), getY()-1))
                        successDir = false;
                    else
                        setDirection(down);
                }
                // pick a new numSquaresToMoveInCurrentDirection that is between 8~60
                randomGenerateMoveInCurrentDir();
            }
        }
        else
        {
            // the Protester will then attempt to take one step in its currently
            // facing direction
            if (getDirection() == left && ProtestorCanMoveHere(getX()-1, getY()))
            {
                moveTo(getX()-1, getY());
                decNumSquaresToMoveInCurrentDir();
            }
            else if (getDirection() == right && ProtestorCanMoveHere(getX()+1, getY()))
            {
                moveTo(getX()+1, getY());
                decNumSquaresToMoveInCurrentDir();
            }
            else if (getDirection() == up && ProtestorCanMoveHere(getX(), getY()+1))
            {
                moveTo(getX(), getY()+1);
                decNumSquaresToMoveInCurrentDir();
            }
            else if (getDirection() == down && ProtestorCanMoveHere(getX(), getY()-1))
            {
                moveTo(getX(), getY()-1);
                decNumSquaresToMoveInCurrentDir();
            }
            else
            {
                resetNumSquaresToMoveInCurrentDir();
            }
        }
    }
}

void HardcoreProtestor::annoyProtestor(int points)
{
    decreaseHP(points);
    if (getHP() <= 0 && getDiedFromBoulder())
    {
        // Protestor died from falling Boulder
        getStudentWorld()->increaseScore(500);
        setProtestorState(true);
        getStudentWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        return;
    }
    else if (getHP() <= 0 && !getDiedFromBoulder())
    {
        // Protestor died from squirt water
        getStudentWorld()->increaseScore(250);
        // Protestor needs to leave the field
        setProtestorState(true);
        getStudentWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        return;
    }
    else
    {
        // Protestor not dead yet
        getStudentWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        setStunned(true);
    }
}

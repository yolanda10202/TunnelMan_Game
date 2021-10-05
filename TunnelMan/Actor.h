#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <queue>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

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

class StudentWorld;
class SquirtWater; // just so TunnelMan knows this is another class
class GoldNuggets; // just so TunnelMan knows this is another class

// Actor class
class Actor : public GraphObject
{
public:
    // Actor constructor
    Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth);
    
    // Actor destructor
    virtual ~Actor();
    
    // Accessors
    // NAME: isDead
    // INPUT: NONE
    // OUTPUT: TRUE if Actor is dead, FALSE if Actor is alive
    // DESCRIPTION: Informs whether or not an Actor is dead.
    bool isDead();
    
    // NAME: getStudentWorld
    // INPUT: NONE
    // OUTPUT: Returns this StudentWorld the Actors are in
    // DESCRIPTION: Allows Actors to access public functions of the StudentWorld class
    StudentWorld* getStudentWorld();
    
    
    // Mutators
    // NAME: setDead
    // INPUT: TRUE to set Actor to dead, FALSE to make Actor alive
    // OUTPUT: NONE
    // DESCRIPTION: Allows us to set an Actor to dead or alive by passing in the correct
    // boolean parameter
    void setDead(bool deadOrNot);
    
    // Other Member functions
    // NAME: doSomething
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: This is a pure virtual function, will not be called. Its derived class
    // must have its own version of this function
    virtual void doSomething() = 0;
    
    // NAME: amIBoulder
    // INPUT: NONE
    // OUTPUT: Whether or not the Actor is a Boulder
    // DESCRIPTION: This is a virtual function. Only Boulder overrides this function by returning true instead.
    virtual bool amIBoulder();
    
    // NAME: amIProtestor
    // INPUT: NONE
    // OUTPUT: Whether or not the Actor is a Boulder
    // DESCRIPTION: This is a virtual function. Only Protestor overrides this function by returning true instead.
    virtual bool amIProtestor();
    
    // NAME: amIGold
    // INPUT: NONE
    // OUTPUT: Whether or not the Actor is a Boulder
    // DESCRIPTION: This is a virtual function. Only GoldNuggets overrides this function by returning true instead.
    virtual bool amIGold();
    
    // NAME: amIOilBarrel
    // INPUT: NONE
    // OUTPUT: TRUE if the Actor is, FALSE if otherwise
    // DESCRIPTION: Will check if this Actor is an OilBarrel
    virtual bool amIOilBarrel();
    
    // NAME: droppedByTM
    // INPUT: NONE
    // OUTPUT: TRUE if dropped by TM, FALSE if otherwise
    // DESCRIPTION: Returns whether or not the object is dropped by TunnelMan
    virtual bool droppedByTM();
    
    virtual bool getActorState();
    
    virtual void annoyProtestor(int points);
    
private:
    bool m_isDead;
    StudentWorld* m_studentWorld;
};


// Earth class
class Earth : public Actor
{
public:
    // Earth constructor
    Earth(StudentWorld* sw, int x, int y);
    
    // Earth destructor
    virtual ~Earth();
    
    // NAME: doSomething
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Doesn't do anything because it's just an Earth. Silly :P
    virtual void doSomething();
    
    // Accessors
    // NAME: isBoulderHere
    // INPUT: NONE
    // OUTPUT: Returns true if there is a Boulder at this (x, y)
    // DESCRIPTION: Each Earth object will have a variable, hasBoulder. This function returns that variable.
    bool isBoulderHere();
    
    // Mutators
    // NAME: setHasBoulder
    // INPUT: Whether or not theres a Boulder
    // OUTPUT: NONE
    // DESCRIPTION: Sets the Earth object to have a Boulder
    void setHasBoulder(bool thereIsBoulder);
private:
    bool hasBoulder;
};


// TunnelMan class
class TunnelMan : public Actor
{
public:
    // TunnelMan constructor
    TunnelMan(StudentWorld* sw, int x, int y);
    
    // TunnelMan destructor
    virtual ~TunnelMan();
    
    // Accessors
    // NAME: getHP
    // INPUT: NONE
    // OUTPUT: Returns m_hitPoints
    // DESCRIPTION: Allows access to get m_hitPoints
    int getHP();
    
    // NAME: getWater
    // INPUT: NONE
    // OUTPUT: The amount of water squirts available
    // DESCRIPTION: Allows access to get m_waterAmount
    int getWater();
    
    // NAME: getGold
    // INPUT: NONE
    // OUTPUT: The amount of gold nuggets the TM has
    // DESCRIPTION: Allows access to get m_goldAmount
    int getGold();
    
    // NAME:
    // INPUT: NONE
    // OUTPUT: The amount of sonar kit the TM has
    // DESCRIPTION: Allows access to get m_sonarAmount
    int getSonar();
    
    // Mutators
    // NAME: addWater
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Increases m_waterAmount by 5
    void addWater();
    
    // NAME: addGold
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Increases m_goldAmount by 1
    void addGold();
    
    // NAME: addSonar
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Increases m_sonarAmount by 1
    void addSonar();
    
    // NAME: decreaseHP
    // INPUT: points you want to decrease
    // OUTPUT: NONE
    // DESCRIPTION: Decreases m_hitPoints by the points passed in
    void decreaseHP(int points);
    
    // NAME: doSomething
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Let TunnelMan move and do what it should during each tick
    virtual void doSomething();
    
private:
    int m_waterAmount;
    int m_goldAmount;
    int m_sonarAmount;
    int m_hitPoints;
};


// Boulder class
class Boulder : public Actor
{
public:
    // something to keep track of Boulder's state
    enum BoulderState { stable, waiting, falling };
    
    // Boulder constructor
    Boulder(StudentWorld* sw, int x, int y);
    
    // Boulder destructor
    virtual ~Boulder();
    
    // Accessors
    // NAME: getState
    // INPUT: NONE
    // OUTPUT: The state of the Boulder
    // DESCRIPTION: Returns the current state of the Boulder
    BoulderState getState();
    
    // NAME: doSomething
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Let Boulder move and do what it should during each tick
    virtual void doSomething();
    
    // NAME: setState
    // INPUT:
    // OUTPUT:
    // DESCRIPTION:
    void setState(BoulderState curState);
    
    // NAME: fallingBoulder
    // INPUT: the (x, y) of the Boulder
    // OUTPUT: NONE
    // DESCRIPTION: Will let the Boulder fall when it is in a falling state
    void fallingBoulder(int x, int y);
    
    // NAME:
    // INPUT:
    // OUTPUT: TRUE if the Boulder can move to this (x, y)
    //         FALSE if the Boulder can't move to this (x, y)
    // DESCRIPTION: checks to see if a Boulder can move to this (x, y)
    bool canMove(int x, int y);
    
    // NAME: amIBoulder
    // INPUT: NONE
    // OUTPUT: returns TRUE because this is a Boulder
    // DESCRIPTION: Overrides the amIBoulder function from Actor.
    // Will always return true.
    virtual bool amIBoulder() { return true; }
    
private:
    BoulderState m_state;
    int tickCount; 
};


// SquirtWater Class
class SquirtWater : public Actor
{
public:
    // SquirtWater constructor
    SquirtWater(StudentWorld* sw, int TunnelManX, int TunnelManY, Direction TunnelManDir);
    
    // Member functions
    // NAME: doSomething
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Let Squirt do something during each tick, if it is called
    virtual void doSomething();
    
    // NAME: canMove
    // INPUT: the (x, y) and Direction you want to check
    // OUTPUT: TRUE if water can move to this position
    //         FALSE if water can't move to this position
    // DESCRIPTION: checks to see if water can move to the position you want, given direction.
    bool canMove(int squirtX, int squirtY, Direction squirtDir);
private:
    TunnelMan* m_tunnelMan;
    int moved;
};


class Items : public Actor
{
public:
    // Items constructor
    Items(StudentWorld* sw, int imageID, int startX, int startY);
    
    // Items destructor
    virtual ~Items();
    
    // NAME: getTicks
    // INPUT: NONE
    // OUTPUT: returns m_ticks
    // DESCRIPTION: allows access to the number of ticks at the current moment
    int getTicks();
    
    // NAME: increaseTicks
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: increases m_ticks by 1
    void increaseTicks();
    
private:
    int m_ticks;
};


class GoldNuggets : public Items
{
public:
    GoldNuggets(StudentWorld* sw, int x, int y);
    virtual ~GoldNuggets();
    
    // NAME: doSomething
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Let GoldNuggets do what it should do during a tick
    virtual void doSomething();
    
    // NAME: droppedByTM
    // INPUT: NONE
    // OUTPUT: Returns isFromTM
    // DESCRIPTION: Tells us whether or not the current existing GoldNuggets is dropped by the TunnelMan or not.
    bool droppedByTM();
    
    // Mutator
    // NAME: setDroppedByTM
    // INPUT: TRUE or FALSE
    // OUTPUT: NONE
    // DESCRIPTION: Will set isFromTm to whatever boolean value passed in
    void setDroppedByTM(bool TMDropGold);
    
    // NAME: amIGold
    // INPUT: NONE
    // OUTPUT: Always TRUE
    // DESCRIPTION: Will always return true because this is a GoldNugget
    virtual bool amIGold();
    
private:
    bool isFromTM;
};


class OilBarrels : public Items
{
public:
    // constructor
    OilBarrels(StudentWorld* sw, int x, int y);
    
    // destructor
    virtual ~OilBarrels();
    
    // NAME:
    // INPUT:
    // OUTPUT:
    // DESCRIPTION:
    virtual void doSomething();

    // NAME: amIOilBarrel
    // INPUT: NONE
    // OUTPUT: Always true
    // DESCRIPTION: overrides the false and returns true because this is always an OilBarrel object
    virtual bool amIOilBarrel();
    
};

class SonarKit : public Items
{
public:
    // constructor
    SonarKit(StudentWorld* sw, int x, int y);
    
    // destructor
    virtual ~SonarKit();
    
    // NAME: doSomething
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Let SonarKit do something during a tick
    virtual void doSomething();
    
};


class WaterPool : public Items
{
public:
    // WaterPool constructor
    WaterPool(StudentWorld* sw, int x, int y);
    
    // WaterPool destructor
    virtual ~WaterPool();
    
    // NAME: doSomething
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Will let WaterPool doSomething during a tick
    virtual void doSomething();

};


class Protestor : public Actor
{
public:
    // Protestor constructor
    Protestor(StudentWorld* sw, int x, int y, int protestorImage);
    
    // Protestor destructor
    virtual ~Protestor();
    
    // Member functions
    // NAME: doSomething
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: This is a pure virtual function
    virtual void doSomething() = 0;
    
    // TICKS FOR MOVING
    // NAME: countTicksToWaitBetweenMoves
    // INPUT: NONE
    // OUTPUT: The number of ticks a Protestor should wait
    // DESCRIPTION: returns the number of ticks a Protestor should wait
    int countTicksToWaitBetweenMoves();
    
    // NAME: increaseTicks
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: increase the ticksPassed, for moving
    void increaseTicks();
    
    // NAME: getTicksPassed
    // INPUT: NONE
    // OUTPUT: returns ticksPassed
    // DESCRIPTION: get ticksPassed, for moving
    int getTicksPassed();
    
    // NAME: resetTicksPassed
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: reset ticksPassed, ticks for moving
    void resetTicksPassed();
    
    
    
    // FUNCTIONS FOR PROTESTOR YELLING
    // NAME: noShoutForAWhile
    // INPUT: NONE
    // OUTPUT: TRUE if Protestor hasn't shouted for a while and can shout
    //         FALSE if otherwise
    // DESCRIPTION: get m_ticksSinceShouted for shouting
    bool noShoutForAWhile();
    
    // NAME: protestorYells
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Will let Protestor yell at TunnelMan when TunnelMan is too close
    void protestorYells();
    
    // NAME: increaseShoutTicks
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: increases m_ticksSinceShouted by 1
    void increaseShoutTicks();
    
    
    
    // TICKS FOR TURNING
    // NAME: getTicksSinceTurned
    // INPUT: NONE
    // OUTPUT: returns ticksSinceTurned
    // DESCRIPTION: will allow access to ticksSinceTurned
    int getTicksSinceTurned();
    
    // NAME: resetTicksSinceTurned
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: will reset ticksSinceTurned
    void resetTicksSinceTurned();
    
    // NAME: increaseTicksSinceTurned
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: will increase ticksSinceTurned by 1
    void increaseTicksSinceTurned();
    
    
    
    // FUNCTIONS ABOUT HP
    // NAME: getHP
    // INPUT: NONE
    // OUTPUT: returns the Protestor's HP
    // DESCRIPTION: will allow access to Protestor's HP
    int getHP();
    
    // NAME: decreaseHP
    // INPUT: points we want to decrease
    // OUTPUT: NONE
    // DESCRIPTION: will decrease m_hp by points
    void decreaseHP(int points);
    
    // NAME: setHP
    // INPUT: hp we want it to be
    // OUTPUT: NONE
    // DESCRIPTION: we set a Protestor's m_hp to hp
    void setHP(int hp);
    
    
    
    // FUNCTIONS FOR PROTESTOR STUNNED
    // NAME: isProtestorStunned
    // INPUT: NONE
    // OUTPUT: Whether or not the Protestor is stunned
    // DESCRIPTION: returns isStunned
    bool isProtestorStunned();
    
    // NAME: setStunned
    // INPUT: Whether or not the Protestor is stunned
    // OUTPUT: NONE
    // DESCRIPTION: will set isStunned to stunned
    void setStunned(bool stunned);
    
    // NAME: howLongToWaitWhenStunned
    // INPUT: NONE
    // OUTPUT: the ticks a protestor needs to wait
    // DESCRIPTION: Will calculate how long a Protestor needs to wait when it is stunned
    int howLongToWaitWhenStunned();
    
    // NAME: increaseStunnedTicks
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: will increase stunnedTicks by 1
    void increaseStunnedTicks();
    
    // NAME: getStunnedTicks
    // INPUT: NONE
    // OUTPUT: Returns stunnedTicks
    // DESCRIPTION: allows access to stunnedTicks
    int getStunnedTicks();
    
    // NAME: resetStunnedTicks
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: will set stunnedTicks to 0
    void resetStunnedTicks();
    
    
    // FUNCTIONS FOR NUM-SQUARES-TO-MOVE
    // NAME: getNumSquaresToMoveInCurrentDir
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: will allow access to numSquares
    int getNumSquaresToMoveInCurrentDir();
    
    // NAME: decNumSquaresToMoveInCurrentDir
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: will decrease numSquares by 1
    void decNumSquaresToMoveInCurrentDir();
    
    // NAME: resetNumSquaresToMoveInCurrentDir
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: will set numSquares to 0
    void resetNumSquaresToMoveInCurrentDir();
    
    // NAME: randomGenerateMoveInCurrentDir
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: will generate number of squares a Protestor need to move in the current direction randomly
    void randomGenerateMoveInCurrentDir();
    
    
    
    // OTHER FUNCTIONS
    // NAME: canSeeTunnelMan
    // INPUT: NONE
    // OUTPUT: TRUE if can see TM, FALSE if otherwise
    // DESCRIPTION: Will check if Protestor can see TunnelMan from a straight distance
    bool canSeeTunnelMan();
    
    // NAME: protestorPicksGold
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: this is a pure virtual function
    virtual void protestorPicksGold() = 0;
    
    // NAME: getProtestorState
    // INPUT: NONE
    // OUTPUT: TRUE if Protestor is at a leaving state, FALSE if otherwise
    // DESCRIPTION:
    virtual bool getActorState();
    
    // NAME: setProtestorState
    // INPUT: The state of Protestor
    // OUTPUT: NONE
    // DESCRIPTION: will set protestorState to state
    void setProtestorState(bool state);
    
    // NAME: annoyProtestor
    // INPUT: Number of points we want to dec
    // OUTPUT: NONE
    // DESCRIPTION: this is a pure virtual function
    virtual void annoyProtestor(int points);

    
    // NAME: canDirectlySeeTM
    // INPUT: Direction of Protestor, and TM (x, y)
    // OUTPUT: TRUE if TM is at shouting distance, FALSE if otherwise
    // DESCRIPTION: Will check if TM is at a shouting distance between Protestor
    bool canDirectlySeeTM(Direction ProtestorDir, int TMX, int TMY);    

    // NAME: ProtestorCanMoveHere
    // INPUT: protestor (x, y)
    // OUTPUT: TRUE if Protestor can move to this position, FALSE if otherwise
    // DESCRIPTION: will check if Protestor can move to this (x, y)
    bool ProtestorCanMoveHere(int x, int y);
    
    // NAME: amIProtestor
    // INPUT: NONE
    // OUTPUT: Always TRUE
    // DESCRIPTION: will always return true because this is a Protestor object
    virtual bool amIProtestor();

    // NAME: getDiedFromBoulder
    // INPUT: NONE
    // OUTPUT: TRUE if Protestor died from a Boulder, FALSE if otherwise or isn't dead yet
    // DESCRIPTION: will check if Protestor is dead because it got hit by a boulder
    bool getDiedFromBoulder();
    
    // NAME: setDiedFromBoulder
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: sets diedFromtBoulder to true
    void setDiedFromBoulder();
    
private:
    int m_hp;
    int ticksPassed;
    int m_ticksSinceShouted;
    int ticksAfterStunned;
    int numSquaresToMoveInCurrentDirection;
    int ticksSinceTurned;
    bool isStunned;
    bool leaveTheOilField;
    char oilField[64][64];
    bool diedFromBoulder;
    // some queue for maze solving
};

class RegularProtestor : public Protestor
{
public:
    // RegularProtestor constructor
    RegularProtestor(StudentWorld* sw, int x, int y);
        
    // RegularProtestor destructor
    virtual ~RegularProtestor();
    
    // NAME: doSomething
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: will let a RegularProtestor do something during a tick
    virtual void doSomething();
    
    // NAME: protestorPicksGold
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: will let RegularProtestor pick up gold that is dropped by TM
    virtual void protestorPicksGold();
    
    // NAME: annoyProtestor
    // INPUT: points we want to dec
    // OUTPUT: NONE
    // DESCRIPTION: when RegularProtestor is annoyed, this will be called to update RegularProtestor stats
    virtual void annoyProtestor(int points);
    
private:
    int m_HitPoints;
};

class HardcoreProtestor : public Protestor
{
public:
    // HardcoreProtestor constructor
    HardcoreProtestor(StudentWorld* sw, int x, int y);
    
    // HardCoreProtestor destructor
    virtual ~HardcoreProtestor();
    
    // NAME: doSomething
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: will let a HardCoreProtestor do something during a tick
    virtual void doSomething();
    
    // NAME: protestorPicksGold
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: will let HardCoreProtestor pick up gold that is dropped by TM, will not let HCP leave the oil field
    virtual void protestorPicksGold();
    
    // NAME: annoyProtestor
    // INPUT: points to dec
    // OUTPUT: NONE
    // DESCRIPTION: when RegularProtestor is annoyed, this will be called to update RegularProtestor stats
    virtual void annoyProtestor(int points);
    
};

#endif // ACTOR_H_

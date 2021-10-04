#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

struct square
{
    square(int x, int y);
    int m_x;
    int m_y;
};

class StudentWorld : public GameWorld
{
public:
    // constructor
    StudentWorld(std::string assetDir);
    
    // StudentWorld destructor
    // Destructor is still needed in cases where we end the game early
    // (times when cleanUp() won't be called)
    virtual ~StudentWorld();
    
    // NAME: init
    // INPUT: NONE
    // OUTPUT: Returns the Game Status (CONTINUE_GAME)
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
    virtual int init();

    // NAME: move
    // INPUT: NONE
    // OUTPUT: Returns the Game Status
    // DESCRIPTION: Responsible for asking each of the game’s actors to do something if they
    // are not dead yet.
    virtual int move();

    // NAME: cleanUp
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Responsible for freeing all actors that are currently active in the game.
    // Is called by our framework when the player loses a life, or the player
    // successfully completes the current level.
    virtual void cleanUp();
    
    // NAME: getEarth
    // INPUT: The x and y of the Earth object you want to access
    // OUTPUT: This will return EarthPtr[x][y]
    // DESCRIPTION: Accessor to access EarthPtr. This will return EarthPtr[x][y]
    Earth* getEarth(int x, int y);
    // Accessor to access TunnelMan
    TunnelMan* getTunnelMan();

    // NAME: setSquirt
    // INPUT: The state you want m_squirt to be. Either TRUE (TunnelMan squirts water) or
    // FALSE (TunnelMan didn't squirt water).
    // OUTPUT: True (TunnelMan squirts water) or false (TunnelMan didn't squirt water).
    // DESCRIPTION: Change squirt state. Need this to let move() know if it should create a squirt object. Will be called when Player presses SPACEBAR during the game.
    void setSquirt(bool squirt);
    
    // NAME: getSonarTicks
    // INPUT: NONE
    // OUTPUT: Returns an int that represents the required ticks for Sonars to last in this
    // particular level.
    // DESCRIPTION: Accessor to get the number of ticks required for sonar.
    int getSonarTicks();
    
    // NAME: getWaterTicks
    // INPUT: NONE
    // OUTPUT: Returns an int that represents the required ticks for WaterPools to last in
    // this particular level.
    // DESCRIPTION: Accessor to get the number of ticks required for WaterPool.
    int getWaterTicks();
    
    // NAME: overlap
    // INPUT: The x and y of TunnelMan's current position
    // OUTPUT: Whether or not TunnelMan is digging earth.
    // DESCRIPTION: Checks if TunnelMan is digging (overlapping with existing Earth
    // object) the Earth. Removes Earth objects if TunnelMan is indeed digging earth.
    bool overlap(int TunnelManX, int TunnelManY);
    
    // NAME: earthAt
    // INPUT: x and y of a location you want to check
    // OUTPUT: TRUE if there is Earth at (x, y)
    //         FALSE if there is no Earth at (x, y)
    // DESCRIPTION: Checks if there is Earth at (x, y)
    bool earthAt(int x, int y);
    
    // NAME: getDistance
    // INPUT: Two sets of (x, y)
    // OUTPUT: Returns the distance between these two points
    // DESCRIPTION: Get the distance between two objects (euclidean distance)
    double getDistance (int x1, int y1, int x2, int y2);
    
    // NAME: clearCircle
    // INPUT: Center (x, y) and a radius r
    // OUTPUT: TRUE if the circle is clear of objects
    //         FALSE if the circle is not clear of objects
    // DESCRIPTION: Sees if within the radius, there's anything within
    bool clearCircle(int x, int y, double r);
    
    // NAME: has4x4Square
    // INPUT: a location (x, y)
    // OUTPUT: TRUE if within x~x+4 and y~y+4 there is no earth/object
    //         FALSE if otherwise
    // DESCRIPTION: Finds a 4x4 empty square so we can put WaterPool there
    bool has4x4Square(int x, int y);
    
    // NAME: addOilBarrels
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Increases the m_barrels private member in StudentWorld private by 1.
    void addOilBarrels();
    
    // NAME: dropGold
    // INPUT: Location of where the gold should be dropped
    // TunnelMan should drop Gold at it's position
    // OUTPUT: NONE
    // DESCRIPTION: When the Player presses TAB, TunnelMan will drop a GoldNugget object for
    // the protestors to pickup. This function helps the TunnelMan drop gold.
    void dropGold(int x, int y);
    
    // NAME: BouldHere
    // INPUT: position (x, y) and distance between object and Boulder
    // OUTPUT: TRUE if Boulder is there, FALSE otherwise
    // DESCRIPTION: Will check if an Actor is near Boulder
    bool BoulderHere(int x, int y, int r);
    
    // NAME: ProtestorHere
    // INPUT: position (x, y) and distance between object and Protestor
    // OUTPUT: TRUE if Protestor is there, FALSE otherwise
    // DESCRIPTION: Will check if an Actor is near Protestor
    bool ProtestorHere(int x, int y, int r);
    
    // NAME: GoldHereAndFromTM
    // INPUT: position (x, y) and distance between object and GoldNugget
    // OUTPUT: TRUE if Gold is there, FALSE otherwise
    // DESCRIPTION: Will check if an Actor is near a GoldNugget dropped by TM
    bool GoldHereAndFromTM(int x, int y, int r);
    
    // NAME: atIntersection
    // INPUT: position (x, y) and Protestor direction and a vector to store directions possible
    // OUTPUT: a vector containing the directions
    // DESCRIPTION: Will determine if a Protestor has possible "turns" to make
    std::vector<Actor::Direction> atIntersection(int x, int y, Actor::Direction ProtestorDir, std::vector<Actor::Direction> &temp);
        
    // NAME: formatText
    // INPUT: All the things we want to print out
    // OUTPUT: Will create a string that can be passed into setGameStatText
    // DESCRIPTION: Uses stringstream and setw to format the string nicely
    std::string formatText(int level, int lives, int health, int water, int gold, int sonar, int score, int oilLeft);
    
    // NAME: use Sonar
    // INPUT: position (x, y)
    // OUTPUT: NONE
    // DESCRIPTION: When the player uses sonar item, it will search Actors around and set it visible for the player
    void useSonar(int x, int y);
    
    // NAME: searchExit
    // INPUT: The protestor that is leaving and the destination (x, y)
    // OUTPUT:
    // DESCRIPTION:
    int searchExit(Protestor* exitPro, int endX, int endY);
    
    // NAME: moveToExit
    // INPUT: A Protestor pointer (the Protestor you want to move)
    // OUTPUT: NONE
    // DESCRIPTION: Will let the Protestor move to the desired destination according to the map drawn in searchExit()
    void moveToExit(Protestor* exitPro);
    
    // NAME: decProtestorsInGameRn
    // INPUT: NONE
    // OUTPUT: NONE
    // DESCRIPTION: Will update the number of Protestors currently in the game if a Protestor dies or leaves
    void decProtestorsInGameRn();
    
private:
    TunnelMan* m_TunnelMan;
    Earth* EarthPtr[64][64];
    std::vector<Actor*> m_actors;
    bool m_squirt = false;
    int sonarTicks;
    int waterTicks;
    int m_barrels;
    int ticksLastProtestor;
    int ProtestorsInGameRn;
    int field[64][64];
};

#endif // STUDENTWORLD_H_

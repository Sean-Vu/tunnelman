#ifndef ACTOR_H_
#define ACTOR_H_


#include "GraphObject.h"
#include "GameWorld.h"
#include <stack>
#include <queue>


//A class that can serve as the base class for all of your game’s objects (e.g.,
//the TunnelMan, Regular Protesters, Hardcore Protesters, Barrels of oil,
//Nuggets, Earth, etc.):

class Actor: public GraphObject //It must be derived from our GraphObject class
{
public:
	//It must have a simple constructor and destructor.
	//GraphObject(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
	Actor(GameWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
	virtual ~Actor();
	// It must have a virtual method called doSomething() that can be
	//called by the World to get one of the game’s actors to do
	//something.
	//v.You may add
	virtual void doSomething() = 0;
	//virtual bool isAlive() = 0;
	virtual void annoy(int x);
	bool isTunnelManOverlapEarth(Actor* object);
	bool areYouVisible();
	bool isTunnelManClose();
	bool isTunnelManOverlap();
	virtual void setDead();
	virtual bool isAlive();
	GameWorld* getWorld();
private:
	GameWorld* m_studentWorld;
	bool isAlives;

};
/*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*/
class TunnelMan : public Actor
{
public:
	TunnelMan(GameWorld* world);
	virtual ~TunnelMan();
	virtual void annoy(int x);
	virtual void doSomething();
	int getHP();
	int getSquirts();
	int getGold();
	int getSonarKits();
	//void setDead();
	void addGold();
	void addSonarKit();
	void addWater();
	bool isAlive();
	
private:
	//bool isAlives;
	int m_hp;
	int water;
	int sonarkit;
	int goldnuggets;
};
/*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*/
class Earth : public Actor
{
public:
	Earth(GameWorld* world, int startX, int startY);
	virtual ~Earth();
	virtual void doSomething();
	virtual bool isAlive(); // useless
	//virtual void annoy(int x); // useless
	void setDead();
	// Annoyed function
private:
	bool isAlives;
};
/*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*/
class Boulder : public Actor
{
public:
	Boulder(GameWorld* world, int startX, int startY);
	void moveFallingBoulder();
	virtual void doSomething();
	//virtual bool isAlive();
	//virtual void annoy(int x);
	bool getFallingState();
	bool isBoulderOverlap(int startX, int startY);
	virtual ~Boulder();
private:
	//bool isAlives;
	int numTicks;
	int state;
	bool alreadyIncreasedScore;
	// 0 is stable
	// 1 is waiting
	// 2 is falling

};
/*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*/
class Squirt : public Actor
{
public:
	Squirt(GameWorld* world, int startX, int startY, Direction dir);
	virtual void doSomething();
	virtual ~Squirt();
	//virtual bool isAlive();
	//virtual void annoy(int x);
	//void setDead();
private:
	int travelDistance;
	int currentDistance;
	//bool isAlives;
	Direction direction;
};
/*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*/
class Barrel : public Actor
{
public:
	Barrel(GameWorld* world, int startX, int startY);
	virtual void doSomething();
	virtual ~Barrel();
	//virtual bool isAlive();
	//virtual void annoy(int x);
	//void setDead();
	//bool areYouVisible();
	//bool isTunnelManClose();
	//bool isTunnelManOverlap();
private:
	//bool isAlives;
};
/*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*/
class GoldNugget: public Actor
{
public:
	GoldNugget(GameWorld* world, int startX, int startY);
	GoldNugget(GameWorld* world, TunnelMan* tunnelManPtr, int startX, int startY);
	~GoldNugget();
	virtual void doSomething();
	//virtual bool isAlive();
	//virtual void annoy(int x);
	//void setDead();
	
private:
	//bool isAlives;
	bool canTunnelManPickUp; // will be pick-up able if game creates it but not when tunnelman creates it
	bool canProtesterPickUp; // will be pick-up able if tunnelman creates it but not when game creates it 
	int state; // 0 is a permanent state, 1 is a temporary state
	int tickCounter;
};
/*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*/
class SonarKit: public Actor
{
public:
	SonarKit(GameWorld* world, int startX, int startY, int imageID);
	~SonarKit();
	virtual void doSomething();
	//virtual void annoy(int x);
	//void setDead();

private:
	int tickCounter;
	int T;
	//bool isAlives;
};
/*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*/
class WaterPool: public SonarKit
{
public:
	WaterPool(GameWorld* world, int startX, int startY);
	~WaterPool();
	//virtual void annoy(int x);
	//void setDead();
};
/*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*/

class Protestor: public Actor
{
public:
	Protestor(GameWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
	~Protestor();
	virtual void doSomething();
	//virtual bool isAlive();
	virtual void annoy(int x);
	bool isTunnelManOverlap();
	//virtual void setDead();
	int calculateNumSquares();
	int ticksToWait();
	bool leaveOilField();
	bool tunnelManWithinFourUnits();
	bool isProtestorFacingTunnelMan();
	bool canSeeTunnelMan();
	void BFS(std::stack<int>& p, int startX, int startY, int endX, int endY);
	bool outOfBounds(int startX, int startY);
	virtual void bribe(); // redefined in derived classe 
	void boulderHitProtestor();
	void playAnnoySound();
	void gotSquirted(); //voi redefined in derived classes 
	void setHP(int x);
	//virtual void search() = 0; // different for both hardcore and regular 
private:
	int m_hp;
	//bool isAlives;
	int numSquaresToMoveInCurrentDirection;
	bool leaveTheOilFieldState; // false = NOT , true = LEAVE
	int ticksToWaitBetweenMoves;
	int currentTick;
	int nonRestingTick;
	int tickSinceLastPerpendicularTurn;
	bool isBribed;
	bool hasBoulderHit;
	int stunTimer;
	bool playOnceGiveUpOnce;

	std::stack<int> path;
	//1 is up
	//2 is down
	//3 is right
	//4 is left
};
/*//*//*//*//*//*//*//*//*//*//*//*//*//*//*//*//*//*/

class RegularProtestor : public Protestor
{
public:
	RegularProtestor(GameWorld* world, int startX, int startY);
	~RegularProtestor();
	virtual void doSomething();
private:	
};
/*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*//*/*/


class HardCoreProtestor: public Protestor
{
public:
	HardCoreProtestor(GameWorld* world, int startX, int startY);
	~HardCoreProtestor();
	virtual void doSomething();
	//void search();
	void bribe();
private:
	int ticks_to_stare;
};

#endif // ACTOR_H_

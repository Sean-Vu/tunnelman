#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"

#include <string>
#include <vector>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	~StudentWorld();

	void setDisplayText();

	//void incrementScore(int x);

	void removeEarth(Actor* object);

	bool earthUnderBoulder(Actor* object);

	bool overlappedWithBoulder(int startX, int startY);
	
	bool overlappedWithPlayer(int thisX, int thisY);
	
	bool collidedWithBoulder(int startX, int startY);

	bool collidedWithEarth(int startX, int startY);

	bool isEarthOrBoulderBlocking(int startX, int startY);

	TunnelMan* getTunnelMan();

	Protestor* getProtestor(Actor* thisActor);

	bool isBoulderFallingAndCollided(int startX, int startY);
	
	void addObject(Actor* newObject);

	int getBarrels();

	bool isAddable(int startX, int startY);

	void addBoulder();

	void addBarrel();

	void addGold();

	void addSonar();

	void addWaterPool();

	void addGoodie();

	bool ableToAddWaterPool(int startX, int startY);

	void illuminateSonar(Actor* object);

	virtual int init();

	virtual int move();

	virtual void cleanUp();

private:
	// need one vector of regular actors like Regular Protesters, Gold Nuggets, Barrels of oil, Sonar Kits , Boulders
	std::vector<Actor*> objects; // boulders, items, objects, etc.
	Earth* earth[64][64];
	// need another separate pointer to TunnelMan
	TunnelMan* TunnelManPtr;
	int T;
	int probabilityOfHardCore;
};

#endif // STUDENTWORLD_H_

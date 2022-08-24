#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{
	for (int i = 0; i < 64; ++i) // x coord 64 columns
	{
		for (int j = 0; j < 64; ++j) // y coord 60 rows
		{
			earth[i][j] = nullptr;
		}
	}
}
Protestor* StudentWorld::getProtestor(Actor* thisActor)
{
	// loop through all actor objects 
	auto it = objects.begin();
	while (it != objects.end())
	{
		// loop through only protestor objects
		if ((*it)->getID() == TID_PROTESTER || (*it)->getID() == TID_HARD_CORE_PROTESTER)
		{
			if (((thisActor->getX() + 3 >= (*it)->getX()) && (thisActor->getX() <= (*it)->getX() + 3)))
			{
				if (((thisActor->getY()) + 3 >= (*it)->getY()) && (thisActor->getY() <= (*it)->getY() + 3))
				{
					// upcast the actor pointer to protestor pointer
					//cout << "Hit" << endl;
					return dynamic_cast<Protestor*>(*it);
				}
			}
		}
		++it;
	}
	// return null if there was no protestor that overlapped with actor object
	return NULL;
}
StudentWorld::~StudentWorld()
{

}
int StudentWorld::getBarrels()
{
	int numBarrels = 0;
	auto it = objects.begin();
	while (it != objects.end())
	{
		if ((*it)->getID() == TID_BARREL)
		{
			++numBarrels;
		}
		++it;
	}
	return numBarrels;
}

void StudentWorld::setDisplayText()
{
	int level = getLevel();
	int lives = getLives();
	int health = getTunnelMan()->getHP() * 10;
	int squirts = getTunnelMan()->getSquirts();
	int gold = getTunnelMan()->getGold();
	int getBarrel = getBarrels();
	int sonar = getTunnelMan()->getSonarKits();
	int score = getScore();
	int percent = health * 10;

	ostringstream oss;
	oss << "Scr: ";
	oss << setw(6) << setfill('0') << score; // score is 6 digits long
	oss << "  "; // space
	oss << "Lvl: ";
	//oss << " " ; // space
	oss << setw(1) << level;
	oss << "  " ; // space
	oss << "Lives: ";
	//oss << " "; // space
	oss << setw(1) << lives;
	oss << "  "; // space  
	oss << "Hlth: ";
	//oss << " " ; // space
	oss << setw(3) << setfill(' ') << health;
	oss << "%";
	oss << "  " ; // space
	oss << "Wtr: ";
	oss << setw(2) << setfill(' ') << squirts;
	oss << " " ; // space
	oss << "Gld: ";
	oss << setw(2) << setfill(' ')  << gold;
	oss << " "; // space
	oss << "Sonar: ";
	oss << setw(2) << setfill(' ') << sonar;
	oss << " "; // space
	oss << "Oil left: ";
	oss << setw(2) << setfill(' ') << getBarrel;
	oss << " "; // space
	
	string text = oss.str();


	/*
	std::string text = ("Lvl: " + to_string(level) + " Lives: " + to_string(lives) + " Hlth: " + to_string(percent) + "% Wtr: " + to_string(squirts) + " Gld: "
		+ to_string(gold) + " Oil Left: " + to_string(getBarrel) + " Sonar: " + to_string(sonar) + " Scr: " + to_string(score));
	*/
	setGameStatText(text);

}
int StudentWorld::init()
{
	int level = getLevel();
	int otherT = 200 - getLevel();
	T = 0;
	probabilityOfHardCore = min(90, level * 10 + 30);

	TunnelManPtr = new TunnelMan(this); // create a TunnelMan object and insert it into the oil field at the right starting location
	// fill  the oil field with earth objects
	
	// intialize object vector and fill it

	for (int i = 0; i <= 63; ++i) // x coord 64 columns
	{
		for (int j = 0; j <= 59; ++j) // y coord 60 rows
		{
			if (i >= 30 && i <= 33 && j >= 4 && j <= 59) // skips middle column (x, y coordinate ex. (30,4) we skip)
			{
				continue;
			}
			earth[i][j] = new Earth(this, i, j);
		}
	}
	// add boulders
	//No distributed game object may be within a radius (Euclidian distance) of 6 squares of
	//any other distributed game object.
	int B = min((level / 2) + 2, 9);
	for (int i = 0; i < B; ++i)
	{
		addBoulder();
	}
	// add barrels
	int L = min(2 + level, 21);
	for (int i = 0; i < L; ++i)
	{
		addBarrel();
	}
	// add gold
	int G = max(5 - level / 2, 2);
	for (int i = 0; i < G; ++i)
	{
		addGold();
	}
	//objects.push_back(new RegularProtestor(this, 20, 60));
	//objects.push_back(new HardCoreProtestor(this, 40, 60));

	
	/*
	int B = std::min((static_cast<int>(getLevel()) / 2 + 2), 9);
	for (int i = 0; i < B; ++i)
	{
		Boulder* newBoulder = new Boulder(this, 1, 2);
		objects.push_back(newBoulder);
	}
	*/

	return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::removeEarth(Actor* object)
{
	// the object passed in will be talking about the player
	
	for (int i = 0; i <= 63; ++i) // x coord 64 columns
	{
		for (int j = 0; j <= 59; ++j) // y coord 60 rows
		{
			if (earth[i][j] == nullptr)	// if earth has already been deleted skip
			{
				continue;  
			}
			/**/
			if (earth[i][j]->getX() <= object->getX() + 3 && earth[i][j]->getX() >= object->getX() && earth[i][j]->getY() <= object->getY() + 3 && earth[i][j]->getY() >= object->getY()) // if tunnelman player edge overlaps earth to the right
			{
				if (earth[i][j]->isVisible())
				{
					 // plays DIG_SOUND for every four block dug play sound
					int chance = rand() % 20;
					if (chance == 1)
					{
						this->playSound(SOUND_DIG);
					}
					earth[i][j]->setVisible(false);
					earth[i][j]->setDead();
				}
				
				//delete earth[i][j];			// Deletes earth	
				//earth[i][j] = nullptr;		// sets earth to nullptr
			}
		}
	}
	

	/*
	auto it = objects.begin();
	while (it != objects.end())
	{
		if ((*it)->isTunnelManOverlapEarth(object)) // if the tunnelman object and the earth overlap erase it 
		{
			delete* it;
			it = objects.erase(it); 
			return;
		}
		 // if this is true
		// then remove 
		// if not continue to the next object
		++it;

	}
	*/
}
bool StudentWorld::earthUnderBoulder(Actor* object) // passes in Boulder object and checks against each earth object in earth 2D array
{
	for (int i = 0; i <= 63; ++i) // x coord 64 columns
	{
		for (int j = 0; j <= 59; ++j) // y coord 60 rows
		{
			if (earth[i][j] == nullptr)	// if earth has already been deleted skip
			{
				continue;
			}
			if (earth[i][j]->getX() <= object->getX() + 3 && earth[i][j]->getX() >= object->getX() && earth[i][j]->getY() == object->getY() -1 &&  earth[i][j]->isVisible()) // if earth is directly under boulder
			{
				return true;
			}
		}
	}
	return false;
}


// works when two objects don't change their orientation unlike tunnelman 
/*
bool StudentWorld::hasActorCollided(Actor* object) // maybe only works for boulders since 4x4 size // pass in current object to check against everything else in the list
{
	// loop through each actor in the vector and check its X and Y against all other actors to determine a overlap/collision
	auto it = objects.begin();
	while (it != objects.end()) // what do i do  if the object passed in is checked against itself?
	{
		if (((*object).getX() == (*(*it)).getX()  ) && ((*object).getY() == (*(*it)).getY())) // if the object is compared against itself
		{
			++it;
			continue;
		}
		if (((object->getX() + 4 >= (*it)->getX()) && (object->getX() <= (*it)->getX() + 4)))
		{
			if(((object->getY() + 4 >= (*it)->getY()) && (object->getY() <= (*it)->getY() + 4)))
			{
				return true;
			}
		}
		++it;
	}
	return false;
}
*/
bool StudentWorld::isBoulderFallingAndCollided(int startX, int startY)
{
	auto it = objects.begin();
	while (it != objects.end()) // what do i do  if the object passed in is checked against itself?
	{
		if ((*it)->getID() == TID_BOULDER)
		{
			if ((startX == (*(*it)).getX()) && startY == (*(*it)).getY()) // if the object is compared against itself
			{
				++it;
				continue;
			}
			if (((startX + 3 >= (*it)->getX()) && (startX <= (*it)->getX() + 3)))
			{
				if (((startY)+3 >= (*it)->getY()) && (startY <= (*it)->getY() + 3))
				{
					if ((dynamic_cast<Boulder*>(*it)->getFallingState()))
					{
						return true;
					}
				}
			}

		}
		++it;
	}
	return false;
}
bool StudentWorld::overlappedWithBoulder(int startX, int startY) // maybe only works for boulders since 4x4 size // pass in current object to check against everything else in the list
{
	// loop through each actor in the vector and check its X and Y against all other actors to determine a overlap/collision
	auto it = objects.begin();
	while (it != objects.end()) // what do i do  if the object passed in is checked against itself?
	{
		if ((*it)->getID() == TID_BOULDER)
		{
			if ((startX == (*(*it)).getX()) && startY == (*(*it)).getY()) // if the object is compared against itself
			{
				++it;
				continue;
			}
			/*
			if (((startX + 3 >= (*it)->getX()) && (startX <= (*it)->getX() + 3)))
			{
				if (((startY)+3 >= (*it)->getY()) && (startY <= (*it)->getY() + 3))
				{
					if (dynamic_cast<Boulder*>(*it)->getFallingState()) // if overlap and falling
					{
						return true;
					}
				}
			}
			*/
			int boulderX = (*it)->getX() + 1;
			int boulderY = (*it)->getY() + 1;
			int centerX = startX + 1;
			int centerY = startY + 1;

			int distance = sqrt(((boulderX - centerX) * (boulderX - centerX)) + ((boulderY - centerY) * (boulderY - centerY)));
			if (distance <= 2)
			{
				//cout << distance << '\n';
				return true;
			}

		}
		++it;
	}
	return false;
}
bool StudentWorld::overlappedWithPlayer(int thisX, int thisY)
{
	int tunnelManX = getTunnelMan()->getX();
	int tunnelManY = getTunnelMan()->getY();
	if (((tunnelManX + 3 >= thisX) && (tunnelManX <= thisX + 3)))
	{
		if ((tunnelManY + 3 >= thisY) && (tunnelManY <= thisY + 3))
		{
			return true;
		}
	}
	return false;
}
bool StudentWorld::collidedWithBoulder(int startX, int startY) // maybe only works for boulders since 4x4 size // pass in current object to check against everything else in the list
{
	// loop through each actor in the vector and check its X and Y against all other actors to determine a overlap/collision
	auto it = objects.begin();
	while (it != objects.end()) // what do i do  if the object passed in is checked against itself?
	{
		if ((*it)->getID() == TID_BOULDER)
		{
			if ((startX == (*(*it)).getX()) && startY == (*(*it)).getY()) // if the object is compared against itself
			{
				++it;
				continue;
			}
			/*
			if (((startX + 3 >= (*it)->getX()) && (startX <= (*it)->getX() + 3)))
			{
				if (((startY)+3 >= (*it)->getY()) && (startY <= (*it)->getY() + 3))
				{
					if (dynamic_cast<Boulder*>(*it)->getFallingState()) // if overlap and falling
					{
						return true;
					}
				}
			}
			*/
			int boulderX = (*it)->getX() + 1;
			int boulderY = (*it)->getY() + 1;
			int centerX = startX + 1;
			int centerY = startY + 1;

			int distance = sqrt(((boulderX - centerX) * (boulderX - centerX)) + ((boulderY - centerY) * (boulderY - centerY)));
			if (distance <= 3)
			{
				//cout << distance << '\n';
				return true;
			}
			
		}
		++it;
	}
	return false;
}
bool StudentWorld::collidedWithEarth(int startX, int startY)
{
	for (int i = 0; i <= 63; ++i) // x coord 64 columns
	{
		for (int j = 0; j <= 59; ++j) // y coord 60 rows
		{
			if (earth[i][j] == nullptr)	// if earth has already been deleted skip
			{
				continue;
			}
			if (startY + 3 >= earth[i][j]->getY() && startY <= earth[i][j]->getY()) 
			{
				if (startX + 3 >= earth[i][j]->getX() && startX <= earth[i][j]->getX())
				{
					if (earth[i][j]->isVisible()) // if any earth object is visible within this 4x4 bounds of startX and startY we cannot add a water pool
					{
						//std::cout << "passed in x and y is  " << startX << " " << startY << std::endl;
						//std::cout << "collided with earth at " << i << " " << j << std::endl;
						return true;
					}
				}
				
			}
		}
	}
	return false;
}
bool StudentWorld::isEarthOrBoulderBlocking(int startX, int startY)
{
	// startX and startY take in the position of protestor
	// there are two cases one if there is a horizontal line of sight
	// from where tunnel man is at 
	int tunnelManX = getTunnelMan()->getX();
	int tunnelManY = getTunnelMan()->getY();
	if (tunnelManY == startY) // if there is a horizontal line of sight
	{
		if (startX > tunnelManX) // if protestor is in front of tunnelman check from tunnel man's position to protestor position for earth and boulder
		{
			for (int j = startY; j < startY + 4; ++j) // since protestor sprite is 4 high we will loop through each row starting from the bottom and check for earth
			{
				for (int i = tunnelManX + 2; i < startX; ++i) // check each x coordinate in between tunnelman and protestor if there is earth there
				{
					if (earth[i][j] != nullptr && earth[i][j]->isVisible())
					{
						//std::cout << "EARTH BLOCK" << '\n';
						return true;
					}
				}
			}
			// now we check for boulders
			auto it = objects.begin();
			while (it != objects.end())
			{
				if ((*it)->getID() == TID_BOULDER) // loops through each boulder object
				{
					if ((*it)->getX() <= startX && (*it)->getX() >= tunnelManX) //  if there is a boulder in between the protestor and tunnelman's x coordinate
					{
						if ((*it)->getY() >= startY && (*it)->getY() < tunnelManY + 4)
						{
							//std::cout << "BOULDER BLOCK" << '\n';
							return true;
						}
					}
				}
				++it;
			}
			
		}
		if (tunnelManX > startX) // if tunnelman is in front of protestor check from tunnel man's position to protestor position for earth and boulder
		{
			for (int j = startY; j < startY + 4; ++j) // since protestor sprite is 4 high we will loop through each row starting from the bottom and check for earth
			{
				for (int i = startX; i < tunnelManX -2; ++i) // check each x coordinate in between tunnelman and protestor if there is earth there
				{
					if (earth[i][j] != nullptr && earth[i][j]->isVisible())
					{
						return true;
					}
				}
			}
			// now we check for boulders
			auto it = objects.begin();
			while (it != objects.end())
			{
				if ((*it)->getID() == TID_BOULDER) // loops through each boulder object
				{
					if ((*it)->getX() >= startX && (*it)->getX() <= tunnelManX) //  if there is a boulder in between the protestor and tunnelman's x coordinate
					{
						if ((*it)->getY() >= startY && (*it)->getY() < tunnelManY + 4)
						{
							return true;
						}
					}
				}
				++it;
			}

		}
	}
	else if(tunnelManX == startX) // if there is a vertical line
	{
		if (startY > tunnelManY) // if protestor is in front of tunnelman check from tunnel man's position to protestor position for earth and boulder
		{
			for (int j = startX; j < startX + 4; ++j) // since protestor sprite is 4 high we will loop through each row starting from the bottom and check for earth
			{
				for (int i = tunnelManY; i < startY; ++i) // check each x coordinate in between tunnelman and protestor if there is earth there
				{
					if (earth[j][i] != nullptr && earth[j][i]->isVisible())
					{
						return true;
					}
				}
			}
			// now we check for boulders
			auto it = objects.begin();
			while (it != objects.end())
			{
				if ((*it)->getID() == TID_BOULDER) // loops through each boulder object
				{
					if ((*it)->getY() <= startY && (*it)->getY() >= tunnelManY) //  if there is a boulder in between the protestor and tunnelman's y coordinate
					{
						if ((*it)->getX() >= startX && (*it)->getX() < tunnelManX + 4)
						{
							return true;
						}
					}
				}
				++it;
			}

		}
		if ( startY < tunnelManY) // if tunnelman is above of protestor check from tunnel man's position to protestor position for earth and boulder
		{
			for (int j = startX; j < startX + 4; ++j) // since protestor sprite is 4 high we will loop through each row starting from the bottom and check for earth
			{
				for (int i = startY; i < tunnelManY; ++i) // check each x coordinate in between tunnelman and protestor if there is earth there
				{
					if (earth[j][i] != nullptr && earth[j][i]->isVisible())
					{
						return true;
					}
				}
			}
			// now we check for boulders
			auto it = objects.begin();
			while (it != objects.end())
			{
				if ((*it)->getID() == TID_BOULDER) // loops through each boulder object
				{
					if ((*it)->getY() >= (startY) && (*it)->getY() <= tunnelManY) //  if there is a boulder in between the protestor and tunnelman's y coordinate
					{
						if ((*it)->getX() >= startX && (*it)->getX() < tunnelManX + 4)
						{
							return true;
						}
					}
				}
				++it;
			}

		}
	}
	return false;
	
}
TunnelMan* StudentWorld::getTunnelMan()
{
	return TunnelManPtr;
}
void StudentWorld::addObject(Actor* newObject)
{
	objects.push_back(newObject);
}
bool StudentWorld::isAddable(int startX, int startY)
{
	auto it = objects.begin();
	while (it != objects.end()) //loop through each object and check if the object that is going to be 
	{
		int objectX = (*it)->getX() + 2;
		int objectY = (*it)->getY() + 2;
		double distance = sqrt((((startX + 2) - (objectX)) * ((startX+2) - objectX)) + (((startY + 2) - objectY) * ((startY + 2) - objectY)));
		if (distance < 6.0)
		{
			return false;
		}
		++it;
	}
	return true;
}
void StudentWorld::addBoulder()
{
	int possibleX = rand() % (60 - 0 + 1) + 0; // generates posible x coordinates for boulder
	int possibleY = rand() % (56 - 20 + 1) + 20; // generates possible y coordinates for boulder
	// re rolls numbers until valid coordinate
	
	// 32 38
	while ((possibleX >= 27 && possibleX <= 33 && possibleY >= 4 && possibleY <= 59) || !isAddable(possibleX, possibleY) ) // moved back by four since boulder is 4x4
	{
		possibleX = rand() % (60 - 0 + 1) + 0; // generates posible x coordinates for boulder
		possibleY = rand() % (56 - 20 + 1) + 20; // generates possible y coordinates for boulder
		if (isAddable(possibleX, possibleY) && !(possibleX >= 27 && possibleX <= 33 && possibleY >= 4 && possibleY <= 59))
		{
			break;
		}
		
	}

	//cout << possibleX << endl;
	//cout << possibleY << endl;
	objects.push_back(new Boulder(this, possibleX, possibleY));

}
void StudentWorld::addBarrel() // can spawn the same bounds as boulder
{
	int possibleX = rand() % (60 - 0 + 1) + 0; // generates posible x coordinates for boulder
	int possibleY = rand() % (56 - 20 + 1) + 20; // generates possible y coordinates for boulder
	// re rolls numbers until valid coordinate

	// 32 38
	while ((possibleX >= 27 && possibleX <= 33 && possibleY >= 4 && possibleY <= 59) || !isAddable(possibleX, possibleY)) // moved back by four since boulder is 4x4
	{
		possibleX = rand() % (60 - 0 + 1) + 0; // generates posible x coordinates for boulder
		possibleY = rand() % (56 - 20 + 1) + 20; // generates possible y coordinates for boulder
		if (isAddable(possibleX, possibleY) && !(possibleX >= 27 && possibleX <= 33 && possibleY >= 4 && possibleY <= 59))
		{
			break;
		}

	}
	//cout << possibleX << endl;
	//cout << possibleY << endl;
	objects.push_back(new Barrel(this, possibleX, possibleY));
} 
void StudentWorld::addGold() // can be added anywhere from 0 < x < 60 and 0 < y < 56 except for middle shaft
{
	int possibleX = rand() % (60 - 0 + 1) + 0; // generates posible x coordinates for boulder
	int possibleY = rand() % (56 - 0 + 1) + 0; // generates possible y coordinates for boulder
	// re rolls numbers until valid coordinate

	// 32 38
	while ((possibleX >= 27 && possibleX <= 33 && possibleY >= 4 && possibleY <= 59) || !isAddable(possibleX, possibleY)) // moved back by four since boulder is 4x4
	{
		possibleX = rand() % (60 - 0 + 1) + 0; // generates posible x coordinates for boulder
		possibleY = rand() % (56 - 0 + 1) + 0; // generates possible y coordinates for boulder
		if (isAddable(possibleX, possibleY) && !(possibleX >= 27 && possibleX <= 33 && possibleY >= 4 && possibleY <= 59))
		{
			break;
		}

	}
	objects.push_back(new GoldNugget(this, possibleX, possibleY));
} 
void StudentWorld::addSonar()
{
	int possibleX = 0;
	int possibleY = 60;
	objects.push_back(new SonarKit(this, possibleX, possibleY, TID_SONAR));
}
bool StudentWorld::ableToAddWaterPool(int startX, int startY)
{
	// taking the x and y of the water pool need to check if the 4x4 area has visible earth if it does then return false if it doesn't return true;
	// loop throgh each earth object 
	/*
	for (int i = 0; i <= 63; ++i) // x coord 64 columns
	{
		for (int j = 0; j <= 59; ++j) // y coord 60 rows
		{
			if (earth[i][j] == nullptr)	// if earth has already been deleted skip
			{
				continue;
			}
			if (startY + 3 >= earth[i][j]->getY() && startY <= earth[i][j]->getY())
			{
				if (startX + 3 >= earth[i][j]->getX() && startX <= earth[i][j]->getX())
				{
					if (earth[i][j]->isVisible()) // if any earth object is visible within this 4x4 bounds of startX and startY we cannot add a water pool
					{
						return false;
					}
				}

			}
		}
	}
	*/
	for (int i = startX; i < startX + 4; ++i)
	{
		for (int j = startY; j < startY + 4; ++j)
		{
			if (earth[i][j] == nullptr)	// if earth has already been deleted skip
			{
				continue;
			}
			if (earth[i][j]->isVisible())
			{
				return false;
			}
		}
	}
	// if we loop through every earth in the overlap region of the waterpool and no earth overlaps return true;
	return true;

}
void StudentWorld::addWaterPool() // can be added anywhere from 0 < x < 60 and 0 < y < 56 except for middle shaft
{
	// water pool can be anywhere in the oil field that has no earth and no other actors
	int possibleX = rand() % (60 - 0 + 1) + 0; // generates posible x coordinates for waterpool (ask is x the width and y the height)
	int possibleY = rand() % (56 - 0 + 1) + 0; // generates possible y coordinates for waterpool
	// re rolls numbers until valid coordinate

	// 32 38
	int counter = 0;
	while ((possibleX >= 27 && possibleX <= 33 && possibleY >= 4 && possibleY <= 59) || !isAddable(possibleX, possibleY) || !ableToAddWaterPool(possibleX, possibleY)) // moved back by four since boulder is 4x4
	{
		possibleX = rand() % (60 - 0 + 1) + 0; // generates posible x coordinates for boulder
		possibleY = rand() % (56 - 0 + 1) + 0; // generates possible y coordinates for boulder
		if (isAddable(possibleX, possibleY) && !(possibleX >= 27 && possibleX <= 33 && possibleY >= 4 && possibleY <= 59) && ableToAddWaterPool(possibleX, possibleY) && !overlappedWithPlayer(possibleX, possibleY))
		{
			//std::cout << "Water SPAWNED" << '\n';
			objects.push_back(new WaterPool(this, possibleX, possibleY));
			break;
		}
		if (++counter >= 100)
		{
			//std::cout << "Water break" << '\n';
			break;
		}

	}
	
}
void StudentWorld::addGoodie()
{
	int level = static_cast<int>(getLevel());
	int G = level * 25 + 300;
	int chance = rand() % (G - 1 + 1) + 1; // generates a random number form  1 to G
	if (chance == G) //  if the number generated is a 1 in G chance add a goodie
	{
		int oneInFiveChance = rand() % (5 - 1 + 1) + 1;
		if (oneInFiveChance == 5) // one in five chance to add a sonar kit
		{
			addSonar();
		}
		else
		{
			// scan entire field
			// 
			addWaterPool();
		}
	}

}
void StudentWorld::illuminateSonar(Actor* object) // takes in player parameter to get its x and y to illumiante
{
	// get center of tunnelman / player
	int startX = object->getX() + 2; // get player's center x // subject to change with collision 
	int startY = object->getY() + 2; // get player's center y // subject to change with collision 
	
	// loop through each object that is gold or barrel in objects vector and see check if it is in a 12 square radius of its  location
	auto it = objects.begin();
	while (it != objects.end())
	{
		if ((*it)->getID() == TID_GOLD || (*it)->getID() == TID_BARREL) // if the object is barrel or gold (hidden game objects)
		{
			int objectX = (*it)->getX() + 2; // get objects center x
			int objectY = (*it)->getY() + 2; // get objects cetner y
			double distance = sqrt((((startX + 2) - (objectX)) * ((startX + 2) - objectX)) + (((startY + 2) - objectY) * ((startY + 2) - objectY))); // calculates distance between objects
			if (distance < 12.0) // if the distance between objects is less than 12
			{
				// illuminate the object
				(*it)->setVisible(true);
			}
		}
		++it;
	}
	
}
int StudentWorld::move()
{
	setDisplayText();
	if (!getTunnelMan()->isAlive())
	{
		decLives();
		playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED;
	}
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	addGoodie(); // add new goodies at each tick;

	// loop through current number of protestors and check how many
	int currentNumberP = 0;
	std::vector<Actor*>::iterator iP = objects.begin();
	while (iP != objects.end())
	{
		if ((*iP)->getID() == TID_PROTESTER || (*iP)->getID() == TID_HARD_CORE_PROTESTER)
		{
			++currentNumberP;
		}
		++iP;
	}
	int level = getLevel();
	int otherP = 2 + level * 1.5;
	int P = min(15, otherP);
	if (currentNumberP < P)
	{
		if (T == 0)
		{
			// if the number of ticks have passed 
			int chanceHC = rand() % probabilityOfHardCore;
			if (1 == chanceHC)
			{
				objects.push_back(new HardCoreProtestor(this, 60, 60)); // spawn hardcore protestor
				
			}
			else
			{
				objects.push_back(new RegularProtestor(this, 60, 60));
				
			}
			int level = getLevel();
			int otherT = 200 - getLevel();
			T = max(25, otherT);
		}
		--T;
	}

 	TunnelManPtr->doSomething();
	std::vector<Actor*>::iterator it = objects.begin();
	while (it != objects.end())
	{
		(*(*it)).doSomething();
		++it;
	}
	//Remove Dead Actors after each tick
	auto it2 = objects.begin();
	while (it2 != objects.end())
	{
		if (!(*(*it2)).isAlive()) // if actor is dead
		{
			delete* it2;
			it2 = objects.erase(it2);
		}
		else
		{
			++it2;
		}
	}
	if (getBarrels() == 0)
	{
		//advanceToNextLevel();
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
	if (getLives() > 0)
	{
		return GWSTATUS_CONTINUE_GAME;
	}
	return GWSTATUS_PLAYER_DIED;
	
	
}

void StudentWorld::cleanUp()
{
	delete TunnelManPtr; // create a TunnelMan object and insert it into the oil field at the right starting location
// fill  the oil field with earth objects
	std::vector<Actor*>::iterator it = objects.begin();
	while (it != objects.end())
	{
		delete* it;
		it = objects.erase(it);
	}
	for (int i = 0; i <= 63; ++i) // x coord 64 columns
	{
		for (int j = 0; j <= 59; ++j) // y coord 60 rows
		{
			if (i >= 30 && i <= 33 && j >= 4 && j <= 59) // skips middle column (x, y coordinate ex. (30,4) we skip)
			{
				continue;
			}
			if (earth[i][j] != NULL)
			{
				delete earth[i][j];
				earth[i][j];
			}
			else
			{
				//cout << "NULL DELETE: " << '\n';
			}
			
		}
	}
}



// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

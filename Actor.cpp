#include "Actor.h"
#include "GameWorld.h"
#include "StudentWorld.h"
#include <utility>


Actor::Actor(GameWorld* world, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) :
	GraphObject(imageID, startX, startY, dir, size, depth), m_studentWorld(world)
{
	isAlives = true;
}
Actor::~Actor()
{
}
// It must have a virtual method called doSomething() that can be
//called by the World to get one of the game’s actors to do
//something.
//v.You may add
void Actor::doSomething()
{

}
bool Actor::isTunnelManOverlapEarth(Actor* object) // object is player ("this" is going to be in reference to the earth) // really since this can only be use for 
{
	// really since this can only be use for checking if earth and tunnelman overlaps since we specify a hard constant "3", this should be defined in the tunnelman class

	if (getX() <= object->getX() + 3 && getX() >= object->getX() && getY() <= object->getY() + 3 && getY() >= object->getY()) // if tunnelman player edge overlaps earth to the right
	{
		return true;
	}
	return false;
}
bool Actor::areYouVisible()
{
	return isVisible();
}
bool Actor::isTunnelManClose()
{
	int tunnelManX = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX();
	int tunnelManY = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY();
	if (((tunnelManX + 4 >= getX()) && (tunnelManX <= getX() + 4)))
	{
		if ((tunnelManY + 4 >= getY()) && (tunnelManY <= getY() + 4))
		{
			return true;
		}
	}
	return false;
}
bool Actor::isTunnelManOverlap()
{
	int tunnelManX = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX();
	int tunnelManY = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY();
	if (((tunnelManX + 3 >= getX()) && (tunnelManX <= getX() + 3)))
	{
		if ((tunnelManY + 3 >= getY()) && (tunnelManY <= getY() + 3))
		{
			return true;
		}
	}
	return false;
}
void Actor::setDead()
{
	isAlives = false;
}
bool Actor::isAlive()
{
	return isAlives;
}
void Actor::annoy(int x)
{

}
GameWorld* Actor::getWorld()
{
	return m_studentWorld;
}

TunnelMan::TunnelMan(GameWorld* world) : Actor(world, TID_PLAYER, 30, 60, right, 1.0, 0), m_hp(10), water(5), goldnuggets(0), sonarkit(1)
{
	//isAlives = true;
	setVisible(true);
}
 TunnelMan::~TunnelMan()
{

}
 bool TunnelMan::isAlive()
 {
	 if (m_hp <= 0)
	 {
		 Actor::setDead();
		 return Actor::isAlive();
	 }
	 return true;
 }
 void TunnelMan::annoy(int x)
 {
	 getWorld()->playSound(SOUND_PLAYER_ANNOYED);
	 m_hp -= x;
 }
void TunnelMan::doSomething()
{

	///The TunnelMan must check to see if it is currently alive. If not, then its doSomething() method must return immediately – none of the following steps should be performed.
	if (!isAlive())
	{
		return;
	}
	// loop through each earth object and if their "x" and "y" overlap with tunnelman "x" and "y" remove the earth object

	dynamic_cast<StudentWorld*>(getWorld())->removeEarth(this); // casts parent class GameWorld to StudentWorld and allows it access StudentWorld function (passes in tunnelman "this")
	// player DIG sound

	// check if boulder is overlapping with tunnelman if so setDead
	if (dynamic_cast<StudentWorld*>(getWorld())->overlappedWithBoulder(getX(), getY()))
	{
		m_hp = 0;
		Actor::setDead();
	}
	//move tunnel man based on 

	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		// user hit a key this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			dynamic_cast<StudentWorld*>(getWorld())->removeEarth(this);
			 // changes the tunnelman object to face left if pressed
			if (this->getDirection() == left)
			{
				if (!(getX() - 1 < 0) && !(dynamic_cast<StudentWorld*>(getWorld())->collidedWithBoulder(getX() - 1, getY())))// need to check if im able to move there (is tunnelman ' s origin changed when it faces left??? 
				{
					//move if allowed
					moveTo(getX() - 1, getY());
				}
			}
			else
			{
				this->setDirection(left);
			}
			break;
		case KEY_PRESS_RIGHT:
			dynamic_cast<StudentWorld*>(getWorld())->removeEarth(this);
			 // changes the tunnelman object to face right if pressed
			if (this->getDirection() == right)
			{
				if (!(getX() + 1 > 60) && !(dynamic_cast<StudentWorld*>(getWorld())->collidedWithBoulder(getX() + 1, getY())))// need to check if im able to move there (is tunnelman ' s origin changed when it faces right??? 
				{
					//move if allowed
					moveTo(getX() + 1, getY());
				}
			}
			else
			{
				this->setDirection(right);
			}
			break;
		case KEY_PRESS_UP:
			dynamic_cast<StudentWorld*>(getWorld())->removeEarth(this);
			 // changes the tunnelman object to face up if pressed
			if (this->getDirection() == up)
			{
				if (!(getY() + 1 > 60) && !(dynamic_cast<StudentWorld*>(getWorld())->collidedWithBoulder(getX(), getY() + 1)))// need to check if im able to move there (is tunnelman ' s origin changed when it faces up??? 
				{
					//move if allowed

					moveTo(getX(), getY() + 1);
				}
			}
			else
			{
				this->setDirection(up);
			}
			break;
		case KEY_PRESS_DOWN:
			dynamic_cast<StudentWorld*>(getWorld())->removeEarth(this);
			 // changes the tunnelman object to face down if pressed
			if (this->getDirection() == down)
			{
				if (!(getY() - 1 < 0) && !(dynamic_cast<StudentWorld*>(getWorld())->collidedWithBoulder(getX(), getY() - 1)))// need to check if im able to move there (is tunnelman ' s origin changed when it faces left??? 
				{
					//move if allowed
					moveTo(getX(), getY() - 1);
				}
			}
			else
			{
				this->setDirection(down);
			}
			break;
		case KEY_PRESS_SPACE:
 			if (water > 0)
			{
				--water;
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
				dynamic_cast<StudentWorld*>(getWorld())->addObject(new Squirt(getWorld(), getX(), getY(), getDirection())); // adds a new squirt object with x and y direction of player
			}
			break;
		case KEY_PRESS_TAB:
			if (goldnuggets > 0)
			{
				--goldnuggets;
				dynamic_cast<StudentWorld*>(getWorld())->addObject(new GoldNugget(getWorld(), this, getX(), getY()));

			}
			break;
		
		case KEY_PRESS_ESCAPE:
		{
			m_hp = 0;
		}
			break;
		case 'Z': // (Z key or z key)
			if (sonarkit > 0)
			{
				--sonarkit;
				//getWorld()->playSound(SOUND_SONAR);
				dynamic_cast<StudentWorld*>(getWorld())->illuminateSonar(this);

			}
			break;
		case 'z': // (Z key or z key)
			if (sonarkit > 0)
			{
				--sonarkit;
				//getWorld()->playSound(SOUND_SONAR);
				dynamic_cast<StudentWorld*>(getWorld())->illuminateSonar(this);

			}
			break;

			// etc…
		}
	}
}
int TunnelMan::getHP()
{
	return m_hp;
}
int TunnelMan::getSquirts()
{
	return water;
}
int TunnelMan::getGold()
{
	return goldnuggets;
}
int TunnelMan::getSonarKits()
{
	return sonarkit;
}

void TunnelMan::addGold()
{
	++goldnuggets;
}
void TunnelMan::addSonarKit()
{
	sonarkit += 1;
}
void TunnelMan::addWater()
{
	water += 5;
}

Earth::Earth(GameWorld* world, int startX, int startY) : Actor(world, TID_EARTH, startX, startY, right, 0.25, 3)
{
	setVisible(true);
	isAlives = true;
}
Earth::~Earth()
{

}
void Earth::setDead()
{
	setVisible(false);
	isAlives = false;
}
bool Earth::isAlive()
{
	return isAlives;
}
void Earth::doSomething()
{
	return;
}
// Annoyed function

Boulder::Boulder(GameWorld* world, int startX, int startY) : Actor(world, TID_BOULDER, startX, startY, down, 1.0, 1)
{
	setVisible(true);
	dynamic_cast<StudentWorld*>(getWorld())->removeEarth(this); // wherever a boulder spawn it removes it
	//isAlives = true;
	state = 0;
	numTicks = 0;
	alreadyIncreasedScore = false;
}
Boulder::~Boulder()
{

}
bool Boulder::isBoulderOverlap(int startX, int startY) // if the boulder's bottom row touches tunnelman's top row he is hit
{
	/*
	int boulderCenterX = getX() + 2;
	int boulderCenterY = getY() + 2;

	int circleDistanceX = boulderCenterX - (startX + 2);
	if (circleDistanceX < 0) { circleDistanceX *= -1; }
	int circleDistanceY = boulderCenterY - (startY + 2);
	if (circleDistanceY < 0) { circleDistanceY *= -1; }

	if (circleDistanceX > ((4) / 2 + (3.1415 * 2 * 2))) { return false; }
	if (circleDistanceY > ( (4) / 2 + (3.1415 * 2 * 2))) { return false; }

	if (circleDistanceX <= (4 / 2)) { return true; }
	if (circleDistanceY <= (4 / 2)) { return true; }

	int cornerDistanceSquare = (circleDistanceX - 4 / 2) ^ 2 + (circleDistanceY - 4 / 2) * (circleDistanceX - 4 / 2) ^ 2 + (circleDistanceY - 4 / 2);

	return (cornerDistanceSquare <= ((3.1415 * 2 * 2) * (3.1415 * 2 * 2)));
	*/
	if ((getX() <= startX) && (getX() + 4 >= startX) && (getY() == startY)) return true; // if the bottom row of the boulder intersects any  of tunnelman's top row its dead
	return false;

}
void Boulder::moveFallingBoulder()
{
	if (!(dynamic_cast<StudentWorld*>(getWorld())->earthUnderBoulder(this)) && !(dynamic_cast<StudentWorld*>(getWorld())->collidedWithBoulder(getX(), getY())))
	{
		//(a) hits the bottom of the oil field (i.e., it tries to move to y=-1), (b)
		//runs into the top of another Boulder, or (c)it runs into Earth
		if ((getY() <= -1))
		{
			Actor::setDead();
			return;
		}
		//If the Boulder comes within a radius of 3 (i.e. less than or equal to 3
		//squares, so 2.99 would count) of any Protester(s) or the TunnelMan
		//while falling, it must cause 100 points of annoyance to those actors
		if (isBoulderOverlap(dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX(), dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY()))
		{
			dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->annoy(100);
		}
		moveTo(getX(), getY() - 1);
		return;

	}
	//When any of the above conditions are met the Boulder
	//must set its state to dead so it can be removed from the game at the end
	//of the current tick
	Actor::setDead();
}
bool Boulder::getFallingState()
{
	return (state == 2);
}
void Boulder::doSomething() 

{
	if (Actor::isAlive())
	{
		if (state == 0)
		{
			// call StudentWorld's earthUnderBoulder boolean to check if there is any other under boulder
			if (dynamic_cast<StudentWorld*>(getWorld())->earthUnderBoulder(this)) // asks the StudentWorld's class and checks for the boulder object passed in asks every earth object if its below boulder
			{
				return; //  If there is any Earth below the Boulder, it does nothing
			}
			else
			{
				//However, if none of the 4 squares beneath the Boulder have any
				//Earth, then the Boulder must transition into a waiting state :
				state = 1;
				return;
				// It must enter a waiting state for the next 30 ticks 
			}
		}
		if (state == 1)
		{
			if (numTicks >= 30)
			{
				getWorld()->playSound(SOUND_FALLING_ROCK);
				state = 2;
				return; 
			}
		}
		if(state == 2) // if in falling state check if protestor is underneath
		{
			if (dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this) != NULL) // check if falling boulder and protestor overlap
			{
				dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->annoy(100);
				if (!alreadyIncreasedScore) // increase score only once
				{
					getWorld()->increaseScore(500);
					alreadyIncreasedScore = true;
				}
				dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->boulderHitProtestor();
			}
			moveFallingBoulder();
			return;
			
		}
		++numTicks;
	}
	return;
}



Squirt::Squirt(GameWorld* world, int startX, int startY, Direction dir) : Actor(world, TID_WATER_SPURT, startX, startY, dir, 1.0, 1), travelDistance(16), currentDistance(0), direction(dir)
{
	setVisible(true);
}
Squirt::~Squirt()
{

}
/*
void Squirt::setDead()
{
	Actor::setDead();
}
*/
void Squirt::doSomething()
{
	if (currentDistance >= travelDistance)
	{
		//If a Squirt has traveled through its full travel distance, then it immediately sets its state
		//to dead, so it can be removed from the oil field at the end of the current tick.
		setDead();
	}
	if (direction == left)
	{
		if (!(getX() - 1 < 0) && !(dynamic_cast<StudentWorld*>(getWorld())->collidedWithBoulder(getX() - 1, getY())) && (!(dynamic_cast<StudentWorld*>(getWorld())->collidedWithEarth(getX() - 1, getY()))))// need to check if im able to move there (is tunnelman ' s origin changed when it faces left??? 
		{
			// if collided with protestor annoy the protestor 2 points 
			// check if squirt collided with protestor
			if (dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this) != NULL)
			{
				// by this point squirt collided with protestor so annoy the protestor and then kill it
				dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->annoy(2);
				dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->gotSquirted();
				setDead();
				return;
			}
			++currentDistance;
			moveTo(getX() - 1, getY());
			return;
		}
		setDead();
	}
	if (direction == right)
	{
		if (!(getX() + 1 > 60) && !(dynamic_cast<StudentWorld*>(getWorld())->collidedWithBoulder(getX() + 1, getY())) && (!(dynamic_cast<StudentWorld*>(getWorld())->collidedWithEarth(getX() + 1, getY()))))// need to check if im able to move there (is tunnelman ' s origin changed when it faces left??? 
		{
			if (dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this) != NULL)
			{
				// by this point squirt collided with protestor so annoy the protestor and then kill it

				dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->annoy(2);
				dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->gotSquirted();
				//dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->playAnnoySound();
				setDead();
				return;
			}
			++currentDistance;
			moveTo(getX() + 1, getY());
			return;
		}
		setDead();
	}
	if (direction == up)
	{
		if (!(getY() + 1 > 60) && !(dynamic_cast<StudentWorld*>(getWorld())->collidedWithBoulder(getX(), getY() + 1)) && (!(dynamic_cast<StudentWorld*>(getWorld())->collidedWithEarth(getX(), getY() + 1))))// need to check if im able to move there (is tunnelman ' s origin changed when it faces left??? 
		{
			if (dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this) != NULL)
			{
				// by this point squirt collided with protestor so annoy the protestor and then kill it
				dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->annoy(2);
				dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->gotSquirted();
				//dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->playAnnoySound();
				setDead();
				return;
			}
			++currentDistance;
			moveTo(getX(), getY() + 1);
			return;
		}
		setDead();
	}
	if (direction == down)
	{
		if (!(getY() - 1 < 0) && !(dynamic_cast<StudentWorld*>(getWorld())->collidedWithBoulder(getX(), getY() - 1)) && (!(dynamic_cast<StudentWorld*>(getWorld())->collidedWithEarth(getX(), getY() - 1))))// need to check if im able to move there (is tunnelman ' s origin changed when it faces left??? 
		{
			if (dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this) != NULL)
			{
				// by this point squirt collided with protestor so annoy the protestor and then kill it
				dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->annoy(2);
				dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->gotSquirted();
				//dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->playAnnoySound();
				setDead();
				return;
			}
			++currentDistance;
			moveTo(getX(), getY() - 1);
			return;
		}
		setDead();
	}

}
/*
void Squirt::annoy(int x)
{

}
*/

Barrel::Barrel(GameWorld* world, int startX, int startY) : Actor(world, TID_BARREL, startX, startY, right, 1.0, 2)
{
	setVisible(false);
}
Barrel::~Barrel()
{

}
/*
bool Barrel::areYouVisible()
{
	return isVisible();
}
bool Barrel::isTunnelManClose()
{
	int tunnelManX = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX();
	int tunnelManY = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY();
	if (((tunnelManX + 4 >= getX()) && (tunnelManX <= getX() + 4)))
	{
		if ((tunnelManY + 4 >= getY()) && (tunnelManY <= getY() + 4))
		{
			return true;
		}
	}
	return false;
}
bool Barrel::isTunnelManOverlap()
{
	int tunnelManX = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX();
	int tunnelManY = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY();
	if (((tunnelManX + 3 >= getX()) && (tunnelManX <= getX() + 3)))
	{
		if ((tunnelManY + 3 >= getY()) && (tunnelManY <= getY() + 3))
		{
			return true;
		}
	}
	return false;
}
*/
void Barrel::doSomething()
{
	if (Actor::isAlive())
	{
		if (!areYouVisible() && isTunnelManClose())
		{
			//Otherwise, if the Barrel is not currently visible AND the TunnelMan is within a radius
			//of 4.0 of it(<= 4.00 units away), then
			//a. The Barrel must make itself visible with the setVisible() method.
			//b.The Barrel’s doSomething() method must immediately return
			setVisible(true);
			return;
		}
		if (isTunnelManOverlap())
		{
			//Otherwise, if the Barrel is within a radius of 3.0 (<= 3.00 units away) from the
			//TunnelMan, then the Barrel will activate, and :
			/*
			a.The Barrel must set its state to dead(so that it will be removed by your
				StudentWorld class from the game at the end of the current tick).
				b.The Barrel must play a sound effect to indicate that the player picked up the
				Goodie : SOUND_FOUND_OIL.
				c.The Barrel must increase the player’s score by 1000 points.
				d.If necessary, the Barrel may inform the StudentWorld object that it was
				picked up.Once all Barrels on the level have been picked up, the player
				finishes the leveland may advance to the next level.
			*/
			dynamic_cast<StudentWorld*>(getWorld())->increaseScore(1000);
			setDead();
			getWorld()->playSound(SOUND_FOUND_OIL);
			// increase score
			// tell player barrel has been picked up;
		}
	}
	return;
}
/*
void Barrel::annoy(int x)
{

}
*/
/*
void Barrel::setDead()
{
	Actor::setDead();
}
*/
GoldNugget::GoldNugget(GameWorld* world, int startX, int startY): Actor(world, TID_GOLD, startX, startY, right, 1.0, 2)
{
	// dropped by game
	tickCounter = 0;
	//isAlives = true;
	canTunnelManPickUp = true;
	canProtesterPickUp = false;
	state = 0; // permanent state when created by game
	setVisible(false);
}
GoldNugget::~GoldNugget()
{

}
GoldNugget::GoldNugget(GameWorld* world, TunnelMan* tunnelManPtr, int startX, int startY) : Actor(world, TID_GOLD, startX, startY, right, 1.0, 2)
{
	// dropped by tunnel man
	tickCounter = 0;
	//isAlives = true;
	canTunnelManPickUp = false;
	canProtesterPickUp = true;
	state = 1; // temporary state when created by tunnelman
	setVisible(true);
}
void GoldNugget::doSomething()
{
	if (Actor::isAlive())
	{
		if (state == 0) // permanent state
		{
			if ((!areYouVisible()) && isTunnelManClose())
			{
				//Otherwise, if the Gold Nugget is not currently visible AND the TunnelMan is within a
				//radius of 4.0 of it(<= 4.00 units away), then:
				setVisible(true);
				return;
			}
			if (canTunnelManPickUp && isTunnelManOverlap())
			{
				//Otherwise, if the Gold Nugget is pickup - able by the TunnelMan and it is within a
				//radius of 3.0 (<= 3.00 units away) from the TunnelMan, then the Gold Nugget
				//will activate, and :
				dynamic_cast<StudentWorld*>(getWorld())->increaseScore(10); // increase the score 
				setDead();
				getWorld()->playSound(SOUND_GOT_GOODIE);
				dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->addGold();
				// increase player score with studentworld player pointer method
				// tell tunnelman that it received a new gold item
			}
		}
		if (state == 1) // temp state dropped by tunnelman
		{
			if (tickCounter >= 100)
			{
				setDead();
				return;
			}
			
			if (canProtesterPickUp && (dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this) != NULL)) // if a pointer to protestor is returned then it picked up the gold
			{
				if (dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->getID() == TID_HARD_CORE_PROTESTER) // if its a hardcore protestor
				{
					dynamic_cast<StudentWorld*>(getWorld())->increaseScore(50);
					dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->bribe(); // bribe hardcore protestor
					getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
					setDead();
					return;

				}
				else
				{
					dynamic_cast<StudentWorld*>(getWorld())->increaseScore(25); // increase the score
					dynamic_cast<StudentWorld*>(getWorld())->getProtestor(this)->bribe(); // bribe the protestor 
					setDead();
					getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
					return;
				}
				// gold object tells protestor that it received a new nugget
				// gold nugget increases player score by 25
			}
			
			++tickCounter;
		}
		
	}
	return;
}


SonarKit::SonarKit(GameWorld* world, int startX, int startY, int imageID): Actor(world, imageID, startX, startY, right, 1.0, 2)
{
	setVisible(true);
	//isAlives = true;
	int level = getWorld()->getLevel();
	tickCounter = 0;
	T = std::max(100, 300 - 10 * level);
}
void SonarKit::doSomething()
{
	if (Actor::isAlive())
	{
		if (isTunnelManOverlap())
		{
			/*
			* Otherwise, if the Sonar Kit is within a radius of 3.0 (<= 3.00 units away) from the
				TunnelMan, then the Sonar Kit will activate, and:
					a. The Sonar Kit must set its state to dead (so that it will be removed by your
						StudentWorld class from the game at the end of the current tick).
					b. The Sonar Kit must play a sound effect to indicate that the TunnelMan picked
						up the Goodie: SOUND_GOT_GOODIE.
					c. The Sonar Kit must tell the TunnelMan object that it just received a new Sonar
						Kit so it can update its inventory.
					d. The Sonar Kit increases the player’s score by 75 points (This increase can be
					   performed by the TunnelMan class or the Sonar Kit class).
			*/
			setDead();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			if (getID() == TID_SONAR)
			{
				dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->addSonarKit();
				getWorld()->increaseScore(75);
			}
			if (getID() == TID_WATER_POOL)
			{
				dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->addWater();
				getWorld()->increaseScore(100);
			}
			

			// increase player's score by 75
			return;
		}
		if (tickCounter >= T)
		{
			setDead();
			return;
		}
		++tickCounter;
	}
	return;
}
SonarKit::~SonarKit()
{

}


WaterPool::WaterPool(GameWorld* world, int startX, int startY) : SonarKit(world, startX, startY, TID_WATER_POOL)
{

}
  
WaterPool::~WaterPool()
{

}

Protestor::Protestor(GameWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth): Actor(world, imageID, startX, startY, dir, size, depth)
{
	//isAlives = true;
	setVisible(true);
	numSquaresToMoveInCurrentDirection = calculateNumSquares();
	leaveTheOilFieldState = false; // indicates that it should not leave the oil field
	ticksToWaitBetweenMoves = ticksToWait(); // calculates waiting ticks
	tickSinceLastPerpendicularTurn = 0;
	currentTick = 0;
	isBribed = false;
	hasBoulderHit = false;
	int level = getWorld()->getLevel();
	stunTimer = 0;
	playOnceGiveUpOnce = false;
}
Protestor::~Protestor()
{

}

int Protestor::calculateNumSquares()
{
	// should be a random value form 8 <= x <= 60;
	int numSquares = rand() % (60 - 8 + 1) + 8; 
	return numSquares;
}
int Protestor::ticksToWait()
{
	int level = getWorld()->getLevel();
	int tickWaiting = std::max(0, 3 - level / 4);
	return tickWaiting;
}
bool Protestor::leaveOilField()
{
	if (isBribed) // check is bribed first
	{
		leaveTheOilFieldState = true;
		return true;
	}
	if (m_hp <= 0 || hasBoulderHit) // leave oil field 
	{
		if (!playOnceGiveUpOnce)
		{
			getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
			playOnceGiveUpOnce = true;
		}
		leaveTheOilFieldState = true;
		return true;
	}
	return false;
}
bool Protestor::tunnelManWithinFourUnits()
{
	// if tunnelman is within four distance units return true
	int tunnelManX = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX() + 2;
	int tunnelManY = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY() + 2;
	int protestorX = getX() + 2;
	int protestorY = getY() + 2;
	double distance = sqrt((((tunnelManX) - (protestorX)) * ((tunnelManX) - protestorX)) + (((tunnelManY) - protestorY) * ((tunnelManY) - protestorY)));
	//std::cout << "distance is: " << distance << std::endl;
	if (distance < 4.0)
	{
		return true;
	}
	return false;
	
}
bool Protestor::isProtestorFacingTunnelMan()
{
	if (getDirection() == up) // if protesor is facing up
	{
		// to be facing the same direction as tunnelman, tunnelman has to be directly above the protestor meaning
		// the protestor's and tunnelman's x coordinate are the same and the y coordinate is above the protestor
		if (dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX() == getX() && dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY() >= getY()) // if the x coordinate is the same
		{
			return true;
		}
	}
	if (getDirection() == down) // if protesor is down down
	{
		// to be facing the same direction as tunnelman, tunnelman has to be directly below the protestor meaning
		// the protestor's and tunnelman's x coordinate are the same and the y coordinate is below the protestor
		if (dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX() == getX() && dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY() <= getY()) // if the x coordinate is the same
		{
			return true;
		}
	}
	if (getDirection() == right) // if protesor is facing right
	{
		// to be facing the same direction as tunnelman, tunnelman has to be directly to the right the protestor meaning
		// the protestor's and tunnelman's y coordinate are the same and the x coordinate is larger the protestor
		if (dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX() >= getX() && dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY() == getY()) // if the x coordinate is the same
		{
			return true;
		}
	}
	if (getDirection() == left) // if protesor is facing left
	{
		// to be facing the same direction as tunnelman, tunnelman has to be directly to the left the protestor meaning
		// the protestor's and tunnelman's y coordinate are the same and the x coordinate is less than the protestor
		if (dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX() <= getX() && dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY() == getY()) // if the x coordinate is the same
		{
			return true;
		}
	}
	return false;
}
bool Protestor::canSeeTunnelMan()
{
	return (dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX() == getX() || dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY() == getY());
	//Is in a straight horizontal or vertical line of sight to the TunnelMan (even if
	// the Regular Protester isn’t currently facing the TunnelMan)
}

void Protestor::BFS(std::stack<int>& p, int startX, int startY, int endX, int endY) {
	// 1) initialize queue for BFS
	if (startX == endX && startY == endY)
	{
		return;
	}
	std::queue<int> opx;
	std::queue<int> opy;
	// 2) intialize array to keep track of parent node
	std::pair<int, int> backtrack[64][64];
	// 2) push starting points onto the queue
	opx.push(startX);
	opy.push(startY);
	// 3) 2D array to keep track of visited nodes
	bool visited[64][64];
	for (int i = 0; i < 64; i++) 
	{
		for (int k = 0; k < 64; k++) 
		{
			visited[i][k] = false;
		}
	}
	visited[startX][startY] = true;
	// 4) initialize starting points
	int iX = startX;
	int iY = startY;
	while (!opx.empty() && !opy.empty())
	{
		iX = opx.front();
		iY = opy.front();
		if (iX == endX && iY == endY) {
			break;
		}
		opx.pop();
		opy.pop();
		// visit neighbors
		if (outOfBounds(iX, iY + 1) && visited[iX][iY + 1] == false) // UP
		{
			opx.push(iX);
			opy.push(iY + 1);
			visited[iX][iY] = true;
			backtrack[iX][iY + 1].first = iX;
			backtrack[iX][iY + 1].second = iY;
		}
		if (outOfBounds(iX + 1, iY) && visited[iX + 1][iY] == false) // RIGHT
		{
			opx.push(iX + 1);
			opy.push(iY);
			visited[iX][iY] = true;
			backtrack[iX + 1][iY].first = iX;
			backtrack[iX + 1][iY].second = iY;
		}
		if (outOfBounds(iX - 1, iY) && visited[iX - 1][iY] == false) // LEFT
		{
			opx.push(iX - 1);
			opy.push(iY);
			visited[iX][iY] = true;
			backtrack[iX - 1][iY].first = iX;
			backtrack[iX - 1][iY].second = iY;
		}
		if (outOfBounds(iX, iY - 1) && visited[iX][iY - 1] == false) // DOWN
		{
			opx.push(iX);
			opy.push(iY - 1);
			visited[iX][iY] = true;
			backtrack[iX][iY - 1].first = iX;
			backtrack[iX][iY - 1].second = iY;
		}
		
	}
	int i = iX; // 60
	int j = iY; // 60
	while (backtrack[i][j].first != startX || backtrack[i][j].second != startY) {
		int x = backtrack[i][j].first;
		int y = backtrack[i][j].second;
		if (i == x) 
		{ // if the x is the same vertical movement
			if (j > y) 
			{ // if current move is greater than last move we moved up
				p.push(1); // push up
				--j;
				continue;
			}// current move is less than last move so moved down
			p.push(2);
			++j;
			continue;
		}
		// if we didn't move vertically we moved left or right horizontally
		if (i > x)
		{//if current move is greater in x than last move  
			p.push(3);
			--i;
			continue;
		}// we moved left since we didnt move right
		++i;
		p.push(4);
	}
	int x = backtrack[i][j].first;
	int y = backtrack[i][j].second;
	if (i == x) 
	{// if the x is the same vertical movement
		if (j > y) 
		{// if current move is greater than last move we moved up
			p.push(1);// push up
			--j;
			return;
		}// current move is less than last move so moved down
		p.push(2);
		++j;
		return;
	}
	// if we didn't move vertically we moved left or right horizontally
	if (i > x)
	{//if current move is greater in x than last move  
		p.push(3);
		--i;
		return;
	}// we moved left since we didnt move right
	++i;
	p.push(4);
	return;
}
bool Protestor::outOfBounds(int startX, int startY)
{
	if (startX > 60 || startY > 60 || startX < 0 || startY < 0)
	{
		return false;
	}
	if (dynamic_cast<StudentWorld*>(getWorld())->collidedWithBoulder(startX, startY) || dynamic_cast<StudentWorld*>(getWorld())->collidedWithEarth(startX, startY))
	{
		if (dynamic_cast<StudentWorld*>(getWorld())->isBoulderFallingAndCollided(startX, startY)) // if boulder is in a falling state at this position we can still move
		{
			return true;
		}
		return false;
	}
	return true;
}
void Protestor::doSomething() /*/*/ // reset currenTick if tick timer has elapsed
{
	
	if (isAlive()) // 1) regular protestor must check if its alive if not return
	{
		// need to check if boulder and protestor overlap
		if (stunTimer > 0)
		{
			--stunTimer;
			if (leaveOilField())
			{

				//3) Otherwise, if the Regular Protester is in a leave-the-oil-field state (because their
				//hit points reached zero due to being repeatedly squirted by the TunnelMan’s squirt
				//gun or bonked by a falling Boulder), then:
				if (getX() == 60 && getY() == 60)
				{
					/*  a). If the Regular Protester is at its exit point (at location x=60, y=60) then it
						will immediately set its status to dead so that it will be removed from the
						game at the end of the current tick (by your StudentWorld class).
					*/
					setDead();
					return;
				}
				// b). Otherwise, the Regular Protester must move one square closer to its exit
				//point(at x = 60, y = 60)
				// utilize a breath first seach to compute the next move 
				// convert the current oil field to a 2D array (studentworld)

				if (path.empty())
				{
					BFS(path, getX(), getY(), 60, 60);
				}
				int d = path.top();
				path.pop();
				switch (d)
				{
				case 1:
					setDirection(up);
					moveTo(getX(), getY() + 1);
					break;
				case 3:
					setDirection(right);
					moveTo(getX() + 1, getY());
					break;
				case 4:
					setDirection(left);
					moveTo(getX() - 1, getY());
					break;
				case 2:
					setDirection(down);
					moveTo(getX(), getY() - 1);
					break;
				default:
					break;
				}
				return;

			}
			return;
		}
		if (currentTick <= ticksToWaitBetweenMoves)
		{
			// 2) if Regular Protester is in a “rest state” during the current tick, it must do
			//nothing other than update its resting tick count, and immediately return
			++currentTick;
			return;
		}
		++nonRestingTick;
		currentTick = 0;
		if (leaveOilField())
		{
			
			//3) Otherwise, if the Regular Protester is in a leave-the-oil-field state (because their
			//hit points reached zero due to being repeatedly squirted by the TunnelMan’s squirt
			//gun or bonked by a falling Boulder), then:
			if (getX() == 60 && getY() == 60)
			{
				/*  a). If the Regular Protester is at its exit point (at location x=60, y=60) then it
					will immediately set its status to dead so that it will be removed from the
					game at the end of the current tick (by your StudentWorld class).
				*/
				setDead();
				return;
			}
			// b). Otherwise, the Regular Protester must move one square closer to its exit
			//point(at x = 60, y = 60)
			// utilize a breath first seach to compute the next move 
			// convert the current oil field to a 2D array (studentworld)
			
			if (path.empty())
			{
				BFS(path, getX(), getY(), 60, 60);
			}
			int d = path.top();
			path.pop();
			switch (d)
			{
			case 1:
				setDirection(up);
				moveTo(getX(), getY() + 1);
				break;
			case 3:
				setDirection(right);
				moveTo(getX() + 1, getY());
				break;
			case 4:
				setDirection(left);
				moveTo(getX() - 1, getY());
				break;
			case 2:
				setDirection(down);
				moveTo(getX(), getY() - 1);
				break;
			default:
				break;
			}
			return;

		}
		
		 // by this point the protesor is neither in a resting state or dead so incremement non resting tick
		++tickSinceLastPerpendicularTurn;
		if (tunnelManWithinFourUnits() && isProtestorFacingTunnelMan())
		{
			if (nonRestingTick >= 15)
			{
				// If both are true and the Regular Protester hasn’t shouted within its last
				//non - resting 15 ticks3
				//std::cout << "Hit" << std::endl;
				nonRestingTick = 0; // reset the nonRestingTick to 0
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->annoy(2);
				return;
				/*
				a. Play the shouting sound: SOUND_PROTESTER_YELL
				b. Inform the TunnelMan that he’s been annoyed for a total of 2 annoyance
				   points (deducting 2 points from the TunnelMan’s hit points, and possibly
				c. Update some state variable in a manner that prevents this Regular
				   Protester from shouting again for at least 15 non-resting ticks.
				d. Return immediately.
				*/
			}
		}
		
		int x = getX();
		int y = getY();
		if (getID() == TID_HARD_CORE_PROTESTER)
		{
			int tunX = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX();
			int tunY = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY();
			int level = getWorld()->getLevel();
			int M = 16 + level * 2;
			int distance = sqrt((((tunX - x) * (tunX - x))) + ((tunY - y) * (tunY - y)));
			if (distance < M)
			{
				std::stack<int> p;
				BFS(p, x, y, tunX, tunY);
				int level = getWorld()->getLevel();
				int M = 16 + level * 2;
				if (static_cast<int>(p.size()) < M) // we can move the Hardcore protestor then 
				{
					//std::cout << "Stack Size is " << p.size() << " " << "M is " << M << '\n';
					//std::cout << "Doing A DFS SEARCH" << '\n';
					if (!p.empty())
					{
						int d = p.top();
						p.pop();
						switch (d)
						{
						case 1:
							setDirection(up);
							//std::cout << "going up!" << '\n';
							moveTo(getX(), getY() + 1);
							break;
						case 3:
							setDirection(right);
							//std::cout << "going right!" << '\n';
							moveTo(getX() + 1, getY());
							break;
						case 4:
							setDirection(left);
							//std::cout << "going left!" << '\n';
							moveTo(getX() - 1, getY());
							break;
						case 2:
							setDirection(down);
							//std::cout << "going down!" << '\n';
							moveTo(getX(), getY() - 1);
							break;
						default:
							break;
						}
						return;
					}
					return;
				}
			}
		}

		if (!tunnelManWithinFourUnits() && canSeeTunnelMan() && !dynamic_cast<StudentWorld*>(getWorld())->isEarthOrBoulderBlocking(getX(), getY()))
		{
			/*
			Then the Regular Protester will:
				a. Change its direction to face in the direction of the TunnelMan, AND then
				   take one step toward him.
				b. The Regular Protester will set its numSquaresToMoveInCurrentDirection
				   value to zero, forcing it to pick a new direction/distance to move during its
				   ext non-resting tick (unless of course, the Regular Protester still sees the
				   TunnelMan in its line of sight, in which case it will continue to move toward
				the TunnelMan).
			*/
		
			if (dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX() == getX()) // if tunnel man above or below protesor
			{
				if (dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY() > getY()) // if tunnel man is above protestor
				{
					setDirection(up); // face up
					moveTo(getX(), getY() + 1);
					numSquaresToMoveInCurrentDirection = 0;
					return;
				}
				else
				{
					setDirection(down); // otherwise tunnelman is below protestor so face down
					moveTo(getX(), getY() - 1);
					numSquaresToMoveInCurrentDirection = 0;
					return;
				}
			}
			if (dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY() == getY()) // if tunnel man in front of or behind protestor
			{
				if(dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX() > getX()) // if tunnel man is in front protestor
				{
					setDirection(right); // face up
					moveTo(getX() + 1, getY());
					numSquaresToMoveInCurrentDirection = 0;
					return;
				}
				else
				{
					setDirection(left); // otherwise tunnelman is behind protestor so face down
					moveTo(getX() - 1, getY());
					numSquaresToMoveInCurrentDirection = 0;
					return;
				}
			}
			return;

		}
		--numSquaresToMoveInCurrentDirection;
		
		//std::cout << "x is: " << x << '\n';
		//std::cout << "y is: " << y << '\n';
		bool leftInBounds = outOfBounds(x - 1, y);
		bool leftNonBlocking = leftInBounds && !dynamic_cast<StudentWorld*>(getWorld())->isEarthOrBoulderBlocking(x - 1, y);
		//std::cout << "leftNonBlocking: " << leftNonBlocking << std::endl;

		bool rightInBounds = outOfBounds(x + 1, y);
		bool rightNonBlocking = rightInBounds && !dynamic_cast<StudentWorld*>(getWorld())->isEarthOrBoulderBlocking(x + 1, y);
		//std::cout << "rightNonBlocking: " << rightNonBlocking << std::endl;
		bool upInBounds = outOfBounds(x, y + 1);
		bool upNonBlocking = upInBounds && !dynamic_cast<StudentWorld*>(getWorld())->isEarthOrBoulderBlocking(x, y + 1);
		//std::cout << "upNonBlocking: " << upNonBlocking << std::endl;
		bool downInBounds = outOfBounds(x, y - 1);
		bool downNonBlocking = downInBounds && !dynamic_cast<StudentWorld*>(getWorld())->isEarthOrBoulderBlocking(x, y - 1);
		//std::cout << "downNonBlocking: " << downNonBlocking << std::endl;
		
		if (numSquaresToMoveInCurrentDirection <= 0)
		{
			/*
				Otherwise, the Regular Protester can’t directly see the TunnelMan. As such, it will
				decrement its numSquaresToMoveInCurrentDirection variable by one. If the Regular
				currently-selected direction (i.e., numSquaresToMoveInCurrentDirection <= 0),
				then:
			*/
			int count = 0;
			bool changedDirection = false;
			do 
			{
				int chance = rand() % 4 + 1; // generates a number 1 through 4
				if (chance == 1 && upNonBlocking) // up
				{
					changedDirection = true;
					setDirection(up);
					numSquaresToMoveInCurrentDirection = calculateNumSquares();
				}
				if (chance == 2 && downNonBlocking) // down
				{
					changedDirection = true;
					setDirection(down);
					numSquaresToMoveInCurrentDirection = calculateNumSquares();
				}
				if (chance == 3 && rightNonBlocking) // right
				{
					changedDirection = true;
					setDirection(right);
					numSquaresToMoveInCurrentDirection = calculateNumSquares();
				}
				if (chance == 4 && leftNonBlocking) // left
				{
					changedDirection = true;
					setDirection(left);
					numSquaresToMoveInCurrentDirection = calculateNumSquares();
				}
				++count;
			} while ((count < 10) && !changedDirection);
		}
		else
		{
			Direction dir = getDirection();
			switch (dir)
			{
			case up: // if he is facing up he can turn left or right
			case down: //if he is down up he can turn left or right
				if (tickSinceLastPerpendicularTurn >= 200 &&
					(rightNonBlocking|| leftNonBlocking))
				{
					tickSinceLastPerpendicularTurn = 0;
				
				
					if (rightNonBlocking && leftNonBlocking) // if both directions are viable pick one randomly
					{
						int chance = rand() % 2;
						if (chance == 1) // pick up
						{
							setDirection(right);
							numSquaresToMoveInCurrentDirection = calculateNumSquares();
						}
						else
						{
							setDirection(left);
							numSquaresToMoveInCurrentDirection = calculateNumSquares();
							// else pick down
						}
						break;
					}
					if (rightNonBlocking)
					{
						setDirection(right);
						numSquaresToMoveInCurrentDirection = calculateNumSquares();
					}
					else
					{
						setDirection(left);
						numSquaresToMoveInCurrentDirection = calculateNumSquares();
					}
				}
				break;
			case left: // if he is facing left he can turn up or down
			case right: // if he is facing right he can turn up or down 
				if (tickSinceLastPerpendicularTurn >= 200 &&
					(upNonBlocking || leftNonBlocking))
				{
					tickSinceLastPerpendicularTurn = 0;
			
					if (upNonBlocking && downNonBlocking) // if both directions are viable pick one randomly
					{
						int chance = rand() % 2;
						if (chance == 1) // pick up
						{
							setDirection(up);
							numSquaresToMoveInCurrentDirection = calculateNumSquares();
						}
						else
						{
							setDirection(down);
							numSquaresToMoveInCurrentDirection = calculateNumSquares();
							// else pick down
						}
						break;
					}
					if (upNonBlocking)
					{
						setDirection(up);
						numSquaresToMoveInCurrentDirection = calculateNumSquares();
					}
					else
					{
						setDirection(down);
						numSquaresToMoveInCurrentDirection = calculateNumSquares();
					}
				}
				break;
			}
		}
		// no conditional statement needed
		Direction d = getDirection();
		switch (d)
		{
		case up:
			if (upNonBlocking)
			{
				moveTo(getX(), getY() + 1);
			}
			else
			{
				numSquaresToMoveInCurrentDirection = 0;
			}
			break;
		case down:
			if (downNonBlocking)
			{
				moveTo(getX(), getY() - 1);
			}
			else
			{
				numSquaresToMoveInCurrentDirection = 0;
			}
			break;
		case right:
			if (rightNonBlocking)
			{
				moveTo(getX() + 1, getY());
			}
			else
			{
				numSquaresToMoveInCurrentDirection = 0;
			}
			break;
		case left:
			if (leftNonBlocking)
			{
				moveTo(getX() - 1, getY());
			}

			break;

		}
		
		
	}
	return;
}
void Protestor::annoy(int x)
{
	if (!leaveOilField()) // if protestor is already dead dont play sound
	{
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	}
	m_hp -= x;
}
bool Protestor::isTunnelManOverlap()
{
	int tunnelManX = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getX();
	int tunnelManY = dynamic_cast<StudentWorld*>(getWorld())->getTunnelMan()->getY();
	if (((tunnelManX + 3 >= getX()) && (tunnelManX <= getX() + 3)))
	{
		if ((tunnelManY + 3 >= getY()) && (tunnelManY <= getY() + 3))
		{
			return true;
		}
	}
	return false;
}
/*
void Protestor::setDead()
{
	Actor::setDead();
}
*/
void Protestor::bribe()
{
	isBribed = true;
}
void Protestor::boulderHitProtestor()
{
	hasBoulderHit = true;
}
void Protestor::playAnnoySound()
{
	getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
}
void Protestor::gotSquirted()
{
	// reintialize stun timer every time we get hit by squirt
	int level = getWorld()->getLevel();
	stunTimer = std::max(50, 100 - level * 10);
	if (m_hp <= 0)
	{
		if (getID() == TID_HARD_CORE_PROTESTER)
		{
			getWorld()->increaseScore(250);
		}
		else
		{
			getWorld()->increaseScore(100);
		}
	}
}
void Protestor::setHP(int x)
{
	m_hp = x;
}

RegularProtestor::RegularProtestor(GameWorld* world, int startX, int startY) : Protestor(world, TID_PROTESTER, startX, startY, left, 1.0, 0)
{
	setHP(5); // set regular protestor hp to 5
}
RegularProtestor::~RegularProtestor()
{

}
void RegularProtestor::doSomething()
{
	// call protestor's do something
	Protestor::doSomething();
}

HardCoreProtestor::HardCoreProtestor(GameWorld* world, int startX, int startY): Protestor(world, TID_HARD_CORE_PROTESTER, startX, startY, left, 1.0, 0)
{
	setHP(20); // sets HardCore Protestor hp in base class to 20
	ticks_to_stare = 0;
}
HardCoreProtestor::~HardCoreProtestor()
{

}
void HardCoreProtestor::doSomething()
{
	if (ticks_to_stare > 0)
	{
		--ticks_to_stare;
	}
	else
	{
		Protestor::doSomething();
	}
}

void HardCoreProtestor::bribe()
{
	int level = getWorld()->getLevel();
	ticks_to_stare = std::max(50, 100 - level * 10);

}


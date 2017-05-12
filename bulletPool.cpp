#include <iostream>
#include <sstream>
#include <cmath>
#include "ioMod.h"
#include "gamedata.h"
#include "bulletPool.h"
#include "player3.h"


CollisionStrategy* getStrategy(const string& name) {
  std::string sName = Gamedata::getInstance().getXmlStr(name+"/strategy");
  if ( sName == "midpoint" ) return new MidPointCollisionStrategy;
  if ( sName == "rectangular" ) return new RectangularCollisionStrategy;
  if ( sName == "perpixel" ) return new PerPixelCollisionStrategy;
  throw std::string("No strategy in getStrategy");
} 


BulletPool::~BulletPool() {
  delete strategy;
}

BulletPool::BulletPool(const std::string& n) :
  name(n),
  strategy( getStrategy(name) ),
  frameInterval(Gamedata::getInstance().getXmlInt(name+"/interval")),
  timeSinceLastFrame( 0 ),
  bulletList(),
  freeList(),
  flipType(SDL_FLIP_NONE)
{ }

BulletPool::BulletPool(const BulletPool& b) :
  name(b.name),
  strategy(b.strategy),
  frameInterval(b.frameInterval),
  timeSinceLastFrame( b.timeSinceLastFrame ),
  bulletList(b.bulletList), 
  freeList(b.freeList),
  flipType(b.flipType)
{ }

bool BulletPool::collidedWith(const Drawable* obj) const {
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    if ( strategy->execute(*ptr, *obj) ) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}

void BulletPool::shoot(const Vector2f& position, const Vector2f& velocity) {
	if (timeSinceLastFrame > frameInterval) {
    // If no bullets in pool, make one:
    if ( freeList.empty() ) {
      Bullet b(name);
      b.setPosition( position );
      if(flipType == SDL_FLIP_NONE) {
        b.setVelocity( velocity );
      }
      else {
        b.setVelocity( -velocity );
      }
      bulletList.push_back( b );
    }
    else {
      Bullet b = freeList.front();
      freeList.pop_front();
      b.reset();
      if(flipType == SDL_FLIP_NONE) {
        b.setVelocity( velocity );
      }
      else {
        b.setVelocity( -velocity );
      }
      b.setPosition(position);
      bulletList.push_back( b );
    }
		timeSinceLastFrame = 0;
	}
}

void BulletPool::draw() const { 
  std::stringstream stream;
  SDL_Color color = {255,255,255,255};



   stream << "(Missles)";
  IOmod::getInstance().
    writeText(stream.str(), color, 10, 10);
  stream.clear();
  stream.str("");

  stream << "Active: " << bulletList.size();
  IOmod::getInstance().
    writeText(stream.str(), color, 10, 40);
  stream.clear();
  stream.str("");

  stream << "Pooled: " << freeList.size();
  IOmod::getInstance().
    writeText(stream.str(), color, 10, 70);



  for ( Bullet bullet : bulletList ) { 
      if(flipType == SDL_FLIP_HORIZONTAL) {
        
       // std::cout << "flip: " << flipType << std::endl;
      }
      bullet.draw();
  }
}

void BulletPool::update(Uint32 ticks) { 
	timeSinceLastFrame += ticks;
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    ptr->setFlip(flipType);
    if (ptr->goneTooFar()) {  // Check to see if we should free a Bullet
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }   
    else ++ptr;
  }
}

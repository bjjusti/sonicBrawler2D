#include <cmath>
#include <random>
#include <functional>
#include "enemy.h"
#include "gamedata.h"
#include "renderContext.h"


const float PI = 4.0f*std::atan(1.0f);

using RADRAND_t = decltype(std::bind(std::declval<std::uniform_real_distribution<float> >(),std::declval<std::mt19937>()));
using NORRAND_t = decltype(std::bind(std::declval<std::normal_distribution<float> >(),std::declval<std::mt19937>()));

std::mt19937 getRand2(){
  static std::random_device rd;
  return std::mt19937(rd());
}

RADRAND_t getRadianDist2(){
  return std::bind(std::uniform_real_distribution<float>(0.0f,2.0f*PI),getRand2());
}

NORRAND_t getNormalDist2(float u, float dev){
  return std::bind(std::normal_distribution<float>(u,dev),getRand2());
}


Vector2f Enemy::makeVelocity(int vx, int vy) const {
  static auto rad = getRadianDist2();
  auto nor = getNormalDist2(vx,vy);

  float v_rad = rad();
  float v_mag = nor();
  
  return v_mag*Vector2f(std::cos(v_rad),std::sin(v_rad));
}

void Enemy::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Enemy::Enemy( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( RenderContext::getInstance()->getFrames(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  flipType(SDL_FLIP_NONE),
  scale(1)
{ }

Enemy::Enemy(const Enemy& s) :
  Drawable(s), 
  frames(s.frames),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  flipType(SDL_FLIP_NONE),
  scale(s.scale)
  { }

inline namespace {
  constexpr float SCALE_EPSILON = 2e-7;
}

void Enemy::draw() const { 
  if(getScale() < SCALE_EPSILON) return;
  if(flipType == SDL_FLIP_NONE) {
    frames[currentFrame]->draw(getX(), getY(), getScale());
  }
  else {
    frames[currentFrame]->drawFlip(getX(), getY(), getScale());
  }
}

void Enemy::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() > 450) {
    setVelocityY( -fabs( getVelocityY() ) );
  }
  if ( getY() < 375) {
    setVelocityY( fabs( getVelocityY() ) ); 
  }

  if ( getX() < 0) {
      setVelocityX( fabs( getVelocityX() ) );
      flipType = SDL_FLIP_NONE;
  }
  if ( getX() > worldWidth-frameWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
    flipType = SDL_FLIP_HORIZONTAL;
  }

  if(getVelocityX() < 0) {
    flipType = SDL_FLIP_HORIZONTAL;
  }  

}


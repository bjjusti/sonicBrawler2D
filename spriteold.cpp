#include <cmath>
#include <random>
#include <functional>
#include "sprite.h"
#include "gamedata.h"
#include "renderContext.h"

const float PI = 4.0f*std::atan(1.0f);

using RADRAND_t = decltype(std::bind(std::declval<std::uniform_real_distribution<float> >(),std::declval<std::mt19937>()));
using NORRAND_t = decltype(std::bind(std::declval<std::normal_distribution<float> >(),std::declval<std::mt19937>()));

std::mt19937 getRand(){
  static std::random_device rd;
  return std::mt19937(rd());
}

RADRAND_t getRadianDist(){
  return std::bind(std::uniform_real_distribution<float>(0.0f,2.0f*PI),getRand());
}

NORRAND_t getNormalDist(float u, float dev){
  return std::bind(std::normal_distribution<float>(u,dev),getRand());
}

Vector2f Sprite::makeVelocity(int vx, int vy) const {
  static auto rad = getRadianDist();
  auto nor = getNormalDist(vx,vy);

  float v_rad = rad();
  float v_mag = nor();

  return v_mag*Vector2f(std::cos(v_rad),std::sin(v_rad));
}


Sprite::Sprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
           ),
  frame( RenderContext::getInstance()->getFrame(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()), 
  scale(1)
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s), 
  frame(s.frame),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  scale(s.scale)
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  scale = rhs.scale;
  return *this;
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void Sprite::draw() const { 
  if(getScale() < SCALE_EPSILON) return;
  frame->draw(getX(), getY()); 
}

void Sprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-frameHeight) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }


  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-frameWidth) {
    setVelocityX( -std::abs( getVelocityX() ) );
  }  
}

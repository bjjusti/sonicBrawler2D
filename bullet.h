#ifndef BULLET_H
#define BULLET_H
#include <iostream>
#include "multisprite.h"
#include "gamedata.h"

class Bullet : public MultiSprite {
public:
  explicit Bullet(const string& name) :
    MultiSprite(name), 
    distance(0), 
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
    tooFar(false),
    flipType(SDL_FLIP_NONE) 
  { }
  
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }
  void setFlip(SDL_RendererFlip x) {
    flipType = x;
  }

private:
  float distance;
  float maxDistance;
  bool tooFar;
  SDL_RendererFlip flipType;
};
#endif

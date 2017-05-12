#ifndef ENEMY__H
#define ENEMY__H
#include <SDL_image.h>
#include <string>
#include <vector>
#include <cmath>
#include <list>
#include "drawable.h"


class Enemy : public Drawable {
public:
  Enemy(const std::string&);
  Enemy(const Enemy&);
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  void setFlip(SDL_RendererFlip x) {
    flipType = x;
  }
  SDL_RendererFlip getFlip() {
    return flipType;
  
  }

protected:
  const std::vector<Frame *> frames;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  SDL_RendererFlip flipType;
  float scale;
  Vector2f makeVelocity(int,int) const;
  void advanceFrame(Uint32 ticks);
};
#endif

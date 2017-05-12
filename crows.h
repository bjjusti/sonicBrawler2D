#ifndef CROWS__H
#define CROWS__H
#include <SDL_image.h>
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class Crows : public Drawable {
public:
  Crows(const std::string&);
  Crows(const Crows&);
  

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }

  inline float getScale()const{ return scale; }
  void  setScale(float s){ scale = s; }

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
  int getDistance(const Crows*) const;
  void advanceFrame(Uint32 ticks);
};
#endif

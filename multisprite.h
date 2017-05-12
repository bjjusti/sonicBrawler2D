#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <SDL_image.h>
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&);
  MultiSprite(const MultiSprite&);
  SDL_RendererFlip flipType;
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
  

  void advanceFrame(Uint32 ticks);
};
#endif

#ifndef COINSPRITE__H
#define COINSPRITE__H
#include <SDL_image.h>
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "viewport.h"

class CoinSprite : public Drawable {
public:
  CoinSprite(const std::string&);
  CoinSprite(const CoinSprite&);
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
  void stop();
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

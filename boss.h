#ifndef BOSS__H
#define BOSS__H
#include <SDL_image.h>
#include <string>
#include <vector>
#include <cmath>
#include <list>
#include <SDL.h>
#include <SDL_ttf.h>
#include <sstream>
#include "drawable.h"
#include "viewport.h"


class Boss : public Drawable {
public:
  Boss(const std::string&);
  Boss(const Boss&);
  //~Boss();
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
  void writeHP(const std::string& msg, SDL_Color c, int x, int y) const;
  void drawHP();
  void sDamage();
  void tDamage();
  int getHP();

protected:
  const std::vector<Frame *> frames;
  TTF_Font* font;
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
  int HP;
  int maxHP;
  void advanceFrame(Uint32 ticks);
};
#endif

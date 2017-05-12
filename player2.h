#ifndef PLAYER2__H
#define PLAYER2__H
#include <SDL_image.h>
#include <string>
#include <vector>
#include <cmath>
//#include "clock.h"
#include "drawable.h"
#include "player.h"

class Player2 : public Player {
public:
  Player2(const std::string&);
  Player2(const Player2&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  virtual void stop();
  virtual void right();
  virtual void left();
  virtual void up(); 
  virtual void down();
  virtual void idle();
  virtual void reset();

protected:
  std::vector<Frame *> frames;
  
  //Clock& clock;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float minFrameDelay;
  float frameDelay;
  Vector2f initialVelocity;
  const float slowDown;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  SDL_RendererFlip flipType;
  Player& operator=(const Player&);
  void advanceFrame(Uint32 ticks);
};
#endif

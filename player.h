#ifndef PLAYER__H
#define PLAYER__H
#include <SDL_image.h>
#include <string>
#include <vector>
#include <cmath>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <sstream>
#include "drawable.h"
#include "collisionStrategy.h"
#include "viewport.h"

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);
 // ~Player();
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
  virtual bool collidedWith(const Drawable*) const;
  void writeHP(const std::string& msg, SDL_Color c, int x, int y) const;
  void drawHP();
  int  getHP();
  void punch();
  void special();
  void enemyHurt();
  void bossHurt();
  const Vector2f& getPlayerPos() const;
  bool pFlag; 
  //bool mFlag;
  unsigned currentFrame;
  TTF_Font* font;
 

protected:
  std::vector<Frame *> frames;
  //std::vector<Frame *> idlef;
  //std::vector<Frame *> punch1;
  //std::vector<Frame *> punch2;
  //std::vector<Frame *> punch3;
  
  //Clock& clock;
 

 
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
  int punchCount;
  int HP;
  int maxHP;
  SDL_RendererFlip flipType;
  CollisionStrategy* strategy;
  Player& operator=(const Player&);
  void advanceFrame(Uint32 ticks);
};
#endif

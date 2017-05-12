#ifndef PLAYER3__H
#define PLAYER3__H
#include <SDL_image.h>
#include <string>
#include <vector>
#include <cmath>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <sstream>
//#include "clock.h"
#include "drawable.h"
#include "player.h"
#include "bulletPool.h"
#include "viewport.h"

class Player3 : public Player {
public:
  Player3(const std::string&);
  Player3(const Player3&);
  ~Player3();
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
  void setpFlag(bool);
  bool getpFlag();
  unsigned getcFrame();
  void shoot();
  void snapToPlayer();
  void writeHP(const std::string& msg, SDL_Color c, int x, int y) const;
  void drawHP();
  virtual bool collidedWith(const Drawable*) const;
  SDL_RendererFlip getFlip();
  TTF_Font* font;


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
  int count;
  SDL_RendererFlip flipType;
  bool snap;
  Player& operator=(const Player&);
  void advanceFrame(Uint32 ticks);

private:
  std::string bulletName;
  BulletPool bullets;
  float minSpeed;
  bool pFlag;
  int HP;
  int maxHP;
};
#endif

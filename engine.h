#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "hud.h"
#include "warning.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "sound.h"

class CollisionStrategy;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();
  int punchCheck;
  int missleCheck;


private:
  const RenderContext* rc;
  const IOmod& io;
  Hud& hud;
  Warning& warning;
  Clock& clock;
  SDLSound &playSound;
  //Player* p1;
  //Player* p2;
  //Player* p3;

  SDL_Renderer * const renderer;
  World world_b;
  World world_m;
  World world_f;
  Viewport& viewport;

  std::vector<Player*> sprites;
  std::vector<Drawable*> painters;
  std::vector<Drawable*>enemies;
  std::vector<Drawable*> misc;
  std::vector<Drawable*> bossv;
  //std::vector<Drawable*> ctrl;

  
  int coinCount;
  int currentSprite;
  bool makeVideo;
  CollisionStrategy* strategy;
  int collisions;
  bool hudFlag;
  bool sFlag;
  bool gFlag;
  void draw();
  void update(Uint32);
  void printScales() const;
  void checkForCollisions();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
};

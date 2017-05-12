#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "crows.h"
#include "player.h"
#include "player2.h"
#include "player3.h"
#include "coinsprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "sound.h"
#include "explodingSprite.h"
#include "explodingSpriteB.h"
#include "multisprite.h"


class CrowLThan {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) 
const {
    return lhs->getScale() < rhs->getScale();
  }
};


Engine::~Engine() { 
  std::cout << "Terminating program" << std::endl;
  for(auto* s: sprites) {
    delete s;
  }
  for(auto* p: painters) {
    delete p;
  }
  for(auto* e: enemies) {
    delete e;
  }
  for(auto* m: misc) {
    delete m;
  }
  for(auto *b: bossv) {
    delete b;
  }
  delete strategy;
}

Engine::Engine() :
  punchCheck(1),
  missleCheck(1),
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  hud(Hud::getInstance()),
  warning(Warning::getInstance()),
  clock( Clock::getInstance() ),
  playSound(SDLSound::getInstance()),
  renderer( rc->getRenderer() ),
  world_b("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  world_m("middle", Gamedata::getInstance().getXmlInt("middle/factor") ),
  world_f("front", Gamedata::getInstance().getXmlInt("front/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  painters(),
  enemies(),
  misc(),
  bossv(),
  coinCount(0),
  currentSprite(-1),
  makeVideo( false ),
  strategy( new RectangularCollisionStrategy),
  collisions(0),
  hudFlag(false),
  sFlag(false),
  gFlag(false)
{
  constexpr float u = 0.5f; //mean size
  constexpr float d = 0.25f; //std dev

  std::random_device rd;
  std::mt19937 mt(rd());
  std::normal_distribution<float> dist(u,d);
  unsigned int n = Gamedata::getInstance().getXmlInt("crowNum");

  for(unsigned int i = 0; i<n; ++i) { //create n scaled crows
    auto* s = new Crows("crow");
    float scale = dist(mt);
    while(scale < 0.1f) scale = dist(mt);
    s->setScale(scale);
    painters.push_back(s);
  }
   
  std::vector<Drawable*>::iterator ptr = painters.begin();
  sort(ptr, painters.end(), CrowLThan()); //sorts crows 
  

  unsigned int m = Gamedata::getInstance().getXmlInt("enemyNum");
  for(unsigned int i = 0; i<m; ++i) { //create m enemies
    auto* s = new Enemy("enemy");
    enemies.push_back(s);
  }

  bossv.push_back( new Boss("boss")); 
  sprites.push_back( new Player("sonic"));
  //sprites.push_back( new Player2("knuckles"));
  sprites.push_back( new Player3("tails"));
  misc.push_back( new CoinSprite("rings"));
  switchSprite();
}

void Engine::printScales() const {
  for (Drawable* s : enemies) {
    std::cout << s->getScale() << std::endl;
  }
}

void Engine::draw() {
  SDL_Color color = {0,255,255,0};

  world_b.draw();
  for(auto * p : painters) {
    if(p->getScale() < 0.4f) {
      p->draw();
    }
  }

  world_m.draw();
  for(auto * m : painters) {
    if(m->getScale() >=0.4f && m->getScale() <=0.5) {
      m->draw();
    }
  }

  world_f.draw();
  for(auto * x : painters) {
    if(x->getScale() > 0.5f) {
      x->draw();
    }
  }

  for(auto * e : enemies) {
    if(e != NULL) {
      e->draw();
    }
  }

  for(auto *r: misc) {
    r->draw();
  }

  


  //draw name in bottom left corner
  int posY = Gamedata::getInstance().getXmlInt("view/height");
  const std::string title = Gamedata::getInstance().getXmlStr("screenTitle");
  io.writeText(title, color, 10, (posY-30));


  //draw coin count
  std::stringstream stream;
  SDL_Color color2 = {0xff,255,0,0};
  stream << coinCount << " coins";
  IOmod::getInstance().
    writeText(stream.str(), color2, 365, 40);

  //draw boss
  
  unsigned int em = Gamedata::getInstance().getXmlInt("enemyNum");

  if((unsigned)coinCount >= em*5 && static_cast<Boss*>(bossv[0])->getHP() >= 0) {
     if(bossv[0] != NULL) {
       ExplodingSpriteB* end = dynamic_cast<ExplodingSpriteB*>(bossv[0]);
     
       if(!end) {
        bossv[0]->draw();
       }
       else if(end) {
         std::stringstream streamend;
         streamend << "You WON!" ;
         IOmod::getInstance().writeText(streamend.str(), color2, 365, 60);
         
         std::stringstream streamend2;
         streamend2 << "Press 'r' to restart, or [ESC] to quit";
         IOmod::getInstance().writeText(streamend2.str(), color2, 365, 90);

       }  
     }
   
    if(static_cast<Boss*>(bossv[0])->getHP() > 0) {
      static_cast <Boss*>(bossv[0])->drawHP();
    }
 
  }

  


  //sonic punch & special animations  
  sprites[0]->draw();
  static_cast <Player*>(sprites[0])->drawHP();

  if(punchCheck == 1 && sprites[0]->pFlag == true) {
    punchCheck = 2;
  }
  if(punchCheck == 2 && sprites[0]->pFlag == true){
    if(sprites[0]->currentFrame < 6) {
      sprites[0]->pFlag = true;
    }
    else {
      sprites[0]->pFlag = false;
      punchCheck = 1;
    }    
  }

 
  //tails missle animations
  sprites[1]->draw();
  static_cast <Player3*>(sprites[1])->drawHP();
  
  if(missleCheck == 1 && static_cast<Player3*>(sprites[1])->getpFlag() == true) {
    missleCheck = 2;
  }
  if(missleCheck == 2 && static_cast<Player3*>(sprites[1])->getpFlag() == true){
    if(static_cast<Player3*>(sprites[1])->getcFrame() < 7) {
      static_cast<Player3*>(sprites[1])->setpFlag(true);
    }
    else {
      static_cast<Player3*>(sprites[1])->setpFlag(false);
      missleCheck = 1;
    }    
  }



  if(clock.getSeconds() <= 5) {
    hud.drawHud();
    std::stringstream streamy;
    SDL_Color colory = {255,255,255,255};
    streamy << "Defeat all the enemies!";
    IOmod::getInstance().
    writeText(streamy.str(), colory, 320, 100);
  }
  if(hudFlag == true && clock.getSeconds()>5) {
    hud.drawHud();
  }

  if(sFlag == true && coinCount < 100) {
    warning.drawWarning();
  }

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto* s : sprites) s->update(ticks);
  for(auto* p : painters) p->update(ticks);
  for(auto* e : enemies) e->update(ticks);
  for(auto* r : misc) r->update(ticks);
  unsigned int em = Gamedata::getInstance().getXmlInt("enemyNum");
  
  if((unsigned)coinCount >= em*5 && static_cast<Boss*>(bossv[0])->getHP() >= 0) {
    if(bossv[0] != NULL) {
      ExplodingSpriteB* end = dynamic_cast<ExplodingSpriteB*>(bossv[0]);
     
      if(!end) {
        for(auto* b : bossv) b->update(ticks);
      }
    
    }
  }
  else {//
  }
  world_f.update();
  world_m.update();
  world_b.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  if((unsigned)currentSprite > sprites.size()) {
    currentSprite = 0;
  }
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}


void Engine::checkForCollisions() {
  for(unsigned i = 0; i<enemies.size(); ++i) {
    Enemy* en = static_cast<Enemy*>(enemies[i]);
    ExplodingSprite* es = dynamic_cast<ExplodingSprite*>(enemies[i]);

    if (!es && static_cast<Player3*>(sprites[1])->collidedWith(en)) {//tails bullet
      ++collisions;
      enemies[i] = new ExplodingSprite(*en);
      delete(en);
      coinCount += 5;

    }
    else if (!es && static_cast<Player*>(sprites[0])->collidedWith(en) && sprites[0]->pFlag == true) {//sonic attack
      ++collisions;
      enemies[i] = new ExplodingSprite(*en);
      delete(en); 
      coinCount += 5;
    }
    else if(es && es->chunkCount() == 0) {
      enemies.erase(enemies.begin() + i);
      delete(es);
    }
    else if (!es && static_cast<Player*>(sprites[0])->collidedWith(en) && sprites[0]->pFlag == false) {
       if(gFlag != true) {
         sprites[0]->enemyHurt();
         playSound[2];
         ++collisions;
       }
    }
  }

//boss collisions
    Boss* b = static_cast<Boss*>(bossv[0]);
    ExplodingSpriteB* bs = dynamic_cast<ExplodingSpriteB*>(bossv[0]);

    if (!bs && static_cast<Player3*>(sprites[1])->collidedWith(b)) {//tails bullet
      ++collisions;
      if((static_cast<Boss*>(bossv[0])->getHP()) <= 0) { 
        bossv[0] = new ExplodingSpriteB(*b);
        delete(b);
      }
      else {
        static_cast<Boss*>(bossv[0])->tDamage();
      }
      

    }
    else if (!bs && static_cast<Player*>(sprites[0])->collidedWith(b) && sprites[0]->pFlag == true) {//sonic attack
      ++collisions;
      if((static_cast<Boss*>(bossv[0])->getHP()) <= 0) { 
        bossv[0] = new ExplodingSpriteB(*b);
        delete(b);
      }
      else {
        static_cast<Boss*>(bossv[0])->sDamage();
      } 
     
    }
    else if(bs && bs->chunkCount() == 0) {
      bossv.erase(bossv.begin());
      delete(bs);
    }
    else if (!bs && static_cast<Player*>(sprites[0])->collidedWith(b) && sprites[0]->pFlag == false) {
       if(gFlag != true) {
         sprites[0]->bossHurt();
         playSound[2];
         ++collisions;
       }
    }
    else {
    //
    }
 


}
void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
  SDLSound sound;

  while ( !done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
	if (keystate[SDL_SCANCODE_R]) {
           Engine engine;
           engine.play();
           done = true;
	}
        if(sprites[0]->getHP() <= 0){
          Engine engine;
          engine.play();
          done = true;
        }
        if (keystate[SDL_SCANCODE_G]){
          playSound[4];
          if(gFlag == false) {
            gFlag = true;
            coinCount += 100;
          }
          else if(gFlag == true) {
            gFlag = false;
          }
        } 
        if (keystate[SDL_SCANCODE_U]) {
          sprites[0]->punch();
          playSound[1];
        }   
        if (keystate[SDL_SCANCODE_I]) {
          if (sFlag == false) {
            if(coinCount >= 100) { 
              sprites[0]->special();
              playSound[3];
            }
            sFlag = true;
          }
          else {
             if(coinCount >= 100) { 
              sprites[0]->special();
              playSound[3];
             }
             sFlag = false;
          }
        }  
        if ( keystate[SDL_SCANCODE_M] ) {
	  sound.toggleMusic();
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          sprites[1]->reset();
          static_cast<Player3*>(sprites[1])->shoot();
          playSound[0];
        }
        if (keystate[SDL_SCANCODE_F1]) {
          if(hudFlag == false) {
            hudFlag = true;
          }
          else if(hudFlag == true) {
            hudFlag = false;
          }
        }

        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_J] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }
    if (keystate[SDL_SCANCODE_A] && keystate[SDL_SCANCODE_D]) {
      for(auto* s : sprites) s->idle(); 
     }
    else if (keystate[SDL_SCANCODE_A]) {
      for(auto* s : sprites) { 
        s->reset();
        s->left();
      }
    }
    else if (keystate[SDL_SCANCODE_D]) {
      for(auto* s : sprites) { 
        s->reset();
        s->right();
      }
    }
    if (keystate[SDL_SCANCODE_W] && keystate[SDL_SCANCODE_S]) {
      for(auto* s : sprites) s->idle();
    }
    else if (keystate[SDL_SCANCODE_W]) {
      for(auto* s : sprites) { 
        s->reset();
        s->up();
      }   
    }
    else if (keystate[SDL_SCANCODE_S]) {
      for(auto* s : sprites) {
        s->reset();
        s->down();
      }
    }
   
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      draw();
      update(ticks);
      checkForCollisions();
    
      if(coinCount == 100) { 
        sound.switchMusic();
      }
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}

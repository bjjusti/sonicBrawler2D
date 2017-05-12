#include <cmath>
#include <random>
#include <functional>
#include "boss.h"
#include "gamedata.h"
#include "renderContext.h"



void Boss::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Boss::Boss( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( RenderContext::getInstance()->getFrames(name) ),
  font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font2/file").c_str(),Gamedata::getInstance().getXmlInt("font2/size"))),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  flipType(SDL_FLIP_NONE),
  scale(1),
  HP(1500),
  maxHP(1500)
{ }

Boss::Boss(const Boss& s) :
  Drawable(s), 
  frames(s.frames),
  font(s.font),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  flipType(SDL_FLIP_NONE),
  scale(s.scale),
  HP(s.HP),
  maxHP(s.maxHP)
  { }

/*
Boss::~Boss() {
  TTF_CloseFont(font);
  TTF_Quit(); 
}*/

void Boss::draw() const { 
  if(flipType == SDL_FLIP_NONE) {
    frames[currentFrame]->draw(getX(), getY(), getScale());
  }
  else {
    frames[currentFrame]->drawFlip(getX(), getY(), getScale());
  }
}


void Boss::sDamage() {
  HP -= 50;

}

void Boss::tDamage() {
  HP -= 20;
}

int Boss::getHP() {
  return HP;

}

void Boss::writeHP(const std::string& msg, SDL_Color c, int x, int y) const {
  int textWidth;
  int textHeight;
  SDL_Renderer* renderer = RenderContext::getInstance()->getRenderer();

  SDL_Surface* surface = 
    TTF_RenderText_Solid(font, msg.c_str(), c);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  textWidth = surface->w;
  textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}


void Boss::drawHP() {

    int distx = getX();
    int disty = getY()-30;
    
    distx -= Viewport::getInstance().getX();
    disty -= Viewport::getInstance().getY();
   
    SDL_Color textColor = {0xff, 255, 0, 0};    
    SDL_Color textColor2 = {0xff, 255, 0, 0};

    if(HP < 500) {
      textColor2 = {0xff, 0, 0, 0};
    }

    SDL_Renderer* renderer = RenderContext::getInstance()->getRenderer();

    std::stringstream strm;
    std::stringstream strm2;
    strm<< HP;
    strm2 << "HP:          /" << maxHP;
    writeHP(strm.str(), textColor2,distx+20, disty);
    writeHP(strm2.str(), textColor, distx, disty);

    SDL_Rect r;
    r.x = distx-5;
    r.y = disty;
    r.w = 90;
    r.h = 30;

    // set the blend mode so that alpha blending will work;
    // the default blend mode is SDL_BLENDMODE_NONE
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/3 );

    // Render rect
    SDL_RenderFillRect( renderer, &r );

    SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
    SDL_RenderDrawRect( renderer, &r );

  
}



void Boss::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() > 450) {
    setVelocityY( -fabs( getVelocityY() ) );
  }
  if ( getY() < 375) {
    setVelocityY( fabs( getVelocityY() ) ); 
  }

  if ( getX() < 0) {
      setVelocityX( fabs( getVelocityX() ) );
      flipType = SDL_FLIP_NONE;
  }
  if ( getX() > worldWidth-frameWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
    flipType = SDL_FLIP_HORIZONTAL;
  }

  if(getVelocityX() < 0) {
    flipType = SDL_FLIP_HORIZONTAL;
  }  

}


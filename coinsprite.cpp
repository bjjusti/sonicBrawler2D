#include "coinsprite.h"
#include "gamedata.h"
#include "renderContext.h"

void CoinSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

CoinSprite::CoinSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  flipType(SDL_FLIP_NONE),
  frames( RenderContext::getInstance()->getFrames(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

CoinSprite::CoinSprite(const CoinSprite& s) :
  Drawable(s), 
  flipType(SDL_FLIP_NONE),
  frames(s.frames),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

void CoinSprite::draw() const{ 
  if(flipType == SDL_FLIP_NONE) {
    frames[currentFrame]->draw(getX()+Viewport::getInstance().getX(),getY()+Viewport::getInstance().getY());
  }
  else {
    frames[currentFrame]->drawFlip(getX()+Viewport::getInstance().getX(),getY()+Viewport::getInstance().getY());
  }
}


void CoinSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-frameHeight) {
    setVelocityY( -fabs( getVelocityY() ) ); 
  }

  if ( getX() < 0) {
      setVelocityX( fabs( getVelocityX() ) );
      flipType = SDL_FLIP_NONE;
  }
  if ( getX() > worldWidth-frameWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
    flipType = SDL_FLIP_HORIZONTAL;
  }  
  
  
}

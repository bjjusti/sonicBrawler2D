#include "player2.h"
#include "gamedata.h"
#include "renderContext.h"

/*Class for Knuckles*/


Player2::Player2( const std::string& name) :
  Player(name),
  frames( RenderContext::getInstance()->getFrames(name) ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  minFrameDelay( Gamedata::getInstance().getXmlFloat(name+"/minFrameDelay")),
  frameDelay(0),
  initialVelocity(getVelocity()),
  slowDown(Gamedata::getInstance().getXmlFloat(name+"/slowDown")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  flipType(SDL_FLIP_NONE)
{ }

Player2::Player2(const Player2& s) :
  Player(s.getName()), 
  frames(s.frames),  
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  minFrameDelay(s.minFrameDelay),
  frameDelay(s.frameDelay),
  initialVelocity(s.initialVelocity),
  slowDown(s.slowDown),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  flipType(SDL_FLIP_NONE)
  { }

void Player2::advanceFrame(Uint32 ticks) {
  if( frameDelay > frameInterval ) {
    timeSinceLastFrame += ticks;
    if (timeSinceLastFrame > frameInterval) {
      currentFrame = (currentFrame+1) % numberOfFrames;
      timeSinceLastFrame = 0;
    }
    frameDelay = ::fmod(frameDelay, frameInterval);
  }
  int magnitude = getVelocity().magnitude();
  if ( magnitude < initialVelocity.magnitude()/2) {
    frameDelay+=minFrameDelay;
  }
  else frameDelay = frameDelay+ticks*magnitude/1000;
}

void Player2::reset(){
  frames = RenderContext::getInstance()->getFrames("knuckles");
  
  numberOfFrames = Gamedata::getInstance().getXmlInt("knuckles/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("knuckles/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("knuckles/minFrameDelay");

 

}


void Player2::idle() {
  stop();
  frames = RenderContext::getInstance()->getFrames("knuckles_idle");
  
  
  numberOfFrames = Gamedata::getInstance().getXmlInt("knuckles_idle/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("knuckles_idle/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("knuckles_idle/minFrameDelay");



}


void Player2::stop() {
  setVelocityX( slowDown*getVelocityX());
  setVelocityY(0);
}

void Player2::right() {
  if(getX() < worldWidth-frameWidth) {
    setVelocityX(-initialVelocity[0]);
    if(flipType==SDL_FLIP_HORIZONTAL) {
      flipType = SDL_FLIP_NONE;
    }
  }
}

void Player2::left() {
  if(getX()>0) {
    setVelocityX(initialVelocity[0]);
    flipType = SDL_FLIP_HORIZONTAL;
  }

}

void Player2::up() {
  if(getY()>380) {
    setVelocityY(-initialVelocity[1]);
  }

}

void Player2::down() {
  if(getY() < worldHeight-frameHeight) {
    setVelocityY(initialVelocity[1]);
  }

}


void Player2::draw() const { 
  if(flipType == SDL_FLIP_NONE) {
    frames[currentFrame]->draw(getX(), getY());
  }
  else {
    frames[currentFrame]->drawFlip(getX(), getY());
  }
}


void Player2::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() < worldHeight-frameHeight) {
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
  idle();

}


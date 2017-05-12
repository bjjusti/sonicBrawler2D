#include "player.h"
#include "gamedata.h"
#include "renderContext.h"

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  pFlag(false),
  mFlag(false),
  currentFrame(0),
  flipType(SDL_FLIP_NONE),
  frames( RenderContext::getInstance()->getFrames(name) ),
  idlef( RenderContext::getInstance()->getFrames("sonic_idle") ),
  punch1( RenderContext::getInstance()->getFrames("sonic_p1") ),
  punch2( RenderContext::getInstance()->getFrames("sonic_p2") ),
  punch3( RenderContext::getInstance()->getFrames("sonic_p3") ),
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
  punchCount(1)
{ }

Player::Player(const Player& s) :
  Drawable(s), 
  pFlag(s.pFlag),
  mFlag(s.mFlag),
  currentFrame(s.currentFrame),
  flipType(s.flipType),
  frames(s.frames), 
  idlef(s.idlef),
  punch1(s.punch1),
  punch2(s.punch2),
  punch3(s.punch3), 
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
  punchCount( s.punchCount )
{}

void Player::advanceFrame(Uint32 ticks) {
  if( frameDelay > frameInterval ) {
    timeSinceLastFrame += ticks;
    if (timeSinceLastFrame > frameInterval) {
      currentFrame = (currentFrame+1) % numberOfFrames;
      //std::cout<<"Player frame: " << cGurrentFrame << std::endl;
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



void Player::reset(){
  if(pFlag == true) {
    pFlag = false;
  }
  if(mFlag == true) {
    mFlag = false;
  }

   numberOfFrames = Gamedata::getInstance().getXmlInt("sonic/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("sonic/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("sonic/minFrameDelay");

/*
  frames.clear();
  frames.resize(RenderContext::getInstance()->getFrames("sonic").size());
  //currentFrame = 0;

  frames = RenderContext::getInstance()->getFrames("sonic");
 

  ///frameDelay = 0;
  //timeSinceLastFrame = 0;
*/
}


void Player::idle() {

  stop();
  pFlag = false;
  mFlag = false;


  numberOfFrames = Gamedata::getInstance().getXmlInt("sonic_idle/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("sonic_idle/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("sonic_idle/minFrameDelay");

 // frames.clear();
  //frames.resize(RenderContext::getInstance()->getFrames("sonic_idle").size());
  //frames = RenderContext::getInstance()->getFrames("sonic_idle");
  
  //currentFrame = 0;
  /*
  
  */
  //frameDelay = 0;
  //timeSinceLastFrame = 0;

}

void Player::punch() {
  stop();
  mFlag = false;
  pFlag = true;
  //frames.clear();
  if(punchCount == 1) {
  /*frames.resize(RenderContext::getInstance()->getFrames("sonic_p1").size());  
  frames = RenderContext::getInstance()->getFrames("sonic_p1");
  */
  numberOfFrames = Gamedata::getInstance().getXmlInt("sonic_p1/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("sonic_p1/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("sonic_p1/minFrameDelay");
  currentFrame = 0;
  }

  if(punchCount == 2) {
  /*frames.resize(RenderContext::getInstance()->getFrames("sonic_p2").size());  
  frames = RenderContext::getInstance()->getFrames("sonic_p2");
  */
  numberOfFrames = Gamedata::getInstance().getXmlInt("sonic_p2/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("sonic_p2/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("sonic_p2/minFrameDelay");
  currentFrame = 0;
  }


  if(punchCount == 3) {
  /*frames.resize(RenderContext::getInstance()->getFrames("sonic_p3").size());  
  frames = RenderContext::getInstance()->getFrames("sonic_p3");
*/
  numberOfFrames = Gamedata::getInstance().getXmlInt("sonic_p3/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("sonic_p3/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("sonic_p3/minFrameDelay");
  currentFrame = 0;
  punchCount = 0;

  } 
  ++punchCount;

}

void Player::stop() {
  setVelocityX( slowDown*getVelocityX());
  setVelocityY(0);
}

void Player::right() {
  mFlag = true;
  pFlag = false;
  if(getX() < worldWidth-frameWidth) {
    setVelocityX(-initialVelocity[0]);
    if(flipType==SDL_FLIP_HORIZONTAL) {
      flipType = SDL_FLIP_NONE;
    }
  }
}

void Player::left() {
  mFlag = true;
  pFlag = false;
  if(getX()>0) {
    setVelocityX(initialVelocity[0]);
    flipType = SDL_FLIP_HORIZONTAL;
  }

}

void Player::up() {
  mFlag = true;
  pFlag = false;
  if(getY()>380) {
    setVelocityY(-initialVelocity[1]);
  }

}

void Player::down() {
  mFlag = true;
  pFlag = false;
  if(getY() < worldHeight-frameHeight) {
    setVelocityY(initialVelocity[1]);
  }

}


void Player::draw() const {  
  //moving
  if(pFlag == false && mFlag == true) {
    if(flipType == SDL_FLIP_NONE) {
      frames[currentFrame]->draw(getX(), getY());
    }
    else {
      frames[currentFrame]->drawFlip(getX(), getY());
    }
  }
  //idle
  else if(pFlag == false && mFlag == false) {
    if(flipType == SDL_FLIP_NONE) {
      idlef[currentFrame]->draw(getX(), getY());
    }
    else {
      idlef[currentFrame]->drawFlip(getX(), getY());
    }
  }
  //punch1
  else if(pFlag == true && punchCount == 1) {
    if(flipType == SDL_FLIP_NONE) {
      punch1[currentFrame]->draw(getX(), getY());
    }
    else {
      punch1[currentFrame]->drawFlip(getX(), getY());
    }   
  }
  //punch2
  else if(pFlag == true && punchCount == 2) {
    if(flipType == SDL_FLIP_NONE) {
      punch2[currentFrame]->draw(getX(), getY());
    }
    else {
      punch2[currentFrame]->drawFlip(getX(), getY());
    }  
  }
  //punch3
  else if(pFlag == true && punchCount == 3) {
    if(flipType == SDL_FLIP_NONE) {
      punch3[currentFrame]->draw(getX(), getY());
    }
    else {
      punch3[currentFrame]->drawFlip(getX(), getY());
    } 
  }

}


void Player::update(Uint32 ticks) { 
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
  if(pFlag != true) {
    mFlag = false;
    idle();
  }

}


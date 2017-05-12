#include "player3.h"
#include "gamedata.h"
#include "renderContext.h"

Player3::Player3( const std::string& name) :
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
  flipType(SDL_FLIP_NONE),
  snap(false),
  font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font1/file").c_str(),Gamedata::getInstance().getXmlInt("font1/size"))),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet")),
  bullets(bulletName),
  minSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  pFlag(false),
  HP(1000),
  maxHP(1000)
{ }

Player3::Player3(const Player3& s) :
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
  flipType(s.flipType),
  font(s.font),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed),
  pFlag(s.pFlag),
  HP(s.HP),
  maxHP(s.maxHP)
  { }

Player3::~Player3() {
  TTF_CloseFont(font);
  TTF_Quit(); 
}

void Player3::advanceFrame(Uint32 ticks) {
  if( frameDelay > frameInterval ) {
    timeSinceLastFrame += ticks;
    if (timeSinceLastFrame > frameInterval) {
      currentFrame = (currentFrame+1) % frames.size();
      //std::cout<<"Player3 frame: " << currentFrame << std::endl;
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

bool Player3::getpFlag() {
  return pFlag;
}

void Player3::setpFlag(bool x) {
  pFlag = x;
}

unsigned Player3::getcFrame() {
  return currentFrame;
}

SDL_RendererFlip Player3::getFlip() {
  return flipType;

}

void Player3::snapToPlayer() {
  snap = true;
}


void Player3::reset(){
  //currentFrame = 0;
  //frames.resize(30);
  frames.clear();
  //frames.resize(RenderContext::getInstance()->getFrames("tails").size());
  //std::cout<<"frames reset size: "<<frames.size()<<std::endl;
  frames = RenderContext::getInstance()->getFrames("tails");
  
  numberOfFrames = Gamedata::getInstance().getXmlInt("tails/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("tails/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("tails/minFrameDelay");

}

void Player3::shoot() {
  stop();
  pFlag = true;
  currentFrame = 0;
  //frames.resize(30);
  
  frames.clear();
  //frames.resize(RenderContext::getInstance()->getFrames("tails_missle").size());
  //std::cout<<"frames shoot size: "<<frames.size()<<std::endl;
  
  frames = RenderContext::getInstance()->getFrames("tails_missle");
  
  numberOfFrames = Gamedata::getInstance().getXmlInt("tails_missle/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("tails_missle/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("tails_missle/minFrameDelay");

 
  float x = getX()+getFrame()->getWidth()/4;
  float y = getY()+getFrame()->getHeight()/4.5;
  // I'm not adding minSpeed to y velocity:
  bullets.shoot( Vector2f(x, y), 
    Vector2f(minSpeed+getVelocityX(), 0)
  );
}



void Player3::writeHP(const std::string& msg, SDL_Color c, int x, int y) const {
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


void Player3::drawHP() {

    int distx = getX();
    int disty = getY()-30;
    
    distx -= Viewport::getInstance().getX();
    disty -= Viewport::getInstance().getY();
    
    SDL_Color textColor = {0xff, 255, 0, 0};
    SDL_Color textColor2 = {0xff, 0, 0, 0};
    SDL_Renderer* renderer = RenderContext::getInstance()->getRenderer();

    std::stringstream strm;
    std::stringstream strm2;
    strm<< "HP: " << HP << "/" << maxHP;
    strm2<< "IMMORTAL";
    writeHP(strm.str(), textColor,distx, disty);
    writeHP(strm2.str(),textColor2,distx,disty+10);
    SDL_Rect r;
    r.x = distx-5;
    r.y = disty;
    r.w = 95;
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


bool Player3::collidedWith(const Drawable* obj) const {
  return bullets.collidedWith( obj );
}


void Player3::idle() {
  if(pFlag == true) {
    pFlag = false;
  }
  if(currentFrame > 8) {
    currentFrame = 0;
  }
  reset();
  
}


void Player3::stop() {
  setVelocityX( slowDown*getVelocityX());
  setVelocityY(0);
  if(currentFrame > 10) {
    currentFrame = 0;
  }
}

void Player3::right() {
  pFlag = false;
  if(getX() < worldWidth-frameWidth) {
    setVelocityX(-initialVelocity[0]);
    if(flipType==SDL_FLIP_HORIZONTAL) {
      flipType = SDL_FLIP_NONE;
    }
  }
}

void Player3::left() {
  pFlag = false;
  if(getX()>0) {
    setVelocityX(initialVelocity[0]);
    flipType = SDL_FLIP_HORIZONTAL;
  }

}

void Player3::up() {
  pFlag = false;
  if(getY()>0) {
    setVelocityY(-initialVelocity[1]);
  }

}

void Player3::down() {
  pFlag = false;
  if(getY() < worldHeight-frameHeight) {
    setVelocityY(initialVelocity[1]);
  }

}


void Player3::draw() const { 
  if(currentFrame < frames.size()){
    /*if(snap == true) { 
      int xSnap = Player::getPlayerX();
      int ySnap = Player::getPlayerY();


      if(flipType == SDL_FLIP_NONE) {
        frames[currentFrame]->draw(xSnap, ySnap);
        bullets.draw();
      }
      else {
        frames[currentFrame]->drawFlip(xSnap, ySnap);
        bullets.draw();
      }
    }
    else { */ 
      if(flipType == SDL_FLIP_NONE) {
        frames[currentFrame]->draw(getX(), getY());
        bullets.draw();
      }
      else {
        frames[currentFrame]->drawFlip(getX(), getY());
        bullets.draw();
      }
    //}

  }
}


void Player3::update(Uint32 ticks) { 
  if(currentFrame > frames.size()) {
    currentFrame = 0;
  }

  //drawHP();
  advanceFrame(ticks);
  bullets.setFlip(flipType);
  if(snap == true) {
    this->setPosition(Player::getPlayerPos());
  }
  else {
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);
  }
  snap = false;
  
  

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > frameHeight) {
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
  bullets.update(ticks);
  stop(); 
  if(pFlag!=true) { 
    idle();
  }

}


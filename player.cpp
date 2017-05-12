#include "player.h"
#include "gamedata.h"
#include "renderContext.h"

CollisionStrategy* getStrategyP(const string& name) {
  std::string sName = Gamedata::getInstance().getXmlStr(name+"/strategy");
  if ( sName == "midpoint" ) return new MidPointCollisionStrategy;
  if ( sName == "rectangular" ) return new RectangularCollisionStrategy;
  if ( sName == "perpixel" ) return new PerPixelCollisionStrategy;
  throw std::string("No strategy in getStrategy");
} 

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  pFlag(false),
  currentFrame(0),
  font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font1/file").c_str(),Gamedata::getInstance().getXmlInt("font1/size"))),
  frames( RenderContext::getInstance()->getFrames(name) ),
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
  punchCount(1),
  HP(1000),
  maxHP(1000),
  flipType(SDL_FLIP_NONE),
  strategy(getStrategyP(name))
{ }

Player::Player(const Player& s) :
  Drawable(s), 
  pFlag(s.pFlag),
  currentFrame(s.currentFrame),
  font(s.font),
  frames(s.frames),  
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
  punchCount(s.punchCount),
  HP(s.HP),
  maxHP(s.maxHP),
  flipType(s.flipType),
  strategy(s.strategy)
  { }

/*Player::~Player() {
  TTF_CloseFont(font);
  TTF_Quit(); 
}*/

void Player::advanceFrame(Uint32 ticks) {
  if( frameDelay > frameInterval ) {
    timeSinceLastFrame += ticks;
    if (timeSinceLastFrame > frameInterval) {
      currentFrame = (currentFrame+1) % frames.size();
      //std::cout<<"Player frame: " << currentFrame << std::endl;
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

int Player::getHP() {
  return HP;


}

void Player::enemyHurt() {
 // if(timeSinceLastFrame > 1){
  HP -= 5;
  //}
}

void Player::bossHurt() {

  HP -= 300;  

}


void Player::reset(){
  if(pFlag == true) {
    pFlag = false;
  }

  frames.clear();
  frames.resize(RenderContext::getInstance()->getFrames("sonic").size());

  frames = RenderContext::getInstance()->getFrames("sonic");
  numberOfFrames = Gamedata::getInstance().getXmlInt("sonic/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("sonic/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("sonic/minFrameDelay");

}

const Vector2f& Player::getPlayerPos() const {
  return getPosition();

}

void Player::writeHP(const std::string& msg, SDL_Color c, int x, int y) const {
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


void Player::drawHP() {

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


bool Player::collidedWith(const Drawable* obj) const {
  if ( strategy->execute(*this, *obj) ) {
      return true;
  }
  else {
    return false;
  }
}



void Player::special() {
  pFlag = true;
  frames.resize(RenderContext::getInstance()->getFrames("sonic_s").size());
  frames = RenderContext::getInstance()->getFrames("sonic_s");
  
  
  numberOfFrames = Gamedata::getInstance().getXmlInt("sonic_s/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("sonic_s/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("sonic_s/minFrameDelay");

  currentFrame = 0;

}


void Player::idle() {

  stop();

  if(pFlag == true) {
    pFlag = false;
  }
  frames.resize(RenderContext::getInstance()->getFrames("sonic_idle").size());
  frames = RenderContext::getInstance()->getFrames("sonic_idle");
  
  //currentFrame = 0;
  
  numberOfFrames = Gamedata::getInstance().getXmlInt("sonic_idle/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("sonic_idle/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("sonic_idle/minFrameDelay");

  //frameDelay = 0;
  //timeSinceLastFrame = 0;

}

void Player::punch() {
  stop();
  pFlag = true;
  frames.clear();
  if(punchCount == 1) {
  frames.resize(RenderContext::getInstance()->getFrames("sonic_p1").size());  
  frames = RenderContext::getInstance()->getFrames("sonic_p1");

  numberOfFrames = Gamedata::getInstance().getXmlInt("sonic_p1/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("sonic_p1/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("sonic_p1/minFrameDelay");
  currentFrame = 0;

}

  if(punchCount == 2) {
  frames.resize(RenderContext::getInstance()->getFrames("sonic_p2").size());  
  frames = RenderContext::getInstance()->getFrames("sonic_p2");

  numberOfFrames = Gamedata::getInstance().getXmlInt("sonic_p2/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("sonic_p2/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("sonic_p2/minFrameDelay");
  currentFrame = 0;
  }


  if(punchCount == 3) {
  frames.resize(RenderContext::getInstance()->getFrames("sonic_p3").size());  
  frames = RenderContext::getInstance()->getFrames("sonic_p3");

  numberOfFrames = Gamedata::getInstance().getXmlInt("sonic_p3/frames");

  frameInterval = Gamedata::getInstance().getXmlInt("sonic_p3/frameInterval");

  minFrameDelay = Gamedata::getInstance().getXmlFloat("sonic_p3/minFrameDelay");

  punchCount = 0;
  currentFrame = 0;

  }
  punchCount++;

}



void Player::stop() {
  setVelocityX( slowDown*getVelocityX());
  setVelocityY(0);
}

void Player::right() {
  if(getX() < worldWidth-frameWidth) {
    setVelocityX(-initialVelocity[0]);
    if(flipType==SDL_FLIP_HORIZONTAL) {
      flipType = SDL_FLIP_NONE;
    }
  }
}

void Player::left() {
  if(getX()>0) {
    setVelocityX(initialVelocity[0]);
    flipType = SDL_FLIP_HORIZONTAL;
  }

}

void Player::up() {
  if(getY()>380) {
    setVelocityY(-initialVelocity[1]);
  }

}

void Player::down() {
  if(getY() < worldHeight-frameHeight) {
    setVelocityY(initialVelocity[1]);
  }

}


void Player::draw() const{
  if(currentFrame < frames.size()) {
    if(flipType == SDL_FLIP_NONE) {
      frames[currentFrame]->draw(getX(), getY());
    }
    else {
      frames[currentFrame]->drawFlip(getX(), getY());
    }
  }
}


void Player::update(Uint32 ticks) { 
  advanceFrame(ticks);
  if(currentFrame > frames.size()) {
    currentFrame = 0;
  }
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
    idle();
  }

}


#include <SDL_image.h>
#include <sstream>
#include "hud.h"
#include "gamedata.h"
#include "renderContext.h"

Hud& Hud::getInstance() {
  static Hud instance;
  return instance;
}

/*Hud::~Hud() { 
  TTF_CloseFont(font);
  TTF_Quit(); 
}*/

Hud::Hud() : 
  init(TTF_Init()),
  x(Gamedata::getInstance().getXmlInt("hud/x")),  
  y(Gamedata::getInstance().getXmlInt("hud/y")),
  w(Gamedata::getInstance().getXmlInt("hud/w")),
  h(Gamedata::getInstance().getXmlInt("hud/h")),
  viewport(Viewport::getInstance()),
  clock(Clock::getInstance()),
  renderer( RenderContext::getInstance()->getRenderer() ),
  font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file").c_str(),
                    Gamedata::getInstance().getXmlInt("font/size"))),
  textColor({0xff, 0, 0, 0})
{
  if ( init == -1 ) {
    throw std::string("error: Couldn't init font");
  }
  if (font == NULL) {
    throw std::string("error: font not found");
  }
  textColor.r = Gamedata::getInstance().getXmlInt("hud/r");
  textColor.g = Gamedata::getInstance().getXmlInt("hud/g");
  textColor.b = Gamedata::getInstance().getXmlInt("hud/b");
  textColor.a = Gamedata::getInstance().getXmlInt("hud/a");
}


void Hud::writeText(const std::string& msg, SDL_Color c, int x, int y) const {
  int textWidth;
  int textHeight;

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


void Hud::drawHud() {
    
    //averagefps,secs,x,y,how to play

    int distx = x+5;
    int disty = y+5;


    std::stringstream strm;
    strm<< "avg fps: " << clock.getAvgFps();
    writeText(strm.str(), textColor,distx, disty);
 
    std::stringstream strm2;
    strm2<< "seconds: " << clock.getSeconds();
    writeText(strm2.str(), textColor,distx, disty+25);
 

    std::stringstream strm3;
    strm3 << "X: " << viewport.getObjectToTrack()->getX();
    writeText(strm3.str(), textColor, distx, disty+50);
    

    std::stringstream strm4;
    strm4<< "Y: " << viewport.getObjectToTrack()->getY();
    writeText(strm4.str(), textColor,distx, disty+75);

    std::stringstream strm5;
    strm5<< "Toggle HUD: F1";
    writeText(strm5.str(), textColor,distx, disty+100);

    std::stringstream strm6;
    strm6<< "Left: a  |   Right: d";
    writeText(strm6.str(), textColor,distx, disty+130);


    std::stringstream strm7;
    strm7<< "Up:   w |   Down:s";
    writeText(strm7.str(), textColor,distx, disty+160);

    std::stringstream strm8;
    strm8<< "Tails missles: [Space]";
    writeText(strm8.str(), textColor,distx, disty+190);

    std::stringstream strm9;
    strm9<< "Sonic Punch: U";
    writeText(strm9.str(), textColor,distx, disty+220);
 
    std::stringstream strm10;
    strm10<< "Use power: I";
    writeText(strm10.str(), textColor,distx, disty+250);


    std::stringstream strm11;
    strm11<< "Toggle Music: M";
    writeText(strm11.str(), textColor,distx, disty+280);

    
     std::stringstream strm12;
    strm12<< "Toggle gawdmode: G";
    writeText(strm12.str(), {255,255,0,0},distx, disty+310);

    std::stringstream strm13;
    if(viewport.getObjectToTrack()->getName() == "sonic") {
      strm13<< "Switch to tails: J";
      writeText(strm13.str(), textColor,distx, disty+340);
    }
    else if(viewport.getObjectToTrack()->getName() == "tails") {
      strm13<< "Switch to sonic: J";
      writeText(strm13.str(), textColor,distx, disty+340);
    }

    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    // set the blend mode so that alpha blending will work;
    // the default blend mode is SDL_BLENDMODE_NONE
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/3 );

    // Render rect
    SDL_RenderFillRect( renderer, &r );

    SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
    SDL_RenderDrawRect( renderer, &r );

  
}




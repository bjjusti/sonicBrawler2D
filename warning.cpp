#include <SDL_image.h>
#include <sstream>
#include "warning.h"
#include "gamedata.h"
#include "renderContext.h"

Warning& Warning::getInstance() {
  static Warning instance;
  return instance;
}

/*Warning::~Warning() { 
  TTF_CloseFont(font);
  TTF_Quit(); 
}*/

Warning::Warning() : 
  init(TTF_Init()),
  x(Gamedata::getInstance().getXmlInt("warning/x")),  
  y(Gamedata::getInstance().getXmlInt("warning/y")),
  w(Gamedata::getInstance().getXmlInt("warning/w")),
  h(Gamedata::getInstance().getXmlInt("warning/h")),
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
  textColor.r = Gamedata::getInstance().getXmlInt("warning/r");
  textColor.g = Gamedata::getInstance().getXmlInt("warning/g");
  textColor.b = Gamedata::getInstance().getXmlInt("warning/b");
  textColor.a = Gamedata::getInstance().getXmlInt("warning/a");
}


void Warning::writeText(const std::string& msg, SDL_Color c, int x, int y) const {
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


void Warning::drawWarning() {
    
    //averagefps,secs,x,y,how to play

    int distx = x+5;
    int disty = y+5;


    std::stringstream strm;
    strm<< "You must collect 100 coins to unlock";
    writeText(strm.str(), textColor,distx, disty);
  
    std::stringstream strm2;
    strm2<< "power! Defeat enemies to get coins.";
    writeText(strm2.str(), textColor,distx,disty+20);

    std::stringstream strm3;
    strm3<< "Press 'I' to remove this box";
    writeText(strm3.str(), textColor,distx,disty+80);
     


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

    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
    SDL_RenderDrawRect( renderer, &r );

  
}




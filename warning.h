#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "viewport.h"
#include "clock.h"

class Warning {
public:
  static Warning& getInstance();
  //~Warning();
  void writeText(const std::string&, SDL_Color, int, int) const;
  void drawWarning();
  SDL_Renderer* getRenderer() const { return renderer; }
 
private:
  int init;
  int x;
  int y;
  int w;
  int h;
  Viewport& viewport;
  Clock& clock;
  SDL_Renderer* renderer;
  TTF_Font* font;
  SDL_Color textColor;
  Warning();
  Warning(const Warning&);
  Warning& operator=(const Warning&);
  
};

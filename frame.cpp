#include "drawable.h"
#include "frame.h"
#include "ioMod.h"
#include "viewport.h"
#include "renderContext.h"

Frame::Frame( SDL_Texture* tex ) : 
  renderer(RenderContext::getInstance()->getRenderer()),
  texture( tex ),
  rect{0,0,0,0}
{ SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h); }

Frame::Frame( const Frame& frame ) :
  renderer(frame.renderer),
  texture(frame.texture), 
  rect(frame.rect)
{ }


Frame& Frame::operator=(const Frame& rhs) {
  renderer = rhs.renderer;
  texture = rhs.texture;
  rect = rhs.rect;
  return *this;
}

void Frame::drawFlip(int x, int y, float scale) const {
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  int tempHeight = scale*rect.h;
  int tempWidth = scale*rect.w;
  SDL_Rect dest = {x, y, tempWidth, tempHeight};
  SDL_RenderCopyEx(renderer, texture, NULL, &dest, 0.0, NULL, SDL_FLIP_HORIZONTAL);
}

void Frame::drawFlip(int x, int y) const{
  drawFlip(x,y,1.0f);
}

void Frame::draw(int x, int y, float scale) const {
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  int tempHeight = scale*rect.h;
  int tempWidth = scale*rect.w;
  SDL_Rect dest = {x, y, tempWidth, tempHeight};
  SDL_RenderCopy(renderer, texture, &rect, &dest);

}

void Frame::draw(int x, int y) const {
  draw(x,y,1.0f);
}

void Frame::draw(int sx, int sy, int dx, int dy) const {
  SDL_Rect src = { sx, sy, rect.w, rect.h };    
  SDL_Rect dst = { dx, dy, rect.w, rect.h };
  SDL_RenderCopy(renderer, texture, &src, &dst);
}

Frame* Frame::crop(SDL_Rect sub)const{
  if(sub.x+sub.w > rect.w || sub.y+sub.h > rect.h){
    std::cerr << "Attempted to crop image with invalid geometry."
              << std::endl
              << "(0,0 + "<<rect.w << "x"<<rect.h<<")"
              << " --> "
              << "("<<sub.x<<","<<sub.y<<" + "<<sub.w << "x"<<sub.h<<")"
              << std::endl;
    return nullptr;
  }

  Frame* cloned = new Frame(*this);
  cloned->rect = sub;

  return cloned;
}


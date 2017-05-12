#include <vector>
#include <list>
#include "chunk.h"
#include "boss.h"


class ExplodingSpriteB : public Boss {
public:
  ExplodingSpriteB(const Boss& s);
  ~ExplodingSpriteB();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
private:
  std::list<Chunk*> chunks; // An ExplodingSpriteB is a list of sprite chunks
  std::list<Chunk*> freeList; // When a chunk gets out of range it goes here

  ExplodingSpriteB(const ExplodingSpriteB&); // Explicit disallow (Item 6)
  ExplodingSpriteB& operator=(const ExplodingSpriteB&); // (Item 6)
};

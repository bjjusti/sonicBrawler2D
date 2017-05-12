CXX = g++

# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl2-config --cflags` -g -W -Wall -std=c++11 -Weffc++ -Wextra -pedantic -O0 -I `sdl2-config --prefix`/include/

LDFLAGS = `sdl2-config --libs` -lm -lexpat -lSDL2_ttf -lSDL2_image -lSDL2_mixer

OBJS = \
  renderContext.o \
	ioMod.o \
	parseXML.o \
	gamedata.o \
	viewport.o \
	world.o \
	unpack.o \
	frame.o \
	frameFactory.o \
	frameGenerator.o \
	sprite.o \
	multisprite.o \
	vector2f.o \
	clock.o \
	engine.o \
	main.o \
        crows.o \
        player.o \
        player2.o \
        player3.o \
        sound.o \
        bullet.o \
        bulletPool.o \
        enemy.o \
        explodingSprite.o \
        explodingSpriteB.o \
        collisionStrategy.o \
        chunk.o \
        hud.o \
        coinsprite.o \
        warning.o \
        boss.o
EXEC = run

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
	rm -rf frames/*.bmp

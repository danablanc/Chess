CC = gcc
OBJS = ChessGame.o CHArrayList.o CHGameParser.o CHSettingsParser.o ConsoleMode.o XMLParser.o CHMiniMaxNode.o CHMiniMax.o Main.o \
Button.o Cell.o GameWin.o GUIManager.o LoadWin.o MainWin.o Panel.o SettingWin.o GuiMode.o
EXEC = chessprog
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main -lm
#SDL_COMP_FLAG = -I/usr/include/SDL2 -D_REENTRANT
#SDL_LIB = -L/usr/lib/x86_64-linux-gnu -Wl,-rpath,/usr/lib/x86_64-linux-gnu -Wl,--enable-new-dtags -lSDL2 -lSDL2main -lm

VPATH = graphics

all: $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o chessprog
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
Main.o: Main.c  GuiMode.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessGame.o: ChessGame.c CHArrayList.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHArrayList.o: CHArrayList.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHGameParser.o: CHGameParser.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHSettingsParser.o: CHSettingsParser.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
XMLParser.o: XMLParser.c ChessGame.h CHSettingsParser.h CHGameParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHMiniMax.o: CHMiniMax.c CHMiniMaxNode.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHMiniMaxNode.o: CHMiniMaxNode.c ChessGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ConsoleMode.o: ConsoleMode.c CHMiniMax.h XMLParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Button.o: graphics/Button.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
GuiMode.o: GuiMode.c GUIManager.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUIManager.o: GUIManager.c GameWin.h SettingWin.h LoadWin.h MainWin.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
GameWin.o: GameWin.c Panel.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
SettingWin.o: SettingWin.c ConsoleMode.h Button.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
LoadWin.o: LoadWin.c ConsoleMode.h Button.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
MainWin.o: MainWin.c Button.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
Panel.o: Panel.c Cell.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c
Cell.o: Cell.c Button.h ConsoleMode.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c graphics/$*.c

clean:
	rm -f $(OBJS) $(EXEC)

#include"NMGame.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Framework* game=new NMGame();

	game->Run();

	delete game;
	return 0;
}


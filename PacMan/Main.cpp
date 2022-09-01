#include "MainGame.h"

int main(int argc, char* args[])
{
	std::unique_ptr<MainGame> mainGame(new MainGame{ "Maing Game",800,640 });
	mainGame->run();
	return 0;
}
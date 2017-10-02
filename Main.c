
#include "GuiMode.h"

int main(int argc, char* argv[]){
	assert((argc==2)||(argc==1));
	if (argv[1] == NULL){
		startGame();
	}
	else
	if (strcmp(argv[1], "") == 0){
		startGame();
	}
	else if (strcmp(argv[1], "-c") == 0){
		startGame();
	}

	else if (strcmp(argv[1], "-g") == 0){
		startGuiMode();
	}

	return 0;

}

#include "GameWin.h"


//Inner functions
int isClickOnGame(int x, int y,SPGameWin* src) {
	assert(src!=NULL);
	int start_x=GAME_ARGS_INIT,end_x=GAME_ARGS_INIT,start_y=GAME_ARGS_INIT,end_y=GAME_ARGS_INIT, cell=GAME_ARGS_INIT;
	bool shown= false;
	cell = pixelToIndex(x,y);
	if(inRange(x,BOARD_START_X,BOARD_END_X) && inRange(y,BOARD_START_Y,BOARD_END_Y)) //checks where there was a click on button
		return cell; //returns on whick button there was a click
	for(int i=0;i<src->setPanel->buttonsCounter;i++){ //copies parameters needed
		start_x = src->setPanel->buttonsList[i]->location->x;
		end_x = src->setPanel->buttonsList[i]->location->x + src->setPanel->buttonsList[i]->location->w;
		start_y = src->setPanel->buttonsList[i]->location->y;
		end_y = src->setPanel->buttonsList[i]->location->y + src->setPanel->buttonsList[i]->location->h;
		shown = src->setPanel->buttonsList[i]->toShow;
		if(inRange(x,start_x,end_x) && inRange(y,start_y,end_y) && shown) //checks wheer there was a click on button
			return i+(GRID*GRID); //returns on which button there was a click
	}
	return ERROR_FLAG;
}

int pixelToIndex(int x, int y){
	int cell_x,cell_y;
	cell_x = xValue(x,BOARD_START_X,BOARD_CELL_DIM); //find the cell's x value using MACRO
	cell_y = yValue(BOARD_END_Y,y,BOARD_CELL_DIM); //find the cell's y value using MACRO
	return (cell_y*GRID) + cell_x; //combine and return index
}

SPGameWin* spGameWindowCreate(CH_Game* game,const char** board_images){
	assert(game!=NULL);
	int correct;
	SPGameWin* res = (SPGameWin*) calloc(sizeof(SPGameWin), sizeof(char));
	if (res == NULL ) {
		failMessage("Couldn't create game window!");
		return NULL ;
	} // Create an application window with the following settings:
	res->window = SDL_CreateWindow("Chess Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HIGHT,SDL_WINDOW_OPENGL);
	// Check that the window was successfully created
	if (res->window == NULL ) { // In the case that the window could not be made...
		failMessage("Couldn't create game window!");
		spGameWindowDestroy(res);
		return NULL ;
	}
	res->renderer = SDL_CreateRenderer(res->window, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->renderer == NULL ) { //checks allocation
		failMessage("Couldn't create game window!");
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_Surface* boardSurface = SDL_LoadBMP("./images/Boards/600x600/brown.bmp"); //creates board surface
	if (boardSurface == NULL ) {//checks allocation
		failMessage("Couldn't create game window!");
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_Rect boardloc = { .x = BOARD_PANEL_START_X, .y = BOARD_PANEL_START_Y, .h = BOARD_HEIGHT, .w = BOARD_WIDTH };
	res->boardPanel = createPanel(res->renderer,boardSurface,&boardloc,false); //creates board panel
	SDL_FreeSurface(boardSurface); //We finished with the surface -> delete it
	SDL_Surface* setSurface = SDL_LoadBMP("./images/new/brownSquare.bmp");//creates settings surface
	if (setSurface == NULL ) {//checks allocation
		failMessage("Couldn't create game window!");
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_Rect setloc = { .x = SET_WINDOW_START_X, .y = SET_WINDOW_START_Y, .h = SET_HEIGHT, .w = SET_WIDTH };
	res->setPanel = createPanel(res->renderer,setSurface,&setloc,true); //create settings panel
	if(res->boardPanel==NULL || res->setPanel==NULL){//checks allocation
		failMessage("Couldn't create game window panel!");
		spGameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(setSurface); //We finished with the surface -> delete it
	res->game = game;
	initDragArgs(res); //initalize board args
	correct = boardUpdate(res->boardPanel,res->game,board_images);  //update board
	if(!correct)
		return NULL;
	res->is_up_to_date = true; //the game is up to date!
	res->check_printed = false;
	return res;
}

void spGameWindowDestroy(SPGameWin* src) {
	if (!src) { //if no need to destroy
		return;
	} // destroy fields of struct
	if (src->boardPanel != NULL ) {
		destroyPanel(src->boardPanel);
	}
	src->boardPanel = NULL;
	if (src->setPanel != NULL ) {
		destroyPanel(src->setPanel);
	}
	src->setPanel = NULL;
	if (src->renderer != NULL ) {
		SDL_DestroyRenderer(src->renderer);
	}
	src->renderer = NULL;
	if (src->window != NULL ) {
		SDL_DestroyWindow(src->window);
	}
	src->window = NULL;
	free(src);
}

void spGameWindowDraw(SPGameWin* src) {
	if(src == NULL){ // no need to draw
		return;
	}
	SDL_SetRenderDrawColor(src->renderer,BOARD_R, BOARD_G, BOARD_B, 0);
	SDL_RenderClear(src->renderer);
	drawPanel(src->setPanel); //draw settings panel
	drawPanel(src->boardPanel); //draw board panel
	if(src->target_x!=GAME_ARGS_INIT && src->to_drag)
		drawCell(src->moving_cell); //draw the moving cell
	SDL_RenderPresent(src->renderer);
}

SP_GAME_EVENT spGameWindowHandleEvent(SPGameWin* src, SDL_Event* event,const char** board_images) {
	if (event == NULL || src == NULL )
		return SP_GAME_EVENT_INVALID_ARGUMENT;
	SP_GAME_EVENT eve;
	int result;
	int res = isClickOnGame(event->button.x, event->button.y,src); //get event
	switch (event->type) {
	case SDL_MOUSEBUTTONUP: //click on something (mouse up)
		eve = buttonUp(src,event,res,board_images);
		if(eve != NO_EVENT)
			return eve;
		break;
	case SDL_WINDOWEVENT: //window close
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			return SP_GAME_EVENT_QUIT;
		break;
	case SDL_MOUSEBUTTONDOWN: // button down event
		result = buttonDown(src,event,res,board_images);
		if(result != NO_EVENT)
			return SP_GAME_EVENT_QUIT;
		break;
	case SDL_MOUSEMOTION: // motion event
		drag(src);
		break;
	default:
		return SP_GAME_EVENT_NONE;
	}
	return SP_GAME_EVENT_NONE;
}

int buttonDown(SPGameWin* src, SDL_Event* event, int res, const char** board_images){
	assert(src != NULL);
	int row, col;
	bool check;
	row = (res/GRID); // row of event
	col = (res%GRID); // col of event
	if ((event->button.button == SDL_BUTTON_RIGHT) && inRange(res,0,((GRID*GRID)-1)) //get moves
			&& (src->game->difficulty <= DEFAULT_DIFFICULTY) && (src->game->mode == DEFAULT_MODE)){ // get move pre-requesits
		check = getMovesGui(src,row+1,col+1,board_images); // get moves
		if(!check){ // if an error occurred
			failMessage("Couldn't allocate memory!");
			return QUIT;
		}
	}
	else if ((event->button.button == SDL_BUTTON_LEFT) && inRange(res,0,((GRID*GRID)-1))){
		if(playerFig(src->game,row,col)){ // if player's figure so start to drag
			src->to_drag = true;
			src->moving_cell = src->boardPanel->cellsList[res];
		}
		src->source_x = src->boardPanel->cellsList[res]->location->x;
		src->source_y = src->boardPanel->cellsList[res]->location->y;
	}
	return NO_EVENT;
}

SP_GAME_EVENT buttonUp(SPGameWin* src, SDL_Event* event, int res, const char** board_images){
	assert(src != NULL);
	int correct,cell_src;
	if (res == EXIT_GAME_BUTTON) //click on exit
		return exitMessege(src, true);
	if (res == MAIN_MENU_GAME_BUTTON)//click on main menu
		return exitMessege(src, false);
	if (res == RESTART_GAME_BUTTON)//click on restart
		return SP_GAME_EVENT_RESTART;
	if (res == LOAD_GAME_BUTTON)//click on load
		return SP_GAME_EVENT_LOAD;
	if (res == SAVE_GAME_BUTTON)//click on save
		return SP_GAME_EVENT_SAVE;
	if (res == UNDO_GAME_BUTTON)//click on undo
		return SP_GAME_EVENT_UNDO;
	if (inRange(res,0,((GRID*GRID)-1)) && (event->button.button == SDL_BUTTON_RIGHT)){ //right click
		correct = boardUpdate(src->boardPanel,src->game,board_images);
		if(!correct)
			return SP_GAME_EVENT_QUIT;
		return SP_GAME_EVENT_NONE;
	}
	if (inRange(res,0,((GRID*GRID)-1)) && (event->button.button == SDL_BUTTON_LEFT)){ //left click
		cell_src = pixelToIndex(src->source_x+CELL_OFFSET+1,src->source_y+CELL_OFFSET);
		if(playerFig(src->game,cell_src/GRID,cell_src%GRID)){
			correct = moving(src,cell_src,res,board_images); // make move
			if(correct == QUIT) return SP_GAME_EVENT_QUIT; // check if move succeeded
			else if(correct == RESTART) return SP_GAME_EVENT_RESTART;
			return SP_GAME_EVENT_NONE;
		}
	}
	return NO_EVENT;
}

void drag(SPGameWin* src){
	int press,mouse_x,mouse_y,delta_x,delta_y,distance;
	press = SDL_GetMouseState(&mouse_x,&mouse_y);//gets mouse position in window
	if(press && src->to_drag && inRange(mouse_x,BOARD_MIN_X,BOARD_MAX_X) && inRange(mouse_y,BOARD_MIN_Y,BOARD_MAX_Y)){ // to drag
		src->target_x = (mouse_x - CELL_OFFSET+1);
		src->target_y = (mouse_y - CELL_OFFSET);
		delta_x = src->target_x - src->moving_cell->location->x;
		delta_y = src->target_y - src->moving_cell->location->y;
		distance = dist(delta_x,delta_y); // computing distane using MACRO
		if(distance>DRAG_TIME){  //updates the diatance every 5
			src->moving_cell->location->x = src->target_x;
			src->moving_cell->location->y = src->target_y;
		}
	}
	else if (press && src->to_drag){
		src->target_x = borderX(mouse_x);
		src->target_y = borderY(mouse_y);
	}
}

int moving(SPGameWin* src,int cell_src,int res ,const char** board_images){
	assert(src != NULL); //assertion
	int move[CELL_SIZE*CELL_SIZE],result,mouse_x,mouse_y,cell_trg; //initializes params
	bool check1,check2;
	SDL_GetMouseState(&mouse_x,&mouse_y); //get mouse position
	cell_trg = pixelToIndex(mouse_x,mouse_y); // transfer pixels to cell on board
	move[START_ROW_PLACE] = (cell_src/GRID)+1; move[START_COL_PLACE] = (cell_src%GRID)+1;
	move[DST_ROW_PLACE] = (cell_trg/GRID)+1; move[DST_COL_PLACE] = (cell_trg%GRID)+1;
	elem* element = transfer(src->game,move);
	if(element == NULL){
		failMessage("Couldn't allocate memory!");
		return QUIT; //quit
	}
	result = isValidMove(src->game,element); //checks if the move is valid
	free(element);
	if(result == ALLOCATION_ERROR){
		failMessage("Couldn't allocate memory!");
		return QUIT; //quit
	}
	else if(result == 1){ //valid move
		result = setMove(src->game,move);//do the move!
		if (result != 0){
			failMessage("Couldn't allocate memory!");
			return QUIT; //quit
		}
		src->check_printed = false; src->is_up_to_date = false; //the saved game is not up to date
		src->moving_cell->location->x = src->source_x; //drag and drop application
		src->moving_cell->location->y = src->source_y;
		check1 = textureUpdate(src->boardPanel->cellsList[res],src->moving_cell->img);
		check2 = textureUpdate(src->moving_cell,NULL);
		if((!check1)||(!check2))
			return QUIT; //quit
		result = boardUpdate(src->boardPanel,src->game,board_images);
		if(!result) return QUIT; //quit
		undoUpdate(src->setPanel,src->game);
		initDragArgs(src);
		if(src->game->game_status != REGULAR){ //check for change in status
			result = statusMessege(src);
			if(result == QUIT_BUTTONID || result == ERROR_FLAG) return QUIT; //quit
			else if(result == RESTART_BUTTONID) return RESTART; //restart
		}
		if(!PCMove(src,board_images))
			return QUIT; //quit
	}
	else{
		src->moving_cell->location->x = src->source_x; //drag and drop init
		src->moving_cell->location->y = src->source_y;
		initDragArgs(src);
	}
	return NONE; //None
}


int boardUpdate(Panel* panel,CH_Game* game,const char** board_images){
	assert(panel != NULL);
	assert(game != NULL);
	const char* img= NULL;
	SDL_Texture* cellTexture = NULL;
	SDL_Surface* boardSurface = SDL_LoadBMP("./images/Boards/600x600/brown.bmp"); //creates board surface
	if (boardSurface == NULL ) {
		failMessage("Couldn't create game window!");
		return 0;
	}
	SDL_Texture* panelTexture = SDL_CreateTextureFromSurface(panel->renderer,
			boardSurface); //create texture for enable button
	if (panelTexture == NULL) {
		failMessage("Couldn't create game window!");
		return 0;
	}
	SDL_FreeSurface(boardSurface); //We finished with the surface -> delete it
	for(int i=0; i<GRID*GRID;i++){
			SDL_DestroyTexture(panel->cellsList[i]->cellTexture);
			img = cellToImage(game->gameBoard[i/GRID][i%GRID],board_images); //get image of cell
			if(img != NULL){ //updat the image if neccessary
				SDL_Surface* cellSurface = SDL_LoadBMP(img);
				if (cellSurface == NULL){//checks allocation
					failMessage("Couldn't create surface!");
					return 0;
				}
				cellTexture = SDL_CreateTextureFromSurface(panel->cellsList[i]->renderer,cellSurface);//create texture for enable button
				if (cellTexture == NULL){//checks allocation
					SDL_FreeSurface(cellSurface);
					failMessage("Couldn't create texture!");
					return 0;
				}
				SDL_FreeSurface(cellSurface);
			}
			panel->cellsList[i]->cellTexture = cellTexture;
			drawCell(panel->cellsList[i]); //draw the cell!
			cellTexture = NULL;
	}
	return 1;
}

const char* cellToImage(char ch,const char** board_images){
	assert(board_images != NULL);
	switch(ch){ //switch according to the char that represents the cell
	case PAWN_W: // return the path to the specific image
		return board_images[WHITE_PAWN_IMG];
	case BISHOP_W:
		return board_images[WHITE_BISHOP_IMG];
	case ROOK_W:
		return board_images[WHITE_ROOK_IMG];
	case KNIGHT_W:
		return board_images[WHITE_KNIGHT_IMG];
	case QUEEN_W:
		return board_images[WHITE_QUEEN_IMG];
	case KING_W:
		return board_images[WHITE_KING_IMG];
	case PAWN_B:
		return board_images[BLACK_PAWN_IMG];
	case BISHOP_B:
		return board_images[BLACK_BISHOP_IMG];
	case ROOK_B:
		return board_images[BLACK_ROOK_IMG];
	case KNIGHT_B:
		return board_images[BLACK_KNIGHT_IMG];
	case QUEEN_B:
		return board_images[BLACK_QUEEN_IMG];
	case KING_B:
		return board_images[BLACK_KING_IMG];
	case RED_CELL:
		return board_images[RED_CELL_IMG];
	case YELLOW_CELL:
		return board_images[YELLOW_CELL_IMG];
	case GREEN_CELL:
		return board_images[GREEN_CELL_IMG];
	default:
		return NULL;
	}
}

bool getMovesGui(SPGameWin* src,int row ,int col ,const char** board_images){
	char** board = (char**)malloc(sizeof(char*)*GRID);
	int correct = 0;
	if(board == NULL){ // allocation error
		printf("ERROR: Couldn't allocate space\n");
		return false;
	}
	for(int i=0;i<GRID;i++){ //allocates board
		board[i] = (char*)malloc(sizeof(char)*GRID);
		if(board[i] == NULL){//checks allocation
			for(int j =0;j<i;j++)
				free(board[j]);
			free(board);
			printf("ERROR: Couldn't allocate space\n");
			return false;
		}
	}
	for(int i=0;i<GRID;i++){ //init
		for(int j=0;j<GRID;j++)
			board[i][j] = DEFAULT_CELL; //default char
	}
	getMovesBonus(src->game,row,col,board,true); //invokes the get moves feature
	for (int i=0;i<GRID;i++){
		for (int j=0;j<GRID;j++){
			char c = src->game->gameBoard[i][j];
			if(board[i][j] !=DEFAULT_CELL) //there is a move in get moves board
				src->game->gameBoard[i][j] = board[i][j]; //update
			board[i][j] = c;
		}
	}
	correct = boardUpdate(src->boardPanel,src->game,board_images); //updates the board
	if(!correct)
		return false;
	for (int i=0;i<GRID;i++){
		for (int j=0;j<GRID;j++)
			src->game->gameBoard[i][j] = board[i][j];
	}
	for(int i=0;i<GRID;i++) // frees memory
		free(board[i]);
	free(board);
	return true;
}

void initDragArgs(SPGameWin* src){
	assert(src!=NULL); //assertion
	src->moving_cell = NULL; // initialize args
	src->to_drag = false;
	src->target_x = GAME_ARGS_INIT;
	src->target_y = GAME_ARGS_INIT;
	src->source_x = GAME_ARGS_INIT;
	src->source_y = GAME_ARGS_INIT;
}

int PCMove(SPGameWin* src,const char** board_images){
	int *move,correct;
	if(src->game->mode == 1 && src->game->currentPlayer == playerPC(src->game)){ //pc move
		move = miniMaxPcMove(src->game,src->game->difficulty); //get the move
		if(!move){//checks allocation
			failMessage("Couldn't allocate memory!");
			return 0;
		}
		for(int i=0;i<CELL_SIZE*CELL_SIZE;i++)
			move[i]++;
		if(setMove(src->game,move)!=0){ //makes the move
			failMessage("Couldn't allocate memory!");
			return 0;
		}
		free(move);
		correct = boardUpdate(src->boardPanel,src->game,board_images); //update the board according to the computer's move
		if(!correct)
			return 0;
		undoUpdate(src->setPanel,src->game); //updates undo button
		src->is_up_to_date = false;
		src->check_printed = false; //updates flag fields
	}
	return 1;
}

int statusMessege(SPGameWin* src){
	assert(src!=NULL);
	char *string;
	if(src->game->game_status == CHECK){ // if there is a check
		if(!(src->check_printed)){ //check wasnt printed this round
			src->check_printed = true; // check is printed
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
					"Game Status", "There is a CHECK!", NULL);
		}
		return CHECK_FLAG;
	}
    const SDL_MessageBoxButtonData buttons[] = { {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, RESTART_BUTTONID, "Restart" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, QUIT_BUTTONID, "Quit" }};
    const SDL_MessageBoxColorScheme colorScheme = {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { WHITE,   WHITE,   WHITE },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            {   WHITE, BLACK,   BLACK },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            { BLACK, BLACK,   BLACK },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            {   WHITE,   WHITE, WHITE },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            { WHITE,   BLACK, WHITE }
        }
    };
    if(src->game->game_status == CHECKMATE){ //checkmate the winner is
    	switchPlayer(src->game);
    	if(strcmp(color(src->game->currentPlayer),"white")==0) //checks for winner
    		string = "white player won!";
    	else
    		string= "black player won!";
    	switchPlayer(src->game);
    }
    else if(src->game->game_status == TIE){//tie
    	string = "it is a tie!";
    }
    const SDL_MessageBoxData messageboxdata = {SDL_MESSAGEBOX_INFORMATION,NULL,"Game Status",
    		string ,SDL_arraysize(buttons),buttons,&colorScheme};
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        failMessage("Message box ERROR!");
        return ERROR_FLAG;
    }
    return buttonid;
}


SP_GAME_EVENT exitMessege(SPGameWin* src, bool flag){
	assert(src!=NULL);
	if(src->is_up_to_date){ //the game is up to date
		if (flag==true) // flag represents where to go: main menu / quit game
			return SP_GAME_EVENT_QUIT;
		else
			return SP_GAME_EVENT_MAIN_MENU;
	}
    const SDL_MessageBoxButtonData buttons[] = { {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, YES_BUTTONID, "Yes" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, NO_BUTTONID, "No" },{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
        		CANCEL_BUTTONID, "Cancel" }};
    const SDL_MessageBoxColorScheme colorScheme = {
            { /* .colors (.r, .g, .b) */
                /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                { WHITE,   WHITE,   WHITE },
                /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                {   WHITE, BLACK,   BLACK },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                { BLACK, BLACK,   BLACK },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                {   WHITE,   WHITE, WHITE },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                { WHITE,   BLACK, WHITE }
            }
    };
    const SDL_MessageBoxData messageboxdata = {SDL_MESSAGEBOX_INFORMATION,NULL,
    		"Warning","Do you want to save game before exiting ?",SDL_arraysize(buttons),buttons,&colorScheme};
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        failMessage("Message box ERROR!");
        return -1;
    }
    if (buttonid == YES_BUTTONID){ // user wants to save game
    	if (flag==true)
    		return SP_GAME_EVENT_SAVE_EXIT;
    	else return SP_GAME_EVENT_SAVE_MAIN_MENU;
    }
    else if (buttonid == NO_BUTTONID){ // user doesnt want to save game
    	if (flag==true)
    		return SP_GAME_EVENT_QUIT;
    	else return SP_GAME_EVENT_MAIN_MENU;
    }
    else
    	return SP_GAME_EVENT_NONE;
}

void undoUpdate(Panel* panel, CH_Game* game){
	assert(panel != NULL); // assertions
	assert(game != NULL);
	if(game->undo_hist->actualSize <HISTORY_MIN_LIMIT || game->mode == TWO_PLAYER_MODE) // turn undo to be disable
		panel->buttonsList[UNDO_GAME_BUTTON-(GRID*GRID)]->isEnabled = false;
	else // turn undo to be enable
		panel->buttonsList[UNDO_GAME_BUTTON-(GRID*GRID)]->isEnabled = true;
	drawButton(panel->buttonsList[UNDO_GAME_BUTTON-(GRID*GRID)]);
}

void spGameWindowHide(SPGameWin* src) {
	SDL_HideWindow(src->window);
}

void spGameWindowShow(SPGameWin* src) {
	SDL_ShowWindow(src->window);
}


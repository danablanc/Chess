
#include "CHGameParser.h"

bool parserIsTuple(const char* str,GameCommand *curr, int cell){
	int count=0,count2=0, length=0;
	if((str==NULL) ||((str[0]!='<') ||(str[strlen(str)-1]!='>')))
		return false;
	if (strstr(str,",")==NULL)
		return false;
	length=strlen(str);
	for (int i=0;i<length;i++){
		if (str[i]!=',')
			count++;
		else
			break;
	}
	if (count!=2)
		curr->move[cell]=INVALID_POSITION_FLAG;
	else
	{
		char c=str[1];
		curr->move[cell]=c-'0';
	}
	while (str[0]!=',')
		str++; //pointer on ','
	for (int i=0;i<length-count;i++){
		if (str[i]!='>')
				count2++;
			else
				break;
		}
		if (count2!=2)
			curr->move[cell+1]=INVALID_POSITION_FLAG;
		else
		{
			char c=str[1];
		    int num = c;
			num=num-ASCII_OFFSET;
			curr->move[cell+1]=num;
		}

	return true;
}



GameCommand parserGamePraseLine(char* str){
	int flag=0; //flag that indicates if the command supposed to have another segment and which one
	bool first_tup = false ,second_tup = false; //flag that represents if the tuple that represents move is valid
	GameCommand curr_command; //represents the command
	for(int i=0;i<SP_MAX_LINE_LENGTH;i++)
		curr_command.path[i]= '\0';
	curr_command.validArg = false; //initialize valide.arg to be false
	if(str == NULL){ //if line is null - invalid line
		curr_command.cmd = CH_GINVALID_LINE;
		return curr_command;
	}
	char *token, line[SP_MAX_LINE_LENGTH+1]; //represents the line
	strcpy(line,str); //copy the given string
	line[strlen(str)] = '\0';
	token = strtok(line," \t\r\n"); //gets the first token seperated by the given delimiter
	curr_command.cmd = parserIsGameCommand(token); //initializes the command
	token=strtok(NULL," \t\r\n"); //gets the second token
	if (curr_command.cmd == CH_SAVE) flag= PATH_COMMANDS_FLAG_GAME;
	if((curr_command.cmd == CH_MOVE)||(curr_command.cmd==CH_GET_MOVES)) flag= MOVES_COMMANDS_FLAG;
	if (!flag){// not move or save
		if (token!=NULL){//have more tokens - invalid command
			curr_command.cmd = CH_GINVALID_LINE;
			return curr_command;
		}
		return curr_command; //valid command -return
    }
    else if ((token == NULL)) // no more tokens
    	curr_command.cmd = CH_GINVALID_LINE;
    else{
    	if(flag == MOVES_COMMANDS_FLAG){ //move token - supposed to have 2 tuples that represent move
    		first_tup = parserIsTuple(token,&curr_command,0); //get the source cell
    		token=strtok(NULL," \t\r\n"); //third token - supposed to be the string "to"
    		if(curr_command.cmd == CH_GET_MOVES){
    			if((!first_tup)||(token!=NULL)) curr_command.cmd=CH_GINVALID_LINE;
    			else curr_command.validArg = true;
    			return curr_command;}
    		if ((!first_tup)||(token==NULL)){
    			curr_command.cmd=CH_GINVALID_LINE;
    			return curr_command;}
    		if(!strcmp(token,"to")){
        		token=strtok(NULL," \t\r\n"); //fourth token - supposed to be the string <x,y>
    			second_tup = parserIsTuple(token,&curr_command,2); //get the destination cell
        		token=strtok(NULL," \t\r\n"); //supposed to be null - no more tokens
                if(token == NULL && first_tup && second_tup) // no more tokens after the column number
                	curr_command.validArg = true;
                else curr_command.cmd=CH_GINVALID_LINE;}
    		else curr_command.cmd=CH_GINVALID_LINE;}
    	else if(flag == PATH_COMMANDS_FLAG_GAME){//load or save token - supposed to have a path
    		strcpy(curr_command.path , token);
    		token= strtok(NULL," \t\r\n");
    	    if(token == NULL) // no more tokens after the column number
    	    	curr_command.validArg = true;}
       else curr_command.cmd = CH_GINVALID_LINE;}
    return curr_command;
}


GAME_COMMAND_NAME parserIsGameCommand(const char* str){
	GAME_COMMAND_NAME cmd;
	if (!str){ //if string not valid
			cmd =CH_GINVALID_LINE;
			return cmd;
	}
	else if (!strcmp(str,"move")) //conditions that check if that a valid command in the game
		cmd = CH_MOVE;
	else if (!strcmp(str,"get_moves"))
		cmd = CH_GET_MOVES;
	else if (!strcmp(str,"save"))
		cmd = CH_SAVE;
	else if (!strcmp(str,"undo"))
		cmd = CH_UNDO;
	else if (!strcmp(str,"reset"))
		cmd = CH_RESET;
	else if (!strcmp(str,"quit"))
		cmd = CH_GQUIT;
	else
		cmd =CH_GINVALID_LINE; //if the command is not valid
	return cmd;
}


#include <stdio.h> 
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <imm.h>
#pragma comment (lib ,"imm32.lib") 


//DEFINE BASIC GLOBAL VARIABLE START
#define height 21
#define width 26
#define dfly_time 5  //NUM OF ENEMIES PER APPEAR 

int space[height][width] = {0};  //TWO-DIMENTIONAL ARRAY AS INTERFACE 

int player_x , player_y;

int CRCM , num;

int demon_x[dfly_time] , demon_y[dfly_time];

int bs_HP , bs_appear , bs_move , bs_skill;  //BS HP & APPEARANCE & MOVEMENT & SKILL

int bs_x[10] , bs_y[10];

int bs_atk_x , bs_atk_y;
//DEFINE BASIC GLOBAL VARIABLE END

void HideCHTypewriting()
{
}

void HideCursor()  //HIDE THE CURSOR
{
	CONSOLE_CURSOR_INFO cursor_info = {1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}



void gotoxy(int x, int y)  //CURSOR SKIP
{
    COORD pos = {x,y};
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut,pos);
}



void output()  //OUTPUT 
{ 
	int i , j;
	gotoxy(0,0);  //LET CURSOR SKIP TO 0,0

    //DEFINE SYMBLE START 
    for(i = 0 ; i < height ; i++)
    {
	    for(j = 0 ; j < width ; j++)
        if(space[i][j] == 0) printf(" ");  //EMPTY
        else if(space[i][j] == 1) printf("U");  //PLAYER
		else if(space[i][j] == 2) printf("*");  //EMENY
		else if(space[i][j] == 3) printf("o");  //BULLET
		else if(space[i][j] == 4) printf("||");  //WALL
		else if(space[i][j] == 5) printf("=");  //BOTTOM
		else if(space[i][j] == 6 && bs_appear == 1) printf("#");  //BS
		
        printf("\n");
    }
    //DEFINE SYMBLE END


}

void RealTimeProcessing()
{
    int i , j , b , k;

	for(i = 0 ; i < height ; i++)
		for(j = 0 ; j < width ; j++)
			if(space[i][j] == 3)
			{

			    //HIT AN ENEMY START
				for(k = 0 ; k < dfly_time ; k++)
				{
				    if(i == demon_x[k] && j == demon_y[k])
			    	{
			    		space[demon_x[k]][demon_y[k]] = 0;
				    	num++;
				    	demon_x[k] = 0;
				    	demon_y[k] = rand()%(width-1);
				    	space[demon_x[k]][demon_y[k]] = 2;
			    	}
				}
                //HIT AN ENEMY END

				for(b = 0 ; b < 10 ; b++) if(i == bs_x[i]+1 && j == bs_y[i]) bs_HP--;  //HIT BS
				
				if(i == player_x && j == player_y) CRCM = 3;  //PLAYER DEAD

				//BULLET RISING START
				space[i][j] = 0;
				if(i > 0) space[i-1][j] = 3;
				//BULLET RISING END
			}




	for(k = 0 ; k < dfly_time ; k++)
	{
	    if(demon_x[k] == player_x && demon_y[k] == player_y) CRCM = 3;  //CRASH PLAYER

        //OUT OF SCREEN START
    	if(demon_x[k] > height)
	    {
    		space[demon_x[k]][demon_y[k]] = 0;
    		demon_x[k] = 0;
    		demon_y[k] = rand() % (width-1);
	    	space[demon_x[k]][demon_y[k]] = 2;
	    }
	    //OUT OF SCREEN END

	}
	
    //ENEMY MOVEMENT
	static int n = 0;
	
	if(n < 10) n++;
	
	if(n == 10)
	{
		for(k = 0 ; k < dfly_time ; k++)
		{
		space[demon_x[k]][demon_y[k]] = 0;
		demon_x[k]++;
		space[demon_x[k]][demon_y[k]] = 2;
		n = 0;
		}
		
		//BS MOVE HORIZONTAL START
		if(bs_appear==1&&bs_skill==0)
		{
			for(i=0;i<10;i++)
			{
				space[bs_x[i]][bs_y[i]] = 0;
				bs_y[i]+=bs_move;
				space[bs_x[i]][bs_y[i]] = 6;
			}
				if(bs_y[2] == 0)
					bs_move = -bs_move;
				if(bs_y[4] == width-1)
				{
					bs_move = -bs_move;
				}
		}
		//BS MOVE HORIZONTAL END
		
		//BS MOVEMENT WHEN SKILLED START
		if(bs_appear == 1)
		{
			if(bs_HP < 50)
			{
				for(i = 0 ; i < 10 ; i++)
				{
        		space[bs_x[i]][bs_y[i]] = 0;
	        	bs_x[i]++;
				}

				bs_y[0] = player_y;
				bs_y[1] = player_y-1;
				bs_y[2] = player_y-2;
				bs_y[3] = player_y+1;
				bs_y[4] = player_y+2;
				bs_y[5] = player_y;
				bs_y[6] = player_y-1;
				bs_y[7] = player_y+1;
				bs_y[8] = player_y-1;
				bs_y[9] = player_y+1;

	        	for(i = 0 ; i < 10 ; i++) space[bs_x[i]][bs_y[i]] = 6;

				bs_skill = 1;
			}
		}
	}

	//BS CRASH PLAYER
	for(i = 0 ; i < 10 ; i++)
	{
	if(bs_x[i] == player_x&&bs_y[i] == player_y) CRCM = 3;
	}

	//OUTPUT FRAME BEGIN
	for(i = 0 ; i < height ; i++)
	    for(j = 0 ; j < width ; j++)
	    {
		    if(j == width-1)
			    space[i][j] = 4;
		    else if(i == height-1)
			    space[i][j] = 5;
	    }
    //OUTPUT FRAME END

	//BS APPEAR BEGIN
	if(num>5)
	{
	    bs_appear = 1; 
        for(i = 0 ; i < 10 ; i++)
    	{
    	    space[bs_x[i]][bs_y[i]] = 6;
    	}

	}
	//BS APPEAR END

	if(bs_HP == 0) CRCM = 2;  //VICTORY

	//BS ATTACK BEGIN
	if(bs_appear== 1 && bs_skill == 0)
	{
		        space[bs_atk_x][bs_atk_y] = 0;
			    bs_atk_x++;
				bs_atk_y = bs_y[0];
			    space[bs_atk_x][bs_atk_y] = 3;
				if (bs_atk_x == height) bs_atk_x = 2;
	}
    //BS ATTACK END

}



void startup()
{ 
    int i , j , k;

    player_x = height - 2;  //RelativePOSx	
    player_y = width / 2;  //RelativePOSy
    

    for(k = 0 ; k < dfly_time ; k++)
    {
	    demon_x[k] = rand() % 2;
	    demon_y[k] = rand() % (width - 1);
	    space[demon_x[k]][demon_x[k]] = 2;
    }
    space[player_x][player_y] = 1;

    CRCM = 0;

    num = 0;//COUNT OF BEATEN ANAMIES
  
}


void BS()
{

    //BS BASIC START
    bs_HP = 250;    //BS HP
    bs_appear = 0;  //WEATHER BS APPEAR
    bs_move = 1;    //BS MOVING
    bs_skill = 0;   //BS SKILL "Perish Together"--CRASH PLAYER
    //BS BASIC END


    //BS IMAGE START
    bs_x[0] = 1;bs_y[0] = 12;
    bs_x[1] = 1;bs_y[1] = 11;
    bs_x[2] = 1;bs_y[2] = 10;
    bs_x[3] = 1;bs_y[3] = 13;
    bs_x[4] = 1;bs_y[4] = 14;
    bs_x[5] = 0;bs_y[5] = 12;
    bs_x[6] = 0;bs_y[6] = 11;
    bs_x[7] = 0;bs_y[7] = 13;
    bs_x[8] = 2;bs_y[8] = 11;
    bs_x[9] = 2;bs_y[9] = 13;
    //BS IMAGE END 
  
  
    //BS WEAPON START
    bs_atk_x = bs_x[0]+1;
    bs_atk_y = bs_y[0];
    //BS WEAPON END


}


void UpdateWhenGetch()
{   
    char input;
    if( kbhit() )  //RESPONSE EVERY INPUT
	{

	input = getch();  //MONITOR KEYBOARD INPUT

    //PLAYER MOVE START
    if(input == 'w' && player_x != 0)
	{
		space[player_x][player_y] = 0;
		player_x--;
		space[player_x][player_y] = 1;
	}  //MOVE FORWARD 

	if(input == 's' && player_x != height-2)
	{
	    space[player_x][player_y] = 0;
		player_x++;
		space[player_x][player_y] = 1;
	}  //MOVE BACKWARD

	if(input == 'a' && player_y != 0)
	{
	    space[player_x][player_y] = 0;
		player_y--;
		space[player_x][player_y] = 1;
	}  //MOVE LAFT 

	if(input == 'd' && player_y != width-2) 
	{
	    space[player_x][player_y] = 0;
		player_y++;
		space[player_x][player_y] = 1;
	}  //MOVE RIGHT
	//PLAYER MOVE END

	if(input == 'z') CRCM = 1;//EXIT GAME

	if(input == 'j') space[player_x-1][player_y] = 3;  //FIRE ONE BULLET

	}

}

void main()
{   
    system("mode con cols=28 lines=27"); //SZ OF THE WINDOW
    system("title ");  //TITLE 
    system("color 08");  //COLOR
    
	system ("cls");
    
	int i , j;
	
	HideCursor();  //HIDE THE CURSOR
	
	//INITIALIZE GAME DATA START
	startup();
	BS();
	//INITIALIZE GAME DATA END 
	
    while(1)  //GAME START LOOPING
    {
    	//OUTPUT NOTICE START
    	printf("[ WASD ] 上下左右\n");
        printf("[ J ] 键射击\n\n");
        printf("击败敌机: [ %d ] 架\n",num);
        if(bs_appear == 1) printf("警告!BOSS出现!\nBOSS血量: [ %d ] " , bs_HP);
        //OUTPUT NOTICE END

	    output();  //START TO OUTPUT

        //REALTIME CALCULATE START
        RealTimeProcessing();  //RELEVANT INPUT
        UpdateWhenGetch();  //IRRELEVANT INPUT
        //REALTIME CALCULATE END

        if(CRCM) break;  //EXIT GAME
    }

	system("cls");  //CLS

    //PRINT BLANK BEGIN
	for(i = 0 ; i < 10 ; i++) printf("\n");  //VERTICAL BLANK
	for(j = 0 ; j < 12 ; j++) printf(" ");  //HORIZONTAL BLANK
    //PRINT BLANK END

	if(CRCM == 1)
	{
		printf("\n\t  您一共击毁\n\t    [ %d ]\n\t  架敌方飞机\n\n\n\t",num);
		printf(" 退出游戏成功\n\n");
		
	    Sleep (2000);
	}
	else if(CRCM == 2)
	{
    	printf("\n\t  您一共击毁\n\t    [ %d ]\n\t  架敌方飞机\n\n\n\t 恭喜您胜利了\n",num);
    	
        Sleep (2000);
	}
	  else
	{
		printf("\b你失败了\n");

		Sleep (2000);
	}
}


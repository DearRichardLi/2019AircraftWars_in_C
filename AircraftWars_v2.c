#include <stdio.h> 
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <imm.h>

//DEFINE BASIC GLOBAL VARIABLE START
#define height 21 
#define width 26 
#define dfly_time 5 // Total enemies 

int space[height][width] = {0}; // Display 

int player_x, player_y; // Player position 

int CRCM; 	// Game State 
int num; 		// Score 

int demon_x[dfly_time], demon_y[dfly_time]; // Enemy Positions 

int bs_HP, bs_appear, bs_move, bs_skill; // Boss related variables 
int bs_x[10], bs_y[10]; // Boss Positions 
int bs_atk_x, bs_atk_y; // Boss Gun Position 

void HideCHTypewriting(void){ 
}

void HideCursor(void){ 
	CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info); 
}

void gotoxy(int x, int y){ 
  COORD pos = {x, y}; 
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(hOut, pos); 
}

void output(void){ // Render On Screen 
	gotoxy(0,0);  
  for(int i = 0 ; i < height ; i++){ 
	   for(int j = 0 ; j < width ; j++){ // !!Changed to switch-case structure 
	   	switch(space[i][j]){ 
	   		case 0: printf(" "); break; // Empty 
	   		case 1: printf("U"); break; // Player 
	   		case 2: printf("*"); break; // Enemy 
	   		case 3: printf("o"); break; // Bullet 
	   		case 4: printf("||"); break; // Wall 
	   		case 5: printf("="); break; // Bottom 
	   		case 6: 
	   			if(bs_appear == 1) printf("#"); break; // Boss 
	   	}
    }
    printf("\n"); 
  }
}



void RealTimeProcessing(void){ 
//*************************************************************************** 
// Bullets hit & move 
	for(int i = 0 ; i < height ; i++){  
		for(int j = 0 ; j < width ; j++){  // preform traversal on all points  
			if(space[i][j] == 3){ // If it is Bullet then... 
				for(int k = 0 ; k < dfly_time ; k++){ // Hit enemy? 
				  if(i == demon_x[k] && j == demon_y[k]){ 
			    	space[demon_x[k]][demon_y[k]] = 0; 
				    num++; 
				    demon_x[k] = 0; 
				    demon_y[k] = rand() % (width - 1); // Generate new 
				    space[demon_x[k]][demon_y[k]] = 2; 
			    }
				}
				for(int k = 0 ; k < 10 ; k++) 
					if(i == bs_x[i] + 1 && j == bs_y[i]) // Hit boss? 
						bs_HP--; 
				if(i == player_x && j == player_y) // Hit player? 
					CRCM = 3; 
				space[i][j] = 0; 
				if(i > 0) space[i - 1][j] = 3; // Move bullet forward 
			}
		}
	}
//*************************************************************************** 
// Enemy collisions & generation 
	for(int k = 0 ; k < dfly_time ; k++){ 
   	if(demon_x[k] == player_x && demon_y[k] == player_y) // Enemy hit player 
			CRCM = 3; 
  	if(demon_x[k] >= height){ // Demon runs out of screen // Is here a bug? (using ">" instead of ">=" ) 
  		space[demon_x[k]][demon_y[k]] = 0; 
  		demon_x[k] = 0; 
  		demon_y[k] = rand() % (width-1); // Generate new 
	 		space[demon_x[k]][demon_y[k]] = 2; 
	 	}
	}
	
//*************************************************************************** 
// Enemy & Boss movements 
	static int n = 0; // Counter 
	if(n < 10) n++; 
	if(n == 10){ 
		n = 0; // !!Moved counterclear operation here 
		
		for(int k = 0; k < dfly_time; k++){ // All demon move forward 
			space[demon_x[k]][demon_y[k]] = 0; 
			demon_x[k]++; 
			space[demon_x[k]][demon_y[k]] = 2; 
		}
		if(bs_appear == 1){ // When boss apperaing... 
			if(bs_skill == 0){ // Not skilled 
				for(int i = 0; i < 10; i++){ 
					space[bs_x[i]][bs_y[i]] = 0; 
					bs_y[i] += bs_move; 
					space[bs_x[i]][bs_y[i]] = 6; // Perform boss horizontal move 
				}
					if(bs_y[2] == 0 || bs_y[4] == width - 1) // !!Combined if statements 
						bs_move = -bs_move; // Boss touching edge, change direction. 
			}
			else if(bs_HP < 50){ // Skilled and low HP 
				for(int i = 0 ; i < 10 ; i++){ // Move boss forward 
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
				bs_y[9] = player_y+1; // Set coords 
 	     	for(int i = 0 ; i < 10 ; i++) 
					space[bs_x[i]][bs_y[i]] = 6; // Display boss 
				bs_skill = 1;
			}
		}
	}

//*************************************************************************** 
// Boss hit 
	for(int i = 0 ; i < 10 ; i++) 
		if(bs_x[i] == player_x&&bs_y[i] == player_y) CRCM = 3; 

//*************************************************************************** 
// Render walls 
	for(int i = 0 ; i < height ; i++){ 
	    for(int j = 0 ; j < width ; j++){ 
		    if(j == width - 1) 
			    space[i][j] = 4; 
		    else if(i == height - 1) 
			    space[i][j] = 5; 
	    }
	  }

//*************************************************************************** 
// Render Boss 
	if(num>5){ 
	    bs_appear = 1; 
      for(int i = 0 ; i < 10 ; i++) 
    	   space[bs_x[i]][bs_y[i]] = 6;
	}

//*************************************************************************** 
// Victory 
	if(bs_HP == 0) CRCM = 2; 

//*************************************************************************** 
// Boss attack 
	if(bs_appear== 1 && bs_skill == 0){ // Appeared but not skilled 
		space[bs_atk_x][bs_atk_y] = 0; // Create Bullet Stream 
		bs_atk_x++; 
		bs_atk_y = bs_y[0]; 
		space[bs_atk_x][bs_atk_y] = 3; 
		if (bs_atk_x == height) bs_atk_x = 2; 
	}
}



void startup(void){ 
    player_x = height - 2; 
    player_y = width / 2; 
    space[player_x][player_y] = 1; // Initial Position 
    
    for(int i = 0; i < height; i++) 
    	for(int j = 0; j < width; j++) 
    		space[i][j] = 0; // Clear Display 
  	
    for(int k = 0 ; k < dfly_time ; k++){ // Initialize & Display Enemies 
	    demon_x[k] = rand() % 2; 
	    demon_y[k] = rand() % (width - 1); 
	    space[demon_x[k]][demon_x[k]] = 2; 
    }

    CRCM = 0; // Initialize Mode 
    
    num = 0; // Initialize Score 
}


void BS(void){ 
  bs_HP = 250;    // Boss HP 250 
  bs_appear = 0;  // Boss not appearing 
  bs_move = 1;    // Boss is moving 
  bs_skill = 0;   // Boss is not skilled 
  
  bs_x[0] = 1; bs_y[0] = 12;	
  bs_x[1] = 1; bs_y[1] = 11;	
  bs_x[2] = 1; bs_y[2] = 10;	
  bs_x[3] = 1; bs_y[3] = 13;	
  bs_x[4] = 1; bs_y[4] = 14;	
  bs_x[5] = 0; bs_y[5] = 12;	
  bs_x[6] = 0; bs_y[6] = 11;	
  bs_x[7] = 0; bs_y[7] = 13;	
  bs_x[8] = 2; bs_y[8] = 11;	
  bs_x[9] = 2; bs_y[9] = 13; // Initialize Boss Shape 
  
  bs_atk_x = bs_x[0] + 1; 
  bs_atk_y = bs_y[0]; // Initialize BossGun Position 
}



void UpdateWhenGetch(void){ 
  char input;
  if(kbhit()){ // On hitting keaboard... 
		input = getch();  
		
    if(input == 'w' && player_x > 0){ // Forward 
			space[player_x][player_y] = 0; 
			player_x--; 
			space[player_x][player_y] = 1; 
		}
		
		if(input == 's' && player_x < height - 2){ // Backward 
	    space[player_x][player_y] = 0; 
			player_x++; 
			space[player_x][player_y] = 1; 
		}
		
		if(input == 'a' && player_y > 0){ // Left 
	    space[player_x][player_y] = 0;
			player_y--;
			space[player_x][player_y] = 1;
		}
		
		if(input == 'd' && player_y < width - 2){ // Right 
	    space[player_x][player_y] = 0; 
			player_y++; 
			space[player_x][player_y] = 1; 
		}
		
		if(input == 'z') CRCM = 1; // Exit 
		
		if(input == 'j') space[player_x - 1][player_y] = 3; // Fire Bullet 
	}
}



int main(void){ 
  system("mode con cols=28 lines=27"); 
  system("title "); 
  system("color 08"); 
	system ("cls"); 
	HideCursor(); // Window Init 
    
	startup(); 
	BS(); // Game Init 
	
  while(1){ 
   	printf("[ WASD ] 上下左右\n");
    printf("[ J ] 键射击\n\n");
    printf("击败敌机: [ %d ] 架\n", num);
    if(bs_appear == 1) printf("警告!BOSS出现!\nBOSS血量: [ %d ] " , bs_HP);

	  output(); // Render 
    RealTimeProcessing(); // Engine Cycle 
    UpdateWhenGetch(); // Update Player 

    if(CRCM) break; // Exit 
  }
	system("cls"); 
	
	// Print Blank 
	for(int i = 0; i < 10; i++) printf("\n"); 
	for(int i= 0; i < 12; i++) printf(" "); 
	
	switch(CRCM){ // !!Changed to switch-case structure  
		case 1: { 
			printf("\n\t  您一共击毁\n\t    [ %d ]\n\t  架敌方飞机\n\n\n\t",num); 
			printf(" 退出游戏成功\n\n"); 
    	Sleep (2000); 
    	break; 
		}
		case 2: { 
    	printf("\n\t  您一共击毁\n\t    [ %d ]\n\t  架敌方飞机\n\n\n\t 恭喜您胜利了\n",num);
    	Sleep (2000); 
    	break; 
		}
	  default: { 
			printf("\b你失败了\n");
			Sleep (2000); 
			break; 
		}
	}
	system("Pause"); 
}


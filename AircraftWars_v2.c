#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <windows.h> 
#include <conio.h> 
#include <imm.h> 

#define height 21 
#define width 26 
#define nEnemies 8 // Total enemies 

// Game settings 
#define Boss_Speed 10 
#define Boss_InitialHP 100 
#define Boss_SkilledHP 20 
#define Boss_ScoreTrigger 16 

int Display[height][width]; // Display 

int player_x, player_y; // Player position 

int GameState; 	// Game State 
int Score; 			// Score 

int Enemy_x[nEnemies], Enemy_y[nEnemies]; // Enemy Positions 

int Boss_HP, Boss_appear, Boss_move, Boss_skill; // Boss related variables 
int Boss_x[10], Boss_y[10]; // Boss Positions 
int Boss_atk_x, Boss_atk_y; // Boss Gun Position 

void HideCursor(void){ 
	CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info); 
}

void setCursor(int x, int y){ 
	COORD pos = {x, y}; 
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleCursorPosition(hOut, pos); 
}

void Render(void){ // Render On Screen 
	setCursor(0,0);  
	for(int i = 0; i < height; i++){ 
		for(int j = 0; j < width; j++){ // !!Changed to switch-case structure 
			switch(Display[i][j]){ 
				case 0: putch(' '); break; // Empty 
				case 1: putch('U'); break; // Player 
				case 2: putch('*'); break; // Enemy 
				case 3: putch('o'); break; // Bullet - User 
				case 7: putch('x'); break; // Bullet - Boss 
				case 4: printf("||"); break; // Wall 
				case 5: putch('='); break; // Bottom 
				case 6: if(Boss_appear == 1) putch('#'); break; // Boss 
			}
		}
		putch('\n'); 
	}
}



void EngineCycle(void){ 
//*************************************************************************** 
// Bullets hit & move 
	for(int i = 0; i < height; i++){ 
		for(int j = 0; j < width; j++){ // preform traversal on all points  
			int bit = Display[i][j]; 
			if(bit == 3){ // If it is UserBullet then... 
				for(int k = 0; k < nEnemies; k++){ // Hit enemy? 
					if(i == Enemy_x[k] && j == Enemy_y[k]){ 
						Display[Enemy_x[k]][Enemy_y[k]] = 0; 
						Score++; 
						Enemy_x[k] = 0; 
						Enemy_y[k] = rand() % (width - 1); // Generate new 
						Display[Enemy_x[k]][Enemy_y[k]] = 2; 
					}
				}
				for(int k = 0; k < 10; k++) 
					if(i == Boss_x[k] + 1 && j == Boss_y[k]) // Hit boss? 
						Boss_HP--; 
			}
			if(bit == 3 || bit == 7){ 
				if(i == player_x && j == player_y) // Hit player? 
					GameState = 3; 
				Display[i][j] = 0; 
				if(i > 0) Display[i - 1][j] = bit; // Move bullet forward 
			}
		}
	}
	
//*************************************************************************** 
// Enemy collisions & generation 
	for(int k = 0; k < nEnemies; k++){ 
		if(Enemy_x[k] == player_x && Enemy_y[k] == player_y) // Enemy hit player 
			GameState = 3; 
		if(Enemy_x[k] >= height){ // Enemy runs out of screen // Is here a bug? (using ">" instead of ">=" ) 
			Display[Enemy_x[k]][Enemy_y[k]] = 0; 
			Enemy_x[k] = 0; 
			Enemy_y[k] = rand() % (width-1); // Generate new 
			Display[Enemy_x[k]][Enemy_y[k]] = 2; 
		}
	}
	
//*************************************************************************** 
// Enemy & Boss movements 
	static int timer = 0; // Counter 
	if(timer < Boss_Speed) timer++; 
	else{ 
		timer = 0; // !!Moved counterclear operation here 
		
		for(int k = 0; k < nEnemies; k++){ // All Enemy move forward 
			Display[Enemy_x[k]][Enemy_y[k]] = 0; 
			Enemy_x[k]++; 
			Display[Enemy_x[k]][Enemy_y[k]] = 2; 
		}
		if(Boss_appear == 1){ // When boss apperaing... 
			if(Boss_skill == 0){ // Not skilled 
				for(int i = 0; i < 10; i++){ 
					Display[Boss_x[i]][Boss_y[i]] = 0; 
					Boss_y[i] += Boss_move; 
					Display[Boss_x[i]][Boss_y[i]] = 6; // Perform boss horizontal move 
				}
					if(Boss_y[2] <= 0 || Boss_y[4] >= width - 1) // !!Combined if statements 
						Boss_move = -Boss_move; // Boss touching edge, change direction. 
			}
			if(Boss_HP <= Boss_SkilledHP){ // Low HP, then skilled 
				for(int i = 0; i < 10; i++){ // Move boss forward 
					Display[Boss_x[i]][Boss_y[i]] = 0; 
					Boss_x[i]++; 
				}
				Boss_y[0] = player_y; 
				Boss_y[1] = player_y-1; 
				Boss_y[2] = player_y-2; 
				Boss_y[3] = player_y+1; 
				Boss_y[4] = player_y+2; 
				Boss_y[5] = player_y; 
				Boss_y[6] = player_y-1; 
				Boss_y[7] = player_y+1; 
				Boss_y[8] = player_y-1; 
				Boss_y[9] = player_y+1; // Set coords 
				for(int i = 0; i < 10; i++) 
					Display[Boss_x[i]][Boss_y[i]] = 6; // Display boss 
				Boss_skill = 1; 
			}
		}
	}
	
//*************************************************************************** 
// Boss collision attack 
	for(int i = 0; i < 10; i++) 
		if(Boss_x[i] == player_x && Boss_y[i] == player_y) GameState = 3; 
	
//*************************************************************************** 
// Boss bullet attack 
	if(Boss_appear == 1 && Boss_skill == 0){ // Appeared but not skilled 
		Display[Boss_atk_x][Boss_atk_y] = 0; // Create Bullet Stream 
		Boss_atk_x++; 
		Boss_atk_y = Boss_y[0]; 
		if(Boss_atk_x >= height) Boss_atk_x = 2; // Boundary restrictions should before access  
		Display[Boss_atk_x][Boss_atk_y] = 7; // Bug here: access out of bounds 
	}
	
//*************************************************************************** 
// Draw walls 
	for(int i = 0; i < height; i++){ 
			for(int j = 0; j < width; j++){ 
				if(j == width - 1) 
					Display[i][j] = 4; 
				else if(i == height - 1) 
					Display[i][j] = 5; 
			}
		}
	
//*************************************************************************** 
// Draw Boss 
	if(Score >= Boss_ScoreTrigger){ 
			Boss_appear = 1; 
			for(int i = 0; i < 10; i++) 
				Display[Boss_x[i]][Boss_y[i]] = 6; 
	}
	
//*************************************************************************** 
// Victory 
	if(Boss_HP <= 0) GameState = 2; 
}



void Init(void){ 
	srand((unsigned)time(NULL)); // Random seed 
	
	for(int i = 0; i < height; i++) 
		for(int j = 0; j < width; j++) 
			Display[i][j] = 0; // Clear Display 
	
	player_x = height - 2; 
	player_y = width / 2; 
	Display[player_x][player_y] = 1; // Initial Position 
	
	for(int k = 0; k < nEnemies; k++){ // Initialize & Display Enemies 
		Enemy_x[k] = rand() % 2; 
		Enemy_y[k] = rand() % (width - 1); 
		Display[Enemy_x[k]][Enemy_y[k]] = 2; 
	}
	
	GameState = 0; // Initialize Mode 
	Score = 0; // Initialize Score 
	
	
	Boss_HP = Boss_InitialHP; 		// Boss HP 250 
	Boss_appear = 0; 	// Boss not appearing 
	Boss_move = 1; 		// Boss is moving 
	Boss_skill = 0; 	// Boss is not skilled 
	
	Boss_x[0] = 1; Boss_y[0] = 12; 
	Boss_x[1] = 1; Boss_y[1] = 11; 
	Boss_x[2] = 1; Boss_y[2] = 10; 
	Boss_x[3] = 1; Boss_y[3] = 13; 
	Boss_x[4] = 1; Boss_y[4] = 14; 
	Boss_x[5] = 0; Boss_y[5] = 12; 
	Boss_x[6] = 0; Boss_y[6] = 11; 
	Boss_x[7] = 0; Boss_y[7] = 13; 
	Boss_x[8] = 2; Boss_y[8] = 11; 
	Boss_x[9] = 2; Boss_y[9] = 13; // Initialize Boss Shape 
	
	Boss_atk_x = Boss_x[0] + 1; 
	Boss_atk_y = Boss_y[0]; // Initialize BossGun Position 
}



void MovePlayer(void){ 
	if(kbhit()){ // On hitting keaboard... 
		switch(getch()){ // !!Changed to switch-case structure 
			case 'w': 
			case 'W': 
				if(player_x > 0){ // Forward 
					Display[player_x][player_y] = 0; 
					player_x--; 
					Display[player_x][player_y] = 1; 
				} break; 
			case 's': 
			case 'S': 
				if(player_x < height - 2){ // Backward 
					Display[player_x][player_y] = 0; 
					player_x++; 
					Display[player_x][player_y] = 1; 
				} break; 
			case 'a': 
			case 'A': 
				if(player_y > 0){ // Left 
					Display[player_x][player_y] = 0; 
					player_y--; 
					Display[player_x][player_y] = 1; 
				} break; 
			case 'd': 
			case 'D': 
				if(player_y < width - 2){ // Right 
					Display[player_x][player_y] = 0; 
					player_y++; 
					Display[player_x][player_y] = 1; 
				} break; 
			case 'z': 
				GameState = 1; break; // Exit 
			case 'j': 
				Display[player_x - 1][player_y] = 3; break; // Fire Bullet 
		}
	}
}



int main(void){ 
	system("mode con cols=28 lines=27"); 
	system("title "); 
	system("color 08"); 
	system ("cls"); 
	HideCursor(); // Window Init 
	
	Init(); // Game Init 
	
	unsigned int iteration = 0; 
	do{ 
		puts("[ WASD ] 上下左右"); 
		puts("[ J ] 键射击\n"); 
		printf("击败敌机: [ %d ] 架\n", Score); 
		if(Boss_appear == 1){ // Flash boss title 
				if(iteration & 0x08) 
					puts("警告!BOSS出现!"); 
				else puts("                "); 
				printf("BOSS血量: [ %3d ] ", Boss_HP); 
		}
		Render(); // Render 
		EngineCycle(); // Engine Cycle 
		MovePlayer(); // Update Player 
		
		Sleep(10); // !!Slowing down main loop to avoid flashes 
		iteration++; 
	}while(!GameState); 
	system("cls"); 
	
	// Print Blank 
	for(int i = 0; i < 10; i++) putch('\n'); 
	for(int i= 0; i < 12; i++) putch(' '); 
	
	switch(GameState){ // !!Changed to switch-case structure 
		case 1: { 
			printf("\n\t  您一共击毁\n\t    [ %d ]\n\t  架敌方飞机\n\n\n\t", Score); 
			printf(" 退出游戏成功\n\n"); 
			break; 
		}
		case 2: { 
			printf("\n\t  您一共击毁\n\t    [ %d ]\n\t  架敌方飞机\n\n\n\t 恭喜您胜利了\n", Score); 
			break; 
		}
		default: { 
			printf("\b你失败了\n"); 
			break; 
		}
	}
	Sleep(2000); 
	system("Pause"); 
	return 0; 
}


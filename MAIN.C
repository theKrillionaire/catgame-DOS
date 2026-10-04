#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <conio.h>
#include <dos.h>

enum bool {
    false,
    true
};

enum catMoods {
    M_NRML,
    M_HNRY,
    M_SLPY,
    M_SLPG,
	M_DEAD
};

struct catStats {
    int food;
    int sleep;
    int sleeping;
	int mood;
};




void update(struct catStats* stats) {
    stats->food -= 1;
    if(!stats->sleeping) stats->sleep -= 1;
    else stats->sleep += 1;
	if(stats->sleep >= 100) stats->sleeping = false;	
}

void drawImage(char* image, int posX, int posY, int size) {
	int sprSize = size * size;
	FILE* outbin;
	char fileName[32];
	snprintf(fileName, 32, "images/%s", image);
	
    outbin = fopen(fileName, "rb");

    if (outbin != NULL) {
    	int i = 0;
		int color = 0;
        while((color = fgetc(outbin)) != EOF) {
        _setcolor(color);
        _setpixel(i % size + posX, i / size + posY);
		i++;
        }
    }
}

void drawCat(int mood) {
    switch (mood) {
		case M_NRML:
			drawImage("happy.spr", 144,84, 32);
			break;
			
		case M_HNRY:
			drawImage("sad.spr", 144,84, 32);
			break;
			
		case M_SLPY:
			drawImage("tired.spr", 144,84, 32);
			break;
			
		case M_SLPG:
			drawImage("sleep.spr", 144,84, 32);
			break;
		case M_DEAD:
			drawImage("dead.spr", 144, 84, 32);
			break;
		
	}
}

int main() {
	
    struct catStats stats;
    struct catStats oldStats;
    char buffer[34];
    int running = true;
    unsigned long tick;
    unsigned long lastTick = 0;
	int g = 0;

    
    stats.food = 100;
    stats.sleep = 100;
    stats.sleeping = false;
	stats.mood = M_NRML;



	
    if(_setvideomode(_MRES16COLOR) == 0) {
        printf("Error! your PC does not support the video mode!");
    }
	
	_settextposition(5,13);
	_outtext("Simple Cat Game");
	_settextposition(15, 5);
	_outtext("Press enter to start the game!");
	_settextcolor(8);
	_settextposition(24,22);
	_outtext("by theKrillionaire");
	_settextcolor(7);
	g = getch();
	if( g == 'e' || g == 'E' ) {
		_setvideomode(_DEFAULTMODE);
		return 0;
	}
	
	_clearscreen(_GCLEARSCREEN);
	
    drawImage("catBody.spr", 128,68, 64);
	drawCat(M_NRML);

    while(running) {
		
		if( stats.food <= 0 || stats.sleep <= 0) {
			_settextposition(7,13); 
			_outtext("Your cat died!!");
			_settextposition(9,5);
			_outtext("Press any key to quit, thank you!");
			drawCat(M_DEAD);
			getch();
			_setvideomode(_DEFAULTMODE);
			return 0;
		}
		
        tick = *(unsigned long far *)MK_FP(0x40,0x6c);

        if(tick - lastTick >= 5) { 
            lastTick = tick;
    
            oldStats.food = stats.food;
            oldStats.sleep = stats.sleep;
            oldStats.sleeping = stats.sleeping;
			oldStats.mood = stats.mood;

            update(&stats);
			
			if(stats.sleeping) stats.mood = M_SLPG;
			else if(stats.food <= 25) stats.mood = M_HNRY;
			else if(stats.sleep <= 25) stats.mood = M_SLPY;
			else stats.mood = M_NRML;
			
			if(stats.mood != oldStats.mood) {
				drawCat(stats.mood);
			}
			
			
			_setcolor(0);
			_rectangle(_GFILLINTERIOR, 32, 168, 311, 175);
			snprintf(buffer,34,"food %i, sleep %i, sleeping %s", stats.food, stats.sleep, stats.sleeping ? "Yes" : "No");
			
			_settextposition(22, 5);
			_outtext(buffer);
        }

        if(kbhit()) { 
            int key = getch();

            if(key == 'f' || key == 'F') {
                if(stats.food <= 98) {
                    stats.food += 2;
                }
            }
			else if(key == 's' || key == 'S') {
				stats.sleeping = !stats.sleeping;
			}
            else if(key == 'e' || key == 'E') {
                _setvideomode(_DEFAULTMODE);
                return 0;
            }

        }
    }
    

    getch();
    _setvideomode(_DEFAULTMODE);
    return 0;
}

#include <stdio.h>
#include <graph.h>
#include <conio.h>
#include <dos.h>

enum bool {
    false,
    true
};

struct catStats {
    int food;
    int sleep;
    int sleeping;

};


void update(struct catStats* stats) {
    stats->food -= 1;
    if(!stats->sleeping) stats->sleep -= 1;
    else stats->sleep += 1;
}

int main() {

    struct catStats stats;
    char buffer[256];
    int running = true;
    unsigned long tick;
    unsigned long lastTick;

    
    stats.food = 100;
    stats.sleep = 100;
    stats.sleeping = false;


    if(_setvideomode(_MRES16COLOR) == 0) {
        printf("Error! your PC does not support the video mode!");
    }

    while(running) {
        tick = *(unsigned long far *)MK_FP(0x40,0x6c);
        _clearscreen(_GCLEARSCREEN);
        snprintf(buffer,256,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n   food %i, sleep %i, sleeping %s", stats.food, stats.sleep, stats.sleeping ? "Yes" : "No");
        _outtext(buffer);

        if(tick - lastTick >= 7) { 
            lastTick = tick;
            update(&stats);
        }

        if(kbhit()) { 
            int key = getch();

            if(key == 'f' || key == 'F') {
                if(stats.food <= 98) {
                    stats.food += 2;
                }
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

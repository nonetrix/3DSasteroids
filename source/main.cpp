#include <3ds.h>
#include <iostream>
#include <unistd.h>

//TODO add a button to change the amount of asteroids using classes :D 

class enity {
    public:
        int x = 1;
        int y = 1;

        void addpos(int xa, int ya) 
        {
            x += + xa;
            y += + ya;
        }

        void removepos(int xa, int ya) 
        {
            x += - xa;
            y += - ya;
        }

        void setpos(int sx, int sy) 
        {
            x = sx;
            y = sy;
        }
};

class UI {
    public:
        void clear() 
        {
            printf("\x1B[2J\x1B[H");
        }

        /* see comment in player.die
        void printdead() {
            
        }
        */
};

class player : public enity, UI {
    public:
        int score = 0;
        int highscore = 0;
        char text = '|';

        void updatepos() 
        {
            printf("\x1b[0;1m\x1b[%d;%dH%c\n", y, x, text);
        }

        void die() {
            if(score > highscore) 
            {
                highscore = score;
            }
            //TODO find out how to move this to UI class without 10,000 compiler errors 
            printf("\x1b[10;17H\x1b[0;31mYou died! Press(A)\x1b[0;0m");
            printf("\x1b[14;17H\x1b[0;0mScore: %d\x1b[0;0m", score);
            printf("\x1b[15;17H\x1b[0;0mHigh score: %d\x1b[0;0m", highscore);
            
            x = 0;
            y = 0;

            score = 0;
            while(1) {
                //Update the screen
                gfxFlushBuffers();
		        gfxSwapBuffers();
		        gspWaitForVBlank();
                
                //Check pressed buttons
                hidScanInput();
                u32 kDown = hidKeysDown();
                
                if(kDown & KEY_A) 
                {
                    break;
                }
            }
        }
};

class asteroid : public enity {
    public:
        char text = '@';

        void updatepos() 
        {
            printf("\x1b[0;47m\x1b[%d;%dH@\n\x1b[1;40m", y, x);
        }
};


int main() {
    player player1;
    asteroid asteroid1;
    UI UI;
    
    gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

    while(aptMainLoop()) {
        
        UI.clear();

        player1.updatepos();
        asteroid1.updatepos();
        asteroid1.y--;

        if(asteroid1.y < 1) 
        {
            asteroid1.y = 29;
            asteroid1.x = rand() % 50;
        }
        
        if(player1.x == asteroid1.x && player1.y == asteroid1.y) 
        {
            player1.die();
        }

        //Check pressed buttons
        hidScanInput();
        u32 kDown = hidKeysDown();

        if(kDown & KEY_UP && player1.y > 1) 
        {
            player1.text = '|';
            player1.removepos(0, 1);
        } 
        
        else if(kDown & KEY_DOWN && player1.y < 29) 
        {
            player1.text = '|';
            player1.addpos(0, 1);
        } 
        
        else if(kDown & KEY_LEFT && player1.x > 1) 
        {
            player1.text = '-';
            player1.removepos(1, 0);
        } 
        
        else if(kDown & KEY_RIGHT && player1.x < 50) 
        {
            player1.text = '-';
            player1.addpos(1, 0);
        } 

        /*
        else if(kDown & KEY_A) 
        {
            player1.die();
        }
        */
        
        //update score
        player1.score++;

        //update the screen
        gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
    }
}

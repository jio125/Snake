#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>

const int width = 120;
const int height = 60;
WINDOW *win; //creates pointer to game window

//define global variables
int headX, headY, tailX, tailY, fruitX, fruitY, length, score;
bool gameOver;
typedef enum { //stores current direction of snakes motion
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
} Direction;
typedef struct{ //stores snake body
    int x;
    int y;
    Direction dir;
} snakeBody;
snakeBody snake[6608];

//functions used
void setup(); //initializes the game window 
void draw(); //draws each game instance to the screen
void input(); //takes keyboard input
void logic(); //performs logic such as collision detection
void delay(int number_of_seconds); //creates delay to slow game execution

int main(){
play:
    setup();
    draw();
    while(!gameOver){
        draw();
        input();
        logic();
    }
    if(gameOver){
        mvwprintw(win, height/2, (width/2) - 18, "GAME OVER - PLAY AGAIN? (y/n)");
        nodelay(win, false);
        char again = wgetch(win);
        while ((again != 'y') && (again != 'n')){ //prevents accidental closure of game at game over
            again = wgetch(win);
        }
        if(again == 'y'){
            gameOver = false;
            goto play;
        }
        else if(again == 'n'){
            endwin(); //ends ncurses mode
            return 0;
        }
    }
}

void setup(){
    system("resize -s 60 120"); //sets the terminal window size
    system("clear"); //clears the terminal screen
    srand(time(0)); //seeds the random number generator with the current time

    initscr();  //initialize ncurses
    win = newwin(height, width, 0, 0); //create new window for game    
    noecho(); //disables echoing of input
    keypad(win, TRUE); //enables function keys
    raw(); //enables direct input (dont wait for enter)
    nodelay(win, TRUE); //makes input detection non-blocking
    curs_set(0);
    refresh(); //loads new window
    
    wborder(win,'|','|','=','=','+','+','+','+'); //creates border for game window
    mvwprintw(win, 1, 55, "Snake Game");
    int count;
    for(count = 1; count<= width - 2; count++){
        mvwprintw(win, 2, count, "=");
    }
    wrefresh(win); //refreshes the screen

    //initialize variables
    gameOver = false; 
    fruitX = (rand() % (width - 2)) + 1; //sets coordinate for fruit
    fruitY = (rand() % (height - 4)) + 1;
    length = 1;
    score = 0;
    headX = 20; //initial snake position
    headY = 20;
    tailX = 19;
    tailY = 20;
    for(int i = 0; i < (sizeof(snake))/(sizeof(snakeBody)); i++){
        snake[i].x = 0;
        snake[i].y = 0;
        snake[i].dir = STOP;
    }
    snake[0].x = headX;
    snake[0].y = headY;
    snake[0].dir = STOP;
}

void draw(){
    for(int i = 0; i < length; i++){
        mvwprintw(win, snake[i].y, snake[i].x, "*");
    }
    mvwprintw(win, tailY, tailX, " ");
    mvwprintw(win, 0, 0, "+"); //prevent corner from being cleared
    mvwprintw(win, fruitY, fruitX, "8"); //prints fruit
    mvwprintw(win, 1, 2, "Current length: %d", length);
    mvwprintw(win, 1, 25, "Score: %d", score);
    wrefresh(win);
    delay(100);//slows down the game to a playable speed
}

void input(){
    int in = wgetch(win); //gets user input
    if(in != ERR){
        switch(in){ //assigns direction based on keypress
            case KEY_UP:
                tailX = snake[length - 1].x;
                tailY = snake[length - 1].y;
                for(int i = length - 1; i > 0; i--){
                    snake[i].x = snake[i - 1].x;
                    snake[i].y = snake[i - 1].y;
                    snake[i].dir = snake[i - 1].dir;
                }
                snake[0].y = snake[0].y - 1;
                snake[0].dir = UP;
                break;
            case KEY_DOWN:
                tailX = snake[length - 1].x;
                tailY = snake[length - 1].y;
                for(int i = length - 1; i > 0; i--){
                    snake[i].x = snake[i - 1].x;
                    snake[i].y = snake[i - 1].y;
                    snake[i].dir = snake[i - 1].dir;
                }
                snake[0].y = snake[0].y + 1;
                snake[0].dir = DOWN;
                break;
            case KEY_RIGHT:
                tailX = snake[length - 1].x;
                tailY = snake[length - 1].y;
                for(int i = length - 1; i > 0; i--){
                    snake[i].x = snake[i - 1].x;
                    snake[i].y = snake[i - 1].y;
                    snake[i].dir = snake[i - 1].dir;
                }
                snake[0].x = snake[0].x + 1;
                snake[0].dir = RIGHT;
                break;
            case KEY_LEFT:
                tailX = snake[length - 1].x;
                tailY = snake[length - 1].y;
                for(int i = length - 1; i > 0; i--){
                    snake[i].x = snake[i - 1].x;
                    snake[i].y = snake[i - 1].y;
                    snake[i].dir = snake[i - 1].dir;
                }
                snake[0].x = snake[0].x - 1;
                snake[0].dir = LEFT;
                break;
            case 'x': //allows for quit functionality
                gameOver = true;
                break;
        }
        headX = snake[0].x;
        headY = snake[0].y;
    }
    else if(in == ERR){
       switch(snake[0].dir){
            case STOP:
                 break;
            case UP:
                tailX = snake[length - 1].x;
                tailY = snake[length - 1].y;
                for(int i = length - 1; i > 0; i--){
                    snake[i].x = snake[i - 1].x;
                    snake[i].y = snake[i - 1].y;
                    snake[i].dir = snake[i - 1].dir;
                }
                snake[0].y = snake[0].y - 1;        
                break;
            case DOWN:
                tailX = snake[length - 1].x;
                tailY = snake[length - 1].y;
                for(int i = length - 1; i > 0; i--){
                    snake[i].x = snake[i - 1].x;
                    snake[i].y = snake[i - 1].y;
                    snake[i].dir = snake[i - 1].dir;
                }
                snake[0].y = snake[0].y + 1;
                break;
            case RIGHT:
                tailX = snake[length - 1].x;
                tailY = snake[length - 1].y;
                for(int i = length - 1; i > 0; i--){
                    snake[i].x = snake[i - 1].x;
                    snake[i].y = snake[i - 1].y;
                    snake[i].dir = snake[i - 1].dir;
                }
                snake[0].x = snake[0].x + 1;
                break;
            case LEFT:
                tailX = snake[length - 1].x;
                tailY = snake[length - 1].y;
                for(int i = length - 1; i > 0; i--){
                    snake[i].x = snake[i - 1].x;
                    snake[i].y = snake[i - 1].y;
                    snake[i].dir = snake[i - 1].dir;
                }
                snake[0].x = snake[0].x - 1;
                break;
        }
        headX = snake[0].x;
        headY = snake[0].y;
    }
}

void logic(){
    if ((headX == fruitX) && (headY == fruitY)){ //checks if fruit was eaten
        length = length + 1;
        score = score + 10;
        fruitX = (rand() % (width - 2)) + 1; //sets new fruit coordinates
        fruitY = (rand() % (height - 4)) + 3;
        bool clear;
        for(int i = 0; i < length; i++){
            if(fruitX == snake[i].x && fruitY == snake[i].y){
                clear = false;
                while(!clear){
                    fruitX = (rand() % (width - 2)) + 1;
                    fruitY = (rand() % (height - 4)) + 3;
                    if(fruitX != snake[i].x && fruitY != snake[i].y){
                        clear = true;
                    }
                }
            }
        }
    }
    if((headX == 0) || (headX == width - 1) || (headY == 2) || (headY == height - 1)){ //checks wall collisions
        gameOver = true;
    }
    for(int i = length - 1; i >0; i--){ //checks for self collisions
        if(headX == snake[i].x && headY == snake[i].y){
            gameOver = true;
        }
    }
}

void delay(int number_of_seconds){
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while(clock() < start_time + milli_seconds){
    }
}
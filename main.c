#include "stdio.h"
#include "stdlib.h"
#include "sdl.h"
#include "SDL2_gfxPrimitives.h"
#include "time.h"

#include "formulas.h"
#include "wall.h"
#include "robot.h"

int done = 0;


int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return 1;
    }

    window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, 0);

    struct Robot robot;
    struct Wall_collection *head = NULL;
    int front_right_sensor,front_left_diagonal_sensor,front_right_diagonal_sensor,left_sensor=0;
    clock_t start_time, end_time;
    int msec;

    int mazeOpt = 0;

    if(mazeOpt == 0) {
        insertAndSetFirstWall(&head, 1,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2, 10, OVERALL_WINDOW_HEIGHT/2);
        insertAndSetFirstWall(&head, 2,  OVERALL_WINDOW_WIDTH/2-100, OVERALL_WINDOW_HEIGHT/2+100, 10, OVERALL_WINDOW_HEIGHT/2-100);
        insertAndSetFirstWall(&head, 3,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2+100, 150, 10);
        insertAndSetFirstWall(&head, 4,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2, 150, 10);
        insertAndSetFirstWall(&head, 5,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-200, 10, 300);
        insertAndSetFirstWall(&head, 6,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2-100, 10, 100);
        insertAndSetFirstWall(&head, 7,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-200, 450, 10);
        insertAndSetFirstWall(&head, 8,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2-100, 250, 10);
        insertAndSetFirstWall(&head, 9,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2-200, 10, 300);
        insertAndSetFirstWall(&head, 10,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2-100, 10, 300);
        insertAndSetFirstWall(&head, 11,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2+200, OVERALL_WINDOW_WIDTH/2-100, 10);
        insertAndSetFirstWall(&head, 12,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2+100, OVERALL_WINDOW_WIDTH/2-100, 10);
    } else if(mazeOpt == 1) {
        //Maze Design 1
        insertAndSetFirstWall(&head, 1,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-175, 10, OVERALL_WINDOW_HEIGHT/5);
        insertAndSetFirstWall(&head, 2,  OVERALL_WINDOW_WIDTH/2-140, OVERALL_WINDOW_HEIGHT/2-175, 10, OVERALL_WINDOW_HEIGHT/5);
        insertAndSetFirstWall(&head, 3,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-175, OVERALL_WINDOW_HEIGHT/4, 10);
        insertAndSetFirstWall(&head, 4,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-85, OVERALL_WINDOW_HEIGHT/4, 10);
        insertAndSetFirstWall(&head, 5,  OVERALL_WINDOW_WIDTH/2+125, OVERALL_WINDOW_HEIGHT/2+50, 10, OVERALL_WINDOW_HEIGHT/4.5);
        insertAndSetFirstWall(&head, 6,  OVERALL_WINDOW_WIDTH/2+245, OVERALL_WINDOW_HEIGHT/2+50, 10, OVERALL_WINDOW_HEIGHT/4.35);
        insertAndSetFirstWall(&head, 7,  OVERALL_WINDOW_WIDTH/2+125, OVERALL_WINDOW_HEIGHT/2+50, OVERALL_WINDOW_HEIGHT/4, 10);
        insertAndSetFirstWall(&head, 8,  OVERALL_WINDOW_WIDTH/2+125, OVERALL_WINDOW_HEIGHT/2+150, OVERALL_WINDOW_HEIGHT/4, 10);
        insertAndSetFirstWall(&head, 9,  OVERALL_WINDOW_WIDTH/2-225, OVERALL_WINDOW_HEIGHT/2+50, 10, OVERALL_WINDOW_HEIGHT/2);
        insertAndSetFirstWall(&head, 10,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2+50, 10, OVERALL_WINDOW_HEIGHT/2);
        insertAndSetFirstWall(&head, 11,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2+50, OVERALL_WINDOW_HEIGHT/2.5, 10);
        insertAndSetFirstWall(&head, 12,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2+140, OVERALL_WINDOW_HEIGHT/4, 10);
        insertAndSetFirstWall(&head, 13,  OVERALL_WINDOW_WIDTH/2-40, OVERALL_WINDOW_HEIGHT/2+140, 10, OVERALL_WINDOW_HEIGHT/13);
        insertAndSetFirstWall(&head, 14,  OVERALL_WINDOW_WIDTH/2+125, OVERALL_WINDOW_HEIGHT/2-25, OVERALL_WINDOW_HEIGHT/2, 10);
        insertAndSetFirstWall(&head, 15,  OVERALL_WINDOW_WIDTH/2-180, OVERALL_WINDOW_HEIGHT/2-25, OVERALL_WINDOW_HEIGHT/4, 10);
        insertAndSetFirstWall(&head, 16,  OVERALL_WINDOW_WIDTH/2-65, OVERALL_WINDOW_HEIGHT/2-175, 10, OVERALL_WINDOW_HEIGHT/3);
        insertAndSetFirstWall(&head, 17,  OVERALL_WINDOW_WIDTH/2-55, OVERALL_WINDOW_HEIGHT/2-175, OVERALL_WINDOW_HEIGHT/1.25, 10);
        insertAndSetFirstWall(&head, 18,  OVERALL_WINDOW_WIDTH/2+150, OVERALL_WINDOW_HEIGHT/2-175, 10, OVERALL_WINDOW_HEIGHT/6.85);
        insertAndSetFirstWall(&head, 19,  OVERALL_WINDOW_WIDTH/2+60, OVERALL_WINDOW_HEIGHT/2-115, OVERALL_WINDOW_HEIGHT/5, 10);
        insertAndSetFirstWall(&head, 20,  OVERALL_WINDOW_WIDTH/2-350, OVERALL_WINDOW_HEIGHT/2-240, OVERALL_WINDOW_HEIGHT/1.005, 10);
        insertAndSetFirstWall(&head, 21,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2-240, OVERALL_WINDOW_HEIGHT/1.005, 10);
        insertAndSetFirstWall(&head, 22,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2+230, OVERALL_WINDOW_HEIGHT/1.005, 10);
        insertAndSetFirstWall(&head, 23,  OVERALL_WINDOW_WIDTH/2-340, OVERALL_WINDOW_HEIGHT/2+230, OVERALL_WINDOW_HEIGHT/4, 10);
        insertAndSetFirstWall(&head, 24,  OVERALL_WINDOW_WIDTH/2-320, OVERALL_WINDOW_HEIGHT/2-250, 10, OVERALL_WINDOW_HEIGHT/1);
        insertAndSetFirstWall(&head, 25,  OVERALL_WINDOW_WIDTH/2+310, OVERALL_WINDOW_HEIGHT/2-265, 10, OVERALL_WINDOW_HEIGHT/5);
        insertAndSetFirstWall(&head, 26,  OVERALL_WINDOW_WIDTH/2+310, OVERALL_WINDOW_HEIGHT/2-85, 10, OVERALL_WINDOW_HEIGHT/7);
        insertAndSetFirstWall(&head, 27,  OVERALL_WINDOW_WIDTH/2+250, OVERALL_WINDOW_HEIGHT/2-85, OVERALL_WINDOW_HEIGHT/7, 10);
        insertAndSetFirstWall(&head, 28,  OVERALL_WINDOW_WIDTH/2+310, OVERALL_WINDOW_HEIGHT/2-15, 10, OVERALL_WINDOW_HEIGHT/1.05);
    } else if(mazeOpt == 2) {
        // Maze Design 2

            int gap_width = 100;
        int centreX = OVERALL_WINDOW_WIDTH/2;   //These integers determine the position of the
        int centreY = OVERALL_WINDOW_HEIGHT/2;  //centre of the circle
        float wallWidth = 5;
        float resolution = 0.07;  //lower values give greater resolution, any less than 0.000001 requires at minimum 32Gb ram

        //spawn tube
        insertAndSetFirstWall(&head, 1, 30, 183, 5, 300);
        insertAndSetFirstWall(&head, 1, 107, 290, 5, 300);
        insertAndSetFirstWall(&head, 1, 30, 183, 80, 5);

        float radius = 220;
        for(float x = -radius; x <= radius; x+=resolution) {
            //outer circle
            if (x >= -radius + 7.5) {
                insertAndSetFirstWall(&head, x, centreX + x, centreY - sqrt(radius*radius - x*x),wallWidth, wallWidth);
                insertAndSetFirstWall(&head, radius+x, centreX + x, centreY + sqrt(radius*radius - x*x), wallWidth, wallWidth);
            }
            //diagonal lines
            if (x>12 && x<95) {
                insertAndSetFirstWall(&head, x, centreX+100+x, centreY-35-x/1.5, 5, 5);
            }
            if (x>-60 && x<60) {
                insertAndSetFirstWall(&head, x, centreX+50+x, centreY-80+x/1.5, 5, 5);
            }
            if (x>-60 && x<60) {
                insertAndSetFirstWall(&head, x, centreX+50+x, centreY+x/1.5, 5, 5);
            }
            if (x>5 && x<40) {
                insertAndSetFirstWall(&head, x, centreX+80+x, centreY-130-x/0.8, 5, 5);
            }
            if (x>-65 && x<-50) {
                insertAndSetFirstWall(&head, x, centreX+40+x, centreY-290-x/0.2, 5, 5);
            }
        }

        //internal circle section
        radius -= gap_width;
        for(float x = -radius; x <= radius; x+=resolution) {
            if (x <= radius - 7.5) {
                if (x <= radius - 130) {
                    insertAndSetFirstWall(&head, x, centreX + x, centreY - sqrt(radius*radius - x*x),wallWidth, wallWidth);
                }
                insertAndSetFirstWall(&head, radius+x, centreX + x, centreY + sqrt(radius*radius - x*x), wallWidth, wallWidth);
            }
        }
    } else if(mazeOpt == 3) {
         /* Outside border */
        insertAndSetFirstWall(&head, 2,  0, 0, 320, 10);         /*broken line on top*/
        insertAndSetFirstWall(&head, 2,  370, 0, 290, 10);
        insertAndSetFirstWall(&head, 3,  0, 0, 10, 480);    /*left side*/
        insertAndSetFirstWall(&head, 4,  630, 0, 10, 480);  /*right side*/
        insertAndSetFirstWall(&head, 5,  0, 470, 640, 10);  /*bottom*/
      /* Map */
        insertAndSetFirstWall(&head, 2,  320, 0, 10, 90);
        insertAndSetFirstWall(&head, 2,  370, 0, 10, 30);
        insertAndSetFirstWall(&head, 2,  320, 90, 50, 10);
      /* Map */
        insertAndSetFirstWall(&head, 2,  320, 0, 10, 90);
        insertAndSetFirstWall(&head, 2,  370, 0, 10, 30);
        insertAndSetFirstWall(&head, 2,  320, 90, 50, 10);
    }

    // SETUP MAZE
    // You can create your own maze here. line of code is adding a wall.
    // You describe position of top left corner of wall (x, y), then width and height going down/to right
    // Relative positions are used (OVERALL_WINDOW_WIDTH and OVERALL_WINDOW_HEIGHT)
    // But you can use absolute positions. 10 is used as the width, but you can change this.


    setup_robot(&robot);
    updateAllWalls(head, renderer);

    SDL_Event event;
    while(!done){
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        //Move robot based on user input commands/auto commands
        if (robot.auto_mode == 1)
            robotAutoMotorMove(&robot, front_right_sensor, front_right_diagonal_sensor, front_left_diagonal_sensor,left_sensor);
        robotMotorMove(&robot);

        //Check if robot reaches endpoint. and check sensor values
        if (checkRobotReachedEnd(&robot, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT/2+100, 10, 100)){
            end_time = clock();
            msec = (end_time-start_time) * 1000 / CLOCKS_PER_SEC;
            robotSuccess(&robot, msec);
        }
        else if(checkRobotHitWalls(&robot, head))
            robotCrash(&robot);
        //Otherwise compute sensor information
        else {
            front_left_diagonal_sensor = checkRobotSensorFrontLeftDiagonalAllWalls(&robot, head);
            if (front_left_diagonal_sensor>0)
                printf("Getting close on the left diag. Score = %d\n", front_left_diagonal_sensor);

            front_right_sensor = checkRobotSensorFrontRightAllWalls(&robot, head);
            if (front_right_sensor>0)
                printf("Getting close on the front. Score = %d\n", front_right_sensor);

            front_right_diagonal_sensor = checkRobotSensorFrontRightDiagonalAllWalls(&robot, head);
            if (front_right_diagonal_sensor>0)
                printf("Getting close on the right diag. Score = %d\n", front_right_diagonal_sensor);

            left_sensor = checkRobotSensorLeftAllWalls(&robot, head);
            if(left_sensor>0)
                printf("Getting close on the left. Score = %d\n", left_sensor);
        }
        robotUpdate(renderer, &robot);
        updateAllWalls(head, renderer);

        // Check for user input
        SDL_RenderPresent(renderer);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                done = 1;
            }
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            if(state[SDL_SCANCODE_UP] && robot.direction != DOWN){
                robot.direction = UP;
            }
            if(state[SDL_SCANCODE_DOWN] && robot.direction != UP){
                robot.direction = DOWN;
            }
            if(state[SDL_SCANCODE_LEFT] && robot.direction != RIGHT){
                robot.direction = LEFT;
            }
            if(state[SDL_SCANCODE_RIGHT] && robot.direction != LEFT){
                robot.direction = RIGHT;
            }
            if(state[SDL_SCANCODE_SPACE]){
                setup_robot(&robot);
            }
            if(state[SDL_SCANCODE_RETURN]){
                robot.auto_mode = 1;
                start_time = clock();
            }
        }

        SDL_Delay(120);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    printf("DEAD\n");
}

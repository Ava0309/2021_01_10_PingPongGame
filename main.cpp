#include <bits/stdc++.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <time.h>
#include <windows.h>
//----------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//----------------------------------------------------------------------
#include "util.hpp"
//----------------------------------------------------------------------
using namespace std;

//----------------------------------------------------------------------
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int CENTER_WIDTH = 5;

const int PLAYER_SPEED = 15;
const int PLAYER_ACCLERATE = 5;

const float BALL_MAXSPEED = 10.0f;
const float BALL_ACCELERATE = 1.05f;
const float BALL_INIT_SPEED = 4.0f;
const int BALL_WIDTH = 10;
const int BALL_HEIGHT = 10;
const int BALL_INITIAL_X_POSITION = 315;
const int BALL_INITIAL_Y_POSITION = 235;

const double PI = atan(1) * 4;

typedef struct {
   SDL_Rect pos;
   int speed;
   int score;
} Player;

typedef struct {
   float x;
   float y;
   float vx;
   float vy;
   float speed;
} Ball;

//----------------------------------------------------------------------
// functions
//----------------------------------------------------------------------
int main (int argc, char* argv[])
{
   puts("Starting SDL Application...");
   SDL_Event e;
   SDL_Renderer *ren = nullptr;
   SDL_Window *win = nullptr;

   initialize(&ren, &win, SCREEN_WIDTH, SCREEN_HEIGHT);

   int board_width, board_height;
   SDL_Texture *squareTex = IMG_LoadTexture(ren, "img/pong_board.png");
   SDL_QueryTexture(squareTex, NULL, NULL, &board_width, &board_height);
   
   float red, green, blue;
   srand(time(NULL));
   red = rand() % 256;
   green = rand() % 256;
   blue = rand() % 256;
   
   SDL_Color ChangeColor = {red, green, blue};
   SDL_Color p1_Score_Color = {255, 255, 0}, p2_Score_Color = {255, 255, 0};
   
   //-------------------------------------------------------------------
   // Define players
   //-------------------------------------------------------------------
   Player p1, p2;

   // initialize x, y, w, h
   p1.pos = p2.pos = SDL_Rect{
      0,
      0,
      board_width,
      board_height,
   };
   
   p1.speed = p2.speed = PLAYER_SPEED;
   p1.score = p2.score = 0;

   p1.pos.x = board_width/2 + 10;
   p2.pos.x = SCREEN_WIDTH - p2.pos.w- 10 - p2.pos.w/2;

   p1.pos.y = SCREEN_HEIGHT/2 - p1.pos.h/2;
   p2.pos.y = SCREEN_HEIGHT/2 - p2.pos.h/2;


   //-------------------------------------------------------------------
   // Define Ball
   //-------------------------------------------------------------------
   Ball b;
   b.x = BALL_INITIAL_X_POSITION;
   b.y = BALL_INITIAL_Y_POSITION;
   b.vx = 0;
   b.vy = 0;
   b.speed = 0;

   //-------------------------------------------------------------------
   // start the game
   //-------------------------------------------------------------------
   puts("Starting Game Loop");

   bool quit = false;
   char buffer[512];
   const Uint8 *keystates = SDL_GetKeyboardState(NULL);
   int p1_font_size = 40, p2_font_size = 40;
   int pattern = 1;

   unsigned int frame = 0;
   while(!quit) {
      while(SDL_PollEvent(&e)) {
         if(e.type == SDL_QUIT)
            quit = true;
         if(e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.scancode) {
               case SDL_SCANCODE_ESCAPE:
                  quit = true;
                     break;
               }
            }
      }
      
      //----------------------------------------------------------------
      // Game Mode (by Human or by AI)
      //----------------------------------------------------------------
	   if (keystates[SDL_SCANCODE_1] || keystates[SDL_SCANCODE_KP_1])    // Player 1 contorlled by human & Player 2 controlled by human
         pattern = 1;
      if (keystates[SDL_SCANCODE_2] || keystates[SDL_SCANCODE_KP_2])    // Player 1 contorlled by human & Player 2 controlled by AI
         pattern = 2;
      if (keystates[SDL_SCANCODE_3] || keystates[SDL_SCANCODE_KP_3])    // Player 1 contorlled by AI & Player 2 controlled by human
         pattern = 3;
      if (keystates[SDL_SCANCODE_4] || keystates[SDL_SCANCODE_KP_4])    // Player 1 controlled by AI & Player 2 controlled by AI
         pattern = 4;
	   
      //----------------------------------------------------------------
      // Player Movement
      //----------------------------------------------------------------
      switch (pattern) {
		   case 1:                                                        // Player 1 contorlled by human & Player 2 controlled by human
            if(keystates[SDL_SCANCODE_UP])   p1.pos.y -= p1.speed;
              if(keystates[SDL_SCANCODE_DOWN]) p1.pos.y += p1.speed;
              if(keystates[SDL_SCANCODE_W])    p2.pos.y -= p2.speed;
              if(keystates[SDL_SCANCODE_S])    p2.pos.y += p2.speed;
              break;
         case 2:                                                        // Player 1 contorlled by human & Player 2 controlled by AI
            if(keystates[SDL_SCANCODE_UP])   p1.pos.y -= p1.speed;
            if(keystates[SDL_SCANCODE_DOWN]) p1.pos.y += p1.speed;
            if(b.y < p2.pos.y + p2.pos.h/2)  p2.pos.y -= p2.speed;
            if(b.y > p2.pos.y + p2.pos.h/2)  p2.pos.y += p2.speed;
            break;
         case 3:                                                        // Player 1 controlled by AI & Player 2 controlled by human
            if(b.y < p1.pos.y + p1.pos.h/2)  p1.pos.y -= p1.speed;
            if(b.y > p1.pos.y + p1.pos.h/2)  p1.pos.y += p1.speed;
            if(keystates[SDL_SCANCODE_W])    p2.pos.y -= p2.speed;
            if(keystates[SDL_SCANCODE_S])    p2.pos.y += p2.speed;
            break;
         case 4:                                                        // Player 1 controlled by AI & Player 2 controlled by AI
            if(b.y < p1.pos.y + p1.pos.h/2)  p1.pos.y -= p1.speed;
            if(b.y > p1.pos.y + p1.pos.h/2)  p1.pos.y += p1.speed;
            if(b.y < p2.pos.y + p2.pos.h/2)  p2.pos.y -= p2.speed;
            if(b.y > p2.pos.y + p2.pos.h/2)  p2.pos.y += p2.speed;
            break;
      }
      
      //----------------------------------------------------------------
      // Initiate the game and ball moving
      //----------------------------------------------------------------
      if (keystates[SDL_SCANCODE_0] || keystates[SDL_SCANCODE_KP_0] || keystates[SDL_SCANCODE_KP_ENTER]) {
         srand(time(NULL));
       	if (rand() % 2 == 1) {
       	   b.x = p1.pos.x + board_width;
		      b.y = p1.pos.y + p1.pos.h/2;
         }
         else {
            b.x = p2.pos.x;
		      b.y = p2.pos.y + p2.pos.h/2;
         }

         p1_Score_Color = p2_Score_Color = {255, 255, 0};
         double angel = rand() / PI;
         if (angel < PI/2)
            angel = PI/2;
		   b.vx = BALL_INIT_SPEED * cos(angel);
		   b.vy = BALL_INIT_SPEED * sin(angel);
		   b.speed = BALL_INIT_SPEED;
		   p1_font_size = p2_font_size = 40;

		   Sleep(500);
	   }

      //----------------------------------------------------------------
      // Limit Ball's Max Speed
      //----------------------------------------------------------------
      if(b.vx > BALL_MAXSPEED) b.vx = BALL_MAXSPEED;
      if(b.vy > BALL_MAXSPEED) b.vy = BALL_MAXSPEED;

      //----------------------------------------------------------------
      // Update Ball coordinates
      //----------------------------------------------------------------
      b.x += b.vx;
      b.y += b.vy;

      //----------------------------------------------------------------
      // Boundary Collision of Ball
      //----------------------------------------------------------------
      // Y axis
      if (b.y < 0) {
         // Update ball's position and speed
         b.y = 0;
         b.vy *= -1;

         SDL_RenderClear(ren);

         SDL_RenderCopy(ren, squareTex, NULL, &p1.pos);
         SDL_RenderCopy(ren, squareTex, NULL, &p2.pos);
      }
      else
         if(b.y + BALL_HEIGHT >= SCREEN_HEIGHT) {
            // Update ball's position and speed
            b.y = SCREEN_HEIGHT - BALL_HEIGHT;
            b.vy *= -1;
         }

      // X axis
      if(b.x < 0) {
         // Update ball's position and speed, and the player's score
         p2.score ++;
         p2_Score_Color = ChangeColor;
         
         b.x = BALL_INITIAL_X_POSITION;
         b.y = BALL_INITIAL_Y_POSITION;
         b.vx = 0;
         b.vy = 0;
         b.speed = 0;
         
         p2_font_size = 60;
         if (p2.pos.x >= 445)
            p2.pos.x -= 5;

         if (p2.score == 10 || p2.score == 15 || p2.score == 20)
            p1.speed += PLAYER_ACCLERATE;
      }
      else
         if(b.x + BALL_WIDTH >= SCREEN_WIDTH) {
            // Update ball's position and speed, and the player's score
            p1.score ++;
            p1_Score_Color = ChangeColor;
            
            b.x = BALL_INITIAL_X_POSITION;
			   b.y = BALL_INITIAL_Y_POSITION;
			   b.vx = 0;
			   b.vy = 0;
			   b.speed = 0;
			   
			   p1_font_size = 60;
			   if (p1.pos.x <= 195)
			      p1.pos.x += 5;
			   if (p1.score == 10 || p1.score == 15 || p1.score == 20)
               p2.speed += PLAYER_ACCLERATE;
         }

      //----------------------------------------------------------------
      // Boundary Collision of Players
      //----------------------------------------------------------------
      if(p1.pos.y < 0)                          p1.pos.y = 0;
      if(p1.pos.y + p1.pos.h > SCREEN_HEIGHT)   p1.pos.y = SCREEN_HEIGHT - p1.pos.h;
      if(p2.pos.y < 0)                          p2.pos.y = 0;
      if(p2.pos.y + p2.pos.h > SCREEN_HEIGHT)   p2.pos.y = SCREEN_HEIGHT - p2.pos.h;

      //----------------------------------------------------------------
      // Create a dummpy b_rect structure to detect collision
       //----------------------------------------------------------------
      SDL_Rect b_rect  = {
         (int) b.x,
         (int) b.y,
         BALL_WIDTH,
         BALL_HEIGHT,
      };
      
      //----------------------------------------------------------------
      // Player Collision
      //----------------------------------------------------------------
      double cos_vx = (double)b.vx / (double)b.speed;
      double sin_vy = (double)b.vy / (double)b.speed;
      srand(time(NULL));

      // player 1 and ball
      if (SDL_HasIntersection(&p1.pos, &b_rect)) {
         b.x = p1.pos.x + p1.pos.w;
         b.speed *= BALL_ACCELERATE * ((rand()%10 == 0)? 2.5 : 1);
         b.vx = -1 * b.speed * cos_vx;
         if(keystates[SDL_SCANCODE_UP]) {
		   	b.vy = ((b.vy>0)? 1 : -1) * b.speed * sin_vy - p1.speed;
		   	double update_vx = pow(b.vx,2) ,update_vy = pow(b.vy,2);
			   b.speed = sqrt(update_vx + update_vy);
         }
         else {
            if(keystates[SDL_SCANCODE_DOWN]) {
               b.vy = ((b.vy>0)? -1 : 1) * b.speed * sin_vy + p1.speed;
               double update_vx = pow(b.vx,2) ,update_vy = pow(b.vy,2);
               b.speed = sqrt(update_vx + update_vy);
			   }
			   else
			      b.vy = ((b.vy>0)? 1 : -1) * b.speed * ((sin_vy>0)? sin_vy : -1 * sin_vy);
         }
      }
      // player 2 and ball
      else {
         if (SDL_HasIntersection(&p2.pos, &b_rect)) {
            b.x = p2.pos.x - BALL_WIDTH;
			   b.speed *= BALL_ACCELERATE * ((rand()%10 == 0)? 2.5 : 1);
			   b.vx = -1 * b.speed * cos_vx;
			   if(keystates[SDL_SCANCODE_W]) {
			      b.vy = ((b.vy>0)? 1 : -1) * b.speed * sin_vy - p2.speed;
			      double update_vx = pow(b.vx,2) ,update_vy = pow(b.vy,2);
			      b.speed = sqrt(update_vx + update_vy);
			   }
			   else {
			      if(keystates[SDL_SCANCODE_S]) {
                  b.vy = ((b.vy>0)? -1 : 1) * b.speed * sin_vy + p2.speed;
                  double update_vx = pow(b.vx,2) ,update_vy = pow(b.vy,2);
                  b.speed = sqrt(update_vx + update_vy);
			      }
			      else
			         b.vy = ((b.vy>0)? 1 : -1) * b.speed * ((sin_vy>0)? sin_vy : -1 * sin_vy);
			   }
         }
      }
      
      SDL_RenderClear(ren);

      SDL_RenderCopy(ren, squareTex, NULL, &p1.pos);
      SDL_RenderCopy(ren, squareTex, NULL, &p2.pos);
      
      //----------------------------------------------------------------
      // Pause, Continue or Leave the game
      //----------------------------------------------------------------
      int temp_pattern, temp_b_speed, temp_b_vx, temp_b_vy;
      if (keystates[SDL_SCANCODE_P]) {                                 //P for "Pause"
         if(pattern!=0) {
            temp_pattern = pattern;   pattern = 0;
            temp_b_speed = b.speed;   b.speed = 0;
            temp_b_vx = b.vx;         b.vx = 0;
            temp_b_vy = b.vy;         b.vy = 0;
         }
      }
      if (keystates[SDL_SCANCODE_C] && pattern == 0) {                 //C for "Continue"
         pattern = temp_pattern;
         b.speed = temp_b_speed;
         b.vx = temp_b_vx;
         b.vy = temp_b_vy;
      }
      if (keystates[SDL_SCANCODE_L]) {                                 //L for "Leave"
         pattern = 0;
         b.speed = 0;
         b.vx = 0;
         b.vy = 0;
         srand(time(NULL));
         p1_font_size = p2_font_size = 125;
         p1_Score_Color = p2_Score_Color = {static_cast<uint8_t>(rand()%256), static_cast<uint8_t>(rand()%200+rand()%56), static_cast<uint8_t>(rand()%156+rand()%100)};
      }
      
      //----------------------------------------------------------------
      // Draw the center line
      //----------------------------------------------------------------
      renderTexture(squareTex, ren, SCREEN_WIDTH/2 - CENTER_WIDTH/2, 0, CENTER_WIDTH, SCREEN_HEIGHT);
      
      //----------------------------------------------------------------
      // Draw the Ball
      //----------------------------------------------------------------
      renderTexture(squareTex, ren, b.x, b.y, BALL_WIDTH, BALL_HEIGHT);

      //----------------------------------------------------------------
      // Display the score
      //----------------------------------------------------------------
      sprintf(buffer, "%d", p1.score);
      SDL_Texture *p1score = renderText(buffer, "fonts/sample.ttf", p1_Score_Color, p1_font_size, ren);
      sprintf(buffer, "%d", p2.score);
      SDL_Texture *p2score = renderText(buffer, "fonts/sample.ttf", p2_Score_Color, p2_font_size, ren);

      int width;
      SDL_QueryTexture(p1score, NULL, NULL, &width, NULL);

      renderTexture(p1score, ren, SCREEN_WIDTH/2 - width - 10, 10);
      renderTexture(p2score, ren, SCREEN_WIDTH/2 + 10, 10);

      SDL_DestroyTexture(p1score);
      SDL_DestroyTexture(p2score);

      SDL_RenderPresent(ren);

      frame++;
   }

   SDL_DestroyTexture(squareTex);
   Cleanup(&ren, &win);
   return 0;
}

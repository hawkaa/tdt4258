/* global includes  */
#include <stdlib.h>
#include <stdio.h>

/* local includes */
#include "pong.h"
#include "screen.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define PLAYER_WIDTH 5
#define PLAYER_HEIGHT 50
#define PLAYER_SPEED 10
#define BALL_SIZE 5
#define BALL_SPEED 9
#define MARGIN_HORIZONTAL 10
#define MARGIN_VERTICAL 0

static screen_elem player_left, player_right, ball;
static int game_over = 1;

static void reset_ball(void)
{
	ball.dx = BALL_SPEED * 2 / 3;
	ball.dy = BALL_SPEED / 3;
	ball.x = SCREEN_WIDTH / 2;
	ball.y = SCREEN_HEIGHT  / 2;
}

void
pong_init(void)
{
	player_left.x = MARGIN_HORIZONTAL;
	player_left.y = MARGIN_VERTICAL;
	player_left.height = PLAYER_HEIGHT;
	player_left.width = PLAYER_WIDTH;
	player_left.c = 0xaaff;
	player_left.dx = 0;
	player_left.dy = 0;
	
	player_right.x = SCREEN_WIDTH - (MARGIN_HORIZONTAL + PLAYER_WIDTH);	
	player_right.y = MARGIN_VERTICAL;
	player_right.height = PLAYER_HEIGHT;
	player_right.width = PLAYER_WIDTH;
	player_right.c = 0x00ff;
	player_right.dx = 0;
	player_right.dy = 0;
	
	ball.height = BALL_SIZE;
	ball.width = BALL_SIZE;
	ball.c = 0xbb00;
	reset_ball();
	draw_element(NULL, &player_left);	
	draw_element(NULL, &player_right);
	draw_element(NULL, &ball);
}

static void 
move_player(screen_elem* player)
{
	/* does player move? */
	if(player->dy == 0)
		return;

	/* is on top */
	if (player->y <= MARGIN_VERTICAL && player->dy < 0)
		return;
		
	if(player->y >= (SCREEN_HEIGHT - MARGIN_VERTICAL) - PLAYER_HEIGHT && player->dy > 0)
		return;
	
	/* everything good */
	screen_elem old_player = *player;
	if (player->dy > 0)
	{
		player->y = (player->y + MARGIN_VERTICAL +  player->dy > SCREEN_HEIGHT - MARGIN_VERTICAL - PLAYER_HEIGHT ?
					SCREEN_HEIGHT - PLAYER_HEIGHT - MARGIN_VERTICAL: player->y + player->dy);
	}
	else if (player->dy < 0)
	{
		player->y = (player->y + player->dy < 0 ? 0 : player->y + player->dy);	
	}
	
	/* TODO update screen */
	draw_element_one_update(&old_player, player, 0);
	return;
}

static void
move_ball(screen_elem *ball)
{
	/* is ball moving? */
	if( ball->dy == 0 && ball->dx == 0)
		return;

	/* if ball is outside horizontal bounds */
	if( ball->x < -BALL_SIZE || ball->x > SCREEN_WIDTH + BALL_SIZE + BALL_SPEED)
	{
		reset_ball();
		return;
	}
	
	/* if ball is outside vertical bounds */
	if(	ball->y <= BALL_SPEED || ball->y > SCREEN_HEIGHT - (BALL_SIZE + BALL_SPEED))
		ball->dy = -ball->dy;

	/* ball should now be within one of the player areas */
	screen_elem old_ball = *ball;
	ball->x += ball->dx;
	ball->y += ball->dy;
	
	/* if ball is within left player area */
	if( ball->x  < MARGIN_HORIZONTAL + PLAYER_WIDTH && ball->x > MARGIN_HORIZONTAL)
	{
		if(ball->y <= player_left.y + PLAYER_HEIGHT && ball->y >= player_left.y)
		{
			if((ball->x - (player_left.x + PLAYER_WIDTH)  ) > ball->dx)// && 
				ball->x = player_left.x + PLAYER_WIDTH;
			ball->dx = -ball->dx;
		}
			
	} else
	/* if ball is within right player area */
	if( ball->x >= SCREEN_WIDTH - (MARGIN_HORIZONTAL + PLAYER_WIDTH + BALL_SIZE) && ball->x < SCREEN_WIDTH - MARGIN_HORIZONTAL)
	{
		if(ball->y <= player_right.y + PLAYER_HEIGHT && ball->y >= player_right.y)
		{
			if((ball->x + BALL_SIZE  - player_right.x) < ball->dx)
				ball->x = player_right.x - BALL_SIZE;
			ball->dx = -ball->dx;
		}
	}
	draw_element_one_update(&old_ball, ball, 0);
}

/*
 * The positions and the screen is updated at the same time
 * thus increasing the gamespeed when increasing the screen update frequency
 */
void 
timer_tick(void)
{
	move_player(&player_left);
	move_player(&player_right);
	move_ball(&ball);	
}

void
up_left_button_press(void)
{
	player_left.dy = -PLAYER_SPEED;
}

void
up_left_button_release(void)
{
	player_left.dy = 0;
}

void 
down_left_button_press(void)
{
	player_left.dy = PLAYER_SPEED;
}

void
down_left_button_release(void)
{
	player_left.dy = 0;
}

void 
up_right_button_press(void)
{
	player_right.dy = -PLAYER_SPEED;
}

void
up_right_button_release(void)
{
	player_right.dy = 0;
}

void 
down_right_button_press(void)
{
	player_right.dy = PLAYER_SPEED;
}

void
down_right_button_release(void)
{
	player_right.dy = 0;
}

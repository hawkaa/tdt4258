/* global includes  */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* local includes */
#include "pong.h"
#include "screen.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define PLAYER_WIDTH 10
#define PLAYER_HEIGHT 50
#define PLAYER_SPEED 10
#define BALL_SIZE 5
#define MARGIN_HORIZONTAL 10
#define MARGIN_VERTICAL 10

static screen_elem player_left, player_right, ball;
static int game_over = 1;

static void reset_ball(void)
{
	ball.dx = 0;
	ball.dy = 0;
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
	player_left.c = 0xff00;
	player_left.dx = 0;
	player_left.dy = 0;
	
	player_right.x = SCREEN_WIDTH - (MARGIN_HORIZONTAL + PLAYER_WIDTH);	
	player_right.y = MARGIN_VERTICAL;
	player_right.height = PLAYER_HEIGHT;
	player_right.width = PLAYER_WIDTH;
	player_right.c = 0xff00;
	player_right.dx = 0;
	player_right.dy = 0;
	
	ball.height = BALL_SIZE;
	ball.width = BALL_SIZE;
	ball.c = 0xff00;
	reset_ball();
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
	draw_element_y_overlap(&old_player, player);
	return;
}

static void
change_direction_if_intersection(screen_elem *ball, screen_elem *player)
{
	float angle;	
	/* if ball intersects with player */
	if(	ball->y <= player->y + PLAYER_HEIGHT &&
		ball->y >= player->y)
		angle = asin(PLAYER_WIDTH / ( (ball->y + BALL_SIZE / 2) + (player->y + PLAYER_HEIGHT/2)));
	
	return;
}

static void
move_ball(screen_elem *ball)
{
	/* is ball moving? */
	if( ball->dy == 0 && ball->dx == 0)
		return;

	/* if ball is outside horizontal bounds */
	if( ball->x < 0 || ball->x > SCREEN_WIDTH)
	{
		ball->dx = 0;
		ball->dy = 0;
		game_over = 1;
		return;
	}		
	
	/* if ball is outside vertical bounds */
	if(	ball->y < 0 || ball->y > SCREEN_HEIGHT)
		ball->dy = -ball->dy;
	
	ball->x += ball->dx;
	ball->y += ball->dy;

	/* if ball is within empty area */
	if( ball->x >= MARGIN_HORIZONTAL + PLAYER_WIDTH ||
		ball->x <= SCREEN_WIDTH - (MARGIN_HORIZONTAL + PLAYER_WIDTH))
		return;	
	
	/* ball should now be within one of the player areas */
	
	/* if ball is within left player area */
	if( ball->x <= MARGIN_HORIZONTAL + PLAYER_WIDTH && ball->x > MARGIN_HORIZONTAL)
	{
		change_direction_if_intersection(ball, &player_left);
		return;
	}
	/* if ball is within right player area */
	if( ball->x >= SCREEN_WIDTH - (MARGIN_HORIZONTAL + PLAYER_WIDTH) && ball->x < SCREEN_WIDTH - MARGIN_HORIZONTAL)
	{
		change_direction_if_intersection(ball, &player_right);	
		return;
	}
}

void 
timer_tick(void)
{
	if(game_over)
	{
		game_over = 0;
		reset_ball();
	}
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

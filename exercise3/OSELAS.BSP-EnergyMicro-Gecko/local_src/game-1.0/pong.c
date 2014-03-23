/* global includes  */
#include <stdlib.h>
#include <stdio.h>

/* local includes */
#include "pong.h"
#include "screen.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define PLAYER_WIDTH 10
#define PLAYER_HEIGHT 50
#define PLAYER_SPEED 10
#define BALL_SIZE 5
#define MARGIN 10

static screen_elem player_left, player_right, ball;
static int game_over = 1;

static void reset_ball_pos(void)
{
	ball.dx = 0;
	ball.dy = 0;
	ball.x = SCREEN_WIDTH / 2;
	ball.y = SCREEN_HEIGHT  / 2;
}

void
pong_init(void)
{
	player_left.x = MARGIN;
	player_left.y = MARGIN;
	player_left.height = PLAYER_HEIGHT;
	player_left.width = PLAYER_WIDTH;
	player_left.c = 0xffff;
	player_left.dx = 0;
	player_left.dy = 0;
	
	player_right.x = SCREEN_WIDTH - (MARGIN + PLAYER_WIDTH);	
	player_right.y = MARGIN;
	player_right.height = PLAYER_HEIGHT;
	player_right.width = PLAYER_WIDTH;
	player_right.c = 0xffff;
	player_right.dx = 0;
	player_right.dy = 0;
	
	ball.height = BALL_SIZE;
	ball.width = BALL_SIZE;
	ball.c = 0xff00;
	reset_ball_pos();
}

static void 
move_player(screen_elem* player)
{
	/* does player move? */
	if(player->dy == 0)
		return;

	/* is on top */
	if (player->y <= 0 && player->dy < 0)
		return;
		
	if(player->y >= SCREEN_HEIGHT - PLAYER_HEIGHT && player->dy > 0)
		return;
	
	/* everything good */
	screen_elem old_player = *player;
	if (player->dy > 0)
	{
		player->y = (player->y + player->dy > SCREEN_HEIGHT - PLAYER_HEIGHT ?
					SCREEN_HEIGHT - PLAYER_HEIGHT : player->y + player->dy);
	}
	else if (player->dy < 0)
	{
		player->y = (player->y + player->dy < 0 ? 0 : player->y + player->dy);	
	}
	
	/* TODO update screen */
	draw_element(&old_player, player);
	return;
}

static void
change_direction_if_intersection(screen_elem *ball, screen_elem *player)
{
	/* if ball intersects with player */
	if(	ball->y <= player->y + PLAYER_HEIGHT &&
		ball->y >= player->y)
		ball->dx = -ball->dx;	
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
		return;
	}		
	
	/* if ball is outside vertical bounds */
	if(	ball->y < 0 || ball->y > SCREEN_HEIGHT)
		ball->dy = -ball->dy;
	
	ball->x += ball->dx;
	ball->y += ball->dy;

	/* if ball is within empty area */
	if( ball->x >= MARGIN + PLAYER_WIDTH ||
		ball->x <= SCREEN_WIDTH - (MARGIN + PLAYER_WIDTH))
		return;	
	
	/* ball should now be within one of the player areas */
	
	/* if ball is within left player area */
	if( ball->x <= MARGIN + PLAYER_WIDTH && ball->x > MARGIN)
	{
		change_direction_if_intersection(ball, &player_left);
		return;
	}
	/* if ball is within right player area */
	if( ball->x >= SCREEN_WIDTH - (MARGIN + PLAYER_WIDTH) && ball->x < SCREEN_WIDTH - MARGIN)
	{
		change_direction_if_intersection(ball, &player_right);	
		return;
	}
}

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

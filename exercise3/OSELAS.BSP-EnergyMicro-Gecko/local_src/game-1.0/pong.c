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
#define PLAYER_SPEED 1
#define BALL_SIZE 5
#define MARGIN 10

static screen_elem player1, player2, ball;

void
pong_init(void)
{
	player1.x = MARGIN;
	player1.y = MARGIN;
	player1.height = PLAYER_HEIGHT;
	player1.width = PLAYER_WIDTH;
	player1.c = 0xffff;
	player1.dx = 0;
	player1.dy = 0;
	
	player2.x = SCREEN_WIDTH - (MARGIN + PLAYER_WIDTH);	
	player2.y = MARGIN;
	player2.height = PLAYER_HEIGHT;
	player2.width = PLAYER_WIDTH;
	player2.c = 0xffff;
	player2.dx = 0;
	player2.dy = 0;
	
	ball.x = SCREEN_WIDTH / 2;
	ball.y = SCREEN_HEIGHT  / 2;
	ball.height = BALL_SIZE;
	ball.width = BALL_SIZE;
	ball.dx = 0;
	ball.dy = 0;
	ball.c = 0xff00;
	
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
move_ball(screen_elem ball)
{
	
}

void 
timer_tick(void)
{
	move_player(&player1);
	move_player(&player2);
	
	if(	ball.x <= player1.x + PLAYER_WIDTH)
	{
		if(	ball.y <= player1.y + PLAYER_HEIGHT ||
			ball.y >= player1.y )
		{
			//TODO: endre vinkel
			ball.dx = -ball.dx;
		}
	}
	else if(ball.x >= player2.x)
	{
		if(	ball.y <= player2.y + PLAYER_HEIGHT ||
			ball.y >= player2.y )
		{
			//TODO: endre vinkel
			ball.dx = -ball.dx;
		}
	}
	else
	{
		ball.x += ball.dx;
		ball.y += ball.dy;
	}
}

void
up_left_button_press(void)
{
	player1.dy = -PLAYER_SPEED;
}

void
up_left_button_release(void)
{
	player1.dy = 0;
}

void 
down_left_button_press(void)
{
	player1.dy = PLAYER_SPEED;
}

void
down_left_button_release(void)
{
	player1.dy = 0;
}

void 
up_right_button_press(void)
{
	player2.dy = -PLAYER_SPEED;
}

void
up_right_button_release(void)
{
	player2.dy = 0;
}

void 
down_right_button_press(void)
{
	player2.dy = PLAYER_SPEED;
}

void
down_right_button_release(void)
{
	player2.dy = 0;
}

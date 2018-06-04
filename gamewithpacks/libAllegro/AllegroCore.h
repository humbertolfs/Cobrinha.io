#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define FPS2 60
#define maxPlayers 4
#define score 50

double startingTime;

ALLEGRO_DISPLAY *main_window;
ALLEGRO_EVENT_QUEUE *eventsQueue;

typedef struct direc
{
	int dir, pressed;
} direc;

typedef struct DADOS
{
    float x, y;
    int r, g, b, pontos;
    float orientacao[25];
} DADOS;

typedef struct sync
{
	int numPlayers;
} sync;

//Funções Centrais da Allegro
bool coreInit();
bool windowInit(int W, int H, char title[]);
void allegroEnd();

//Funções para Controle da quantidade de Frames por Segundo
void startTimer();
double getTimer();
void FPSLimit();


#endif

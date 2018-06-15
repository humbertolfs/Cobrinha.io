#ifndef GAME_H
#define GAME_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define FPS 60
#define maxPlayers 4

double startingTime;

ALLEGRO_DISPLAY *main_window;
ALLEGRO_EVENT_QUEUE *eventsQueue;

typedef struct direc
{
	int pressed, keyChanged, velocity, scoreAux, dead, scored, ij, win;
} direc;

typedef struct Snake
{
	float x;
	float y;
	float radius;
	char skin;
	short int orientacao[25];
	short int score;
	char name[19];
} Snake;

typedef struct corAux
{
	char cor;
	char login[6];
} corAux;

typedef struct sync
{
	short int numPlayers;
	short int disc[maxPlayers];
	short int eatedFoods[50];
	short int eFSize;
	short int win[maxPlayers];
} sync;

//Funções Centrais da Allegro
bool coreInit();
bool windowInit(int W, int H, char title[]);
void allegroEnd();
bool testIP(char ip[16]);

//Funções para Controle da quantidade de Frames por Segundo
void startTimer();
double getTimer();
void FPSLimit();


#endif

#include "AllegroCore.h"



bool coreInit()
{
    //modules and add-ons initialization
	if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return false;
    }

    eventsQueue = al_create_event_queue();
    if (!eventsQueue)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        return false;
    }

    return true;
}

bool windowInit(int W, int H, char title[])
{
    //window and event queue creation
    main_window = al_create_display(W, H);
    if (!main_window)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
    al_set_window_title(main_window, title);

    //registra janela na fila de eventos
    al_register_event_source(eventsQueue, al_get_display_event_source(main_window));

    return true;
}

void allegroEnd()
{
    al_destroy_display(main_window);
}

double getTimer()
{
    return al_get_time() - startingTime;
}

void startTimer()
{
    startingTime = al_get_time();
}

void FPSLimit()
{
	if (getTimer() < 1.0/FPS)
    {
        al_rest((1.0 / FPS) - getTimer());
    }
}

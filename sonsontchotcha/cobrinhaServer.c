#include "libSocket/server.h"
#include "libAllegro/AllegroCore.h"

Snake player[maxPlayers];
direc pack_server;
sync syncy;
corAux cory;

int worldWidth = 2000;
int worldHeight = 2000;
enum directions { UP, DOWN, LEFT, RIGHT };
int *orientation;
int moveSpeed = 2;
int count, z, idAtual, quantPlayers, scoreAux, l, alguem;;

int main()
{
    //Inicialização básica do Allegro, como vista no Client
    if(!coreInit())
        return -1;

    //Criação de uma janela 150px por 100px com o titulo "Server"
    if (!windowInit(150, 100, "Server"))
        return -1;

    struct msg_ret_t retorno;
    bool sair = false;

    orientation = (int *) malloc(1 * sizeof(int));

    *orientation = 0;

    syncy.numPlayers = 0;

    for(l = 0; l < maxPlayers; l++)
    {
        player[l].disc = 0;
    }
    
    for(l = 0; l < maxPlayers; l++)
    {
        player[l].seed = 3;
    }

    serverInit(maxPlayers);

    while(!sair)
    {
        startTimer();
        int id = acceptConnection();

        if(id != NO_CONNECTION)
        {
            printf("Alguem se Conectou com ID : %d\n", id);
            quantPlayers = id;
            if(quantPlayers == 0)
            {
                alguem = 1;

                player[id].x = 1000;
                player[id].y = 1000;
                player[id].score = 20;
                player[id].radius = 20;

                for(l = 0; l < 25; l++)
                {
                    player[id].orientacao[l] = -1;
                }

                for (count = 0; count < (player[id].score / 20) + 5; count++)
                {
                    player[id].orientacao[count] = 0;
                }

                sendMsgToClient(&id, sizeof(int), id);

                recvMsgFromClient(&cory, id, WAIT_FOR_IT);
                player[id].skin = cory.cor;

            } else if(quantPlayers > 0){

                    orientation = (int *) realloc(orientation, (quantPlayers+1) * sizeof(int));

                    orientation[quantPlayers] = 0;

                    player[id].x = 1000;
                    player[id].y = 1000;
                    player[id].score = 20;
                    player[id].radius = 20;

                    syncy.numPlayers = quantPlayers;

                    for(l = 0; l < 25; l++)
                    {
                        player[id].orientacao[l] = -1;
                    }

                    for (count = 0; count < (player[id].score / 20) + 5; count++)
                    {
                        player[id].orientacao[count] = 0;
                    }

                    sendMsgToClient(&id, sizeof(int), id);
                    
                    recvMsgFromClient(&cory, id, WAIT_FOR_IT);
                    player[id].skin = cory.cor;

                }            
        }
        
        if(alguem)
        {   
            retorno = recvMsg(&pack_server);
            if(retorno.status != NO_MESSAGE)
            {   
                id = retorno.client_id;
                
                if(pack_server.dead)
                {
                    disconnectClient(id);
                    player[id].disc = 1;
                }

                player[id].score = pack_server.scoreAux;
                if(player[id].score/20 == 1)
                {
                    player[id].orientacao[((player[id].score / 20) + 5) + 1] = 0; 
                }

                if (pack_server.pressed)
                {
                    switch (pack_server.dir)
                    {
                        case RIGHT:
                            orientation[id]--;
                            break;
                        case LEFT:
                            orientation[id]++;
                            break;
                    }

                    if (orientation[id] == 360)
                        orientation[id] = 0;
                    else if (orientation[id] == -1)
                        orientation[id] = 359;
                }

                for (count = (player[id].score / 20) + 5; count > 0; count--)
                    player[id].orientacao[count] = player[id].orientacao[count-1];

                player[id].orientacao[0] = orientation[id];

                player[id].x += cos(orientation[id] * 3.1415926 / 180.0) * moveSpeed;
                player[id].y -= sin(orientation[id] * 3.1415926 / 180.0) * moveSpeed;

                if (player[id].x > worldWidth)
                        player[id].x -= worldWidth;
                    else if (player[id].x < 0)
                        player[id].x += worldWidth;

                if (player[id].y > worldHeight)
                        player[id].y -= worldHeight;
                    else if (player[id].y < 0)
                        player[id].y += worldHeight;

                for(idAtual = 0; idAtual <= quantPlayers; idAtual++)
                {
                    if(!player[idAtual].disc)
                    {
                        if(idAtual != id)
                        {
                            for (count = (player[id].score / 20) + 5; count > 0; count--)
                                player[idAtual].orientacao[count] = player[idAtual].orientacao[count-1];

                            player[idAtual].orientacao[0] = orientation[idAtual];

                            (player[idAtual].x) += cos(orientation[idAtual] * 3.1415926 / 180.0) * moveSpeed;
                            (player[idAtual].y) -= sin(orientation[idAtual] * 3.1415926 / 180.0) * moveSpeed;

                            if (player[idAtual].x > worldWidth)
                                    player[idAtual].x -= worldWidth;
                                else if (player[idAtual].x < 0)
                                    player[idAtual].x += worldWidth;

                            if (player[idAtual].y > worldHeight)
                                    player[idAtual].y -= worldHeight;
                                else if (player[idAtual].y < 0)
                                    player[idAtual].y += worldHeight;
                        }
                    }
                }

            } else {

                for(idAtual = 0; idAtual <= quantPlayers; idAtual++)
                {
                    if(!player[idAtual].disc)
                    {
                        for (count = (player[id].score / 20) + 5; count > 0; count--)
                            player[idAtual].orientacao[count] = player[idAtual].orientacao[count-1];

                        player[idAtual].orientacao[0] = orientation[idAtual];

                        (player[idAtual].x) += cos(orientation[idAtual] * 3.1415926 / 180.0) * moveSpeed;
                        (player[idAtual].y) -= sin(orientation[idAtual] * 3.1415926 / 180.0) * moveSpeed;

                        if (player[idAtual].x > worldWidth)
                                player[idAtual].x -= worldWidth;
                            else if (player[idAtual].x < 0)
                                player[idAtual].x += worldWidth;

                        if (player[idAtual].y > worldHeight)
                                player[idAtual].y -= worldHeight;
                            else if (player[idAtual].y < 0)
                                player[idAtual].y += worldHeight;
                    }
                }
            }

            broadcast(&syncy, sizeof(sync));

            for(z = 0; z <= quantPlayers; z++)
            {
                if(!player[z].disc)
                {
                    broadcast(&player[z], sizeof(Snake));
                }
            }
            
        }
        
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        FPSLimit();
    }

    free(orientation);

    allegroEnd();

    return 0;
}
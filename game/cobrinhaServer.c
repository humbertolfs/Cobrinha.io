#include "libSocket/server.h"
#include "libAllegro/AllegroCore.h"

DADOS packet_server;
direc pack_server;
int l;

enum directions { UP, DOWN, LEFT, RIGHT };
int *orientation;
float *orientation_rad;
int moveSpeed = 2;
const float FPS = 60.0;
int count, z, idAtual;

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
    
    for(z = 0; z < maxPlayers; z++)
    {
        for(l=0; l<25; l++)
        {
            packet_server.orientacao[z][l] = -1;
        }
    }
    for(z = 0; z < maxPlayers; z++)
    {
        for (count = 0; count < (score / 2); count++)
        {
            packet_server.orientacao[z][count] = 0;
        }
    }

    orientation = (int *) malloc(1 * sizeof(int));
    orientation_rad = (float *) malloc(1 * sizeof(float));
    *orientation = 0;
    *orientation_rad = 0;

    serverInit(maxPlayers);

    while(!sair)
    {
        startTimer();
        int id = acceptConnection();

        if(id != NO_CONNECTION)
        {
            printf("Alguem se Conectou com ID %d\n", id);
            packet_server.quantPlayers = id;
            packet_server.x[id] = 2000;
            packet_server.y[id] = 2000;

            if(packet_server.quantPlayers > 0)
            {
                orientation = (int *) realloc(orientation, packet_server.quantPlayers * sizeof(int));
                orientation_rad = (float *) realloc(orientation_rad, packet_server.quantPlayers * sizeof(float));
                orientation[packet_server.quantPlayers] = 0;
                orientation_rad[packet_server.quantPlayers] = 0;
            }
            
            
            sendMsgToClient(&id, sizeof(int), id);
        }
            
        for(idAtual = 0; idAtual <= packet_server.quantPlayers; idAtual++)
        {   
            //printf("%i\n", idAtual);
            retorno = recvMsgFromClient(&pack_server, idAtual, WAIT_FOR_IT);
            if(retorno.status == MESSAGE_OK)
            {   
                id = retorno.client_id;
                
                printf("Recebi algo de %i/%i :%i %i\n",id, idAtual, pack_server.pressed, pack_server.dir);

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

                orientation_rad[id] = orientation[id] * 3.1415926 / 180.0;
                for (count = (score / 2) - 1; count > 0; count--)
                    packet_server.orientacao[id][count] = packet_server.orientacao[id][count-1];

                packet_server.orientacao[id][0] = orientation_rad[id];

                packet_server.x[id] += cos(orientation_rad[id]) * moveSpeed;
                packet_server.y[id] -= sin(orientation_rad[id]) * moveSpeed;

                packet_server.r[id] = 249;
                packet_server.g[id] = 38;
                packet_server.b[id] = 114;

                packet_server.pontos[id] = score;
            }
        }
        broadcast(&packet_server, sizeof(DADOS));

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        FPSLimit();
    }

    free(orientation);
    free(orientation_rad);

    allegroEnd();

    return 0;
}

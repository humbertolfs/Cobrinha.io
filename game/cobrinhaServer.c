#include "libSocket/server.h"
#include "libAllegro/AllegroCore.h"

DADOS packet_server;
direc pack_server;
int l;

float x = 2000, y = 2000;
enum directions { UP, DOWN, LEFT, RIGHT };
int orientation = 0;
float orientation_rad = 0;
float *orientation_array;
int dir = LEFT;
int pressed = 0;
int moveSpeed = 2;
const float FPS = 60.0;

int score = 50;

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
    int i=0;
    
    for(l=0; l<25; l++)
    {
        packet_server.orientacao[0][l] = -1;
        packet_server.orientacao[1][l] = -1;
        packet_server.orientacao[2][l] = -1;
        packet_server.orientacao[3][l] = -1;
    }

    orientation_array = (float *) malloc((score / 2) * sizeof(float));

    for (count = 0; count < (score / 2); count++)
    {
        orientation_array[count] = orientation_rad;
    }

    serverInit(4);

    while(!sair)
    {
        startTimer();
        int id = acceptConnection();

        if(id != NO_CONNECTION)
        {
            printf("Alguem se Conectou com ID %d\n", id);
            packet_server.quantPlayers = id;
            //printf("%i\n", quantPlayers);
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

                orientation_array = realloc(orientation_array, (score / 2) * sizeof(float));

                if (pack_server.pressed)
                {
                    switch (pack_server.dir)
                    {
                    case RIGHT:
                        orientation--;
                        break;
                    case LEFT:
                        orientation++;
                        break;
                    }

                    if (orientation == 360)
                        orientation = 0;
                    else if (orientation == -1)
                        orientation = 359;
                }

                orientation_rad = orientation * 3.1415926 / 180.0;
                for (count = (score / 2) - 1; count > 0; count--)
                    orientation_array[count] = orientation_array[count-1];

                orientation_array[0] = orientation_rad;

                x += cos(orientation_rad) * moveSpeed;
                y -= sin(orientation_rad) * moveSpeed;

                packet_server.x[id] = x;
                packet_server.y[id] = y;
                packet_server.r[id] = 249;
                packet_server.g[id] = 38;
                packet_server.b[id] = 114;
                for(z = 0; z < (score/2); z++)
                {
                    packet_server.orientacao[id][z] = orientation_array[z];
                }
                packet_server.pontos[id] = score;

                sendMsgToClient(&packet_server, sizeof(DADOS), id);
            }
        }

        
        
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        FPSLimit();
    }

    allegroEnd();

    return 0;
}
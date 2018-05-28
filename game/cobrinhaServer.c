#include "libSocket/server.h"
#include "libAllegro/AllegroCore.h"

DADOS packet_server;
int l;

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
        packet_server.orientacao[l] = -1;
    }

    serverInit(4);

    while(!sair)
    {
        startTimer();
        int id = acceptConnection();

        if(id != NO_CONNECTION)
            printf("Alguem se Conectou com ID %d\n", id);

        retorno = recvMsg(&packet_server);

        if(retorno.status == MESSAGE_OK)
        {
            id = retorno.client_id;
            
            for(i=0; i<4; i++)
            {   
                if(i != id)
                {
                    sendMsgToClient(&packet_server, sizeof(DADOS), i);   
                }
            }
        }

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        FPSLimit();
    }

    allegroEnd();

    return 0;
}
#include "libSocket/server.h"
#include "libAllegro/AllegroCore.h"


int main()
{
    //Inicialização básica do Allegro, como vista no Client
    if(!coreInit())
        return -1;

    //Criação de uma janela 150px por 100px com o titulo "Server"
    if (!windowInit(150, 100, "Server"))
        return -1;

    if(!inputInit())
        return -1;

    if(!fontInit())
        return -1;


    DADOS pacote_server;
    struct msg_ret_t retorno;
    bool sair = false;
    int i=0;


    serverInit(4);

    while(!sair)
    {
        startTimer();
        int id = acceptConnection();

        if(id != NO_CONNECTION)
            printf("Alguem se Conectou com ID %d\n", id);

        retorno = recvMsg(&pacote_server);

        if(retorno.status == MESSAGE_OK)
        {
            id = retorno.client_id;

            puts("Recebi a Seguinte mensagem:");
            printf("%s\n\n", pacote_server.mensagem);

            puts("Também recebi o seguinte valor:");
            printf("%d\n\n", pacote_server.valor);

            
            puts("Enviando respostas...");
            strcpy(pacote_server.mensagem, "Olá meu caro cliente!");
            pacote_server.valor = 0+i;
            sendMsgToClient(&pacote_server, sizeof(DADOS), id);

            ++i;
        }

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        FPSLimit();
    }

    allegroEnd();

    return 0;
}
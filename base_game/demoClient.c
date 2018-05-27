#include "libSocket/client.h"
#include "libAllegro/AllegroCore.h"

#define LARGURA 640
#define ALTURA 480

int main(int argc, char const *argv[])
{
    //A primeira coisa a ser feita é inicializar os módulos centrais. Caso algum deles falhe, o programa já para por aí.
    if (!coreInit())
        return -1;

    //Agora vamos criar a nossa janela. Largura e Altura em pixels, o título é uma string.
    if (!windowInit(LARGURA, ALTURA, "TITULO DA JANELA :3"))
        return -1;

    //Agora inicializamos nosso teclado e mouse, para que a janela responda às entradas deles
    if (!inputInit())
        return -1;

    //Agora inicializamos nossas fontes
    if(!fontInit())
        return -1;

    //E por fim todas as imagens que vamos utilizar no programa.
    if(!loadGraphics())
        return -1;

    //para sair do jogo
    bool demo = true;

    //Variáveis para controlar opção escolhida e laços.
    int option = 0;
    int i, j;

    //Criamos uma nova estrutura que será enviada e recebida do servidor.
    DADOS pacote;
    //Carregamos uma mensagem e um valor nesse pacote.
    char msg[]={"Oi Servidor!"};
    strcpy(pacote.mensagem, msg);
    pacote.valor = 42;

    //Esse IP irá nos conectar a "nós mesmos", apenas para efeito de testes.
    char ServerIP[30]={"127.0.0.1"};
    connectToServer(ServerIP);

    //Texto padrão para demonstração.
    char sampleText[13][40] = {
                               {"Lorem ipsum dolor sit amet,"},
                               {"consectetur adipiscing elit,"},
                               {"sed do eiusmod tempor incididunt"},
                               {"ut labore et dolore magna aliqua."},
                               {"Ut enim ad minim veniam, quis"},
                               {"nostrud exercitation ullamco"},
                               {"laboris nisi ut aliquip ex ea"},
                               {"commodo consequat. Duis aute"},
                               {"irure dolor in reprehenderit"},
                               {"in voluptate velit esse cillum"},
                               {"dolore eu fugiat nulla pariatur."},
                               {"Excepteur sint occaecat cupidatat"},
                               {"non proident, sunt in culpa qui."}
                              };

    while(demo)
    {
        //Iniciamos a contagem do tempo quando o laço se inicia
        startTimer();

        //Enquanto a fila de eventos não estiver vazia
        while(!al_is_event_queue_empty(eventsQueue))
        {
            //Cria um novo evento para armazenar as entradas recebidas
            ALLEGRO_EVENT eventoDemo;
            //Espera por um evento
            al_wait_for_event(eventsQueue, &eventoDemo);

            //Se o tipo do evento for uma tecla sendo apertada
            if(eventoDemo.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                //Vamos ver qual tecla foi...
                switch(eventoDemo.keyboard.keycode)
                {
                    //Apertou seta para a direita
                    case ALLEGRO_KEY_RIGHT:
                        option = (option+1)%4;
                        break;

                    //Apertou seta para a esquerda
                    case ALLEGRO_KEY_LEFT:
                        option = (option+3)%4;
                        break;
                }
            }

            //Evento especial caso tenha clicado no X da janela
            if(eventoDemo.type == ALLEGRO_EVENT_DISPLAY_CLOSE) demo = false;
                
        }
        
        //Agora vamos tratar as opções, cada vez que uma seta for apertada, devemos circular pelas janelas...
        switch(option)
        {
            case 0:
                //essa janela contem um exemplo de imagem
                al_draw_text(ubuntu, al_map_rgb(255, 255, 255), LARGURA/2, 10, ALLEGRO_ALIGN_CENTRE, "Exemplo: Imagem1");
                al_draw_bitmap(hscs, 50, 50, 0);
                break;

            case 1:
                //essa janela contem outro exemplo de imagem
                al_draw_text(ubuntu, al_map_rgb(0, 0, 255), LARGURA/2, 10, ALLEGRO_ALIGN_CENTRE, "Exemplo: Imagem2");
                al_draw_bitmap(objects, 140, 60, 0);
                break;

            case 2:
                //essa janela mostra o uso de fontes
                al_draw_text(ubuntu, al_map_rgb(255, 255, 255), LARGURA/2, 10, ALLEGRO_ALIGN_CENTRE, "Exemplo de Fonte:");
                
                for(i=0; i<13; ++i)
                    al_draw_text(start, al_map_rgb(255, 255, 0), LARGURA/2, 60+(i*32), ALLEGRO_ALIGN_CENTRE, sampleText[i]);

                break;

            case 3:
                //agora vamos testar nossa comunicação
                //apenas mostrando o titulo
                al_draw_text(ubuntu, al_map_rgb(255, 255, 255), LARGURA/2, 10, ALLEGRO_ALIGN_CENTRE, "Enviando Mensagem para Servidor:");

                //colocando a mensagem no pacote e o valor
                strcpy(pacote.mensagem, "Oi Servidor!");
                pacote.valor = 42;
                //mostrando na tela
                al_draw_text(ubuntu, al_map_rgb(255, 255, 0), LARGURA/2, 100, ALLEGRO_ALIGN_CENTRE, pacote.mensagem);
                al_draw_textf(ubuntu, al_map_rgb(255, 255, 0), LARGURA/2, 132, ALLEGRO_ALIGN_CENTRE, "Valor: %d", pacote.valor);

                al_draw_text(ubuntu, al_map_rgb(255, 0, 0), LARGURA/2, 220, ALLEGRO_ALIGN_CENTRE, "...");
                
                //enviando a mensagem
                sendMsgToServer(&pacote, sizeof(pacote));

                //recebendo a mensagem
                //experimentem trocar WAIT_FOR_IT por DONT_WAIT...
                recvMsgFromServer(&pacote, WAIT_FOR_IT);

                //exibindo a mensagem recebida
                al_draw_text(ubuntu, al_map_rgb(255, 255, 255), LARGURA/2, 300, ALLEGRO_ALIGN_CENTRE, pacote.mensagem);
                al_draw_textf(ubuntu, al_map_rgb(255, 255, 255), LARGURA/2, 332, ALLEGRO_ALIGN_CENTRE, "Valor: %d", pacote.valor);

                break;
        }



        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        FPSLimit();
    }

    return 0;
}

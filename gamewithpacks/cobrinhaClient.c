#include "libAllegro/AllegroCore.h"
#include "libSocket/client.h"

bool initialize();
void cameraUpdate(int *cameraPosition, int x, int y);
void redrawBackground();
void drawCircle(float pos_x, float pos_y, float raio, int r, int g, int b);
void drawChar(float pos_x, float pos_y, float raio, int r, int g, int b, float *orientacao_array, int pontuacao);
void drawEnemy(float pos_x, float pos_y, float raio, int r, int g, int b, float *orientacao_array, int pontuacao_inimigo, int pontuacao_principal);

enum directions { UP, DOWN, LEFT, RIGHT };
int dir = LEFT;
int pressed = 0;
int moveSpeed = 2;
const float FPS = 60.0;
int myid, enemyid, l, quantPlayers = 0, z, scoreAtual, x, y;

int worldWidth = 4950;
int worldHeight = 4350;
int screenWidth = 1280;
int screenHeight = 720;
int cameraPosition[2] = { 0, 0 };

DADOS packet;
direc pack;

ALLEGRO_KEYBOARD_STATE keyState;
ALLEGRO_TRANSFORM *camera;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP  *background;
ALLEGRO_TIMER *timer;

int main(void)
{
	if (!initialize())
	{
		return -1;
	}

	bool done = false;
	bool draw = false;

	char ServerIP[30]={"127.0.0.1"};
    connectToServer(ServerIP);

    recvMsgFromServer(&myid, WAIT_FOR_IT);

    printf("%i\n", myid);

	al_start_timer(timer);

	while (!done)
	{
		// EVENTOS
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);

		if (events.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (events.keyboard.keycode)
			{
			case ALLEGRO_KEY_RIGHT:
				dir = RIGHT;
				pressed = 1;
				pack.dir = RIGHT;
				pack.pressed = 1;
				break;
			case ALLEGRO_KEY_LEFT:
				dir = LEFT;
				pressed = 2;
				pack.dir = LEFT;
				pack.pressed = 2;
				break;
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			}
		}
		
		if (events.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (events.keyboard.keycode)
			{
			case ALLEGRO_KEY_RIGHT:
				if (pressed == 1)
					pressed = 0;
					pack.pressed = 0;
				break;
			case ALLEGRO_KEY_LEFT:
				if (pressed == 2)
					pressed = 0;
					pack.pressed = 0;
				break;
			}
		}

		if (events.type == ALLEGRO_EVENT_TIMER)
		{
			sendMsgToServer(&pack, sizeof(direc));

			draw = true;
		}

		if (draw)
		{	
			//recvMsgFromServer(&packet, DONT_WAIT);

			//printf("%i %i\n", packet.x[myid], packet.y[myid]);
			
			// ATUALIZAÇÃO DA IMAGEM
			cameraUpdate(cameraPosition, x, y);
			al_identity_transform(&camera);
			al_translate_transform(&camera, -cameraPosition[0], -cameraPosition[1]);
			al_use_transform(&camera);

			redrawBackground();

			for(z = 0; z <= quantPlayers; z++)
	        {	
	        	printf("ID Atual: %i\n", z);
	        	recvMsgFromServer(&packet, DONT_WAIT);
	        	if(z == myid || quantPlayers == 0)
	        	{	
	        		printf("entrei no meu x: %f y: %f\n", packet.x, packet.y);
	        		drawChar(packet.x, packet.y, 20, packet.r, packet.g, packet.b, packet.orientacao, packet.pontos);
	        		scoreAtual = packet.pontos;
	        		x = packet.x;
	        		y = packet.y;
	        	} else if(z != myid && quantPlayers > 0){
	        		printf("entrei no inimigo x: %f y: %f\n", packet.x, packet.y);
	        		drawEnemy(packet.x, packet.y, 20, packet.r, packet.g, packet.b, packet.orientacao, packet.pontos, scoreAtual);
	        	}
	        	//quantPlayers = packet.quantPlayers;
	        	printf("Jogadores : %i\n", quantPlayers);
	        }

			al_flip_display();

			draw = false;
		}
	}

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);

	return 0;
}

bool initialize()
{

	if (!al_init())
	{
		fprintf(stderr, "Falha ao inicializar a biblioteca Allegro.\n");
		return false;
	}

	if (!al_init_primitives_addon())
	{
		fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
		return false;
	}

	if (!al_install_keyboard())
	{
		fprintf(stderr, "Falha ao inicializar teclado.\n");
		return false;
	}

	if (!al_init_image_addon())
	{
		fprintf(stderr, "Falha ao inicializar add-on de imagens.\n");
		return false;
	}

	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 6, ALLEGRO_SUGGEST);	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	al_hold_bitmap_drawing(true);
	display = al_create_display(screenWidth, screenHeight);

	if (!display)
	{
		fprintf(stderr, "Falha ao criar janela.\n");
		return false;
	}

	// configura a fila de eventos
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	// configura o timer
	timer = al_create_timer(1.0 / FPS);
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_clear_to_color(al_map_rgb(30, 30, 30));

	background = al_load_bitmap("Resources/bg_tile.png");

	if (!background)
	{
		fprintf(stderr, "failed to load background bitmap!\n");
		return -1;
	}

	al_set_window_title(display, "Slither.io");

	return true;
}

void redrawBackground()
{
	al_clear_to_color(al_map_rgb(13, 17, 22));
	al_draw_bitmap_region(background, cameraPosition[0], cameraPosition[1], screenWidth, screenHeight, cameraPosition[0], cameraPosition[1], 0);
}

void cameraUpdate(int *cameraPosition, int x, int y)
{
	cameraPosition[0] = -(screenWidth / 2) + x;
	cameraPosition[1] = -(screenHeight / 2) + y;

	if (cameraPosition[0] < 0) cameraPosition[0] = 0;
	if (cameraPosition[1] < 0) cameraPosition[1] = 0;
	if (cameraPosition[0] > worldWidth - screenWidth) cameraPosition[0] = worldWidth - screenWidth;
	if (cameraPosition[1] > worldHeight - screenHeight) cameraPosition[1] = worldHeight - screenHeight;
}

void drawCircle(float pos_x, float pos_y, float raio, int r, int g, int b)
{
	int i;
	for (i = 5; i > 0; i--)
	{
		float t = (i - 1) / 5;
		t = sqrt(t);
		t = 1 - cos(t*3.1415926 / 2);
		al_draw_filled_circle(pos_x, pos_y, raio + i * 0.2, al_map_rgba(r, g, b, (1 - t) * (255 / i)));
	}
}

void drawChar(float pos_x, float pos_y, float raio, int r, int g, int b, float *orientacao_array, int pontuacao)
{
	int i, tamanho = (int)pontuacao / 2;
	float k;
	for (i = tamanho - 1; i >= 0; i--)
	{
		if(orientacao_array[i] != -1)
		{
			float orientacao_rad = orientacao_array[i];
			// círculos
			k = (float)(2 * (tamanho - 1) - i) / (2 * (tamanho - 1));
			drawCircle(pos_x - (cos(orientacao_rad) * i * raio), pos_y + (sin(orientacao_rad) * i * raio), raio, (int)(k*r), (int)(k*g), (int)(k*b));

			if (!i)
			{
				// cálculos
				float xcos = cos(orientacao_rad) * ((raio)-(0.1 * 2 * raio) - (raio / 4));
				float xsin = sin(orientacao_rad) * ((raio)-(0.2 * 2 * raio) - (raio / 4));
				float ycos = cos(orientacao_rad) * ((raio)-(0.2 * 2 * raio) - (raio / 4));
				float ysin = sin(orientacao_rad) * -((raio)-(0.1 * 2 * raio) - (raio / 4));

				float offset_x1 = xcos + xsin;
				float offset_x2 = xcos - xsin;
				float offset_y1 = ycos + ysin;
				float offset_y2 = ysin - ycos;

				// olhos (branco)
				drawCircle(pos_x + offset_x1, pos_y + offset_y1, (float)raio / 4, 255, 255, 255);
				drawCircle(pos_x + offset_x2, pos_y + offset_y2, (float)raio / 4, 255, 255, 255);

				// olhos (preto)
				drawCircle(pos_x + offset_x1 - (raio / 8), pos_y + offset_y1, (float)raio / 8, 0, 0, 0);
				drawCircle(pos_x + offset_x2 - (raio / 8), pos_y + offset_y2, (float)raio / 8, 0, 0, 0);
			}
		}
	}
}

void drawEnemy(float pos_x, float pos_y, float raio, int r, int g, int b, float *orientacao_array, int pontuacao_inimigo, int pontuacao_principal)
{
	float raio_scaled = raio * pontuacao_inimigo / pontuacao_principal;
	int i, tamanho = (int)pontuacao_inimigo / 2;
	float k;
	for (i = tamanho - 1; i >= 0; i--)
	{	
		if(orientacao_array[i] != -1)
		{
			float orientacao_rad = orientacao_array[i];
			// círculos
			k = (float)(2 * (tamanho - 1) - i) / (2 * (tamanho - 1));
			drawCircle(pos_x - (cos(orientacao_rad) * i * raio_scaled), pos_y + (sin(orientacao_rad) * i * raio_scaled), raio_scaled, (int)(k*r), (int)(k*g), (int)(k*b));

			if (!i)
			{
				// cálculos
				float xcos = cos(orientacao_rad) * ((raio_scaled)-(0.1 * 2 * raio_scaled) - (raio_scaled / 4));
				float xsin = sin(orientacao_rad) * ((raio_scaled)-(0.2 * 2 * raio_scaled) - (raio_scaled / 4));
				float ycos = cos(orientacao_rad) * ((raio_scaled)-(0.2 * 2 * raio_scaled) - (raio_scaled / 4));
				float ysin = sin(orientacao_rad) * -((raio_scaled)-(0.1 * 2 * raio_scaled) - (raio_scaled / 4));

				float offset_x1 = xcos + xsin;
				float offset_x2 = xcos - xsin;
				float offset_y1 = ycos + ysin;
				float offset_y2 = ysin - ycos;

				// olhos (branco)
				drawCircle(pos_x + offset_x1, pos_y + offset_y1, (float)raio_scaled / 4, 255, 255, 255);
				drawCircle(pos_x + offset_x2, pos_y + offset_y2, (float)raio_scaled / 4, 255, 255, 255);

				// olhos (preto)
				drawCircle(pos_x + offset_x1 - (raio_scaled / 8), pos_y + offset_y1, (float)raio_scaled / 8, 0, 0, 0);
				drawCircle(pos_x + offset_x2 - (raio_scaled / 8), pos_y + offset_y2, (float)raio_scaled / 8, 0, 0, 0);
			}
		}
	}
}
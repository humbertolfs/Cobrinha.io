#include "libAllegro/AllegroCore.h"
#include "libSocket/client.h"

// Funções do jogo
bool initialize();
void cameraUpdate(int *cameraPosition, int x, int y);
void redrawBackground();
void getSkinRGB(char skin, int ret[3]);
void drawCircle(float pos_x, float pos_y, float raio, int r, int g, int b, int a);
void drawChar(Snake character);
void drawEnemy(Snake enemy);
void drawFood();

//structs
Snake player[maxPlayers];
direc pack;
sync syncy;
corAux cory;

// Variáveis de controle
int *eatedFoodsX = NULL;
int *eatedFoodsY = NULL;
int eFSize = 0;
int cameraPosition[2] = { 0, 0 };
int worldWidth = 2000;
int worldHeight = 2000;
int screenWidth = 1280;
int screenHeight = 720;
enum directions { UP, DOWN, LEFT, RIGHT };
float moveSpeed = 2;
char name[6];
char ip[16];
bool scored = false;
bool dead = false;
int myid, enemyid, l, z, quantPlayers, retorno, tempo, myscore;


// Variáveis do Allegro
ALLEGRO_KEYBOARD_STATE keyState;
ALLEGRO_TRANSFORM *camera;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP  *background;
ALLEGRO_TIMER *timer;
ALLEGRO_FONT *raleway16;
ALLEGRO_FONT *raleway48;


int main(void)
{
	if (!initialize())
	{
		return -1;
	}

	for(l = 0; l < maxPlayers; l++)
    {
        syncy.disc[l] = 0;
    }

	pack.dead = 0;
	bool mainLoop = true;
	bool escPlay = false;
	bool tbSelected = false;
	bool tb2Selected = false;
	char selectedSkin = 1, totalSkins = 5, nextSkin = 2, previousSkin = 5;

	// ###############################################################
	// #						Loop do jogo						 #
	// ###############################################################

	while (mainLoop)
	{
		// "Reset" das variáveis toda vez que volta pro começo do loop (MUDAR P FALSE E TRUE, RESPECTIVAMENTE DPS DOS TESTES)
		bool mainScreen = true;
		bool playScreen = false;

		bool MO = false;

		cameraPosition[0] = 0;
		cameraPosition[1] = 0;

		al_identity_transform(&camera);
		al_translate_transform(&camera, 0, 0);
		al_use_transform(&camera);

		bool playMO = false;
		bool leftMO = false;
		bool rightMO = false;

		// ###############################################################
		// #					Loop do menu inicial					 #
		// ###############################################################

		while (mainScreen)
		{
			// Exibe o cursor do mouse
			al_show_mouse_cursor(display);

			// A partir daqui se carrega as imagens que vão ser exibidas no menu inicial
			ALLEGRO_BITMAP  *logo, *textbox, *play, *left, *right, *skin1, *skin2, *skin3;
			char path[80], extra[7];

			if (selectedSkin - 1 < 1)
				previousSkin = totalSkins;
			else
				previousSkin = selectedSkin - 1;

			if (selectedSkin + 1 > totalSkins)
				nextSkin = 1;
			else
				nextSkin = selectedSkin + 1;


			logo = al_load_bitmap("res/logo.png");
			textbox = al_load_bitmap("res/textbox.png");
			play = al_load_bitmap("res/play.png");
			left = al_load_bitmap("res/left.png");
			right = al_load_bitmap("res/right.png");

			strcpy(path, "res/skins/");

			if (previousSkin >= 10)
			{
				extra[0] = (previousSkin / 10) + '0';
				extra[1] = (previousSkin % 10) + '0';
				extra[2] = '.';
				extra[3] = 'p';
				extra[4] = 'n';
				extra[5] = 'g';
				extra[6] = '\0';
			}
			else
			{
				extra[0] = previousSkin + '0';
				extra[1] = '.';
				extra[2] = 'p';
				extra[3] = 'n';
				extra[4] = 'g';
				extra[5] = '\0';
			}

			strcat(path, extra);
			skin1 = al_load_bitmap(path);

			strcpy(path, "res/skins/");

			if (selectedSkin >= 10)
			{
				extra[0] = (selectedSkin / 10) + '0';
				extra[1] = (selectedSkin % 10) + '0';
				extra[2] = '.';
				extra[3] = 'p';
				extra[4] = 'n';
				extra[5] = 'g';
				extra[6] = '\0';
			}
			else
			{
				extra[0] = selectedSkin + '0';
				extra[1] = '.';
				extra[2] = 'p';
				extra[3] = 'n';
				extra[4] = 'g';
				extra[5] = '\0';
			}

			strcat(path, extra);
			skin2 = al_load_bitmap(path);

			strcpy(path, "res/skins/");

			if (nextSkin >= 10)
			{
				extra[0] = (nextSkin / 10) + '0';
				extra[1] = (nextSkin % 10) + '0';
				extra[2] = '.';
				extra[3] = 'p';
				extra[4] = 'n';
				extra[5] = 'g';
				extra[6] = '\0';
			}
			else
			{
				extra[0] = nextSkin + '0';
				extra[1] = '.';
				extra[2] = 'p';
				extra[3] = 'n';
				extra[4] = 'g';
				extra[5] = '\0';
			}

			strcat(path, extra);
			skin3 = al_load_bitmap(path);

			// A partir daqui se carrega as imagens (e se calcula a posição que elas serão exibidas)

			float tbpos[2] = { (screenWidth / 2) - al_get_bitmap_width(textbox) - 15, screenHeight*0.40 };
			float tb2pos[2] = { (screenWidth / 2) + 15, screenHeight*0.40 };
			float playpos[2] = { (screenWidth / 2) - al_get_bitmap_width(play) / 2 , screenHeight*0.55 };
			float leftpos[2] = { (screenWidth / 2) - (3 * al_get_bitmap_width(skin1) / 2) - 50 , al_get_bitmap_height(right) / 2 + screenHeight * 0.85 };
			float rightpos[2] = { (screenWidth / 2) + 3 * al_get_bitmap_width(skin3) / 2 + 20 , al_get_bitmap_height(right) / 2 + screenHeight * 0.85 };


			al_clear_to_color(al_map_rgb(22, 28, 34));

			al_draw_tinted_bitmap_region(background, al_map_rgba_f(1, 1, 1, 0.2), cameraPosition[0], cameraPosition[1], screenWidth, screenHeight, 0, 0, 0);
			al_draw_bitmap(logo, (screenWidth / 2) - al_get_bitmap_width(logo) / 2, screenHeight*0.20, 0);

			if (tbSelected)
			{
				al_draw_bitmap(textbox, tbpos[0], tbpos[1], 0);
			}
			else
				al_draw_tinted_bitmap(textbox, al_map_rgba_f(1, 1, 1, 0.5f), tbpos[0], tbpos[1], 0);

			if (tb2Selected)
			{
				al_draw_bitmap(textbox, tb2pos[0], tb2pos[1], 0);
			}
			else
				al_draw_tinted_bitmap(textbox, al_map_rgba_f(1, 1, 1, 0.5f), tb2pos[0], tb2pos[1], 0);

			if (playMO)
				al_draw_bitmap(play, playpos[0], playpos[1], 0);
			else
				al_draw_tinted_bitmap(play, al_map_rgba_f(1, 1, 1, 0.5f), playpos[0], playpos[1], 0);

			if (leftMO)
				al_draw_bitmap(left, leftpos[0], leftpos[1], 0);
			else
				al_draw_tinted_bitmap(left, al_map_rgba_f(1, 1, 1, 0.5f), leftpos[0], leftpos[1], 0);

			if (rightMO)
				al_draw_bitmap(right, rightpos[0], rightpos[1], 0);
			else
				al_draw_tinted_bitmap(right, al_map_rgba_f(1, 1, 1, 0.5f), rightpos[0], rightpos[1], 0);

			al_draw_tinted_bitmap(skin1, al_map_rgba_f(1, 1, 1, 0.2), (screenWidth / 2) - (3 * al_get_bitmap_width(skin1) / 2) - 10, screenHeight*0.85, 0);
			al_draw_bitmap(skin2, (screenWidth / 2) - al_get_bitmap_width(skin2) / 2, screenHeight*0.85, 0);
			al_draw_tinted_bitmap(skin3, al_map_rgba_f(1, 1, 1, 0.2), (screenWidth / 2) + al_get_bitmap_width(skin3) / 2 + 10, screenHeight*0.85, 0);
			ALLEGRO_FONT *raleway36 = al_load_font("res/ttf/Raleway-Light.ttf", 36, 0);
			al_draw_text(raleway36, al_map_rgb(255, 255, 255), screenWidth / 2, screenHeight*0.75, ALLEGRO_ALIGN_CENTRE, "escolha sua skin");

			if (!strlen(name))
				al_draw_text(raleway36, al_map_rgba_f(1, 1, 1, 0.2), tbpos[0] + al_get_bitmap_width(textbox) / 2, tbpos[1] + 5, ALLEGRO_ALIGN_CENTRE, "digite o login");

			if (!strlen(ip))
				al_draw_text(raleway36, al_map_rgba_f(1, 1, 1, 0.2), tb2pos[0] + al_get_bitmap_width(textbox) / 2, tb2pos[1] + 5, ALLEGRO_ALIGN_CENTRE, "digite o IP");

			al_draw_text(raleway36, al_map_rgb(255, 255, 255), tbpos[0] + al_get_bitmap_width(textbox) / 2, tbpos[1] + 5, ALLEGRO_ALIGN_CENTRE, name);
			al_draw_text(raleway36, al_map_rgb(255, 255, 255), tb2pos[0] + al_get_bitmap_width(textbox) / 2, tb2pos[1] + 5, ALLEGRO_ALIGN_CENTRE, ip);
			al_flip_display();

			// Loop para deixar o programa esperando acontecer algum evento sem atualizar os gráficos
			while (1)
			{
				ALLEGRO_EVENT events;
				al_wait_for_event(event_queue, &events);

				// Eventos de tecla
				if (events.type == ALLEGRO_EVENT_KEY_CHAR)
				{
					if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE && !escPlay)
					{
						mainLoop = false;
						mainScreen = false;
						break;
					}
					else
					{
						const int inputChar = events.keyboard.unichar;
						int sizeName = strlen(name);
						int sizeIp = strlen(ip);
						if (tbSelected)
						{
							if (events.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
							{
								if (sizeName > 0)
								{
									name[sizeName - 1] = '\0';
								}
							}
							else if ((
								(inputChar >= 48 && inputChar <= 57) // número
								|| (inputChar >= 65 && inputChar <= 90) // letra maiúscula
								|| (inputChar >= 97 && inputChar <= 122) //letra minúscula
								|| (inputChar == 95 || inputChar == 46) // underline e ponto
								) && sizeName < 5) {
								name[sizeName] = inputChar;
								name[sizeName + 1] = '\0';
							}
							break;
						}
						else if (tb2Selected)
						{
							if (events.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
							{
								if (sizeIp > 0)
								{
									ip[sizeIp - 1] = '\0';
								}
							}
							else if ((
								(inputChar >= 48 && inputChar <= 57) // número
								|| (inputChar == 46) // ponto
								) && sizeIp < 15) {
								ip[sizeIp] = inputChar;
								ip[sizeIp + 1] = '\0';
							}
							break;
						}
					}
				}

				// Evento para ESC durante o jogo não bugar com o ESC durante o menu inicial
				if (events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					escPlay = false;
				}

				// Eventos de mover o mouse
				if (events.type == ALLEGRO_EVENT_MOUSE_AXES)
				{
					if (events.mouse.x >= tbpos[0] && events.mouse.x <= tbpos[0] + al_get_bitmap_width(textbox) && events.mouse.y >= tbpos[1] && events.mouse.y <= tbpos[1] + al_get_bitmap_height(textbox))
					{
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT);
					}
					else if (events.mouse.x >= tb2pos[0] && events.mouse.x <= tb2pos[0] + al_get_bitmap_width(textbox) && events.mouse.y >= tb2pos[1] && events.mouse.y <= tb2pos[1] + al_get_bitmap_height(textbox))
					{
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT);
					}
					else if (events.mouse.x >= playpos[0] && events.mouse.x <= playpos[0] + al_get_bitmap_width(play) && events.mouse.y >= playpos[1] && events.mouse.y <= playpos[1] + al_get_bitmap_height(play))
					{
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
						playMO = true;

						if (!MO)
						{
							MO = true;
							break;
						}
					}
					else if (events.mouse.x >= leftpos[0] && events.mouse.x <= leftpos[0] + al_get_bitmap_width(left) && events.mouse.y >= leftpos[1] && events.mouse.y <= leftpos[1] + al_get_bitmap_height(left))
					{
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
						leftMO = true;

						if (!MO)
						{
							MO = true;
							break;
						}
					}
					else if (events.mouse.x >= rightpos[0] && events.mouse.x <= rightpos[0] + al_get_bitmap_width(right) && events.mouse.y >= rightpos[1] && events.mouse.y <= rightpos[1] + al_get_bitmap_height(right))
					{
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
						rightMO = true;

						if (!MO)
						{
							MO = true;
							break;
						}
					}
					else
					{
						al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
						leftMO = false;
						rightMO = false;
						playMO = false;

						if (MO)
						{
							MO = false;
							break;
						}
					}
				}

				// Eventos de clique do mouse

				if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
				{
					if (events.mouse.button & 1)
					{
						if (events.mouse.x >= tbpos[0] && events.mouse.x <= tbpos[0] + al_get_bitmap_width(textbox) && events.mouse.y >= tbpos[1] && events.mouse.y <= tbpos[1] + al_get_bitmap_height(textbox))
						{
							tbSelected = true;
							tb2Selected = false;
							break;
						}
						else if (events.mouse.x >= tb2pos[0] && events.mouse.x <= tb2pos[0] + al_get_bitmap_width(textbox) && events.mouse.y >= tb2pos[1] && events.mouse.y <= tb2pos[1] + al_get_bitmap_height(textbox))
						{
							tb2Selected = true;
							tbSelected = false;
							break;
						}
						else
						{
							bool changed = false;

							if (tb2Selected || tbSelected)
								changed = true;

							tb2Selected = false;
							tbSelected = false;

							if (events.mouse.x >= leftpos[0] && events.mouse.x <= leftpos[0] + al_get_bitmap_width(left) && events.mouse.y >= leftpos[1] && events.mouse.y <= leftpos[1] + al_get_bitmap_height(left))
							{
								if (selectedSkin - 1 < 1)
									selectedSkin = totalSkins;
								else
									selectedSkin--;

								break;
							}
							else if (events.mouse.x >= rightpos[0] && events.mouse.x <= rightpos[0] + al_get_bitmap_width(right) && events.mouse.y >= rightpos[1] && events.mouse.y <= rightpos[1] + al_get_bitmap_height(right))
							{
								if (selectedSkin + 1 > totalSkins)
									selectedSkin = 1;
								else
									selectedSkin++;
								break;
							}
							else if (events.mouse.x >= playpos[0] && events.mouse.x <= playpos[0] + al_get_bitmap_width(play) && events.mouse.y >= playpos[1] && events.mouse.y <= playpos[1] + al_get_bitmap_height(play))
							{
								// Evento de clique no botão de jogar
								// Aqui deve ficar a parte de conexão com o servidor, casos de erro de conexão, etc.

								//Verifica se a string do nome não está vazia e nem o IP
								if (strlen(name) > 0 && strlen(ip) > 0)
								{
									cory.cor = selectedSkin;

									// Inicializa o player
									char ServerIP[30]={"127.0.0.1"};
									enum conn_ret_t ans = connectToServer(ServerIP);

								    if (ans != SERVER_UP) 
									{
									    if (ans == SERVER_DOWN) {
									      puts("Server is down!");
									      exit(1);
									    } else if (ans == SERVER_FULL) {
									      puts("Server is full!");
									      exit(1);
									    } else if (ans == SERVER_CLOSED) {
									      puts("Server is closed for new connections!");
									      exit(1);
									    } else {
									      puts("Server didn't respond to connection!");
									      exit(1);
									    }
									} else {
										recvMsgFromServer(&myid, WAIT_FOR_IT);

								    	printf("Meu id eh %i\n", myid);

								    	sendMsgToServer(&cory, sizeof(corAux));
									}

									// Vai para a tela do jogo
									dead = false;
									al_hide_mouse_cursor(display);
									playScreen = true;
									mainScreen = false;
									break;
								}
								else
								{
									al_show_native_message_box(display, "Cobrinha.io", "Preencha todos os dados!", "Tente novamente",
										NULL, ALLEGRO_MESSAGEBOX_WARN);
								}
							}

							if (changed)
								break;
						}
					}
				}
			}
		}

		// ###############################################################
		// #					Loop da tela de jogo					 #
		// ###############################################################

		while (playScreen)
		{
			pack.pressed = 0;

			al_start_timer(timer);

			// Loop para deixar o programa esperando acontecer algum evento sem atualizar os gráficos
			while (1)
			{
				startTimer();

				// A verificação das teclas acontece sem o uso de eventos para reduzir o lag
				ALLEGRO_KEYBOARD_STATE keyState;
				al_get_keyboard_state(&keyState);

				if (al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
				{
					escPlay = true;
					playScreen = false;
					break;
				}

				if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT) && pack.pressed != 1)
				{
					pack.dir = RIGHT;
					pack.pressed = 1;
				}
				else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT) && pack.pressed != 2)
				{
					pack.dir = LEFT;
					pack.pressed = 2;
				}
				else if (!al_key_down(&keyState, ALLEGRO_KEY_RIGHT) && !al_key_down(&keyState, ALLEGRO_KEY_LEFT))
				{
					pack.pressed = 0;
				}

				// Verifica se está pressionado
				if (pack.pressed)
				{
					sendMsgToServer(&pack, sizeof(direc));
				}


				// Atualização dos gráficos do jogo
				cameraUpdate(cameraPosition, player[myid].x, player[myid].y);
				al_identity_transform(&camera);
				al_translate_transform(&camera, -cameraPosition[0], -cameraPosition[1]);
				al_use_transform(&camera);

				redrawBackground();

				if (recvMsgFromServer(&syncy, DONT_WAIT) == sizeof(syncy)) 
				{
					quantPlayers = syncy.numPlayers;

					for(z = 0; z <= quantPlayers; z++)
			        {	
			        	if(!syncy.disc[z])
                    	{
			        		recvMsgFromServer(&player[z], WAIT_FOR_IT);
			        	}
			        }
				} else {
					for(z = 0; z <= quantPlayers; z++)
					{
						if(!syncy.disc[z])
                    	{
							(player[z].x)++;
							(player[z].y)++;
						}
					}
				}

				pack.scoreAux = player[myid].score;

				drawFood();

				// Verifica se a cobra morreu00
				if (!dead)
					drawChar(player[myid]);
				else
				{	
					pack.dead = 1;
					sendMsgToServer(&pack, sizeof(direc));

					redrawBackground();
					al_draw_text(raleway48, al_map_rgb(255, 255, 255), cameraPosition[0] + (screenWidth / 2), cameraPosition[1] + (screenHeight / 2), ALLEGRO_ALIGN_CENTRE, "Fim de jogo :(");

					al_flip_display();
					al_rest(2.0);

					escPlay = true;
					playScreen = false;
					break;
				}

				for(z = 0; z <= quantPlayers; z++)
		        {	
		        	if(!syncy.disc[z])
                    {
			       		if(z != myid)
			       		{
			        		drawEnemy(player[z]);
			        	}
			        }
		        }

				al_draw_text(raleway16, al_map_rgb(255, 255, 255), cameraPosition[0] + 15, cameraPosition[1] + 15, 0, "Aperte ESC para sair");

				al_flip_display();

				// Aumenta a pontuação, caso tenha comido
				if (scored)
				{
					pack.scoreAux++;

					sendMsgToServer(&pack, sizeof(direc));

					scored = false;
				}

				// Limita o FPS
        		FPSLimit();
			}
		}
	}

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);

	return 0;
}

bool initialize()
{
	// Inicializa as funções do Allegro e configura o jogo
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

	if (!al_install_mouse())
	{
		fprintf(stderr, "Falha ao inicializar mouse.\n");
		return false;
	}

	if (!al_init_font_addon())
	{
		fprintf(stderr, "Falha ao inicializar add-on de fontes.\n");
		return false;
	}

	if (!al_init_ttf_addon())
	{
		fprintf(stderr, "Falha ao inicializar add-on de TTF.\n");
		return false;
	}

	if (!al_init_image_addon())
	{
		fprintf(stderr, "Falha ao inicializar add-on de imagens.\n");
		return false;
	}

	// Descomente as próximas linhas para rodar o jogo em tela cheia

	al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 6, ALLEGRO_SUGGEST);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	al_hold_bitmap_drawing(true);

	//ALLEGRO_DISPLAY_MODE disp_data;
	//al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
	//screenWidth = disp_data.width;
	//screenHeight = disp_data.height;
	display = al_create_display(screenWidth, screenHeight);

	if (!display)
	{
		fprintf(stderr, "Falha ao criar janela.\n");
		return false;
	}

	// Configura a fila de eventos
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

	// Configura o timer
	timer = al_create_timer(1.0 / FPS);
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	raleway16 = al_load_font("res/ttf/Raleway-Medium.ttf", 16, 0);
	raleway48 = al_load_font("res/ttf/Raleway-Medium.ttf", 48, 0);

	al_clear_to_color(al_map_rgb(30, 30, 30));

	background = al_load_bitmap("res/bg_tile.png");

	if (!background)
	{
		fprintf(stderr, "failed to load background bitmap!\n");
		return -1;
	}

	al_set_window_title(display, "Cobrinha.io");

	strcpy(name, "");
	strcpy(ip, "");

	srand((unsigned)time(NULL));

	return true;
}

void getSkinRGB(char skin, int ret[3])
{
	// Retorna a cor da skin escolhida

	switch (skin)
	{
	case 1:
		ret[0] = 249;
		ret[1] = 38;
		ret[2] = 114;
		break;
	case 2:
		ret[0] = 116;
		ret[1] = 38;
		ret[2] = 249;
		break;
	case 3:
		ret[0] = 68;
		ret[1] = 218;
		ret[2] = 76;
		break;
	case 4:
		ret[0] = 245;
		ret[1] = 114;
		ret[2] = 42;
		break;
	case 5:
		ret[0] = 245;
		ret[1] = 232;
		ret[2] = 42;
		break;
	}

}

void redrawBackground()
{
	// Desenha o background na tela
	al_clear_to_color(al_map_rgb(13, 17, 22));
	al_draw_bitmap_region(background, cameraPosition[0], cameraPosition[1], screenWidth, screenHeight, cameraPosition[0], cameraPosition[1], 0);
}

void cameraUpdate(int *cameraPosition, int x, int y)
{
	// Atualiza a posição da câmera
	cameraPosition[0] = -(screenWidth / 2) + x;
	cameraPosition[1] = -(screenHeight / 2) + y;

	if (cameraPosition[0] < 0) cameraPosition[0] = 0;
	if (cameraPosition[1] < 0) cameraPosition[1] = 0;
	if (cameraPosition[0] > worldWidth - screenWidth) cameraPosition[0] = worldWidth - screenWidth;
	if (cameraPosition[1] > worldHeight - screenHeight) cameraPosition[1] = worldHeight - screenHeight;
}

void drawCircle(float pos_x, float pos_y, float raio, int r, int g, int b, int a)
{
	// Desenha um círculo com anti-aliasing
	int i;
	for (i = 5; i > 0; i--)
	{
		float t = (i - 1) / 5;
		t = sqrt(t);
		t = 1 - cos(t*3.1415926 / 2);
		al_draw_filled_circle(pos_x, pos_y, raio + i * 0.2, al_map_rgba(r, g, b, (1 - t) * (a / i)));
	}
}

void drawChar(Snake character)
{
	// Desenha a cobra do usuário
	int i, ballsNumber = (int)(character.score / 20) + 5;

	int rgb[3];
	getSkinRGB(character.skin, rgb);

	float k;
	for (i = ballsNumber - 1; i >= 0; i--)
	{
		if(character.orientacao[i] != -1)
		{
			// Desenha os segmentos da cobra
			float rad_orientation = character.orientacao[i] * 3.1415926 / 180.0;
			float cos_rad = cos(rad_orientation);
			float sin_rad = sin(rad_orientation);
			k = (float)(2 * (ballsNumber - 1) - i) / (2 * (ballsNumber - 1));
			if (i == ballsNumber - 1)
				drawCircle(character.x - (cos_rad * (i-1) * character.radius) - (cos_rad * (character.score % 20)), character.y + (sin_rad * (i - 1) * character.radius) + (sin_rad * (character.score % 20)), character.radius, (int)(k*rgb[0]), (int)(k*rgb[1]), (int)(k*rgb[2]), 255);
			else
				drawCircle(character.x - (cos_rad * i * character.radius), character.y + (sin_rad * i * character.radius), character.radius, (int)(k*rgb[0]), (int)(k*rgb[1]), (int)(k*rgb[2]), 255);

			if (!i)
			{
				// Cálculos pros olhos
				float xcos = cos_rad * ((character.radius) - (0.2 * character.radius) - (character.radius / 4));
				float xsin = sin_rad * ((character.radius) - (0.4 * character.radius) - (character.radius / 4));
				float ycos = cos_rad * ((character.radius) - (0.4 * character.radius) - (character.radius / 4));
				float ysin = sin_rad * -((character.radius) - (0.2 * character.radius) - (character.radius / 4));

				float offset_x1 = xcos + xsin;
				float offset_x2 = xcos - xsin;
				float offset_y1 = ycos + ysin;
				float offset_y2 = ysin - ycos;

				// Desenha a parte branca dos olhos
				drawCircle(character.x + offset_x1, character.y + offset_y1, character.radius / 4, 255, 255, 255, 255);
				drawCircle(character.x + offset_x2, character.y + offset_y2, character.radius / 4, 255, 255, 255, 255);

				// Desenha a parte preta dos olhos
				drawCircle(character.x + offset_x1 - (character.radius / 8), character.y + offset_y1, character.radius / 8, 0, 0, 0, 255);
				drawCircle(character.x + offset_x2 - (character.radius / 8), character.y + offset_y2, character.radius / 8, 0, 0, 0, 255);
			}
		}
	}
}

void drawEnemy(Snake enemy)
{
	// Desenha a cobra do adversário
	int i, ballsNumber = (int)(enemy.score / 20) + 5;

	int rgb[3];
	getSkinRGB(enemy.skin, rgb);

	float k;
	for (i = ballsNumber - 1; i >= 0; i--)
	{
		if(enemy.orientacao[i] != -1)
		{
			// Desenha os segmentos da cobra
			float rad_orientation = enemy.orientacao[i] * 3.1415926 / 180.0;
			float cos_rad = cos(rad_orientation);
			float sin_rad = sin(rad_orientation);
			k = (float)(2 * (ballsNumber - 1) - i) / (2 * (ballsNumber - 1));
			if (i == ballsNumber - 1)
				drawCircle(enemy.x - (cos_rad * (i - 1) * enemy.radius) - (cos_rad * (enemy.score % 20)), enemy.y + (sin_rad * (i - 1) * enemy.radius) + (sin_rad * (enemy.score % 20)), enemy.radius, (int)(k*rgb[0]), (int)(k*rgb[1]), (int)(k*rgb[2]), 255);
			else
				drawCircle(enemy.x - (cos_rad * i * enemy.radius), enemy.y + (sin_rad * i * enemy.radius), enemy.radius, (int)(k*rgb[0]), (int)(k*rgb[1]), (int)(k*rgb[2]), 255);


			/*_draw_line(enemy.x - (cos(rad_orientation) * i * enemy.radius) - enemy.radius, enemy.y + (sin(rad_orientation) * i * enemy.radius), enemy.x - (cos(rad_orientation) * i * enemy.radius) + enemy.radius, enemy.y + (sin(rad_orientation) * i * enemy.radius), al_map_rgb(255, 0, 0), 1);
			al_draw_line(enemy.x - (cos(rad_orientation) * i * enemy.radius), enemy.y + (sin(rad_orientation) * i * enemy.radius) - enemy.radius, enemy.x - (cos(rad_orientation) * i * enemy.radius), enemy.y + (sin(rad_orientation) * i * enemy.radius) + enemy.radius, al_map_rgb(255, 0, 0), 1);

			al_draw_line(player[myid].x - enemy.radius, player[myid].y, player[myid].x + enemy.radius, player[myid].y, al_map_rgb(255, 255, 255), 1);
			al_draw_line(player[myid].x, player[myid].y - enemy.radius, player[myid].x , player[myid].y + enemy.radius, al_map_rgb(255, 255, 255), 1);*/



			if ((enemy.x - (cos_rad * i * enemy.radius) - enemy.radius + 5 > player[myid].x - player[myid].radius &&
				enemy.x - (cos_rad * i * enemy.radius) - enemy.radius - 5 < player[myid].x + player[myid].radius &&
				enemy.y + (sin_rad * i * enemy.radius) > player[myid].y - player[myid].radius &&
				enemy.y + (sin_rad * i * enemy.radius) < player[myid].y + player[myid].radius))
			{
				dead = true;
			}

			if (!i)
			{
				// Cálculos pros olhos
				float xcos = cos_rad * ((enemy.radius) - (0.2 * enemy.radius) - (enemy.radius / 4));
				float xsin = sin_rad * ((enemy.radius) - (0.4 * enemy.radius) - (enemy.radius / 4));
				float ycos = cos_rad * ((enemy.radius) - (0.4 * enemy.radius) - (enemy.radius / 4));
				float ysin = sin_rad * -((enemy.radius) - (0.2 * enemy.radius) - (enemy.radius / 4));

				float offset_x1 = xcos + xsin;
				float offset_x2 = xcos - xsin;
				float offset_y1 = ycos + ysin;
				float offset_y2 = ysin - ycos;

				// Desenha a parte branca dos olhos
				drawCircle(enemy.x + offset_x1, enemy.y + offset_y1, (float)enemy.radius / 4, 255, 255, 255, 255);
				drawCircle(enemy.x + offset_x2, enemy.y + offset_y2, (float)enemy.radius / 4, 255, 255, 255, 255);

				// Desenha a parte preta dos olhos
				drawCircle(enemy.x + offset_x1 - (enemy.radius / 8), enemy.y + offset_y1, (float)enemy.radius / 8, 0, 0, 0, 255);
				drawCircle(enemy.x + offset_x2 - (enemy.radius / 8), enemy.y + offset_y2, (float)enemy.radius / 8, 0, 0, 0, 255);
			}
		}
	}
}

void drawFood()
{
	srand(player[myid].seed);
	int i, j, k;
	for (i = 0; i < worldWidth; i += 95)
	{
		for (j = 0; j < worldHeight; j += 110)
		{
			char eated = false;
			for (k = 0; k < eFSize; k++)
			{
				if (eatedFoodsX[k] == i && eatedFoodsY[k] == j)
				{
					eated = true;
				}
			}

			int rX = rand() % worldWidth;
			int rY = rand() % worldHeight;

			if (!eated)
			{

				if ((rX - 3 > player[myid].x - player[myid].radius &&
					rX - 3 < player[myid].x + player[myid].radius &&
					rY > player[myid].y - player[myid].radius &&
					rY < player[myid].y + player[myid].radius)
					||
					(rX + 3 > player[myid].x - player[myid].radius &&
						rX + 3 < player[myid].x + player[myid].radius &&
						rY > player[myid].y - player[myid].radius &&
						rY < player[myid].y + player[myid].radius)
					||
					(rX > player[myid].x - player[myid].radius &&
						rX < player[myid].x + player[myid].radius &&
						rY - 3 > player[myid].y - player[myid].radius &&
						rY - 3 < player[myid].y + player[myid].radius)
					||
					(rX > player[myid].x - player[myid].radius &&
						rX < player[myid].x + player[myid].radius &&
						rY + 3 > player[myid].y - player[myid].radius &&
						rY + 3 < player[myid].y + player[myid].radius))
				{
					// ENCOSTA EM UM PONTO
					scored = true;
					eFSize++;
					eatedFoodsX = realloc(eatedFoodsX, eFSize * sizeof(int));
					eatedFoodsY = realloc(eatedFoodsY, eFSize * sizeof(int));

					eatedFoodsX[eFSize - 1] = i;
					eatedFoodsY[eFSize - 1] = j;

					//printf("Encostou no ponto x: %d y: %d\n", i, j);
				}

				if (rX >= cameraPosition[0] - 8 && rX <= cameraPosition[0] + screenWidth + 8 &&
					rY >= cameraPosition[1] - 8 && rY <= cameraPosition[1] + screenHeight + 8)
					drawCircle(rX, rY, 3, 255, 253, 224, 255);
			}
		}
	}
}
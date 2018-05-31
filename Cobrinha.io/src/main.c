#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Funções do jogo
bool initialize();
void cameraUpdate(int *cameraPosition, int x, int y);
void redrawBackground();
int *getSkinRGB(char skin);
void drawCircle(float pos_x, float pos_y, float raio, int r, int g, int b);
void drawChar(float pos_x, float pos_y, float raio, char skin, float *orientacao_array, int pontuacao);
void drawEnemy(float pos_x, float pos_y, float raio, char skin, float *orientacao_array, int pontuacao_inimigo, int pontuacao_principal);

// Variáveis de controle
float x = 2000, y = 2000;
int cameraPosition[2] = { 0, 0 };
int orientation = 0;
float orientation_rad = 0;
float *orientation_array = NULL;
int worldWidth = 4950;
int worldHeight = 4350;
int screenWidth = 1280;
int screenHeight = 720;
enum directions { UP, DOWN, LEFT, RIGHT };
int dir = LEFT;
int moveSpeed = 2;
const float FPS = 60.0;
char name[19];
char ip[16];

// Variáveis do Allegro
ALLEGRO_KEYBOARD_STATE keyState;
ALLEGRO_TRANSFORM *camera;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP  *background;
ALLEGRO_TIMER *timer;
ALLEGRO_FONT *raleway16;


int main(void)
{
	if (!initialize())
	{
		return -1;
	}

	bool mainLoop = true;
	bool escPlay = false;
	bool tbSelected = false;
	bool tb2Selected = false;
	char selectedSkin = 1, totalSkins = 5, nextSkin, previousSkin;

	// Loop do jogo
	while (mainLoop)
	{
		// "Reset" das variáveis toda vez que volta pro começo do loop
		bool mainScreen = true;
		bool playScreen = false;

		bool MO = false;

		x = 2000;
		y = 2000;
		cameraPosition[0] = 0;
		cameraPosition[1] = 0;
		orientation = 0;
		orientation_rad = 0;
		orientation_array = NULL;

		al_identity_transform(&camera);
		al_translate_transform(&camera, 0, 0);
		al_use_transform(&camera);

		bool playMO = false;
		bool leftMO = false;
		bool rightMO = false;

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


			logo = al_load_bitmap("C:\\Users\\Uanderson\\\Documents\\C\\Slither.io\\logo.png");
			textbox = al_load_bitmap("C:\\Users\\Uanderson\\\Documents\\C\\Slither.io\\textbox.png");
			play = al_load_bitmap("C:\\Users\\Uanderson\\\Documents\\C\\Slither.io\\play.png");
			left = al_load_bitmap("C:\\Users\\Uanderson\\\Documents\\C\\Slither.io\\left.png");
			right = al_load_bitmap("C:\\Users\\Uanderson\\\Documents\\C\\Slither.io\\right.png");

			strcpy_s(path, 80, "C:\\Users\\Uanderson\\\Documents\\C\\Slither.io\\skins\\");

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

			strcat_s(path, 80, extra);
			skin1 = al_load_bitmap(path);

			strcpy_s(path, 80, "C:\\Users\\Uanderson\\\Documents\\C\\Slither.io\\skins\\");

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

			strcat_s(path, 80, extra);
			skin2 = al_load_bitmap(path);

			strcpy_s(path, 80, "C:\\Users\\Uanderson\\\Documents\\C\\Slither.io\\skins\\");

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

			strcat_s(path, 80, extra);
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
			ALLEGRO_FONT *raleway36 = al_load_font("C:\\Users\\Uanderson\\\Documents\\C\\Slither.io\\ttf\\Raleway-Light.ttf", 36, 0);
			al_draw_text(raleway36, al_map_rgb(255, 255, 255), screenWidth / 2, screenHeight*0.75, ALLEGRO_ALIGN_CENTRE, "escolha sua skin");

			if (!strlen(name))
				al_draw_text(raleway36, al_map_rgba_f(1, 1, 1, 0.2), tbpos[0] + al_get_bitmap_width(textbox) / 2, tbpos[1] + 5, ALLEGRO_ALIGN_CENTRE, "digite o nome");

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
								) && sizeName < 18) {
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
								if (strlen(name) > 0 && strlen(ip) > 0)
								{
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
		
		while (playScreen)
		{
			bool draw = false;
			int pressed = 0;
			int score = 20;

			int count;

			orientation_array = malloc((score / 2) * sizeof(float));

			for (count = 0; count < (score / 2); count++)
			{
				orientation_array[count] = orientation_rad;
			}

			al_start_timer(timer);

			// Loop para deixar o programa esperando acontecer algum evento sem atualizar os gráficos
			while (1)
			{
				ALLEGRO_EVENT events;
				al_wait_for_event(event_queue, &events);

				// Eventos de apertar de tecla
				if (events.type == ALLEGRO_EVENT_KEY_DOWN)
				{
					if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					{
						escPlay = true;
						playScreen = false;
						break;
					}

					switch (events.keyboard.keycode)
					{
					case ALLEGRO_KEY_RIGHT:
						dir = RIGHT;
						pressed = 1;
						break;
					case ALLEGRO_KEY_LEFT:
						dir = LEFT;
						pressed = 2;
						break;
					}
				}

				// Eventos de soltar de tecla
				if (events.type == ALLEGRO_EVENT_KEY_UP)
				{
					switch (events.keyboard.keycode)
					{
					case ALLEGRO_KEY_RIGHT:
						if (pressed == 1)
							pressed = 0;
						break;
					case ALLEGRO_KEY_LEFT:
						if (pressed == 2)
							pressed = 0;
						break;
					}
				}

				// Eventos de timer
				if (events.type == ALLEGRO_EVENT_TIMER)
				{
					orientation_array = realloc(orientation_array, (score / 2) * sizeof(float));

					if (pressed)
					{
						switch (dir)
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
						orientation_array[count] = orientation_array[count - 1];

					orientation_array[0] = orientation_rad;

					x += cos(orientation_rad) * moveSpeed;
					y -= sin(orientation_rad) * moveSpeed;

					draw = true;
				}

				// Atualização dos gráficos do jogo
				if (draw)
				{

					cameraUpdate(cameraPosition, x, y);
					al_identity_transform(&camera);
					al_translate_transform(&camera, -cameraPosition[0], -cameraPosition[1]);
					al_use_transform(&camera);

					redrawBackground();

					drawChar(x, y, 20, selectedSkin, orientation_array, score);
					drawEnemy(300, 200, 20, previousSkin, orientation_array, 10, score);

					al_draw_text(raleway16, al_map_rgb(255, 255, 255), cameraPosition[0] + 15, cameraPosition[1] + 15, 0, "Aperte ESC para sair");

					al_flip_display();

					draw = false;
				}
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
	al_set_new_display_option(ALLEGRO_SAMPLES, 6, ALLEGRO_SUGGEST);	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
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

	raleway16 = al_load_font("C:\\Users\\Uanderson\\\Documents\\C\\Slither.io\\ttf\\Raleway-Medium.ttf", 16, 0);

	al_clear_to_color(al_map_rgb(30, 30, 30));

	background = al_load_bitmap("C:\\Users\\Uanderson\\\Documents\\C\\Slither.io\\bg_tile.png");

	if (!background)
	{
		fprintf(stderr, "failed to load background bitmap!\n");
		return -1;
	}

	al_set_window_title(display, "Cobrinha.io");

	strcpy_s(name, 19, "");
	strcpy_s(ip, 16, "");
	return true;
}

int *getSkinRGB(char skin)
{
	// Retorna a cor da skin escolhida
	int *color = malloc(3*sizeof(int));

	switch (skin)
	{
	case 1:
		color[0] = 249;
		color[1] = 38;
		color[2] = 114;
		break;
	case 2:
		color[0] = 116;
		color[1] = 38;
		color[2] = 249;
		break;
	case 3:
		color[0] = 68;
		color[1] = 218;
		color[2] = 76;
		break;
	case 4:
		color[0] = 245;
		color[1] = 114;
		color[2] = 42;
		break;
	case 5:
		color[0] = 245;
		color[1] = 232;
		color[2] = 42;
		break;
	}

	return color;
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

void drawCircle(float pos_x, float pos_y, float raio, int r, int g, int b)
{
	// Desenha um círculo com anti-aliasing
	int i;
	for (i = 5; i > 0; i--)
	{
		float t = (i - 1) / 5;
		t = sqrt(t);
		t = 1 - cos(t*3.1415926 / 2);
		al_draw_filled_circle(pos_x, pos_y, raio + i * 0.2, al_map_rgba(r, g, b, (1 - t) * (255 / i)));
	}
}

void drawChar(float pos_x, float pos_y, float raio, char skin, float *orientacao_array, int pontuacao)
{
	// Desenha a cobra do usuário
	int i, tamanho = (int)pontuacao / 2;

	int r = getSkinRGB(skin)[0];
	int g = getSkinRGB(skin)[1];
	int b = getSkinRGB(skin)[2];

	float k;
	for (i = tamanho - 1; i >= 0; i--)
	{
		// Desenha os segmentos da cobra
		float orientacao_rad = orientacao_array[i];
		k = (float)(2 * (tamanho - 1) - i) / (2 * (tamanho - 1));
		drawCircle(pos_x - (cos(orientacao_rad) * i * raio), pos_y + (sin(orientacao_rad) * i * raio), raio, (int)(k*r), (int)(k*g), (int)(k*b));

		if (!i)
		{
			// Cálculos pros olhos
			float xcos = cos(orientacao_rad) * ((raio)-(0.1 * 2 * raio) - (raio / 4));
			float xsin = sin(orientacao_rad) * ((raio)-(0.2 * 2 * raio) - (raio / 4));
			float ycos = cos(orientacao_rad) * ((raio)-(0.2 * 2 * raio) - (raio / 4));
			float ysin = sin(orientacao_rad) * -((raio)-(0.1 * 2 * raio) - (raio / 4));

			float offset_x1 = xcos + xsin;
			float offset_x2 = xcos - xsin;
			float offset_y1 = ycos + ysin;
			float offset_y2 = ysin - ycos;

			// Desenha a parte branca dos olhos
			drawCircle(pos_x + offset_x1, pos_y + offset_y1, (float)raio / 4, 255, 255, 255);
			drawCircle(pos_x + offset_x2, pos_y + offset_y2, (float)raio / 4, 255, 255, 255);

			// Desenha a parte preta dos olhos
			drawCircle(pos_x + offset_x1 - (raio / 8), pos_y + offset_y1, (float)raio / 8, 0, 0, 0);
			drawCircle(pos_x + offset_x2 - (raio / 8), pos_y + offset_y2, (float)raio / 8, 0, 0, 0);
		}
	}
}

void drawEnemy(float pos_x, float pos_y, float raio, char skin, float *orientacao_array, int pontuacao_inimigo, int pontuacao_principal)
{
	// Desenha a cobra do adversário (escalonado de acordo com o tamanho da cobra do usuário)
	float raio_scaled = raio * pontuacao_inimigo / pontuacao_principal;
	int i, tamanho = (int)pontuacao_inimigo / 2;

	int r = getSkinRGB(skin)[0];
	int g = getSkinRGB(skin)[1];
	int b = getSkinRGB(skin)[2];

	float k;
	for (i = tamanho - 1; i >= 0; i--)
	{
		// Desenha os segmentos da cobra
		float orientacao_rad = orientacao_array[i];
		k = (float)(2 * (tamanho - 1) - i) / (2 * (tamanho - 1));
		drawCircle(pos_x - (cos(orientacao_rad) * i * raio_scaled), pos_y + (sin(orientacao_rad) * i * raio_scaled), raio_scaled, (int)(k*r), (int)(k*g), (int)(k*b));

		if (!i)
		{
			// Cálculos pros olhos
			float xcos = cos(orientacao_rad) * ((raio_scaled)-(0.1 * 2 * raio_scaled) - (raio_scaled / 4));
			float xsin = sin(orientacao_rad) * ((raio_scaled)-(0.2 * 2 * raio_scaled) - (raio_scaled / 4));
			float ycos = cos(orientacao_rad) * ((raio_scaled)-(0.2 * 2 * raio_scaled) - (raio_scaled / 4));
			float ysin = sin(orientacao_rad) * -((raio_scaled)-(0.1 * 2 * raio_scaled) - (raio_scaled / 4));

			float offset_x1 = xcos + xsin;
			float offset_x2 = xcos - xsin;
			float offset_y1 = ycos + ysin;
			float offset_y2 = ysin - ycos;

			// Desenha a parte branca dos olhos
			drawCircle(pos_x + offset_x1, pos_y + offset_y1, (float)raio_scaled / 4, 255, 255, 255);
			drawCircle(pos_x + offset_x2, pos_y + offset_y2, (float)raio_scaled / 4, 255, 255, 255);

			// Desenha a parte preta dos olhos
			drawCircle(pos_x + offset_x1 - (raio_scaled / 8), pos_y + offset_y1, (float)raio_scaled / 8, 0, 0, 0);
			drawCircle(pos_x + offset_x2 - (raio_scaled / 8), pos_y + offset_y2, (float)raio_scaled / 8, 0, 0, 0);
		}
	}
}
#include "game.h"

void Game::Init()
{
	running = 1; x = 20; y = 20;

	//�������
	player = new Player(10, 10, 50, 100);
	player->Ready();
	win=player->CreateWindow();

	//������Ⱦ��
	render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (render == NULL) {
		SDL_Log("Cannot create renderer,%s", SDL_GetError());
		running = 0;
		return;
	}	
}

void Game::Events()
{
	//�¼�
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running = 0;
			return;
			//�����¼�
		case SDL_KEYDOWN:
			keyboard_state_array = SDL_GetKeyboardState(NULL);
			if (keyboard_state_array[SDL_SCANCODE_UP]) {
				player->SetDirectionY(-1);
				//SDL_Log("UP");
			}
			else if (keyboard_state_array[SDL_SCANCODE_DOWN]) {
				player->SetDirectionY(1);
				//SDL_Log("DOWN");
			}
			if (keyboard_state_array[SDL_SCANCODE_LEFT]) {
				player->SetDirectionX(-1);
				//SDL_Log("LEFT");
			}
			else if (keyboard_state_array[SDL_SCANCODE_RIGHT]) {
				player->SetDirectionX(1);
				//SDL_Log("RIGHT");
			}
			break;
		case SDL_KEYUP:
			keyboard_state_array = SDL_GetKeyboardState(NULL);
			if (!keyboard_state_array[SDL_SCANCODE_UP]&& !keyboard_state_array[SDL_SCANCODE_DOWN]) {
				player->SetDirectionY(0);
				//SDL_Log("UP");
			}
			if (!keyboard_state_array[SDL_SCANCODE_LEFT]&& !keyboard_state_array[SDL_SCANCODE_RIGHT]) {
				player->SetDirectionX(0);
				//SDL_Log("LEFT");
			}
			break;
		}
	}
	return;
}


void Game::Loop()
{
	player->PhysicsProcess(60);

	//�����ӿ�
	if (player->get_x_expand()) {
		x += 400* player->get_x_expand();
		SDL_SetWindowPosition(win, x, y);
		player->x_expanded();
		SDL_Log("X Expaned to: %d", x);
	}
	if (player->get_y_expand()) {
		y += 200* player->get_y_expand();
		SDL_SetWindowPosition(win, x, y);
		player->y_expanded();
		SDL_Log("Y Expaned to %d", y);
	}
}

void Game::Render()
{
	//����ɫ
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderClear(render);
	player->Render(render);
	SDL_RenderPresent(render);//��Ч
}

void Game::Clean()
{
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(render);
	SDL_Quit();
	return;
}

bool Game::IsRunning()
{
	return running;
}

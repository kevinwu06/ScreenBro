#pragma once
#include "scene.h"
#include "player.h"
#include "bullet.h"
#include <algorithm>
#include "timer.h"
#include "enemy.h"
#include <stdlib.h>	//���������
#include <time.h>	//���������
#include "scene_manager.h"
#include <iostream>
#include <string>
#include "platform.h" //ƽ̨

extern SceneManager scene_manager;
extern TTF_Font *font;

class GameScene :public Scene {
public:
	GameScene(Window* win) :window(win) {}
	~GameScene() = default;

	//��ʼ��
	void on_enter() {
		SDL_Log("Enter Game Scene");
		
		//�����ʼ��
		std::fill(bullet, bullet + BULLET_MAX, nullptr);
		std::fill(enemy, enemy + ENEMY_MAX, nullptr);
		std::fill(platform, platform + PLATFORM_MAX, nullptr);

		//���������
		srand((unsigned int)time(NULL));

		//�������
		player = new Player(window);
		player->on_create();

		//ʱ���ʱ��
		game_timer.set_wait_time(1);
		game_timer.set_callback([this]() {
			this->timer_number++;
			std::string str = std::to_string(this->timer_number);
			if (timer_number < 10) { str = '0'+str; }
			//��������
			SDL_Surface* time_surface = TTF_RenderUTF8_Solid(font, str.c_str(), {200,0,0,255});
			this->time_texture = SDL_CreateTextureFromSurface(window->get_render(), time_surface);
			SDL_FreeSurface(time_surface);
		});

		//��������ʱ��
		shoot_timer.set_wait_time(0.1);
		shoot_timer.set_callback([this]() {
			this->player->set_can_shoot(true);
		});

		//�������ɼ�ʱ��
		enemy_timer.set_wait_time(1);
		enemy_timer.set_callback([this](){
			this->can_create_enemy = true;
		});

		//����ƽ̨
		platform[0] = new Platform();
		platform[0]->set_size(500, 100);
		platform[0]->on_create();

		platform[1] = new Platform();
		platform[1]->set_position(200, 400);
		platform[1]->set_size(800, 300);
		platform[1]->on_create();

		platform[2] = new Platform();
		platform[2]->set_position(500, 0);
		platform[2]->set_size(100, 1000);
		platform[2]->on_create();
		/*for (int i = 0; i < 4; i++) {
			platform[i] = new Platform();
			platform[i]->set_position(200, 400);
			platform[i]->set_size(800, 300);
			platform[i]->on_create();
		}*/

		player->set_platform(platform[0]);
	};

	//����ѭ��
	void on_update() {
		player->PhysicsProcess(delta);
		game_timer.on_update(delta);
		shoot_timer.on_update(delta);
		enemy_timer.on_update(delta);

		//��������
		CreateEnemy();
		//�����ײ
		CheckCollision();
	};

	//����ѭ��
	void on_draw() {
		//����ɫ
		SDL_SetRenderDrawBlendMode(window->get_render(), SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(window->get_render(), 0, 0, 0, SDL_ALPHA_TRANSPARENT);
		SDL_RenderClear(window->get_render());
		//����ɫ
		SDL_SetRenderDrawBlendMode(platform[1]->get_render(), SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(platform[1]->get_render(), 255, 255, 255, 255);
		SDL_RenderClear(platform[1]->get_render());
		//�ӵ�
		BulletUpdate();
		//����
		EnemyUpdate();
		//ƽ̨
		platform[1]->on_update();
		//���
		player->on_update();
		//����
		SDL_Rect time_rect = { 10,0,50,50 };
		SDL_RenderCopy(window->get_render(), time_texture, NULL, &time_rect);
		//��Ч
		SDL_RenderPresent(platform[1]->get_render());
		SDL_RenderPresent(window->get_render());
	};

	//����
	void on_input() {
		//���̻�ȡ
		keyboard_state_array = SDL_GetKeyboardState(NULL);
		//�������
		if (keyboard_state_array[SDL_SCANCODE_J]) {
			PlayerShoot();
		}
		//�ƶ����ƣ�Y��+1�����£�X��+1�����ң�
		if (keyboard_state_array[SDL_SCANCODE_W]|| keyboard_state_array[SDL_SCANCODE_UP]) {
			player->SetDirectionY(-1);
		}
		else if (keyboard_state_array[SDL_SCANCODE_S]||keyboard_state_array[SDL_SCANCODE_DOWN]) {
			player->SetDirectionY(1);
		}
		else {
			player->SetDirectionY(0);
		}
		if (keyboard_state_array[SDL_SCANCODE_A]||keyboard_state_array[SDL_SCANCODE_LEFT]) {
			player->SetDirectionX(-1);
		}
		else if (keyboard_state_array[SDL_SCANCODE_D]|| keyboard_state_array[SDL_SCANCODE_RIGHT]) {
			player->SetDirectionX(1);
		}
		else {
			player->SetDirectionX(0);
		}

		//�¼�
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					scene_manager.switch_to(SceneManager::SceneType::Menu);
					break;
				case SDLK_k:
					for (int i = 0; i < PLATFORM_MAX; i++) {
						if (platform[i] != nullptr && platform[i] != player->get_platform()) {
							if (player->change_platform(platform[i])) {
								break;
							}
						}
					}
					break;
				default:
					break;
				}
			default:
				break;
			}
		}
		return;
	};

	//�˳�
	void on_exit() {
		return;
	};

	//�ͷź���
	void on_destroy() {
		//���
		player->on_destroy();
		//��ʱ��
		SDL_DestroyTexture(time_texture);
		//�ӵ����
		for (int i = 0; i < BULLET_MAX;i++) {
			if (bullet[i] != nullptr) {
				SDL_Log("[%d]Bullet Destroyed",i);
				bullet[i]->on_destroy();
			}
		}
		for (int i = 0; i < ENEMY_MAX; i++) {
			if (enemy[i] != nullptr) {
				SDL_Log("[%d]Enemy Destroyed", i);
				enemy[i]->on_destroy();
			}
		}
		for (int i = 0; i < PLATFORM_MAX; i++) {
			if (platform[i] != nullptr) {
				SDL_Log("[%d]Platform Destroyed", i);
				platform[i]->on_destroy();
			}
		}
		delete this;
	}

//---------��Ϸ����---------
public:
	//��������
	void CreateEnemy() {
		if (can_create_enemy) {
			//Ѱ�Ҵ�ŵ��˵�λ��
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (enemy[i] == nullptr) {
					SDL_Log("[%d]Enemy Created", i);
					enemy[i] = new Enemy(platform[1], player);
					//����ƽ̨
					enemy[i]->set_platform(platform[1]);
					//��������
					enemy[i]->on_create();
					break;
				}
			}
			can_create_enemy = false;
		}
	}

	//���˸���
	void EnemyUpdate() {
		for (int i = 0; i < ENEMY_MAX; i++) {
			if (enemy[i] != nullptr) {
				//�Ƿ����
				if (enemy[i]->get_remove_state()) {
					enemy[i]->on_destroy();
					enemy[i] = nullptr;
					SDL_Log("[%d]Enemy Destroyed", i);
					continue;
				}
				//�����ƶ��ж�
				enemy[i]->find_player();
				//���������߼�
				enemy[i]->PhysicsProcess(delta);
				//���˺͵���
				for (int j = i + 1; j < ENEMY_MAX; j++) {
					if (enemy[j] != nullptr) {
						if (enemy[i]->check_collision(enemy[j]) == true) {
							enemy[i]->handle_collision(enemy[j]);
						}
					}
				}
				if (enemy[i]->check_collision(player) == true) {
					enemy[i]->handle_collision(player);
				}
				//������ʾ
				enemy[i]->on_update();
			}
		}
	}

	//�����ӵ��������
	void PlayerShoot() {
		if (player->get_can_shoot()) {
			//Ѱ�Ҵ���ӵ���λ��
			for (int i = 0; i < BULLET_MAX; i++) {
				if (bullet[i] == nullptr) {
					bullet[i] = new Bullet(window,i);
					bullet[i]->set_position(player->get_position());
					bullet[i]->set_direction(player->get_direction());
					break;
				}
			}
			player->set_can_shoot(false);
		}
	}

	//�ӵ�����
	void BulletUpdate() {
		for (int i = 0; i < BULLET_MAX; i++) {
			if (bullet[i] != nullptr) {
				if (bullet[i]->get_remove_state()) {
					bullet[i]->on_destroy();
					bullet[i] = nullptr;
					SDL_Log("[%d]Bullet Destroyed", i);
					continue;
				}
				bullet[i]->on_update();
			}
		}
	}

	//�����ײ
	void CheckCollision() {
		//�ӵ��͵���
		for (int i = 0; i < BULLET_MAX; i++) {
			if (bullet[i] != nullptr) {
				for (int j = 0; j < ENEMY_MAX; j++) {
					if (bullet[i]!=nullptr&&enemy[j] != nullptr) {
						if (check_hurt(bullet[i], enemy[j])) {
							break;
						}
					}
				}
			}
		}
	}

	//����ӵ��Ƿ�򵽵��ˣ��ڼ����ײ��ʹ�ã�
	bool check_hurt(Bullet *bu,Enemy *en) {
		if (bu->get_position().x >= en->get_position().x &&
			bu->get_position().x <= en->get_position().x+en->get_size().x &&
			bu->get_position().y >= en->get_position().y &&
			bu->get_position().y <= en->get_position().y+en->get_size().y
			) {
			//�ӵ����
			bu->on_remove();
			////�������
			en->on_remove();
			return true;
			}
		return false;
	}

private:
	Window* window = nullptr;						//����ָ��
	Player* player = nullptr;						//���ָ��
	Platform* platform[PLATFORM_MAX] = { nullptr };
	SDL_Texture* time_texture = nullptr;
	const Uint8* keyboard_state_array = nullptr;	//����״ָ̬��
	Bullet* bullet[BULLET_MAX] = { nullptr };			//�ӵ�ָ������
	Enemy* enemy[ENEMY_MAX] = { nullptr };
	Timer shoot_timer;
	Timer enemy_timer;
	Timer game_timer;
	int timer_number=0;
	float delta = 1 / FRAMERATE;
	bool can_create_enemy = false;				//�Ƿ�������ɵ���
};


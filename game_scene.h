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
#include <vector>
#include "core.h"
#include "screen_window.h"

extern SceneManager scene_manager;

class GameScene :public Scene {
public:
	GameScene() = default;

	~GameScene() = default;

	void on_create() {
		window = new ScreenWindow();

		//�����ʼ��
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
			if (timer_number < 10) { str = '0' + str; }
			//��������
			SDL_Surface* time_surface = TTF_RenderUTF8_Solid(font, str.c_str(), { 200,0,0,255 });
			this->time_texture = SDL_CreateTextureFromSurface(window->get_render(), time_surface);
			SDL_FreeSurface(time_surface);
			});

		//��������ʱ��
		shoot_timer.set_wait_time(0.1);
		shoot_timer.set_callback([this]() {
			this->player->set_can_shoot(true);
			});

		//����ƽ̨
		platform[0] = new Platform();
		platform[0]->set_position(0, 40);
		platform[0]->set_size(500, 100);
		platform[0]->on_create();

		platform[1] = new Platform();
		platform[1]->set_position(200, 400);
		platform[1]->set_size(800, 300);
		platform[1]->on_create();

		platform[2] = new Platform();
		platform[2]->set_position(500, 40);
		platform[2]->set_size(100, 1000);
		platform[2]->on_create();

		player->set_platform(platform[0]);

		//�������ɼ�ʱ��
		enemy_timer.set_wait_time(1);
		enemy_timer.set_callback([this]() {
			this->can_create_enemy = true;
			});

		//����
		font = TTF_OpenFont("resource/pixel.TTF", 40);
		if (!font) { SDL_Log("font not loaded"); }

		is_created = 1;
	}

	//��ʼ��
	void on_enter() {
		SDL_Log("[Game Scene] -> Game Scene");
		if (is_created == 0) {
			on_create();
		}
		window->on_enter();
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
		//CheckCollision();
	};

	//����ѭ��
	void on_draw() {
		//����ɫ
		SDL_SetRenderDrawBlendMode(window->get_render(), SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(window->get_render(), 0, 0, 0, SDL_ALPHA_TRANSPARENT);
		SDL_RenderClear(window->get_render());
		//����ɫ
		platform[0]->on_draw();
		platform[1]->on_draw();
		platform[2]->on_draw();
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
		SDL_RenderPresent(platform[0]->get_render());
		SDL_RenderPresent(platform[1]->get_render());
		SDL_RenderPresent(platform[2]->get_render());
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
		SDL_HideWindow(window->get_window());
		return;
	};

	//�ͷź���
	void on_destroy() {
		SDL_HideWindow(window->get_window());
		//���
		player->on_destroy();
		//��ʱ��
		SDL_DestroyTexture(time_texture);

		for (int i = 0; i < PLATFORM_MAX; i++) {
			if (platform[i] != nullptr) {
				platform[i]->on_destroy();
				SDL_Log("[%d]Platform Destroyed", i);
			}
		}
		//�ӵ����
		for (auto& bullet : bullets) {
			SDL_Log("Bullet Destroyed");
			bullet.on_destroy();
		}

		for (auto& enemy : enemies) {
			SDL_Log("Enemy Destroyed");
			enemy.on_destroy();
		}
		
		TTF_CloseFont(font);
		window->on_destroy();

		delete this;
	}

//---------��Ϸ����---------
public:
	//��������
	void CreateEnemy() {
		if (can_create_enemy) {
			if (platform[1] != nullptr) {
				enemies.push_back(Enemy(platform[1], player));
			}
			SDL_Log("enemy created");
			can_create_enemy = false;
		}
	}

	//���˸���
	void EnemyUpdate() {
		for (auto& enemy : enemies) {
			//�Ƿ����
			if (enemy.get_remove_state()) {
				enemy.on_destroy();
				SDL_Log("Enemy Destroyed");
				continue;
			}
			//�����ƶ��ж�
			enemy.find_player();
			//���������߼�
			enemy.PhysicsProcess(delta);
			//������ʾ
			enemy.on_update();	
		}
		//��������˼����ײ
		for (auto it1 = enemies.begin(); it1 != enemies.end(); ++it1) {
			for (auto it2 = it1 + 1; it2 != enemies.end(); ++it2) {
				if (checkCollision(it1->rect, it2->rect)) {
					handleCollision(*it1, *it2);
				}
			}
			if (checkCollision(it1->rect, player->rect)) {
				handleCollision(*it1, *player);
			}
		}
	}

	//�����ӵ��������
	void PlayerShoot() {
		if (player->get_can_shoot()) {
			//Ѱ�Ҵ���ӵ���λ��
			bullets.push_back(Bullet(player->get_platform(), player->get_position(), player->get_direction()));
			player->set_can_shoot(false);
		}
	}

	//�ӵ�����
	void BulletUpdate() {
		for (auto bullet_it = bullets.begin(); bullet_it != bullets.end();) {
			if (bullet_it->get_remove_state()) {
				SDL_Log("Bullet Destroyed");
				bullet_it = bullets.erase(bullet_it);
			}
			else {
				for (auto enemy_it = enemies.begin(); enemy_it != enemies.end();) {
					if (checkCollision(bullet_it->rect, enemy_it->rect)) {
						enemy_it = enemies.erase(enemy_it);
						bullet_it->on_remove();
					}
					else {
						++enemy_it;
					}
				}
				bullet_it->on_update();
				++bullet_it;
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
	ScreenWindow* window = nullptr;						//����ָ��
	Player* player = nullptr;						//���ָ��

	Platform* platform[PLATFORM_MAX] = { nullptr };
	SDL_Texture* time_texture = nullptr;
	const Uint8* keyboard_state_array = nullptr;	//����״ָ̬��
	Timer shoot_timer;
	Timer enemy_timer;
	Timer game_timer;
	int timer_number=0;
	float delta = 1 / FRAMERATE;
	bool can_create_enemy = false;				//�Ƿ�������ɵ���
	bool is_created = 0;

	std::vector<Enemy> enemies;
	std::vector<Bullet> bullets;

	//ͼƬ��Դ
	TTF_Font* font = nullptr;
};


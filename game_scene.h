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

extern SceneManager scene_manager;

class GameScene :public Scene {
public:
	GameScene(Window* win) :window(win) {}
	~GameScene() = default;

	//��ʼ��
	void on_enter() {
		SDL_Log("Enter Game Scene");
		
		//�����ʼ��
		std::fill(bullet, bullet + 50, nullptr);
		std::fill(enemy, enemy + 50, nullptr);

		//�������
		player = new Player(window);
		player->on_create();

		//��������ʱ��
		shoot_timer.set_wait_time(0.5);
		shoot_timer.set_callback([this]() {
			this->player->set_can_shoot(true);
		});

		//�������ɼ�ʱ��
		enemy_timer.set_wait_time(5);
		enemy_timer.set_callback([this](){
			this->can_create_enemy = true;
		});
	};

	//����ѭ��
	void on_update() {
		player->PhysicsProcess(delta);
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
		SDL_SetRenderDrawColor(window->get_render(), 255, 255, 255, 255);
		SDL_RenderClear(window->get_render());
		//�ӵ�
		BulletUpdate();
		//����
		EnemyUpdate();
		//���
		player->on_update();
		//��Ч
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
		if (keyboard_state_array[SDL_SCANCODE_W]) {
			player->SetDirectionY(-1);
		}
		else if (keyboard_state_array[SDL_SCANCODE_S]) {
			player->SetDirectionY(1);
		}
		if (keyboard_state_array[SDL_SCANCODE_A]) {
			player->SetDirectionX(-1);
		}
		else if (keyboard_state_array[SDL_SCANCODE_D]) {
			player->SetDirectionX(1);
		}

		//�ɿ�����
		if (!keyboard_state_array[SDL_SCANCODE_W] && !keyboard_state_array[SDL_SCANCODE_S]) {
			player->SetDirectionY(0);
		}
		if (!keyboard_state_array[SDL_SCANCODE_A] && !keyboard_state_array[SDL_SCANCODE_D]) {
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
		player->on_destroy();
		for (int i = 0; i < 50;i++) {
			if (bullet[i] != nullptr) {
				SDL_Log("[%d]Bullet Destroyed",i);
				bullet[i]->on_destroy();
			}
			if (enemy[i] != nullptr) {
				SDL_Log("[%d]Enemy Destroyed", i);
				enemy[i]->on_destroy();
			}
		}
		delete this;
	}

//---------��Ϸ����---------
public:
	//��������
	void CreateEnemy() {
		if (can_create_enemy) {
			//���������
			srand((unsigned int)time(NULL));
			//Ѱ�Ҵ�ŵ��˵�λ��
			for (int i = 0; i < 50; i++) {
				if (enemy[i] == nullptr) {
					SDL_Log("[%d]Enemy Created", i);
					enemy[i] = new Enemy(window);
					enemy[i]->on_create();
					enemy[i]->set_position(window->get_size().x*2, rand() % (int)(window->get_size().y * 2));
					break;
				}
			}
			can_create_enemy = false;
		}
	}

	//���˸���
	void EnemyUpdate() {
		for (int i = 0; i < 50; i++) {
			if (enemy[i] != nullptr) {
				//�ƶ��ж�
				if (enemy[i]->get_position().x > player->get_position().x) { enemy[i]->SetDirectionX(-1); }
				else if (enemy[i]->get_position().x < player->get_position().x) { enemy[i]->SetDirectionX(1); }
				if (enemy[i]->get_position().y > player->get_position().y) { enemy[i]->SetDirectionY(-1); }
				else if (enemy[i]->get_position().y < player->get_position().y) { enemy[i]->SetDirectionY(1); }
				//���������߼�
				enemy[i]->PhysicsProcess(delta);
				//������ʾ
				enemy[i]->on_update();
			}
		}
	}

	//�����ӵ��������
	void PlayerShoot() {
		if (player->get_can_shoot()) {
			//Ѱ�Ҵ���ӵ���λ��
			for (int i = 0; i < 50; i++) {
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
		for (int i = 0; i < 50; i++) {
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
		for (int i = 0; i < 50; i++) {
			if (bullet[i] != nullptr) {
				for (int j = 0; j < 50; j++) {
					if (enemy[j] != nullptr) {
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
			int num = bu->get_number();
			bu->on_destroy();
			bullet[num] = nullptr;
			SDL_Log("[%d]Bullet Destroyed", num);
			////�������
			num = en->get_number();
			en->on_destroy();
			enemy[num] = nullptr;
			SDL_Log("[%d]Bullet Destroyed", num);
			//SDL_Log("[True]check hurt used");
			return true;
			}
		//SDL_Log("[False]check hurt used");
		return false;
	}

private:
	Window* window = nullptr;						//����ָ��
	Player* player = nullptr;						//���ָ��
	const Uint8* keyboard_state_array = nullptr;	//����״ָ̬��
	Bullet* bullet[50] = { nullptr };			//�ӵ�ָ������
	Enemy* enemy[50] = { nullptr };
	Timer shoot_timer;
	Timer enemy_timer;
	float delta = 1 / FRAMERATE;
	bool can_create_enemy = false;				//�Ƿ�������ɵ���
};


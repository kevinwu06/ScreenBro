#pragma once
#include "constant.h"
#include "object.h"
#include "vector2.h"
#include "player.h"

extern SDL_Texture* img_enemy;

class Enemy : public Object
{
public:
	Enemy(Platform* plat,Player* play):platform(plat),player(play){
		random_position();
	}

	~Enemy() = default;

	//�����߼�
	void tick_physics(State state, float delta) {
		Move(ENEMY_SPEED,delta);
		return;
	}

	void Move(float speed, float delta)
	{
		//���ٶ�
		if (speed == 0) {
			velocity = Vector2(0, 0);
		}
		else {
			if (velocity.x < speed) {
				velocity.x += ACCELERATION * delta;
			}
			if (velocity.y < speed) {
				velocity.y += ACCELERATION * delta;
			}
		}

		//ԭ��λ��
		int temp_x = position.x, temp_y = position.y;

		//�ƶ�����
		if (direction.x && direction.y) {
			position.x += direction.x * velocity.x * 0.707;
			position.y += direction.y * velocity.y * 0.707;
		}
		else if (direction.x) {
			position.x += velocity.x * direction.x;
		}
		else if (direction.y) {
			position.y += velocity.y * direction.y;
		}

		//ǽ����ײ
		if (position.x + size.x > platform->get_position().x + platform->get_size().x ||
			position.x < platform->get_position().x) {
			position.x = temp_x;
		}
		if (position.y + size.y > platform->get_position().y + platform->get_size().y
			|| position.y < platform->get_position().y) {
			position.y = temp_y;
		}
	}

	void set_position(Vector2 pos) {
		position = pos;
	}

	void set_position_x(float x) {
		position.x = x;
	}

	void set_position_y(float y) {
		position.y = y;
	}

	void set_position(float x,float y) {
		position.x = x, position.y = y;
	}

	void on_create() {
		
	}

	void find_player() {
		//��������ͬһƽ̨
		if (player->get_platform() == platform) {
			//�ƶ��ж�
			if (position.x > player->get_position().x + player->get_size().x / 4) { SetDirectionX(-1); }
			else if (position.x + size.x / 4 < player->get_position().x) { SetDirectionX(1); }
			else { SetDirectionX(0); }
			if (position.y > player->get_position().y + player->get_size().y / 4) { SetDirectionY(-1); }
			else if (position.y + size.y / 4 < player->get_position().y) { SetDirectionY(1); }
			else { SetDirectionY(0); }
		}
		else {
			SetDirectionX(0);
			SetDirectionY(0); 
		}
	}

	void on_update() {
		rect = { (int)position.x,(int)position.y,50,50 };
		int rel_position_x = position.x - platform->border_left();
		int rel_position_y = position.y - platform->border_up();
		//��ʾ
		SDL_Rect show_rect = { rel_position_x, rel_position_y, (int)size.x, (int)size.y };
		SDL_RenderCopy(platform->get_render(), platform->img_enemy, NULL, &show_rect);
	}

	void on_exit() {}

	void on_destroy() {
		delete this;
	}

	void on_remove() {
		can_remove = true;
	}

	bool get_remove_state() {
		return can_remove;
	}

	Platform* get_platform(){
		return platform;
	}

private:
	//���λ��
	void random_position() {
		//���λ�������ڵ�ͼ��Ե
		int edge = rand() % 4;
		switch (edge) {
		case 0:	//�ϱ�
			set_position(platform->border_left() + rand() % (int)platform->get_size().x, platform->border_up());
			break;
		case 1: // �±�
			set_position(platform->border_left() + rand() % (int)platform->get_size().x, platform->border_down());
			break;
		case 2: // ���
			set_position(platform->border_left(), platform->border_up() + rand() % (int)platform->get_size().y);
			break;
		case 3: // �ұ�
			set_position(platform->border_right(), platform->border_up() + rand() % (int)platform->get_size().y);
			break;
		default:
			break;
		}
	}

private:
	Platform* platform = nullptr;
	Player* player = nullptr;
	int number=0; //�������е����
	bool can_remove = false;
};
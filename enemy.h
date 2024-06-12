#pragma once
#include "constant.h"
#include "object.h"
#include "vector2.h"
#include "player.h"

extern SDL_Texture* img_enemy;

class Enemy : public Object
{
public:
	Enemy(Platform* win, Player* play):window(win),player(play){}
	~Enemy() = default;

	//�����߼�
	void tick_physics(State state, float delta) {
		on_move(ENEMY_SPEED,delta);
		return;
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
		//���λ�������ڵ�ͼ��Ե
		int edge = rand() % 4;
		switch (edge) {
		case 0:	//�ϱ�
			set_position(platform->get_left_border()+rand() %(int)platform->get_size().x, platform->get_up_border());
			break;
		case 1: // �±�
			set_position(platform->get_left_border() + rand() % (int)platform->get_size().x, platform->get_down_border());
			break;
		case 2: // ���
			set_position(platform->get_left_border(), platform->get_up_border() + rand() % (int)platform->get_size().y);
			break;
		case 3: // �ұ�
			set_position(platform->get_right_border(), platform->get_up_border() + rand() % (int)platform->get_size().y);
			break;
		default:
			break;
		}
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
		int rel_position_x = position.x - window->get_left_border();
		int rel_position_y = position.y - window->get_up_border();
		//��ʾ
		rect = { rel_position_x, rel_position_y, (int)size.x, (int)size.y };
		SDL_RenderCopy(platform->get_render(), platform->img_enemy, NULL, &rect);
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

	//��ȡ�������е����
	int get_number() {
		return number;
	}

	//�������������ײ
	bool check_collision(Object* ob) {
		if (position.x+size.x >= ob->get_position().x &&
			position.x <= ob->get_position().x + ob->get_size().x &&
			position.y+size.y >= ob->get_position().y &&
			position.y <= ob->get_position().y + ob->get_size().y)
		{
			return true;
		}
		return false;
	}

	// ������ײ������&���ˣ�
	void handle_collision(Enemy* ob) {
		//ԭ��λ��
		int temp_x = position.x, temp_y = position.y;
		int ob_temp_x = ob->get_position().x, ob_temp_y = ob->get_position().y;

		// �����������
		float dx = position.x - ob->get_position().x;
		float dy = position.y - ob->get_position().y;
		float distance = sqrt(dx * dx + dy * dy);

		if (distance == 0.0f) {
			distance = 1.0f; // �������0
		}

		float overlap = (size.x/2 + ob->get_size().x/2) - distance;

		// ������������������
		position.x += (dx / distance) * (overlap / 2);
		position.y += (dy / distance) * (overlap / 2);
		ob->set_position(ob->get_position().x - (dx / distance) * (overlap / 2), ob->get_position().y - (dy / distance) * (overlap / 2));

		//ǽ����ײ
		if (get_right_border() > platform->get_right_border() ||
			get_left_border() < platform->get_left_border()) {
			position.x = temp_x;
		}
		if (get_down_border() > platform->get_down_border()
			|| get_up_border() < platform->get_up_border()) {
			position.y = temp_y;
		}
		//��һ������ǽ����ײ
		if (ob->get_right_border() > platform->get_right_border() ||
		 ob->get_left_border() < platform->get_left_border()) {
			ob->set_position_x(ob_temp_x);
		}
		if (ob->get_down_border() > platform->get_down_border()
			||  ob->get_up_border()< platform->get_up_border()) {
			ob->set_position_y(ob_temp_y);
		}
	}

	// ������ײ������&��ң�
	void handle_collision(Player* ob) {
		//ԭ��λ��
		int temp_x = position.x, temp_y = position.y;

		// �����������
		float dx = position.x - ob->get_position().x;
		float dy = position.y - ob->get_position().y;
		float distance = sqrt(dx * dx + dy * dy);

		if (distance == 0.0f) {
			distance = 1.0f; // �������0
		}

		float overlap = (size.x / 2 + ob->get_size().x / 2) - distance;

		// ������������Һ͵���
		position.x += (dx / distance) * (overlap);
		position.y += (dy / distance) * (overlap);
		//ob->set_position(ob->get_position().x - (dx / distance) * (overlap / 2), ob->get_position().y - (dy / distance) * (overlap / 2));

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

private:
	Platform* window = nullptr;
	Player* player = nullptr;
	int number=0; //�������е����
	bool can_remove = false;
};
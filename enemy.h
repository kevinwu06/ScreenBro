#pragma once
#include "constant.h"
#include "object.h"
#include "vector2.h"

extern SDL_Texture* img_enemy;

class Enemy : public Object
{
public:
	Enemy(Window* win):window(win){}
	~Enemy() = default;

	//�����߼�
	void tick_physics(State state, float delta) {
		on_move(ENEMY_SPEED,delta);
		return;
	}

	void set_position(Vector2 pos) {
		position = pos;
	}

	void set_position(float x,float y) {
		position.x = x, position.y = y;
	}

	void on_create() {
	}

	void on_update() {
		//��ʾ
		rect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
		SDL_RenderCopy(window->get_render(), img_enemy, NULL, &rect);
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
	}

	// ������ײ������&��ң�
	void handle_collision(Player* ob) {
		// �����������
		float dx = position.x - ob->get_position().x;
		float dy = position.y - ob->get_position().y;
		float distance = sqrt(dx * dx + dy * dy);

		if (distance == 0.0f) {
			distance = 1.0f; // �������0
		}

		float overlap = (size.x / 2 + ob->get_size().x / 2) - distance;

		// ������������������
		position.x += (dx / distance) * (overlap);
		position.y += (dy / distance) * (overlap);
		//ob->set_position(ob->get_position().x - (dx / distance) * (overlap / 2), ob->get_position().y - (dy / distance) * (overlap / 2));
	}

private:
	Window* window = nullptr;
	int number=0; //�������е����
	bool can_remove = false;
};
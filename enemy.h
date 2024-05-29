#pragma once
#include "constant.h"
#include "object.h"
#include "vector2.h"

extern SDL_Texture* img_player;

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
		relevant_position = position.operator-(window->get_position());
		rect = { (int)relevant_position.x, (int)relevant_position.y, (int)size.x, (int)size.y };
		SDL_RenderCopy(window->get_render(), img_player, NULL, &rect);
	}

	void on_exit() {}

	void on_destroy() {
		delete this;
	}

	//��ȡ�������е����
	int get_number() {
		return number;
	}
private:
	Window* window = nullptr;
	int number; //�������е����
};
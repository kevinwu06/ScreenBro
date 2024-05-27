#pragma once
#include "vector2.h"
#include <functional>
#include "window.h"
#include "constant.h"

extern SDL_Texture* img_player;

class Bullet {
public:
	Bullet(Window* win) :window(win) {}
	~Bullet() = default;

	void set_position(Vector2 pos) {
		position = pos.operator+(15);
	}
	void set_direction(Vector2 dir) {
		direction = dir;
	}
	void on_update() {
		relevant_position = position.operator-(window->get_position());
		position.operator+=(direction.operator*(20));
		rect = { (int)relevant_position.x, (int)relevant_position.y, (int)size.x, (int)size.y };
		SDL_RenderCopy(window->get_render(), img_player, NULL, &rect);

		if (position.x + size.x > 2 * window->get_size().x || position.x < 0)can_remove=1;
		if (position.y + size.y > 2 * window->get_size().y || position.y < 0)can_remove=1;
	}

	void on_destroy() {
		delete this;
	}

	bool get_remove_state() {
		return can_remove;
	}

private:
	Vector2 position = Vector2(200,200);		//�ӵ�λ��
	Vector2 direction = Vector2(0, 0);			//�ӵ�����
	Vector2 size = Vector2(20, 20);				//�ӵ���С
	Vector2 relevant_position = Vector2(0, 0);	//�ӵ���Դ���λ��
	int damage = 10;
	bool valid = true;
	bool can_remove = false;
	Window* window;
	SDL_Rect rect = {};
};
#pragma once
#include "constant.h"
#include "object.h"
#include "vector2.h"

extern SDL_Texture* img_player;

class Player : public Object
{
public:
	Player(Window* win) { window = win; };
	~Player()=default;

	void on_create(){
		SetCurrentState(State::IDLE);
	}

	void on_destroy()
	{
		delete this;
	}

	void tick_physics(State state, float delta) {
		// ÿ��״̬�������߼�����
		switch (state) {
		case State::IDLE:
			on_move(0,delta);
			break;
		case State::WALK:
			on_move(SPEED,delta);
			on_update();
			break;
		}
	}

	State get_next_state(State current) {
		// ��ȡ��һ��״̬���߼�
		switch (current) {
		case State::IDLE:
			if (direction.x != 0 || direction.y != 0) {
				SDL_Log("[PLAYER] IDLE -> WALK");
				return State::WALK;
			}
			break;
		case State::WALK:
			if (direction.x == 0 && direction.y == 0) {
				SDL_Log("[PLAYER] WALK -> IDLE");
				return State::IDLE;
			}
			break;
		}
		return current;
	}

	void transition_state(State current, State next) {
		// ״̬ת���߼�:��ɫ�����任
		switch (current) {
		case State::IDLE:
			break;
		case State::WALK:
			break;
		}
	}

	void on_update()
	{
		//�����ʾ
		relevant_position=position.operator-(window->get_position());
		rect = { (int)relevant_position.x, (int)relevant_position.y, (int)size.x, (int)size.y };
		SDL_Point p{ 25,25 };
		SDL_RenderCopyEx(window->get_render(), img_player, NULL, &rect, angle, &p, SDL_FLIP_NONE);

		//�����ƶ��ж�
		if (relevant_position.x + size.x / 2 > window->get_size().x) {
			window->on_move(window->get_size().x,0);
			window->on_update();
		}
		else if (relevant_position.x + size.x / 2 < 0) {
			window->on_move(-window->get_size().x,0);
			window->on_update();
		}
		if (relevant_position.y + size.y / 2 > window->get_size().y) {
			window->on_move(0, window->get_size().y);
			window->on_update();
		}
		else if (relevant_position.y + size.y / 2 < 0) {
			window->on_move(0, -window->get_size().y);
			window->on_update();
		}
	}

	void on_move(float speed,float delta)
	{
		//���ٶ�
		if (speed == 0) {
			velocity=Vector2(0,0);
		}
		else{
			if (velocity.x<speed) {
				velocity.x += ACCELERATION*delta;
			}
			if (velocity.y<speed) {
				velocity.y += ACCELERATION*delta;
			}
		}

		//�沿����
		if (speed) {
			face_direction.x = direction.x;
			face_direction.y = direction.y;

			if (direction.x && direction.y) {
				if (direction.x > 0) {
					angle = 90 + direction.y * 45;
				}
				else {
					angle = -90 - direction.y * 45;
				}
			}
			else if (direction.x) {
				angle = direction.x * 90;
			}
			else if (direction.y) {
				angle = 90 + direction.y * 90;
			}
		}

		//ԭ��λ�ã�����ǽ����ײ��
		float temp_x = position.x, temp_y = position.y;

		//�ƶ�����
		if (direction.x && direction.y) {
			position.x += direction.x * velocity.x * 0.707;
			position.y += direction.y * velocity.y * 0.707;
		}
		else if(direction.x) {
			position.x += velocity.x*direction.x;
		}
		else if (direction.y) {
			position.y += velocity.y*direction.y;
		}

		//ǽ����ײ
		if (position.x+size.x > 2 * window->get_size().x || position.x < 0)position.x = temp_x;
		if (position.y+size.y > 2 * window->get_size().y || position.y < 0)position.y = temp_y;
	}

	//����y�ƶ�����
	void SetDirectionY(float dir)
	{
		direction.y = dir;
	}

	//����x�ƶ�����
	void SetDirectionX(float dir)
	{
		direction.x = dir;
	}

	//��ȡ����泯����
	Vector2 get_direction() {
		return face_direction;
	}

	//��ȡ���λ��
	Vector2 get_position() {
		return position;
	}

	//�����Ƿ���Լ������
	void set_can_shoot(bool val) {
		can_shoot = val;
	}

	//��ȡ�Ƿ���Լ������
	bool get_can_shoot() {
		return can_shoot;
	}
private:
	bool can_shoot = 1;							//�Ƿ�������						
};
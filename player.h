#pragma once
#include "constant.h"
#include "object.h"
#include "vector2.h"
#include "screen_window.h"

class Player : public Object
{
public:
	bool is_player = 1;
public:
	Player(ScreenWindow* win) : window(win){ 
		position = Vector2(50, 50);
	};
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
			Move(0,delta);
			break;
		case State::WALK:
			Move(SPEED,delta);
			on_update();
			break;
		}
	}

	State get_next_state(State current) {
		// ��ȡ��һ��״̬���߼�
		switch (current) {
		case State::IDLE:
			if (direction.x != 0 || direction.y != 0) {
				//SDL_Log("[PLAYER] IDLE -> WALK");
				return State::WALK;
			}
			break;
		case State::WALK:
			if (direction.x == 0 && direction.y == 0) {
				//SDL_Log("[PLAYER] WALK -> IDLE");
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
		//�������
		rect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
		SDL_Point p{ size.x/2,size.y/2 };
		SDL_RenderCopyEx(window->get_render(), window->img_player, NULL, &rect, angle, &p, SDL_FLIP_NONE);
	}

	void Move(float speed,float delta)
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
		if (border_right() > platform->border_right() ||
			border_left() < platform->border_left()) {
			position.x = temp_x;
		}
		if (border_down() > platform->border_down()
			|| border_up() < platform->border_up()) {
			position.y = temp_y;
		}
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

	//��������ƽ̨
	void set_platform(Platform* plat) {
		platform = plat;
		platform->raise_window();
	}
	
	bool change_platform(Platform* plat) {
		//ǽ����ײ
		if (border_right() > plat->border_right() ||
			border_left() < plat->border_left() ||
			border_down() > plat->border_down() ||
			border_up() < plat->border_up()) {
			return false;
		}
		else { 
			set_platform(plat);
			return true;
		}
	}

private:
	ScreenWindow* window=nullptr;
	bool can_shoot = 1;							//�Ƿ�������
};
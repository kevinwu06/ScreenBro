#pragma once
#include <functional>
#include "constant.h"
class Timer {
public:
	Timer() = default;
	~Timer() = default;

	//����
	void on_update(float delta) {
		if (paused) return;

		pass_time += delta;
		if (pass_time >= wait_time) {
			if ((!one_shot || (one_shot && !shotted))&&callback) {
				callback();
			}
			shotted = true;
			pass_time = 0;
		}
	}

	//���ûص�����
	void set_callback(std::function<void()> callback) {
		this->callback = callback;
	}

	//��ʼ
	void start() {
		pass_time = 0;
		shotted = false;
	}

	//���ô������(s)
	void set_wait_time(float val) {
		wait_time = val;
	}

	//�Ƿ�Ϊһ����
	void set_one_shot(bool flag) {
		one_shot = true;
	}

	//��ͣ
	void pause() {
		paused = true;
	}

	//����
	void resume() {
		paused = false;
	}

private:
	std::function<void()> callback;
	float pass_time=0;
	bool shotted=true;
	bool one_shot = false;
	float wait_time=0;
	bool paused = false;
};
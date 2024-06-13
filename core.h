#pragma once
#include "constant.h"
#include "object.h"

//��ײ���
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_HasIntersection(&a, &b);
}

// ������ײ������&���ˣ�
void handleCollision(Object& a, Object& b) {
	//ԭ��λ��
	float ax = a.position.x, ay = a.position.y;
	float bx = b.position.x, by = b.position.y;

	// �����������
	float dx = a.position.x - b.position.x;
	float dy = a.position.y - b.position.y;
	float distance = sqrt(dx * dx + dy * dy);

	if (distance == 0.0f) {
		distance = 1.0f; // �������0
	}

	float overlap = (a.size.x / 2 + b.size.x / 2) - distance;

	// ������������������
	a.position.x += (dx / distance) * (overlap / 2);
	a.position.y += (dy / distance) * (overlap / 2);
	if (b.is_player!=1) {
		b.position.x -= (dx / distance) * (overlap / 2);
		b.position.y -= (dy / distance) * (overlap / 2);
	}

	//ǽ����ײ

}
	
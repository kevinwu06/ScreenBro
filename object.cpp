#include "object.h"

Object::Object(float x, float y, float width, float height)
    : m_x(x), m_y(y), m_width(width), m_height(height) {

}

Object::~Object() {}

void Object::Update() {
    // ���¶����߼�
}

void Object::Render(SDL_Renderer* render) {
    // ��Ⱦ����
 
    //���ƾ���
    rect = { (int)m_x, (int)m_y, (int)m_width, (int)m_height };
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);//���ģʽ
    SDL_SetRenderDrawColor(render, 255, 0, 0, 100);//͸����100
    SDL_RenderDrawRect(render, &rect);
}

void Object::Move()
{
}

bool Object::CheckCollision(const Object& other) {
    // �򵥵ľ�����ײ���
    if (m_x < other.m_x + other.m_width &&
        m_x + m_width > other.m_x &&
        m_y < other.m_y + other.m_height &&
        m_y + m_height > other.m_y) {
        return true; // ������ײ
    }
    return false; // δ������ײ
}

//---------״̬��----------
void Object::SetCurrentState(State state) {
    transition_state(current_state, state);
    current_state = state;
    state_time = 0.0f;
}

void Object::Ready() {
    // �������������ӳ�ʼ���߼�
    SetCurrentState(IDLE);
}

void Object::PhysicsProcess(float delta) {
    while (true) {
        State next = get_next_state(current_state);
        if (current_state == next) {
            break;
        }
        current_state = next;
        SetCurrentState(current_state);
    }
    tick_physics(current_state, delta);
    state_time += delta;
}
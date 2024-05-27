#pragma once
#include "constant.h"
#include "window.h"

//�������
class Object
{
public:
    enum class State {
        IDLE,
        WALK,
    };

    Object() = default;
    ~Object()=default;

    // ������ײ���
    //bool CheckCollision(const Object& other) {
    //    
    //    if (position.x < other.position.x + other.m_width &&
    //        position.x + m_width > other.position.x &&
    //        position.y < other.position.y + other.m_height &&
    //        position.y + m_height > other.position.y) {
    //        return true; // ������ײ
    //    }
    //    return false; // δ������ײ
    //}

    //����״̬
    void SetCurrentState(State state) {
        transition_state(current_state, state);
        current_state = state;
        state_time = 0.0f;
    }

    //״̬��ѭ��
    void PhysicsProcess(float delta) {
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

protected:
    float m_width=50;
    float m_height=50;

    //״̬��
    State current_state=State::IDLE;
    float state_time=0;
    //״̬�������߼�
    virtual void tick_physics(State state, float delta) { return; }
    // ��ȡ��һ��״̬���߼�
    virtual State get_next_state(State current){return current;}
    // ״̬ת���߼�
    virtual void transition_state(State current, State next) { return; }
};


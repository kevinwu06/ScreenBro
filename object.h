#pragma once
#include "resource.h"

//�������
class Object
{
public:
    Object(float x, float y, float width, float height);
    ~Object();

    //����
    virtual void Update(); // ���¶����߼�
    virtual void Render(SDL_Renderer* renderer);

    //��ײ���
    bool CheckCollision(const Object& other);

    //״̬��
    void SetCurrentState(State state);
    void Ready();
    void PhysicsProcess(float delta);

protected:
    float m_x;
    float m_y;
    float m_width;
    float m_height;
    SDL_Rect rect;

    //״̬��
    State current_state;
    float state_time;
    //״̬�������߼�
    virtual void tick_physics(State state, float delta) {}
    // ��ȡ��һ��״̬���߼�
    virtual State get_next_state(State current){return current;}
    // ״̬ת���߼�
    virtual void transition_state(State current, State next){}
};


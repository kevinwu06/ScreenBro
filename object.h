#pragma once
#include "resource.h"

class Object
{
public:
    Object(float x, float y, float width, float height);
    ~Object();

    virtual void Update();
    virtual void Render(SDL_Renderer* renderer);
    void Move();

    bool CheckCollision(const Object& other);

    //---------״̬��---------
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

    virtual void tick_physics(State state, float delta) {
    }
    //״̬�������߼�

    virtual State get_next_state(State current){
        return current;
    }
    // ���������ӻ�ȡ��һ��״̬���߼�

    virtual void transition_state(State current, State next) {

    }
    // ����������״̬ת���߼�
};


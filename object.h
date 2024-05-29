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

    //��ʼ��
    virtual void on_create() {
    
    }

    //����
    virtual void on_update() {
    }

    //�˳�
    virtual void on_exit(){
    
    }

    //�ͷ�
    virtual void on_destroy(){
        delete this;
    }

    void on_move(float speed, float delta)
    {
        //���ٶ�
        if (speed == 0) {
            velocity = Vector2(0, 0);
        }
        else {
            if (velocity.x < speed) {
                velocity.x += ACCELERATION * delta;
            }
            if (velocity.y < speed) {
                velocity.y += ACCELERATION * delta;
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

        //�ƶ�����
        if (direction.x && direction.y) {
            position.x += direction.x * velocity.x * 0.707;
            position.y += direction.y * velocity.y * 0.707;
        }
        else if (direction.x) {
            position.x += velocity.x * direction.x;
        }
        else if (direction.y) {
            position.y += velocity.y * direction.y;
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

    //��ȡ�泯����
    Vector2 get_direction() {
        return face_direction;
    }

    //��ȡλ��
    Vector2 get_position() {
        return position;
    }

    //��ȡ��С
    Vector2 get_size() {
        return size;
    }

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

protected:
    Vector2 size = Vector2(50, 50);				//��С
    Vector2 velocity = Vector2(0, 0);			//�ٶ�
    Vector2 position = Vector2(0, 0);			//λ��
    Vector2 window_position = Vector2(0, 0);	//����λ��
    Vector2 relevant_position = Vector2(0, 0);	//�����Դ��ڵ�λ��
    SDL_Rect rect = {50,50};                    //ͼ����ʾ����
    Window* window = nullptr;
    Vector2 face_direction = Vector2(0, 0);		//����泯����
    Vector2 direction = Vector2(0, 0);			//����ƶ�����
    float angle = 0;							//���ͼ���泯�Ƕ�

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


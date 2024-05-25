#pragma once
#include "resource.h"
#include "object.h"

class Player : public Object
{
public:
    Player(float x, float y, float w, float h);
    ~Player();

    //���ں���Ⱦ��
    void CreateWindow();
    void CreateRender();
    SDL_Window* GetWindow();
    SDL_Renderer* GetRender();

    //�ƶ�����
    void Move(float speed);
    void SetDirectionY(float dir);
    void SetDirectionX(float dir);

    //״̬��
    void tick_physics(State state, float delta);
    State get_next_state(State current) override;
    void transition_state(State current, State next);

    //�����߼�
    void Update();


private:
    float v_x, v_y; //�ٶ�
    float dir_x, dir_y; //����
    float angle; //�Ƕ�
    int win_x, win_y;
    SDL_RendererFlip flip;
    SDL_Window *win;
    SDL_Renderer* render;
    SDL_Texture* texture;
};
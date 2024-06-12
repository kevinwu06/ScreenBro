#pragma once
#include "scene.h"
#include "scene_manager.h"

extern SceneManager scene_manager;
extern SDL_Texture *img_menu;

class MenuScene :public Scene {
public:
	MenuScene(Window* win) :window(win) {}
	~MenuScene() = default;

	void on_enter() {
		SDL_Log("Enter Menu Scene");
	};
	void on_update() {
	};
	void on_draw() {
		//����ɫ
		SDL_SetRenderDrawColor(window->get_render(), 0, 0, 0, 255);
		SDL_RenderClear(window->get_render());

		SDL_Rect rect{0,0,window->get_size().x,window->get_size().y};
		SDL_RenderCopy(window->get_render(), img_menu, 0, &rect);
		SDL_RenderPresent(window->get_render());//��Ч
	};
	void on_input() {
		//�¼�
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = 0;
					break;
				case SDLK_SPACE:
					scene_manager.switch_to(SceneManager::SceneType::Game);
					break;
				default:
					break;
				}
			}
		}
	};
	void on_exit() {
		return;
	};
	void on_destroy() {
		delete this;
	}
private:
	Window* window = nullptr;
};
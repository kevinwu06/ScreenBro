#pragma once
#include "constant.h"
#include "vector2.h"
#include "windows.h"
#include <SDL_syswm.h> //用于窗口透明
#include "SDL_ttf.h"

class Window {
public:
	Window() {
		//获取屏幕大小
		SDL_DisplayMode displayMode;
		if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
			SDL_Log("SDL_GetCurrentDisplayMode Error: %s\n", SDL_GetError());
			return;
		}
		screenWidth = displayMode.w;
		screenHeight = displayMode.h;

		position = Vector2(100.0, 100.0);
		size = Vector2(960.0, 540.0);

		//窗口
		window = SDL_CreateWindow(
			"WindowShooter",
			position.x, position.y,
			size.x, size.y, SDL_WINDOW_SHOWN
		);
		if (window == NULL) {
			SDL_Log("Cannot create window,%s", SDL_GetError());
			return;
		}
		//渲染器
		render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (render == NULL) {
			SDL_Log("Cannot create renderer,%s", SDL_GetError());
			return;
		}

		return;
	};
	~Window()=default;
	
	void on_enter() {
		SDL_ShowWindow(window);
	}

	void on_exit() {
		SDL_HideWindow(window);
	}

	void on_destroy() {
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(render);
		delete this;
	}
	
	Vector2 get_position() {
		return position;
	}

	Vector2 get_size(){
		return size;
	}

	int get_screen_width() {
		return screenWidth;
	}

	int get_screen_height() {
		return screenHeight;
	}

	SDL_Window* get_window() { return window; }
	SDL_Renderer* get_render() { return render; }

private:
	SDL_Window* window;
	SDL_Renderer* render;
	Vector2 size = Vector2(0, 0);
	Vector2 position = Vector2(0, 0);
	int screenWidth;
	int screenHeight;
};
#pragma once
#include "constant.h"
#include "vector2.h"
#include "windows.h"
#include <SDL_syswm.h> //���ڴ���͸��
#include "SDL_ttf.h"

class Window {
public:
	Window()=default;
	~Window()=default;
	void on_create() {
		//SDL��ʼ��
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			SDL_Log("Init failed, %s", SDL_GetError());
			return;
		}

		//��ȡ��Ļ��С
		SDL_DisplayMode displayMode;
		if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
			SDL_Log("SDL_GetCurrentDisplayMode Error: %s\n", SDL_GetError());
			return;
		}
		screenWidth = displayMode.w;
		screenHeight = displayMode.h;

		size = Vector2((float)screenWidth, (float)screenHeight);

		//����
		window = SDL_CreateWindow(
			"WindowShooter",
			position.x, position.y,
			size.x, size.y, SDL_WINDOW_BORDERLESS |SDL_WINDOW_SHOWN
		);
		if (window == NULL) {
			SDL_Log("Cannot create window,%s", SDL_GetError());
			return;
		}
		//��Ⱦ��
		render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (render == NULL) {
			SDL_Log("Cannot create renderer,%s", SDL_GetError());
			return;
		}


		// ʹ����ʼ���ö�
		SDL_SetWindowAlwaysOnTop(window, SDL_TRUE);
		//����͸��
		SetTransparent(window, RGB(0, 0, 0));


		//SDL_IMG��ʼ��
		if (!IMG_Init(IMG_INIT_PNG)) {
			SDL_Log("Cannot init img,%s", SDL_GetError());
			return;
		}

		//SDL_TTF ��ʼ��
		TTF_Init();

		return;
	}

	void on_update() {
		SDL_SetWindowPosition(window, position.x, position.y);
	}

	void on_destroy() {
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(render);
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
		delete this;
	}
	void on_move(float x,float y) {
		position.x += x;
		position.y += y;
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

	void SetTransparent(SDL_Window* window, COLORREF colorKey) {
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(window, &wmInfo);

		HWND hwnd = wmInfo.info.win.window;
		LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hwnd, colorKey, 0, LWA_COLORKEY);
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
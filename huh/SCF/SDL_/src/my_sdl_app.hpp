

#ifndef MY_SDL_APP
#define MY_SDL_APP

#include "SDL.h"

struct my_sdl_app
{
	bool active;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	struct Window
	{
		const char* title = nullptr;
		int width = 0;
		int height = 0;
	} window_;

	struct Time
	{
		float fps_dt = 0;
		float dt = 0;
		Uint32 ticks = 0;

		void Update()
		{
			dt = float((SDL_GetTicks() - ticks) / 1000.0f);
			ticks = SDL_GetTicks();
			if (dt < fps_dt) SDL_Delay(fps_dt - dt);
		}
	} time;

	struct Camera
	{
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float offsetx = 0.0f;
		float offsety = 0.0f;

		void SetOffset(int mouse_x, int mouse_y)
		{
			offsetx = mouse_x - x * scale;
			offsety = mouse_y - y * scale;
		}
		void FollowOffset(int mouse_x, int mouse_y)
		{
			x = (mouse_x - offsetx) / scale;
			y = (mouse_y - offsety) / scale;
		}
	} camera;

	struct Input
	{
		const int key_map[2][4] = { 0,3,3,0,1,2,2,1 };
		struct Mouse
		{
			int x = 0;
			int y = 0;
			char buttons[5]{ 0,0,0,0,0 };
			Sint32 wheely = 0;
		} mouse;
		char* keyboard = (char*)calloc(200, sizeof(char));
		SDL_Event event;

		bool Update()
		{
			SDL_GetMouseState(&mouse.x, &mouse.y);

			for (auto& i : mouse.buttons)
			{
				if (i == 1) i = 2;
				if (i == 3) i = 0;
			}

			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_QUIT: return false; break;
				case SDL_MOUSEBUTTONDOWN: mouse.buttons[event.button.button] = 1; break;
				case SDL_MOUSEBUTTONUP: mouse.buttons[event.button.button] = 3; break;
				case SDL_MOUSEWHEEL: mouse.wheely = event.wheel.y; break;
				}
			}

			const Uint8* keys = SDL_GetKeyboardState(0);
			for (int i = 0; i < 200; ++i) keyboard[i] = key_map[(int)(keys[i])][keyboard[i]];

			return true;
		}
	} input;

	my_sdl_app(const char* window_title, int window_width, int window_height, float fps)
	{
		SDL_Init(SDL_INIT_VIDEO);
		active = true;
		window_ = { window_title,window_width,window_height };
		window = SDL_CreateWindow(window_.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_.width, window_.height, 0);
		renderer = SDL_CreateRenderer(window, -1, 0);
		time.fps_dt = 1000.0f / fps;
		time.ticks = SDL_GetTicks();
	}

	~my_sdl_app()
	{
		free(input.keyboard);

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	bool Update()
	{
		time.Update();

		active = input.Update();

		SDL_RenderSetScale(renderer, camera.scale, camera.scale);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		return active;
	}

};

#endif


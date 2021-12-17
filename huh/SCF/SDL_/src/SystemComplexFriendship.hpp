

#ifndef SYSTEM_COMPLEX_FRINDSHIP
#define SYSTEM_COMPLEX_FRINDSHIP

#include "my_sdl_app.hpp"
#include <list>

struct SystemComplexFriendship
{
	my_sdl_app* app = nullptr;

	std::list<SDL_Rect> rects;
	struct plink { SDL_Rect* a, * b; };
	std::list<plink> links;

	SDL_Rect* grabbed = nullptr;
	SDL_Point grabbed_offset{ 0,0 };

	SDL_Rect* linked = nullptr;
	SDL_Point linked_offset{ 0,0 };

	SystemComplexFriendship()
	{
		app = new my_sdl_app("demo", 1280, 720, 60.0f);//MOUSE SDLPOINT AND 1280X720
	}

	~SystemComplexFriendship()
	{
		rects.clear();
		delete app;
		app = nullptr;
	}

	bool Update()
	{
		app->Update();

		SDL_Point mouse{ app->input.mouse.x,app->input.mouse.y };

		if (app->input.keyboard[SDL_SCANCODE_R] == 1)
		{
			rects.clear();
			links.clear();
		}

		if (app->input.mouse.buttons[SDL_BUTTON_MIDDLE] == 1)
		{
			for (auto& i : rects)
			{
				if (SDL_PointInRect(&mouse, &i))
				{
					linked = &i;
					linked_offset = { mouse.x - i.x,mouse.y - i.y };
					break;
				}
			}
		}
		if (app->input.mouse.buttons[SDL_BUTTON_MIDDLE] == 2 && linked != nullptr)
		{
			SDL_SetRenderDrawColor(app->renderer, 255, 255, 0, 255);
			SDL_RenderDrawLine(app->renderer, (*linked).x + linked_offset.x, (*linked).y + linked_offset.y, mouse.x, mouse.y);
		}
		if (app->input.mouse.buttons[SDL_BUTTON_MIDDLE] == 3 && linked != nullptr)
		{
			for (auto& i : rects)
			{
				if (SDL_PointInRect(&mouse, &i) && &i != linked)
				{
					links.push_back({ linked,&i });
					break;
				}
			}
			linked = nullptr;
		}

		if (app->input.mouse.buttons[SDL_BUTTON_LEFT] == 1 && grabbed == nullptr)
		{
			for (auto& i : rects)
			{
				if (SDL_PointInRect(&mouse, &i))
				{
					grabbed = &i;
					grabbed_offset = { mouse.x - i.x,mouse.y - i.y };
				}
			}
		}
		if (app->input.mouse.buttons[SDL_BUTTON_LEFT] == 2 && grabbed != nullptr)
		{
			(*grabbed).x = app->input.mouse.x - grabbed_offset.x;
			(*grabbed).y = app->input.mouse.y - grabbed_offset.y;
		}
		if (app->input.mouse.buttons[SDL_BUTTON_LEFT] == 3 && grabbed != nullptr)
		{
			grabbed = nullptr;
		}

		if (app->input.mouse.buttons[SDL_BUTTON_RIGHT] == 1)
		{
			rects.push_back({ app->input.mouse.x - 50,app->input.mouse.y - 50,100,100 });
		}

		for (auto& i : rects)
		{
			if (&i == grabbed)
			{
				SDL_SetRenderDrawColor(app->renderer, 0, 255, 255, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
			}
			//SDL_RenderFillRect(app->renderer, &i);
			//SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(app->renderer, &i);
		}

		SDL_SetRenderDrawColor(app->renderer, 0, 255, 0, 255);
		for (auto& i : links)
		{
			SDL_RenderDrawLine(app->renderer, i.a->x + i.a->w / 2, i.a->y + i.a->h / 2, i.b->x + i.b->w / 2, i.b->y + i.b->h / 2);
		}

		SDL_RenderPresent(app->renderer);

		return app->active;
	}
};

#endif


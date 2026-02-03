/**
 * How to compile with CMake:
 * 1. mkdir build
 * 2. cd build
 * 3. cmake .. -G "MinGW Makefiles"
 * 4. cmake --build .
 */

#include <SDL3/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void print_surface_format(SDL_Surface *surface)
{
	SDL_PixelFormat format = surface->format;
	printf("Pixel format code in hexadecimal: 0x%xu\n", format);
}

void print_random_data(SDL_Surface *surface)
{
	int num_pixels = surface->w * surface->h;
	uint32_t *pixels = (uint32_t*) surface->pixels;
	for(int i=0; i<num_pixels; i++)
	{
		pixels[i] = (uint32_t) rand();
	}

}
int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow(
			"Broken TV",
		       	600,
		       	600,
		       	SDL_WINDOW_RESIZABLE);
	SDL_Surface *surface = SDL_GetWindowSurface(window);
//	print_surface_format(surface);
	SDL_Event event;
	bool running = 1;
	while(running)
	{
		SDL_PollEvent(&event);
		if(event.type == SDL_EVENT_QUIT) running = 0;
		print_random_data(surface);
		SDL_UpdateWindowSurface(window);
		SDL_Delay(100);
	}	


	return 0;
}

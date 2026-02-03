#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdint.h>

#define WIDTH 800
#define HEIGHT 800
#define x0 WIDTH/2.0
#define y0 HEIGHT/10.0
#define POINTS 0xffffU
#define GRAVITY 2.0

double r1 = 100.0;
double r2 = 250.0;
double m1 = 5.0;
double m2 = 1.0;
double theta1 = 3*SDL_PI_D/2;
double theta2 = SDL_PI_D/2;
double theta1_vel = 0.0;
double theta2_vel = 0.0;

uint32_t point = 0;

double calc_theta1_acc()
{
	float num1 = -GRAVITY*(2*m1+m2)*SDL_sin(theta1);
	float num2 = -m2*GRAVITY*SDL_sin(theta1-2*theta2);
	float num3 = -2*SDL_sin(theta1-theta2)*m2;
	float num4 = theta2_vel*theta2_vel*r2;
	float num5 = theta1_vel*theta1_vel*r1*SDL_cos(theta1-theta2);
	float den = r1*(2*m1+m2-m2*SDL_cos(2*theta1-2*theta2));

	return (num1+num2+num3*(num4+num5))/den;
}

double calc_theta2_acc()
{
	float num1 = 2*SDL_sin(theta1-theta2); 
	float num2 = theta1_vel*theta1_vel*r1*(m1+m2);
	float num3 = GRAVITY*(m1+m2)*SDL_cos(theta1);
	float num4 = theta2_vel*theta2_vel*r2*m2*SDL_cos(theta1-theta2);
	float den = r2*(2*m1+m2-m2*SDL_cos(2*theta1-2*theta2));

	return (num1*(num2+num3+num4))/den;
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

static void draw_filled_circle(SDL_Renderer *r, int cx, int cy, int radius)
{
	for(int dy = -radius; dy <= radius; dy++) {
		int dx = (int)SDL_sqrt((float)(radius*radius - dy*dy));
		SDL_RenderLine(r, cx-dx, cy+dy, cx+dx, cy+dy);
	}
}

static void clearAndResetFrame(SDL_Renderer *r)
{
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
}

void show_double_pendulum(SDL_Renderer *r, SDL_FPoint* trace)
{
	SDL_RenderLine(r, x0, y0, 
			x0+r1*SDL_sin(theta1),
			y0+r1*SDL_cos(theta1));
	draw_filled_circle(r, 
			x0+r1*SDL_sin(theta1),
		       	y0+r1*SDL_cos(theta1),
		       	10);
	SDL_RenderLine(r,
			x0+r1*SDL_sin(theta1),
		       	y0+r1*SDL_cos(theta1),
			(x0+r1*SDL_sin(theta1))+r2*SDL_sin(theta2),
			(y0+r1*SDL_cos(theta1))+r2*SDL_cos(theta2));
	draw_filled_circle(r, 
			(x0+r1*SDL_sin(theta1))+r2*SDL_sin(theta2),
			(y0+r1*SDL_cos(theta1))+r2*SDL_cos(theta2),
		       	10);
	trace[point].x = (x0+r1*SDL_sin(theta1))+r2*SDL_sin(theta2);
	trace[point].y = (y0+r1*SDL_cos(theta1))+r2*SDL_cos(theta2);
	point += 1;
	SDL_RenderLines(r, trace, point);
	theta1_vel += calc_theta1_acc();
	theta2_vel += calc_theta2_acc();
	theta1 += theta1_vel;
	theta2 += theta2_vel;
	SDL_RenderPresent(r);
}


int main()
{
	double x1 = x0+r1*SDL_sin(theta1);
	double y1 = y0+r1*SDL_cos(theta1);
	SDL_FPoint trace[POINTS] = {0};

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *w;
	SDL_Renderer *r;
	SDL_Surface *s;
	SDL_Texture *t;
	SDL_Event event;

	SDL_CreateWindowAndRenderer(
			"Double Pendulum",
		       	WIDTH,
		       	HEIGHT,
		       	SDL_WINDOW_RESIZABLE,
		       	&w,
		       	&r);

	clearAndResetFrame(r);


	while(1)
	{
		SDL_PollEvent(&event);
		if(event.type == SDL_EVENT_QUIT) break;
		show_double_pendulum(r, trace);
		SDL_Delay(10);
		clearAndResetFrame(r);
	}

	SDL_DestroyTexture(t);
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);

	SDL_Quit();
	
	return 0;
}

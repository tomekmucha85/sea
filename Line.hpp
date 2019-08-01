#ifndef LINE_HPP
#define LINE_HPP
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <vector>
#include <stdexcept>
#include <Screen.hpp>
#include <VisualComponent.hpp>


class Line : public VisualComponent
{
    private:
		Coordinates start = {0,0};
		Coordinates end = {0,0};
		Coordinates center = {0,0};
		int angle = 0;
		SDL_Color line_fill_color;
    public:
		Line(Coordinates* ptr_my_line_start, Coordinates* ptr_my_line_end);
        void SetStart(Coordinates my_start);
        void SetEnd(Coordinates my_end);
		void SetCenter(Coordinates my_center);
		Coordinates TellCenter();
		Coordinates CalculateCenterFromLineStartAndEnd(Coordinates my_line_start, Coordinates my_line_end);
		void Render();
		void Move(double step_x, double step_y);
		void SetColor(SDL_Color my_color);
};

#endif //LINE_HPP
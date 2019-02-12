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
		Color line_fill_color;
    public:
		Line(Coordinates* ptr_my_line_start, Coordinates* ptr_my_line_end);
		void Render();
		void Move(double step_x, double step_y);
		void SetColor(Color my_color);
};

#endif //LINE_HPP
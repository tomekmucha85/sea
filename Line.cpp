#include <Line.hpp>

Line::Line(Coordinates* ptr_my_line_start, Coordinates* ptr_my_line_end) : VisualComponent()
{
	SetStart(*ptr_my_line_start);
	SetEnd(*ptr_my_line_end);
	SetCenter(CalculateCenterFromLineStartAndEnd(start, end));
}

void Line::SetStart(Coordinates my_start)
{
	start = my_start;
}

void Line::SetEnd(Coordinates my_end)
{
	end = my_end;
}

void Line::SetCenter(Coordinates my_center)
{
	center = my_center;
}

Coordinates Line::TellCenter()
{
	return center;
}

Coordinates Line::CalculateCenterFromLineStartAndEnd(Coordinates my_line_start, Coordinates my_line_end)
{
	double x_cordinate = (my_line_start.x + my_line_end.x)/2;
	double y_cordinate = (my_line_start.y + my_line_end.y) / 2;
	Coordinates result = {x_cordinate, y_cordinate};
	return result;
}

void Line::Move(double step_x, double step_y)
{
	start.x += step_x;
	start.y += step_y;
	end.x += step_x;
	end.y += step_y;
}

void Line::SetColor(Color my_color)
{
	line_fill_color = my_color;
}

void Line::Render()
{
	//printf("Will render line.\n");
	SDL_SetRenderDrawColor(Screen::renderer, line_fill_color.red,
		line_fill_color.green,
		line_fill_color.blue,
		line_fill_color.alpha);
	SDL_RenderDrawLine(Screen::renderer, 
		static_cast<int>(start.x),
		static_cast<int>(start.y),
		static_cast<int>(end.x),
		static_cast<int>(end.y)
	);
	//Reset renderer color back to white
	SDL_SetRenderDrawColor(Screen::renderer, 255, 255, 255, 255);
}
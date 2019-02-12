#include <Line.hpp>

Line::Line(Coordinates* ptr_my_line_start, Coordinates* ptr_my_line_end) : VisualComponent(ptr_my_line_start, ptr_my_line_end)
{
	printf("Spawned a line!\n");
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
	SDL_SetRenderDrawColor(TellScreen()->renderer, line_fill_color.red,
		line_fill_color.green,
		line_fill_color.blue,
		line_fill_color.alpha);
	SDL_RenderDrawLine(TellScreen()->renderer, 
		static_cast<int>(start.x),
		static_cast<int>(start.y),
		static_cast<int>(end.x),
		static_cast<int>(end.y)
	);
	//Reset renderer color back to white
	SDL_SetRenderDrawColor(TellScreen()->renderer, 255, 255, 255, 255);
}
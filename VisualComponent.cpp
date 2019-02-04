#include <VisualComponent.hpp>

//####################################
// DEFINITIONS OF STATIC CLASS MEMBERS
//####################################

Screen* VisualComponent::ptr_screen = nullptr;

//#######################
// CONSTRUCTOR
//#######################

VisualComponent::VisualComponent(PreciseRect* ptr_my_position)
{
	SetPosition(*ptr_my_position);
}

VisualComponent::VisualComponent(Coordinates* ptr_my_center)
{
	SetCenter(*ptr_my_center);
}

//##########################
// SETTING & TELLING PARAMS
//##########################

VisualComponentType VisualComponent::TellMyType()
{
	return type;
}

void VisualComponent::SetMyType(VisualComponentType my_type)
{
	type = my_type;
}

Screen* VisualComponent::TellScreen()
{
	return ptr_screen;
}

void VisualComponent::SetScreen(Screen *ptr_my_screen)
{
	ptr_screen = ptr_my_screen;
}

void VisualComponent::SetPosition(PreciseRect my_position)
{
	position = my_position;
}

void VisualComponent::SetPositionX(int new_x)
{
	position.x = new_x;
}

void VisualComponent::SetPositionY(int new_y)
{
	position.y = new_y;
}

void VisualComponent::SetPositionW(int new_w)
{
	position.w = new_w;
}

void VisualComponent::SetPositionH(int new_h)
{
	position.h = new_h;
}

void VisualComponent::SetCenter(Coordinates my_center)
{
	center = my_center;
}

void VisualComponent::SetAngleDegrees(int my_angle)
{
	angle = Angle::NormalizeAngle(my_angle);
}

void VisualComponent::TurnByAngleDegrees(int my_angle)
{
	angle += my_angle;
	angle = Angle::NormalizeAngle(angle);
}


//#####################
// UTILITIES
//#####################

SDL_Rect VisualComponent::ConvertPreciseRectToSdlRect(PreciseRect my_rect)
{
	SDL_Rect result =
	{
		static_cast<int>(my_rect.x),
		static_cast<int>(my_rect.y),
		static_cast<int>(my_rect.w),
		static_cast<int>(my_rect.h)
	};
	return result;
}

SDL_Point VisualComponent::ConvertCoordinatesToSdlPoint(Coordinates my_coordinates)
{
	SDL_Point result =
	{
		static_cast<int>(my_coordinates.x),
		static_cast<int>(my_coordinates.y),
	};
	return result;
}

PreciseRect VisualComponent::ConvertSdlRectToPreciseRect(SDL_Rect my_rect)
{
	PreciseRect result =
	{
		static_cast<double>(my_rect.x),
		static_cast<double>(my_rect.y),
		static_cast<double>(my_rect.w),
		static_cast<double>(my_rect.h),
	};
	return result;
}

//#####################
// VIRTUAL METHODS
//#####################

void VisualComponent::SetColor(Color my_color)
{
	printf("Default implementation of SetColor called!\n");
}

void VisualComponent::SetClipAccordingToWallType(WallType my_type)
{
	printf("Default implementation of SetClipAccordingToWallType called.\n");
}
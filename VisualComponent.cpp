#include <VisualComponent.hpp>

//####################################
// DEFINITIONS OF STATIC CLASS MEMBERS
//####################################

Screen* VisualComponent::ptr_screen = nullptr;

//#######################
// CONSTRUCTOR
//#######################

VisualComponent::VisualComponent()
{
	Logger::Log("Spawned a visual component", debug_full);
}

/*
VisualComponent::VisualComponent(PreciseRect* ptr_my_position)
{
	//SetPosition(*ptr_my_position);
}

VisualComponent::VisualComponent(Coordinates* ptr_my_center)
{
	//SetCenter(*ptr_my_center);
}

VisualComponent::VisualComponent(Coordinates* ptr_my_start, Coordinates* ptr_my_end)
{
	//SetStart(*ptr_my_start);
	//SetEnd(*ptr_my_end);
}
*/
//##########################
// DESTRUCTOR
//##########################

VisualComponent::~VisualComponent()
{
	//Virtual destructor.
	;
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

//#####################
// UTILITIES
//#####################

SDL_Rect VisualComponent::CheckTextureDimensions(SDL_Texture* ptr_my_texture)
{
	int w, h;
	//printf("Texture address is %p", ptr_my_texture);
	SDL_QueryTexture(ptr_my_texture, NULL, NULL, &w, &h);
	SDL_Rect result = { 0,0,w,h };
	return result;
}

PreciseRect VisualComponent::CalculatePositionAroundCenter(Coordinates my_center, 
	double width,
	double height)
{
	//#TODO - ucywilizowaæ
	double calculated_position_x = static_cast<int>(my_center.x - (width / 2));
	double calculated_position_y = static_cast<int>(my_center.y - (height / 2));
	double calculated_position_w = width;
	double calculated_position_h = height;
	//printf("Position calculated around center (center x: %f y: %f): x: %f y: %f w: %f h: %f\n",center.x, center.y, position.x, position.y, position.w, position.h);
	return { calculated_position_x, calculated_position_y, calculated_position_w, calculated_position_h };
}

Coordinates VisualComponent::CalculateCenterOfGivenPosition(PreciseRect my_position)
{
	double calculated_center_x = static_cast<int>(my_position.x + (my_position.w / 2));
	double calculated_center_y = static_cast<int>(my_position.y + (my_position.h / 2));
	return { calculated_center_x, calculated_center_y };
}

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

void VisualComponent::SetAngleDegrees(int my_angle)
{
	printf("Default implementation of SetAngleDegrees called!\n");
}

void VisualComponent::TurnByAngleDegrees(int my_angle)
{
	printf("Default implementation of TurnByAngleDegrees called!\n");
}

void VisualComponent::PlayCurrentAnimation()
{
	//printf("Default implementation of PlayCurrentAnimation called!\n");
	;
}

void VisualComponent::SetCurrentAnimation(AnimationType my_animation_type)
{
	printf("Default implementation of SetCurrentAnimation called!\n");
}

void VisualComponent::SetInterruptingAnimation(AnimationType my_animation_type,
	int times_to_be_played)
{
	printf("Default implementation of SetInterruptingAnimation called!\n");
}

void VisualComponent::SetColor(Color my_color)
{
	printf("Default implementation of SetColor called!\n");
}

void VisualComponent::SetClipAccordingToWallType(WallType my_type)
{
	printf("Default implementation of SetClipAccordingToWallType called.\n");
}
#ifndef CUSTOM_DEFINED_TYPES_HPP
#define CUSTOM_DEFINED_TYPES_HPP

//Functions to be passed, executing triggered event
typedef void(*TriggeredEvent)();

struct Color
{
	int red = 255;
	int green = 255;
	int blue = 255;
	int alpha = 255;
};


enum BehaviorMode { beh_chase_hero, beh_idle };

#endif
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 25

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect Frames[MAX_FRAMES];

private:
	float current_frame;
	int LastFrame = 0;
	int Loops = 0;

public:

	void PushBack(const SDL_Rect& rect)
	{
		Frames[LastFrame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= LastFrame)
			current_frame = 0;

		return Frames[(int)current_frame];
	}
	bool Finished() const
	{
		return Loops > 0;
	}
};

#endif

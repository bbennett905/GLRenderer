#pragma once

//TODO enums for SDL event/ keystate wrapper

namespace Input
{
	//Sets the callback used to handle input
	void SetInputHandler(void (* input_callback)(const unsigned char *, double, int, int));

	//Poll events and call the input handler
	void PollEvents(double delta_time);
}
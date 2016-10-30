#pragma once

class Window;

namespace Input
{
	//Initializes input
	void InputInit(Window * window);

	//Sets the callback used to handle input
	void SetInputHandler(void (* input_callback)(const unsigned char *, double, int, int, int));

	//Poll events and call the input handler
	void PollEvents(double delta_time);
}
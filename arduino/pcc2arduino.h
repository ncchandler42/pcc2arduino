#ifndef PCC2ARDUINO_H
#define PCC2ARDUINO_H

#include "Arduino.h"
#include "Stream.h"

#define PCC_BAUD 115200

class PCController
{
public:
	PCController(Stream& ser_) 
	{ 
		ser = &ser_;
		
		for (int i = 0; i < 8; i++)
			inp[i] = 0;
	}
	
	byte 
		x_axis,
		y_axis,
		z_axis,
		r_axis,
		u_axis,
		v_axis;
		
	bool
		dpad_up,
		dpad_down,
		dpad_left,
		dpad_right,
		
		a, b, x, y,
	
		lb, rb,
	
		lt, rt,
	
		select, start,
	
		ls_press, rs_press;
		
	bool update();
	bool updateInc();
	
	void parseButtons();
	void parseAxes();
	
	void displayButtons();
	void displayAxes();
	
private:
	Stream * ser;
		
	byte inp[8];
};


	
#endif //PCC2ARDUINO_H

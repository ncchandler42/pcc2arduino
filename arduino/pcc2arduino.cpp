#include "pcc2arduino.h"

///////////////////////
// reads in bytes from serial
// returns true when the controller state is ready
///////////////////////
bool PCController::update()
{
	if (!ser || !ser->available())
		return false;
	
	//find the start of a valid parsel
	while (ser->peek() != '<')
	{
		byte b = ser->read();
		
		if (b == -1)
			return false;
	}	
	byte parsel[10];
	ser->readBytes(parsel, 10);
	
	if (parsel[0] == '<' && parsel[9] == '>')
	{
		//copies the data part of the parsel into the input, to be parsed
		memcpy(inp, parsel + 1, 8); 
		
		parseButtons();
		parseAxes();
		
		ser->write('*');
		
		return true;
	}
	
	return false;
}


bool PCController::updateInc()
{
	static int i = 0;
	static bool reading = false;
	
	if(!ser)
		return false;
	
	//Notify the system
	ser->write('*');
	
	byte b;
	
	//wait for response
	while(!ser->available());
	b = ser->read();
	
	if (i == 0 && b == '<')
	{
		reading = true;
		return false;
	}
	  
	if (reading && i >= 0 && i < 8)
	{
		inp[i] = b;
		i++;
		return false;
	}
	
	if (reading && i == 8 && b == '>')
	{
		reading = false;
		i = 0;
		
		parseButtons();
		parseAxes();
		
		return true;
	}
	
	return false;
}
/////////////////////////////////////////////////
// Parses 2 bytes into 16 buttons (bools)
/////////////////////////////////////////////////
void PCController::parseButtons()
{
	dpad_up    = inp[0] & 1;
	dpad_down  = inp[0] & 2;
	dpad_left  = inp[0] & 4;
	dpad_right = inp[0] & 8;
		
	a = inp[0] & 16;
	b = inp[0] & 32;
	x = inp[0] & 64;
	y = inp[0] & 128;
		
	lb = inp[1] & 1;
	rb = inp[1] & 2;
	
	lt = inp[1] & 4;
	rt = inp[1] & 8;
	
	select = inp[1] & 16;
	start  = inp[1] & 32;
	
	ls_press = inp[1] & 64;
	rs_press = inp[1] & 128;
}

////////////////////////////////////////////////
// Parses 12 bytes into 6 axes, 2 bytes per axis
// Serial architecture is "little-endian"
// UPDATE: axis values are now 0-255, one byte
//         ... makes conversion a lot simpler
////////////////////////////////////////////////
void PCController::parseAxes()
{
	x_axis = inp[2];
	
	y_axis = inp[3];
	
	z_axis = inp[4];
	
	r_axis = inp[5];
	
	u_axis = inp[6];
	
	v_axis = inp[7];
}

void PCController::displayButtons()
{
	if (ser)
	{
		ser->print("up:    ");
		ser->print(dpad_up);
		ser->println();
		ser->print("down:  ");
		ser->print(dpad_down);
		ser->println();
		ser->print("left:  ");
		ser->print(dpad_left);
		ser->println();
		ser->print("right: ");
		ser->print(dpad_right);
		ser->println();

		ser->print("a: ");
		ser->print(a);
		ser->println();
		ser->print("b: ");
		ser->print(b);
		ser->println();
		ser->print("x: ");
		ser->print(x);
		ser->println();
		ser->print("y: ");
		ser->print(y);
		ser->println();

		ser->print("lb: ");
		ser->print(lb);
		ser->println();
		ser->print("rb: ");
		ser->print(rb);
		ser->println();
		ser->print("lt: ");
		ser->print(lt);
		ser->println();
		ser->print("rt: ");
		ser->print(rt);
		ser->println();

		ser->print("select: ");
		ser->print(select);
		ser->println();
		ser->print("start:  ");
		ser->print(start);
		ser->println();
	
		ser->print("LS press: ");
		ser->print(ls_press);
		ser->println();
		ser->print("RS press: ");
		ser->print(rs_press);
		ser->println();
		ser->println();
	}
}
void PCController::displayAxes()
{
	if (ser)
	{
		ser->print("X axis: ");
		ser->print(x_axis);
		ser->println();

		ser->print("Y axis: ");
		ser->print(y_axis);
		ser->println();

		ser->print("Z axis: ");
		ser->print(z_axis);
		ser->println();

		ser->print("R axis: ");
		ser->print(r_axis);
		ser->println();

		ser->print("U axis: ");
		ser->print(u_axis);
		ser->println();

		ser->print("V axis: ");
		ser->print(v_axis);
		ser->println();
		ser->println();
	}
}

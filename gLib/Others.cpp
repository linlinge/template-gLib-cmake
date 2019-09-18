#include "Others.h"
V3 get_color(float min,float max,float confidence)
{
	V3 color_temp;
	float n=4.0; // 
	float step=(max-min)/n;
	float c=confidence-min;
	if(c<step)
	{
		color_temp.r=0;
		color_temp.g=confidence/step;
		color_temp.b=255;
	}
	else if(c<2*step)
	{
		color_temp.r=0;
		color_temp.g=255;
		color_temp.b=255-(confidence-step)/step;
	}
	else if(c<3*step)
	{
		color_temp.r=(confidence-2*step)/step;
		color_temp.g=255;
		color_temp.b=0;
	}
	else if(c<4*step)
	{
		color_temp.r=255;
		color_temp.g=255-(confidence-3*step)/step;
		color_temp.b=0;
	}
	
	return color_temp;
}
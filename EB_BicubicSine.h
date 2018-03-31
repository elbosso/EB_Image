#ifndef EB_BICUBICSINE
#define EB_BICUBICSINE

#include<EB_BicubicInterpolator.h>

/**Diese Klasse realisiert eine Interpolation mittels eines bikubischen
Sinus.*/
class EB_BicubicSine:public EB_BicubicInterpolator
{
public:
	///Konstruktor
	EB_BicubicSine(void)
	{
	};
	///Destruktor
	virtual ~EB_BicubicSine(void){};
	/**Berechnung der Gewichte für die Interpolation mittels eines Sinus.
	@param x Argument der Funktion.
	@return Gewicht für die Interpolation.*/
	float BicubicFunc(float x)
	{
		float a, y, y2, y3, result;

		a=0.1;  /* arbitrary value */
		result=0;

		if (x < 0) 
		{
			y = -x;
		} 
		else 
		{
			y=x;
		}
		y2=y*y;
		y3=y2*y;
		if (y <= 1.0) 
		{
			result=((a+2)*y3 - (a+3)*y2 + 1);
		} 
		else 
		{
  			if (y <= 2.0) 
			{
				result=(a*y3 - 5*a*y2 + 8*a*y - 4*a);
			}
		}
		return(result); 	 
	};
};

#endif

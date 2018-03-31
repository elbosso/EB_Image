#ifndef EB_BICUBICSPLINE
#define EB_BICUBICSPLINE

#include<EB_BicubicInterpolator.h>

/**Diese Klasse realisiert eine Interpolation mittels des bikubischen
Splineverfahrens.*/
class EB_BicubicSpline:public EB_BicubicInterpolator
{
public:
	///Konstruktor
	EB_BicubicSpline(void):fac1(2.0/3.0),fac2(1.0/6.0)
	{
	};
	///Destruktor
	virtual ~EB_BicubicSpline(void){};
	/**Berechnung der Gewichte für die Interpolation mittels einer Spline.
	@param x Argument der Funktion.
	@return Gewicht für die Interpolation.*/
	float BicubicFunc(float x)
	{
		float y, y2, y3, result;
		if (x < 0) 
		{
			y = -x; 
		} 
		else 
		{
			y = x;
		}
		y2=y*y;
		y3=y2*y;
		if (y < 1.0) 
		{
			result=(fac1 + 0.5 * y3 - y2);
		}
		else
		{
			if (y <= 2.0) 
			{
				result=fac2*(2.0-y)*(2.0-y)*(2.0-y);
			}
		}
		return(result); 	 
	};
	///Faktor der Interpolationsfunktion.
	double fac1;
	///Faktor der Interpolationsfunktion.
	double fac2;
};

#endif

#ifndef EB_BILINEARTRIANGLE
#define EB_BILINEARTRIANGLE

#include<EB_BilinearInterpolator.h>

/**Diese Klasse realisiert eine Interpolation mittels der Dreiecksfunktion.*/
class EB_BilinearTriangle:public EB_BilinearInterpolator
{
public:
	///Konstruktor
	EB_BilinearTriangle(void)
	{
	};
	///Destruktor
	virtual ~EB_BilinearTriangle(void){};
	/**Berechnung der Gewichte für die Interpolation mittels der Dreiecksfuktion.
	@param x Argument der Funktion.
	@return Gewicht für die Interpolation.*/
	float BilinearFunc(float x)
	{
		float result;

		result = 0.0F;

		if ((x >= -1.0F) && (x <= 0.0F))
		{ 
			result = x + 1.0F;
		} 
		else 
		{
			if ((x > 0.0F) && (x <= 1.0F)) result = 1.0F - x;
		}
		return(result); 	 
	};
};

#endif

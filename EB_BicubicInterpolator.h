#ifndef EB_BICUBICINTERPOLATOR
#define EB_BICUBICINTERPOLATOR

#include<EB_TransformationInterpolator.h>

/**Diese Klasse ist die Basisklasse für die verschidenen bikubischen Interpolationsverfahren. Sie reserviert den für die Gewichte und
Positionen der Nachbarpixel benötigten Speicher und berechnet die Positionen
der Nachbarn. Die Berechnung der Gewichte ist die Aufgabe von von dieser
Klasse abgeleiteten Klassen.
@author Jürgen EL BOSSO Key*/
class EB_BicubicInterpolator:public EB_TransformationInterpolator
{
public:
	/**Konstruktor. Dieser Konstruktor übergibt die Menge an benötigten
	Positionen und Gewichten an den Basisklassenkonstruktor.*/
	EB_BicubicInterpolator(void):EB_TransformationInterpolator(16)
	{
	};
	///Destruktor.
	virtual ~EB_BicubicInterpolator(void){};
	/**Bikubische Interpolation. Diese Methode berechnet lediglich die
	Positionen der zur Interpolation benutzten Nachbarn. Zur Berechnung
	der Gewichte wird die virtuelle Methode 
	\Ref{EB_BicubicInterpolator::BicubicFunc} aufgerufen, die in von dieser 
	Klasse abgeleiteten Klassen überladen werden muß.
	@param x gebrochenzahlige Pixelkoordinate.
	@param y gebrochenzahlige Pixelkoordinate.*/
	void calculate(float x,float y)
	{
   		unsigned int index=(unsigned int)x+(unsigned int)y*width;
   		float a=x-(float)((unsigned int)x);
   		float b=y-(float)((unsigned int)y);
		int newx=(int)x,newy=(int)y;
		int m, n;

   		for (m = -1; m <=2; ++m) 
		{
       		for (n = -1; n <=2; ++n) 
			{
         		if ((newx+m >= 1) && (newx+m < (int)(width-1))) 
				{
              		if ((newy+n >=1) && (newy+n < (int)(height-1))) 
					{  
                 		indices[m+1+4*(n+1)] = index +m + width*n;
						factors[m+1+4*(n+1)] = BicubicFunc(-a+(float)m)*BicubicFunc(-b+(float)n);	
					} 
					else 
					{
                 		indices[m+1+4*(n+1)]=index;
						factors[m+1+4*(n+1)]=0.0;
					}
				} 
				else 
				{
               		indices[m+1+4*(n+1)]=index;
					factors[m+1+4*(n+1)]=0.0;
         		}
        	} 
   		} 
	}
	/**Berechnung der Gewichte für die Interpolation. Diese Methode muß in
	abgeleiteten Klassen für spezielle bikubische Interpolatoren überladen 
	werden. 
	@param x Parameter für die jeweils benutzte Funktion.
	@return Gewicht für die Interpolation.*/
	virtual float BicubicFunc(float x)=0;
};

#endif

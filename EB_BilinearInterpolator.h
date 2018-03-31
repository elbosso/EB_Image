#ifndef EB_BILINEARINTERPOLATOR
#define EB_BILINEARINTERPOLATOR

#include<EB_TransformationInterpolator.h>

/**Diese Klasse ist die Basisklasse für die verschidenen bilinearen Interpolationsverfahren. Sie reserviert den für die Gewichte und
Positionen der Nachbarpixel benötigten Speicher und berechnet die Positionen
der Nachbarn. Die Berechnung der Gewichte ist die Aufgabe von von dieser
Klasse abgeleiteten Klassen.
@author Jürgen EL BOSSO Key*/
class EB_BilinearInterpolator:public EB_TransformationInterpolator
{
public:
	/**Konstruktor. Dieser Konstruktor übergibt die Menge an benötigten
	Positionen und Gewichten an den Basisklassenkonstruktor.*/
	EB_BilinearInterpolator(void):EB_TransformationInterpolator(4)
	{
	};
	///Destruktor.
	virtual ~EB_BilinearInterpolator(void){};
	/**Bilineare Interpolation. Diese Methode berechnet lediglich die
	Positionen der zur Interpolation benutzten Nachbarn. Zur Berechnung
	der Gewichte wird die virtuelle Methode 
	\Ref{EB_BilinearInterpolator::BilinearFunc} aufgerufen, die in von dieser 
	Klasse abgeleiteten Klassen überladen werden muß.
	@param x gebrochenzahlige Pixelkoordinate.
	@param y gebrochenzahlige Pixelkoordinate.*/
	void calculate(float x,float y)
	{
   		unsigned int index=(unsigned int)x+(unsigned int)y*width;
   		float a=x-(float)((unsigned int)x);
   		float b=y-(float)((unsigned int)y);
		
   		indices[0]=index;
   		if (x < (float)(width-1)) 
		{
      		indices[1]=index+1;
      		if (y < (float)(height-1)) 
			{
         		indices[2]=index+width;
         		indices[3]=index+width+1;
			}
			else 
			{
	        	indices[2] = 0;
	         	indices[3] = 0;       
	   	   	}
	   	} 
		else 
		{
	    	indices[1] = 0;
	    	indices[3] = 0;
	    	if (y < (float)(height-1)) 
			{
	        	indices[2] = index+width;        
	    	} 
			else 
			{
	    		indices[2] = 0;
	    	}
		}     
  
   /* Do weighted average with surrounding pixels using BilinearFunc() */

		factors[0]=BilinearFunc(-a)*BilinearFunc(b); 
		factors[2]=BilinearFunc(-a)*BilinearFunc(-(1-b));
		factors[1]=BilinearFunc(1-a)*BilinearFunc(b);
		factors[3]=BilinearFunc(-(1-a))*BilinearFunc(-(1-b));
	}
	/**Berechnung der Gewichte für die Interpolation. Diese Methode muß in
	abgeleiteten Klassen für spezielle bilineare Interpolatoren überladen 
	werden. 
	@param x Parameter für die jeweils benutzte Funktion.
	@return Gewicht für die Interpolation.*/
	virtual float BilinearFunc(float x)=0;
};

#endif

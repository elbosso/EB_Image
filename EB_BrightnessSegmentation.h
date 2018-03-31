#ifndef EB_BRIGHTNESSSEGMENTATION_H
#define EB_BRIGHTNESSSEGMENTATION_H

#include"EB_ImageSegmentation.h"

/**Diese Klasse implementiert einen Sonderfall der Anbstandsberechnung:
Hier wird der Mittelwert aller Intensitäten der übergebenen Farben berechnet, 
sozusagen deren Helligkeit.
@author Jürgen "EL BOSSO" Key*/
class EB_BrightnessSegmentation:public EB_ImageSegmentation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_BrightnessSegmentation}.*/
    EB_BrightnessSegmentation (void)
    {
    };
	///Destruktor.
    virtual ~ EB_BrightnessSegmentation (void)
    {
//		delete(lut);
    };
//@}
/**@name public Methoden*/
//@{
	/**Referenzwert für H setzen. Diese Methode setzt den Wert, zu dem die Differenz des jeweiligen H-Wertes berechnet werden soll.
	@param h Referenzwert, wird auf das Intervall zwischen 0.0 und 1.0 beschränkt*/
	void setReferenceH(float h)
	{
	cout<<h<<endl;
		if(h>1.0F)
			Hc=1.0F;
		else if(h<0.0F)
			Hc=0.0F;
		else
			Hc=h;
	}
	/**Distanzberechnung. Diese Methode berechnet die Helligkeit der übergebenen
	Farbe als Mittelwert aller Intensitäten.
	@param p Farbe, für die die Differenz berechnet werden soll.
	@return Mittelwert der enthaltenen Intensitäten.*/
    float computeDistance (EB_PixelDescriptor &p)const
	{
		unsigned int i;
		float sum,d=1.0;
		
		for(i=0;i<p.giveComponentCount();++i)
		{
			sum+=p[i];
		}
		sum/=(float)p.giveComponentCount();
				d=Hc-sum;
				if(d<0.0F)
					d+=1.0;
				if(d>0.5F)
					d=1.0F-d;
		return(d);	
	};
//@}
	float Hc;
};

#endif

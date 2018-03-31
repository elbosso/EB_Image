#ifndef EB_HUESEGMENTATION_H
#define EB_HUESEGMENTATION_H

#include"EB_ImageSegmentation.h"

/**Diese Klasse berechnet den Abstand der H-Komponente des HSI-Farbraumes aus dem übergebenen Pixel im RGB-Farbraum von einem Referenzwert. Dabei wird eine minimale Sättigung in Rechnung gestellt, die die Farbe mindestens haben muß. Liegt der Sättigungswert darunter, wird die Differenz pauschal auf das Maximum gesetzt
@author Jürgen "EL BOSSO" Key*/
class EB_HueSegmentation:public EB_ImageSegmentation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_HueSegmentation}. Der Referenzwert und die Sättigungsschwelle werden dabei jeweils auf 0.0 gesetzt.*/
    EB_HueSegmentation (void):Hc(0.0F),Sc(0.0F)
    {
    };
	///Destruktor.
    virtual ~ EB_HueSegmentation (void)
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
		if(h>1.0F)
			Hc=1.0F;
		else if(h<0.0F)
			Hc=0.0F;
		else
			Hc=h;
	}
	/**Schwelwert für Ssetzen. Diese Methode setzt den minimalen Sättigungswert. Unterschreitet eine Farbe diesen Wert, wird der Abstand vom Referenzwert pauschal mit dem Maximum, 1.0, bewertet. 
	@param s Schwellwert, wird auf das Intervall zwischen 0.0 und 1.0 beschränkt*/
	void setThresholdS(float s)
	{
		if(s>1.0F)
			Sc=1.0F;
		else if(s<0.0F)
			Sc=0.0F;
		else
			Sc=s;
	}
	/**Distanzberechnung. Diese Methode berechnet die H-Komponente der Farbe und interpretiert dafür die ersten drei Bestandteile der übergebenen Farbe als Rot, Grün und Blau im RGB-Farbraum. Ist die Sättigung nicht zu gering, wird anschließend die Differenz zwischen dem errechneten und dem Referenzwert gebildet und zurückgegeben.
	@param p Farbe, für die die Differenz berechnet werden soll.
	@return Abstand des berechneten H-Wertes vom Referezwert.*/
    float computeDistance (EB_PixelDescriptor &p)const
	{
		float H=0,S;
		float r,g,b,min,d=1.0;
		
		if(p.giveComponentCount()>2)
		{
			r=p[0];
			g=p[1];
			b=p[2];
        	H = acos((0.5F*((r-g)+(r-b)))/sqrt((r-g)*(r-g)+(r-b)*(g-b)));
            if(b>g)
				H =M_PI-H;
			H/=M_PI;
			min=g;
			if(r<min)
			min=r;
			if(b<min)
			min=b;
			S=1-(3*min)/(r+b+g);
			if(S>Sc)
			{
				d=Hc-H;
				if(d<0.0F)
					d+=1.0;
				if(d>0.5F)
					d=1.0F-d;
			}
		}
		return(d);	
	};
//@}
	float Hc;
	float Sc;
};

#endif

#ifndef EB_INTENSITYSIGMOIDTRANS_H
#define EB_INTENSITYSIGMOIDTRANS_H

#include"EB_IntensityTransformation.h"
#include<math.h>

/**Diese Klasse berechnet die Sigmoidfunktion. Sie kann über entsprechende Parameter in
x- und y-Richtung verschoben, gestaucht und gestreckt werden.
@author Jürgen "EL BOSSO" Key*/
class EB_IntensitySigmoidTrans:public EB_IntensityTransformation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_IntensitySigmoidTrans}.*/
    EB_IntensitySigmoidTrans (void):xstretch(1.0),ystretch(1.0),xshift(0.0),yshift(0.0)
    {
    };
	///Destruktor.
    virtual ~ EB_IntensitySigmoidTrans (void)
    {
    };
//@}
/**@name public Methoden*/
//@{
	/**Parameter setzen.
	@param Verschiebung in x-Richtung*/
	void setXShift(float s)
	{
		xshift=s;
	}
	/**Parameter setzen.
	@param Verschiebung in y-Richtung*/
	void setYShift(float s)
	{
		yshift=s;
	}
	/**Parameter setzen.
	@param Streckung in x-Richtung*/
	void setXStretch(float s)
	{
		xstretch=s;
	}
	/**Parameter setzen.
	@param Streckung in y-Richtung*/
	void setYStretch(float s)
	{
		ystretch=s;
	}
	/**Sigmoidfunktion.
	@param intensity Argument für die Sigmoidfunktion.
	@return Ergebnis der Sigmoidfunktion.*/
    virtual float transform (float intensity)const
	{
		return(yshift+ystretch/(1.0+exp(-(intensity-xshift)*xstretch)));
	};
//@}
protected:
	///Streckung in x-Richtung.
	float xstretch;
	///Streckung in y-Richtung.
	float ystretch;
	///Verschiebung in x-Richtung.
	float xshift;
	///Verschiebung in y-Richtung.
	float yshift;
};

#endif

#ifndef EB_INTENSITYGAMMATRANS_H
#define EB_INTENSITYGAMMATRANS_H

#include"EB_IntensityTransformation.h"

/**Diese Klasse berechnet die Funktion der Gammakorrektur.
@author Jürgen "EL BOSSO" Key*/
class EB_IntensityGammaTrans:public EB_IntensityTransformation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_IntensityGammaTrans}.*/
    EB_IntensityGammaTrans (void):factor(1.0)
    {
    };
	///Destruktor.
    virtual ~ EB_IntensityGammaTrans (void)
    {
    };
//@}
/**@name public Methoden*/
//@{
	/**Parameter setzen.
	@param gamma Parameter für die Gamma-Korrektur: >1 heller, <1 dunkler*/
	void setGamma(float f)
	{
		factor=1.0/f;
	}
	/**Gammakorrektur. Zur Transformation wird angenommen, dass die 
	Argumente im Intervall 0,1 liegen.
	@param intensity Argument für die Gammafunktion.
	@return Ergebnis der Gammafunktion.*/
    virtual float transform (float intensity)const
	{
      return(pow(intensity,factor));
//			return(1.0/(1.0+exp(-intensity)));
	};
//@}
protected:
	///Kehrwert des Parameters der Gammafunktion.
	float factor;
};

#endif

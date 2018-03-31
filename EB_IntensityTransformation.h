#ifndef EB_INTENSITYTRANSFORMATION_H
#define EB_INTENSITYTRANSFORMATION_H

#include<stdlib.h>

/**Diese Klasse stellt eine Schnittstelle für einwertige Funktionen dar, die
zum Beispiel auch zu Intensitätstransformationen in Bilder benutzt werden können (Gammakorrektur o.ä.).
@author Jürgen "EL BOSSO" Key*/
class EB_IntensityTransformation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_IntensityTransformation}.*/
    EB_IntensityTransformation (void)
    {
    };
	///Destruktor.
    virtual ~ EB_IntensityTransformation (void)
    {
    };
//@}
/**@name public Methoden*/
//@{
	/**Transformation. Diese Methode ist das Herzstück der Klasse. Sie ist pur virtuell und muß von 
	abgeleiteten Klassen überladen werden. Hier muß die entsprechende, die
	Transformation beschreibende Funktion implementiert werden.
	@param intensity Argument der Transformationsfunktion.
	@return Resultat der Transformationsfunktion.*/
    virtual float transform (float intensity)const=0;
//@}
};

#endif

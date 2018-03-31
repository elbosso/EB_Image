#ifndef EB_LOGPOLARTRANSFORMATION_H
#define EB_LOGPOLARTRANSFORMATION_H

#include "EB_PolCartTransformation.h"

/**Diese Klasse realisiert eine spezielle Transformation aus Polar- in kartesische Koordinaten, wie sie
zur Berechnung eines Panoramabildes aus Bildern von omnidirektionalen Kameras gebraucht wird.
Diese Transformation berücksichtigt im speziellen eine Klasse von Kameras, die die Rundumsicht
mittels eines konvexen, parabolischen Spiegels erzeugen.
@author Jürgen "EL BOSSO" Key*/
class EB_LogPolarTransformation:public EB_PolCartTransformation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_LogPolarTransformation}.*/
    EB_LogPolarTransformation (void)
    {
    };
	///Destruktor.
    virtual ~ EB_LogPolarTransformation (void)
    {
    };
//@}
/**@name public Methoden*/
//@{
	/**Höhe des Resultats. 
	@return Von den Dimensionen der Eingagsdaten abhängige Höhe des Resultats.*/
	unsigned int giveDestinationHeight(void)const
	{
	
//	cout<<outer<<" "<<inner<<" "<<height<<" "<<(sqrt(outer)-sqrt(inner))*height*0.5<<endl;
		return((unsigned int)((sqrt(outer)-sqrt(inner))*height*0.5));
	};
	/**Breite des Resultats. 
	@return Von den Dimensionen der Eingagsdaten abhängige Breite des Resultats.*/
	unsigned int giveDestinationWidth(void)const
	{
		return((unsigned int)((double)360*(span/M_PI)/sample));
	};
	/**Omni->Panorama-Konversion. Diese Methode führt eine quadratische Transformation
	aus Polar- in kartesische Koordinaten durch. Man beachte die nichtlineare Transformation und 
	die mathematisch negative Abfolge der Winkel im Ergebnis. Beides ist durch die Methode der
	Gewinnung des Rundumblickes bei den vorhandenen Omnikameras begründet.
	@param x x-Koordinate im Resultat.
	@param y y-Koordinate im Resultat.
	@return linear kodierte Koordinate des Punktes in der Eingabedaten, dessen Inhalt an die angegebene
	Stelle des Resultats kopiert werden soll.*/
    unsigned int transform (unsigned int x, unsigned int y)const
    {
		unsigned int index=0;
		unsigned int sourcemiddlex=width/2;
		unsigned int sourcemiddley=height/2;
		double angle;
		double s;
		double step=(sqrt(outer)-sqrt(inner))/(double)giveDestinationHeight();
		
		if ((width)&&(height))
		{
			angle=(-(double)x*sample)*M_PI/180.0/*spanfactor*/+startangle-M_PI+span;
			s=(sqrt(outer)-step*(double)y);
			s*=s*height*0.5;
			origx=(sin(angle)*s)+sourcemiddlex;
			origy=(cos(angle)*s)+sourcemiddley;
			if((origy>0.0)&&(origy<height))
			{
				if((origx>0.0)&&(origx<width))
					index=(unsigned int)orgx+(unsigned int)origy*width;	
			}
			else
				origx=origy=0.0;
		}
		return(index);
	};
//@}
};

#endif

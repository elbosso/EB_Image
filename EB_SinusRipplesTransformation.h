#ifndef EB_SINUSRIPPLESTRANSFORMATION_H
#define EB_SINUSRIPPLESTRANSFORMATION_H

#include "EB_ImageTransformation.h"
#include <math.h>

/**Diese Klasse dient eher Effekten als klassischer Bildverarbeitung.
Der Grund ihrer Implementation war ein Versuch, zu veranschaulichen, wie
mächtig der Mechanismus der Transformationsinterfaces ist.
Die vorliegende Klasse implementiert den von Photoshop o.ä. her bekanten
Ripples-Effekt dabei ist es so, daß verschiedene Sinusfunktionen überlagert werden können. Bei jeder dieser Sinusfunktionen ist es so, daß Phase, Frequenz,
Amplitude und Winkel zur unteren Bildkante getrennt eingestellt werden können.
Der visuelle Effekt dieser Transformation erscheint als abwechselnde Streckung und Stauchung des Bildinhaltes.
Eine Frequenz von 1 bedeutet dabei, daß exakt ein gestreckter und ein gestauchter Bereich im Bild auftauchen. Hat die Phase den Wert 0, taucht
der gestauchte Bereich ganz links im Bild auf. Die Amlitude oder das Displacement bestimmt, wie groß die maximale Stauchung oder Streckung sein darf.
Der Winkel schließlich bestimmt, in welche Richtung sich die Wellen ausbreiten, 
ein Wert von 0.0 bestimmt hier die Ausbreitung von links entlang der unteren Bildkante. 
@author Jürgen "EL BOSSO" Key*/
class EB_SinusRipplesTransformation:public EB_ImageTransformation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_SinusRipplesTransformation}, die noch keine Funktionen definiert hat*/
    EB_SinusRipplesTransformation (void)
    {
    };
	/**Copy-Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_SinusRipplesTransformation}als genaue Kopie der übergebenen Instanz.*/
    EB_SinusRipplesTransformation(const EB_SinusRipplesTransformation &source):EB_ImageTransformation(source)
	{
	};   
	///Destruktor.
    virtual ~ EB_SinusRipplesTransformation (void)
    {
    };
//@}
/**@name public Methoden*/
//@{
	/**Höhe des Resultats. 
	@return Von den Dimensionen der Eingagsdaten abhängige Höhe des Resultats.*/
	unsigned int giveDestinationHeight(void)const
	{
		return(height);
	};
	/**Breite des Resultats. 
	@return Von den Dimensionen der Eingagsdaten abhängige Breite des Resultats.*/
	unsigned int giveDestinationWidth(void)const
	{
		return(width);
	};
	/**Funktion hinzufügen. Diese Methode fügt eine weitere Funktion zu der Liste von unterschiedlich parametriereten Sinusfunktionen hinzu, die in ihrer Gesamtheit die Transformation bestimmen.
	@param o Hat die Phase den Wert 0.0, taucht
der erste gestauchte Bereich ganz links im Bild auf.
	@param f Eine Frequenz von 1 bedeutet dabei, daß exakt ein gestreckter und ein gestauchter Bereich im Bild auftauchen. 
	@param md Die Amlitude oder das Displacement bestimmt, wie groß die maximale Stauchung oder Streckung sein soll.
	@param alpha Der Winkel bestimmt, in welche Richtung sich die Wellen ausbreiten, 
ein Wert von 0.0 bestimmt hier die Ausbreitung von links entlang der unteren Bildkante. 
	*/
	void addSinus(double o,double f,double md,double alpha)
	{
		sinlist.add(EB_SinusDescriptor(o,f,md,alpha));
		haslut=false;
	}
	/**Funktionen entfernen. Diese Mathode entfernt alle bisher definierten Sinusfunktionen aus der Liste.*/
	void clearSinusList(void)
	{
		sinlist.clear();
	};
	/**Ripples-Effekt. Diese Methode stellt den von Photoshop o.ä. her bekanten
Ripples-Effekt nach. Es werden verschiedene Sinusfunktionen überlagert. Bei jeder dieser Sinusfunktionen ist es so, daß Phase, Frequenz,
Amplitude und Winkel zur unteren Bildkante getrennt eingestellt werden können.
Der visuelle Effekt dieser Transformation erscheint als abwechselnde Streckung und Stauchung des Bildinhaltes.
	@param x x-Koordinate im Resultat.
	@param y y-Koordinate im Resultat.
	@return linear kodierte Koordinate des Punktes in der Eingabedaten, dessen Inhalt an die angegebene
	Stelle des Resultats kopiert werden soll.*/
    unsigned int transform (unsigned int x, unsigned int y)const 
    {
		unsigned int index=0,i;
		double angle,sinangle,displacement;
		EB_SinusDescriptor sf;
		
		if ((width)&&(height))
		{
			sinlist.init();
			origx=(double)x;
			origy=(double)y;
			try
			{
				for(i=0;i<sinlist.giveNodeCount();++i)
				{
					sf=sinlist.giveNext();
					angle=sf.offset+sf.frequency*2*M_PI*(x/(double)width);
					sinangle=sin(angle);
					displacement=(sf.maxdisplacement*sinangle);
					origx+=cos(sf.rotangle)*displacement;
					origy+=sin(sf.rotangle)*displacement;				
				}
			}
			catch(...)
			{
			}
			if((origy>0.0)&&(origy<height))
			{
				if((origx>0.0)&&(origx<width))
					index=(unsigned int)origx+(unsigned int)origy*width;	
			}
			else
				origx=origy=0.0;
		}
		return(index);
	};
	protected:
	class EB_SinusDescriptor
	{
		public:
		EB_SinusDescriptor(void):offset(0.0),
		frequency(1.0),
		maxdisplacement(1.0),
		rotangle(0.0)
		{
		};
		EB_SinusDescriptor(double o,double f,double md,double alpha):offset(o),
		frequency(f),
		maxdisplacement(md),
		rotangle(alpha)
		{
		};
		virtual ~EB_SinusDescriptor(void)
		{
		};		
		double offset;
		double frequency;
		double maxdisplacement;
		double rotangle;
	};
	mutable EB_UnsortedList<EB_SinusDescriptor> sinlist;
//@}
};

#endif

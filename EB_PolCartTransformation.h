#ifndef EB_POLCARTTRANSFORMATION_H
#define EB_POLCARTTRANSFORMATION_H

#include "EB_ImageTransformation.h"

/**Diese Klasse dient als Basisklasse für Spielarten der kartesisch-polaren Koordinatentransformation.
Sie implementiert dazu hauptsaechlich Hilfsfunktionen zur Parametrisierung solcher Funktionen.
Es lassen sich dabei der Start- und Endwinkel einer solchen Transformation ebenso wie minimaler
und maximaler Radius sowie die Abtastrate im Winkel festlegen.
@author Jürgen "EL BOSSO" Key*/
class EB_PolCartTransformation:public EB_ImageTransformation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_PolCartTransformation}.*/
    EB_PolCartTransformation (void):inner(0.0),outer(1.0),sample(1.0),startangle(0.0),span(M_PI)
    {
    };
    /**Copy-Konstruktor. Dieser Konstruktor erzeugt eine identische Kopie der 
	übergebenen Instanz.
	Die Kopie wird mittels deep copy erzeugt. Beide Instanzen sind vollständig unabhängig.
	@param source Instanz, von der die Kopie erzeugt werden soll.*/
    EB_PolCartTransformation(const EB_PolCartTransformation &source):EB_ImageTransformation(source),inner(source.sample),outer(source.sample),sample(source.sample),startangle(source.startangle),span(source.span)
	{
	};   
	///Destruktor.
    virtual ~ EB_PolCartTransformation (void)
    {
    };
//@}
/**@name Operatoren*/
//@{
	/**Zuweisungsoperator. Dieser Operator erzeugt eine Kopie der übergebenen
  in der aktuellen Instanz. Dies ist eine Deep Copy: Es existieren danach
  zwei völlig unabhängige Instanzen.
  @param other Instanz, die kopiert werden soll.
  @return Referenz auf die aktuelle Instanz*/
    EB_PolCartTransformation & operator = (const EB_PolCartTransformation &source)
    {
		EB_ImageTransformation::operator=(source);
		if(this!=&source)
		{
			inner=source.sample;
			outer=source.sample;
			sample=source.sample;
			startangle=source.startangle;
			span=source.span;
		}
		return(*this);
    };
//@}
/**@name public Methoden*/
//@{
	/**Minimaler Radius. Diese Methode setzt den minimalen Radius der Transformation fest.
	Das bedeutet, daß der Inhalt auf diesem Radius die unterste Zeile im Resultat bildet.
	@param r Minimaler Radius. Wird interpretiert als Verhältnis zum größten noch vollständig in den Eingangsdaten
	liegenden Radius, der durch die Hälfte der Höhe oder Breite der Eingangsdaten
	- je nachdem, was kleiner ist - bestimmt wird.
	Ist dieser Wert kleiner als 0, wird der Betrag verwendet, ist dieser größer als der maximale
	Radius, wird der minimale auf 0.0 gesetzt.*/
	void setInnerRadius(double r)
	{
		if(r<0.0)
			r=fabs(r);
		if(r>outer)
			inner=0.0;
		else
			inner=r;
		haslut=false;
	};
	/**Maximaler Radius. Diese Methode setzt den maximalen Radius der Transformation fest.
	Das bedeutet, daß der Inhalt auf diesem Radius die oberste Zeile im Resultat bildet.
	@param r Maximaler Radius. Wird interpretiert als Verhältnis zum größten noch vollständig in den Eingangsdaten
	liegenden Radius, der durch die Hälfte der Höhe oder Breite der Eingangsdaten
	- je nachdem, was kleiner ist - bestimmt wird.
	Ist dieser Wert kleiner als 0, wird der Betrag verwendet, ist dieser kleiner als der minimale 
	oder größer als 1.0, wird er auf 1.0 gesetzt*/
	void setOuterRadius(double r)
	{
		if(r<0.0)
			r=fabs(r);
		if((r<inner)||(r>1.0))
			outer=1.0;
		else
			outer=r;
		haslut=false;
	};
	/**Abtastrate. Diese Methode setzt die Winkelabtastrate fest.
	Eine 1.0 bedeutet dabei, daß das Bild mit einer Abtastrate von einem Grad pro Bildspalte des Resultats abgetatset wird.
	Eine 2.0 würde eine Abtastrate von 2 Grad pro Bildspalte des Resultats
	ergeben.
	@param s Abtastrate in Grad pro Bildspalte des Resultatbildes.*/
	void setSampling(double s)
	{
		sample=fabs(s);	
	};
	/**Startwinkel. Diese Methode legt den Winkel des Resultates fest, der der mittelsten Spalte
	des Resultats entspricht. Dabei bedeutet 0 Grad vom Bildmittelpunkt gerade nach oben. Die Zählweise
	ist mathematisch positiv.
	@param a Startwinkel in Radiant. Liegt der Wert nicht innerhalb des Intervalls (-2PI, 2PI), wird 0.0
	benutzt.*/
	void setStartingAngle(double a)
	{
		if((a<-2*M_PI)||(a>2*M_PI))
			startangle=0.0;
		else
			startangle=a;	
		haslut=false;
	};
	/**Startwinkel. Diese Methode legt den Winkel des Resultates fest, der der mittelsten Spalte
	des Resultats entspricht. Dabei bedeutet 0 Grad vom Bildmittelpunkt gerade nach oben. Die Zählweise
	ist mathematisch positiv.
	@param a Startwinkel in Grad. Liegt der Wert nicht innerhalb des Intervalls (-360, 360), wird 0.0
	benutzt.*/
	void setStartingAngleInGrad(double a)
	{
		if((a<-360.0)||(a>360.0))
			startangle=0.0;
		else
			startangle=a*M_PI/180.0;	
		haslut=false;
	};
	/**Winkelöffnung. Diese Methode legt den Winkelbereich des Resultates fest. Das Resultat enthält das transformierte
	Bild, welches sich rechts und links des Startwinkels bis zum hier angegebenen Winkel erstreckt. Bei einem
	Startwinkel von PI/8 und einem Öffnungswert von PI/4 würde also der Bereich von -3/8PI bis 5/8PI transformiert.
	@param a Öffnungswinkel in Radiant. Liegt der Betrag des Wertes nicht innerhalb des Intervalls (0, PI), wird 0.0
	benutzt.*/
	void setSpan(double a)
	{
		if(fabs(a)>M_PI)
			span=0.0;
		else
			span=fabs(a);	
		haslut=false;
	};
	/**Winkelöffnung. Diese Methode legt den Winkelbereich des Resultates fest. Das Resultat enthält das transformierte
	Bild, welches sich rechts und links des Startwinkels bis zum hier angegebenen Winkel erstreckt. Bei einem
	Startwinkel von 45Grad und einem Öffnungswert von 90Grad würde also der Bereich von 45Grad bis 135Grad 
	transformiert.
	@param a Öffnungswinkel in Grad. Liegt der Betrag des Wertes nicht innerhalb des Intervalls (0, 180), wird 0.0
	benutzt.*/
	void setSpanInGrad(double a)
	{
		if(fabs(a)>180)
			span=0.0;
		else
			span=fabs(a)*M_PI/180.0;	
		haslut=false;
	};
//@}
protected:
	///Minimaler Transformationsradius.
	double inner;
	///Maximaler Transformationsradius.
	double outer;
	///Abtastrate für den Winkel.
	double sample;
	///Startwinkel.
	double startangle;
	///Winkelbereich.
	double span;
};

#endif

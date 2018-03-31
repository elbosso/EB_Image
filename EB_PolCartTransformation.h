#ifndef EB_POLCARTTRANSFORMATION_H
#define EB_POLCARTTRANSFORMATION_H

#include "EB_ImageTransformation.h"

/**Diese Klasse dient als Basisklasse f�r Spielarten der kartesisch-polaren Koordinatentransformation.
Sie implementiert dazu hauptsaechlich Hilfsfunktionen zur Parametrisierung solcher Funktionen.
Es lassen sich dabei der Start- und Endwinkel einer solchen Transformation ebenso wie minimaler
und maximaler Radius sowie die Abtastrate im Winkel festlegen.
@author J�rgen "EL BOSSO" Key*/
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
	�bergebenen Instanz.
	Die Kopie wird mittels deep copy erzeugt. Beide Instanzen sind vollst�ndig unabh�ngig.
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
	/**Zuweisungsoperator. Dieser Operator erzeugt eine Kopie der �bergebenen
  in der aktuellen Instanz. Dies ist eine Deep Copy: Es existieren danach
  zwei v�llig unabh�ngige Instanzen.
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
	Das bedeutet, da� der Inhalt auf diesem Radius die unterste Zeile im Resultat bildet.
	@param r Minimaler Radius. Wird interpretiert als Verh�ltnis zum gr��ten noch vollst�ndig in den Eingangsdaten
	liegenden Radius, der durch die H�lfte der H�he oder Breite der Eingangsdaten
	- je nachdem, was kleiner ist - bestimmt wird.
	Ist dieser Wert kleiner als 0, wird der Betrag verwendet, ist dieser gr��er als der maximale
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
	Das bedeutet, da� der Inhalt auf diesem Radius die oberste Zeile im Resultat bildet.
	@param r Maximaler Radius. Wird interpretiert als Verh�ltnis zum gr��ten noch vollst�ndig in den Eingangsdaten
	liegenden Radius, der durch die H�lfte der H�he oder Breite der Eingangsdaten
	- je nachdem, was kleiner ist - bestimmt wird.
	Ist dieser Wert kleiner als 0, wird der Betrag verwendet, ist dieser kleiner als der minimale 
	oder gr��er als 1.0, wird er auf 1.0 gesetzt*/
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
	Eine 1.0 bedeutet dabei, da� das Bild mit einer Abtastrate von einem Grad pro Bildspalte des Resultats abgetatset wird.
	Eine 2.0 w�rde eine Abtastrate von 2 Grad pro Bildspalte des Resultats
	ergeben.
	@param s Abtastrate in Grad pro Bildspalte des Resultatbildes.*/
	void setSampling(double s)
	{
		sample=fabs(s);	
	};
	/**Startwinkel. Diese Methode legt den Winkel des Resultates fest, der der mittelsten Spalte
	des Resultats entspricht. Dabei bedeutet 0 Grad vom Bildmittelpunkt gerade nach oben. Die Z�hlweise
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
	des Resultats entspricht. Dabei bedeutet 0 Grad vom Bildmittelpunkt gerade nach oben. Die Z�hlweise
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
	/**Winkel�ffnung. Diese Methode legt den Winkelbereich des Resultates fest. Das Resultat enth�lt das transformierte
	Bild, welches sich rechts und links des Startwinkels bis zum hier angegebenen Winkel erstreckt. Bei einem
	Startwinkel von PI/8 und einem �ffnungswert von PI/4 w�rde also der Bereich von -3/8PI bis 5/8PI transformiert.
	@param a �ffnungswinkel in Radiant. Liegt der Betrag des Wertes nicht innerhalb des Intervalls (0, PI), wird 0.0
	benutzt.*/
	void setSpan(double a)
	{
		if(fabs(a)>M_PI)
			span=0.0;
		else
			span=fabs(a);	
		haslut=false;
	};
	/**Winkel�ffnung. Diese Methode legt den Winkelbereich des Resultates fest. Das Resultat enth�lt das transformierte
	Bild, welches sich rechts und links des Startwinkels bis zum hier angegebenen Winkel erstreckt. Bei einem
	Startwinkel von 45Grad und einem �ffnungswert von 90Grad w�rde also der Bereich von 45Grad bis 135Grad 
	transformiert.
	@param a �ffnungswinkel in Grad. Liegt der Betrag des Wertes nicht innerhalb des Intervalls (0, 180), wird 0.0
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
	///Abtastrate f�r den Winkel.
	double sample;
	///Startwinkel.
	double startangle;
	///Winkelbereich.
	double span;
};

#endif

#ifndef EB_IMAGEREGION_H
#define EB_IMAGEREGION_H

#include"EB_UnsortedList.h"
#include"EB_ImageCoordinatePair.h"

#define MY_MIN(x,y) (((x)<(y))?(x):(y))
#define MY_MAX(x,y) (((x)>(y))?(x):(y))

/**Diese Klasse stellt eine abstrakte Implementation zweidimensionaler 
geschlossener Regionen dar. Die Definition der Umgrenzung von Regionen 
erfolgt mittels einer Punktliste. Die Punkte werden in der Reihenfolge 
ihrer Festlegung mit Liniensegmenten verbunden und zum Abschlu� wird der letzte
noch mit dem ersten verbunden, damit die Region geschlossen ist. 
@author J�rgen "EL BOSSO" Key*/
class EB_ImageRegion
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_ImageRegion}, die noch keine Punkte definiert hat*/
    EB_ImageRegion (void):preprocessed (false)
    {
    };
    EB_ImageRegion(const EB_ImageRegion &source):coordlist(source.coordlist),preprocessed(source.preprocessed),xmin(source.xmin),xmax(source.xmax),ymin(source.ymin),ymax(source.ymax)
	{
	};    
	///Destruktor.
    virtual ~ EB_ImageRegion (void)
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
    EB_ImageRegion & operator = (EB_UnsortedList < EB_ImageCoordinatePair > &list)
    {
	coordlist = list;
	preprocessed = false;
	return (*this);
    }
//@}
/**@name public Methoden*/
//@{
	/**Punkt hinzuf�gen. Diese Methode f�gt einen Punkt durch Angabe der 
	expliziten Koordinaten zur Liste der Eckpunkte der Region hinzu.
	@param x x-Komponente des Punktes.
	@param y y-Komponente des Punktes.
	@return Referenz auf die aktuelle Instanz.*/
    EB_ImageRegion & addPoint (int x, int y)
    {
	coordlist.append (EB_ImageCoordinatePair (x, y));
	preprocessed = false;
	return (*this);
    };
	/**Punkt hinzuf�gen. Diese Methode f�gt einen Punkt als Instanz der
	Klasse \Ref{EB_ImageCoordinatePair} 
	zur Liste der Eckpunkte der Region hinzu.
	@param p Instanz der Klasse \Ref{EB_ImageCoordinatePair}, die einen
	Punkt in zweidimensionalen Daten repr�sentiert.
	@return Referenz auf die aktuelle Instanz.*/
    EB_ImageRegion & addPoint (EB_ImageCoordinatePair &p)
    {
	coordlist.append (p);
	preprocessed = false;
	return (*this);
    };
	/**Region l�schen. Diese Methode l�scht s�mtliche Eckpunkte der Region
	und l�st sie damit praktisch auf.
	@return Referenz auf die aktuelle Instanz.*/
    EB_ImageRegion & undefine (void)
    {
	coordlist.clear ();
	preprocessed = false;
	return (*this);
    };
	/**Check, ob Punkt in der Region liegt. Zur Zeit kann dieser Check nur
	richtig erfolgen, wenn sich die Begrenzungslinien der Region nicht �berschneiden.
	@param x x-Komponente des zu pr�fenden Punktes.
	@param y y-Komponente des zu pr�fenden Punktes.
	@return true, wenn der Punkt innerhalb der Region liegt, ansonsten false.*/
    bool isInside (int x, int y);
    /**Test, ob Region definiert. Diese Methode zeigt an, ob die Punkte in der Liste bereits
    eine Region definieren oder nicht, Zur Zeit erfolgt der Test nur anhand der Anzahl 
    an Punkten in der Liste.
    @return true, wenn die Punkte in der Liste eine Region definieren, sonst false.*/
    bool isRegion(void)
    {
		if(coordlist.giveNodeCount()>2)
			return(true);
		else
			return(false);
    }
//@}
  protected:
  	/**Preprocessing. Viele Operationen brauchen, wenn sich die Anzahl und
	Positionen der Regioneneckpunkte nicht �ndern, beim Test, ob Punkte
	inner- oder au�erhalb der Region liegen, nur einmal 
	und nicht f�r jeden zu testenden Punkt ausgef�hrt werden. Diese Operationen
	sind in dieser Methode vereint.*/
    void doPreprocess (void)
    {
	findExtrema ();
	preprocessed = true;
    }
	/**Extremwerte finden. Diese Methode sucht die Werte der gr��ten Ausdehnung
	der Region in x- und y-Richtung. Mit dem resultierenden umschlie�enden
	Rechteck wird dann in \Ref{isInside} ein schneller Ablehnungstest
	durchgef�hrt.*/
    void findExtrema (void)
    {
	EB_ImageCoordinatePair tpair;
	unsigned int i;

	  xmin = 100000;
	  xmax = -100000;
	  ymin = 100000;
	  ymax = -100000;
	  coordlist.init ();
	for (i = 0; i < coordlist.giveNodeCount (); ++i)
	    {
		tpair = coordlist.giveNext ();
		if (tpair.xposition < xmin)
		    xmin = tpair.xposition;
		if (tpair.xposition > xmax)
		    xmax = tpair.xposition;
		if (tpair.yposition < ymin)
		    ymin = tpair.yposition;
		if (tpair.yposition > ymax)
		    ymax = tpair.yposition;
	    }
    };
	///Liste der Koordinaten der Eckpunkte der Region.
    EB_UnsortedList < EB_ImageCoordinatePair > coordlist;
	///Flags, die anzeigen, ob bereits die Vorverarbeitung stattgefunden hat und ob diese noch g�ltig ist und ob eine Region definiert ist (ob Punkte in der Liste enthalten sind) 
    bool preprocessed;
	///Die Eckpunkte des die Region umschlie�enden Rechtecks.
    int xmin, xmax, ymin, ymax;
};

#endif

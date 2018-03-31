#ifndef EB_SINUSFILTER_H
#define EB_SINUSFILTER_H

#include"EB_Filter.h"

/**Diese Klasse stellt die Implementation von zweidimensionalen Sinusfiltern 
mit einstellbarer Frequenz und Phasenlage dar. Die Filterfunktion ist hier sin(x*xfreq+xoff)*sin(y*yfreq+yoff). Die Frequenz bezieht sich hierbei auf die Abmessungen der Filtermaske. Eine Frequenz von 1 in x-und y-Richtung würde also bedeuten, daß genau eine Periode der Funktion in der Maske enthalten wäre.
@author Jürgen "EL BOSSO" Key*/
class EB_SinusFilter:public EB_Filter
{
public: 
/**@name Konstruktoren und Destruktor*/
//@{
  /**Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
  \Ref{EB_SinusFilter} mit vorgegebenen Kantenlängen. Die Frequenz ist für beide Dimensionen 1.0, der Offset ist für beide Dimensionen 0.0. 
  @param x Breite der Filtermaske.
  @param y Höhe der Filtermaske.*/ 
  EB_SinusFilter(unsigned int x,unsigned int y):EB_Filter(x,y),xfreq(1.0),yfreq(1.0),xoff(0.0),yoff(0.0)
  {
		calculateFilterMask();
  };
  /**Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
  \Ref{EB_SinusFilter} mit vorgegebenen Kantenlängen. Die Frequenz ist für beide Dimensionen 1.0, der Offset ist für beide Dimensionen 0.0. 
  @param x Breite der Filtermaske.
  @param y Höhe der Filtermaske.
	@param xf Frequenz der Filterfunktion in x-Richtung bezogen auf die Breite der Filtermaske.
	@param yf Frequenz der Filterfunktion in y-Richtung bezogen auf die Höhe der Filtermaske.
	@param xo Offset der Filterfunktion in x-Richtung in Radiant.
	@param yo Offset der Filterfunktion in y-Richtung in Radiant.*/
  EB_SinusFilter(unsigned int x,unsigned int y,float xf,float yf,float xo,float yo):EB_Filter(x,y),xfreq(xf),yfreq(yf),xoff(xo),yoff(yo)
  {
  		calculateFilterMask();
	};
    /**Copy-Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse 
    \Ref{EB_SinusFilter} als Kopie der übergebenen Instanz.
	@param other Instanz, die zum Erstellen der Kopie benutzt wird.*/
  EB_SinusFilter(const EB_SinusFilter &other):EB_Filter(other){};
  ///Destruktor.
  virtual ~EB_SinusFilter(){};
//@}
/**@name public Methoden*/
//@{
	/**Frequenz setzen. Diese Methode setzt die Frequenz des Filters in x-Richtung.
	@param xf Frequenz der Filterfunktion in x-Richtung bezogen auf die Breite der Filtermaske.*/
	void setXFrequency(float xf)
	{
		xfreq=xf;
		calculateFilterMask();
	};
	/**Frequenz setzen. Diese Methode setzt die Frequenz des Filters in y-Richtung.
	@param yf Frequenz der Filterfunktion in y-Richtung bezogen auf die Höhe der Filtermaske.*/
	void setYFrequency(float yf)
	{
		yfreq=yf;
	};
	/**Offset setzen. Diese Methode setzt den Offset der Filterfunktion in x-Richtung.
	@param xo Offset der Filterfunktion in x-Richtung in Radiant.*/
	void setXOffset(float xo)
	{
		xoff=xo;
	};
	/**Offset setzen. Diese Methode setzt den Offset der Filterfunktion in y-Richtung.
	@param yo Offset der Filterfunktion in y-Richtung in Radiant.*/
	void setYOffset(float yo)
	{
		yoff=yo;
	};
//@}
 protected:
 	/**Filtermaske berechnen. Diese Methode berechnet bei der Konstruktion einer Instanz oder nach einer Parameteränderung die Werte der Filtermaske.*/
 	void calculateFilterMask(void);
///Frequenz in x-Richtung bezogen auf die Filterbreite.
	float xfreq;
///Frequenz in y-Richtung bezogen auf die Filteröhe.
	float yfreq;
///Offset der Filterfunktion in x-Richtung.
	float xoff;
///Offset der Filterfunktion in y-Richtung.
	float yoff;
};

#endif

#ifndef __EB_COLORDISTHISTOGRAM_H
#define __EB_COLORDISTHISTOGRAM_H

#include "EB_LookUpTable.h"

class EB_Band;
/**Diese Klasse stellt Intensitätshistogramme dar. Intensitätshistogramme deshalb, weil hier
nur die Häufigkeiten einzelner Float-Werte summiert werden und nicht die von Vektoren von 
Float-Werten, wie das zum Beispiel bei Farben, die ja durch eine Menge von Float-Werten
dargestellt werden, wäre. Es ist möglich, die Anzahl der einzelnen Stützstellen des Histogramms
frei zu bestimmen, es ist nicht möglich, mit dieser Klasse nichtäquidistante Histogramme zu
modellieren.  
@author Jürgen "EL BOSSO" Key*/
class EB_ColorDistHistogram
{ 
	public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse 
	\Ref{EB_ColorDistHistogram} mit der angegebenen Anzahl an Stützstellen.
	@param count Gewünschte Anzahl an Stützstellen.*/
    EB_ColorDistHistogram (unsigned int count);
    /**Copy-Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse 
    \Ref{EB_ColorDistHistogram} als Kopie der übergebenen Instanz.
	@param other Instanz, die zum Erstellen der Kopie benutzt wird.*/
    EB_ColorDistHistogram (const EB_ColorDistHistogram & other);
    ///Destruktor.
    virtual ~ EB_ColorDistHistogram ();
//@}
/**@name Operatoren*/
//@{
	/**Zuweisungsoperator. Dieser Operator erzeugt eine Kopie der übergebenen
  in der aktuellen Instanz. Dies ist eine Deep Copy: Es existieren danach
  zwei völlig unabhängige Instanzen.
  @param other Instanz, die kopiert werden soll.
  @return Referenz auf die aktuelle Instanz*/
    EB_ColorDistHistogram & operator = (const EB_ColorDistHistogram & other);
    EB_ColorDistHistogram & operator = (float value)
	{
    	unsigned int i;

    	for (i = 0; i < spointcount; i++)
		{
	    	histogram[i] = value;
		}
		return(*this);
	};
  /**Indexoperator. Diese Methode erlaubt es, auf den Wert des Histogramms an
  einer bestimmten Stelle zuzugreifen.
  @param index Laufende Nummer.
  @return Referenz auf einen Histogrammwert.*/
    float &operator[](unsigned int index) const {return(histogram[index]);};
	
//@}
/**@name public Methoden*/
//@{
	/**Ermittlung der Stützstellenanzahl. Mit dieser Methode kann man die aktuelle 
	Stützstellenanzahl der aktuellen Instanz abfragen.
	@return Die aktuelle Stützstellenanzahl.*/
    unsigned int giveSPointCount (void) const
    {
	return (spointcount);
    };
	/**Ermittlung der Sampleanzahl. Mit dieser Methode kann man die Anzahl der bisher zur 
	Berechnung des Histogramms benutzten Samples bestimmen. 
	@return Die Zahl der bisher zur Berechnung des Histogramms benutzten Samples.*/
    unsigned int giveSampleCount (void) const
    {
	return (samplecount);
    };
	/**Häufigkeiten ermitteln. Mit dieser Methode lassen sich die absoluten Häufigkeiten der
	Samples über den Intervallen des Histogramms ermitteln. Jede Stützstelle des Histogramms
	entspricht einem Intensitätsintervall, daher ergibt die absolute Häufigkeit für eine bestimmte
	Stützstelle die Anzahl an Samples, deren Intensität in dem dieser Stützstelle zugeordneten
	Intervall liegt.
	@return Float-Array mit ebensovielen Elementen, wie das Histogramm in der aktuellen Instanz 
	Stützstellen hat.*/
    float *giveAbsHistogramPointer (void)
    {
	return (histogram);
    };
	/**Häufigkeiten ermitteln. Diese Methode entspricht im Prinzip \Ref{giveAbsHistogramPointer},
	mit dem Unterschied, daß nun relative Häufigkeiten zurückgegeben werden. Das heißt, daß
	die Häufigkeiten an jeder Stützstelle durch die Gesamtanzahl aller zur Berechnung des
	Histogramms benutzten Samples dividiert werden.
	@return Float-Array mit ebensovielen Elementen, wie das Histogramm in der aktuellen Instanz 
	Stützstellen hat.*/
    float *giveRelHistogramPointer (void);
	/**Histogramm löschen. Mit dieser Methode werden alle Informationen in der 
	Klasse wieder zurückgesetzt, so daß es so erscheint, als ob das Histogramm
	völlig leer ist.*/
    void eraseHistogram (void);
	/**Histogramm löschen. Mit dieser Methode werden alle Informationen in der 
	Klasse wieder zurückgesetzt, so daß es so erscheint, als ob das Histogramm
	völlig leer ist. Zusätzlich werden die Stützstellen- und Intervallanzahl
	für das Histogramm neu spezifiziert.
	@param newspointcount Gibt die gewünschte Anzahl an Stützstellen an.*/
    void eraseHistogram (unsigned int newspointcount);
	/**Absolute Häufigkeit ermitteln. Diese Methode gestattet es, für
	eine bestimmte Stützstelle des Histogrammes die Anzahl Samples
	zu ermitteln, die in das zugehörige Intervall fielen. Dies ist praktisch
	die absolute Häufigkeit.
	@param index Nummer der Stützstelle, deren absolute Häufigkeit ermittelt
	werden soll.
	@return Anzahl Samples, die auf das zugehörige Intervall entfielen.*/
    float giveAbsValue (unsigned int index) const;
	/**Relative Häufigkeit ermitteln. Diese Methode gestattet es, für
	eine bestimmte Stützstelle des Histogrammes relativ zur Gesamtanzahl
	Samples die Anzahl der Samples
	zu ermitteln, die in das zugehörige Intervall fielen. Dies ist praktisch
	die relative Häufigkeit.
	@param index Nummer der Stützstelle, deren relative Häufigkeit ermittelt
	werden soll.
	@return Verhältnis zwischen der Anzahl Samples, die auf das zugehörige 
	Intervall entfielen und der Gesamtanzahl.*/
    float giveRelValue (unsigned int index) const;
	/**Histogramm berechnen. Diese Methode ermittelt entsprechend der eingestellten
	Stützstellenanzahl ein Histogramm über alle Intensitätswerte in der 
	übergebenen Instanz der Klasse \Ref{EB_Band}. Die Breite und Grenzen der
	einzelnen zu den Stützstellen gehörenden Intervalle werden durch die Grenzen
	des Intensitätsbereiches des Bandes definiert.
	@param band Instanz der Klasse \Ref{EB_Band}, über deren Intensitätswerte
	das Histogramm berechnet werden soll.*/
    void calculateHistogram (EB_Band & band);
	/**Histogrammausgleich.*/
    void aequalizeHistogram (void);
	/**Mittelwertfilterung. Diese Methode erlaubt es, das Histogramm mit
	einem Boxfilter einstellbarer Breite zu filtern.
	@param width Breite des angewendeten Boxfilters.*/
    void smoothBox(unsigned int width);
	/**Mittelwertfilterung. Diese Methode erlaubt es, das Histogramm mit
	einem Boxfilter einstellbarer Breite zu filtern. Dabie wird das Histogramm
	als Ring aufgefasst, d.h. Zur Filterung der ersten Elemente werden die letzten mit einbezogen und umgekehrt.
	@param width Breite des angewendeten Boxfilters.*/
    void smoothBoxClosed(unsigned int width);
	/**Maximumsuche. Der maximale Wert im Histogramm wird gesucht und zurückgegeben.
	@return Der maximale Absolutwert des Histogramms.*/
	float giveMax(void);
	/**Maximumsuche. Der Index des maximalen Wertes im Histogramm wird gesucht 
	und zurückgegeben.
	@return Der Index (die Stützstelle) des maximalen Absolutwertes 
	des Histogramms.*/
	unsigned int giveIndexOfMax(void);
	/**Minimumsuche. Der minimale Wert im Histogramm wird gesucht und zurückgegeben.
	@return Der minimale Absolutwert des Histogramms.*/
	float giveMin(void);
	/**Minimumsuche. Der Index des minimalen Wertes im Histogramm wird gesucht 
	und zurückgegeben.
	@return Der Index (die Stützstelle) des minimalen Absolutwertes 
	des Histogramms.*/
	unsigned int giveIndexOfMin(void);
	/**Elementweise Multiplikation. Diese Methode wird zur Elementweisen Multiplikation
	zweier Instanzen der Klasse \Ref{EB_ColorDistHistogram} benutzt. Enthält die
	aktuelle Instanz mehr Elemente als other, werden die überzähligen Elemente auf 0.0
	gesetzt.
	@param other Referenz auf die Instanz, deren Elemente mit denen der aktuellen
	multipliziert werden sollen.
	@return Referenz auf die aktuelle Instanz*/
	EB_ColorDistHistogram &EB_ColorDistHistogram::mulElementwise (const EB_ColorDistHistogram &other);
//@}
  protected:
///Anzahl der für die Erstellung des Histogramms benutzten Samples.  
    unsigned int samplecount;
///Anzahl der Stützstellen im Histogramm.
    unsigned int spointcount;
  public:
      EB_LookUpTable lookuptable;
  protected:
///Feld zur Aufnahme der absoluten Häufigkeiten.
    float *histogram;
///Feld zur Aufnahme der relativen Häufigkeiten.
    float *scaledhistogram;
};

#endif /* __COLORDISTHISTOGRAM_H */

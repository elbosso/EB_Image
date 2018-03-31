#ifndef __EB_COLORDISTHISTOGRAM_H
#define __EB_COLORDISTHISTOGRAM_H

#include "EB_LookUpTable.h"

class EB_Band;
/**Diese Klasse stellt Intensit�tshistogramme dar. Intensit�tshistogramme deshalb, weil hier
nur die H�ufigkeiten einzelner Float-Werte summiert werden und nicht die von Vektoren von 
Float-Werten, wie das zum Beispiel bei Farben, die ja durch eine Menge von Float-Werten
dargestellt werden, w�re. Es ist m�glich, die Anzahl der einzelnen St�tzstellen des Histogramms
frei zu bestimmen, es ist nicht m�glich, mit dieser Klasse nicht�quidistante Histogramme zu
modellieren.  
@author J�rgen "EL BOSSO" Key*/
class EB_ColorDistHistogram
{ 
	public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse 
	\Ref{EB_ColorDistHistogram} mit der angegebenen Anzahl an St�tzstellen.
	@param count Gew�nschte Anzahl an St�tzstellen.*/
    EB_ColorDistHistogram (unsigned int count);
    /**Copy-Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse 
    \Ref{EB_ColorDistHistogram} als Kopie der �bergebenen Instanz.
	@param other Instanz, die zum Erstellen der Kopie benutzt wird.*/
    EB_ColorDistHistogram (const EB_ColorDistHistogram & other);
    ///Destruktor.
    virtual ~ EB_ColorDistHistogram ();
//@}
/**@name Operatoren*/
//@{
	/**Zuweisungsoperator. Dieser Operator erzeugt eine Kopie der �bergebenen
  in der aktuellen Instanz. Dies ist eine Deep Copy: Es existieren danach
  zwei v�llig unabh�ngige Instanzen.
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
	/**Ermittlung der St�tzstellenanzahl. Mit dieser Methode kann man die aktuelle 
	St�tzstellenanzahl der aktuellen Instanz abfragen.
	@return Die aktuelle St�tzstellenanzahl.*/
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
	/**H�ufigkeiten ermitteln. Mit dieser Methode lassen sich die absoluten H�ufigkeiten der
	Samples �ber den Intervallen des Histogramms ermitteln. Jede St�tzstelle des Histogramms
	entspricht einem Intensit�tsintervall, daher ergibt die absolute H�ufigkeit f�r eine bestimmte
	St�tzstelle die Anzahl an Samples, deren Intensit�t in dem dieser St�tzstelle zugeordneten
	Intervall liegt.
	@return Float-Array mit ebensovielen Elementen, wie das Histogramm in der aktuellen Instanz 
	St�tzstellen hat.*/
    float *giveAbsHistogramPointer (void)
    {
	return (histogram);
    };
	/**H�ufigkeiten ermitteln. Diese Methode entspricht im Prinzip \Ref{giveAbsHistogramPointer},
	mit dem Unterschied, da� nun relative H�ufigkeiten zur�ckgegeben werden. Das hei�t, da�
	die H�ufigkeiten an jeder St�tzstelle durch die Gesamtanzahl aller zur Berechnung des
	Histogramms benutzten Samples dividiert werden.
	@return Float-Array mit ebensovielen Elementen, wie das Histogramm in der aktuellen Instanz 
	St�tzstellen hat.*/
    float *giveRelHistogramPointer (void);
	/**Histogramm l�schen. Mit dieser Methode werden alle Informationen in der 
	Klasse wieder zur�ckgesetzt, so da� es so erscheint, als ob das Histogramm
	v�llig leer ist.*/
    void eraseHistogram (void);
	/**Histogramm l�schen. Mit dieser Methode werden alle Informationen in der 
	Klasse wieder zur�ckgesetzt, so da� es so erscheint, als ob das Histogramm
	v�llig leer ist. Zus�tzlich werden die St�tzstellen- und Intervallanzahl
	f�r das Histogramm neu spezifiziert.
	@param newspointcount Gibt die gew�nschte Anzahl an St�tzstellen an.*/
    void eraseHistogram (unsigned int newspointcount);
	/**Absolute H�ufigkeit ermitteln. Diese Methode gestattet es, f�r
	eine bestimmte St�tzstelle des Histogrammes die Anzahl Samples
	zu ermitteln, die in das zugeh�rige Intervall fielen. Dies ist praktisch
	die absolute H�ufigkeit.
	@param index Nummer der St�tzstelle, deren absolute H�ufigkeit ermittelt
	werden soll.
	@return Anzahl Samples, die auf das zugeh�rige Intervall entfielen.*/
    float giveAbsValue (unsigned int index) const;
	/**Relative H�ufigkeit ermitteln. Diese Methode gestattet es, f�r
	eine bestimmte St�tzstelle des Histogrammes relativ zur Gesamtanzahl
	Samples die Anzahl der Samples
	zu ermitteln, die in das zugeh�rige Intervall fielen. Dies ist praktisch
	die relative H�ufigkeit.
	@param index Nummer der St�tzstelle, deren relative H�ufigkeit ermittelt
	werden soll.
	@return Verh�ltnis zwischen der Anzahl Samples, die auf das zugeh�rige 
	Intervall entfielen und der Gesamtanzahl.*/
    float giveRelValue (unsigned int index) const;
	/**Histogramm berechnen. Diese Methode ermittelt entsprechend der eingestellten
	St�tzstellenanzahl ein Histogramm �ber alle Intensit�tswerte in der 
	�bergebenen Instanz der Klasse \Ref{EB_Band}. Die Breite und Grenzen der
	einzelnen zu den St�tzstellen geh�renden Intervalle werden durch die Grenzen
	des Intensit�tsbereiches des Bandes definiert.
	@param band Instanz der Klasse \Ref{EB_Band}, �ber deren Intensit�tswerte
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
	/**Maximumsuche. Der maximale Wert im Histogramm wird gesucht und zur�ckgegeben.
	@return Der maximale Absolutwert des Histogramms.*/
	float giveMax(void);
	/**Maximumsuche. Der Index des maximalen Wertes im Histogramm wird gesucht 
	und zur�ckgegeben.
	@return Der Index (die St�tzstelle) des maximalen Absolutwertes 
	des Histogramms.*/
	unsigned int giveIndexOfMax(void);
	/**Minimumsuche. Der minimale Wert im Histogramm wird gesucht und zur�ckgegeben.
	@return Der minimale Absolutwert des Histogramms.*/
	float giveMin(void);
	/**Minimumsuche. Der Index des minimalen Wertes im Histogramm wird gesucht 
	und zur�ckgegeben.
	@return Der Index (die St�tzstelle) des minimalen Absolutwertes 
	des Histogramms.*/
	unsigned int giveIndexOfMin(void);
	/**Elementweise Multiplikation. Diese Methode wird zur Elementweisen Multiplikation
	zweier Instanzen der Klasse \Ref{EB_ColorDistHistogram} benutzt. Enth�lt die
	aktuelle Instanz mehr Elemente als other, werden die �berz�hligen Elemente auf 0.0
	gesetzt.
	@param other Referenz auf die Instanz, deren Elemente mit denen der aktuellen
	multipliziert werden sollen.
	@return Referenz auf die aktuelle Instanz*/
	EB_ColorDistHistogram &EB_ColorDistHistogram::mulElementwise (const EB_ColorDistHistogram &other);
//@}
  protected:
///Anzahl der f�r die Erstellung des Histogramms benutzten Samples.  
    unsigned int samplecount;
///Anzahl der St�tzstellen im Histogramm.
    unsigned int spointcount;
  public:
      EB_LookUpTable lookuptable;
  protected:
///Feld zur Aufnahme der absoluten H�ufigkeiten.
    float *histogram;
///Feld zur Aufnahme der relativen H�ufigkeiten.
    float *scaledhistogram;
};

#endif /* __COLORDISTHISTOGRAM_H */

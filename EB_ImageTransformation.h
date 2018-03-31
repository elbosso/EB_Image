#ifndef EB_IMAGETRANSFORMATION_H
#define EB_IMAGETRANSFORMATION_H

#include<stdlib.h>

/**Diese Klasse stellt eine Schnittstelle f�r Koordinatentransformationen in rechteckigen Intensit�tsverteilungen
wie zum Beispiel Bildern dar. Solche Transformationen sind beispielsweise Drehen, Skalieren, Spiegeln
oder Konvertierung zwischen Koordinatensystemen, zum Beispiel polar <-> kartesisch. Diese Klasse
enth�lt mehrere Methoden, die in abgeleiteten Klassen �berladen werden m�ssen.  Zum einen sind
das die Methoden, die die Gr��e des Resultatbildes in Abh�ngigkeit von den Dimensionen des zu
verarbeitenden Bildes berechnen. Daneben ist die Transformationsmethode selbst zu �berladen. Zu den von 
der Schnittstelle bereitgestellten Funktionen geh�rt die selbstst�ndig erstellte Look-Up-Tabelle, die 
die Ausf�hrung derselben Transformation auf unterschiedlichen Eingangsdaten mit den gleichen
Dimensionen beschleunigt.
@author J�rgen "EL BOSSO" Key*/
class EB_ImageTransformation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_ImageTransformation}.*/
    EB_ImageTransformation (void):width(0),height(0),haslut(false),lut(NULL),origx(0.0),origy(0.0)
    {
	};
    /**Copy-Konstruktor. Dieser Konstruktor erzeugt eine identische Kopie der 
	�bergebenen Instanz.
	Die Kopie wird mittels deep copy erzeugt. Beide Instanzen sind vollst�ndig unabh�ngig.
	@param source Instanz, von der die Kopie erzeugt werden soll.*/
    EB_ImageTransformation(const EB_ImageTransformation &source):width(source.width),height(source.height),haslut(source.haslut),lut(NULL),origx(source.origx),origy(source.origy)
	{
		unsigned int i,pixelcount=width*height;
		
		if(haslut)
		{
			lut=new unsigned int[pixelcount];
			for(i=0;i<pixelcount;++i)
				lut[i]=source.lut[i];
		}
	};    
	///Destruktor.
    virtual ~ EB_ImageTransformation (void)
    {
		delete(lut);
    };
//@}
/**@name Operatoren*/
//@{
	/**Zuweisungsoperator. Dieser Operator erzeugt eine Kopie der �bergebenen
  in der aktuellen Instanz. Dies ist eine Deep Copy: Es existieren danach
  zwei v�llig unabh�ngige Instanzen.
  @param other Instanz, die kopiert werden soll.
  @return Referenz auf die aktuelle Instanz*/
    EB_ImageTransformation & operator = (const EB_ImageTransformation &source)
    {
		unsigned int i,pixelcount=width*height;

		if(this!=&source)
		{
			width=source.width;
			height=source.height;
			haslut=source.haslut;
			lut=NULL;
			if(haslut)
			{
				lut=new unsigned int[pixelcount];
				for(i=0;i<pixelcount;++i)
					lut[i]=source.lut[i];
			}	
		}
		return(*this);
    }
//@}
/**@name public Methoden*/
//@{
	/**Eingangsdimensionen setzen. Mit dieser Methode werden dee aktuellen Instanz die Dimensionen
	der zu transformierenden Daten bekanntgegeben. Unterscheiden sie sich von den vorher g�ltigen Werten,
	wird die eventuell bereits berechnete Look-Up-Tabelle ung�ltig.
	@param w Breite der Eingangsdaten.
	@param h H�he der Eingangsdaten.*/
	void setSourceDimensions(unsigned int w,unsigned int h)const 
	{
		if((width!=w)||(height!=h))
		{
			haslut=false;
			delete(lut);
		}
		width=w;
		height=h;
	};
	/**H�he des Resultats. Diese Methode ist pur virtuell. Sie mu� von abgeleiteten Klassen �berladen werden.
	Das Resultat dieser Funktion mu� die H�he des Resultats der Transformation sein.
	@return Von den Dimensionen der Eingagsdaten abh�ngige H�he des Resultats.*/
	virtual unsigned int giveDestinationHeight(void)const=0;
	/**Breite des Resultats. Diese Methode ist pur virtuell. Sie mu� von abgeleiteten Klassen �berladen werden.
	Das Resultat dieser Funktion mu� die Breite des Resultats der Transformation sein.
	@return Von den Dimensionen der Eingagsdaten abh�ngige Breite des Resultats.*/
	virtual unsigned int giveDestinationWidth(void)const=0;
	/**Transformation. Diese Methode ist das Herzst�ck der Klasse. Sie ist pur virtuell und mu� von 
	abgeleiteten Klassen �berladen werden. Dabei ist es so, da� die �bergebenen Koordinaten einen Punkt
	im Resultat spezifizieren. Die Methode mu� dann den linear kodierten (die Eingangsdaten
	 werden dabei zeilenweise durchnumeriert) Punkt in den Eingangsdaten berechnen, dessen Inhalt an dieser 
	 Stelle in das Resultat eingetragen werden soll.
	@param x x-Koordinate im Resultat.
	@param y y-Koordinate im Resultat.
	@return linear kodierte Koordinate des Punktes in der Eingabedaten, dessen Inhalt an die angegebene
	Stelle des Resultats kopiert werden soll.*/
    virtual unsigned int transform (unsigned int x, unsigned int y)const =0;
	/**Transformation. Hier  ist es so, da� die �bergebenen Koordinaten einen Punkt
	im Resultat linear kodieren. Die Methode liefert den linear kodierten 
	Punkt in den Eingangsdaten, dessen Inhalt an dieser 
	 Stelle in das Resultat eingetragen werden soll.
	@param index linear kodierte Koordinate des Resultats.
	@return linear kodierte Koordinate des Punktes in der Eingabedaten, dessen Inhalt an die angegebene
	Stelle des Resultats kopiert werden soll.*/
    unsigned int transform (unsigned int index)const
	{
		unsigned int dw=giveDestinationWidth();
		
		return(transform(index%dw,index/dw));
	};
	/**Transformation mit LUT. Die  �bergebenen Koordinaten kodieren einen Punkt
	im Resultat. Die Methode liefert den linear kodierten Punkt in den Eingangsdaten, dessen Inhalt an dieser 
	 Stelle in das Resultat eingetragen werden soll. Ist noch keine LUT erstellt, wird dies f�r alle Elemente
	 des Resultats nachgeholt. In der LUT stehen danach die linear kodierten Koordinaten der jeweils zugeh�rigen
	 Punkte der Eingabedaten.
	@param x x-Koordinate im Resultat.
	@param y y-Koordinate im Resultat.
	@return linear kodierte Koordinate des Punktes in der Eingabedaten, dessen Inhalt an die angegebene
	Stelle des Resultats kopiert werden soll.*/
    unsigned int transformWithLUT(unsigned int x, unsigned int y)const
	{
		return(transformWithLUT(giveDestinationWidth()*y+x));
	};
	/**Transformation mit LUT. Hier spezifizieren die  �bergebenen Koordinaten linear kodiert einen Punkt
	im Resultat. Die Methode liefert den linear kodierten Punkt in den Eingangsdaten, dessen Inhalt an dieser 
	 Stelle in das Resultat eingetragen werden soll. Ist noch keine LUT erstellt, wird dies f�r alle Elemente
	 des Resultats nachgeholt. In der LUT stehen danach die linear kodierten Koordinaten der jeweils zugeh�rigen
	 Punkte der Eingabedaten.
	@param index linear kodierte Koordinate des Resultats.
	@return linear kodierte Koordinate des Punktes in der Eingabedaten, dessen Inhalt an die angegebene
	Stelle des Resultats kopiert werden soll.*/
	unsigned int transformWithLUT(unsigned int index)const
	{
		unsigned int i,j,w=giveDestinationWidth(),h=giveDestinationHeight(),ind;

		if(haslut==false)
		{
			delete(lut);
			lut=new unsigned int[w*h];
    		ind = 0;
    		for (j = 0; j < h; j++)
			{
	    		for (i = 0; i < w; i++)
				{
					lut[ind]=transform(i,j);
					ind++;
				}
			}	
			haslut=true;
		}
		return(lut[index]);
	};
	float giveOrigX(void) const
	{
		return(origx);
	};
	float giveOrigY(void) const
	{
		return(origy);
	};
//@}
  protected:
  ///Breite der Eingangsdaten.
  mutable unsigned int width;
  ///H�he der Eingangsdaten.
  mutable unsigned int height;
  ///Flag, das anzeigt, ob eine g�ltige LUT vorliegt.
  mutable bool haslut;
  ///Zeiger auf ein Feld von unsigned ints, das die LUT darstellt.
  mutable unsigned int *lut;
  ///Die Float-Koordinaten des Originalpixels zum Interpolieren!
  mutable float origx;
  ///Die Float-Koordinaten des Originalpixels zum Interpolieren!
  mutable float origy;
};

#endif

#ifndef EB_IMAGESEGMENTATION_H
#define EB_IMAGESEGMENTATION_H

/**Diese Klasse stellt eine Schnittstelle f�r die farbbasierte Segmentation dar.
Diese Segmentation geschieht nicht in dieser Klasse, sie dient lediglich dazu,
den Abstand zwischen zwei Farben zu berechnen. Dabei ist die konkrete Wahl
des Abstandma�es in einer abgeleiteten  Klasse zu treffen.
@author J�rgen "EL BOSSO" Key*/
class EB_ImageSegmentation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_ImageSegmentation}.*/
    EB_ImageSegmentation (void)
    {
    };
	///Destruktor.
    virtual ~ EB_ImageSegmentation (void)
    {
//		delete(lut);
    };
//@}
/**@name public Methoden*/
//@{
	/**Vergleichsfarbe setzen. Diese Methode legt die Vergleichsfarbe zum sp�teren 
	Berechnen der Distanz fest.*/
	void setColor(const EB_PixelDescriptor &compcolor)
	{
		color=compcolor;
	}
	/**Distanzberechnung. Diese Methode ist das Herzst�ck der Klasse. Sie ist pur virtuell und mu� von 
	abgeleiteten Klassen �berladen werden. Die Methode mu�  mit einem entsprechenden
	Distanzma� den Abstand zwischen der �bergebenen und der Vergleichsfarbe 
	berechnen und dieses Resultat auf das Intervall zwischen 0.0  und 1.0 normiert zur�ckgeben.
	@param p Farbe, f�r die die Differenz berechnet werden soll.
	@return Abstand von der Vergleichsfarbe.*/
    virtual float computeDistance (EB_PixelDescriptor &p)const =0;
//@}
  protected:
  ///Farbe, gegen die die Distanz berechnet werden soll.
	EB_PixelDescriptor color;
};

#endif

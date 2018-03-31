#ifndef EB_IMAGESEGMENTATION_H
#define EB_IMAGESEGMENTATION_H

/**Diese Klasse stellt eine Schnittstelle für die farbbasierte Segmentation dar.
Diese Segmentation geschieht nicht in dieser Klasse, sie dient lediglich dazu,
den Abstand zwischen zwei Farben zu berechnen. Dabei ist die konkrete Wahl
des Abstandmaßes in einer abgeleiteten  Klasse zu treffen.
@author Jürgen "EL BOSSO" Key*/
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
	/**Vergleichsfarbe setzen. Diese Methode legt die Vergleichsfarbe zum späteren 
	Berechnen der Distanz fest.*/
	void setColor(const EB_PixelDescriptor &compcolor)
	{
		color=compcolor;
	}
	/**Distanzberechnung. Diese Methode ist das Herzstück der Klasse. Sie ist pur virtuell und muß von 
	abgeleiteten Klassen überladen werden. Die Methode muß  mit einem entsprechenden
	Distanzmaß den Abstand zwischen der übergebenen und der Vergleichsfarbe 
	berechnen und dieses Resultat auf das Intervall zwischen 0.0  und 1.0 normiert zurückgeben.
	@param p Farbe, für die die Differenz berechnet werden soll.
	@return Abstand von der Vergleichsfarbe.*/
    virtual float computeDistance (EB_PixelDescriptor &p)const =0;
//@}
  protected:
  ///Farbe, gegen die die Distanz berechnet werden soll.
	EB_PixelDescriptor color;
};

#endif

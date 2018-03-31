#ifndef _EB_PixelDescriptor_h_
#define _EB_PixelDescriptor_h_

#include"EB_Image_Exceptions.h"

/**Diese Klasse implementiert das abstrakte Konzept eines Bildelements (PICture ELement).Sie hängt eng
mit dem mit der Klasse \Ref{EB_Image} eingeführten Konzept zusammen. Ein Bild besteht aus mehreren Bändern, die
zum Beispiel die einzelnen Dimensionen eines Farbraumes darstellen können. Ein Pixel definiert die Farbe des Bildes
an einer ganz bestimmten Stelle durch Zusammenfassen der Intensitätswerte aller Bänder an dieser Stelle. In der Klasse
\Ref{EB_Image} werden die Intensitäten aller Pixel durch Float-Werte dargestellt. Daher enthält ein Pixel so viele
Float-Werte, wie das korrespondierende Bild Bänder enthält.
@author Jürgen "EL BOSSO" Key*/
class EB_PixelDescriptor
{ public:
/**@name Konstruktoren und Destruktor*/
	//@{
	/**Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse \Ref{EB_PixelDescriptor}. @param bands Anzahl an Komponenten in diesem Pixel. @ param initvalue Wert, der allen Komponenten des Pixels zugewiesen wird.*/ 
	EB_PixelDescriptor (unsigned int bands=1, float initvalue = 0.0):componentcount (bands), components (NULL)
    {
	unsigned int b;
	  components = new float[componentcount];

	for (b = 0; b < componentcount; b++)
	      components[b] = initvalue;
    };
	/**Copy-Konstruktor. Dieser Konstruktor erzeugt eine identische Kopie der übergebenen Instanz.
	Die Kopie wird mittels deep copy erzeugt. Beide Instanzen sind vollständig unabhängig.
	@param pd Instanz, von der die Kopie erzeugt werden soll.*/
    EB_PixelDescriptor (const EB_PixelDescriptor & pd):componentcount (pd.componentcount), components (NULL)
    {
	unsigned int b;
	components = new float[componentcount];

	for (b = 0; b < componentcount; b++)
	    components[b] = pd.components[b];
    };
    ///Destruktor.
    virtual ~ EB_PixelDescriptor (void)
    {
	delete (components);
    };
//@}
/**@name Operatoren*/
//@{
	/**Zuweisungsoperator. Dieser Operator erstellt eine Kopie der übergebenen Instanz
	in der aktuellen Instanz. Dabei wird eine deep copy durchgeführt, danach sind beide 
	Instanzen vollständig unabhängig voneinander.
	@param pd Instanz, von der eine Kopie erzeugt werden soll.
	@return Referenz auf die aktuelle Instanz.*/
    EB_PixelDescriptor & operator = (const EB_PixelDescriptor & pd)
    {
	unsigned int b;

	if (this != &pd)
	    {
		delete (components);
		componentcount = pd.componentcount;
		components = new float[componentcount];

		for (b = 0; b < componentcount; b++)
		    components[b] = pd.components[b];
	    }
	return (*this);
    };
	/**Indexoperator. Mit diesem Operator kann man auf die einzelnen Komponenten des Pixels 
	zugreifen. ist der Index ungültig, wird eine Exception geworfen.
	@param index bestimmt, auf welche Komponente zugegriffen werden soll.
	@return Intensitätswert des Pixels im entsprechenden Band.*/
    float &operator[] (unsigned int index) const throw (EBIIndexOutOfRangeEXP)
    {
	if (index < componentcount)
	    return (components[index]);
	else
	    throw EBIIndexOutOfRangeEXP ();
    };
	/**Größe. 
	@return Anzahl an Intensitätswerten in der aktuellen Instanz*/
    unsigned int giveComponentCount (void) const
    { return (componentcount);
    };
//@}
/**@name public Methoden*/
//@{
	/**Größenänderung. Mit dieser Methode kann die Anzahl der Komponenten eines Pixels
	geändert werden. Es ist sowohl eine Verringerung als auch eine Erhöhung der
	Komponentenanzahl möglich. Der vorherige Inhalt des Pixels geht verloren.
	@param newcount neue Komponentenanzahl für diesen Pixel.
	@initvalue Initialisierungswert für alle Komponenten.
	@return Referenz auf die aktuelle Instanz.*/
    EB_PixelDescriptor & changeComponentCount (unsigned int newcount, float initvalue = 0.0)
    {
	unsigned int b;

	delete (components);
	componentcount = newcount;
	components = new float[componentcount];
	for (b = 0; b < componentcount; b++)
	    components[b] = initvalue;
	return (*this);
    };
//@}
  protected:
    ///Anzahl der Komponenten
    unsigned int componentcount;
    ///Zeiger auf das Feld mit den Komponenten
    float *components;

};

#endif

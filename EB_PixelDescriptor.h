#ifndef _EB_PixelDescriptor_h_
#define _EB_PixelDescriptor_h_

#include"EB_Image_Exceptions.h"

/**Diese Klasse implementiert das abstrakte Konzept eines Bildelements (PICture ELement).Sie h�ngt eng
mit dem mit der Klasse \Ref{EB_Image} eingef�hrten Konzept zusammen. Ein Bild besteht aus mehreren B�ndern, die
zum Beispiel die einzelnen Dimensionen eines Farbraumes darstellen k�nnen. Ein Pixel definiert die Farbe des Bildes
an einer ganz bestimmten Stelle durch Zusammenfassen der Intensit�tswerte aller B�nder an dieser Stelle. In der Klasse
\Ref{EB_Image} werden die Intensit�ten aller Pixel durch Float-Werte dargestellt. Daher enth�lt ein Pixel so viele
Float-Werte, wie das korrespondierende Bild B�nder enth�lt.
@author J�rgen "EL BOSSO" Key*/
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
	/**Copy-Konstruktor. Dieser Konstruktor erzeugt eine identische Kopie der �bergebenen Instanz.
	Die Kopie wird mittels deep copy erzeugt. Beide Instanzen sind vollst�ndig unabh�ngig.
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
	/**Zuweisungsoperator. Dieser Operator erstellt eine Kopie der �bergebenen Instanz
	in der aktuellen Instanz. Dabei wird eine deep copy durchgef�hrt, danach sind beide 
	Instanzen vollst�ndig unabh�ngig voneinander.
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
	zugreifen. ist der Index ung�ltig, wird eine Exception geworfen.
	@param index bestimmt, auf welche Komponente zugegriffen werden soll.
	@return Intensit�tswert des Pixels im entsprechenden Band.*/
    float &operator[] (unsigned int index) const throw (EBIIndexOutOfRangeEXP)
    {
	if (index < componentcount)
	    return (components[index]);
	else
	    throw EBIIndexOutOfRangeEXP ();
    };
	/**Gr��e. 
	@return Anzahl an Intensit�tswerten in der aktuellen Instanz*/
    unsigned int giveComponentCount (void) const
    { return (componentcount);
    };
//@}
/**@name public Methoden*/
//@{
	/**Gr��en�nderung. Mit dieser Methode kann die Anzahl der Komponenten eines Pixels
	ge�ndert werden. Es ist sowohl eine Verringerung als auch eine Erh�hung der
	Komponentenanzahl m�glich. Der vorherige Inhalt des Pixels geht verloren.
	@param newcount neue Komponentenanzahl f�r diesen Pixel.
	@initvalue Initialisierungswert f�r alle Komponenten.
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

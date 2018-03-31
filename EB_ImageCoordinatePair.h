#ifndef EB_IMAGECOORDINATEPAIR_H
#define EB_IMAGECOORDINATEPAIR_H

/**Diese Klasse stellt zweidimensionale Koordinaten dar. Sie wird zum Beispiel 
f�r die Implementierung des Features der Beschr�nkung von 
Bildverarbeitungsoperationen auf konvexe Regionen in der Klasse 
\Ref{EB_Image} ben�tigt.
@author J�rgen "EL BOSSO" Key*/
class EB_ImageCoordinatePair 
{
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse 
	\Ref{EB_ImageCoordinatePair}, die die Koordinate (0,0) verk�rpert.*/
    EB_ImageCoordinatePair(void):xposition(0),yposition(0){};
	/**Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse 
	\Ref{EB_ImageCoordinatePair}, die die angegebene Koordinate verk�rpert.
	@param x x-Komponente der Koordinate.
	@param y y-Komponente der Koordinate.*/
    EB_ImageCoordinatePair(int x,int y):xposition(x),yposition(y){};
	///Destruktor.
    virtual ~EB_ImageCoordinatePair(void){};
//@}
	///x-Komponente der Koordinate.
	int xposition;
	///y-Komponente der Koordinate.
	int yposition;
};

#endif

#ifndef EB_TRANSFORMATIONINTERPOLATOR
#define EB_TRANSFORMATIONINTERPOLATOR

#include<EB_Vector.h>

/**Diese Klasse stellt eine Schnittstelle zu beliebigen Interpolatoren bereit.
Instanzen von von dieser Klasse abgeleiteten Klassen benutzt zum Beispiel die
Methode \Ref{EB_Image::transform} der Klasse \Ref{EB_Image} um die Ergebnisse
von Koordinatentransformationen auf Bildern zu verbessern. Die Schnittstelle
ist so konzipiert, daß hier noch gar keine Verarbeitung stattfindet. Hier wird
lediglich das Interface für konkrete Interpolationsalgorithmen definiert.
Darüber hinaus stellt diese Klasse einige administrative Methoden bereit, die
viele Interpolatoren benötigen oder die die Interpolation beschleunigen.
Die Interpolation wird auch von den von dieser Klasse abgeleiteten Klassen 
nicht auf dem Bild vollzogen. Es erfolgt lediglich die Berechnung der Koordinaten der in die Interpolation einzubezieheneden Koordinaten sowie der
Gewichte, mit dem die einzelnen Pixel in das Ergebnis eingehen. Das Ergebnis
sind zwei Vektoren, auf die der Aufrufer danach zurückgreifen kann. 
@author Jürgen EL BOSSO Key*/
class EB_TransformationInterpolator
{
public:
	/**Konstruktor. Dieser Konstruktor reserviert lediglich den zur Speicherung
	der Koordinaten und Gewichte nötigen Speicher.
	@param samplecount Maximale Anzahl der zur Interpolation eines Pixels
	herangezogenen Nachbarn. Diese Anzahl legt die Länge der Vektoren fest.*/
	EB_TransformationInterpolator(unsigned int samplecount):members(samplecount),
		indices(members),
		factors(members),
		width(0),
		height(0)
	{
	};
	///Destruktor
	virtual ~EB_TransformationInterpolator(void){};
	/**Eingangsdimensionen setzen. Mit dieser Methode werden dee aktuellen Instanz die Dimensionen
	der zu transformierenden Daten bekanntgegeben.
	@param w Breite der Eingangsdaten.
	@param h Höhe der Eingangsdaten.*/
	void setSourceDimensions(unsigned int w,unsigned int h)
	{
		width=w;
		height=h;
	};
	/**Interpolation berechnen. Diese Methode ist pur virtuell und muß von
	abgeleiteten Klassen überladen werden. Dabei sollte die Funktion aus
	den gebrochenen Pixelkoordinaten die Koordinaten aller zur Interpolation
	nötigen Nachbarn berechnen und linear kodiert im Vektor \Ref{indices}
	ablegen. Die zugehörigen Gewichte müssen im entsprechenden Element des 
	Vektors\Ref{factors} abgelegt werden.
	@param x gebrochenzahlige Pixelkoordinate.
	@param y gebrochenzahlige Pixelkoordinate.*/
	virtual void calculate(float x,float y)=0;
	/**Interpolatormächtigkeit. Dahinter verbirgt sich letztlich nur die 
	Aussage, wie viele Pixel zur Interpolation herangezogen werden sollen.
	@returns Anzahl der zur Interpolation benutzten Nachbarn.*/
	unsigned int giveSampleCount(void){return(members);};
	/**Positionen ermitteln, Diese Methode liefert einen Vektor, der nach
	Aufruf der Methode \Ref{EB_TransformationInterpolator::calculate} die
	Positionen der für eine spezifische Koordinate benötigten Nachbarn 
	enthält.
	@return Referenz auf einen Vektor, der die Positionen der zur Interpolation
	zu benutzenden Pixel linear kodiert enthält*/
	EB_Vector<unsigned int> &giveIndices(void){return(indices);};
	/**Gewichte ermitteln, Diese Methode liefert einen Vektor, der nach
	Aufruf der Methode \Ref{EB_TransformationInterpolator::calculate} die
	Gewichte der für eine spezifische Koordinate benötigten Nachbarn 
	enthält.
	@return Referenz auf einen Vektor, der die Gewichte der zur Interpolation
	zu benutzenden Pixel linear kodiert enthält*/
	EB_Vector<float> &giveFactors(void){return(factors);};
protected:
	///Maximale Anzahl an Nachbarn, die ein bestimmtes Interpolationsschema benötigt.
	unsigned int members;
	///Vektor, in dem die jeweiligen Positionen der zur Interpolation benutzten Pixel liegen.
	EB_Vector<unsigned int> indices;
	///Vektor, in dem die jeweiligen Gewichte der zur Interpolation benutzten Pixel liegen.	
	EB_Vector<float> factors;
	///Breite des bearbeiteten Bildes (wird für die lineare Kodierung benötigt)
	unsigned int width;
	///Höhe des bearbeiteten Bildes (wird für die lineare Kodierung benötigt)
	unsigned int height;
};

#endif

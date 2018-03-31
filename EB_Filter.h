#ifndef EB_FILTER_H
#define EB_FILTER_H

#include <iostream>
#ifndef EB_BAND_H
#include"EB_Band.h"
#endif


/**Callbackfunktion zur Berechnung von Filterkoeffizienten in Abh�ngigkeit
von der Position $(x,y)$ in der Filtermaske f�r die Klasse \Ref{EB_Filter}.*/
typedef float (*cffuncptr) (unsigned int x, unsigned int y,unsigned int width,unsigned int height);

/**Diese Klasse stellt die Implementation von zweidimensionalen Filtern mit
beliebigen Abmessungen dar. Es werden nur Methoden zur Organisation von 
und zum Zugriff auf Filtermasken zur Verf�gung gestellt. Die Filtermasken 
d�rfen beliebige Abmessungen haben.
@author J�rgen "EL BOSSO" Key*/
class EB_Filter:public EB_Band
{
public: 
/**@name Konstruktoren und Destruktor*/
//@{
  /**Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
  \Ref{EB_Filter} mit vorgegebenen Kantenl�ngen. Ist eine der Kantenl�ngen 
  gerade, wird die n�chstgr��ere ungerade Zahl benutzt, also zum Beispiel 5
  statt 4. 
  @param x Breite der Filtermaske.
  @param y H�he der Filtermaske.*/ 
  EB_Filter(unsigned int x,unsigned int y);
    /**Copy-Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse 
    \Ref{EB_Filter} als Kopie der �bergebenen Instanz.
	@param other Instanz, die zum Erstellen der Kopie benutzt wird.*/
  EB_Filter(const EB_Filter &other);
  ///Destruktor.
  virtual ~EB_Filter(){};
//@}
/**@name Operatoren*/
//@{
	/**Zuweisungsoperator. Dieser Operator erzeugt eine Kopie der �bergebenen
  in der aktuellen Instanz. Dies ist eine Deep Copy: Es existieren danach
  zwei v�llig unabh�ngige Instanzen.
  @param other Instanz, die kopiert werden soll.
  @return Referenz auf die aktuelle Instanz*/
  EB_Filter &operator=(const EB_Filter &other);
//@}
/**@name public Methoden*/
//@{
	/**Einzelnen Filterkoeffizienten setzen. Diese Methode erlaubt es,
	effizient Filtermasken zu definieren, die symmetrisch bez�glich
	der x- und y-Achse sind. Der �bergebene Wert wird an der angegebenen
	Stelle und an den entsprechenden an den Achsen gespiegelten Stellen,
	insgesamt also viermal in die Maske eingetragen.
	@param x Spalte der Filtermaske.
	@param y Zeile der Filtermaske.
	@param coefficient Wert f�r den Filterkoeffizienten an der entsprechenden
	Stelle.*/
  void setValueSquareSymm(unsigned int x, unsigned int y, float coefficient);
	/**Einzelnen Filterkoeffizienten setzen. Diese Methode erlaubt es,
	effizient Filtermasken zu definieren, die symmetrisch bez�glich
	der x-Achse sind. Der �bergebene Wert wird an der angegebenen
	Stelle und an den entsprechenden an der x-Achse gespiegelten Stelle,
	insgesamt also zweimal in die Maske eingetragen.
	@param x Spalte der Filtermaske.
	@param y Zeile der Filtermaske.
	@param coefficient Wert f�r den Filterkoeffizienten an der entsprechenden
	Stelle.*/
  void setValueXSymm(unsigned int x, unsigned int y, float coefficient);
	/**Einzelnen Filterkoeffizienten setzen. Diese Methode erlaubt es,
	effizient Filtermasken zu definieren, die symmetrisch bez�glich
	der y-Achse sind. Der �bergebene Wert wird an der angegebenen
	Stelle und an den entsprechenden an der y-Achse gespiegelten Stelle,
	insgesamt also zweimal in die Maske eingetragen.
	@param x Spalte der Filtermaske.
	@param y Zeile der Filtermaske.
	@param coefficient Wert f�r den Filterkoeffizienten an der entsprechenden
	Stelle.*/
  void setValueYSymm(unsigned int x, unsigned int y, float coefficient);
	/**Filterkoeffizienten durch Funktion definieren. Diese Methode erlaubt es,
	Filtermasken entsprechend einer Funktion zu definieren. 
	Die Funktion mu� zwei int Argumente annehmen, die die Position 
	des zu berechnenden Koeffizienten darstellen  und den Filterkoeffizienten
	als float zur�ckliefern.
	@param function Zeiger auf die Funktion, die die Koeffizienten berechnet.
	@param y Zeile der Filtermaske.
	@param coefficient Wert f�r den Filterkoeffizienten an der entsprechenden
	Stelle.*/
  void setValueFunc(cffuncptr function);
  float givePosSum(void)const;
  float giveNegSum(void)const;
//@}
 protected:
//  unsigned int semiwidth;
//  unsigned int semiheight;
};

#endif

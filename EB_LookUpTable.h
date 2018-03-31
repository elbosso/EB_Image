/****************************************************************************
*
* File        : LookUpTable.h
* Class       : LookUpTable
*  based on   :   
*  uses       :   
*
* Author      : VIP-Projektbearbeiter <jkey@cortex.informatik.tu-ilmenau.de>
*               TU Ilmenau / Fak. IA / FG Neuroinformatik
* Date        : Wed May 27 1998 created
*
* Version     : 0.03
* State       : final pre-alpha
*
* Description : look-up table used in classes for image processing
*
****************************************************************************/

#ifndef _EB_LOOKUPTABLE_H
#define _EB_LOOKUPTABLE_H

/***************************************************************************
 * Used Headerfiles
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "EB_Image_Exceptions.h"
#include "EB_IntensityTransformation.h"

/**Diese Klasse stellt eine abstrakte Implementation einer 
Lookuptable dar. Die Tabelle wird mit Integerzahlen indiziert und
enth�lt Gleitkommawerte. 
@author J�rgen "EL BOSSO" Key*/
class EB_LookUpTable
{  
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_LookUpTable}mit der festgelegten Anzahl an Eintr�gen.
	@param number Gew�nschte Anzahl der Eintr�ge.*/
  EB_LookUpTable(unsigned int number);
	/**Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_LookUpTable}mit der festgelegten Anzahl an Eintr�gen.
	Alle Eintr�ge in der Tabelle werden mit dem angegebenen
	Wert initialisiert.
	@param number Gew�nschte Anzahl der Eintr�ge.
	@param initvalue Wert, mit dem die Eintr�ge in der Tabelle 
	initialisiert werden sollen.*/
  EB_LookUpTable(unsigned int number,float initvalue);
	/**Copy-Konstruktor. Dieser Konstruktor erzeugt eine identische Kopie der 
	�bergebenen Instanz.
	Die Kopie wird mittels deep copy erzeugt. Beide Instanzen sind vollst�ndig unabh�ngig.
	@param other Instanz, von der die Kopie erzeugt werden soll.*/
  EB_LookUpTable(const EB_LookUpTable &other);
	///Destruktor.
  virtual ~EB_LookUpTable();
//@}
/**@name Operatoren*/
//@{
	/**Zuweisungsoperator. Dieser Operator erstellt eine Kopie der �bergebenen 
	Instanz
	in der aktuellen Instanz. Dabei wird eine deep copy durchgef�hrt, 
	danach sind beide 
	Instanzen vollst�ndig unabh�ngig voneinander.
	@param other Instanz, von der eine Kopie erzeugt werden soll.
	@return Referenz auf die aktuelle Instanz.*/
  EB_LookUpTable &operator=(const EB_LookUpTable &other);
//@}
/**@name public Methoden*/
//@{
	/**Eintragszahl �ndern. Mit dieser Methode l��t sich die Anzahl 
	der Eintr�ge in der Tabelle nachtr�glich �ndern. Die bereits
	in der Tabelle enthaltenen Eintr�ge gehen dann verloren.
	@param newsize Neue Anzahl an Eintr�gen f�r die Tabelle.*/
  void redimension(unsigned int newsize);
	/**Eintragszahl �ndern. Mit dieser Methode l��t sich die Anzahl 
	der Eintr�ge in der Tabelle nachtr�glich �ndern. Die bereits
	in der Tabelle enthaltenen Eintr�ge gehen dann verloren.
	Alle Eintr�ge in der Tabelle werden mit dem angegebenen
	Wert initialisiert.
	@param newsize Neue Anzahl an Eintr�gen f�r die Tabelle.
	@param initvalue Wert, mit dem die Eintr�ge in der Tabelle 
	initialisiert werden sollen.*/
  void redimension(unsigned int newsize,float initvalue);
  /**Tabellenwert holen. Diese Methode gibt den in der Tabelle am 
  spezifizierten Platz stehenden Wert zur�ck.
  @param index Position in der Tabelle, deren Wert abgefragt werden soll.
  Zeigt dieser Wert auf eine Position au�erhalb der Tabelle, wird eine Exception 
  geworfen. 
  @return Gleitkommawert aus der Tabelle.*/
  float giveValue(unsigned int index) const throw(EBIIndexOutOfRangeEXP);
  /**Abfrage Eitragsanzahl. Diese Methode liefert die Anzahl der Eintr�ge in der 
  Tabelle.
  @return Die Anzahl der Eintr�ge in der Tabelle.*/
  unsigned int giveEntryCount(void)const {return(entrycount);};
  /**Tabellenwert setzen. Diese Methode erlaubt es, den Tabellenwert am 
  spezifizierten Platz neu zu setzen.
  @param index Position in der Tabelle, deren Wert ge�ndert  werden soll.
	@param value Wert, der an der angegebenen Position in die Tabelle 
	geschrieben werden soll.*/  
  void setValue(unsigned int index,float value){if(index<entrycount)table[index]=value;};
	/**Initialisierung. Diese Methode erlaubt es, alle Eintr�ge der Tabelle
	auf einmal mit einem Wert zu beschreiben.
	@param value Wert, mit dem die Tabelleneintr�ge initialisiert werden sollen.*/
  void init(float value);
  /**Gammakorrektur. Diese Methode berechnet die Eintr�ge f�r die Tabelle so,
  da� sie dem Verlauf der Gammakorrekturfunktion entsprechen.
  @param factor Argument der Gammafunktion.*/
  void calculateGammas(float factor);
  /**Kontrastkorrektur. Diese Methode berechnet die Eintr�ge in der Tabelle so, 
  da� sie auf ein Bild angewendet eine �nderung des Kontrastes bewirken
  w�rden.*/
  void calculateContrast(float factor);
  /**Nichtlineare Funktion. Mit dieser Methode wird die Look-Up-Tabelle mit
  Eintr�gen entsprechend der Transformationsvorschrift in trans gef�llt.
  Dabei wird der Bereich der Funktion von 0 bis 1 benutzt.
  @param trans Instanz einer von \Ref{EB_IntensityTransformation} abgeleiteten
  Klasse, die die eigentliche Funktion repr�sentiert.*/
	void calculateIntensityTransformation(const EB_IntensityTransformation &trans);
//@}
protected:
  /*geschuetzte Member*/
  //Anzahl an Eintr�gen in der Tabelle.
  unsigned int entrycount;
  //Die Tabelle selbst.
  float *table;
};


#endif /* __LOOKUPTABLE_H */

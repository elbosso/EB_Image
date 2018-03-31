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
enthält Gleitkommawerte. 
@author Jürgen "EL BOSSO" Key*/
class EB_LookUpTable
{  
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_LookUpTable}mit der festgelegten Anzahl an Einträgen.
	@param number Gewünschte Anzahl der Einträge.*/
  EB_LookUpTable(unsigned int number);
	/**Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_LookUpTable}mit der festgelegten Anzahl an Einträgen.
	Alle Einträge in der Tabelle werden mit dem angegebenen
	Wert initialisiert.
	@param number Gewünschte Anzahl der Einträge.
	@param initvalue Wert, mit dem die Einträge in der Tabelle 
	initialisiert werden sollen.*/
  EB_LookUpTable(unsigned int number,float initvalue);
	/**Copy-Konstruktor. Dieser Konstruktor erzeugt eine identische Kopie der 
	übergebenen Instanz.
	Die Kopie wird mittels deep copy erzeugt. Beide Instanzen sind vollständig unabhängig.
	@param other Instanz, von der die Kopie erzeugt werden soll.*/
  EB_LookUpTable(const EB_LookUpTable &other);
	///Destruktor.
  virtual ~EB_LookUpTable();
//@}
/**@name Operatoren*/
//@{
	/**Zuweisungsoperator. Dieser Operator erstellt eine Kopie der übergebenen 
	Instanz
	in der aktuellen Instanz. Dabei wird eine deep copy durchgeführt, 
	danach sind beide 
	Instanzen vollständig unabhängig voneinander.
	@param other Instanz, von der eine Kopie erzeugt werden soll.
	@return Referenz auf die aktuelle Instanz.*/
  EB_LookUpTable &operator=(const EB_LookUpTable &other);
//@}
/**@name public Methoden*/
//@{
	/**Eintragszahl ändern. Mit dieser Methode läßt sich die Anzahl 
	der Einträge in der Tabelle nachträglich ändern. Die bereits
	in der Tabelle enthaltenen Einträge gehen dann verloren.
	@param newsize Neue Anzahl an Einträgen für die Tabelle.*/
  void redimension(unsigned int newsize);
	/**Eintragszahl ändern. Mit dieser Methode läßt sich die Anzahl 
	der Einträge in der Tabelle nachträglich ändern. Die bereits
	in der Tabelle enthaltenen Einträge gehen dann verloren.
	Alle Einträge in der Tabelle werden mit dem angegebenen
	Wert initialisiert.
	@param newsize Neue Anzahl an Einträgen für die Tabelle.
	@param initvalue Wert, mit dem die Einträge in der Tabelle 
	initialisiert werden sollen.*/
  void redimension(unsigned int newsize,float initvalue);
  /**Tabellenwert holen. Diese Methode gibt den in der Tabelle am 
  spezifizierten Platz stehenden Wert zurück.
  @param index Position in der Tabelle, deren Wert abgefragt werden soll.
  Zeigt dieser Wert auf eine Position außerhalb der Tabelle, wird eine Exception 
  geworfen. 
  @return Gleitkommawert aus der Tabelle.*/
  float giveValue(unsigned int index) const throw(EBIIndexOutOfRangeEXP);
  /**Abfrage Eitragsanzahl. Diese Methode liefert die Anzahl der Einträge in der 
  Tabelle.
  @return Die Anzahl der Einträge in der Tabelle.*/
  unsigned int giveEntryCount(void)const {return(entrycount);};
  /**Tabellenwert setzen. Diese Methode erlaubt es, den Tabellenwert am 
  spezifizierten Platz neu zu setzen.
  @param index Position in der Tabelle, deren Wert geändert  werden soll.
	@param value Wert, der an der angegebenen Position in die Tabelle 
	geschrieben werden soll.*/  
  void setValue(unsigned int index,float value){if(index<entrycount)table[index]=value;};
	/**Initialisierung. Diese Methode erlaubt es, alle Einträge der Tabelle
	auf einmal mit einem Wert zu beschreiben.
	@param value Wert, mit dem die Tabelleneinträge initialisiert werden sollen.*/
  void init(float value);
  /**Gammakorrektur. Diese Methode berechnet die Einträge für die Tabelle so,
  daß sie dem Verlauf der Gammakorrekturfunktion entsprechen.
  @param factor Argument der Gammafunktion.*/
  void calculateGammas(float factor);
  /**Kontrastkorrektur. Diese Methode berechnet die Einträge in der Tabelle so, 
  daß sie auf ein Bild angewendet eine Änderung des Kontrastes bewirken
  würden.*/
  void calculateContrast(float factor);
  /**Nichtlineare Funktion. Mit dieser Methode wird die Look-Up-Tabelle mit
  Einträgen entsprechend der Transformationsvorschrift in trans gefüllt.
  Dabei wird der Bereich der Funktion von 0 bis 1 benutzt.
  @param trans Instanz einer von \Ref{EB_IntensityTransformation} abgeleiteten
  Klasse, die die eigentliche Funktion repräsentiert.*/
	void calculateIntensityTransformation(const EB_IntensityTransformation &trans);
//@}
protected:
  /*geschuetzte Member*/
  //Anzahl an Einträgen in der Tabelle.
  unsigned int entrycount;
  //Die Tabelle selbst.
  float *table;
};


#endif /* __LOOKUPTABLE_H */

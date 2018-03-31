/****************************************************************************
*
* File        : EB_Image.h
* Class       : EB_Image
*  based on   :   
*  uses       : EB_Band 
*
* Author      : VIP-Projektbearbeiter <jkey@cortex.informatik.tu-ilmenau.de>
*               TU Ilmenau / Fak. IA / FG Neuroinformatik
* Date        : Tue May 26 1998 created
*
* Version     : 0.15
* State       : final pre alpha
*
* Description : This class implements some well known methods from the 
*               classical view of image processing. 
*
****************************************************************************/

#ifndef EB_IMAGE_H
#define EB_IMAGE_H

/***************************************************************************
 * Used Headerfiles
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>

#include "EB_Image_Exceptions.h"
#include "EB_Band.h"
#include "PK_EB_Kohonen.h"
#include "EB_PixelDescriptor.h"
#include "EB_ImageRegion.h"
#include "EB_ImageAdvancement.h"
#include "EB_ImageTransformation.h"
#include "EB_IntensityTransformation.h"
#include "EB_TransformationInterpolator.h"
#include "EB_ImageSegmentation.h"
#include "EB_Vector.h"
class EB_Filter;

#define TORED 1
#define TOGREEN 2
#define TOBLUE 3
#define TOGRAY 4
#define TOYELLOW 5
#define TOPURPLE 6
#define TOTURQUOIS 7
#define BLACKORANGEGREEN 8
#define YELLOWGREENRED 9
#define BLACKGREENRED 10
#define YELLOWBROWNGREEN 11
#define TOTEST 12

#define RED 0
#define GREEN 1
#define BLUE 2

struct EB_Image_Info
{
	unsigned int width;
	unsigned int height;
	unsigned int bandcount;
};

/**Diese Klasse stellt Bilder als abstrakte Konzepte dar. Die Bilder sind alle 
rechteckig parallel zur x- und y-Achse. Es k�nnen beliebig viele B�nder
(Instanzen der Klasse \Ref{EB_Band}) in einem Bild vorhanden sein. F�r ein Bild,
dessen Pixel zum Beispiel durch je einen Rot- Gr�n- und Blau-Farbwert beschrieben
werden, m��te das Bild drei B�nder enthalten. F�r jede Instanz l��t sich 
eine Region definieren, auf die die Bildbearbeitungsoperationen beschr�nkt werden
sollen (bei manchen Operationen ist das nicht sinnvoll). Diese Regionen sind 
Instanzen der Klasse \Ref{EB_ImageRegion}. Zur Zeit funktionieren nur 
Regionen, deren begrenzende Liniensegmente einander nicht schneiden.
@author J�rgen "EL BOSSO" Key*/
class EB_Image
{
  public:
/**@name Konstruktoren und Destruktor*/
//@{
/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse 
	\Ref{EB_Image},die keine B � nder enth � lt.Damit hat er eine sehr geringe Zeit - und 
	Platzkomplexit�t.*/ 
	EB_Image (void);
 /**Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{EB_Image}.
 @param x Breite des Bildes.
 @param y H�he des Bildes.
 @param usedbands Anzahl an B�ndern im Bild.
 @param color Intensit�tswert, mit dem alle Pixel aller B�nder initialisiert werden.
 Liegt dieser Wert au�erhalb des durch min und max bestimmten Intervalls,
 findet keine Initialisierung statt. Voreinstellung ist -1.0.
 @param min Minimaler Intensit�tswert in jedem Band. Voreinstellung ist 0.0.
 @param max Maximaler Intensit�tswert in jedem Band. Voreinstellung ist 1.0.*/
    EB_Image (unsigned int x, unsigned int y, unsigned int usedbands, float color = -1.0, float min = 0.0, float max = 1.0);
 /**Copy-Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{EB_Image} als Kopie von der �bergebenen Instanz.
 @param Image Instanz, von der die Kopie erzeugt wird.*/
    EB_Image (const EB_Image & Image);
 /**Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{EB_Image} als Kopie von der �bergebenen Instanz der Klasse 
 \Ref{EB_Band}. Als Resultat entsteht ein Bild mit nur einem Band, welches 
 eine Kopie der �bergebenen Instanz darstellt.
 @param Band Instanz, die zum Erstellen des Images benutzt wird.*/
    EB_Image (const EB_Band & Band);
 /**Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{EB_Image} als Kopie Instanz der Klasse 
 \Ref{EB_Band}, auf die der Zeiger verweist. Als Resultat 
 entsteht ein Bild mit nur einem Band, welches 
 eine Kopie der �bergebenen Instanz darstellt.
 @param Band Zeiger auf eine Instanz, die zum Erstellen des Images benutzt wird.*/
    EB_Image (const EB_Band * Band);
 /**File-Konstruktor. Dieser Konstruktor entspricht dem parameterlosen 
 Konstruktor.*/
    EB_Image (const char *name);
    ///Destruktor
    virtual ~ EB_Image ();
    //@}
  /**@name Operatoren*/
    //@{
  /**Zuweisungsoperator. Dieser Operator erzeugt eine Kopie der �bergebenen
  in der aktuellen Instanz. Dies ist eine Deep Copy: Es existieren danach
  zwei v�llig unabh�ngige Instanzen.
  @param Image Instanz, die kopiert werden soll.
  @return Referenz auf die aktuelle Instanz*/
    EB_Image & operator = (const EB_Image & Image);
    //@}
  /**@name public Methoden*/
    //@{
  /**Breite. Diese Methode liefert die Breite des Bildes in der aktuellen Instanz.
  @return Breite der aktuellen Instanz.*/
    unsigned int giveWidth (void) const
    {
	return (width);
    };
  /**H�he. Diese Methode liefert die H�he des Bildes in der aktuellen Instanz.
  @return H�he der aktuellen Instanz.*/
    unsigned int giveHeight (void) const
    {
	return (height);
    };
  /**Maximale Intensit�t. Diese Methode liefert den maximalen Intensit�tswert 
  der aktuellen Instanz.
  @return Maximaler Intensit�tswert der aktuellen Instanz.*/
    float giveMaxFloat (void) const
    {
	return (maxfloat);
    };
  /**Minimale Intensit�t. Diese Methode liefert den minimalen Intensit�tswert 
  der aktuellen Instanz.
  @return Minimaler Intensit�tswert der aktuellen Instanz.*/
    float giveMinFloat (void) const
    {
	return (minfloat);
    };
  /**Bandanzahl. Diese Methode liefert die Anzahl an B�ndern im Bild.
  @return Anzahl B�nder im Bild.*/
    unsigned int giveBandCount (void) const
    {
	return (pixel.giveComponentCount ());
    };
  /**B�nder hinzuf�gen. Mit dieser Methode ist es m�glich, die B�nder eines 
  anderen Bildes hinter denen der aktuellen Instanz dieser hinzuzuf�gen.
  Dabei werden die B�nder hinter den bereits vorhandenen angeh�ngt. Hatte die
  aktuelle Instanz also beispielsweise bereits 3 B�nder (Indizes 0-2), so 
  ist nach dieser Operation das zweite Band der Instanz source jetzt das mit 
  Index 4 in der aktuellen Instanz. Die Kopieen werden mittels deep copy
  angelegt, die beiden Instanzen sind nach der Operation vollst�ndig unabh�ngig
  voneinander.
  @param source Instanz, von der die B�nder �bernommen werden sollen.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & addBands (const EB_Image & source)throw(EBIImageCorruptedEXP);
  /**Band hinzuf�gen. Mit dieser Methode ist es m�glich, ein Band 
  hinter denen der aktuellen Instanz dieser hinzuzuf�gen.
  Dabei wird das Band hinter den bereits vorhandenen angeh�ngt. Hatte die
  aktuelle Instanz also beispielsweise bereits 3 B�nder (Indizes 0-2), so 
  ist nach dieser Operation der Index f�r das neue Band 3. 
  Das Band wird mittels deep copy 
  kopiert, die beiden Instanzen sind nach der Operation vollst�ndig unabh�ngig
  voneinander.
  @param source Instanz, die zur aktuellen Instanz hinzugef�gt werden soll.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & addBand (const EB_Band & source)throw(EBIImageCorruptedEXP);
  /**Band ersetzen. Mit dieser Methode ist es m�glich, ein Band 
  in der aktuellen Instanz zu ersetzen.
  Dazu wird das zu ersetzende Band zerst�rt und an seiner Stelle mittels Copy-
  Konstruktor von der �bergebenen Instanz ein neues erstellt. 
  @param source Instanz, die zur aktuellen Instanz hinzugef�gt werden soll.
  @param index Nummer des Bandes, das ersetzt werden soll. Existiert kein Band
  mit diesem Index, wird eine Exception geworfen.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & overwriteBand (const EB_Band & source, unsigned int index) throw (EBIIndexOutOfRangeEXP,EBIImageCorruptedEXP);
  /**Handle auf Band. Diese Methode liefert einen Pointer auf das Band
  mit dem entsprechenden Index in der aktuellen Instanz
  @param index Nummer des Bandes, das ersetzt werden soll. Existiert kein Band
  mit diesem Index, wird eine Exception geworfen.
  @return Zeiger auf eine Instanz der Klasse \Ref{EB_Band}*/
    EB_Band *giveBand (unsigned int index) const throw (EBIIndexOutOfRangeEXP);
  /**Komponieren von Bildern. Diese Methode �bertr�gt den Inhalt des von
  der aktuellen Instanz verk�rperten Bildes in ein anderes. Dabei kann man noch einen 
  Offset angeben, der die Position des Bildes im Zielbild angibt. Dieser Offset
  bezieht sich auf die linke obere Ecke. Sind im aktuellen Bild keine B�nder vorhanden
  oder stimmen die Anzahlen der B�nder beider Bilder nicht �berein, werden Exceptions
  geworfen. Das Bild in der aktuellen Instanz wird hierbei nicht ver�ndert
  @param other Instanz der Klasse \Ref{EB_Image} und Ziel der Kopie.
  @param left Offset vom linken Rand f�r die Kopie.
  @param top Offset vom oberen Rand f�r die Kopie.
  @return Referenz auf other.*/
    EB_Image & copyWholeTo (EB_Image & other, int left, int top) throw (EBINoValuesInBandEXP, EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
  /**Komponieren von Bildern. Diese Methode �bertr�gt den Inhalt des von
  der Zielinstanz verk�rperten Bildes in die aktuelle Instanz. Dabei kann man noch einen 
  Offset angeben, der die Position des Bildes im Zielbild angibt. Dieser Offset
  bezieht sich auf die linke obere Ecke. Stimmen die Anzahlen der B�nder beider 
  Bilder nicht �berein oder enth�lt ein Band eines der beiden Bilder keine
  Daten, werden Exceptions
  geworfen. Das Bild in der aktuellen Instanz wird ver�ndert.
  @param other stanz der Klasse \Ref{EB_Image} und Quelle der Kopie.
  @param left Offset vom linken Rand f�r die Kopie.
  @param top Offset vom oberen Rand f�r die Kopie.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & copyWholeFrom (const EB_Image & other, int left, int top) throw (EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
  /**Dynamikbereich �ndern. Mit dieser Methode werden nachtr�glich die Grenzen
  der Intensit�tswerte in den einzelnen B�ndern ge�ndert. Dabei wird nicht abgeschnitten,
  sondern skaliert. Wird der Dynamikbereich von 0 bis 10 auf 0 bis 1 ge�ndert,
  wird der Intensit�tswert eines Pixels von beispielsweise vorher 5 auf 
  0.5 ge�ndert. Das Bild in der aktuellen Instanz wird nicht ver�ndert. 
  @param newmin Minimaler Intensit�tswert in jedem Band.
  @param newmax Maximaler Intensit�tswert in jedem Band.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & rescaleBands (float newmin, float newmax);
  /**Farbraumkonversion. Diese Methode erlaubt es, ein Bild in den L*a*b- 
  Farbraum zu konvertieren. Dabei wird angenommen, da� das Ausgangsbild im 
  RGB-Farbraum lag. Das Bild in der aktuellen Instanz wird 
  ver�ndert. Hat die aktuelle Instanz nicht genau drei B�nder, wird eine 
  Exception geworfen.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
	EB_Image &convertRGBToLAB (void) throw (EBIWrongNumberOfBandsEXP);
  /**Farbraumkonversion. Diese Methode erlaubt es, ein Bild in den RGB- 
  Farbraum zu konvertieren. Dabei wird angenommen, da� das Ausgangsbild im 
  L*a*b-Farbraum lag. Das Bild in der aktuellen Instanz wird 
  ver�ndert. Hat die aktuelle Instanz nicht genau drei B�nder, wird eine 
  Exception geworfen.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
	EB_Image &convertLABToRGB (void) throw (EBIWrongNumberOfBandsEXP);
	/**Grauwertwandlung. Diese Methode wandelt die Intensit�ten aller
	B�nder des Bildes in einen Intensit�tswert um. Bei einem normalen
	RGB Bild entspricht das der Umwandlung in das entsprechende
	Grauwertbild. Es ist m�glich, f�r jedes Band Faktoren einzugeben, mit
	denen das jeweilige Band gewichtet werden soll. 
	@param p Instanz der Klasse \Ref{EB_PixelDescriptor}, die die Faktoren
	f�r die entsprechenden B�nder enth�lt. Enth�lt sie weniger Werte als B�nder
	vorhanden sind, wird f�r die �berz�hligen der letzte wert in p benutzt.
	@return Instanz der Klasse \Ref{EB_Image} mit genau einem Band.*/	
	EB_Image toGray(const EB_PixelDescriptor &p=EB_PixelDescriptor(1,1.0));
  /**Band extrahieren. Diese Methode erlaubt es, ein Band der aktuellen Instanz 
  zu extrahieren. Daraus wird dann eine neue Instanz der Klasse \Ref{EB_Image}
  konstruiert. Das entsprechende Band wird mittels deep copy kopiert, 
  so da� beide Instanzen vollkommen unabh�ngig voneinander sind. Das 
  Bild in der aktuellen Instanz wird nicht ver�ndert. Ist in der aktuellen
  Instanz kein Band definiert oder ist f�r diesen Index kein Band
  definiert, wird eine Exception geworfen. 
  @param index Nummer des Bandes, das extrahiert werden soll. Liegt dieser
  Wert �ber der Anzahl an B�ndern im Bild, wird das erste Band benutzt.
  @return Instanz der Klasse \Ref{EB_Image} mit nur einem Band.*/
    EB_Image &extractSingleBand (unsigned int index)throw(EBINoValuesInBandEXP,EBIIndexOutOfRangeEXP);
  /**Band extrahieren. Diese Methode erlaubt es, ein Band der aktuellen Instanz 
  zu extrahieren. Daraus wird dann eine Falschfarbendarstellung 
  der Intensit�ten des Ursprungsbandes in einer neuen Instanz der Klasse 
  \Ref{EB_Image} im RGB-Farbraum. Die neue und die aktuelle Instanz sind
  anschlie�end vollkommen unabh�ngig voneinander. Das 
  Bild in der aktuellen Instanz wird nicht ver�ndert. Ist in der aktuellen
  Instanz kein Band definiert oder ist f�r diesen Index kein Band
  definiert, wird eine Exception geworfen. 
  @param index Nummer des Bandes, das extrahiert werden soll. Liegt dieser
  Wert �ber der Anzahl an B�ndern im Bild, wird das erste Band benutzt.
  @param mode Falschfarbenmodus.
  Hier sind bisher folgende Modi definiert:
  \begin{itemize}
  		\item TORED
		\item TOGREEN
		\item TOBLUE
		\item TOGRAY
		\item TOYELLOW
		\item TOPURPLE
		\item TOTURQUOIS
		\item BLACKORANGEGREEN
		\item YELLOWGREENRED
		\item BLACKGREENRED
		\item YELLOWBROWNGREEN
  \end{itemize}
  @return Instanz der Klasse \Ref{EB_Image} mit drei B�ndern.*/
    EB_Image extractSingleBandAsRGB (unsigned int index, unsigned int mode = TOGRAY)throw(EBINoValuesInBandEXP,EBIIndexOutOfRangeEXP);
  /**Bildausschnitt extrahieren. Diese Methode gestattet es. einen definierten, 
  rechteckigen Bereich des durch die aktuelle Instanz verk�rperten Bildes 
  auszuschneiden und in einer neuen Instanz zu speichern. Die Parameter 
  beschereiben dabei die linke obere Ecke sowie die Breite und H�he des 
  Ausschnittes. Ragt der Ausschnitt dabei �ber das Bild hinaus, wird den
  entsprechenden Pixeln der niedrigstm�gliche Intensit�tswert zugewiesen.
  Stimmen die Anzahlen der B�nder beider 
  Bilder nicht �berein oder enth�lt ein Band eines der beiden Bilder keine
  Daten, werden Exceptions
  geworfen.Das Bild in der aktuellen Instanz wird nicht ver�ndert.
  @param xstart x-Koordinate der linken oberen Ecke des Ausschnittes.
  @param ystart y-Koordinate der linken oberen Ecke des Ausschnittes.
  @param xdim Breite des Ausschnittes.
  @param ydim H�he des Ausschnittes.
  @return Instanz der Klasse \Ref{EB_Image}.*/
    EB_Image cutoutAPiece (int xstart, int ystart, unsigned int xdim, unsigned int ydim)throw (EBINoValuesInBandEXP, EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
  /**Histogrammausgleich. Diese Methode f�hrt einen Histogrammausgleich auf
  dem Bild aus. Dabei wird jedes Band separat behandelt.
  Das Bild in der aktuellen Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & aequalize (void);
  /**Gammakorrektur. Diese Methode f�hrt eine Gammakorrektur auf
  dem Bild aus. Dies ist eine �ber der Intensit�t nichtlineare 
  Helligkeits�nderung. Dabei wird jedes Band separat behandelt.
  Das Bild in der aktuellen Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param factor Parameter f�r die Gammakorrektur. Ist dieser Wert kleiner 
  als eins, wird das Bild dunkler, ansonsten heller.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & gammaCorrect (float factor);
  /**Kontrast�nderung. Diese Methode f�hrt eine Kontrast�nderung auf
  dem Bild aus. Dabei wird jedes Band separat behandelt.
  Das Bild in der aktuellen Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param factor Parameter f�r die Kontrast�nderung. Je h�her dieser Wert ist, desto 
  mehr wird der Kontrast verst�rkt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & changeContrast (float factor);
  /**Intensit�ts�nderung. Diese Methode f�hrt in den einzelnen B�ndern
  eine Intensit�ts�nderung 
  entsprechend der nichtlinearen Funktion in trans aus. Dabei dienen die
  Intensit�ten als Argumente f�r diese Funktion und die Ergebnisse der Funktion
  als neue Intensit�ten. Jedes Band wird separat behandelt.
  Der Intensit�tsbereich wird in 256 Stufen diskretisiert um die Operation mittels einer Look-Up-Tabelle beschleunigen zu k�nnen.
  Die aktuelle Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param trans Instanz einer von \Ref{EB_IntensityTransformation} abgeleiteten
  Klasse, die die eigentliche Funktion repr�sentiert.
  @return Referenz auf die aktuelle Instanz.*/
EB_Image &transformWithLUT(const EB_IntensityTransformation &trans);
  /**Intensit�ts�nderung. Diese Methode f�hrt in den einzelnen B�ndern
  eine Intensit�ts�nderung 
  entsprechend der nichtlinearen Funktion in trans aus. Dabei dienen die
  Intensit�ten als Argumente f�r diese Funktion und die Ergebnisse der Funktion
  als neue Intensit�ten. Jedes Band wird separat behandelt.
  Die aktuelle Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param trans Instanz einer von \Ref{EB_IntensityTransformation} abgeleiteten
  Klasse, die die eigentliche Funktion repr�sentiert.
  @return Referenz auf die aktuelle Instanz.*/
EB_Image &transform(const EB_IntensityTransformation &trans);
  /**Koordinatentransformation. Das Bild wird entsprechend der Vorschrift im 
  Transformationsobjekt transformiert. Jedes Band wird separat behandelt.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  Die aktuelle Instanz wird ver�ndert.
  @param trans Instanz der Klasse \Ref{EB_ImageTransformation} oder von ihr
  abgeleiteter Klassen, die die Transformation beschreibt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & transform (const EB_ImageTransformation &trans);
  /**Koordinatentransformation. Das Bild wird entsprechend der Vorschrift im 
  Transformationsobjekt transformiert.Dabei wird das Bild entsprechend
  der Vorschrift im Interpolationsobjekt interpoliert.
  Jedes Band wird separat behandelt.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  Die aktuelle Instanz wird ver�ndert.
  @param trans Instanz der Klasse \Ref{EB_ImageTransformation} oder von ihr
  abgeleiteter Klassen, die die Transformation beschreibt.
  @param ip Instanz der Klasse \Ref{EB_TransformationInterpolator} oder von ihr
  abgeleiteter Klassen, die die Interpolation beschreibt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & transform (const EB_ImageTransformation &trans,EB_TransformationInterpolator &ip);
  /**Koordinatentransformation. Das Band wird entsprechend der Vorschrift im 
  Transformationsobjekt transformiert. Jedes Band wird separat behandelt.
  Es wird eine Look-Up-Tabelle im Transformationobjekt erzeugt,
  was wiederholte Anwendungen dieses Objektes beschleunigt.  
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  Die aktuelle Instanz wird ver�ndert.
  @param trans Instanz der Klasse \Ref{EB_ImageTransformation} oder von ihr
  abgeleiteter Klassen, die die Transformation beschreibt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & transformWithLUT (const EB_ImageTransformation &trans);
	/**Farb�hnlichkeit. Betrachtet man die Gesamtheit aller Intensit�tswerte der B�nder
	an einer bestimmten Stelle im Bild, kann man das als die Farbe eines Pixels betrachten.
	Diese Methode berechnet die �hnlichkeit der Farbe jedes Pixels zu der
	in der Instanz \Ref{EB_ImageSegmentation} vorliegenden Farbdefinition mittels
	des dort definierten Distanzma�es.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param trans Instanz der Klasse \Ref{EB_ImageSegmentation} oder von ihr
  abgeleiteter Klassen.
  @return Instanz der Klasse \Ref{EB_Image}, die nur ein Band enth�lt, in dem die �hnlichkeit des entsprechenden Pixels der aktuellen Instanz zu der in trans hinterlegten Farbdefinition gespeichert ist.*/	
	EB_Image computeColorDistances(const EB_ImageSegmentation &segment);
	/**Farbsegmentation. Betrachtet man die Gesamtheit aller Intensit�tswerte der B�nder
	an einer bestimmten Stelle im Bild, kann man das als die Farbe eines Pixels betrachten.
	Diese Methode segmentiert das Bild in der aktuellen Instanz abh�ngig von der �hnlichkeit der Farbe jedes Pixels zu der
	in der Instanz \Ref{EB_ImageSegmentation} vorliegenden Farbdefinition. Mittels
	des dort definierten Distanzma�es wird ein Abstand der beiden Farbwerte berechnet. Dieser Abstand wird in das Ergebnisbild eingetragen. 
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param trans Instanz der Klasse \Ref{EB_ImageSegmentation} oder von ihr
  abgeleiteter Klassen.
  @param maxdistance Ist der berechnete Abstand gr��er als dieser Wert, ist das Ergebnisbild an dieser Stelle schwarz, ansonsten wei�.
  @return Instanz der Klasse \Ref{EB_Image}, die nur ein Band enth�lt. Dieses Band stellt in seinen Intensit�ten den Abstand der Farbe des jeweiligen Pixels von der in trans definierten dar. Dabei bedeutet eine hohe Intensit�t einen hohen Abstand.*/	
	EB_Image segment(const EB_ImageSegmentation &segment,float maxdistance);
	/**Farbsegmentation. Betrachtet man die Gesamtheit aller Intensit�tswerte der B�nder
	an einer bestimmten Stelle im Bild, kann man das als die Farbe eines Pixels betrachten.
	Diese Methode segmentiert das Bild in der aktuellen Instanz abh�ngig von der �hnlichkeit der Farbe jedes Pixels zu der
	in der Instanz \Ref{EB_ImageSegmentation} vorliegenden Farbdefinition. Mittels
	des dort definierten Distanzma�es wird ein Abstand der beiden Farbwerte berechnet. Abh�ngig von diesem Abstand wird entweder die Originalfarbe oder Schwarz in das Ergebnisbild eingetragen. 
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param trans Instanz der Klasse \Ref{EB_ImageSegmentation} oder von ihr
  abgeleiteter Klassen.
  @param maxdistance Ist der berechnete Abstand kleiner als dieser Wert, wird in das Ergebnisbild an dieser Stelle die Originalfarbe eingesetzt, ansonsten schwarz.
  @return Instanz der Klasse \Ref{EB_Image}, die nur ein Band enth�lt. Dieses Band stellt in seinen Intensit�ten den Abstand der Farbe des jeweiligen Pixels von der in trans definierten dar. Dabei bedeutet eine hohe Intensit�t einen hohen Abstand.*/	
EB_Image mask(const EB_ImageSegmentation &segment,float maxdistance);
	/**Farbsegmentation. Betrachtet man die Gesamtheit aller Intensit�tswerte der B�nder
	an einer bestimmten Stelle im Bild, kann man das als die Farbe eines Pixels betrachten.
	Diese Methode segmentiert das Bild in der aktuellen Instanz abh�ngig von der �hnlichkeit der Farbe jedes Pixels zu der
	in der Instanz \Ref{EB_ImageSegmentation} vorliegenden Farbdefinition. Mittels
	des dort definierten Distanzma�es wird ein Abstand der beiden Farbwerte berechnet. Abh�ngig von diesem Abstand wird entweder die Originalfarbe oder Schwarz in das Ergebnisbild eingetragen. 
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param trans Instanz der Klasse \Ref{EB_ImageSegmentation} oder von ihr
  abgeleiteter Klassen.
  @param mindistance Ist der berechnete Abstand gr��er als dieser Wert, wird in das Ergebnisbild an dieser Stelle die Originalfarbe eingesetzt, ansonsten schwarz.
  @return Instanz der Klasse \Ref{EB_Image}, die nur ein Band enth�lt. Dieses Band stellt in seinen Intensit�ten den Abstand der Farbe des jeweiligen Pixels von der in trans definierten dar. Dabei bedeutet eine hohe Intensit�t einen hohen Abstand.*/	
EB_Image invmask(const EB_ImageSegmentation &segment,float mindistance);
  /**Intensit�tsbestimmung. Diese Methode erlaubt es, die Intensit�t 
  in einem bestimmten Band an einer
  bestimmten Stelle zu ermitteln. Bei Koordinaten, f�r die keine Werte 
  vorliegen wird 0.0 zur�ckgegeben.
  @param x x-Koordinate des Pixels, dessen Intensit�tswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensit�tswert ermittelt werden soll.
  @param index Nummer des Bandes, aus dem der Intensit�tswert ermittelt
  werden soll. Liegt dieser
  Wert �ber der Anzahl an B�ndern im Bild, wird 0.0 zur�ckgegeben.
  @return Intensit�tswert.*/
    float giveFloatValue (unsigned int x, unsigned int y, unsigned int index);
  /**Intensit�tsvektor ermitteln. Diese Methode ermittelt auf einmal alle Intensit�tswerte 
  aus allen B�ndern an der spezifizierten Stelle. Bei Koordinaten, f�r die keine Werte 
  vorliegen wird 0.0 zur�ckgegeben.
  @param x x-Koordinate des Pixels, dessen Intensit�tswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensit�tswert ermittelt werden soll.
  @return Referenz auf eine Instanz der Klasse \Ref{EB_PixelDescriptor}.*/
    EB_PixelDescriptor & givePixel (unsigned int x, unsigned int y);
  /**Intensit�tsvektor ermitteln. Diese Methode ermittelt auf einmal alle Intensit�tswerte 
  aus allen B�ndern an der spezifizierten Stelle. Bei Koordinaten, f�r die keine Werte 
  vorliegen wird 0.0 zur�ckgegeben.
  @param index Laufende Nummer des Pixels. Die Pixel werden dabei 
  von der obersten Zeile beginnend zeilenweise durchnummeriert.
  @return Referenz auf eine Instanz der Klasse \Ref{EB_PixelDescriptor}.*/
    EB_PixelDescriptor & givePixel (unsigned int index);
  /**Intensit�tsbestimmung. Diese Methode erlaubt es, die Intensit�t 
  in einem bestimmten Band an einer
  bestimmten Stelle zu ermitteln. Der Intensit�tsbereich wird dabei auf den
  Wertebereich des Typs unsigned char abgebildet. Bei Koordinaten, f�r die keine Werte 
  vorliegen wird 0 zur�ckgegeben.
  @param x x-Koordinate des Pixels, dessen Intensit�tswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensit�tswert ermittelt werden soll.
  @param index Nummer des Bandes, aus dem der Intensit�tswert ermittelt
  werden soll. Liegt dieser
  Wert �ber der Anzahl an B�ndern im Bild, wird 0.0 zur�ckgegeben.
  @return Intensit�tswert.*/
    unsigned char giveCharValue (unsigned int x, unsigned int y, unsigned int index);
  /**Intensit�ts�nderung. Diese Methode erlaubt es, die Intensit�t 
  in einem bestimmten Band an einer
  bestimmten Stelle zu �ndern. Liegt der neue Wert au�erhalb des vereinbarten
  Intensit�tsintervalls, wird an der entsprechenden Stelle der n�chstliegende
  Grenzwert eingesetzt. Bei Koordinaten, f�r die keine Werte 
  vorliegen wird die �nderung ignoriert.
  @param x x-Koordinate des Pixels, dessen Intensit�tswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensit�tswert ermittelt werden soll.
  @param color Intensit�tswert.
  @param index Nummer des Bandes, aus dem der Intensit�tswert ermittelt
  werden soll. Liegt dieser
  Wert �ber der Anzahl an B�ndern im Bild, wird 0.0 zur�ckgegeben.*/
    void setValue (unsigned int x, unsigned int y, float color, unsigned int index);
  /**Intensit�ts�nderung. Diese Methode erlaubt es, die Intensit�t 
  in allen B�ndern an einer
  bestimmten Stelle zu �ndern. Liegt der neue Wert au�erhalb des vereinbarten
  Intensit�tsintervalls, wird an der entsprechenden Stelle der n�chstliegende
  Grenzwert eingesetzt. Bei Koordinaten, f�r die keine Werte 
  vorliegen wird die �nderung ignoriert.
  @param x x-Koordinate des Pixels, dessen Intensit�tswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensit�tswert ermittelt werden soll.
  @param color Instanz der Klasse \Ref{EB_PixelDescriptor}, der einen Vektor
  mit Intensit�ten enth�lt. Enth�lt sie weniger Komponenten als das Bild B�nder, werden
  nur die B�nder ge�ndert, f�r die Intensit�ten vorhanden sind; sind mehr enthalten,
  werden die �berz�hligen ignoriert.*/
    void setPixel (unsigned int x, unsigned int y, EB_PixelDescriptor color);
  /**Intensit�ts�nderung. Diese Methode erlaubt es, die Intensit�t 
  in einem bestimmten Band an einer
  bestimmten Stelle zu �ndern. Liegt der neue Wert au�erhalb des vereinbarten
  Intensit�tsintervalls, wird an der entsprechenden Stelle der n�chstliegende
  Grenzwert eingesetzt. Bei Koordinaten, f�r die keine Werte 
  vorliegen wird die �nderung ignoriert.
  @param pindex Laufende Nummer des Pixels. Die Pixel werden dabei 
  von der obersten Zeile beginnend zeilenweise durchnummeriert.
  @param color Intensit�tswert.
  @param index Nummer des Bandes, aus dem der Intensit�tswert ermittelt
  werden soll. Liegt dieser
  Wert �ber der Anzahl an B�ndern im Bild, wird 0.0 zur�ckgegeben.*/
    void setValue (unsigned int pindex, float color, unsigned int index);
  /**Intensit�ts�nderung. Diese Methode erlaubt es, die Intensit�t 
  in allen B�ndern an einer
  bestimmten Stelle zu �ndern. Liegt der neue Wert au�erhalb des vereinbarten
  Intensit�tsintervalls, wird an der entsprechenden Stelle der n�chstliegende
  Grenzwert eingesetzt. Bei Koordinaten, f�r die keine Werte 
  vorliegen wird die �nderung ignoriert.
  @param pindex Laufende Nummer des Pixels. Die Pixel werden dabei 
  von der obersten Zeile beginnend zeilenweise durchnummeriert.
  @param color Instanz der Klasse \Ref{EB_PixelDescriptor}, der einen Vektor
  mit Intensit�ten enth�lt. Enth�lt sie weniger Komponenten als das Bild B�nder, werden
  nur die B�nder ge�ndert, f�r die Intensit�ten vorhanden sind; sind mehr enthalten,
  werden die �berz�hligen ignoriert.*/
    void setPixel (unsigned int pindex, EB_PixelDescriptor color);
  /**Intensit�ts�nderung. Diese Methode erlaubt es, die Intensit�t 
  in einem bestimmten Band an einer
  bestimmten Stelle zu �ndern. Dabei entspricht ein Wert von 0 dem unteren, ein Wert von 
  255 dem oberen Ende des f�r dieses Band festgelegten Intensit�tsintervalls.
  Bei Koordinaten, f�r die keine Werte 
  vorliegen wird die �nderung ignoriert.
  @param x x-Koordinate des Pixels, dessen Intensit�tswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensit�tswert ermittelt werden soll.
  @param color Intensit�tswert.
  @param index Nummer des Bandes, aus dem der Intensit�tswert ermittelt
  werden soll. Liegt dieser
  Wert �ber der Anzahl an B�ndern im Bild, wird 0.0 zur�ckgegeben.*/
    void setValue (unsigned int x, unsigned int y, unsigned char color, unsigned int index);
  /**Intensit�ts�nderung. Diese Methode erlaubt es, die Intensit�t 
  in einem bestimmten Band an einer
  bestimmten Stelle zu �ndern. Dabei entspricht ein Wert von 0 dem unteren, ein Wert von 
  255 dem oberen Ende des f�r dieses Band festgelegten Intensit�tsintervalls.
  Bei Koordinaten, f�r die keine Werte 
  vorliegen wird die �nderung ignoriert.
  @param pindex Laufende Nummer des Pixels. Die Pixel werden dabei 
  von der obersten Zeile beginnend zeilenweise durchnummeriert.
  @param color Intensit�tswert.
  @param index Nummer des Bandes, aus dem der Intensit�tswert ermittelt
  werden soll. Liegt dieser
  Wert �ber der Anzahl an B�ndern im Bild, wird 0.0 zur�ckgegeben.*/
    void setValue (unsigned int pindex, unsigned char color, unsigned int index);
  /**Rotation. Diese Methode rotiert das Bild um den angegebenen Betrag. Die Drehung erfolgt um den 
  Mittelpunkt des Bildes. Das Bild in der aktuellen Instanz wird ver�ndert.
	Es ist m�glich, f�r jedes Band einen Intensit�tswert anzugeben,
	mit dem Bildbereiche des Resultats ausgef�llt werden, die keine Entsprechung im Original
	haben.
  @param angle Dieser Wert gibt den Winkel an, um den das Bild gedreht werden soll.
  Dabei bedeutet ein positiver Wert mathematisch positiven Drehsinn (entgegen der Uhrzeigerrichtung).
  Die Einheit der Winkelangabe ist Grad.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param p Bereiche im Bild, die nach der Rotation undefinierten Inhalt h�tten, 
  werden mit dieser Farbe aufgef�llt. Instanz der Klasse \Ref{EB_PixelDescriptor}, 
  die die Intensit"aten
	f�r die entsprechenden B�nder enth�lt. Enth�lt sie weniger Werte als B�nder
	vorhanden sind, wird f�r die �berz�hligen der letzte Wert in p benutzt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & rotate (float angle,const EB_PixelDescriptor &p=EB_PixelDescriptor(1,-10000.0))throw(EBIImageCorruptedEXP);
  /**Rotation. Diese Methode rotiert das Bild um den angegebenen Betrag. Die Drehung erfolgt um den angegebenen Punkt des Bildes, der auch au�erhalb liegen darf. Das Bild in der aktuellen Instanz wird ver�ndert.
  @param angle Dieser Wert gibt den Winkel an, um den das Bild gedreht werden soll.
  Dabei bedeutet ein positiver Wert mathematisch positiven Drehsinn (entgegen der Uhrzeigerrichtung).
  Die Einheit der Winkelangabe ist Grad.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param rotcenterx x-Koordinate des Punktes, um den das Bild rotiert wird.
  @param rotcentery y-Koordinate des Punktes,um den das Bild rotiert wird.
  @param p Bereiche im Bild, die nach der Rotation undefinierten Inhalt h�tten, 
  werden mit dieser Farbe aufgef�llt. Instanz der Klasse \Ref{EB_PixelDescriptor}, 
  die die Intensit"aten
	f�r die entsprechenden B�nder enth�lt. Enth�lt sie weniger Werte als B�nder
	vorhanden sind, wird f�r die �berz�hligen der letzte Wert in p benutzt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & rotate (float angle,int rotcenterx,int rotcentery,const EB_PixelDescriptor &p=EB_PixelDescriptor(1,-10000.0))throw(EBIImageCorruptedEXP);
  /**Rotation. Diese Methode rotiert das Bild um den angegebenen Betrag. Die Drehung erfolgt um den 
  Mittelpunkt des Bildes. Das Bild in der aktuellen Instanz wird ver�ndert
  und in seinen Dimensionen so ge�ndert, da� der gesamte Bildinhalt des
  Originals hineinpa�t.
	Es ist m�glich, f�r jedes Band einen Intensit�tswert anzugeben,
	mit dem Bildbereiche des Resultats ausgef�llt werden, die keine Entsprechung im Original
	haben.
  @param angle Dieser Wert gibt den Winkel an, um den das Bild gedreht werden soll.
  Dabei bedeutet ein positiver Wert mathematisch positiven Drehsinn (entgegen der Uhrzeigerrichtung).
  Die Einheit der Winkelangabe ist Grad.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param p Bereiche im Bild, die nach der Rotation undefinierten Inhalt h�tten, 
  werden mit dieser Farbe aufgef�llt. Instanz der Klasse \Ref{EB_PixelDescriptor}, 
  die die Intensit"aten
	f�r die entsprechenden B�nder enth�lt. Enth�lt sie weniger Werte als B�nder
	vorhanden sind, wird f�r die �berz�hligen der letzte Wert in p benutzt.
  @return Referenz auf die aktuelle Instanz.*/
  EB_Image &rotateToFit(float angle,const EB_PixelDescriptor &p=EB_PixelDescriptor(1,-10000.0))throw(EBIImageCorruptedEXP);
  /**Skalierung. Diese Methode gestattet es, ein Bild zu skalieren. Es ist m�glich,
  die neue Breite und H�he unabh�ngig voneinander anzugeben.
  Das Bild in der aktuellen Instanz wird ver�ndert.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param newx Neue Breite des Bildes.
  @param newy Neue H�he des Bildes.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & scale (unsigned int newx, unsigned int newy)throw(EBIImageCorruptedEXP);
  /**Skalierung. Diese Methode gestattet es, ein Bild zu skalieren. Die neuen Werte
  f�r Breite und H�he werden aus der �bergebenen Pixelmaximalanzahl berechnet.
  Das Bild in der aktuellen Instanz wird ver�ndert.
  @param sqrpixels Maximalanzahl an Pixeln im Bild nach der Skalierung.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & scale (unsigned int sqrpixels)throw(EBIImageCorruptedEXP);
  /**Skalierung. Diese Methode gestattet es, ein Bild zu skalieren. Die neue H�he wird 
  unter Beachtung des H�hen-Breiten-Verh�ltnisses aus der angegebenen neuen Breite
  berechnet.
  Das Bild in der aktuellen Instanz wird ver�ndert.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param newwidth Neue Breite des Bildes.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & scaleToWidth (unsigned int newwidth)throw(EBIImageCorruptedEXP);
  /**Skalierung. Diese Methode gestattet es, ein Bild zu skalieren. Die neue Breite wird 
  unter Beachtung des H�hen-Breiten-Verh�ltnisses aus der angegebenen neuen H�he
  berechnet.
  Das Bild in der aktuellen Instanz wird ver�ndert.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param newheight Neue H�he des Bildes.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & scaleToHeight (unsigned int newheight)throw(EBIImageCorruptedEXP);
    /**�berblenden. Mit dieser Methode k�nnen zwei Bilder mit festen Faktoren �berblendet werden.
    Dabei werden die Intensit�ten jedes Bildes f�r jedes Pixel bandweise addiert. Faktoren
    bestimmen, mit welchem Anteil die aktuelle und die �bergebene Instanz zu dieser Addition 
    beitragen. Ist die Summe der Faktoren gr��er als 0, werden beide entsprechend zur�ckskaliert.
    Stimmen die B�nderanzahlen beider Bilder nicht �berein, wird eine Exception geworfen.
 	Das Bild in der aktuellen Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
 	@param other Bild, das demjenigen in der aktuellen Instanz �berlagert werden soll.
 	@param otherfac Faktor f�r other.
 	@param ownfac Faktor f�r die aktuelle Instanz.
 	@param left Offset von other vom linken Rand.
 	@param top Offset von other vom oberen Rand.
	@return Referenz auf die aktuelle Instanz.*/
    EB_Image & overlay (EB_Image & other, float otherfac = 0.5, float ownfac = 0.5, int left = 0, int top = 0)throw(EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
    /**�berblenden. Mit dieser Methode k�nnen zwei Bilder mit festen Faktoren �berblendet werden.
    Dabei werden die Intensit�ten jedes Bildes f�r jedes Pixel bandweise addiert. Wie stark der
    Einflu� des Bildes in der aktuellen Instanz auf das Ergebnis ist, wird durch die
    Intensit�t an der entsprechenden Stelle im festgelegten Band im sogenannten 
    alphaimage bestimmt. Je h�her die Intensit�t in diesem Band ist, desto st�rker bestimmt
    der Inhalt von other das Resultat.
    Stimmen die B�nderanzahlen der aktuellen Instanz und von other nicht �berein, 
    wird eine Exception geworfen. Das Bild in der aktuellen Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
 	@param other Bild, das demjenigen in der aktuellen Instanz �berlagert werden soll.
 	@param alphaimage Bild, dessen Intensit�tswerte bestimmen, zu welchem Anteil die aktuelle 
	@param bandnumber Nummer des Bandes aus alphaimage, das als Maske benutzt werden soll.
	@param left Offset von other vom linken Rand.
 	@param top Offset von other vom oberen Rand.
 	@param left Offset von alphaimage vom linken Rand.
 	@param top Offset von alphaimage vom oberen Rand.
 	@param factor Mit diesem Faktor ist eine Intensit�tsbeeinflussung des Ergebnisses m�glich.
 	Mit diesem Wert wird das Ergebnis der beschriebenen Addition multipliziert. Bei einem Wert
 	von 0.5 ergibt sich also ein Ergebnis, welches halb so hell als ohne diese Multiplikation 
 	w�re.
	@return Referenz auf die aktuelle Instanz.*/
    EB_Image & overlay (EB_Image & other, EB_Image & alphaimage, unsigned int bandnumber, int left = 0, int top = 0, int aleft = 0, int atop = 0, float factor = 1.0f)throw(EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
    /**�berblenden. Mit dieser Methode k�nnen zwei Bilder mit festen Faktoren �berblendet werden.
    Dabei werden die Intensit�ten jedes Bildes f�r jedes Pixel bandweise addiert. Wie stark der
    Einflu� des Bildes in der aktuellen Instanz auf das Ergebnis ist, wird durch die
    Intensit�t an der entsprechenden Stelle im entsprechenden Band im sogenannten 
    alphaimage bestimmt. Je h�her die Intensit�t in diesem Band ist, desto st�rker bestimmt
    der Inhalt von other das Resultat.
    Stimmen die B�nderanzahlen der drei Bilder nicht �berein, wird eine Exception geworfen.
 	Das Bild in der aktuellen Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
 	@param other Bild, das demjenigen in der aktuellen Instanz �berlagert werden soll.
 	@param alphaimage Bild, dessen Intensit�tswerte bestimmen, zu welchem Anteil die aktuelle 
 	Instanz und otherfac in des Ergebnis eingehen sollen.
 	@param left Offset von other vom linken Rand.
 	@param top Offset von other vom oberen Rand.
 	@param left Offset von alphaimage vom linken Rand.
 	@param top Offset von alphaimage vom oberen Rand.
	@return Referenz auf die aktuelle Instanz.*/
    EB_Image & overlay (EB_Image & other, EB_Image & alphaimage, int left = 0, int top = 0, int aleft = 0, int atop = 0)throw(EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
	/**Farbreduktion.Werden die Zusammenfassungen der Intensit�tswerte aller B�nder an einer 
	bestimmten Stelle als Farbe dieses Pixels bezeichnet, ist diese Funktion in der Lage, die
	Anzahl unterschiedlicher Farben im Bild zu reduzieren. Diese Reduktion geschieht in der 
	vorliegenden Implementation durch eine Clusterung der Farben bei vorher festgelegter 
	Anzahl der Cluster. Ist dies geschehen, wird jedem Pixel als Farbe das Zentrum des Clusters
	zugeordnet, zu dem seine Farbe geh�rt.	
	Das Bild in der aktuellen Instanz wird ver�ndert.
	@param colorcount Anzahl der Farben, die �brigbleiben sollen.
	@param quality Ein zwischen 0.3 und 1.0 beschr�nkter Faktor, der bestimmt, wie viele
	Pixel zur Bestimmung der Clusterzentren einbezogen werden sollen. Je h�her dieser Wert, 
	desto besser ist das Ergebnis, desto mehr Rechenzeit wird aber auch verbraucht.
	@return Referenz auf die aktuelle Instanz.*/
    EB_Image & quantColors (unsigned int colorcount, float quality = 0.3)throw(EBIOutOfMemoryEXP,EBINoValuesInBandEXP);
	/**Farbkorrektur. Diese Methode erlaubt eine Farbkorrektur nach einem patentrechtlich gesch�tzten Verfahren.*/
    EB_Image & correctColors (unsigned int neuroncount = 10, float colorizer = 1.0)throw (EBIWrongNumberOfBandsEXP,EBIOutOfMemoryEXP);
  /**Spiegeln. Das Bild wird an x- und y-Achse gespiegelt. Das Bild in der aktuellen Instanz wird 
  ver�ndert.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & doubleMirror (void)throw(EBIImageCorruptedEXP);
  /**Spiegeln. Das Bild wird an x-Achse gespiegelt. Das Bild in der aktuellen Instanz wird 
  ver�ndert.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & verticalMirror (void)throw(EBIImageCorruptedEXP);
  /**Spiegeln. Das Bild wird an y-Achse gespiegelt. Das Bild in der aktuellen Instanz wird 
  ver�ndert.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & horizontalMirror (void)throw(EBIImageCorruptedEXP);
  /**Gl�ttung. Das Bild wird mit einem quadratischen Boxfilter mit 
  beliebiger Kantenl�nge gegl�ttet. Jedes Band wird separat behandelt.
  Jeder Pixel des Bandes wird mit dem Filter beaufschlagt. 
  Die aktuelle Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt nicht die eventuell definierte Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param smoothwidth Kantenl�nge des quadratischen Filters.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & smoothBox (unsigned int smoothwidth);
  /**Gl�ttung. Das Bild wird mit einem quadratischen Binomialfilter mit 
  beliebiger Kantenl�nge gegl�ttet. Jedes Band wird separat behandelt.
  Jeder Pixel des Bandes wird mit dem Filter beaufschlagt. 
  Die aktuelle Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt nicht die eventuell definierte Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param smoothwidth Kantenl�nge des quadratischen Filters.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & smoothBinom (unsigned int smoothwidth);
  /**Filterung. Das Bild wird mit einem von einer Instanz der Klasse \Ref{EB_Filter} dargestellten
  Filter gefiltert. Jeder Pixel jedes Bandes des Bildes wird mit dem Filter beaufschlagt. Es
  werden zur Zeit nur zweidimensionale Filter zugelassen. Das bedeutet, da� jedes Band mit
  der angegebenen Filtermaske beaufschlagt wird und nur Werte des jeweiligen Bandes zur Berechnung
  des neuen Intensit�tswertes benutzt werden k�nnen.
  Das Bild in der aktuellen Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param filter Instanz der Klasse \Ref{EB_Filter}.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & convolute (const EB_Filter & filter)throw(EBIImageCorruptedEXP);
  /**Filterung. Das Bild wird mit einem von einer Instanz der Klasse \Ref{EB_Filter} dargestellten
  Filter gefiltert. Jeder Pixel jedes Bandes des Bildes wird entsprechend der Intensit�t
  in alphaimage an der entsprechenden Stelle mit dem Filter beaufschlagt. Es
  werden zur Zeit nur zweidimensionale Filter zugelassen. Das bedeutet, da� jedes Band mit
  der angegebenen Filtermaske beaufschlagt wird und nur Werte des jeweiligen Bandes zur Berechnung
  des neuen Intensit�tswertes benutzt werden k�nnen.
  Das Bild in der aktuellen Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param filter Instanz der Klasse \Ref{EB_Filter}.
  @param alphaimage Instanz der Klasse \Ref{EB_Image}, deren Intensit�tswerte angeben, wie sehr
  das Ergebnis der Operation f�r jeden einzelnen Pixel vom gefilterten und vom Originalwert
  abh�ngt. Dabei wird jeweils das entsprechende Band benutzt.
  Enth�lt diese Instanz keine B�nder, wird eine Exception geworfen. Enth�lt sie nur
  ein Band, wird dieses f�r alle B�nder benutzt. Enth�lt sie mehr als ein Band, jedoch weniger als 
  die aktuelle Instanz, wird eine Exception geworfen. 
  @param left Offset der linken Ecke des alphaimage. 
  @param top Offset der oberen Ecke des alphaimage.
  @param factor Wenn dieser Wert gr��er als 1.0 oder kleiner als -1.0 ist, wird ihm der
  n�chstliegende der beiden genannten Werte zugewiesen. Ist der Wert danach gr��er als 0, dient
  er als ganz normale Schwelle: f�r jeden Pixel wird bestimmt, wie sich der Intensit�tswert 
  des jeweiligen Bands in alphaimage zur L�nge des Intensit�tsintervalls verh�lt. Ist dieses
  Verh�ltnis gr��er als factor, wird der gefilterte Wert in das Ergebnis eingesetzt, ansonsten 
  der Originalwert. Ist factor kleiner als 0, wird der Betrag als Schwelle benutzt. Im Falle 
  der Benutzung des gefilterten Wertes wird dieser nochmals linear mit dem Original verrechnet:
  Mit $o$ als Originalwert, $f$ als gefiltertem Wert, $i$ als Intensit�t in alphaimage an der 
  entsprechenden Stelle und $r$ als Resultat ergibt sich dann: \[r=f*i+(1-i)*o\]
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & convolute (const EB_Filter & filter, EB_Image & alphaimage, int left = 0, int top = 0, float factor = 0.0) throw (EBINoValuesInBandEXP, EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
	/**Morphologische Operation. Diese Methode stellt morphologische Operationen
	mit quadratischen Masken beliebiger Gr��e dar. �ber einen Parameter 
	kann stufenlos eine Betriebsart zwischen den beiden Extrema Erosion
	und Dilatation gew�hlt werden.
	Jedes Band wird gesondert behandelt. 
  Die aktuelle Instanz wird ver�ndert.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param kernelwidth Kantenl�nge der quadratischen Maske.
  @param gamma Parameter zur Beeinflussung des Verhaltens der Operation.
  Werte kleiner als 0.0 werden als 0.0 interpretiert, solche gr��er als 1.0 als 1.0. 
	Dabei entspricht 0.0 der Dilatation und 1.0 der Erosion.  
  @return Referenz auf die aktuelle Instanz.*/
EB_Image &morphologicOperation(unsigned int kernelwidth,float gamma);
	/**Laden. Diese Methode stellt zum einen die Schnittstelle f�r alle Operationen zum Laden
	von Bildinhalten aus Dateien verschiedener Formate zur Verf�gung, zum anderen implementiert 
	sie die Freigabe aller Resourcen, die sowieso neu allokiert werden m�ssen.
	Das Bild in der aktuellen Instanz wird ver�ndert.
	@return Referenz auf die aktuelle Instanz*/
    virtual EB_Image & load (const char *)
    {
	deallocateBands ();
	deallocatePalette ();
	return (*this);
    };
	/**Speichern. Diese Methode stellt die Schnittstelle f�r alle Operationen zum Speichern
	von Bildinhalten in Dateien verschiedener Formate zur Verf�gung.
	Das Bild in der aktuellen Instanz wird nicht ver�ndert.*/
    virtual void save (const char *)
    {
    };
//  EB_Image &changeOverallIntensity(float factor);
    EB_Image trueType (const char *font, const char *text, unsigned int ptsize, EB_PixelDescriptor oncolor, EB_PixelDescriptor offcolor);
	/**Region definieren. Mit dieser Methode wird der aktuellen eine Instanz der Klasse
	\Ref{EB_ImageRegion} zugeordnet. Diese Klasse stellt zweidimensionale Regionen dar.
	Zur Zeit k�nnen nur Regionen sinnvoll benutzt werden. Werden Operationen
	durch diese Regionen beeinflu�t, ist das in der Dokumentation angegeben.
	@param region Instanz der Klasse \Ref{EB_ImageRegion}, die der aktuellen Instanz
	zugeordnet werden soll.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & setRegion (EB_ImageRegion & region);
  /**Region l�schen. Mit dieser Funktion wird die Zuordnung einer Region zum aktuellen Bild
  aufgehoben. Nach Aufruf dieser Methode beeinflussen alle Methoden wieder das ganze Bild.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & unsetRegion (void);
	struct EB_Image_Info giveInfo(void)
	{
		struct EB_Image_Info info;
		
		info.width=giveWidth();
		info.height=giveHeight();
		info.bandcount=giveBandCount();
		return(info);
	}
  /**Medianfilterung. Das Band wird mit einem rechteckigen Medianfilter mit 
  beliebiger Kantenl�nge behandelt. Jedes Band wird separat behandelt.
  Die aktuelle Instanz wird ver�ndert.
  Diese Methode ber�cksichtigt die eventuell definierte Region.
  Diese Methode liefert Fortschrittsberichte �ber advancementcallback. 
  @param fux Kantenl�nge des Filters in x-Richtung.
  @param fuy Kantenl�nge des Filters in y-Richtung.
  @return Referenz auf die aktuelle Instanz.*/
	EB_Image &doMedianFilter(unsigned int fx,unsigned int fy);
	/**Lesen aus einem Stream. Mit dieser Methode kann der Inhalt einer Instanz der Klasse \Ref{EB_Image}  aus einem Stream gelesen werden. Zu Beginn werden der minimale, der maximale Intensit�tswert, die Breite, die H�he und die Anzahl der B�nder gelesen. Anschlie�end werden die B�nder im Bild den entsprechenden Werten angepa�t und schlie�lich die Inhalte der B�nder mittels \Ref{EB_Band::readFromStream} eingelesen. Tritt w�hrend des Lesens ein Fehler auf, wird eine Exception geworfen.
	@param i Stream, von dem gelesen werden soll.
	@return Referenz auf den �bergebenen Stream.*/
	std::istream &readFromStream(std::istream &i) throw (EBICouldNotLoadEXP);
	EB_Image doFire(unsigned int frames=100);
    static advancementcb setAdvancementCallback (advancementcb function);
		/**Abfrage des Streammodus. Siehe \Ref{EB_Band::writeToStream}.
		@return bool true, wenn die Bilder gepackt geschrieben werden sollen.*/
    static bool giveStreamMode (void)
		{
			return(pack);
		}
		/**Setzen des Streammodus. Diese Methode bestimmt, da� Bilder fortan gepackt in den Stream geschrieben werden sollen.*/
    static void setPackedStreamMode ()
		{
			pack=true;
		}
		/**Setzen des Streammodus. Diese Methode bestimmt, da� Bilder fortan ungepackt in den Stream geschrieben werden sollen.*/
    static void setHumanReadableStreamMode ()
		{
			pack=false;
		}
EB_Vector<float> calculateAverages(void);
EB_Vector<float> calculateSinusFilterCascade(void);
//@}
  protected:
  /**B�nder konstruieren. Diese Methode dient als Unterst�tzung an Stellen, wo 
  alle B�nder einer Instanz der Klasse \Ref{EB_Image} neu konstruiert werden m�ssen.
 @param color Intensit�tswert, mit dem alle Pixel aller B�nder initialisiert werden.
 Liegt dieser Wert au�erhalb des durch min und max bestimmten Intervalls,
 findet keine Initialisierung statt.
 @param min Minimaler Intensit�tswert in jedem Band.
 @param max Maximaler Intensit�tswert in jedem Band.*/ void allocateBands (float color, float min, float max);
  /**B�nder zerst�ren. Diese Methode erlaubt es, auf einen Schlag alle B�nder
  in der aktuellen Instanz zu zerst�ren.*/
    void deallocateBands (void);
  /**TODO*/
    void deallocatePalette (void);
    /*geschuetzte Member */
    /// Breite des Bildes in Pixeln.
    unsigned int width;
    /// H�he des Bildes in Pixeln.
    unsigned int height;
    /// Obere Intensit�tsgrenze.
    float maxfloat;
    /// Untere Intensit�tsgrenze.
    float minfloat;
    /// Intensit�tsvektor eines Pixels.
    EB_PixelDescriptor pixel;
    /// Vektor der B�nder.
    EB_Band **bands;
    /// Palette.
    float **palette;
	/// Anzahl Farben in der Palette.
    unsigned int palettecolors;
	/// Callback-Funktion f�r die Fortschrittsanzeige.
    static advancementcb advancementcallback;
	/// Region, auf die die Operationen, die das unterst�tzen, beschr�nkt bleiben.
    EB_ImageRegion iregion;
		static bool pack;
};
	/**Streamausgabeoperator f�r \Ref{EB_Image}. Mit diesem Operator kann man Instanzen der Klasse
	\Ref{EB_Image}
	auf C++ Streams ausgeben. Dabei werden zun�chst der minimale, der maximale Intensit�tswert, die Breite, die H�he und die Anzahl der B�nder ausgegeben. Anschlie�end erfolgt die Ausgabe der einzalnen B�nder mittels \Ref{EB_Band::writeToStream}.
	@param o Eine Referenz auf einen Stream, auf den die Ausgabe erfolgen soll.
	@param pic Eine Referenz auf die Instanz der Klasse \Ref{EB_Image}, die ausgegeben werden soll.
	@return Eine Referenz auf den �bergebenen Stream.*/
std::ostream &operator<<(std::ostream &o,const EB_Image &pic);
	/**Streameingabeoperator f�r \Ref{EB_Image}. Mit diesem Operator kann man Daten aus einem Stream in Instanzen der Klasse \Ref{EB_Image}
	laden. Dabei wir ddavon ausgegangen, da� die Daten in einem Format vorliegen, welches dem entspricht, das bei der Beschreibung des Streamausgabeoperators beschrieben wird. Tritt w�hrend des Lesens ein Fehler auf, wird eine Exception geworfen.
	@param i Eine Referenz auf einen Stream, von dem gelesen werden soll.
	@param s Eine Referenz auf die Instanz der Klasse \Ref{EB_Image}, die die
	gelesenen Daten aufnehmen soll.
	@return Eine Referenz auf den Stream.*/
std::istream &operator>>(std::istream &i,EB_Image &pic) throw (EBICouldNotLoadEXP);


#endif

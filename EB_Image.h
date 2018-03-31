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
rechteckig parallel zur x- und y-Achse. Es können beliebig viele Bänder
(Instanzen der Klasse \Ref{EB_Band}) in einem Bild vorhanden sein. Für ein Bild,
dessen Pixel zum Beispiel durch je einen Rot- Grün- und Blau-Farbwert beschrieben
werden, müßte das Bild drei Bänder enthalten. Für jede Instanz läßt sich 
eine Region definieren, auf die die Bildbearbeitungsoperationen beschränkt werden
sollen (bei manchen Operationen ist das nicht sinnvoll). Diese Regionen sind 
Instanzen der Klasse \Ref{EB_ImageRegion}. Zur Zeit funktionieren nur 
Regionen, deren begrenzende Liniensegmente einander nicht schneiden.
@author Jürgen "EL BOSSO" Key*/
class EB_Image
{
  public:
/**@name Konstruktoren und Destruktor*/
//@{
/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse 
	\Ref{EB_Image},die keine B ä nder enth ä lt.Damit hat er eine sehr geringe Zeit - und 
	Platzkomplexität.*/ 
	EB_Image (void);
 /**Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{EB_Image}.
 @param x Breite des Bildes.
 @param y Höhe des Bildes.
 @param usedbands Anzahl an Bändern im Bild.
 @param color Intensitätswert, mit dem alle Pixel aller Bänder initialisiert werden.
 Liegt dieser Wert außerhalb des durch min und max bestimmten Intervalls,
 findet keine Initialisierung statt. Voreinstellung ist -1.0.
 @param min Minimaler Intensitätswert in jedem Band. Voreinstellung ist 0.0.
 @param max Maximaler Intensitätswert in jedem Band. Voreinstellung ist 1.0.*/
    EB_Image (unsigned int x, unsigned int y, unsigned int usedbands, float color = -1.0, float min = 0.0, float max = 1.0);
 /**Copy-Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{EB_Image} als Kopie von der übergebenen Instanz.
 @param Image Instanz, von der die Kopie erzeugt wird.*/
    EB_Image (const EB_Image & Image);
 /**Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{EB_Image} als Kopie von der übergebenen Instanz der Klasse 
 \Ref{EB_Band}. Als Resultat entsteht ein Bild mit nur einem Band, welches 
 eine Kopie der übergebenen Instanz darstellt.
 @param Band Instanz, die zum Erstellen des Images benutzt wird.*/
    EB_Image (const EB_Band & Band);
 /**Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{EB_Image} als Kopie Instanz der Klasse 
 \Ref{EB_Band}, auf die der Zeiger verweist. Als Resultat 
 entsteht ein Bild mit nur einem Band, welches 
 eine Kopie der übergebenen Instanz darstellt.
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
  /**Zuweisungsoperator. Dieser Operator erzeugt eine Kopie der übergebenen
  in der aktuellen Instanz. Dies ist eine Deep Copy: Es existieren danach
  zwei völlig unabhängige Instanzen.
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
  /**Höhe. Diese Methode liefert die Höhe des Bildes in der aktuellen Instanz.
  @return Höhe der aktuellen Instanz.*/
    unsigned int giveHeight (void) const
    {
	return (height);
    };
  /**Maximale Intensität. Diese Methode liefert den maximalen Intensitätswert 
  der aktuellen Instanz.
  @return Maximaler Intensitätswert der aktuellen Instanz.*/
    float giveMaxFloat (void) const
    {
	return (maxfloat);
    };
  /**Minimale Intensität. Diese Methode liefert den minimalen Intensitätswert 
  der aktuellen Instanz.
  @return Minimaler Intensitätswert der aktuellen Instanz.*/
    float giveMinFloat (void) const
    {
	return (minfloat);
    };
  /**Bandanzahl. Diese Methode liefert die Anzahl an Bändern im Bild.
  @return Anzahl Bänder im Bild.*/
    unsigned int giveBandCount (void) const
    {
	return (pixel.giveComponentCount ());
    };
  /**Bänder hinzufügen. Mit dieser Methode ist es möglich, die Bänder eines 
  anderen Bildes hinter denen der aktuellen Instanz dieser hinzuzufügen.
  Dabei werden die Bänder hinter den bereits vorhandenen angehängt. Hatte die
  aktuelle Instanz also beispielsweise bereits 3 Bänder (Indizes 0-2), so 
  ist nach dieser Operation das zweite Band der Instanz source jetzt das mit 
  Index 4 in der aktuellen Instanz. Die Kopieen werden mittels deep copy
  angelegt, die beiden Instanzen sind nach der Operation vollständig unabhängig
  voneinander.
  @param source Instanz, von der die Bänder übernommen werden sollen.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & addBands (const EB_Image & source)throw(EBIImageCorruptedEXP);
  /**Band hinzufügen. Mit dieser Methode ist es möglich, ein Band 
  hinter denen der aktuellen Instanz dieser hinzuzufügen.
  Dabei wird das Band hinter den bereits vorhandenen angehängt. Hatte die
  aktuelle Instanz also beispielsweise bereits 3 Bänder (Indizes 0-2), so 
  ist nach dieser Operation der Index für das neue Band 3. 
  Das Band wird mittels deep copy 
  kopiert, die beiden Instanzen sind nach der Operation vollständig unabhängig
  voneinander.
  @param source Instanz, die zur aktuellen Instanz hinzugefügt werden soll.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & addBand (const EB_Band & source)throw(EBIImageCorruptedEXP);
  /**Band ersetzen. Mit dieser Methode ist es möglich, ein Band 
  in der aktuellen Instanz zu ersetzen.
  Dazu wird das zu ersetzende Band zerstört und an seiner Stelle mittels Copy-
  Konstruktor von der übergebenen Instanz ein neues erstellt. 
  @param source Instanz, die zur aktuellen Instanz hinzugefügt werden soll.
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
  /**Komponieren von Bildern. Diese Methode überträgt den Inhalt des von
  der aktuellen Instanz verkörperten Bildes in ein anderes. Dabei kann man noch einen 
  Offset angeben, der die Position des Bildes im Zielbild angibt. Dieser Offset
  bezieht sich auf die linke obere Ecke. Sind im aktuellen Bild keine Bänder vorhanden
  oder stimmen die Anzahlen der Bänder beider Bilder nicht überein, werden Exceptions
  geworfen. Das Bild in der aktuellen Instanz wird hierbei nicht verändert
  @param other Instanz der Klasse \Ref{EB_Image} und Ziel der Kopie.
  @param left Offset vom linken Rand für die Kopie.
  @param top Offset vom oberen Rand für die Kopie.
  @return Referenz auf other.*/
    EB_Image & copyWholeTo (EB_Image & other, int left, int top) throw (EBINoValuesInBandEXP, EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
  /**Komponieren von Bildern. Diese Methode überträgt den Inhalt des von
  der Zielinstanz verkörperten Bildes in die aktuelle Instanz. Dabei kann man noch einen 
  Offset angeben, der die Position des Bildes im Zielbild angibt. Dieser Offset
  bezieht sich auf die linke obere Ecke. Stimmen die Anzahlen der Bänder beider 
  Bilder nicht überein oder enthält ein Band eines der beiden Bilder keine
  Daten, werden Exceptions
  geworfen. Das Bild in der aktuellen Instanz wird verändert.
  @param other stanz der Klasse \Ref{EB_Image} und Quelle der Kopie.
  @param left Offset vom linken Rand für die Kopie.
  @param top Offset vom oberen Rand für die Kopie.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & copyWholeFrom (const EB_Image & other, int left, int top) throw (EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
  /**Dynamikbereich ändern. Mit dieser Methode werden nachträglich die Grenzen
  der Intensitätswerte in den einzelnen Bändern geändert. Dabei wird nicht abgeschnitten,
  sondern skaliert. Wird der Dynamikbereich von 0 bis 10 auf 0 bis 1 geändert,
  wird der Intensitätswert eines Pixels von beispielsweise vorher 5 auf 
  0.5 geändert. Das Bild in der aktuellen Instanz wird nicht verändert. 
  @param newmin Minimaler Intensitätswert in jedem Band.
  @param newmax Maximaler Intensitätswert in jedem Band.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & rescaleBands (float newmin, float newmax);
  /**Farbraumkonversion. Diese Methode erlaubt es, ein Bild in den L*a*b- 
  Farbraum zu konvertieren. Dabei wird angenommen, daß das Ausgangsbild im 
  RGB-Farbraum lag. Das Bild in der aktuellen Instanz wird 
  verändert. Hat die aktuelle Instanz nicht genau drei Bänder, wird eine 
  Exception geworfen.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
	EB_Image &convertRGBToLAB (void) throw (EBIWrongNumberOfBandsEXP);
  /**Farbraumkonversion. Diese Methode erlaubt es, ein Bild in den RGB- 
  Farbraum zu konvertieren. Dabei wird angenommen, daß das Ausgangsbild im 
  L*a*b-Farbraum lag. Das Bild in der aktuellen Instanz wird 
  verändert. Hat die aktuelle Instanz nicht genau drei Bänder, wird eine 
  Exception geworfen.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
	EB_Image &convertLABToRGB (void) throw (EBIWrongNumberOfBandsEXP);
	/**Grauwertwandlung. Diese Methode wandelt die Intensitäten aller
	Bänder des Bildes in einen Intensitätswert um. Bei einem normalen
	RGB Bild entspricht das der Umwandlung in das entsprechende
	Grauwertbild. Es ist möglich, für jedes Band Faktoren einzugeben, mit
	denen das jeweilige Band gewichtet werden soll. 
	@param p Instanz der Klasse \Ref{EB_PixelDescriptor}, die die Faktoren
	für die entsprechenden Bänder enthält. Enthält sie weniger Werte als Bänder
	vorhanden sind, wird für die überzähligen der letzte wert in p benutzt.
	@return Instanz der Klasse \Ref{EB_Image} mit genau einem Band.*/	
	EB_Image toGray(const EB_PixelDescriptor &p=EB_PixelDescriptor(1,1.0));
  /**Band extrahieren. Diese Methode erlaubt es, ein Band der aktuellen Instanz 
  zu extrahieren. Daraus wird dann eine neue Instanz der Klasse \Ref{EB_Image}
  konstruiert. Das entsprechende Band wird mittels deep copy kopiert, 
  so daß beide Instanzen vollkommen unabhängig voneinander sind. Das 
  Bild in der aktuellen Instanz wird nicht verändert. Ist in der aktuellen
  Instanz kein Band definiert oder ist für diesen Index kein Band
  definiert, wird eine Exception geworfen. 
  @param index Nummer des Bandes, das extrahiert werden soll. Liegt dieser
  Wert über der Anzahl an Bändern im Bild, wird das erste Band benutzt.
  @return Instanz der Klasse \Ref{EB_Image} mit nur einem Band.*/
    EB_Image &extractSingleBand (unsigned int index)throw(EBINoValuesInBandEXP,EBIIndexOutOfRangeEXP);
  /**Band extrahieren. Diese Methode erlaubt es, ein Band der aktuellen Instanz 
  zu extrahieren. Daraus wird dann eine Falschfarbendarstellung 
  der Intensitäten des Ursprungsbandes in einer neuen Instanz der Klasse 
  \Ref{EB_Image} im RGB-Farbraum. Die neue und die aktuelle Instanz sind
  anschließend vollkommen unabhängig voneinander. Das 
  Bild in der aktuellen Instanz wird nicht verändert. Ist in der aktuellen
  Instanz kein Band definiert oder ist für diesen Index kein Band
  definiert, wird eine Exception geworfen. 
  @param index Nummer des Bandes, das extrahiert werden soll. Liegt dieser
  Wert über der Anzahl an Bändern im Bild, wird das erste Band benutzt.
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
  @return Instanz der Klasse \Ref{EB_Image} mit drei Bändern.*/
    EB_Image extractSingleBandAsRGB (unsigned int index, unsigned int mode = TOGRAY)throw(EBINoValuesInBandEXP,EBIIndexOutOfRangeEXP);
  /**Bildausschnitt extrahieren. Diese Methode gestattet es. einen definierten, 
  rechteckigen Bereich des durch die aktuelle Instanz verkörperten Bildes 
  auszuschneiden und in einer neuen Instanz zu speichern. Die Parameter 
  beschereiben dabei die linke obere Ecke sowie die Breite und Höhe des 
  Ausschnittes. Ragt der Ausschnitt dabei über das Bild hinaus, wird den
  entsprechenden Pixeln der niedrigstmögliche Intensitätswert zugewiesen.
  Stimmen die Anzahlen der Bänder beider 
  Bilder nicht überein oder enthält ein Band eines der beiden Bilder keine
  Daten, werden Exceptions
  geworfen.Das Bild in der aktuellen Instanz wird nicht verändert.
  @param xstart x-Koordinate der linken oberen Ecke des Ausschnittes.
  @param ystart y-Koordinate der linken oberen Ecke des Ausschnittes.
  @param xdim Breite des Ausschnittes.
  @param ydim Höhe des Ausschnittes.
  @return Instanz der Klasse \Ref{EB_Image}.*/
    EB_Image cutoutAPiece (int xstart, int ystart, unsigned int xdim, unsigned int ydim)throw (EBINoValuesInBandEXP, EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
  /**Histogrammausgleich. Diese Methode führt einen Histogrammausgleich auf
  dem Bild aus. Dabei wird jedes Band separat behandelt.
  Das Bild in der aktuellen Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & aequalize (void);
  /**Gammakorrektur. Diese Methode führt eine Gammakorrektur auf
  dem Bild aus. Dies ist eine über der Intensität nichtlineare 
  Helligkeitsänderung. Dabei wird jedes Band separat behandelt.
  Das Bild in der aktuellen Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param factor Parameter für die Gammakorrektur. Ist dieser Wert kleiner 
  als eins, wird das Bild dunkler, ansonsten heller.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & gammaCorrect (float factor);
  /**Kontraständerung. Diese Methode führt eine Kontraständerung auf
  dem Bild aus. Dabei wird jedes Band separat behandelt.
  Das Bild in der aktuellen Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param factor Parameter für die Kontraständerung. Je höher dieser Wert ist, desto 
  mehr wird der Kontrast verstärkt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & changeContrast (float factor);
  /**Intensitätsänderung. Diese Methode führt in den einzelnen Bändern
  eine Intensitätsänderung 
  entsprechend der nichtlinearen Funktion in trans aus. Dabei dienen die
  Intensitäten als Argumente für diese Funktion und die Ergebnisse der Funktion
  als neue Intensitäten. Jedes Band wird separat behandelt.
  Der Intensitätsbereich wird in 256 Stufen diskretisiert um die Operation mittels einer Look-Up-Tabelle beschleunigen zu können.
  Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param trans Instanz einer von \Ref{EB_IntensityTransformation} abgeleiteten
  Klasse, die die eigentliche Funktion repräsentiert.
  @return Referenz auf die aktuelle Instanz.*/
EB_Image &transformWithLUT(const EB_IntensityTransformation &trans);
  /**Intensitätsänderung. Diese Methode führt in den einzelnen Bändern
  eine Intensitätsänderung 
  entsprechend der nichtlinearen Funktion in trans aus. Dabei dienen die
  Intensitäten als Argumente für diese Funktion und die Ergebnisse der Funktion
  als neue Intensitäten. Jedes Band wird separat behandelt.
  Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param trans Instanz einer von \Ref{EB_IntensityTransformation} abgeleiteten
  Klasse, die die eigentliche Funktion repräsentiert.
  @return Referenz auf die aktuelle Instanz.*/
EB_Image &transform(const EB_IntensityTransformation &trans);
  /**Koordinatentransformation. Das Bild wird entsprechend der Vorschrift im 
  Transformationsobjekt transformiert. Jedes Band wird separat behandelt.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  Die aktuelle Instanz wird verändert.
  @param trans Instanz der Klasse \Ref{EB_ImageTransformation} oder von ihr
  abgeleiteter Klassen, die die Transformation beschreibt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & transform (const EB_ImageTransformation &trans);
  /**Koordinatentransformation. Das Bild wird entsprechend der Vorschrift im 
  Transformationsobjekt transformiert.Dabei wird das Bild entsprechend
  der Vorschrift im Interpolationsobjekt interpoliert.
  Jedes Band wird separat behandelt.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  Die aktuelle Instanz wird verändert.
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
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  Die aktuelle Instanz wird verändert.
  @param trans Instanz der Klasse \Ref{EB_ImageTransformation} oder von ihr
  abgeleiteter Klassen, die die Transformation beschreibt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & transformWithLUT (const EB_ImageTransformation &trans);
	/**Farbähnlichkeit. Betrachtet man die Gesamtheit aller Intensitätswerte der Bänder
	an einer bestimmten Stelle im Bild, kann man das als die Farbe eines Pixels betrachten.
	Diese Methode berechnet die Ähnlichkeit der Farbe jedes Pixels zu der
	in der Instanz \Ref{EB_ImageSegmentation} vorliegenden Farbdefinition mittels
	des dort definierten Distanzmaßes.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param trans Instanz der Klasse \Ref{EB_ImageSegmentation} oder von ihr
  abgeleiteter Klassen.
  @return Instanz der Klasse \Ref{EB_Image}, die nur ein Band enthält, in dem die Ähnlichkeit des entsprechenden Pixels der aktuellen Instanz zu der in trans hinterlegten Farbdefinition gespeichert ist.*/	
	EB_Image computeColorDistances(const EB_ImageSegmentation &segment);
	/**Farbsegmentation. Betrachtet man die Gesamtheit aller Intensitätswerte der Bänder
	an einer bestimmten Stelle im Bild, kann man das als die Farbe eines Pixels betrachten.
	Diese Methode segmentiert das Bild in der aktuellen Instanz abhängig von der Ähnlichkeit der Farbe jedes Pixels zu der
	in der Instanz \Ref{EB_ImageSegmentation} vorliegenden Farbdefinition. Mittels
	des dort definierten Distanzmaßes wird ein Abstand der beiden Farbwerte berechnet. Dieser Abstand wird in das Ergebnisbild eingetragen. 
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param trans Instanz der Klasse \Ref{EB_ImageSegmentation} oder von ihr
  abgeleiteter Klassen.
  @param maxdistance Ist der berechnete Abstand größer als dieser Wert, ist das Ergebnisbild an dieser Stelle schwarz, ansonsten weiß.
  @return Instanz der Klasse \Ref{EB_Image}, die nur ein Band enthält. Dieses Band stellt in seinen Intensitäten den Abstand der Farbe des jeweiligen Pixels von der in trans definierten dar. Dabei bedeutet eine hohe Intensität einen hohen Abstand.*/	
	EB_Image segment(const EB_ImageSegmentation &segment,float maxdistance);
	/**Farbsegmentation. Betrachtet man die Gesamtheit aller Intensitätswerte der Bänder
	an einer bestimmten Stelle im Bild, kann man das als die Farbe eines Pixels betrachten.
	Diese Methode segmentiert das Bild in der aktuellen Instanz abhängig von der Ähnlichkeit der Farbe jedes Pixels zu der
	in der Instanz \Ref{EB_ImageSegmentation} vorliegenden Farbdefinition. Mittels
	des dort definierten Distanzmaßes wird ein Abstand der beiden Farbwerte berechnet. Abhängig von diesem Abstand wird entweder die Originalfarbe oder Schwarz in das Ergebnisbild eingetragen. 
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param trans Instanz der Klasse \Ref{EB_ImageSegmentation} oder von ihr
  abgeleiteter Klassen.
  @param maxdistance Ist der berechnete Abstand kleiner als dieser Wert, wird in das Ergebnisbild an dieser Stelle die Originalfarbe eingesetzt, ansonsten schwarz.
  @return Instanz der Klasse \Ref{EB_Image}, die nur ein Band enthält. Dieses Band stellt in seinen Intensitäten den Abstand der Farbe des jeweiligen Pixels von der in trans definierten dar. Dabei bedeutet eine hohe Intensität einen hohen Abstand.*/	
EB_Image mask(const EB_ImageSegmentation &segment,float maxdistance);
	/**Farbsegmentation. Betrachtet man die Gesamtheit aller Intensitätswerte der Bänder
	an einer bestimmten Stelle im Bild, kann man das als die Farbe eines Pixels betrachten.
	Diese Methode segmentiert das Bild in der aktuellen Instanz abhängig von der Ähnlichkeit der Farbe jedes Pixels zu der
	in der Instanz \Ref{EB_ImageSegmentation} vorliegenden Farbdefinition. Mittels
	des dort definierten Distanzmaßes wird ein Abstand der beiden Farbwerte berechnet. Abhängig von diesem Abstand wird entweder die Originalfarbe oder Schwarz in das Ergebnisbild eingetragen. 
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param trans Instanz der Klasse \Ref{EB_ImageSegmentation} oder von ihr
  abgeleiteter Klassen.
  @param mindistance Ist der berechnete Abstand größer als dieser Wert, wird in das Ergebnisbild an dieser Stelle die Originalfarbe eingesetzt, ansonsten schwarz.
  @return Instanz der Klasse \Ref{EB_Image}, die nur ein Band enthält. Dieses Band stellt in seinen Intensitäten den Abstand der Farbe des jeweiligen Pixels von der in trans definierten dar. Dabei bedeutet eine hohe Intensität einen hohen Abstand.*/	
EB_Image invmask(const EB_ImageSegmentation &segment,float mindistance);
  /**Intensitätsbestimmung. Diese Methode erlaubt es, die Intensität 
  in einem bestimmten Band an einer
  bestimmten Stelle zu ermitteln. Bei Koordinaten, für die keine Werte 
  vorliegen wird 0.0 zurückgegeben.
  @param x x-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param index Nummer des Bandes, aus dem der Intensitätswert ermittelt
  werden soll. Liegt dieser
  Wert über der Anzahl an Bändern im Bild, wird 0.0 zurückgegeben.
  @return Intensitätswert.*/
    float giveFloatValue (unsigned int x, unsigned int y, unsigned int index);
  /**Intensitätsvektor ermitteln. Diese Methode ermittelt auf einmal alle Intensitätswerte 
  aus allen Bändern an der spezifizierten Stelle. Bei Koordinaten, für die keine Werte 
  vorliegen wird 0.0 zurückgegeben.
  @param x x-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @return Referenz auf eine Instanz der Klasse \Ref{EB_PixelDescriptor}.*/
    EB_PixelDescriptor & givePixel (unsigned int x, unsigned int y);
  /**Intensitätsvektor ermitteln. Diese Methode ermittelt auf einmal alle Intensitätswerte 
  aus allen Bändern an der spezifizierten Stelle. Bei Koordinaten, für die keine Werte 
  vorliegen wird 0.0 zurückgegeben.
  @param index Laufende Nummer des Pixels. Die Pixel werden dabei 
  von der obersten Zeile beginnend zeilenweise durchnummeriert.
  @return Referenz auf eine Instanz der Klasse \Ref{EB_PixelDescriptor}.*/
    EB_PixelDescriptor & givePixel (unsigned int index);
  /**Intensitätsbestimmung. Diese Methode erlaubt es, die Intensität 
  in einem bestimmten Band an einer
  bestimmten Stelle zu ermitteln. Der Intensitätsbereich wird dabei auf den
  Wertebereich des Typs unsigned char abgebildet. Bei Koordinaten, für die keine Werte 
  vorliegen wird 0 zurückgegeben.
  @param x x-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param index Nummer des Bandes, aus dem der Intensitätswert ermittelt
  werden soll. Liegt dieser
  Wert über der Anzahl an Bändern im Bild, wird 0.0 zurückgegeben.
  @return Intensitätswert.*/
    unsigned char giveCharValue (unsigned int x, unsigned int y, unsigned int index);
  /**Intensitätsänderung. Diese Methode erlaubt es, die Intensität 
  in einem bestimmten Band an einer
  bestimmten Stelle zu ändern. Liegt der neue Wert außerhalb des vereinbarten
  Intensitätsintervalls, wird an der entsprechenden Stelle der nächstliegende
  Grenzwert eingesetzt. Bei Koordinaten, für die keine Werte 
  vorliegen wird die Änderung ignoriert.
  @param x x-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param color Intensitätswert.
  @param index Nummer des Bandes, aus dem der Intensitätswert ermittelt
  werden soll. Liegt dieser
  Wert über der Anzahl an Bändern im Bild, wird 0.0 zurückgegeben.*/
    void setValue (unsigned int x, unsigned int y, float color, unsigned int index);
  /**Intensitätsänderung. Diese Methode erlaubt es, die Intensität 
  in allen Bändern an einer
  bestimmten Stelle zu ändern. Liegt der neue Wert außerhalb des vereinbarten
  Intensitätsintervalls, wird an der entsprechenden Stelle der nächstliegende
  Grenzwert eingesetzt. Bei Koordinaten, für die keine Werte 
  vorliegen wird die Änderung ignoriert.
  @param x x-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param color Instanz der Klasse \Ref{EB_PixelDescriptor}, der einen Vektor
  mit Intensitäten enthält. Enthält sie weniger Komponenten als das Bild Bänder, werden
  nur die Bänder geändert, für die Intensitäten vorhanden sind; sind mehr enthalten,
  werden die überzähligen ignoriert.*/
    void setPixel (unsigned int x, unsigned int y, EB_PixelDescriptor color);
  /**Intensitätsänderung. Diese Methode erlaubt es, die Intensität 
  in einem bestimmten Band an einer
  bestimmten Stelle zu ändern. Liegt der neue Wert außerhalb des vereinbarten
  Intensitätsintervalls, wird an der entsprechenden Stelle der nächstliegende
  Grenzwert eingesetzt. Bei Koordinaten, für die keine Werte 
  vorliegen wird die Änderung ignoriert.
  @param pindex Laufende Nummer des Pixels. Die Pixel werden dabei 
  von der obersten Zeile beginnend zeilenweise durchnummeriert.
  @param color Intensitätswert.
  @param index Nummer des Bandes, aus dem der Intensitätswert ermittelt
  werden soll. Liegt dieser
  Wert über der Anzahl an Bändern im Bild, wird 0.0 zurückgegeben.*/
    void setValue (unsigned int pindex, float color, unsigned int index);
  /**Intensitätsänderung. Diese Methode erlaubt es, die Intensität 
  in allen Bändern an einer
  bestimmten Stelle zu ändern. Liegt der neue Wert außerhalb des vereinbarten
  Intensitätsintervalls, wird an der entsprechenden Stelle der nächstliegende
  Grenzwert eingesetzt. Bei Koordinaten, für die keine Werte 
  vorliegen wird die Änderung ignoriert.
  @param pindex Laufende Nummer des Pixels. Die Pixel werden dabei 
  von der obersten Zeile beginnend zeilenweise durchnummeriert.
  @param color Instanz der Klasse \Ref{EB_PixelDescriptor}, der einen Vektor
  mit Intensitäten enthält. Enthält sie weniger Komponenten als das Bild Bänder, werden
  nur die Bänder geändert, für die Intensitäten vorhanden sind; sind mehr enthalten,
  werden die überzähligen ignoriert.*/
    void setPixel (unsigned int pindex, EB_PixelDescriptor color);
  /**Intensitätsänderung. Diese Methode erlaubt es, die Intensität 
  in einem bestimmten Band an einer
  bestimmten Stelle zu ändern. Dabei entspricht ein Wert von 0 dem unteren, ein Wert von 
  255 dem oberen Ende des für dieses Band festgelegten Intensitätsintervalls.
  Bei Koordinaten, für die keine Werte 
  vorliegen wird die Änderung ignoriert.
  @param x x-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param color Intensitätswert.
  @param index Nummer des Bandes, aus dem der Intensitätswert ermittelt
  werden soll. Liegt dieser
  Wert über der Anzahl an Bändern im Bild, wird 0.0 zurückgegeben.*/
    void setValue (unsigned int x, unsigned int y, unsigned char color, unsigned int index);
  /**Intensitätsänderung. Diese Methode erlaubt es, die Intensität 
  in einem bestimmten Band an einer
  bestimmten Stelle zu ändern. Dabei entspricht ein Wert von 0 dem unteren, ein Wert von 
  255 dem oberen Ende des für dieses Band festgelegten Intensitätsintervalls.
  Bei Koordinaten, für die keine Werte 
  vorliegen wird die Änderung ignoriert.
  @param pindex Laufende Nummer des Pixels. Die Pixel werden dabei 
  von der obersten Zeile beginnend zeilenweise durchnummeriert.
  @param color Intensitätswert.
  @param index Nummer des Bandes, aus dem der Intensitätswert ermittelt
  werden soll. Liegt dieser
  Wert über der Anzahl an Bändern im Bild, wird 0.0 zurückgegeben.*/
    void setValue (unsigned int pindex, unsigned char color, unsigned int index);
  /**Rotation. Diese Methode rotiert das Bild um den angegebenen Betrag. Die Drehung erfolgt um den 
  Mittelpunkt des Bildes. Das Bild in der aktuellen Instanz wird verändert.
	Es ist möglich, für jedes Band einen Intensitätswert anzugeben,
	mit dem Bildbereiche des Resultats ausgefüllt werden, die keine Entsprechung im Original
	haben.
  @param angle Dieser Wert gibt den Winkel an, um den das Bild gedreht werden soll.
  Dabei bedeutet ein positiver Wert mathematisch positiven Drehsinn (entgegen der Uhrzeigerrichtung).
  Die Einheit der Winkelangabe ist Grad.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param p Bereiche im Bild, die nach der Rotation undefinierten Inhalt hätten, 
  werden mit dieser Farbe aufgefüllt. Instanz der Klasse \Ref{EB_PixelDescriptor}, 
  die die Intensit"aten
	für die entsprechenden Bänder enthält. Enthält sie weniger Werte als Bänder
	vorhanden sind, wird für die überzähligen der letzte Wert in p benutzt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & rotate (float angle,const EB_PixelDescriptor &p=EB_PixelDescriptor(1,-10000.0))throw(EBIImageCorruptedEXP);
  /**Rotation. Diese Methode rotiert das Bild um den angegebenen Betrag. Die Drehung erfolgt um den angegebenen Punkt des Bildes, der auch außerhalb liegen darf. Das Bild in der aktuellen Instanz wird verändert.
  @param angle Dieser Wert gibt den Winkel an, um den das Bild gedreht werden soll.
  Dabei bedeutet ein positiver Wert mathematisch positiven Drehsinn (entgegen der Uhrzeigerrichtung).
  Die Einheit der Winkelangabe ist Grad.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param rotcenterx x-Koordinate des Punktes, um den das Bild rotiert wird.
  @param rotcentery y-Koordinate des Punktes,um den das Bild rotiert wird.
  @param p Bereiche im Bild, die nach der Rotation undefinierten Inhalt hätten, 
  werden mit dieser Farbe aufgefüllt. Instanz der Klasse \Ref{EB_PixelDescriptor}, 
  die die Intensit"aten
	für die entsprechenden Bänder enthält. Enthält sie weniger Werte als Bänder
	vorhanden sind, wird für die überzähligen der letzte Wert in p benutzt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & rotate (float angle,int rotcenterx,int rotcentery,const EB_PixelDescriptor &p=EB_PixelDescriptor(1,-10000.0))throw(EBIImageCorruptedEXP);
  /**Rotation. Diese Methode rotiert das Bild um den angegebenen Betrag. Die Drehung erfolgt um den 
  Mittelpunkt des Bildes. Das Bild in der aktuellen Instanz wird verändert
  und in seinen Dimensionen so geändert, daß der gesamte Bildinhalt des
  Originals hineinpaßt.
	Es ist möglich, für jedes Band einen Intensitätswert anzugeben,
	mit dem Bildbereiche des Resultats ausgefüllt werden, die keine Entsprechung im Original
	haben.
  @param angle Dieser Wert gibt den Winkel an, um den das Bild gedreht werden soll.
  Dabei bedeutet ein positiver Wert mathematisch positiven Drehsinn (entgegen der Uhrzeigerrichtung).
  Die Einheit der Winkelangabe ist Grad.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param p Bereiche im Bild, die nach der Rotation undefinierten Inhalt hätten, 
  werden mit dieser Farbe aufgefüllt. Instanz der Klasse \Ref{EB_PixelDescriptor}, 
  die die Intensit"aten
	für die entsprechenden Bänder enthält. Enthält sie weniger Werte als Bänder
	vorhanden sind, wird für die überzähligen der letzte Wert in p benutzt.
  @return Referenz auf die aktuelle Instanz.*/
  EB_Image &rotateToFit(float angle,const EB_PixelDescriptor &p=EB_PixelDescriptor(1,-10000.0))throw(EBIImageCorruptedEXP);
  /**Skalierung. Diese Methode gestattet es, ein Bild zu skalieren. Es ist möglich,
  die neue Breite und Höhe unabhängig voneinander anzugeben.
  Das Bild in der aktuellen Instanz wird verändert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param newx Neue Breite des Bildes.
  @param newy Neue Höhe des Bildes.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & scale (unsigned int newx, unsigned int newy)throw(EBIImageCorruptedEXP);
  /**Skalierung. Diese Methode gestattet es, ein Bild zu skalieren. Die neuen Werte
  für Breite und Höhe werden aus der übergebenen Pixelmaximalanzahl berechnet.
  Das Bild in der aktuellen Instanz wird verändert.
  @param sqrpixels Maximalanzahl an Pixeln im Bild nach der Skalierung.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & scale (unsigned int sqrpixels)throw(EBIImageCorruptedEXP);
  /**Skalierung. Diese Methode gestattet es, ein Bild zu skalieren. Die neue Höhe wird 
  unter Beachtung des Höhen-Breiten-Verhältnisses aus der angegebenen neuen Breite
  berechnet.
  Das Bild in der aktuellen Instanz wird verändert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param newwidth Neue Breite des Bildes.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & scaleToWidth (unsigned int newwidth)throw(EBIImageCorruptedEXP);
  /**Skalierung. Diese Methode gestattet es, ein Bild zu skalieren. Die neue Breite wird 
  unter Beachtung des Höhen-Breiten-Verhältnisses aus der angegebenen neuen Höhe
  berechnet.
  Das Bild in der aktuellen Instanz wird verändert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param newheight Neue Höhe des Bildes.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & scaleToHeight (unsigned int newheight)throw(EBIImageCorruptedEXP);
    /**Überblenden. Mit dieser Methode können zwei Bilder mit festen Faktoren überblendet werden.
    Dabei werden die Intensitäten jedes Bildes für jedes Pixel bandweise addiert. Faktoren
    bestimmen, mit welchem Anteil die aktuelle und die übergebene Instanz zu dieser Addition 
    beitragen. Ist die Summe der Faktoren größer als 0, werden beide entsprechend zurückskaliert.
    Stimmen die Bänderanzahlen beider Bilder nicht überein, wird eine Exception geworfen.
 	Das Bild in der aktuellen Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
 	@param other Bild, das demjenigen in der aktuellen Instanz überlagert werden soll.
 	@param otherfac Faktor für other.
 	@param ownfac Faktor für die aktuelle Instanz.
 	@param left Offset von other vom linken Rand.
 	@param top Offset von other vom oberen Rand.
	@return Referenz auf die aktuelle Instanz.*/
    EB_Image & overlay (EB_Image & other, float otherfac = 0.5, float ownfac = 0.5, int left = 0, int top = 0)throw(EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
    /**Überblenden. Mit dieser Methode können zwei Bilder mit festen Faktoren überblendet werden.
    Dabei werden die Intensitäten jedes Bildes für jedes Pixel bandweise addiert. Wie stark der
    Einfluß des Bildes in der aktuellen Instanz auf das Ergebnis ist, wird durch die
    Intensität an der entsprechenden Stelle im festgelegten Band im sogenannten 
    alphaimage bestimmt. Je höher die Intensität in diesem Band ist, desto stärker bestimmt
    der Inhalt von other das Resultat.
    Stimmen die Bänderanzahlen der aktuellen Instanz und von other nicht überein, 
    wird eine Exception geworfen. Das Bild in der aktuellen Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
 	@param other Bild, das demjenigen in der aktuellen Instanz überlagert werden soll.
 	@param alphaimage Bild, dessen Intensitätswerte bestimmen, zu welchem Anteil die aktuelle 
	@param bandnumber Nummer des Bandes aus alphaimage, das als Maske benutzt werden soll.
	@param left Offset von other vom linken Rand.
 	@param top Offset von other vom oberen Rand.
 	@param left Offset von alphaimage vom linken Rand.
 	@param top Offset von alphaimage vom oberen Rand.
 	@param factor Mit diesem Faktor ist eine Intensitätsbeeinflussung des Ergebnisses möglich.
 	Mit diesem Wert wird das Ergebnis der beschriebenen Addition multipliziert. Bei einem Wert
 	von 0.5 ergibt sich also ein Ergebnis, welches halb so hell als ohne diese Multiplikation 
 	wäre.
	@return Referenz auf die aktuelle Instanz.*/
    EB_Image & overlay (EB_Image & other, EB_Image & alphaimage, unsigned int bandnumber, int left = 0, int top = 0, int aleft = 0, int atop = 0, float factor = 1.0f)throw(EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
    /**Überblenden. Mit dieser Methode können zwei Bilder mit festen Faktoren überblendet werden.
    Dabei werden die Intensitäten jedes Bildes für jedes Pixel bandweise addiert. Wie stark der
    Einfluß des Bildes in der aktuellen Instanz auf das Ergebnis ist, wird durch die
    Intensität an der entsprechenden Stelle im entsprechenden Band im sogenannten 
    alphaimage bestimmt. Je höher die Intensität in diesem Band ist, desto stärker bestimmt
    der Inhalt von other das Resultat.
    Stimmen die Bänderanzahlen der drei Bilder nicht überein, wird eine Exception geworfen.
 	Das Bild in der aktuellen Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
 	@param other Bild, das demjenigen in der aktuellen Instanz überlagert werden soll.
 	@param alphaimage Bild, dessen Intensitätswerte bestimmen, zu welchem Anteil die aktuelle 
 	Instanz und otherfac in des Ergebnis eingehen sollen.
 	@param left Offset von other vom linken Rand.
 	@param top Offset von other vom oberen Rand.
 	@param left Offset von alphaimage vom linken Rand.
 	@param top Offset von alphaimage vom oberen Rand.
	@return Referenz auf die aktuelle Instanz.*/
    EB_Image & overlay (EB_Image & other, EB_Image & alphaimage, int left = 0, int top = 0, int aleft = 0, int atop = 0)throw(EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
	/**Farbreduktion.Werden die Zusammenfassungen der Intensitätswerte aller Bänder an einer 
	bestimmten Stelle als Farbe dieses Pixels bezeichnet, ist diese Funktion in der Lage, die
	Anzahl unterschiedlicher Farben im Bild zu reduzieren. Diese Reduktion geschieht in der 
	vorliegenden Implementation durch eine Clusterung der Farben bei vorher festgelegter 
	Anzahl der Cluster. Ist dies geschehen, wird jedem Pixel als Farbe das Zentrum des Clusters
	zugeordnet, zu dem seine Farbe gehört.	
	Das Bild in der aktuellen Instanz wird verändert.
	@param colorcount Anzahl der Farben, die übrigbleiben sollen.
	@param quality Ein zwischen 0.3 und 1.0 beschränkter Faktor, der bestimmt, wie viele
	Pixel zur Bestimmung der Clusterzentren einbezogen werden sollen. Je höher dieser Wert, 
	desto besser ist das Ergebnis, desto mehr Rechenzeit wird aber auch verbraucht.
	@return Referenz auf die aktuelle Instanz.*/
    EB_Image & quantColors (unsigned int colorcount, float quality = 0.3)throw(EBIOutOfMemoryEXP,EBINoValuesInBandEXP);
	/**Farbkorrektur. Diese Methode erlaubt eine Farbkorrektur nach einem patentrechtlich geschützten Verfahren.*/
    EB_Image & correctColors (unsigned int neuroncount = 10, float colorizer = 1.0)throw (EBIWrongNumberOfBandsEXP,EBIOutOfMemoryEXP);
  /**Spiegeln. Das Bild wird an x- und y-Achse gespiegelt. Das Bild in der aktuellen Instanz wird 
  verändert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & doubleMirror (void)throw(EBIImageCorruptedEXP);
  /**Spiegeln. Das Bild wird an x-Achse gespiegelt. Das Bild in der aktuellen Instanz wird 
  verändert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & verticalMirror (void)throw(EBIImageCorruptedEXP);
  /**Spiegeln. Das Bild wird an y-Achse gespiegelt. Das Bild in der aktuellen Instanz wird 
  verändert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & horizontalMirror (void)throw(EBIImageCorruptedEXP);
  /**Glättung. Das Bild wird mit einem quadratischen Boxfilter mit 
  beliebiger Kantenlänge geglättet. Jedes Band wird separat behandelt.
  Jeder Pixel des Bandes wird mit dem Filter beaufschlagt. 
  Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt nicht die eventuell definierte Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param smoothwidth Kantenlänge des quadratischen Filters.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & smoothBox (unsigned int smoothwidth);
  /**Glättung. Das Bild wird mit einem quadratischen Binomialfilter mit 
  beliebiger Kantenlänge geglättet. Jedes Band wird separat behandelt.
  Jeder Pixel des Bandes wird mit dem Filter beaufschlagt. 
  Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt nicht die eventuell definierte Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param smoothwidth Kantenlänge des quadratischen Filters.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & smoothBinom (unsigned int smoothwidth);
  /**Filterung. Das Bild wird mit einem von einer Instanz der Klasse \Ref{EB_Filter} dargestellten
  Filter gefiltert. Jeder Pixel jedes Bandes des Bildes wird mit dem Filter beaufschlagt. Es
  werden zur Zeit nur zweidimensionale Filter zugelassen. Das bedeutet, daß jedes Band mit
  der angegebenen Filtermaske beaufschlagt wird und nur Werte des jeweiligen Bandes zur Berechnung
  des neuen Intensitätswertes benutzt werden können.
  Das Bild in der aktuellen Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param filter Instanz der Klasse \Ref{EB_Filter}.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & convolute (const EB_Filter & filter)throw(EBIImageCorruptedEXP);
  /**Filterung. Das Bild wird mit einem von einer Instanz der Klasse \Ref{EB_Filter} dargestellten
  Filter gefiltert. Jeder Pixel jedes Bandes des Bildes wird entsprechend der Intensität
  in alphaimage an der entsprechenden Stelle mit dem Filter beaufschlagt. Es
  werden zur Zeit nur zweidimensionale Filter zugelassen. Das bedeutet, daß jedes Band mit
  der angegebenen Filtermaske beaufschlagt wird und nur Werte des jeweiligen Bandes zur Berechnung
  des neuen Intensitätswertes benutzt werden können.
  Das Bild in der aktuellen Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param filter Instanz der Klasse \Ref{EB_Filter}.
  @param alphaimage Instanz der Klasse \Ref{EB_Image}, deren Intensitätswerte angeben, wie sehr
  das Ergebnis der Operation für jeden einzelnen Pixel vom gefilterten und vom Originalwert
  abhängt. Dabei wird jeweils das entsprechende Band benutzt.
  Enthält diese Instanz keine Bänder, wird eine Exception geworfen. Enthält sie nur
  ein Band, wird dieses für alle Bänder benutzt. Enthält sie mehr als ein Band, jedoch weniger als 
  die aktuelle Instanz, wird eine Exception geworfen. 
  @param left Offset der linken Ecke des alphaimage. 
  @param top Offset der oberen Ecke des alphaimage.
  @param factor Wenn dieser Wert größer als 1.0 oder kleiner als -1.0 ist, wird ihm der
  nächstliegende der beiden genannten Werte zugewiesen. Ist der Wert danach größer als 0, dient
  er als ganz normale Schwelle: für jeden Pixel wird bestimmt, wie sich der Intensitätswert 
  des jeweiligen Bands in alphaimage zur Länge des Intensitätsintervalls verhält. Ist dieses
  Verhältnis größer als factor, wird der gefilterte Wert in das Ergebnis eingesetzt, ansonsten 
  der Originalwert. Ist factor kleiner als 0, wird der Betrag als Schwelle benutzt. Im Falle 
  der Benutzung des gefilterten Wertes wird dieser nochmals linear mit dem Original verrechnet:
  Mit $o$ als Originalwert, $f$ als gefiltertem Wert, $i$ als Intensität in alphaimage an der 
  entsprechenden Stelle und $r$ als Resultat ergibt sich dann: \[r=f*i+(1-i)*o\]
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & convolute (const EB_Filter & filter, EB_Image & alphaimage, int left = 0, int top = 0, float factor = 0.0) throw (EBINoValuesInBandEXP, EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP);
	/**Morphologische Operation. Diese Methode stellt morphologische Operationen
	mit quadratischen Masken beliebiger Größe dar. Über einen Parameter 
	kann stufenlos eine Betriebsart zwischen den beiden Extrema Erosion
	und Dilatation gewählt werden.
	Jedes Band wird gesondert behandelt. 
  Die aktuelle Instanz wird verändert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param kernelwidth Kantenlänge der quadratischen Maske.
  @param gamma Parameter zur Beeinflussung des Verhaltens der Operation.
  Werte kleiner als 0.0 werden als 0.0 interpretiert, solche größer als 1.0 als 1.0. 
	Dabei entspricht 0.0 der Dilatation und 1.0 der Erosion.  
  @return Referenz auf die aktuelle Instanz.*/
EB_Image &morphologicOperation(unsigned int kernelwidth,float gamma);
	/**Laden. Diese Methode stellt zum einen die Schnittstelle für alle Operationen zum Laden
	von Bildinhalten aus Dateien verschiedener Formate zur Verfügung, zum anderen implementiert 
	sie die Freigabe aller Resourcen, die sowieso neu allokiert werden müssen.
	Das Bild in der aktuellen Instanz wird verändert.
	@return Referenz auf die aktuelle Instanz*/
    virtual EB_Image & load (const char *)
    {
	deallocateBands ();
	deallocatePalette ();
	return (*this);
    };
	/**Speichern. Diese Methode stellt die Schnittstelle für alle Operationen zum Speichern
	von Bildinhalten in Dateien verschiedener Formate zur Verfügung.
	Das Bild in der aktuellen Instanz wird nicht verändert.*/
    virtual void save (const char *)
    {
    };
//  EB_Image &changeOverallIntensity(float factor);
    EB_Image trueType (const char *font, const char *text, unsigned int ptsize, EB_PixelDescriptor oncolor, EB_PixelDescriptor offcolor);
	/**Region definieren. Mit dieser Methode wird der aktuellen eine Instanz der Klasse
	\Ref{EB_ImageRegion} zugeordnet. Diese Klasse stellt zweidimensionale Regionen dar.
	Zur Zeit können nur Regionen sinnvoll benutzt werden. Werden Operationen
	durch diese Regionen beeinflußt, ist das in der Dokumentation angegeben.
	@param region Instanz der Klasse \Ref{EB_ImageRegion}, die der aktuellen Instanz
	zugeordnet werden soll.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Image & setRegion (EB_ImageRegion & region);
  /**Region löschen. Mit dieser Funktion wird die Zuordnung einer Region zum aktuellen Bild
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
  beliebiger Kantenlänge behandelt. Jedes Band wird separat behandelt.
  Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param fux Kantenlänge des Filters in x-Richtung.
  @param fuy Kantenlänge des Filters in y-Richtung.
  @return Referenz auf die aktuelle Instanz.*/
	EB_Image &doMedianFilter(unsigned int fx,unsigned int fy);
	/**Lesen aus einem Stream. Mit dieser Methode kann der Inhalt einer Instanz der Klasse \Ref{EB_Image}  aus einem Stream gelesen werden. Zu Beginn werden der minimale, der maximale Intensitätswert, die Breite, die Höhe und die Anzahl der Bänder gelesen. Anschließend werden die Bänder im Bild den entsprechenden Werten angepaßt und schließlich die Inhalte der Bänder mittels \Ref{EB_Band::readFromStream} eingelesen. Tritt während des Lesens ein Fehler auf, wird eine Exception geworfen.
	@param i Stream, von dem gelesen werden soll.
	@return Referenz auf den übergebenen Stream.*/
	std::istream &readFromStream(std::istream &i) throw (EBICouldNotLoadEXP);
	EB_Image doFire(unsigned int frames=100);
    static advancementcb setAdvancementCallback (advancementcb function);
		/**Abfrage des Streammodus. Siehe \Ref{EB_Band::writeToStream}.
		@return bool true, wenn die Bilder gepackt geschrieben werden sollen.*/
    static bool giveStreamMode (void)
		{
			return(pack);
		}
		/**Setzen des Streammodus. Diese Methode bestimmt, daß Bilder fortan gepackt in den Stream geschrieben werden sollen.*/
    static void setPackedStreamMode ()
		{
			pack=true;
		}
		/**Setzen des Streammodus. Diese Methode bestimmt, daß Bilder fortan ungepackt in den Stream geschrieben werden sollen.*/
    static void setHumanReadableStreamMode ()
		{
			pack=false;
		}
EB_Vector<float> calculateAverages(void);
EB_Vector<float> calculateSinusFilterCascade(void);
//@}
  protected:
  /**Bänder konstruieren. Diese Methode dient als Unterstützung an Stellen, wo 
  alle Bänder einer Instanz der Klasse \Ref{EB_Image} neu konstruiert werden müssen.
 @param color Intensitätswert, mit dem alle Pixel aller Bänder initialisiert werden.
 Liegt dieser Wert außerhalb des durch min und max bestimmten Intervalls,
 findet keine Initialisierung statt.
 @param min Minimaler Intensitätswert in jedem Band.
 @param max Maximaler Intensitätswert in jedem Band.*/ void allocateBands (float color, float min, float max);
  /**Bänder zerstören. Diese Methode erlaubt es, auf einen Schlag alle Bänder
  in der aktuellen Instanz zu zerstören.*/
    void deallocateBands (void);
  /**TODO*/
    void deallocatePalette (void);
    /*geschuetzte Member */
    /// Breite des Bildes in Pixeln.
    unsigned int width;
    /// Höhe des Bildes in Pixeln.
    unsigned int height;
    /// Obere Intensitätsgrenze.
    float maxfloat;
    /// Untere Intensitätsgrenze.
    float minfloat;
    /// Intensitätsvektor eines Pixels.
    EB_PixelDescriptor pixel;
    /// Vektor der Bänder.
    EB_Band **bands;
    /// Palette.
    float **palette;
	/// Anzahl Farben in der Palette.
    unsigned int palettecolors;
	/// Callback-Funktion für die Fortschrittsanzeige.
    static advancementcb advancementcallback;
	/// Region, auf die die Operationen, die das unterstützen, beschränkt bleiben.
    EB_ImageRegion iregion;
		static bool pack;
};
	/**Streamausgabeoperator für \Ref{EB_Image}. Mit diesem Operator kann man Instanzen der Klasse
	\Ref{EB_Image}
	auf C++ Streams ausgeben. Dabei werden zunächst der minimale, der maximale Intensitätswert, die Breite, die Höhe und die Anzahl der Bänder ausgegeben. Anschließend erfolgt die Ausgabe der einzalnen Bänder mittels \Ref{EB_Band::writeToStream}.
	@param o Eine Referenz auf einen Stream, auf den die Ausgabe erfolgen soll.
	@param pic Eine Referenz auf die Instanz der Klasse \Ref{EB_Image}, die ausgegeben werden soll.
	@return Eine Referenz auf den übergebenen Stream.*/
std::ostream &operator<<(std::ostream &o,const EB_Image &pic);
	/**Streameingabeoperator für \Ref{EB_Image}. Mit diesem Operator kann man Daten aus einem Stream in Instanzen der Klasse \Ref{EB_Image}
	laden. Dabei wir ddavon ausgegangen, daß die Daten in einem Format vorliegen, welches dem entspricht, das bei der Beschreibung des Streamausgabeoperators beschrieben wird. Tritt während des Lesens ein Fehler auf, wird eine Exception geworfen.
	@param i Eine Referenz auf einen Stream, von dem gelesen werden soll.
	@param s Eine Referenz auf die Instanz der Klasse \Ref{EB_Image}, die die
	gelesenen Daten aufnehmen soll.
	@return Eine Referenz auf den Stream.*/
std::istream &operator>>(std::istream &i,EB_Image &pic) throw (EBICouldNotLoadEXP);


#endif

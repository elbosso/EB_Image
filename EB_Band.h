/****************************************************************************
*
* File        : EB_Band.h
* Class       : EB_Band
*  based on   :   
*  uses       : 
*
* Author      : VIP-Projektbearbeiter <jkey@cortex.informatik.tu-ilmenau.de>
*               TU Ilmenau / Fak. IA / FG Neuroinformatik
* Date        : Tue May 26 1998 created
*
* Version     : 0.14
* State       : final pre alpha
*
* Description : This class provides some methods from the 
*               classical view of image processing and some others for the
*               image management of class EB_Image. 
*
****************************************************************************/
#ifndef EB_BAND_H
#define EB_BAND_H

/***************************************************************************
 * Used Headerfiles
 **************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include"EB_ColorDistHistogram.h"
#include"EB_Image.h"
#include"EB_ImageRegion.h"
#include"EB_ImageAdvancement.h"
#include"EB_StopWatch.h"
#include"EB_ImageTransformation.h"
#include"EB_IntensityTransformation.h"
#include"EB_TransformationInterpolator.h"
class EB_Image;
class EB_Filter;

/***************************************************************************
 * Some Defines
 **************************************************************************/

#ifndef PI
#define PI 3.14159265359
#endif

#ifndef M_PI
#define M_PI PI
#endif

/*for dynamic loop-unrolling*/
#define LURBLOCKSIZE 8

/***************************************************************************
 * Class Declaration
 **************************************************************************/

/**Diese Klasse stellt zweidimensionale Intensitätsverteilungen dar. Die Intensitäten
werden als Gleitkommazahlen ausgedrückt. Die Instanzen dieser Klasse legen
jeweils eine obere und untere Schranke für die Intensitäten fest. Beim
Versuch, diese Grenzen zu verletzen, tritt ein Sättigungsmechanismus in Kraft.
Diese Klasse kann zum Beispiel als Verkörperung der verschiedenen Bänder in 
Bildern genutzt werden. (Klasse \Ref{EB_Image} tut das zum Beispiel.) Jede der
durch diese Klasse repräsentierten Verteilungen ist rechteckig mit den Kanten
parallel zur x- bzw. y-Achse.Für jede Instanz läßt sich 
eine Region definieren, auf die die Operationen beschränkt werden
sollen (bei manchen Operationen ist das nicht sinnvoll). Diese Regionen sind 
Instanzen der Klasse \Ref{EB_ImageRegion}. Zur Zeit funktionieren nur 
Regionen, deren begrenzende Liniensegmente einander nicht überschneiden.
@author Jürgen "EL BOSSO" Key*/

class EB_Band
{ public:
/**@name Konstruktoren und Destruktor*/
//@{
 /**Default-Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{EB_Band} der Größe 1x1.*/
    EB_Band (void);
 /**Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{EB_Band}.
 @param x Breite des Bildes.
 @param y Höhe des Bildes.
 @param color Intensitätswert, mit dem alle Werte dieses Bandes initialisiert werden.
 Liegt dieser Wert außerhalb des durch min und max bestimmten Intervalls,
 findet keine Initialisierung statt.
 @param min Minimaler Intensitätswert im Band.
 @param max Maximaler Intensitätswert im Band.*/
    EB_Band (unsigned int x, unsigned int y, float color = 0.0, float min = 0.0, float max = 1.0);
 /**Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{EB_Band} als Kopie von der übergebenen Instanz.
 @param band Zeiger auf die Instanz, von der die Kopie erzeugt wird.*/
      EB_Band (const EB_Band * band);
 /**Copy-Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{EB_Band} als Kopie von der übergebenen Instanz.
 @param band Instanz, von der die Kopie erzeugt wird.*/
      EB_Band (const EB_Band & band);
    ///Destruktor
     ~EB_Band ();
    //@}
  /**@name Operatoren*/
    //@{
  /**Zuweisungsoperator. Dieser Operator erzeugt eine Kopie der übergebenen
  in der aktuellen Instanz. Dies ist eine Deep Copy: Es existieren danach
  zwei völlig unabhängige Instanzen.
  @param Band Instanz, die kopiert werden soll.
  @return Referenz auf die aktuelle Instanz*/
      EB_Band & operator = (const EB_Band & Band);
  /**Zuweisungsoperator. Dieser Operator überträgt mittels deep copy die Werte
  in einem C-Array in die aktuelle Instanz. Es muß sichergestellt sein,
  daß das Array minsdestens so viele Elemente enthält, wie das Band.
  @param content Array mit Werten, die in das Band übertragen werden sollen.
  @return Referenz auf die aktuelle Instanz*/
      EB_Band & operator = (const float *content);
  /**Zuweisungsoperator. Dieser Operator überträgt mittels deep copy die Werte
  in einem C-Array in die aktuelle Instanz. Es muß sichergestellt sein,
  daß das Array minsdestens so viele Elemente enthält, wie das Band.
  @param content Array mit Werten, die in das Band übertragen werden sollen.
  Dabei ist es so, daß ein Wert von 0 in die untere Intensitätsgrenze umgesetzt wird,
  ein Wert von 255 entsprechend in die obere und alle Werte dazwischen werden
  entsprechend auf das Intensitätsintervall aufgeteilt.
  @return Referenz auf die aktuelle Instanz*/
      EB_Band & operator = (const unsigned char *content);
  /**Zuweisungsoperator. Dieser Operator überträgt den angegebenen Wert 
  in alle Elemente des Bandes.
  @param content Wert, der in alle Elemente des Bandes übertragen werden soll.
  @return Referenz auf die aktuelle Instanz*/
      EB_Band & operator = (const float content);
  /**Zuweisungsoperator. Dieser Operator überträgt den angegebenen Wert 
  in alle Elemente des Bandes.
  @param content Wert, der in alle Elemente des Bandes übertragen werden soll.
  Dabei ist es so, daß ein Wert von 0 in die untere Intensitätsgrenze umgesetzt wird,
  ein Wert von 255 entsprechend in die obere und alle Werte dazwischen werden
  entsprechend auf das Intensitätsintervall aufgeteilt.
  @return Referenz auf die aktuelle Instanz*/
      EB_Band & operator = (const unsigned char content);
  /**Indexoperator. Diese Methode erlaubt es, auf die Intensität 
  an einer
  bestimmten Stelle zuzugreifen.
  @param index Laufende Nummer des Pixels. Die Pixel werden dabei 
  von der obersten Zeile beginnend zeilenweise durchnummeriert. Es erfolgt
  kein Test, ob dieser Wert für die aktuelle Instanz gültig ist.
  @return Referenz auf einen Intensitätswert.*/
    float &operator[](unsigned int index) const {return(values[index]);};
    //@}
  /**@name public Methoden*/
    //@{
  /**Breite. Diese Methode liefert die Breite der aktuellen Instanz.
  @return Breite der aktuellen Instanz.*/
    unsigned int giveWidth (void) const
    {
	return (width);
    };
  /**Höhe. Diese Methode liefert die Höhe der aktuellen Instanz.
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
  /**Intensitätsbestimmung. Diese Methode erlaubt es, die Intensität 
  an einer
  bestimmten Stelle zu ermitteln. Bei Koordinaten, für die keine Werte 
  vorliegen wird 0.0 zurückgegeben.
  @param x x-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @return Intensitätswert.*/
    float giveFloatValue (unsigned int x, unsigned int y) const
	{ if ((x < width) && (y < height))
	    return (values[x + y * width]);
	else
	    return (0.0);
    };
  /**Intensitätsvektor ermitteln. Diese Methode erlaubt es, die Intensität 
  an einer
  bestimmten Stelle zu ermitteln. Bei Koordinaten, für die keine Werte 
  vorliegen wird 0.0 zurückgegeben.
  @param index Laufende Nummer des Pixels. Die Pixel werden dabei 
  von der obersten Zeile beginnend zeilenweise durchnummeriert.
  @return Intensitätswert.*/
    float giveFloatValue (unsigned int index) const
	{ if (index < pixelcount)
	    return (values[index]);
	else
	    return (0.0);
    };
  /**Intensitätsbestimmung. Diese Methode erlaubt es, die Intensität 
  an einer
  bestimmten Stelle zu ermitteln. Der Intensitätsbereich wird dabei auf den
  Wertebereich des Typs unsigned char abgebildet. Bei Koordinaten, für die keine Werte 
  vorliegen wird 0.0 zurückgegeben.
  @param x x-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @return Intensitätswert.*/
    unsigned char giveCharValue (unsigned int x, unsigned int y) const
	{ if ((x < width) && (y < height))
	    return ((unsigned char) (((values[x + y * width] - minfloat) / (minmaxinterval)) * 255.0));
	else
	    return (0);
    };
  /**Intensitätsbestimmung. Diese Methode erlaubt es, die Intensität 
  an einer
  bestimmten Stelle zu ermitteln. Der Intensitätsbereich wird dabei auf den
  Wertebereich des Typs unsigned char abgebildet. Bei Koordinaten, für die keine Werte 
  vorliegen wird 0 zurückgegeben.
  @param x x-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @return Intensitätswert.*/
    unsigned char giveCharValue (unsigned int index) const
	{ if (index < pixelcount)
	{
//		if((unsigned char) (((values[index] - minfloat) / (minmaxinterval))*255)>128)//values[index]>0.2)
//			cout<<values[index]<<" save "<<(int)((unsigned char) (((values[index] - minfloat) / (minmaxinterval)) * 255.0))<<
//			" "<<minfloat<<" "<<maxfloat<<" "<<minmaxinterval<<endl;
	    return ((unsigned char) (((values[index] - minfloat) / (minmaxinterval)) * 255.0));
	}
	else
	    return (0);
    };
  /**Intensitätsänderung. Diese Methode erlaubt es, die Intensität 
  an einer
  bestimmten Stelle zu ändern. Liegt der neue Wert außerhalb des vereinbarten
  Intensitätsintervalls, wird an der entsprechenden Stelle der nächstliegende
  Grenzwert eingesetzt. Bei Koordinaten, für die keine Werte 
  vorliegen wird die Änderung ignoriert.
  @param x x-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param color Intensitätswert.*/
    void setValue (unsigned int x, unsigned int y, float value)
    {
	if ((x < width) && (y < height))
	    {
		if (value > maxfloat)
		    values[x + y * width] = maxfloat;
		else if (value < minfloat)
		    values[x + y * width] = minfloat;
		else
		    values[x + y * width] = value;
	    }
    };
  /**Intensitätsänderung. Diese Methode erlaubt es, die Intensität 
  an einer
  bestimmten Stelle zu ändern. Liegt der neue Wert außerhalb des vereinbarten
  Intensitätsintervalls, wird an der entsprechenden Stelle der nächstliegende
  Grenzwert eingesetzt. Bei Koordinaten, für die keine Werte 
  vorliegen wird die Änderung ignoriert.
  @param pindex Laufende Nummer des Pixels. Die Pixel werden dabei 
  von der obersten Zeile beginnend zeilenweise durchnummeriert.
  @param color Intensitätswert.*/
    void setValue (unsigned int index, float value)
    {
	if (index < pixelcount)
	    {
		if (value > maxfloat)
		    values[index] = maxfloat;
		else if (value < minfloat)
		    values[index] = minfloat;
		else
		    values[index] = value;
	    }
    };
  /**Intensitätsänderung. Diese Methode erlaubt es, die Intensität 
  an einer
  bestimmten Stelle zu ändern. Dabei entspricht ein Wert von 0 dem unteren, ein Wert von 
  255 dem oberen Ende des für dieses Band festgelegten Intensitätsintervalls.
  Bei Koordinaten, für die keine Werte 
  vorliegen wird die Änderung ignoriert.
  @param x x-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param y y-Koordinate des Pixels, dessen Intensitätswert ermittelt werden soll.
  @param color Intensitätswert.*/
    void setValue (unsigned int x, unsigned int y, unsigned char value)
    {
	float color, v = (float) value;

	if ((x < width) && (y < height))
	    {
		color = v / 256.0 * (minmaxinterval) + minfloat;
		values[x + y * width] = color;
	    }
    };
  /**Intensitätsänderung. Diese Methode erlaubt es, die Intensität 
  an einer
  bestimmten Stelle zu ändern. Dabei entspricht ein Wert von 0 dem unteren, ein Wert von 
  255 dem oberen Ende des für dieses Band festgelegten Intensitätsintervalls.
  Bei Koordinaten, für die keine Werte 
  vorliegen wird die Änderung ignoriert.
  @param pindex Laufende Nummer des Pixels. Die Pixel werden dabei 
  von der obersten Zeile beginnend zeilenweise durchnummeriert.
  @param color Intensitätswert.*/
    void setValue (unsigned int index, unsigned char value)
    {
	float color, v = (float) value;

	if (index < pixelcount)
	    {
		color = v / 256.0 * (minmaxinterval) + minfloat;
		values[index] = color;
	    }
    };
///Dimensionen ändern.
EB_Band & changeDimensions(unsigned int w,unsigned int h);
  /**Komponieren von Bändern. Diese Methode überträgt den Inhalt der
  aktuellen Instanz in eine andere. Dabei kann man noch einen 
  Offset angeben, der die Position im Ziel angibt. Dieser Offset
  bezieht sich auf die linke obere Ecke. Sind im aktuellen Band keine Werte vorhanden
  wird eine Exception
  geworfen. Die aktuelle Instanz wird hierbei nicht verändert
  @param other Instanz der Klasse \Ref{EB_Band} und Ziel der Kopie.
  @param left Offset vom linken Rand für die Kopie.
  @param top Offset vom oberen Rand für die Kopie.
  @return Referenz auf other.*/
    EB_Band & copyWholeTo (EB_Band & other, int left, int top);
  /**Komponieren von Bändern. Diese Methode überträgt den Inhalt einer 
  zweiten Instanz in die aktuelle Instanz. Dabei kann man noch einen 
  Offset angeben, der die Position im Ziel angibt. Dieser Offset
  bezieht sich auf die linke obere Ecke. Enthält ein Band keine
  Daten, wird eine  Exception
  geworfen. Die aktuelle Instanz wird verändert.
  @param other stanz der Klasse \Ref{EB_Band} und Quelle der Kopie.
  @param left Offset vom linken Rand für die Kopie.
  @param top Offset vom oberen Rand für die Kopie.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & copyWholeFrom (EB_Band & other, int left, int top);
	/**Intensitätsintervall ändern. Mit dieser Methode werden die Grenzen
	des Intensitätsintervalls geändert. Gleichzeitig werden alle 
	Intensitätswerte entsprechend dem neuen Intervall skaliert.
	Wird das Intervall von 0 bis 10 auf 0 bis 1 geändert,
  wird ein Intensitätswert von beispielsweise vorher 5 auf 
  0.5 geändert. Die aktuelle Instanz wird verändert. 
  @param newmin Minimaler Intensitätswert in jedem Band.
  @param newmax Maximaler Intensitätswert in jedem Band.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & rescaleIntensity (float newmin, float newmax);
  /**Histogrammausgleich. Diese Methode führt einen Histogrammausgleich auf
  dem Band aus.
  Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Werte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & aequalize (void);
  /**Gammakorrektur. Diese Methode führt eine Gammakorrektur auf
  dem Band aus. Dies ist eine über der Intensität nichtlineare 
  Helligkeitsänderung.
  Dier aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Werte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param factor Parameter für die Gammakorrektur. Ist dieser Wert kleiner 
  als eins, wird die durchschnittliche Intensität geringer, ansonsten höher.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & gammaCorrect (float factor);
  /**Kontraständerung. Diese Methode führt eine Kontraständerung auf
  dem Band aus. 
  Ddie aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param factor Parameter für die Kontraständerung. Je höher dieser Wert ist, desto 
  mehr werden die Intensitätswerte in Richtung der 
  Intensitätsintervallgrenzen verschoben.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & changeContrast (float factor);
  /**Intensitätsänderung. Diese Methode führt eine Intensitätsänderung 
  entsprechend der nichtlinearen Funktion in trans aus. Dabei dienen die
  Intensitäten als Argumente für diese Funktion und die Ergebnisse der Funktion
  als neue Intensitäten. Der Bereich zwischen der oberen und unteren Intensitätsgrenze
  wird bei Argumenten wie Ergebnissen auf das Intervall (0,1) gemappt.
  Der Intensitätsbereich wird in 256 Stufen diskretisiert um die Operation mittels einer Look-Up-Tabelle beschleunigen zu können.
  Ddie aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param trans Instanz einer von \Ref{EB_IntensityTransformation} abgeleiteten
  Klasse, die die eigentliche Funktion repräsentiert.
  @return Referenz auf die aktuelle Instanz.*/
	EB_Band &transformWithLUT(const EB_IntensityTransformation &trans);
  /**Intensitätsänderung. Diese Methode führt eine Intensitätsänderung 
  entsprechend der nichtlinearen Funktion in trans aus. Dabei dienen die
  Intensitäten als Argumente für diese Funktion und die Ergebnisse der Funktion
  als neue Intensitäten. Der Bereich zwischen der oberen und unteren Intensitätsgrenze
  wird bei Argumenten wie Ergebnissen auf das Intervall (0,1) gemappt.
  Ddie aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param trans Instanz einer von \Ref{EB_IntensityTransformation} abgeleiteten
  Klasse, die die eigentliche Funktion repräsentiert.
  @return Referenz auf die aktuelle Instanz.*/
	EB_Band &transform(const EB_IntensityTransformation &trans);
  /**Transformation. Das Band wird entsprechend der Vorschrift im Transformationsobjekt transformiert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param trans Instanz der Klasse \Ref{EB_ImageTransformation} oder von ihr
  abgeleiteter Klassen, die die Transformation beschreibt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & transform (const EB_ImageTransformation &trans);
  /**Transformation. Das Band wird entsprechend der Vorschrift 
  im Transformationsobjekt transformiert.Dabei wird das Bild entsprechend
  der Vorschrift im Interpolationsobjekt interpoliert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param trans Instanz der Klasse \Ref{EB_ImageTransformation} oder von ihr
  abgeleiteter Klassen, die die Transformation beschreibt.
  @param ip Instanz der Klasse \Ref{EB_TransformationInterpolator} oder von ihr
  abgeleiteter Klassen, die die Interpolation beschreibt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & transform (const EB_ImageTransformation &trans,EB_TransformationInterpolator &ip);
  /**Transformation. Das Band wird entsprechend der Vorschrift im 
  Transformationsobjekt transformiert. Es wird eine Look-Up-Tabelle im Transformationobjekt erzeugt,
  was wiederholte Anwendungen dieses Objektes beschleunigt.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param trans Instanz der Klasse \Ref{EB_ImageTransformation} oder von ihr
  abgeleiteter Klassen, die die Transformation beschreibt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & transformWithLUT (const EB_ImageTransformation &trans);
  /**Glättung. Das Band wird mit einem quadratischen Boxfilter mit 
  beliebiger Kantenlänge geglättet.
  Jeder Pixel des Bandes wird mit dem Filter beaufschlagt. 
  Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt nicht die eventuell definierte Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param smoothwidth Kantenlänge des quadratischen Filters.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & smoothBox (unsigned int smoothwidth);
  /**Glättung. Das Band wird mit einem quadratischen Binomialfilterfilter mit 
  beliebiger Kantenlänge geglättet.
  Jeder Pixel des Bandes wird mit dem Filter beaufschlagt. 
  Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt nicht die eventuell definierte Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param smoothwidth Kantenlänge des quadratischen Filters.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & smoothBinom (unsigned int smoothwidth);
  /**Filterung. Das Bild wird mit einem von einer Instanz der Klasse \Ref{EB_Filter} dargestellten
  Filter gefiltert. Jeder Pixel des Bandes wird mit dem Filter beaufschlagt. 
  Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param filter Instanz der Klasse \Ref{EB_Filter}.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & convolute (const EB_Filter & filter);
  /**Filterung. Das Band wird mit einem von einer Instanz der Klasse \Ref{EB_Filter} dargestellten
  Filter gefiltert. Jeder Pixel des Bandes wird entsprechend der Intensität
  in alphachannel an der entsprechenden Stelle mit dem Filter beaufschlagt. 
  Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param filter Instanz der Klasse \Ref{EB_Filter}.
  @param alphaimage Instanz der Klasse \Ref{EB_Band}, deren Intensitätswerte angeben, wie sehr
  das Ergebnis der Operation für jeden einzelnen Pixel vom gefilterten und vom Originalwert
  abhängt. 
  Enthält diese Instanz keine Bänder, wird eine Exception geworfen. 
  @param left Offset der linken Ecke des alphachannel. 
  @param top Offset der oberen Ecke des alphachannel.
  @param factor Wenn dieser Wert größer als 1.0 oder kleiner als -1.0 ist, wird ihm der
  nächstliegende der beiden genannten Werte zugewiesen. Ist der Wert danach größer als 0, dient
  er als ganz normale Schwelle: für jeden Pixel wird bestimmt, wie sich der Intensitätswert 
  in alphachannel zur Länge des Intensitätsintervalls verhält. Ist dieses
  Verhältnis größer als factor, wird der gefilterte Wert in das Ergebnis eingesetzt, ansonsten 
  der Originalwert. Ist factor kleiner als 0, wird der Betrag als Schwelle benutzt. Im Falle 
  der Benutzung des gefilterten Wertes wird dieser nochmals linear mit dem Original verrechnet:
  Mit $o$ als Originalwert, $f$ als gefiltertem Wert, $i$ als Intensität in alphachannel 
  an der 
  entsprechenden Stelle und $r$ als Resultat ergibt sich dann: \[r=f*i+(1-i)*o\]
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & convolute (const EB_Filter & filter, EB_Band & alphachannel, int left = 0, int top = 0, float factor = 0.0)throw(EBIOutOfMemoryEXP);
	/**Morphologische Operation. Diese Methode stellt morphologische Operationen
	mit quadratischen Masken beliebiger Größe dar. Über einen Parameter 
	kann stufenlos eine Betriebsart zwischen den beiden Extrema Erosion
	und Dilatation gewählt werden. 
  Die aktuelle Instanz wird verändert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param kernelwidth Kantenlänge der quadratischen Maske.
  @param gamma Parameter zur Beeinflussung des Verhaltens der Operation.
  Werte kleiner als 0.0 werden als 0.0 interpretiert, solche größer als 1.0 als 1.0. 
	Dabei entspricht 0.0 der Dilatation und 1.0 der Erosion.  
  @return Referenz auf die aktuelle Instanz.*/
EB_Band &morphologicOperation(unsigned int kernelwidth,float gamma);
  /**Rotation. Diese Methode rotiert das Band um den angegebenen Betrag. Die Drehung erfolgt um den angegebenen Punkt, der auch außerhalb liegen darf. 
  Die aktuelle Instanz wird verändert.
  @param angle Dieser Wert gibt den Winkel an, um den das Band gedreht werden soll.
  Dabei bedeutet ein positiver Wert mathematisch positiven Drehsinn (entgegen der Uhrzeigerrichtung).
  Die Einheit der Winkelangabe ist Grad.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param rotcenterx x-Koordinate des Punktes, um den das Bild rotiert wird.
  @param rotcentery y-Koordinate des Punktes,um den das Bild rotiert wird.
  @param fillintensity Bereiche im Band, die nach der Rotation undefinierten Inhalt hätten, 
  werden mit dieser Intensität aufgefüllt.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & rotate (float angle,int rotcenterx,int rotcentery,float fillintensity=-10000.0);
  /**Rotation. Diese Methode rotiert das Band um den angegebenen Betrag. Die Drehung erfolgt um den Bildmittelpunkt. Die Dimensionen des Resultats
  werden so gewählt, daß der gesamte Inhalt des Originals hineinpaßt. 
  Die aktuelle Instanz wird verändert.
  @param angle Dieser Wert gibt den Winkel an, um den das Band gedreht werden soll.
  Dabei bedeutet ein positiver Wert mathematisch positiven Drehsinn (entgegen der Uhrzeigerrichtung).
  Die Einheit der Winkelangabe ist Grad.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param fillintensity Bereiche im Band, die nach der Rotation undefinierten Inhalt hätten, 
  werden mit dieser Intensität aufgefüllt.
  @return Referenz auf die aktuelle Instanz.*/
  EB_Band &rotateToFit(float angle,float fillintensity=-10000.0);
  /**Skalierung. Diese Methode gestattet es, ein Band zu skalieren. Es ist möglich,
  die neue Breite und Höhe unabhängig voneinander anzugeben.
  Die aktuelle Instanz wird verändert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param newx Neue Breite des Bandes.
  @param newy Neue Höhe des Bandes.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & scale (unsigned int newwidth, unsigned int newheight);
    /**Überblenden. Mit dieser Methode können zwei Bänder mit festen Faktoren überblendet werden.
    Dabei werden die Intensitäten des Bandes für jedes Pixel addiert. Faktoren
    bestimmen, mit welchem Anteil die aktuelle und die übergebene Instanz zu dieser Addition 
    beitragen. Ist die Summe der Faktoren größer als 0, werden beide entsprechend zurückskaliert.
 	Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
 	@param other Band, das der aktuellen Instanz überlagert werden soll.
 	@param otherfac Faktor für other.
 	@param ownfac Faktor für die aktuelle Instanz.
 	@param left Offset von other vom linken Rand.
 	@param top Offset von other vom oberen Rand.
	@return Referenz auf die aktuelle Instanz.*/
    EB_Band & overlay (EB_Band & other, float otherfac, float ownfac, int left = 0, int top = 0);
    /**Überblenden. Mit dieser Methode können zwei Bänder mit festen Faktoren überblendet werden.
    Dabei werden die Intensitäten des Bandes für jedes Pixel addiert. Wie stark der
    Einfluß der aktuellen Instanz auf das Ergebnis ist, wird durch die
    Intensität an der entsprechenden Stelle im sogenannten 
    alphachannel bestimmt. Je höher die Intensität in diesem Band ist, desto stärker bestimmt
    der Inhalt von other das Resultat.
    Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region
  und behandelt nur Bildinhalte innerhalb einer solchen Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
 	@param other Band, das der aktuellen Instanz überlagert werden soll.
 	@param alphachannel Band, dessen Intensitätswerte bestimmen, zu welchem Anteil die aktuelle 
	@param left Offset von other vom linken Rand.
 	@param top Offset von other vom oberen Rand.
 	@param left Offset von alphachannel vom linken Rand.
 	@param top Offset von alphachannel vom oberen Rand.
 	@param factor Mit diesem Faktor ist eine Intensitätsbeeinflussung des Ergebnisses möglich.
 	Mit diesem Wert wird das Ergebnis der beschriebenen Addition multipliziert. Bei einem Wert
 	von 0.5 ergibt sich also ein Ergebnis, dessen Intensitätswerte halb so hoch sind,
	wie sie ohne diese Multiplikation wären.
	@return Referenz auf die aktuelle Instanz.*/
    EB_Band & overlay (EB_Band & other, EB_Band & alphachannel, int oleft = 0, int otop = 0, int aleft = 0, int atop = 0, float factor = 1.0f);
  /**Spiegeln. Das Band wird an x- und y-Achse gespiegelt. Die aktuelle Instanz wird 
  verändert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & doubleMirror (void);
  /**Spiegeln. Das Band wird an der x-Achse gespiegelt. Die aktuelle Instanz wird 
  verändert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & verticalMirror (void);
  /**Spiegeln. Das Band wird der y-Achse gespiegelt. Die aktuelle Instanz wird 
  verändert.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & horizontalMirror (void);
    float *giveDataPointer (void)
    {
	return (values);
    };
/*    EB_Band & changeOverallIntensity (float factor)
    {
	factor = fabs (factor);
	if (factor > 1.0)
	    minfloat = maxfloat - (maxfloat - minfloat) * factor;
	else
	    maxfloat = (maxfloat - minfloat) / factor + maxfloat;
	minmaxinterval = maxfloat - minfloat;
	return (*this);
    }
*/
	/**Region definieren. Mit dieser Methode wird der aktuellen eine Instanz der Klasse
	\Ref{EB_ImageRegion} zugeordnet. Diese Klasse stellt zweidimensionale Regionen dar.
	Zur Zeit können nur Regionen sinnvoll benutzt werden. Werden Operationen
	durch diese Regionen beeinflußt, ist das in der Dokumentation angegeben.
	@param region Instanz der Klasse \Ref{EB_ImageRegion}, die der aktuellen Instanz
	zugeordnet werden soll.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & setRegion (EB_ImageRegion & region);
  /**Region löschen. Mit dieser Funktion wird die Zuordnung einer Region zum aktuellen Band
  aufgehoben. Nach Aufruf dieser Methode beeinflussen alle Methoden wieder das ganze Band.
  @return Referenz auf die aktuelle Instanz.*/
    EB_Band & unsetRegion (void);
  /**Medianfilterung. Das Band wird mit einem rechteckigen Medianfilter mit 
  beliebiger Kantenlänge behandelt.
  Jeder Pixel des Bandes wird mit dem Filter beaufschlagt. 
  Die aktuelle Instanz wird verändert.
  Diese Methode berücksichtigt die eventuell definierte Region.
  Diese Methode liefert Fortschrittsberichte über advancementcallback. 
  @param fux Kantenlänge des Filters in x-Richtung.
  @param fuy Kantenlänge des Filters in y-Richtung.
  @return Referenz auf die aktuelle Instanz.*/
	EB_Band &doMedianFilter(unsigned int fux,unsigned int fuy);
	/**Schreiben in Stream. Diese Methode schreibt den Inhalt des Bandes in den angegebenen Stream. Dabei ist das Format wie folgt:zunächst wird der Wert des Parameters pack in den Stream geschrieben. Danach folgen die Intensitätswerte abhängig vom Wert des Parameters pack gepackt oder ungepackt. Ungepackt bedeutet hierbei jeder Intensitätswerte wird durch ein KLeerzeichen getrennt als Gleitkommazahl ausgegeben. Gepacktes Schreiben bedeutet, daß jeweils vier Intensitätswerte zu 8-Bit-Werten diskretisiert und in einen int-Wert gepreßt werden. Dieser int-Wert wird anschließend in den Stream geschrieben. Mit dieser Methode kann der Speicherufwand ungefähr um den Faktor 4 reduziert werden.
	@param o Stream, in den geschrieben werden soll.
	@param pack true, wenn die Daten gepackt geschrieben werden sollen, false für ungepacktes Schreiben.
	@return Referenz auf den übergebenen Stream.*/
	std::ostream &writeToStream(std::ostream &o,bool pack);
	/**Lesen aus einem Stream. Mit dieser Methode kann der Inhalt eines Bandes aus einem Stream gelesen werden. Zu Beginn wird ein Wert gelesen, der bestimmt, ob die Daten gepackt oder ungepackt vorliegen. Anschließend wird eine der Breite und Höhe des Bandes entsprechende Anzahl Daten gelesenen. Tritt während des Lesens ein Fehler auf, wird eine Exception geworfen.
	@param i Stream, von dem gelesen werden soll.
	@return Referenz auf den übergebenen Stream.*/
	std::istream &readFromStream(std::istream &i) throw (EBICouldNotLoadEXP);
	EB_Band doFire(void);
    static void setAdvancementCallback (advancementcb function)
    {
	advancementcallback = function;
    };
void calculateAverages(EB_Vector<float> &v,unsigned int startindex);
float calculateSinusFilterResponse(float frequency);
//@}
  protected:
    /*geschuetzte Methoden */
    EB_Band & lookUp (const	EB_ColorDistHistogram &CDH);
	void select(float *a,int N, int k);
	void swap(float *a,int i, int j)
	{
		float t=a[i];
		a[i]=a[j];
		a[j]=t;
	};
    /*geschuetzte Member */
    /// Breite des Bildes in Pixeln.
    unsigned int width;
    /// Höhe des Bildes in Pixeln.
    unsigned int height;
    // Anzahl der Intensitätswerte im Band insgesamt.
    unsigned int pixelcount;
    /// Obere Intensitätsgrenze.
    float maxfloat;
    /// Untere Intensitätsgrenze.
    float minfloat;
    //Größe des Intensitätsintervalls.
    float minmaxinterval;
    //Array für die Speicherung der Intensitätswerte.
    float *values;
    //Instanz der Klasse \Ref{ColorDistHistogram}.
//    EB_ColorDistHistogram CDH;
     //Structure that holds the values of the first seven HU-Moments calculated with \Ref{calculateMoments}
//       momente moments;
       //Flag shows, if moments have been already calculated
       int momflag; 
	/// Callback-Funktion für die Fortschrittsanzeige.
    static advancementcb advancementcallback;
	/// Region, auf die die Operationen, die das unterstützen, beschränkt bleiben.
    EB_ImageRegion bregion;
};

#endif

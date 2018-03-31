/****************************************************************************
*
* File        : PNM_Image.h
* Class       : PNM_Image
*  based on   : EB_Image  
*  uses       :   
*
* Author      : VIP-Projektbearbeiter <jkey@cortex.informatik.tu-ilmenau.de>
*               TU Ilmenau / Fak. IA / FG Neuroinformatik
* Date        : Wed May 27 1998 created
*
* Version     : 0.04
* State       : final pre-alpha
*
* Description : Root-class for the various image-file-formats known as
*               PBM.
*
****************************************************************************/

#ifndef PNM_IMAGE_H
#define PNM_IMAGE_H

/***************************************************************************
 * Used Headerfiles
 **************************************************************************/

#include "EB_Image.h"

/* some defines */

#define PBM 0x1
#define PGM 0x2
#define PPM 0x4
#define XVPICS 0x8
#define ASCII 0x10
#define BIN 0x20
#define NO_PNM 0x0

#ifndef SEEK_CUR
#define SEEK_CUR 0
#endif
#ifndef SEEK_SET
#define SEEK_SET 1
#endif
/**@name Modul zum Laden und Speichern im PNM-Format.*/
//@{
/**Diese Klasse dient zum Laden und Speichern von Bilddaten. Dabei ist es
nur möglich, Bilder mit einem oder drei Bändern zu speichern. Ein Bild mit einem Band wird dabei beim Speichern als Grauwertbild im PGM-Format und eins mit drei Bändern als Darstellung im RGB-Farbraum im PPM-Format betrachtet. Beim Laden ergibt ein Grauwertbild im PGM-Format entsprechend eine Instanz mit einem Band und ein Farbbild im PPM-Format eine Instanz mit drei Bändern.
@author Jürgen EL BOSSO Key*/
class PNM_Image:public EB_Image
{
public:
/**@name Konstruktoren und Destruktor*/
//@{
/**Default-Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse \Ref{PNM_Image} mit einer Breite und Höhe von eins, einem Band und dem Dynamikbereich von 0.0 bis 1.0.*/  
    PNM_Image (void)
    {
    };
 /**Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{PNM_Image}.
 @param x Breite des Bildes.
 @param y Höhe des Bildes.
 @param usedbands Anzahl an Bändern im Bild.
 @param color Intensitätswert, mit dem alle Pixel aller Bänder initialisiert werden.
 Liegt dieser Wert außerhalb des durch min und max bestimmten Intervalls,
 findet keine Initialisierung statt. Voreinstellung ist -1.0
 @param min Minimaler Intensitätswert in jedem Band. Voreinstellung ist 0.0
 @param max Maximaler Intensitätswert in jedem Band. Voreinstellung ist 1.0*/
    PNM_Image (unsigned int x, unsigned int y, unsigned int usedbands, float color = -1.0, float min = 0.0, float max = 1.0);
//    PNM_Image (const PNM_Image & Image);
 /**Copy-Konstruktor. Dieser Konstruktor erzeugt eine Instanz
 der Klasse \Ref{PNM_Image} als Kopie von der übergebenen Instanz. 
 @param Image Instanz, von der die Kopie erzeugt wird.*/
    PNM_Image (const EB_Image & Image);
 /**File-Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse\Ref{PNM_Image}, indem er versucht, den Inhalt der Datei mit dem angegebenen Namen als im PPM- oder PGM-Format gespeicherte Bilddaten zu interpretieren.
 @param name Name einer Datei, aus der die Bilddaten gelesen werden sollen.*/
    PNM_Image (const char *name);
///Destruktor.
    virtual ~ PNM_Image ();
//@}
/*    EB_Image & operator = (const EB_Image & Image)
    {
	return (EB_Image::operator = (Image));
    };*/
/**@name public Methoden*/
//@{
 /**Bild laden. Diese Methode versucht, den Inhalt der Datei mit dem angegebenen Namen als im PPM- oder PGM-Format gespeicherte Bilddaten zu interpretieren und die Daten entsprechend in die aktuelle Instanz einzutragen. Kann aus der angegebenen Datei nicht gelesen werden, wird eine Exception geworfen.
 @param name Name einer Datei, aus der die Bilddaten gelesen werden sollen.
 @return Referenz auf die aktuelle Instanz.*/
    virtual EB_Image & load (const char *name)throw(EBIOutOfMemoryEXP,EBIPNMLoaderEXP,EBICouldNotLoadEXP,EBICouldNotOpenLoadFileEXP);
/**Bild speichern. Diese Methode speichert die Bilddaten in der aktuellen Instanz abhängig von der Anzahl der Bänder im PPM- oder PGM-Format in eine Datei mit dem angegebenen Namen. Hat die aktuelle Instanz nicht genau ein oder drei Bänder oder kann eine Datei mit dem angegebenen Namen nicht geschrieben werden, wird eine Exception geworfen
@param name Name einer Datei, in der die Daten gespeichert werden sollen.*/
    virtual void save (const char *name)throw(EBIWrongNumberOfBandsEXP,EBICouldNotOpenSaveFileEXP);
//    int giveType (void);
//@}
  protected:
///Typ der Daten.
    int pnmtype;
///Maximaler Intensitätswert.
    int charmax;
};
//@}

#endif

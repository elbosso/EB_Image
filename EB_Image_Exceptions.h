#ifndef EBIMAGEEXCEPTIONS

#define EBIMAGEEXCEPTIONS

#include"stdio.h"
#include<stdlib.h>

/**Wurzelexception. Von dieser Exception werden alle Exceptions abgeleitet,
die direkt von \Ref{EB_Image} oder von ihr abgeleiteten Klassen geworfen werden.
*/
class EB_ImageException
{ public:
    ///Konstruktor
	EB_ImageException (void)
    {
    };
    ///Destruktor
    virtual ~ EB_ImageException (void)
    {
    };
};

/**Diese Exception wird geworfen, wenn beim Laden einer vorhandenen 
Bilddatei ein Problem auftrat.*/
class EBICouldNotLoadEXP:public EB_ImageException
{
};
/**Diese Exception wird geworfen, wenn versucht wurde, eine nicht existente
Bilddatei zu laden.*/
class EBICouldNotOpenLoadFileEXP:public EBICouldNotLoadEXP
{
};
/**Diese Exception wird geworfen, wenn der Typ der Bilddatei nicht von der
benutzten Loader-Klasse verstanden wird.*/
class EBIWrongFileTypeEXP:public EBICouldNotLoadEXP
{
};
/**Diese Exception wird geworfen, wenn das Format der Bilddatei nicht 
verstanden wurde.*/
class EBIUnsupportedFileTypeEXP:public EBICouldNotLoadEXP
{
};
/**Diese Exception wird von der Klasse \Ref{JPG_Image} geworfen, wenn
beim Laden ein f�r diese Klasse spezifisches Problem auftritt*/
class EBIJPGLoaderEXP:public EBICouldNotLoadEXP
{
};
/**Diese Exception wird von der Klasse \Ref{JPG_Image} geworfen, wenn
die sattsam bekannte Condition premature end of ... auftritt.*/
class EBIJPGPrematureEndOfDataEXP:public EBIJPGLoaderEXP
{
};
/**Diese Exception wird von der Klasse \Ref{PNG_Image} geworfen, wenn
beim Laden ein f�r diese Klasse spezifisches Problem auftritt*/
class EBIPNGLoaderEXP:public EBICouldNotLoadEXP
{
};
/**Diese Exception wird von der Klasse \Ref{PNM_Image} geworfen, wenn
beim Laden ein f�r diese Klasse spezifisches Problem auftritt*/
class EBIPNMLoaderEXP:public EBICouldNotLoadEXP
{
};
/**Diese Exception wird geworfen, wenn beim Speichern einer Bilddatei ein 
Problem auftrat.*/
class EBICouldNotSAVEEXP:public EB_ImageException
{
};
/**Diese Exception wird geworfen, wenn die Datei nicht zum Speichern ge�ffnet 
werden konnte.*/
class EBICouldNotOpenSaveFileEXP:public EBICouldNotSAVEEXP
{
};
/**Diese Exception wird von der Klasse \Ref{JPG_Image} geworfen, wenn
beim Speichern ein f�r diese Klasse spezifisches Problem auftrat.*/
class EBIJPGSaverEXP:public EBICouldNotSAVEEXP
{
};
/**Diese Exception wird geworfen, wenn im Zuge irgendeiner Operation
beim Reservieren von Speicher ein Problem auftrat.*/
class EBIOutOfMemoryEXP:public EB_ImageException
{
};
/**Diese Exception wird geworfen, wenn versucht wurde, auf ein nicht 
verf�gbares Bildelement (Band oder Pixel) zuzugreifen.*/
class EBIIndexOutOfRangeEXP:public EB_ImageException
{
};
/**Diese Exception wird geworfen, wenn Parameter au�erhalb des Toleranzbandes
liegen und die jeweilige Methode keine sichere R�ckfallposition kennt.*/
class EBICorruptedParameterEXP:public EB_ImageException
{
};
/**Diese Exception wird geworfen, wenn eine Operation Inhalte in einem 
bestimmten Band voraussetzt, in dem jedoch keine vorhanden sind.*/
class EBINoValuesInBandEXP:public EBICorruptedParameterEXP
{
};
/**Diese Exception wird geworfen, wenn eine Operation auf zwei Bildern die
gleiche Anzahl an B�ndern in ihnen voraussetzt, diese sich aber unterscheiden.*/
class EBINoEqualNumberOfBandsEXP:public EBICorruptedParameterEXP
{
};
/**Diese Exception wird geworfen, wenn f�r eine Operation eine bestimmte
Anzahl B�nder vorausgesetzt wird und diese Voraussetzung aber nicht erf�llt 
wird.*/
class EBIWrongNumberOfBandsEXP:public EBICorruptedParameterEXP
{
};
/**Diese Exception wird geworfen, wenn die Dimensionen des Bildes die gew�hlte Operation nicht zulassen.*/
class EBIWrongImageDimensionsEXP:public EBICorruptedParameterEXP
{
};
/**Diese Exception wird geworfen, wenn die TrueType-Bibliothek ein
nicht tolerierbares Problem anzeigte.*/
class EBITrueTypeEXP:public EB_ImageException
{
};
/**Diese Exception wird geworfen, wenn bei der Verarbeitung klassenintern ein
Fehler auftrat. Sie sollte also eigentlich nie kommen*/
class EBIImageCorruptedEXP:public EB_ImageException
{
};


#endif

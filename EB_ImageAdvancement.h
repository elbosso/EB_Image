#ifndef ADVANCEMENT_H
#define ADVANCEMENT_H

#define STARTPROCESS 0
#define CURRENTPROCESS 1
#define ENDPROCESS 2
#define MESSAGEONLY 3

#include "EB_String.h"

/**Diese Klasse ist als Hilfe für die Klasse \Ref{EB_Image} gedacht.
Damit kann man eine komfortable Callback-Funktionalität speziell für
Fortschrittsmeldungen aufbauen. (Im Prinzip ist dies nur ein besserer struct.)
Funktionen des Typs \Ref{advancementcb} benutzen Instanzen dieser Klasse
als Botschaften, die Informationen über den Bearbeitungssatand einer bestimmten Operation
beinhalten.
@author Jürgen "EL BOSSO" Key*/
class EB_ImageAdvancement
{
public:
/**@name Konstruktoren und Destruktor*/
//@{
/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der 
	Klasse \Ref{EB_ImageAdvancement}.*/ 
	EB_ImageAdvancement(void){};
	///Destruktor.
	virtual ~EB_ImageAdvancement(void){};
//@}
	/**Typ der Botschaft. Es gibt derzeit vier Typen, die eine Botschaft 
	haben kann: 
	\begin{itemize}
	\item STARTPROCESS zur Anzeige, daß eine Operation begonnen hat,
	\item CURRENTPROCESS zur Information über den momentanen Status einer 
	laufenden Operation, 
	\item ENDPROCESS zur Anzeige des Endes einer laufenden 
	Operation und schließlich 
	\item MESSAGEONLY, die keine besonder Bedeutung hat.
	\end{itemize}*/
	unsigned int a_type;
	/// Dieser Wert gibt an, zu wieviel Prozent eine Operation fertiggestellt ist.
	unsigned int a_percentage;
	/// Hier kann noch eine Textmessage übergeben werden.
	EB_String a_message;
};
/**Callbackfunktion zur Fortschrittsanzeige berechnungsintensiver Methoden
der Klassen \Ref{EB_Band} und \Ref{EB_Image}.*/
typedef void(* advancementcb)(EB_ImageAdvancement &a);

#endif
 

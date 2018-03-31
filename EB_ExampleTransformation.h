#ifndef EB_EXAMPLETRANSFORMATION_H
#define EB_EXAMPLETRANSFORMATION_H

#include "EB_ImageTransformation.h"
#include <math.h>

/**@author Jürgen "EL BOSSO" Key*/
class EB_ExampleTransformation:public EB_ImageTransformation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_ExampleTransformation}, die noch keine Punkte definiert hat*/
    EB_ExampleTransformation (void)
    {
    };
    EB_ExampleTransformation(const EB_ExampleTransformation &source):EB_ImageTransformation(source)
	{
	};   
	///Destruktor.
    virtual ~ EB_ExampleTransformation (void)
    {
    };
//@}
/**@name Operatoren*/
//@{
	/**Zuweisungsoperator. Dieser Operator erzeugt eine Kopie der übergebenen
  in der aktuellen Instanz. Dies ist eine Deep Copy: Es existieren danach
  zwei völlig unabhängige Instanzen.
  @param other Instanz, die kopiert werden soll.
  @return Referenz auf die aktuelle Instanz*/
/*    EB_ExampleTransformation & operator = (EB_UnsortedList < EB_ImageCoordinatePair > &list)
    {
	coordlist = list;
	preprocessed = false;
	return (*this);
    }
*/
//@}
/**@name public Methoden*/
//@{
	unsigned int giveDestinationHeight(void)const
	{
		return((/*unsigned int)(1.5*(float)*/height));
	};
	unsigned int giveDestinationWidth(void)const
	{
		return((/*unsigned int)(1.5*(float)*/width));
	};
    unsigned int transform (unsigned int x, unsigned int y)const 
    {
		unsigned int index=0;
		unsigned int newx,newy;
		double angle,smallangle,sinangle,fractionalpart,displacement,t1;
		
		if ((width)&&(height))
		{
			angle=xfrequency*M_PI*(x/(double)width);
			sinangle=sin(angle);
			displacement=(maxxdisplacement*sinangle);
			origx=x+displacement;
			angle=yfrequency*M_PI*(y/(double)height);
			sinangle=sin(angle);
			displacement=(maxydisplacement*sinangle);
			origy=y+displacement;
			newx=(unsigned int)origx;
			newy=(unsigned int)origy;
			if((origy>0.0)&&(origy<height))
				if((origx>0.0)&&(origx<width))
					index=(unsigned int)newx+(unsigned int)newy*width;	
		}
		return(index);
	};
//@}
};

#endif

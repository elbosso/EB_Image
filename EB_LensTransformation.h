#ifndef EB_LENSTRANSFORMATION_H
#define EB_LENSTRANSFORMATION_H

#include "EB_ImageTransformation.h"
#include <math.h>

/**
Diese Klasse implementiert den allgemein bekannten Linsenoperator auf Bildern.
Dabei wird der Bildinhalt an einer bestimmten Koordinate mit einem festen Faktor skaliert. Dieser Faktor variiert mit der Entfernung von diesem Punkt innerhalb eines kreisförmigen Bereichsmit gegebenem Radius. Bereiche Außerhalb des Radius werden nicht beeinflußt.
@author Jürgen "EL BOSSO" Key*/
class EB_LensTransformation:public EB_ImageTransformation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_LensTransformation}. Dabei werden die Parameter so eingestellt, daß amBild nichts geändert wird.*/
    EB_LensTransformation (void):effectiveness(1.0)
    {
		setMaxScaling(1.0);
		setMidPoint(0,0);
    };
    EB_LensTransformation(const EB_LensTransformation &source):EB_ImageTransformation(source)
	{
	};   
	///Destruktor.
    virtual ~ EB_LensTransformation (void)
    {
    };
//@}
/**@name public Methoden*/
//@{
	/**Höhe des Resultats. Diese Methode liefert die Höhe des Resultats der Transformation.
	@return Von den Dimensionen der Eingagsdaten abhängige Höhe des Resultats.*/
	unsigned int giveDestinationHeight(void)const
	{
		return(height);
	};
	/**Breite des Resultats. Diese Methode liefert die Breite des Resultats der Transformation.
	@return Von den Dimensionen der Eingagsdaten abhängige Breite des Resultats.*/
	unsigned int giveDestinationWidth(void)const
	{
		return((/*unsigned int)(1.5*(float)*/width));
	};
	/**Maximum der Verzerrung. Mittels dieser Methode wird die maximale Verzerrung in der Mitte des kreisförmigen Bereichs festgelegt.
	@param s Faktor, mit dem die Mitte skaliert werden soll.*/
	void setMaxScaling(double s)
	{
		scalefactor=s;
		scalerest=(1.0-scalefactor)*effectiveness;
	};
	/**Mittelpunkt festlegen. Diese Methode legt den Mittelpunkt des beeinflussten Bereiches fest.
	@param x x-Koordinate des Mittelpunkts.
	@param y y-Koordinate des Mittelpunkts.*/ 
	void setMidPoint(int x,int y)
	{
		cx=x;
		cy=y;
	};
	/**Radius festlegen. Diese Methode legt den Radius des beeinflussten Bereiches fest.
	@param r Radius des beeinflussten Bereiches.*/
	void setRadius(double r)
	{
		maxradius=r;
	};
	/**Effektivität festlegen. Diese Methode legt fest, wie stark der Skalierungsfaktor bis zur Grenze des beeinflussten Bereichs abnimmt. Ist der Faktor gleich 1.0, so ist der Skalierungsfaktor an dieser Grenze genau 1. 
	@param r Je näher dieser Wert der 0 kommt, desto näher liegt der Skalierungsfaktor an der Grenze des Bereichs an dem in der Mitte.*/
	void setEffectiveness(double r)
	{
		effectiveness=r;
		scalerest*=effectiveness;
	};
/**Transformation. Diese Transformation implementiert eine ortsabhängige Skalierung. Die Skalierung findet in einem kreisförmigen Bereich statt. Der Skalierungsfaktor variiert über den Radius dieses Bereiches.*/
    unsigned int transform (unsigned int x, unsigned int y)const 
    {
		unsigned int index=x+y*width;
		unsigned int newx,newy;
		int sx=x,sy=y;
		double distance,deltax=sx-cx,deltay=sy-cy,angle,factor;
		
		if ((width)&&(height))
		{
			origx=x;
			origy=y;
			distance=sqrt((double)((sx-cx)*(sx-cx))+(double)((sy-cy)*(sy-cy)));
			if(distance<maxradius)
			{
				factor=distance/maxradius;
				angle=atan2(deltay,deltax);
				
				
				origx=cx+cos(angle)*distance*(scalefactor+scalerest*factor);
				origy=cy+sin(angle)*distance*(scalefactor+scalerest*factor);
				newx=(unsigned int)origx;
				newy=(unsigned int)origy;
				if((origy>0.0)&&(origy<height))
					if((origx>0.0)&&(origx<width))
						index=(unsigned int)newx+(unsigned int)newy*width;	
			}
		}
		return(index);
	};
//@}
protected:
	///x-Koordinate des Bereichsmittelpunktes.
	int cx;
	///x-Koordinate des Bereichsmittelpunktes.
	int cy;
	///Maximaler Skalierungsfaktor.
	double scalefactor;
	///Interner Faktor.
	double scalerest;
	///Radius des beeinflussten Bereiches.
	double maxradius;
	///Effektivität.
	double effectiveness;
};

#endif

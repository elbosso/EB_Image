#ifndef EB_SWIRLTRANSFORMATION_H
#define EB_SWIRLTRANSFORMATION_H

#include "EB_ImageTransformation.h"
#include <math.h>

/**Diese Klasse realisiert eine bloße Effekttransformation.
Sie stellt den aus den verschiedensten Bildverarbeitungsprogrammen
bekannten Swirleffekt nach. Dabei werden Bildinhalte nicht um
den gleichen Winkel sondern um einen variablen Winkel gedreht.
Der Winkel wird dabei aus dem Abstand der Pixel vom Drehzentrum bestimmt.
@author Jürgen "EL BOSSO" Key*/
class EB_SwirlTransformation:public EB_ImageTransformation
{ 
public:
/**@name Konstruktoren und Destruktor*/
//@{
	/**Parameterloser Konstruktor. Dieser Konstruktor erzeugt eine Instanz der Klasse
	\Ref{EB_SwirlTransformation}, die noch keine Punkte definiert hat*/
    EB_SwirlTransformation (void):innerangle(0.0),
		outerangle(1.25*M_PI),
		innerradius(20),
		breadth(80),
		outerradius(innerradius+breadth),
		xs(100),
		ys(100)
    {
    };
    EB_SwirlTransformation(const EB_SwirlTransformation &source):EB_ImageTransformation(source)
	{
	};   
	///Destruktor.
    virtual ~ EB_SwirlTransformation (void)
    {
    };
//@}
/**@name public Methoden*/
//@{
	/**Kreisring definieren. Diese Methode definiert zusammen mit \Ref{setInnerRadius} und \Ref{setBreadth} die Region, in der Bildinhalte verändert werden. Hiermit wird der Mittelpunkt des zu transformierenden Kreisringes festgelegt.
	@param x x-Koordinate des Zentrums des Kreisringes.
	@param y y-Koordinate des Zentrums des Kreisringes.
	*/
	void setCenter(int x,int y)
	{
		xs=x;
		ys=y;
		haslut=false;
	}
	/**Kreisring definieren. Diese Methode definiert zusammen mit \Ref{setCenter} und \Ref{setBreadth} die Region, in der Bildinhalte verändert werden. Hiermit wird der innere Radius des zu transformierenden Kreisringes festgelegt.
	@param r Innerer Radius des zu transformierenden Kreisringes.
	*/
	void setInnerRadius(unsigned int r)
	{
		innerradius=r;
		outerradius=innerradius+breadth;
		haslut=false;
	}
	/**Kreisring definieren. Diese Methode definiert zusammen mit \Ref{setInnerRadius} und \Ref{setCenter} die Region, in der Bildinhalte verändert werden. Hiermit wird die Breite und damit implizit der äußere Radius des zu transformierenden Kreisringes festgelegt.
	@param b Breite des zu transformierenden Kreisringes.
	*/
	void setBreadth(unsigned int b)
	{
		breadth=b;
		outerradius=innerradius+breadth;
		haslut=false;
	}
	/**Winkel setzen. Diese Methode setzt den Winkel, der am inneren Radius des Kreisringes zur Rotation der Bildinhalte benutzt werden soll. Die Winkel werden linear zwischen dem mit dieser und dem mit \Ref{setOuterAngle} gesetzten Wert interpoliert. 
	@param a Am inneren Radius des Kreisringes wird dieser Wert benutzt.
	*/
	void setInnerAngle(double a)
	{
		innerangle=a;
		haslut=false;
	}
	/**
Winkel setzen. Diese Methode setzt den Winkel, der am äußeren Radius des Kreisringes zur Rotation der Bildinhalte benutzt werden soll. Die Winkel werden linear zwischen dem mit \Ref{setOuterAngle} und dem mit dieser Methode gesetzten Wert interpoliert. 
	@param a Am äußeren Radius des Kreisringes wird dieser Wert benutzt.
	*/
	void setOuterAngle(double a)
	{
		outerangle=a;
		haslut=false;
	}
	/**Höhe des Resultats. 
	@return Von den Dimensionen der Eingagsdaten abhängige Höhe des Resultats.*/
	unsigned int giveDestinationHeight(void)const
	{
		return((/*unsigned int)(1.5*(float)*/height));
	};
	/**Breite des Resultats. 
	@return Von den Dimensionen der Eingagsdaten abhängige Breite des Resultats.*/
	unsigned int giveDestinationWidth(void)const
	{
		return((/*unsigned int)(1.5*(float)*/width));
	};
	/**Swirl-Operation. Diese Methode führt die aus den verschiedenen Bildbearbeitungsprogrammen bekannte Swirl-Operation aus. Dabei werden die Inhalte eines Kreisringeses des Bildes abhängig vom Abstand vom Mittelpunkt dieses Kreisringes mit variablen Winkeln rotiert.
	@param x x-Koordinate im Resultat.
	@param y y-Koordinate im Resultat.
	@return linear kodierte Koordinate des Punktes in der Eingabedaten, dessen Inhalt an die angegebene
	Stelle des Resultats kopiert werden soll.*/
    unsigned int transform (unsigned int x, unsigned int y)const 
    {
		unsigned int index=0;
		unsigned int newx,newy;
		int xo(x),yo(y);
		double radius,angle,xd,yd,t1;
		
		if ((width)&&(height))
		{
			xd=(double)(xo-xs);
			yd=(double)(yo-ys);
			radius=sqrt(xd*xd+yd*yd);
			if((radius>=innerradius)&&(radius<=outerradius))
			{
				t1=(radius-(double)innerradius)/((double)(breadth));
				angle=atan2(yd,xd)+innerangle+(outerangle-innerangle)*t1;
				origx=cos(angle)*radius+xs;
				origy=sin(angle)*radius+ys;				
			}
			else
			{
				origx=(double)x;
				origy=(double)y;
			}
			newx=(unsigned int)origx;
			newy=(unsigned int)origy;
			if((origy>0.0)&&(origy<height))
				if((origx>0.0)&&(origx<width))
					index=(unsigned int)newx+(unsigned int)newy*width;	
		}
		return(index);
	};
//@}
protected:
	double innerangle;
	double outerangle;
	unsigned int innerradius,breadth,outerradius;
	int xs,ys;
};


#endif

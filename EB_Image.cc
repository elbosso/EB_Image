/****************************************************************************
*
* File        : EB_Image.cc
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


/***************************************************************************
 * Used Headerfile
 **************************************************************************/
#include "EB_Image.h"
#include "EB_Filter.h"
#include <math.h>
#include "EB_StopWatch.h"
#include "EB_IntensitySigmoidTrans.h"

bool EB_Image::pack=false;

advancementcb EB_Image::advancementcallback = NULL;
advancementcb EB_Image::setAdvancementCallback (advancementcb function)
{
		advancementcb tacb=advancementcallback;
    advancementcallback = function;
    EB_Band::setAdvancementCallback (function);
		return (tacb);
}

/*\Konstruktor*/
EB_Image::EB_Image (void):width (1), height (1), maxfloat (1.0), minfloat (0.0), pixel (0), bands (NULL), palette (NULL), palettecolors (0)
{
}
EB_Image::EB_Image (unsigned int x, unsigned int y, unsigned int usedbands, float color, float min, float max):width (x), height (y), maxfloat (max), minfloat (min), pixel (usedbands), palette (NULL), palettecolors (0)
{
    if (min > max)
	{
	    minfloat = max;
	    maxfloat = min;
	}
    else
	{
	    minfloat = min;
	    maxfloat = max;
	}
    allocateBands (color, minfloat, maxfloat);
}
EB_Image::EB_Image (const EB_Image & Image):width (Image.giveWidth ()), height (Image.giveHeight ()), maxfloat (Image.giveMaxFloat ()), minfloat (Image.giveMinFloat ()), pixel (Image.pixel), palette (NULL), palettecolors (0), iregion (Image.iregion)
{
    unsigned int i;		//, colors, components, k;
    //float *tempcolor;
    bands = new EB_Band *[giveBandCount ()];
    for (i = 0; i < giveBandCount (); i++)
	bands[i] = new EB_Band (Image.giveBand (i));
/*    Image.giveInfoAboutPalette (colors, components);
    if (colors)
    {
	palettecolors=colors;
	palette = new float *[palettecolors];
	if (palette)
	{
	    for (i = 0; i < palettecolors; i++)
	    {
		palette[i] = new float[components];
		if (palette[i])
		{
		    tempcolor = Image.giveColorFromPalette (i);
		    for (k = 0; k < components; k++)
			setColorComponentInPalette (i, k, tempcolor[k]);
		}
	    }
	}
    }
*/ }
EB_Image::EB_Image (const EB_Band & Band):width (Band.giveWidth ()), height (Band.giveHeight ()), maxfloat (Band.giveMaxFloat ()), minfloat (Band.giveMinFloat ()), pixel (1), palette (NULL), palettecolors (0)
{
    iregion.undefine ();
    bands = new EB_Band *[giveBandCount ()];
    bands[0] = new EB_Band (&Band);
}
EB_Image::EB_Image (const EB_Band * Band):width (Band->giveWidth ()), height (Band->giveHeight ()), maxfloat (Band->giveMaxFloat ()), minfloat (Band->giveMinFloat ()), pixel (1), palette (NULL), palettecolors (0)
{
    iregion.undefine ();
    bands = new EB_Band *[giveBandCount ()];
    bands[0] = new EB_Band (Band);
}
EB_Image::EB_Image (const char *):width (1), height (1), maxfloat (1.0), minfloat (0.0), pixel (0), bands (NULL), palette (NULL), palettecolors (0)
{
}

/*\*/
/*\Destruktor*/
EB_Image::~EB_Image ()
{
    deallocateBands ();
    deallocatePalette ();
}

/*\*/
/*\EB_Image &EB_Image::operator=(const EB_Image &Image)*/
EB_Image & EB_Image::operator = (const EB_Image & Image)
{
    unsigned int i;		//, colors, components, k;
    //float *tempcolor;

    if (this != &Image)
	{
	    deallocateBands ();
	    deallocatePalette ();
	    width = Image.giveWidth ();
	    height = Image.giveHeight ();
	    pixel = Image.pixel;
	    minfloat = Image.giveMinFloat ();
	    maxfloat = Image.giveMaxFloat ();
	    palette = NULL;
	    palettecolors = 0;
//	    iregion = Image.iregion;
	    bands = new EB_Band *[giveBandCount ()];
	    for (i = 0; i < giveBandCount (); i++)
		{
		    bands[i] = new EB_Band (Image.giveBand (i));
		}
/*    Image.giveInfoAboutPalette (colors, components);
    if (colors)
    {
	palettecolors=colors;
	palette = new float *[palettecolors];
	if (palette)
	{
	    for (i = 0; i < palettecolors; i++)
	    {
		palette[i] = new float[components];
		if (palette[i])
		{
		    tempcolor = Image.giveColorFromPalette (i);
		    for (k = 0; k < components; k++)
			setColorComponentInPalette (i, k, tempcolor[k]);
		}
	    }
	}
*/ }
    return (*this);
}

/*\*/
/*\EB_Image &EB_Image::addBands(const EB_Image &source)*/
EB_Image & EB_Image::addBands (const EB_Image & source)throw(EBIImageCorruptedEXP)
{
    EB_Band **oldbands = bands;
    unsigned int i, oldbandcount = giveBandCount ();

    bands = new EB_Band *[giveBandCount () + source.giveBandCount ()];
    for (i = 0; i < giveBandCount (); i++)
	bands[i] = oldbands[i];
    pixel.changeComponentCount (giveBandCount () + source.giveBandCount ());
    for (i = 0; i < source.giveBandCount (); i++)
	{
	    bands[i + oldbandcount] = new EB_Band (source.giveBand (i));
		try{
	    	giveBand (i + oldbandcount)->rescaleIntensity (giveMinFloat (), giveMaxFloat ());
		}
		catch(...)
		{
			throw EBIImageCorruptedEXP(); 
		}
	}
    return (*this);
}

/*\*/
/*\EB_Image &EB_Image::addBand(const EB_Band &source)*/
EB_Image & EB_Image::addBand (const EB_Band & source)throw(EBIImageCorruptedEXP)
{
    EB_Band **oldbands = bands;
    unsigned int i, oldbandcount = giveBandCount ();

    bands = new EB_Band *[giveBandCount () + 1];
    for (i = 0; i < giveBandCount (); i++)
	bands[i] = oldbands[i];
    pixel.changeComponentCount (giveBandCount () + 1);
    bands[oldbandcount] = new EB_Band (source);
		try{
    giveBand (oldbandcount)->rescaleIntensity (giveMinFloat (), giveMaxFloat ());
		}
		catch(...)
		{
			throw EBIImageCorruptedEXP(); 
		}
    return (*this);
}

/*\*/
/*\EB_Image &EB_Image::overwriteBand(const EB_Band &source,unsigned int index)*/
EB_Image & EB_Image::overwriteBand (const EB_Band & source, unsigned int index) throw (EBIIndexOutOfRangeEXP,EBIImageCorruptedEXP)
{
    if (index < giveBandCount ())
	{
	    delete (bands[index]);
	    bands[index] = new EB_Band (source);
		try{
	    giveBand (index)->rescaleIntensity (giveMinFloat (), giveMaxFloat ());
		}
		catch(...)
		{
			throw EBIImageCorruptedEXP(); 
		}
	}
    else
	throw EBIIndexOutOfRangeEXP ();
    return (*this);
}

/*\*/
/*\EB_Band *EB_Image::giveBand(unsigned int index)*/
EB_Band *EB_Image::giveBand (unsigned int index) const throw (EBIIndexOutOfRangeEXP)
{
    if ((index < giveBandCount ()) && (bands))
	return (bands[index]);
    else
	throw EBIIndexOutOfRangeEXP ();
}

/*\*/
/*\void EB_Image::allocateBands(float color,float min,float max)*/
void EB_Image::allocateBands (float color, float min, float max)
{
    unsigned int i;

    bands = new EB_Band *[giveBandCount ()];
    for (i = 0; i < giveBandCount (); i++)
	bands[i] = new EB_Band (giveWidth (), giveHeight (), color, min, max);
}

/*\*/
/*\void EB_Image::deallocateBands(void)*/
void EB_Image::deallocateBands (void)
{
    unsigned int i;

    if (bands)
	{
	    for (i = 0; i < giveBandCount (); i++)
		if (bands[i])
		    delete (bands[i]);
	    delete (bands);
	}
}

/*\*/
/*\void EB_Image::deallocatePalette(void)*/
void EB_Image::deallocatePalette (void)
{
    unsigned int i;

    if (palette)
	{
	    for (i = 0; i < palettecolors; i++)
		if (palette[i])
		    delete (palette[i]);
	    delete (palette);
	    palette = NULL;
	}
}

/*\*/
/*\EB_Image &EB_Image::copyWholeTo(EB_Image &other,int left,int top)*/
EB_Image & EB_Image::copyWholeTo (EB_Image & other, int left, int top) throw (EBINoValuesInBandEXP, EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP)
{
    unsigned int i;

    if (bands)
	{
	    if (giveBandCount () == other.giveBandCount ())
		{
		    for (i = 0; i < giveBandCount (); i++)
			{
			    if ((giveBand (i)) && (other.giveBand (i)))
				{
				    try
				    {
					giveBand (i)->copyWholeTo (*(other.giveBand (i)), left, top);
				    }
				    catch (EBINoValuesInBandEXP)
				    {
					throw;
				    }
				    catch (...)
				    {
					throw EBIImageCorruptedEXP();
				    }
				}
			}
		}
	    else
		throw EBINoEqualNumberOfBandsEXP ();
	}
    else
	throw EBINoValuesInBandEXP ();
    return (other);
}

/*\*/
/*\EB_Image &EB_Image::copyWholeFrom(const EB_Image &other,int left,int top)*/
EB_Image & EB_Image::copyWholeFrom (const EB_Image & other, int left, int top) throw (EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP)
{
    unsigned int i;

    if (giveBandCount () == other.giveBandCount ())
	{
	    for (i = 0; i < giveBandCount (); i++)
		{
		    if ((giveBand (i)) && (other.giveBand (i)))
			{
			    try
			    {
				giveBand (i)->copyWholeFrom (*(other.giveBand (i)), left, top);
			    }
				catch (EBINoValuesInBandEXP)
				{
					throw;
				}
			    catch (...)
			    {
				throw EBIImageCorruptedEXP();
			    }
			}
		}
	}
    else
	throw EBINoEqualNumberOfBandsEXP ();
    return (*this);
}

/*\*/
/*\EB_Image &EB_Image::rescaleBands(float newmin,float newmax)*/
EB_Image & EB_Image::rescaleBands (float newmin, float newmax)
{
    unsigned int b;

    for (b = 0; b < giveBandCount (); b++)
	{
	    bands[b]->rescaleIntensity (newmin, newmax);
	}
    minfloat = newmin;
    maxfloat = newmax;
    return (*this);
}

/*\*/
EB_Image & EB_Image::convertRGBToLAB (void) throw (EBIWrongNumberOfBandsEXP)
{
 unsigned int i, width(giveWidth()), height(giveHeight()), pixelcount(height * width);
  float re,gr,bl;
  float x,y,z,lv,av,bv;
  EB_Band *l(NULL), *a(NULL), *b(NULL);
  EB_ImageAdvancement adv;
  unsigned int tenpercent(pixelcount / 10);

  adv.a_type = STARTPROCESS;
  adv.a_percentage=1;
  if (3 == giveBandCount())
    {
      if (advancementcallback)
	{
	  (*advancementcallback) (adv);
	  adv.a_type = MESSAGEONLY;
	}
      adv.a_percentage = 0;
      adv.a_type = CURRENTPROCESS;

      l = new EB_Band (width, height, 0.0, 0.0, 100.0);
      a = new EB_Band (width, height, 0.0, -431.0345, 431.0345);
      b = new EB_Band (width, height, 0.0, -172.4138, 172.4138);

      double third(1.0/3.0);
      double thresh(0.008856);
      double afact(16.0/116.0);
      for (i = 0; i < pixelcount; ++i)
	{
	  if ((advancementcallback) && (!(i % tenpercent)))
	    {
	      (*advancementcallback) (adv);
	      adv.a_percentage += 10;
	    }
	  re = bands[0]->giveFloatValue (i);
	  gr = bands[1]->giveFloatValue (i);
	  bl = bands[2]->giveFloatValue (i);

	  //X, Y, Z tristimulus values defined by CIE in 1931
	  //Xn,Yn,Zn for reference white are assumed 1.0, repectively
	  //they are not exactly, really, but they're so little
	  //off and not using them makes this so much faster..
	  //here they are: Xn = 0.959456, Yn = 1.0, Zn = 1.088754.
	  //calculated by summing the factors below for R==G==B==1.0
	  //see http://www.eng2.uconn.edu/~zhangl/convert.htm
	  x = 0.412453 * re + 0.357580 * gr + 0.189423 * bl;
	  y = 0.212671 * re + 0.715160 * gr + 0.072169 * bl;
	  z = 0.019334 * re + 0.119193 * gr + 0.950227 * bl;

	  double yfact;
	  if (y > thresh)
	    {
	      yfact = pow(y,third);
	      lv = 116.0 * yfact - 16.0;
	    }
	  else
	    {
	      lv = 903.3 * y;
	      yfact = 7.787 * y + afact;
	    }

	  av = (x > thresh) ? pow(x,third) : 7.787 * x + afact;
	  av = 500.0 * (av - yfact);

	  bv = (z > thresh) ? pow(z,third) : 7.787 * z + afact;
	  bv = 200.0 *(yfact - bv);

	  l->setValue (i, lv);
	  a->setValue (i, av);
	  b->setValue (i, bv);
	}
      for (i = 0; i < giveBandCount (); i++)
	delete (bands[i]);
      bands[0] = l;
      bands[1] = a;
      bands[2] = b;
    }
  else
    throw EBIWrongNumberOfBandsEXP ();
  adv.a_type = ENDPROCESS;
  if (advancementcallback)
    (*advancementcallback) (adv);
  return (*this);
}

EB_Image & EB_Image::convertLABToRGB (void) throw (EBIWrongNumberOfBandsEXP)
{
 unsigned int i, width(giveWidth()), height(giveHeight()), pixelcount(height * width);
  float l,a,b, ra, ga, ba,x,y,z;
  EB_Band *R(NULL), *G(NULL), *B(NULL);
  EB_ImageAdvancement adv;
  unsigned int tenpercent(pixelcount / 10);

  adv.a_type = STARTPROCESS;
  adv.a_percentage=1;
  if (giveBandCount () == 3)
    {
      if (advancementcallback)
	{
	  (*advancementcallback) (adv);
	  adv.a_type = MESSAGEONLY;
	}
      adv.a_percentage = 0;
      adv.a_type = CURRENTPROCESS;
      bands[0]->rescaleIntensity (0.0, 100.0);
      bands[1]->rescaleIntensity (-431.0345, 431.0345);
      bands[2]->rescaleIntensity (-172.4138, 172.4138);
      R = new EB_Band (width, height, 0.0, 0.0, 1.0);
      G = new EB_Band (width, height, 0.0, 0.0, 1.0);
      B = new EB_Band (width, height, 0.0, 0.0, 1.0);

      for (i = 0; i < pixelcount; ++i)
	{
	  if ((advancementcallback) && (!(i % tenpercent)))
	    {
	      (*advancementcallback) (adv);
	      adv.a_percentage += 10;
	    }
	  l = bands[0]->giveFloatValue (i);
	  a = bands[1]->giveFloatValue (i);
	  b = bands[2]->giveFloatValue (i);

	  float p((l + 16.0)/116.0);

	  //X, Y, Z tristimulus values defined by CIE in 1931
	  x = pow((p + a/500.0), 3.0);
	  y = pow(p,3.0);;
	  z = pow((p - b/200.0),3.0); 

	  ra =  3.240479 * x - 1.537150 * y - 0.498535 * z;
	  ga = -0.969256 * x + 1.875992 * y + 0.041556 * z;
	  ba =  0.055648 * x - 0.204043 * y + 1.057311 * z;

	  R->setValue (i, ra);
	  G->setValue (i, ga);
	  B->setValue (i, ba);
	}
      for (i = 0; i < giveBandCount (); i++)
	delete (bands[i]);
      bands[0] = R;
      bands[1] = G;
      bands[2] = B;
    }
  else
    throw EBIWrongNumberOfBandsEXP ();
  adv.a_type = ENDPROCESS;
  if (advancementcallback)
    (*advancementcallback) (adv);
  return (*this);
}
/*\EB_Image EB_Image::toGray(const EB_PixelDescriptor &p)*/
EB_Image EB_Image::toGray (const EB_PixelDescriptor &p)
{
	unsigned int b,i,pixelcount=giveWidth()*giveHeight(),bands=giveBandCount();
	float sum=0,factor=0.0;
	for(b=0;b<bands;++b)
	{
		if(b+1>=p.giveComponentCount())
			sum+=p[p.giveComponentCount()-1];
		else
			sum+=p[b];
	}
    EB_Image img(giveWidth(),giveHeight(),1,giveMinFloat()*sum-1.0,giveMinFloat()*sum,giveMaxFloat()*sum);
	EB_Band &band=(*(img.giveBand(0)));
	EB_Band *bandptr=NULL;
    EB_ImageAdvancement a;
	float percentage=0.0,percentageincrement=10.0/(float)bands;
    unsigned int tenpercent = pixelcount / 10;

    a.a_type = STARTPROCESS;
    a.a_percentage=1;
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
	for (i = 0; i < pixelcount; i++)
		band[i]=0.0;
	for(b=0;b<bands;++b)
	{
		if(b+1>=p.giveComponentCount())
			factor=p[p.giveComponentCount()-1];
		else
			factor=p[b];
		bandptr=giveBand(b);
	    for (i = 0; i < pixelcount; i++)
		{
		    if ((advancementcallback) && (!(i % tenpercent)))
			{
			    (*advancementcallback) (a);
				percentage+=percentageincrement;
				a.a_percentage =(int)percentage;
			}
			band[i]+=factor*bandptr->giveFloatValue(i);
		}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (img);
}
/*\EB_Image EB_Image::extractSingleBand(unsigned int index)*/
EB_Image &EB_Image::extractSingleBand (unsigned int index)throw(EBINoValuesInBandEXP,EBIIndexOutOfRangeEXP)
{
/*    EB_Image timage (bands[index]->giveWidth (), bands[index]->giveHeight (), 1, bands[index]->giveMinFloat () - 1.0, bands[index]->giveMinFloat (), bands[index]->giveMaxFloat ());

	if(giveBandCount())
	{
    	if (index < giveBandCount ())
	    	timage.bands[0]->copyWholeFrom (*(bands[index]), 0, 0);
		else
			throw EBIIndexOutOfRangeEXP();
    }
	else
		throw EBINoValuesInBandEXP();
	return (timage);
*/
	EB_Band *tband=NULL;
	unsigned int i;
	
	if(giveBandCount())
	{
    	if (index < giveBandCount ())
		{
	    	tband=bands[index];
    		if (bands)
			{
	    		for (i = 0; i < giveBandCount (); i++)
				if (i!=index)
		    		delete (bands[i]);
	    		delete (bands);
			}
    		bands = new EB_Band *[1];
			bands[0] = tband;
			minfloat=tband->giveMinFloat();
			maxfloat=tband->giveMaxFloat();
			width=tband->giveWidth();
			height=tband->giveHeight();
			pixel.changeComponentCount(1);
		}
		else
			throw EBIIndexOutOfRangeEXP();
    }
	else
		throw EBINoValuesInBandEXP();
	return (*this);
}

/*\*/
/*\EB_Image EB_Image::extractSingleBandAsRGB(unsigned int index,unsigned int mode)*/
EB_Image EB_Image::extractSingleBandAsRGB (unsigned int index, unsigned int mode)throw(EBINoValuesInBandEXP,EBIIndexOutOfRangeEXP)
{
    unsigned int i, width = giveWidth (), height = giveHeight (), pixelcount = height * width;
    EB_Image timage (width, height, 3, 0.0, 0.0, 1.0);
    EB_Band *tband = NULL;
    float valu;
//    unsigned int x, y;
    float oldminf, oldmaxf;

	if(giveBandCount())
	{
    	if (index < giveBandCount ())
	{
    tband = giveBand (index);
    oldmaxf = tband->giveMaxFloat ();
    oldminf = tband->giveMinFloat ();
    tband->rescaleIntensity (0.0, 1.0);
    for (i = 0; i < pixelcount; i++)
	{
	    valu = tband->giveFloatValue (i);
	    switch (mode)
		{
		case TORED:
		    {
			timage.setValue (i, valu, RED);
			break;
		    }
		case TOGREEN:
		    {
			timage.setValue (i, valu, GREEN);
			break;
		    }
		case TOBLUE:
		    {
			timage.setValue (i, valu, BLUE);
			break;
		    }
		case TOGRAY:
		    {
			timage.setValue (i, valu, RED);
			timage.setValue (i, valu, GREEN);
			timage.setValue (i, valu, BLUE);
			break;
		    }
		case TOYELLOW:
		    {
			timage.setValue (i, valu, RED);
			timage.setValue (i, valu, GREEN);
			break;
		    }
		case TOPURPLE:
		    {
			timage.setValue (i, valu, RED);
			timage.setValue (i, valu, BLUE);
			break;
		    }
		case TOTURQUOIS:
		    {
			timage.setValue (i, valu, GREEN);
			timage.setValue (i, valu, BLUE);
			break;
		    }
		case BLACKORANGEGREEN:
		    {
			timage.setValue (i, valu, GREEN);
			valu *= 2.0f;
			if (valu > 1.0f)
			    timage.setValue (i, 2.0f - valu, RED);
			else
			    timage.setValue (i, valu, RED);
			break;
		    }
		case YELLOWGREENRED:
		    {
			timage.setValue (i, 1.0f - valu, GREEN);
			valu *= 2.0f;
			if (valu > 1.0f)
			    timage.setValue (i, 1.0f - (2.0f - valu), RED);
			else
			    timage.setValue (i, 1.0f - valu, RED);
			break;
		    }
		case BLACKGREENRED:
		    {
			timage.setValue (i, valu, RED);
			valu *= 2.0f;
			if (valu > 1.0f)
			    timage.setValue (i, (2.0f - valu), GREEN);
			else
			    timage.setValue (i, valu, GREEN);
			break;
		    }
		case YELLOWBROWNGREEN:
		    {
			timage.setValue (i, 1.0f - valu, RED);
			valu *= 2.0f;
			if (valu > 1.0f)
			    timage.setValue (i, 1.0f - (2.0f - valu), GREEN);
			else
			    timage.setValue (i, 1.0f - valu, GREEN);
			break;
		    }
		case TOTEST:
		    {
			timage.setValue (i, 1.0f-valu, RED);
			timage.setValue (i, valu, GREEN);
			timage.setValue (i, 0.0f, BLUE);
			if (valu > 0.45f)
			{
			    timage.setValue (i, (valu-0.45f)*6.0f, GREEN);
					if(valu>0.5)
						timage.setValue (i, 1.0f, RED);
					else
						timage.setValue (i, (valu-0.45f)*20.0f, RED);
			}
			else
			{
			    timage.setValue (i, 0.0f, GREEN);
					timage.setValue (i, 0.0f, RED);
			}
			break;
		    }
		default:
		    {
			timage.setValue (i, valu, RED);
			timage.setValue (i, valu, GREEN);
			timage.setValue (i, valu, BLUE);
			break;
		    }
		}
	}
    tband->rescaleIntensity (oldminf, oldmaxf);
	}
		else
			throw EBIIndexOutOfRangeEXP();
    }
	else
		throw EBINoValuesInBandEXP();
    return (timage);
}

/*\*/
/*\EB_Image EB_Image::cutoutAPiece(int xstart,int ystart,unsigned int xdim,unsigned int ydim)*/
EB_Image EB_Image::cutoutAPiece (int xstart, int ystart, unsigned int xdim, unsigned int ydim)throw (EBINoValuesInBandEXP, EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP)
{
    EB_Image timage (xdim, ydim, giveBandCount (), giveMinFloat (), giveMinFloat (), giveMaxFloat ());

	try{
	    copyWholeTo(timage, -xstart, -ystart);
	}
	catch(...)
	{
		throw;
	}
    return (timage);
}

/*\*/
/*\EB_Image &EB_Image::aequalize(void)*/
EB_Image & EB_Image::aequalize (void)
{
    EB_ImageAdvancement a;
    unsigned int i;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    if (bands)
	for (i = 0; i < giveBandCount (); i++)
	{
		a.a_percentage=i;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", i);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
		bands[i]->aequalize ();
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
}

/*\*/
/*\EB_Image &EB_Image::gammaCorrect(float factor)*/
EB_Image & EB_Image::gammaCorrect (float factor)
{
    EB_ImageAdvancement a;
    unsigned int i;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    if (bands)
	for (i = 0; i < giveBandCount (); i++)
	{
		a.a_percentage=i;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", i);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
	    bands[i]->gammaCorrect (factor);
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
}

/*\*/
/*\EB_Image &EB_Image::changeContrast(float factor)*/
EB_Image & EB_Image::changeContrast (float factor)
{
    EB_ImageAdvancement a;
    unsigned int i;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    if (bands)
	for (i = 0; i < giveBandCount (); i++)
	{
		a.a_percentage=i;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", i);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
	    bands[i]->changeContrast (factor);
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
}

/*\*/
EB_Image & EB_Image::transformWithLUT(const EB_IntensityTransformation &trans)
{
    EB_ImageAdvancement a;
    unsigned int i;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    if (bands)
	for (i = 0; i < giveBandCount (); i++)
	{
		a.a_percentage=i;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", i);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
	    bands[i]->transformWithLUT(trans);
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
}
EB_Image & EB_Image::transform(const EB_IntensityTransformation &trans)
{
    EB_ImageAdvancement a;
    unsigned int i;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    if (bands)
	for (i = 0; i < giveBandCount (); i++)
	{
		a.a_percentage=i;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", i);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
	    bands[i]->transform(trans);
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
}
EB_Image & EB_Image::transform (const EB_ImageTransformation &trans)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (b = 0; b < giveBandCount (); b++)
	{
		a.a_percentage=b;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", b);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
	try{
	    giveBand (b)->transform(trans);
    }
	catch(...)
	{
		throw EBIImageCorruptedEXP();
	}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
	width=giveBand(0)->giveWidth();	
	height=giveBand(0)->giveHeight();	
    return (*this);
}
EB_Image & EB_Image::transform (const EB_ImageTransformation &trans,EB_TransformationInterpolator &ip)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];
//EB_StopWatch w("transform");

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (b = 0; b < giveBandCount (); b++)
	{
		a.a_percentage=b;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", b);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
	try{
	    giveBand (b)->transform(trans,ip);
    }
	catch(...)
	{
		throw EBIImageCorruptedEXP();
	}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
	width=giveBand(0)->giveWidth();	
	height=giveBand(0)->giveHeight();	
    return (*this);
}
EB_Image & EB_Image::transformWithLUT (const EB_ImageTransformation &trans)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (b = 0; b < giveBandCount (); b++)
	{
		a.a_percentage=b;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", b);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
	try{
	    giveBand (b)->transformWithLUT(trans);
    }
	catch(...)
	{
		throw EBIImageCorruptedEXP();
	}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
	width=giveBand(0)->giveWidth();	
	height=giveBand(0)->giveHeight();	
    return (*this);
}
EB_Image EB_Image::computeColorDistances(const EB_ImageSegmentation &segment)
{
    unsigned int i, pixelcount = height * width;
	EB_Image timg(width,height,1,-1.0,0.0,1.0);
    EB_ImageAdvancement a;
    unsigned int tenpercent = pixelcount / 10;

    a.a_type = STARTPROCESS;
    a.a_percentage=1;
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
    for (i = 0; i < pixelcount; ++i)
	{
	    if ((advancementcallback) && (!(i % tenpercent)))
		{
		    (*advancementcallback) (a);
			a.a_percentage += 10;
		}
		timg.setValue(i,segment.computeDistance(givePixel(i)),0);
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
	return(timg);
}	
EB_Image EB_Image::segment(const EB_ImageSegmentation &segment,float maxdistance)
{
    unsigned int i, pixelcount = height * width;
	EB_Image timg(width,height,1,-1.0,0.0,1.0);
    EB_ImageAdvancement a;
    unsigned int tenpercent = pixelcount / 10;

    a.a_type = STARTPROCESS;
    a.a_percentage=1;
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
    for (i = 0; i < pixelcount; ++i)
	{
	    if ((advancementcallback) && (!(i % tenpercent)))
		{
		    (*advancementcallback) (a);
			a.a_percentage += 10;
		}
		if(maxdistance>segment.computeDistance(givePixel(i)))
			timg.setValue(i,1.0F,0);
		else
			timg.setValue(i,0.0F,0);
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
	return(timg);
}	
EB_Image EB_Image::mask(const EB_ImageSegmentation &segment,float maxdistance)
{
    unsigned int i, pixelcount = height * width;
	EB_PixelDescriptor b(giveBandCount(),giveMinFloat());
    EB_ImageAdvancement a;
    unsigned int tenpercent = pixelcount / 10;
	float distance;

    a.a_type = STARTPROCESS;
    a.a_percentage=1;
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
    for (i = 0; i < pixelcount; ++i)
	{
	    if ((advancementcallback) && (!(i % tenpercent)))
		{
		    (*advancementcallback) (a);
			a.a_percentage += 10;
		}
    if (iregion.isInside (i % width, i / width))
		{
			distance=segment.computeDistance(givePixel(i));
			if(maxdistance>distance)
				setPixel(i,givePixel(i));
			else
				setPixel(i,b);
		}
			else
				setPixel(i,b);
		
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
	return(*this);
}	
EB_Image EB_Image::invmask(const EB_ImageSegmentation &segment,float mindistance)
{
    unsigned int i, pixelcount = height * width;
	EB_PixelDescriptor b(giveBandCount(),giveMinFloat());
    EB_ImageAdvancement a;
    unsigned int tenpercent = pixelcount / 10;
	float distance;

    a.a_type = STARTPROCESS;
    a.a_percentage=1;
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
    for (i = 0; i < pixelcount; ++i)
	{
	    if ((advancementcallback) && (!(i % tenpercent)))
		{
		    (*advancementcallback) (a);
			a.a_percentage += 10;
		}
		distance=segment.computeDistance(givePixel(i));
		if(mindistance<distance)
			setPixel(i,givePixel(i));
		else
			setPixel(i,b);
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
	return(*this);
}	
/*\float EB_Image::giveFloatValue(unsigned int x,unsigned int y, unsigned int index)*/
float EB_Image::giveFloatValue (unsigned int x, unsigned int y, unsigned int index)
{
    if (index < giveBandCount ())
	return (bands[index]->giveFloatValue (x, y));
    else
	return (0.0);
}

/*\*/
/*\EB_PixelDescriptor &EB_Image::givePixel(unsigned int x,unsigned int y)*/
EB_PixelDescriptor & EB_Image::givePixel (unsigned int x, unsigned int y)
{
    unsigned int b;

    for (b = 0; b < giveBandCount (); b++)
	pixel[b] = bands[b]->giveFloatValue (x, y);
    return (pixel);
}

/*\*/
/*\EB_PixelDescriptor &EB_Image::givePixel(unsigned int index)*/
EB_PixelDescriptor & EB_Image::givePixel (unsigned int index)
{
    unsigned int b;

    for (b = 0; b < giveBandCount (); b++)
	pixel[b] = bands[b]->giveFloatValue (index);
    return (pixel);
}

/*\*/
/*\unsigned char EB_Image::giveCharValue(unsigned int x,unsigned int y, unsigned int index)*/
unsigned char EB_Image::giveCharValue (unsigned int x, unsigned int y, unsigned int index)
{
    if (index < giveBandCount ())
	return (bands[index]->giveCharValue (x, y));
    else
	return (0);
}

/*\*/
/*\void EB_Image::setValue(unsigned int x,unsigned int y, float color,unsigned int index)*/
void EB_Image::setValue (unsigned int x, unsigned int y, float color, unsigned int index)
{
    if (index < giveBandCount ())
	bands[index]->setValue (x, y, color);
    else
	throw EBIIndexOutOfRangeEXP ();
}

/*\*/
/*\void EB_Image::setPixel(unsigned int x,unsigned int y, EB_PixelDescriptor color)*/
void EB_Image::setPixel (unsigned int x, unsigned int y, EB_PixelDescriptor color)
{
    unsigned int b, c;
    c = color.giveComponentCount () < giveBandCount ()? color.giveComponentCount () : giveBandCount ();
    for (b = 0; b < c; b++)
	giveBand (b)->setValue (x, y, color[b]);
}

/*\*/
/*\void EB_Image::setValue(unsigned int pindex, float color,unsigned int index)*/
void EB_Image::setValue (unsigned int pindex, float color, unsigned int index)
{
    if (index < giveBandCount ())
	bands[index]->setValue (pindex, color);
    else
	throw EBIIndexOutOfRangeEXP ();
}

/*\*/
/*\void EB_Image::setPixel(unsigned int index, EB_PixelDescriptor color)*/
void EB_Image::setPixel (unsigned int index, EB_PixelDescriptor color)
{
    unsigned int b;

    if (color.giveComponentCount () >= giveBandCount ())
	for (b = 0; b < giveBandCount (); b++)
	    giveBand (b)->setValue (index, color[b]);
}

/*\*/
/*\void EB_Image::setValue(unsigned int x,unsigned int y, unsigned char color,unsigned int index)*/
void EB_Image::setValue (unsigned int x, unsigned int y, unsigned char color, unsigned int index)
{
    if (index < giveBandCount ())
	bands[index]->setValue (x, y, color);
    else
	throw EBIIndexOutOfRangeEXP ();
}

/*\*/
/*\void EB_Image::setValue(unsigned int pindex, unsigned char color,unsigned int index)*/
void EB_Image::setValue (unsigned int pindex, unsigned char color, unsigned int index)
{
    if (index < giveBandCount ())
	bands[index]->setValue (pindex, color);
    else
	throw EBIIndexOutOfRangeEXP ();
}
/*void EB_Image::convolute(Filter *filter)
{
    unsigned int i;
  if((bands)&&(filter))
    {
      for(i=0;i<bandcount;i++)
	{ 
	  bands[i]->convolute(filter);
	}
    }
}
*/EB_Image & EB_Image::rotate (float angle,const EB_PixelDescriptor &p)throw(EBIImageCorruptedEXP)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];
	float fillcolor;

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (b = 0; b < giveBandCount (); b++)
	{
		a.a_percentage=b;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", b);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
		try{
			if(b+1>=p.giveComponentCount())
				fillcolor=p[p.giveComponentCount()-1];
			else
				fillcolor=p[b];
			giveBand (b)->rotate (angle,width/2,height/2,fillcolor);
		}
		catch(...)
		{
			throw EBIImageCorruptedEXP();
		}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);	
	return (*this);
}
EB_Image & EB_Image::rotate (float angle,int rotcenterx,int rotcentery,const EB_PixelDescriptor &p)throw(EBIImageCorruptedEXP)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];
	float fillcolor;

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (b = 0; b < giveBandCount (); b++)
	{
		a.a_percentage=b;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", b);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
		try{
			if(b+1>=p.giveComponentCount())
				fillcolor=p[p.giveComponentCount()-1];
			else
				fillcolor=p[b];
			giveBand (b)->rotate (angle,rotcenterx,rotcentery,fillcolor);
		}
		catch(...)
		{
			throw EBIImageCorruptedEXP();
		}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);	
	return (*this);
}
EB_Image &EB_Image::rotateToFit(float angle,const EB_PixelDescriptor &p)throw(EBIImageCorruptedEXP)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];
	float fillcolor;

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (b = 0; b < giveBandCount (); b++)
	{
		a.a_percentage=b;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", b);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
		try{
			if(b+1>=p.giveComponentCount())
				fillcolor=p[p.giveComponentCount()-1];
			else
				fillcolor=p[b];
			giveBand (b)->rotateToFit(angle,fillcolor);
		}
		catch(...)
		{
			throw EBIImageCorruptedEXP();
		}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
	width=giveBand(0)->giveWidth();	
	height=giveBand(0)->giveHeight();	
	return (*this);
}
EB_Image & EB_Image::scale (unsigned int newx, unsigned int newy)throw(EBIImageCorruptedEXP)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (b = 0; b < giveBandCount (); b++)
	{
		a.a_percentage=b;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", b);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
	try{
	    giveBand (b)->scale (newx, newy);
    }
	catch(...)
	{
		throw EBIImageCorruptedEXP();
	}
	}
    width = newx;
    height = newy;
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
}
EB_Image & EB_Image::scale (unsigned int sqrpixels)throw(EBIImageCorruptedEXP)
{
    unsigned int newheight, newwidth;

    newwidth = (int) ((sqrt (((float) sqrpixels * (float) giveWidth ()) / (float) giveHeight ())));
    newheight = (int) ((float) sqrpixels / newwidth);
	return (scale (newwidth, newheight));
}
EB_Image & EB_Image::scaleToWidth (unsigned int newwidth)throw(EBIImageCorruptedEXP)
{
    unsigned int newheight;

    newheight = (unsigned int) (((float) newwidth * (float) giveHeight ()) / (float) giveWidth ());
	return (scale (newwidth, newheight));
}
EB_Image & EB_Image::scaleToHeight (unsigned int newheight)throw(EBIImageCorruptedEXP)
{
    unsigned int newwidth;

    newwidth = (unsigned int) (((float) newheight * (float) giveWidth ()) / (float) giveHeight ());
	return (scale (newwidth, newheight));
}
EB_Image & EB_Image::overlay (EB_Image & other, float otherfac, float ownfac, int left, int top)throw(EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    if (giveBandCount () == other.giveBandCount ())
	{
	    for (b = 0; b < giveBandCount (); b++)
		{
			a.a_percentage=b;
			if (advancementcallback)
			{
				sprintf (buf, "Band %d", b);
						  a.a_message=buf;
				(*advancementcallback) (a);
			}
			try{
				giveBand (b)->overlay (*(other.giveBand (b)), otherfac, ownfac, left, top);
			}
			catch(...)
			{
				throw EBIImageCorruptedEXP();
			}
		}
	}
    else
		throw EBINoEqualNumberOfBandsEXP ();
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
}
EB_Image & EB_Image::overlay (EB_Image & other, EB_Image & alphaimage, unsigned int bandnumber, int left, int top, int aleft, int atop, float factor)throw(EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    if ((giveBandCount () <= other.giveBandCount ()) && (bandnumber < alphaimage.giveBandCount ()))
	{
	    for (b = 0; b < giveBandCount (); b++)
		{
			a.a_percentage=b;
			if (advancementcallback)
			{
				sprintf (buf, "Band %d", b);
						  a.a_message=buf;
				(*advancementcallback) (a);
			}
			try{
				giveBand (b)->overlay (*(other.giveBand (b)), *(alphaimage.giveBand (bandnumber)), left, top, aleft, atop, factor);
			}
			catch(...)
			{
				throw EBIImageCorruptedEXP();
			}
		}
	}
    else
	throw EBINoEqualNumberOfBandsEXP ();
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
}
EB_Image & EB_Image::overlay (EB_Image & other, EB_Image & alphaimage, int left, int top, int aleft, int atop)throw(EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    if ((giveBandCount () == other.giveBandCount ()) && (giveBandCount () == alphaimage.giveBandCount ()))
	{
	    for (b = 0; b < giveBandCount (); b++)
	    {
			a.a_percentage=b;
			if (advancementcallback)
			{
				sprintf (buf, "Band %d", b);
						  a.a_message=buf;
				(*advancementcallback) (a);
			}
			giveBand (b)->overlay (*(other.giveBand (b)), *(alphaimage.giveBand (b)), left, top, aleft, atop);
		}
	}
    else
		throw EBINoEqualNumberOfBandsEXP ();
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
}
EB_Image & EB_Image::quantColors (unsigned int colorcount, float quality)throw(EBIOutOfMemoryEXP,EBINoValuesInBandEXP)
{
    unsigned int iterator, pixelcount = giveWidth () * giveHeight (), i, b = giveBandCount (), samplecount, bm, randompixel;
    float *pixel = NULL, *weights, s, e;
    EB_Kohonen kohonen (1, 1, colorcount, b, 1.0, 0.0, 1.0);

    if (quality > 1.0)
	quality = 1.0;
    else if (quality < 0.01)
	quality = 0.01;
    samplecount = (int) ((float) pixelcount * quality);

	if(b)
	{
    if (colorcount > 1)
	{
	    pixel = new float[b];
	    weights = new float[b];

	    if ((pixel) && (weights))
		{
		    for (iterator = 0; iterator < samplecount; iterator++)
			{
			    randompixel = (int) (((float) rand () / (float) RAND_MAX) * (float) pixelcount);
			    for (i = 0; i < b; i++)
				pixel[i] = giveBand (i)->giveFloatValue (randompixel);
			    s = 1 * (((float) samplecount - (float) iterator) / (float) samplecount) + 0.1;
			    e = 0.3 * (((float) samplecount - (float) iterator) / (float) samplecount) + 0.05;
			    kohonen.UpdateNeurons (pixel, 1, s, e);
			}
		    for (iterator = 0; iterator < pixelcount; iterator++)
			{
			    for (i = 0; i < b; i++)
				pixel[i] = giveBand (i)->giveFloatValue (iterator);
			    bm = kohonen.SearchBestMatching (pixel);
			    kohonen.fillFeatures (0, 0, bm, weights);
			    for (i = 0; i < b; i++)
				giveBand (i)->setValue (iterator, weights[i]);
			}
		    delete (pixel);
		    delete (weights);
		}
	    else
		throw EBIOutOfMemoryEXP ();
	}
	}
    else
		throw EBINoValuesInBandEXP ();
    return (*this);
}
/*\ auskommentiert Palette
void EB_Image::giveInfoAboutPalette(unsigned int &colors,unsigned int &components)
{
    colors=components=0;
	if (palette)
	{
	    colors=palettecolors;
		components=giveBandCount();
	}
}
float *EB_Image::giveColorFromPalette(unsigned int index)
{
    if((palette)&&(index<palettecolors))
	    return(palette[index]);
	else
		return(NULL);   
}
void EB_Image::setColorComponentInPalette(unsigned int color,unsigned int component,float value)
{
    if(((palette)&&(color<palettecolors)))//&&(component<giveBandCount()))
	{
		//if(value<giveBand(component)->giveMinFloat())
		//    value=giveBand(component)->giveMinFloat();
		//if(value>giveBand(component)->giveMaxFloat())
		//    value=giveBand(component)->giveMaxFloat();
		palette[color][component]=value;
	}
}
void EB_Image::makePalette (unsigned int colors, unsigned int components)
{
    unsigned int i,iterator;

    if (palette)
    {
	for (i = 0; i < palettecolors; i++)
	{
	    if (palette[i])
		delete (palette[i]);
	}
	delete (palette);
	palette = NULL;
    }
    palette = new float *[colors];
    palettecolors = colors;
    if (palette)
	for (iterator = 0; iterator < palettecolors; iterator++)
	    palette[iterator] = new float[components];
}
*/
EB_Image & EB_Image::correctColors (unsigned int neuroncount, float colorizer)throw (EBIWrongNumberOfBandsEXP,EBIOutOfMemoryEXP)
{
    unsigned int iterator, pixelcount = giveWidth () * giveHeight (), i, b = giveBandCount (), bm;
    float *pixel = NULL, *weights, s, e;
    PK_EB_Kohonen kohonen (neuroncount);
    float maxz, minz;
    maxz = 0.0;
    minz = 4.5;

    convertRGBToPOMI ();
    pixel = new float[b];
    weights = new float[b];
    if ((pixel) && (weights))
	{
	    for (iterator = 0; iterator < pixelcount; iterator += 2)
		{
//          randompixel = (int) (((float) rand () / (float) RAND_MAX) * (float) pixelcount);
//          for (i = 0; i < b; i++)pixel[i] = giveBand (i)->giveFloatValue (randompixel);
		    for (i = 0; i < b; i++)
			pixel[i] = giveBand (i)->giveFloatValue (iterator);
		    s = 1 * (((float) pixelcount - (float) iterator) / (float) pixelcount) + 0.1;
//          e=0.3*(((float)pixelcount-(float)iterator)/(float)pixelcount)+0.05;
		    e = 0.2;
		    kohonen.UpdateNeurons (pixel, 1, s, e);
		    if (pixel[2] > maxz)
			maxz = pixel[2];
		    if (pixel[2] < minz)
			minz = pixel[2];
		}
	    float fac = 4.5 / (maxz - minz);
	    for (iterator = 0; iterator < pixelcount; iterator++)
		{
		    for (i = 0; i < b; i++)
			pixel[i] = giveBand (i)->giveFloatValue (iterator);
		    bm = kohonen.SearchBestMatching (pixel);
		    kohonen.fillFeatures (0, 0, bm, weights);
		    for (i = 0; i < b - 1; i++)
			giveBand (i)->setValue (iterator, colorizer * (giveBand (i)->giveFloatValue (iterator) - weights[i]));
		    giveBand (2)->setValue (iterator, ((giveBand (2)->giveFloatValue (iterator) - minz) * fac));
		}
	    delete (pixel);
	    delete (weights);
	}
    else
	throw EBIOutOfMemoryEXP ();
    convertPOMIToRGB ();
    return (*this);
}
EB_Image & EB_Image::doubleMirror (void)throw(EBIImageCorruptedEXP)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (b = 0; b < giveBandCount (); b++)
	{
		a.a_percentage=b;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", b);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
		try
		{
			giveBand (b)->doubleMirror ();
    	}
		catch(...)
		{
			throw EBIImageCorruptedEXP();
		}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
}

/*\*/
/*\EB_Image &EB_Image::verticalMirror (void)*/
EB_Image & EB_Image::verticalMirror (void)throw(EBIImageCorruptedEXP)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (b = 0; b < giveBandCount (); b++)
	{
		a.a_percentage=b;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", b);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
		try
		{
			giveBand (b)->verticalMirror ();
    	}
		catch(...)
		{
			throw EBIImageCorruptedEXP();
		}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
}

/*\*/
/*\EB_Image &EB_Image::horizontalMirror (void)*/
EB_Image & EB_Image::horizontalMirror (void)throw(EBIImageCorruptedEXP)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (b = 0; b < giveBandCount (); b++)
	{
		a.a_percentage=b;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", b);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
		try
		{
			giveBand (b)->horizontalMirror ();
    	}
		catch(...)
		{
			throw EBIImageCorruptedEXP();
		}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
}

/*\*/
/*\EB_Image &EB_Image::convolute (const EB_Filter &filter)*/
EB_Image & EB_Image::convolute (const EB_Filter & filter)throw(EBIImageCorruptedEXP)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (b = 0; b < giveBandCount (); b++)
	{
		a.a_percentage=b;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", b);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
		try
		{
			giveBand (b)->convolute (filter);
    	}
		catch(...)
		{
			throw EBIImageCorruptedEXP();
		}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    minfloat = bands[0]->giveMinFloat();
    maxfloat = bands[0]->giveMaxFloat();
    return (*this);
}

/*\*/
/*\EB_Image &EB_Image::convolute(const EB_Filter &filter,EB_Image &alphaimage,int left,int top,float factor)*/
EB_Image & EB_Image::convolute (const EB_Filter & filter, EB_Image & alphaimage, int left, int top, float factor) throw (EBINoValuesInBandEXP, EBINoEqualNumberOfBandsEXP,EBIImageCorruptedEXP)
{
    EB_ImageAdvancement a;
    unsigned int b;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (alphaimage.giveBandCount () > 0)
	{
	    if (advancementcallback)
		{
		    (*advancementcallback) (a);
		    a.a_type = MESSAGEONLY;
		}
	    for (b = 0; b < giveBandCount (); b++)
		{
		a.a_percentage=b;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", b);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
		    if (alphaimage.giveBandCount () == 1)
			giveBand (b)->convolute (filter, *(alphaimage.giveBand (0)), left, top, factor);
		    else
			{
			    try
			    {
				giveBand (b)->convolute (filter, *(alphaimage.giveBand (b)), left, top, factor);
			    }
			    catch (EBIIndexOutOfRangeEXP)
			    {
				throw EBINoEqualNumberOfBandsEXP ();
			    }
			}
		}
	}
    else
	throw EBINoValuesInBandEXP ();
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    minfloat = bands[0]->giveMinFloat();
    maxfloat = bands[0]->giveMaxFloat();
    return (*this);
}

/*\*/
/*\EB_Image &EB_Image::changeOverallIntensity(float factor)*/
/*EB_Image &EB_Image::changeOverallIntensity(float factor)
{
    unsigned int b;

	for (b = 0; b < giveBandCount (); b++)
	    giveBand (b)->changeOverallIntensity(factor);
    if(giveBandCount()>0)
    {
	minfloat=giveBand(0)->giveMinFloat();
	maxfloat=giveBand(0)->giveMaxFloat();
    }  
    return(*this);  
}*/
/*\*/
/*\EB_Image &EB_Image::trueType(const char *font,const char *text,unsigned int ptsize,EB_PixelDescriptor oncolor,EB_PixelDescriptor offcolor)*/
EB_Image EB_Image::trueType (const char *font,const  char *text, unsigned int ptsize, EB_PixelDescriptor oncolor, EB_PixelDescriptor offcolor)
{
    unsigned int b;
    TT_Engine engine;
    TT_Face face;
    TT_Instance instance;
    TT_Glyph glyph;
    TT_CharMap char_map;
    TT_Glyph_Metrics metrics;
    TT_Face_Properties properties;
    TT_Instance_Metrics imetrics;
    int num_glyphs;
    short glyph_code[128];
    int num_codes;
    TT_Error error;
    TT_Raster_Map Bit;
    unsigned short i, n;
    unsigned short platform, encoding;
    TT_F26Dot6 x, y, z, minx, miny, maxx, maxy;
    unsigned char mask;
    unsigned int index;
    EB_Image timg;
    unsigned int alphaindex;

    if ((oncolor.giveComponentCount () >= giveBandCount ()) && (offcolor.giveComponentCount () >= giveBandCount ()))
	{
	    if ((error = TT_Init_FreeType (&engine)))
		throw EBITrueTypeEXP ();
	    error = TT_Open_Face (engine, font, &face);
	    if (error)
		throw EBITrueTypeEXP ();
	    TT_Get_Face_Properties (face, &properties);
	    num_glyphs = properties.num_Glyphs;
	    error = TT_New_Glyph (face, &glyph);
	    if (error)
		throw EBITrueTypeEXP ();
	    error = TT_New_Instance (face, &instance);
	    if (error)
		throw EBITrueTypeEXP ();
	    error = TT_Set_Instance_Resolutions (instance, 92, 92);
	    if (error)
		throw EBITrueTypeEXP ();
	    if ((error = TT_Set_Instance_PointSize (instance, ptsize)))
		throw EBITrueTypeEXP ();
	    TT_Get_Instance_Metrics (instance, &imetrics);
	    n = properties.num_CharMaps;
	    for (i = 0; i < n; i++)
		{
		    TT_Get_CharMap_ID (face, i, &platform, &encoding);
		    if ((platform == 3 && encoding == 1) || (platform == 0 && encoding == 0))
			{
			    TT_Get_CharMap (face, i, &char_map);
			    i = n + 1;
			}
		}
	    if (i == n)
		throw EBITrueTypeEXP ();
	    for (n = 0; n < 128 && text[n]; n++)
		glyph_code[n] = TT_Char_Index (char_map, (short) text[n]);
	    num_codes = n;
	    x = y = 0;

	    minx = miny = maxx = maxy = 0;

	    for (i = 0; i < num_codes; i++)
		{
		    if (!(error = TT_Load_Glyph (instance, glyph, glyph_code[i], TTLOAD_SCALE_GLYPH)))
			{
			    TT_Get_Glyph_Metrics (glyph, &metrics);

			    z = x + metrics.bbox.xMin;
			    if (minx > z)
				minx = z;

			    z = x + metrics.bbox.xMax;
			    if (maxx < z)
				maxx = z;

			    z = y + metrics.bbox.yMin;
			    if (miny > z)
				miny = z;

			    z = y + metrics.bbox.yMax;
			    if (maxy < z)
				maxy = z;

			    x += metrics.advance & -64;
			}
		}
	    minx = (minx & -64) >> 6;
	    miny = (miny & -64) >> 6;

	    maxx = ((maxx + 63) & -64) >> 6;
	    maxy = ((maxy + 63) & -64) >> 6;

	    maxx -= minx;
	    maxy -= miny;

	    Bit.rows = 2 * ptsize;
	    Bit.width = maxx;
	    Bit.flow = TT_Flow_Down;
	    Bit.cols = (Bit.width + 7) >> 3;
	    Bit.size = (long) Bit.cols * Bit.rows;
	    Bit.bitmap = malloc ((int) Bit.size);
	    for (i = 0; i < Bit.size; i++)
		((unsigned char *) (Bit.bitmap))[i] = 0;
	    minx = (Bit.width - maxx) / 2;
	    miny = (Bit.rows + miny) / 2;
	    maxx += minx;
	    maxy += maxy;
	    x = minx;
	    y = miny;

	    for (i = 0; i < num_codes; i++)
		{
		    if (!(error = TT_Load_Glyph (instance, glyph, glyph_code[i], TTLOAD_SCALE_GLYPH)))
			{
			    TT_Get_Glyph_Metrics (glyph, &metrics);
			    TT_Get_Glyph_Bitmap (glyph, &Bit, (long) x * 64, (long) y * 64);
			    x += metrics.advance / 64;
			}
		}
	    if (oncolor.giveComponentCount () > giveBandCount ())
		timg = EB_Image (Bit.width, Bit.rows, giveBandCount () + 1);
	    else
		timg = EB_Image (Bit.width, Bit.rows, giveBandCount ());
	    alphaindex = giveBandCount ();
//      deallocateBands();
//    width=Bit.width;
	    maxx = Bit.cols;
	    maxy = Bit.rows;
//      allocateBands(0.0,0.0,1.0); 
	    for (y = 0; y < maxy; y++)
		{
		    index = y * timg.giveWidth ();
		    for (x = 0; x < maxx; x++)
			{
			    mask = 128;
			    for (i = 0; i < 8; i++)
				{
				    for (b = 0; b < giveBandCount (); b++)
					{
					    if ((((unsigned char *) (Bit.bitmap))[y * Bit.cols + x]) & mask)
						{
						    if (oncolor.giveComponentCount () > giveBandCount ())
							timg.setValue (index, oncolor[alphaindex], alphaindex);
						    timg.setValue (index, oncolor[b], b);
						}
					    else
						timg.setValue (index, offcolor[b], b);
					}
				    index++;
				    mask = mask >> 1;
				}
			}
		}
	    return (timg);
	}
    return (*this);
}
EB_Image & EB_Image::setRegion (EB_ImageRegion & region)
{
    unsigned int i;

    iregion = region;
    for (i = 0; i < giveBandCount (); i++)
	giveBand (i)->setRegion (region);
    return (*this);
};

EB_Image & EB_Image::unsetRegion (void)
{
    unsigned int i;

    iregion.undefine ();
    for (i = 0; i < giveBandCount (); i++)
	giveBand (i)->unsetRegion ();
    return (*this);
};
EB_Image & EB_Image::smoothBox(unsigned int smoothwidth)
{
    unsigned int i;
    EB_ImageAdvancement a;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (i = 0; i < giveBandCount (); i++)
	{
		a.a_percentage=i;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", i);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
	try{
		giveBand (i)->smoothBox (smoothwidth);
    }
	catch(...)
	{
		throw EBIImageCorruptedEXP();
	}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
};
EB_Image & EB_Image::smoothBinom(unsigned int smoothwidth)
{
    unsigned int i;
    EB_ImageAdvancement a;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (i = 0; i < giveBandCount (); i++)
	{
		a.a_percentage=i;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", i);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
	try{
		giveBand (i)->smoothBinom (smoothwidth);
    }
	catch(...)
	{
		throw EBIImageCorruptedEXP();
	}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
};
EB_Image & EB_Image::morphologicOperation(unsigned int kernelwidth,float gamma)
{
    unsigned int i;
    EB_ImageAdvancement a;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (i = 0; i < giveBandCount (); i++)
	{
		a.a_percentage=i;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", i);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
	try{
		giveBand (i)->morphologicOperation(kernelwidth,gamma);
    }
	catch(...)
	{
		throw EBIImageCorruptedEXP();
	}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
};
EB_Image & EB_Image::doMedianFilter(unsigned int fx,unsigned int fy)
{
    unsigned int i;
    EB_ImageAdvancement a;
    char buf[256];

    a.a_type = STARTPROCESS;
    a.a_percentage=giveBandCount();
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
    for (i = 0; i < giveBandCount (); i++)
	{
		a.a_percentage=i;
	    if (advancementcallback)
		{
		    sprintf (buf, "Band %d", i);
                      a.a_message=buf;
		    (*advancementcallback) (a);
		}
		try{
			giveBand (i)->doMedianFilter(fx,fy);
    	}
		catch(...)
		{
			throw EBIImageCorruptedEXP();
		}
	}
    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
    return (*this);
};
EB_Vector<float> EB_Image::calculateAverages(void)
{
    unsigned int i;
	EB_Vector<float> v(3*8*6);

    for (i = 0; i < 3*8*6; i++)
		v[i]=0.0;
    for (i = 0; i < giveBandCount (); i++)
	{
		giveBand (i)->calculateAverages(v,i*8*6);
    }
    return (v);
};
EB_Vector<float> EB_Image::calculateSinusFilterCascade(void)
{
    unsigned int i,b;
	EB_Vector<float> v(3*8);

    for (i = 0; i < 3*8; i++)
		v[i]=0.0;
    for (i = 0; i < 8; i++)
	{
	    for (b = 0; b < giveBandCount (); b++)
		{
			v[i*3+b]=giveBand (b)->calculateSinusFilterResponse((float)i);
		}
    }
    return (v);
};
EB_Image EB_Image::doFire(unsigned int frames)
{
	EB_Image timg(256,256,2,0.0,0.0,255.0);
	EB_Image ttimg;
  unsigned int i,j;
  EB_ImageAdvancement a;
	char buf[256];
	MPEG_Encoder mpg(1,timg.giveWidth(),timg.giveHeight(),2560000,"/home/elbosso/flame.mpg",12,3);
	EB_IntensitySigmoidTrans siggy;
	siggy.setXShift(0.1);
	siggy.setYShift(-0.1);
	siggy.setXStretch(5.0);
	siggy.setYStretch(1.0);

/*    a.a_type = STARTPROCESS;
    a.a_percentage=1;
    if (advancementcallback)
	{
	    (*advancementcallback) (a);
	    a.a_type = MESSAGEONLY;
	}
	a.a_percentage=0;
    if (advancementcallback)
	{
	    sprintf (buf, "Band %d", i);
                     a.a_message=buf;
	    (*advancementcallback) (a);
	}
*/
	for(j=0;j<frames;++j)
	{
		(*(timg.giveBand(0)))=timg.giveBand(1)->doFire();
/*    a.a_type = ENDPROCESS;
    if (advancementcallback)
	(*advancementcallback) (a);
*/
//	(*(timg.giveBand(0)))=(*(timg.giveBand(1)));
	(*(timg.giveBand(0))).rescaleIntensity(0.0,1.0);
	(*(timg.giveBand(0))).transform(siggy);
	ttimg=timg.extractSingleBandAsRGB(0,TOTEST).verticalMirror();
	{
		EB_StopWatch w("mpegenc");
		mpg.addframetoseq(ttimg);
	}
	}
    return (timg.extractSingleBandAsRGB(0,TOTEST));//BLACKORANGEGREEN));
};
istream &EB_Image::readFromStream(istream &i) throw (EBICouldNotLoadEXP)
{
	unsigned int loop,tbc;
	bool streammode;

	if(!(i.fail()))
	i>>minfloat>>
		maxfloat>>
		width>> 
		height>>
		tbc;
	else
		throw new EBICouldNotLoadEXP();
	if(!(i.fail()))
	{
		pixel.changeComponentCount (tbc);
		deallocateBands();
		allocateBands(-1.0, 0.0, 1.0);
		for(loop=0;loop<giveBandCount();++loop)
			giveBand(loop)->readFromStream(i);
	}
	else 
		throw new EBICouldNotLoadEXP();
	return (i);
}
ostream &operator<<(ostream &o,const EB_Image &pic)
{
	unsigned int i;

	o<<pic.giveMinFloat()<<" "<<
		pic.giveMaxFloat()<<" "<<
		pic.giveWidth()<<" "<< 
		pic.giveHeight()<<" "<<
		pic.giveBandCount()<<" ";
		for(i=0;i<pic.giveBandCount();++i)
		{
pic.giveBand(i)->writeToStream(o,pic.giveStreamMode());
			o<<" ";
		}	
	return(o);
}
istream &operator>>(istream &i,EB_Image &pic) throw (EBICouldNotLoadEXP)
{
	return(pic.readFromStream(i));
}

/****************************************************************************
*
* File        : EB_Band.cc
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


/***************************************************************************
 * Used Headerfile
 **************************************************************************/
#include"EB_Band.h"
#include"EB_Filter.h"
#include"EB_IntensitySigmoidTrans.h"

advancementcb EB_Band::advancementcallback = NULL;

/*\Konstruktoren*/
EB_Band::EB_Band (unsigned int x, unsigned int y, float color, float min, float max):width (x), height (y), pixelcount (width * height), values (NULL)//, CDH (256)
{
    unsigned int i;

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
    minmaxinterval = maxfloat - minfloat;
    values = new float[pixelcount];

//    if (color <minfloat)
//        color=minfloat;
//    else if (color >maxfloat)
//       color=maxfloat;
//    else
    if (!(values))
	throw EBIOutOfMemoryEXP ();
    if ((color >= minfloat) && (color <= maxfloat))
	for (i = 0; i < pixelcount; i++)
	    values[i] = color;
//    if(!(values))
//        throw EBIOutOfMemoryEXP();
//      for (i = 0; i < pixelcount; i++)
//          values[i] = color;
}
EB_Band::EB_Band (void):width (1), height (1), pixelcount (width * height), maxfloat(1.0),minfloat(0.0),values (NULL)//, CDH (256)
{
    minmaxinterval = maxfloat - minfloat;
    values = new float[pixelcount];

//    if (color <minfloat)
//        color=minfloat;
//    else if (color >maxfloat)
//       color=maxfloat;
//    else
    if (!(values))
		throw EBIOutOfMemoryEXP ();
//    if ((color >= minfloat) && (color <= maxfloat))
//		for (i = 0; i < pixelcount; i++)
//	    values[i] = color;
//    if(!(values))
//        throw EBIOutOfMemoryEXP();
//      for (i = 0; i < pixelcount; i++)
//          values[i] = color;
}
EB_Band::EB_Band (const EB_Band * Band):width (Band->giveWidth ()), height (Band->giveHeight ()), pixelcount (Band->pixelcount), maxfloat (Band->giveMaxFloat ()), minfloat (Band->giveMinFloat ()), minmaxinterval (Band->minmaxinterval), values (NULL), /*CDH (Band->CDH),*/ bregion (Band->bregion)	/*,
																																					   momflag (0) */
{
    unsigned int i = 0, k, blocklimit;

    values = new float[pixelcount];
    if (!(values))
	throw EBIOutOfMemoryEXP ();

    blocklimit = (pixelcount / LURBLOCKSIZE) * LURBLOCKSIZE;
    while (i < blocklimit)
	{
	    values [i]= Band->operator[](i);
	    i++;
	    values [i]= Band->operator[](i);
	    i++;
	    values [i]= Band->operator[](i);
	    i++;
	    values [i]= Band->operator[](i);
	    i++;
	    values [i]= Band->operator[](i);
	    i++;
	    values [i]= Band->operator[](i);
	    i++;
	    values [i]= Band->operator[](i);
	    i++;
	    values [i]= Band->operator[](i);
	    i++;
	}
    for (k = i; k < pixelcount; k++)
	values [k]= Band->operator[](k);
}
EB_Band::EB_Band (const EB_Band & Band):width (Band.giveWidth ()), height (Band.giveHeight ()), pixelcount (Band.pixelcount), maxfloat (Band.giveMaxFloat ()), minfloat (Band.giveMinFloat ()), minmaxinterval (Band.minmaxinterval), values (NULL), /*CDH (Band.CDH),*/ bregion (Band.bregion)	/*,
																																				   momflag (0) */
{
    unsigned int i = 0, k, blocklimit;

    values = new float[pixelcount];
    if (!(values))
	throw EBIOutOfMemoryEXP ();

    blocklimit = (pixelcount / LURBLOCKSIZE) * LURBLOCKSIZE;
    while (i < blocklimit)
	{
	    values [i]= Band[i];
	    i++;
	    values [i]= Band[i];
	    i++;
	    values [i]= Band[i];
	    i++;
	    values [i]= Band[i];
	    i++;
	    values [i]= Band[i];
	    i++;
	    values [i]= Band[i];
	    i++;
	    values [i]= Band[i];
	    i++;
	    values [i]= Band[i];
	    i++;
	}
    for (k = i; k < pixelcount; k++)
	values [k]= Band[k];
}

/*\*/
/*\Destruktor*/
EB_Band::~EB_Band ()
{
    delete (values);
}

/*\*/
/*public Methoden*/
/*\EB_Band &EB_Band::operator=(const EB_Band &Band)*/
EB_Band & EB_Band::operator = (const EB_Band & Band)
{
    if (this != &Band)
	{
	    width = Band.giveWidth ();
	    height = Band.giveHeight ();
	    pixelcount = Band.pixelcount;
	    maxfloat = Band.giveMaxFloat ();
	    minfloat = Band.giveMinFloat ();
	    minmaxinterval = Band.minmaxinterval;
	    delete values;
	    values = NULL;
//	    CDH = Band.CDH;
	    bregion = Band.bregion;	/*
					   momflag (0) */
	    unsigned int i = 0, k, blocklimit;

	    values = new float[pixelcount];
	    if (!(values))
		throw EBIOutOfMemoryEXP ();

	    blocklimit = (pixelcount / LURBLOCKSIZE) * LURBLOCKSIZE;
	    while (i < blocklimit)
		{
		    values [i]= Band[i];
		    ++i;
		    values [i]= Band[i];
		    ++i;
		    values [i]= Band[i];
		    ++i;
		    values [i]= Band[i];
		    ++i;
		    values [i]= Band[i];
		    ++i;
		    values [i]= Band[i];
		    ++i;
		    values [i]= Band[i];
		    ++i;
		    values [i]= Band[i];
		    ++i;
		}
	    for (k = i; k < pixelcount; k++)
		values [k]= Band[k];
	}
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::operator=(const float *content)*/
EB_Band & EB_Band::operator = (const float *content)
{
    unsigned int i = 0, k, blocklimit;

    if ((values) && (content))
	{
	    blocklimit = (pixelcount / LURBLOCKSIZE) * LURBLOCKSIZE;
	    while (i < blocklimit)
		{
		    values [i]= content[i];
		    i++;
		    values [i]= content[i];
		    i++;
		    values [i]= content[i];
		    i++;
		    values [i]= content[i];
		    i++;
		    values [i]= content[i];
		    i++;
		    values [i]= content[i];
		    i++;
		    values [i]= content[i];
		    i++;
		    values [i]= content[i];
		    i++;
		}
	    for (k = i; k < pixelcount; k++)
		values [k]= content[k];
	}
    else
	throw EBINoValuesInBandEXP ();
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::operator=(const unsigned char *content)*/
EB_Band & EB_Band::operator = (const unsigned char *content)
{
    unsigned int i = 0, k, blocklimit;
    float factor= 1.0/(256.0 * (minmaxinterval) );

    if (values)
	{
	    blocklimit = (pixelcount / LURBLOCKSIZE) * LURBLOCKSIZE;
	    while (i < blocklimit)
		{
		    values[i]=content[i]*factor+ minfloat;
		    i++;
		    values[i]=content[i]*factor+ minfloat;
		    i++;
		    values[i]=content[i]*factor+ minfloat;
		    i++;
		    values[i]=content[i]*factor+ minfloat;
		    i++;
		    values[i]=content[i]*factor+ minfloat;
		    i++;
		    values[i]=content[i]*factor+ minfloat;
		    i++;
		    values[i]=content[i]*factor+ minfloat;
		    i++;
		    values[i]=content[i]*factor+ minfloat;
		    i++;
		}
	    for (k = i; k < pixelcount; k++)
		    values[k]=content[k]*factor+ minfloat;
//		setValue (k, content[k]);
	}
    else
	throw EBINoValuesInBandEXP ();
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::operator=(const float content)*/
EB_Band & EB_Band::operator = (const float content)
{
    unsigned int i = 0, k, blocklimit;

    if (values)
	{
	    blocklimit = (pixelcount / LURBLOCKSIZE) * LURBLOCKSIZE;
	    while (i < blocklimit)
		{
		    values [i]= content;
		    i++;
		    values [i]= content;
		    i++;
		    values [i]= content;
		    i++;
		    values [i]= content;
		    i++;
		    values [i]= content;
		    i++;
		    values [i]= content;
		    i++;
		    values [i]= content;
		    i++;
		    values [i]= content;
		    i++;
		}
	    for (k = i; k < pixelcount; k++)
		values [k]= content;
	}
    else
	throw EBINoValuesInBandEXP ();
    return (*this);
}
/*\EB_Band &EB_Band::operator=(const unsigned char content)*/
EB_Band & EB_Band::operator = (const unsigned char content)
{
    unsigned int i = 0, k, blocklimit;

    if (values)
	{
	    blocklimit = (pixelcount / LURBLOCKSIZE) * LURBLOCKSIZE;
	    while (i < blocklimit)
		{
		    setValue (i, content);
		    i++;
		    setValue (i, content);
		    i++;
		    setValue (i, content);
		    i++;
		    setValue (i, content);
		    i++;
		    setValue (i, content);
		    i++;
		    setValue (i, content);
		    i++;
		    setValue (i, content);
		    i++;
		    setValue (i, content);
		    i++;
		}
	    for (k = i; k < pixelcount; k++)
		setValue (k, content);
	}
    else
	throw EBINoValuesInBandEXP ();
    return (*this);
}
/*\*/
EB_Band & EB_Band::changeDimensions(unsigned int w,unsigned int h)
{
	    width = w;
	    height = h;
	    pixelcount = w*h;
	    delete values;
	    values = NULL;
	    values = new float[pixelcount];
	    if (!(values))
			throw EBIOutOfMemoryEXP ();
    return (*this);
}
/*\EB_Band &EB_Band::copyWholeTo(EB_Band &other,int left,int top)*/
EB_Band & EB_Band::copyWholeTo (EB_Band & other, int left, int top)
{
    int /*xoff, yoff,*/ x, y, maxx = left + giveWidth (), maxy = top + giveHeight (),ownindex=0;
	
    if (values)
	{
//	    yoff = 0;
	    for (y = top; y < maxy; y++)
		{
//		    xoff = 0;
		    for (x = left; x < maxx; x++)
			{
			    other.setValue (x, y, values[ownindex]);
			    ownindex++;
			}
//		    yoff++;
		}
	}
    else
		throw EBINoValuesInBandEXP ();
    return (other);
}

/*\*/
/*\EB_Band &EB_Band::copyWholeFrom(EB_Band &other,int left,int top)*/
EB_Band & EB_Band::copyWholeFrom (EB_Band & other, int left, int top)
{
    int xoff, yoff, x, y,/* maxx = other.giveWidth (), maxy = other.giveHeight (), */ownwidth = giveWidth (), ownheight = giveHeight (), ownindex=0;

    if (values)
	{
	    yoff = -top;
	    for (y = 0; y < ownheight; y++)
		{
//		    if ((yoff >= 0) && (yoff < maxy))
			{
			    xoff = -left;
	    		for (x = 0; x < ownwidth; x++)
				{
//		    		if ((xoff >= 0) && (xoff < maxx))
						values[ownindex]= other.giveFloatValue (xoff, yoff);
				    xoff++;
					ownindex++;
				}
			}
		    yoff++;
		}
	}
    else
	throw EBINoValuesInBandEXP ();
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::rescaleIntensity(float newmin,float newmax)*/
EB_Band & EB_Band::rescaleIntensity (float newmin, float newmax)
{
    float oldmin = giveMinFloat (), oldmax = giveMaxFloat (), ggold = 1 / (oldmax - oldmin), gg = newmax - newmin;
    unsigned int i = 0, k, blocklimit;

    minfloat = newmin;
    maxfloat = newmax;
    blocklimit = (pixelcount / LURBLOCKSIZE) * LURBLOCKSIZE;
    while (i < blocklimit)
	{
	    values[i]=(values[i] - oldmin) * ggold * gg + newmin;
		++i;
	    values[i]=(values[i] - oldmin) * ggold * gg + newmin;
		++i;
	    values[i]=(values[i] - oldmin) * ggold * gg + newmin;
		++i;
	    values[i]=(values[i] - oldmin) * ggold * gg + newmin;
		++i;
	    values[i]=(values[i] - oldmin) * ggold * gg + newmin;
		++i;
	    values[i]=(values[i] - oldmin) * ggold * gg + newmin;
		++i;
	    values[i]=(values[i] - oldmin) * ggold * gg + newmin;
		++i;
	    values[i]=(values[i] - oldmin) * ggold * gg + newmin;
		++i;
	}
    for (k = i; k < pixelcount; k++)
		values[k]=(values[k] - oldmin) * ggold * gg + newmin;
    minfloat = newmin;
    maxfloat = newmax;
    minmaxinterval = maxfloat - minfloat;
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::lookUp(const EB_ColorDistHistogram &CDH)*/
EB_Band & EB_Band::lookUp (const EB_ColorDistHistogram &CDH)
{
    unsigned int i;
    EB_LookUpTable lut = CDH.lookuptable;
    EB_ImageAdvancement a;
    unsigned int tenpercent=pixelcount/10;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;

	
    for (i = 0; i < pixelcount; ++i)
	{
		if ((advancementcallback) && (!(i % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
	    if (bregion.isInside (i % width, i / width))
			setValue (i, (unsigned char) (lut.giveValue (giveCharValue (i))));
	}
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::aequalize(void)*/
EB_Band & EB_Band::aequalize (void)
{
	EB_ColorDistHistogram CDH(256);

    CDH.calculateHistogram (*this);
    CDH.aequalizeHistogram ();
    return (lookUp (CDH));
}

/*\*/
/*\EB_Band &EB_Band::gammaCorrect(float factor)*/
EB_Band & EB_Band::gammaCorrect (float factor)
{
	EB_ColorDistHistogram CDH(256);

    CDH.lookuptable.calculateGammas (1.0 / factor);
    return (lookUp (CDH));
}

/*\*/
/*\EB_Band &EB_Band::changeContrast(float factor)*/
EB_Band & EB_Band::changeContrast (float factor)
{
	EB_ColorDistHistogram CDH(256);

    CDH.lookuptable.calculateContrast (factor);
		return (lookUp (CDH));
}

/*\*/
EB_Band & EB_Band::transformWithLUT(const EB_IntensityTransformation &trans)
{
	EB_ColorDistHistogram CDH(256);

    CDH.lookuptable.calculateIntensityTransformation(trans);
		return (lookUp (CDH));
}
EB_Band & EB_Band::transform(const EB_IntensityTransformation &trans)
{
    float invfactor=1.0/minmaxinterval,min=giveMinFloat();
    unsigned int i;
    EB_ImageAdvancement a;
    unsigned int tenpercent=pixelcount/10;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;

    for(i=0;i<pixelcount;++i)
	{
		if ((advancementcallback) && (!(i % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
	    if (bregion.isInside (i % width, i / width))
	    	values[i]=trans.transform((values[i] - min)*invfactor)*minmaxinterval+min;
	}
	return(*this);
}
EB_Band &EB_Band::transform(const EB_ImageTransformation &trans)
{
	trans.setSourceDimensions(width,height);
	unsigned int i,w=trans.giveDestinationWidth(),h=trans.giveDestinationHeight(),ind;
    EB_ImageAdvancement a;
    unsigned int tenpercent;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
	float *tv=values;
	
	pixelcount=w*h;
	tenpercent = pixelcount / 10;
	values=new float[pixelcount];
    ind = 0;
    for (i = 0; i < pixelcount; ++i)
	{
		if ((advancementcallback) && (!(i % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
		values[ind]=tv[trans.transform(ind)];
		ind++;
	}
	width=w;
	height=h;
	delete (tv);
    return (*this);
}
EB_Band &EB_Band::transform(const EB_ImageTransformation &trans,EB_TransformationInterpolator &ip)
{
	trans.setSourceDimensions(width,height);
	unsigned int i,j,w=trans.giveDestinationWidth(),h=trans.giveDestinationHeight(),ind;
    EB_ImageAdvancement a;
    unsigned int tenpercent;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
	float *tv=values;
	EB_Vector<unsigned int> &indices=ip.giveIndices();
	EB_Vector<float> &factors=ip.giveFactors();
	unsigned int ipsc=ip.giveSampleCount();
	
	ip.setSourceDimensions(width,height);
	pixelcount=w*h;
	tenpercent = pixelcount / 10;
	values=new float[pixelcount];
    ind = 0;
    for (i = 0; i < pixelcount; ++i)
	{
		if ((advancementcallback) && (!(i % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
		trans.transform(ind);
		ip.calculate(trans.giveOrigX(),trans.giveOrigY());
		values[ind]=0.0;
		values[ind]=tv[indices[0]]*factors[0];
		for(j=1;j<ipsc;++j)
			values[ind]+=tv[indices[j]]*factors[j];
		ind++;
	}
	width=w;
	height=h;
	delete (tv);
    return (*this);
}
EB_Band &EB_Band::transformWithLUT(const EB_ImageTransformation &trans)
{
	trans.setSourceDimensions(width,height);
	unsigned int i,w=trans.giveDestinationWidth(),h=trans.giveDestinationHeight(),ind;
    EB_ImageAdvancement a;
    unsigned int tenpercent;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
	float *tv=values;

	pixelcount=w*h;
	tenpercent = pixelcount / 10;
	values=new float[pixelcount];
    ind = 0;
    for (i = 0; i < pixelcount; ++i)
	{
		if ((advancementcallback) && (!(i % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
		values[ind]=tv[trans.transformWithLUT(ind)];
		ind++;
	}
	width=w;
	height=h;
	delete (tv);
    return (*this);
}
EB_Band & EB_Band::convolute (const EB_Filter & filter)
{
    int fx = filter.giveWidth (), fy = filter.giveHeight (), fsx = fx / 2, fsy = fy / 2, x, y, bigx, bigy;
    float *oldv = values, akku;
    float *fcolumn = NULL;
    int index, findex;
    int modvalue = width - fx;
    EB_ImageAdvancement a;
    unsigned int tenpercent = height / 10;
	unsigned int rowstart;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
	int h=height,w=width;
	float min=giveMinFloat(),max=giveMaxFloat();

    values = new float[pixelcount];
    fcolumn = new float[filter.giveHeight ()];
    if (!(values))
	{
	    values = oldv;
	    throw EBIOutOfMemoryEXP ();
	}
	rowstart=0;
	if(bregion.isRegion())
	{
		memcpy(values,oldv,pixelcount*sizeof(float));
    	for (bigy = 0; bigy < h; bigy++)
		{
	    	if ((advancementcallback) && (!(bigy % tenpercent)))
			{
		    	(*advancementcallback) (a);
				a.a_percentage += 10;
			}
	    	for (bigx = 0; bigx < w; bigx++)
			{
		    	if (bregion.isInside (bigx, bigy))
				{
			    	akku = 0.0;
			    	findex = 0;
			    	index = bigx + (bigy - fsy) * width - fsx;
			    	for (y = -fsy; y <= fsy; y++)
					{
				    	if ((y + bigy >= 0) && (y + bigy < h))
						{
	//                                xfsx=x+fsx;
	//                               bigxx=x+bigx;
					    	for (x = -fsx; x <= fsx; x++)
							{
						    	if ((x + bigx >= 0) && (x + bigx < w))
								{
							    	akku += filter[findex] * oldv[index];
								}
						    	findex++;
						    	index++;
							}
					    	index += modvalue;
						}
				    	else
						{
					    	findex += fx;
					    	index += width;
						}
					}
			    	operator[](bigx+rowstart)=akku;
				}
//			    else
//					operator[](bigx+rowstart)=oldv[bigy * width + bigx];
			}
			rowstart+=w;
		}
	}
	else
	{
    	for (bigy = 0; bigy < fsy; bigy++)
		{
	    	if ((advancementcallback) && (!(bigy % tenpercent)))
			{
		    	(*advancementcallback) (a);
				a.a_percentage += 10;
			}
	    	for (bigx = 0; bigx < w; bigx++)
			{
			    akku = 0.0;
			    findex = 0;
			    index = bigx + (bigy) * width - fsx;
//cout<<fsx<<" fsx,fsy "<<fsy<<" index "<<index<<endl;
			    for (y = -fsy; y <= fsy; y++)
				{
				    if ((y + bigy >= 0) && (y + bigy < h))
					{
					    for (x = -fsx; x <= fsx; x++)
						{
					    	if ((x + bigx >= 0) && (x + bigx < w))
						    	akku += filter[findex] * oldv[index];
//cout<<x<<" x,y "<<y<<endl;
						    findex++;
						    index++;
						}
					    index += modvalue;
					}
				}
			    operator[](bigx+rowstart)=akku;
			}
			rowstart+=w;
		}
    	for (bigy = fsy; bigy < h-fsy; bigy++)
		{
	    	for (bigx = 0; bigx < fsx; bigx++)
			{
			    akku = 0.0;
			    findex = 0;
			    index = bigx + (bigy - fsy) * w;
			    for (y = -fsy; y <= fsy; y++)
				{
				    for (x = -fsx; x <= fsx; x++)
					{
					    if ((x + bigx >= 0) && (x + bigx < w))
						    akku += filter[findex] * oldv[index];
					    findex++;
					    index++;
					}
				    index += modvalue;
				}
			    operator[](bigx+rowstart)=akku;
			}
	    	for (bigx = w-fsx; bigx < w; bigx++)
			{
			    akku = 0.0;
			    findex = 0;
			    index = bigx + (bigy - fsy) * w - fsx;
			    for (y = -fsy; y <= fsy; y++)
				{
				    for (x = -fsx; x <= fsx; x++)
					{
					    if ((x + bigx >= 0) && (x + bigx < w))
						    akku += filter[findex] * oldv[index];
					    findex++;
					    index++;
					}
				    index += modvalue;
				}
			    operator[](bigx+rowstart)=akku;
			}
			rowstart+=w;
		}	
rowstart=fsy*w;
    	for (bigy = fsy; bigy < h-fsy; bigy++)
		{
	    	if ((advancementcallback) && (!(bigy % tenpercent)))
			{
		    	(*advancementcallback) (a);
				a.a_percentage += 10;
			}
	    	for (bigx = fsx; bigx < w-fsx; bigx++)
			{
			    akku = 0.0;
			    findex = 0;
			    index = bigx + (bigy - fsy) * w - fsx;
			    for (y = -fsy; y <= fsy; y++)
				{
//				    if ((y + bigy >= 0) && (y + bigy < height))
					{
//                                xfsx=x+fsx;
//                               bigxx=x+bigx;
					    for (x = -fsx; x <= fsx; x++)
						{
//						    if ((x + bigx >= 0) && (x + bigx < width))
							{
							    akku += filter[findex] * oldv[index];
							}
						    findex++;
						    index++;
						}
					    index += modvalue;
					}
/*				    else
					{
					    findex += fx;
					    index += width;
					}
*/				}
			    operator[](bigx+rowstart)=akku;
//				if(akku>0.2)
//				cout<<akku<<endl;
			}
			rowstart+=w;
		}
    	for (bigy = h-fsy; bigy < h; bigy++)
		{
	    	if ((advancementcallback) && (!(bigy % tenpercent)))
			{
		    	(*advancementcallback) (a);
				a.a_percentage += 10;
			}
	    	for (bigx = 0; bigx < w; bigx++)
			{
			    akku = 0.0;
			    findex = 0;
			    index = bigx + (bigy - fsy) * w - fsx;
			    for (y = -fsy; y <= fsy; y++)
				{
				    if ((y + bigy >= 0) && (y + bigy < h))
					{
					    for (x = -fsx; x <= fsx; x++)
						{
					    	if ((x + bigx >= 0) && (x + bigx < w))
						    	akku += filter[findex] * oldv[index];
						    findex++;
						    index++;
						}
					    index += modvalue;
					}
				}
			    operator[](bigx+rowstart)=akku;
			}
			rowstart+=w;
		}
	}
	delete(oldv);
	min=filter.giveNegSum()*maxfloat+filter.givePosSum()*minfloat;
	max=filter.givePosSum()*maxfloat+filter.giveNegSum()*minfloat;
//	cout<<minfloat<<" "<<min<<" "<<maxfloat<<" "<<max<<" "<<filter.giveNegSum()<<" "<<filter.givePosSum()<<endl;
	minfloat=min;
	maxfloat=max;
	minmaxinterval=maxfloat-minfloat;
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::convolute(const EB_Filter &filter,EB_Band &alphachannel,int left,int top,float factor)*/
EB_Band & EB_Band::convolute (const EB_Filter & filter, EB_Band & alphachannel, int left, int top, float factor)throw(EBIOutOfMemoryEXP)
{
    int fx = filter.giveWidth (), fy = filter.giveHeight (), fsx = fx / 2, fsy = fy / 2, x, y, bigx, bigy, width = giveWidth (), height = giveHeight ();
    int awidth = alphachannel.giveWidth () + left, aheight = alphachannel.giveHeight () + top;
    float *oldv = values, akku;
    float threshold = 0.0;
    int findex, index, modvalue = width - fx;
    EB_ImageAdvancement a;
    unsigned int tenpercent = width / 10;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
	float min,max;

    values = new float[width * height];
    if (!(values))
	{
	    values = oldv;
	    throw EBIOutOfMemoryEXP ();
	}
    if (factor > 1.0)
	factor = 1.0;
    if (factor < -1.0)
	factor = -1.0;
    if (factor >= 0.0)
	threshold = (alphachannel.giveMaxFloat () - alphachannel.giveMinFloat ()) * factor + alphachannel.giveMinFloat ();
    else
	{
	    threshold = alphachannel.giveMinFloat () - 1.0;
	    alphachannel.rescaleIntensity (0.0, 1.0);
	}
#ifdef UNIX
    memcpy (values, oldv, width * height * sizeof (float));
#else
    operator = (oldv);
#endif
    for (bigx = 0; bigx < width; bigx++)
	{
	    if ((advancementcallback) && (!(bigx % tenpercent)))
		{
		    (*advancementcallback) (a);
			a.a_percentage += 10;
		}
	    if ((bigx > left) && (bigx < awidth))
		for (bigy = 0; bigy < height; bigy++)
		    {
		    if (bregion.isInside (bigx, bigy))
			{
			if ((bigy > top) && (bigy < aheight))
			    if ((alphachannel.giveFloatValue (bigx - left, bigy - top) > threshold) || (factor < 0.0))
				{
				    akku = 0.0;
				    findex = 0;
				    index = bigx + (bigy - fsy) * width - fsx;
				    for (y = -fsy; y <= fsy; y++)
					{
					    if ((y + bigy >= 0) && (y + bigy < height))
						{
//                           xfsx=x+fsx;
//                          bigxx=x+bigx;
						    for (x = -fsx; x <= fsx; x++)
							{
							    if ((x + bigx >= 0) && (x + bigx < width))
								{
								    akku += filter.giveFloatValue (findex) * oldv[index];
								}
							    findex++;
							    index++;
							}
						    index += modvalue;
						}
					    else
						{
						    findex += fx;
						    index += width;
						}
					}
				    if (factor < 0.0)
					akku = akku * alphachannel.giveFloatValue (bigx - left, bigy - top) + oldv[bigx + bigy * width] * (1 - alphachannel.giveFloatValue (bigx - left, bigy - top));
				    setValue (bigx, bigy, akku);
				}
			}
		    else
			setValue (bigx, bigy, oldv[bigy * width + bigx]);
				
		    }
	}
	delete(oldv);
	min=filter.giveNegSum()*maxfloat+filter.givePosSum()*minfloat;
	max=filter.givePosSum()*maxfloat+filter.giveNegSum()*minfloat;
	minfloat=min;
	maxfloat=max;
	minmaxinterval=maxfloat-minfloat;
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::rotate(float angle,int rotcenterx,int rotcentery,float fillintensity)*/
EB_Band & EB_Band::rotate (float angle,int rotcenterx,int rotcentery,float fillintensity)
{
    int width = giveWidth (), height = giveHeight ();
    float destcenterx = (float) rotcenterx;
    float destcentery = (float) rotcentery;
    float xshift_fx = cos (angle / 180. * M_PI);
    float yshift_fx = sin (angle / 180. * M_PI);
    float xshift_fy = cos (angle / 180. * M_PI);
    float yshift_fy = sin (angle / 180. * M_PI);
    float curx = (float) width / 2.0 - destcenterx * xshift_fx + destcentery * yshift_fy-(width/2-rotcenterx);
    float cury = (float) height / 2.0 - destcenterx * yshift_fx - destcentery * xshift_fy-(height/2-rotcentery);
    float tmpx;
    float tmpy;
    float intx, inty, mintx, minty;
    int j, i, u, v;
    EB_Band band (width, height, fillintensity,giveMinFloat(),giveMaxFloat());
    EB_ImageAdvancement a;
    unsigned int tenpercent = height / 10;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;

    for (j = 0; j < height; j++)
	{
	    if ((advancementcallback) && (!(j % tenpercent)))
		{
		    (*advancementcallback) (a);
		    a.a_percentage += 10;
		}
	    tmpx = curx;
	    curx -= yshift_fy;
	    tmpy = cury;
	    cury += xshift_fy;
	    for (i = 0; i < width; i++)
		{
		    tmpx += xshift_fx;
		    u = (int) tmpx;
		    tmpy += yshift_fx;
		    if ((u>0)&&(u < width))
			{
			    v = (int) tmpy;
			    if ((v>0)&&(v < height))
				{
				    intx = tmpx - (float) u;
				    mintx = 1.0 - intx;
				    inty = tmpy - (float) v;
				    minty = 1.0 - inty;
				    band.setValue (i, j, minty * (mintx * giveFloatValue (u - 1, v - 1) + intx * giveFloatValue (u, v - 1)) + inty * (mintx * giveFloatValue (u - 1, v) + intx * giveFloatValue (u, v)));
				}
/*			    else
				{
				    band.setValue (i, j, fillcolor);
				}
*/			}
/*		    else
			{
			    band.setValue (i, j, fillcolor);
			}
*/		}
	}
    operator = (band);
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::rotateToFit(float angle,float fillintensity)*/
EB_Band &EB_Band::rotateToFit(float angle,float fillintensity)
{
	float w=width,h=height;
	float edgeangle,r,newheight,newwidth;
	r=sqrt(width*width+height*height);
	if(angle<0.0)
		angle+=360.0;
	if((angle<0.0)||(angle>360.0))
		throw EBICorruptedParameterEXP();
	if(angle<=90.0)
	{
		edgeangle=atan2(h,w)+(angle / 180. * M_PI);
		newheight=sin(edgeangle)*r;
		edgeangle=atan2(-h,w)+(angle / 180. * M_PI);
		newwidth=cos(edgeangle)*r;
	}
	else if(angle<=180.0)
	{
		edgeangle=atan2(-h,w)+(angle / 180. * M_PI);
		newheight=sin(edgeangle)*r;
		edgeangle=atan2(h,w)+(angle / 180. * M_PI);
		newwidth=fabs(cos(edgeangle)*r);
    }
	else if(angle<=270.0)
	{
		edgeangle=atan2(h,w)+(angle / 180. * M_PI);
		newheight=fabs(sin(edgeangle)*r);
		edgeangle=atan2(-h,w)+(angle / 180. * M_PI);
		newwidth=fabs(cos(edgeangle)*r);
    }
	else
	{
		edgeangle=atan2(-h,w)+(angle / 180. * M_PI);
		newheight=fabs(sin(edgeangle)*r);
		edgeangle=atan2(h,w)+(angle / 180. * M_PI);
		newwidth=fabs(cos(edgeangle)*r);
    }
	float destcenterx = (float) newwidth / 2.0;
    float destcentery = (float) newheight / 2.0;
    float xshift_fx = cos (angle / 180. * M_PI);
    float yshift_fx = sin (angle / 180. * M_PI);
    float xshift_fy = cos (angle / 180. * M_PI);
    float yshift_fy = sin (angle / 180. * M_PI);
    float curx = (float) width / 2.0 - destcenterx * xshift_fx + destcentery * yshift_fy;
    float cury = (float) height / 2.0 - destcenterx * yshift_fx - destcentery * xshift_fy;
    float tmpx;
    float tmpy;
    float intx, inty, mintx, minty;
    int j, i, u, v,signedwidth=width,signedheight=height;
    EB_Band band ((int)newwidth, (int)newheight, fillintensity,giveMinFloat(),giveMaxFloat());
    EB_ImageAdvancement a;
    unsigned int tenpercent = height / 10;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
	
    for (j = 0; j < newheight; j++)
	{
	    if ((advancementcallback) && (!(j % tenpercent)))
		{
		    (*advancementcallback) (a);
		    a.a_percentage += 10;
		}
	    tmpx = curx;
	    curx -= yshift_fy;
	    tmpy = cury;
	    cury += xshift_fy;
	    for (i = 0; i < newwidth; i++)
		{
		    tmpx += xshift_fx;
		    u = (int) tmpx;
		    tmpy += yshift_fx;
		    if ((u>0)&&(u < signedwidth))
			{
			    v = (int) tmpy;
			    if ((v>0)&&(v < signedheight))
				{
				    intx = tmpx - (float) u;
				    mintx = 1.0 - intx;
				    inty = tmpy - (float) v;
				    minty = 1.0 - inty;
				    band.setValue (i, j, minty * (mintx * giveFloatValue (u - 1, v - 1) + intx * giveFloatValue (u, v - 1)) + inty * (mintx * giveFloatValue (u - 1, v) + intx * giveFloatValue (u, v)));
				}
/*			    else
				{
				    band.setValue (i, j, 0.0F);
				}
*/			}
/*		    else
			{
			    band.setValue (i, j, 0.0F);
			}
*/		}
	}
std::cout<<fillintensity<<std::endl;
	operator=(band);
	return(*this);
}
/*\*/
/*\EB_Band &EB_Band::scale(unsigned int newwidth,unsigned int newheight)*/
EB_Band & EB_Band::scale (unsigned int newwidth, unsigned int newheight)
{
    int width = giveWidth (), height = giveHeight ();
    int u, v, m, l, ind;
    int s = width / newwidth;
    int t = height / newheight;
    if (s < 1)
	s = 1;
    if (t < 1)
	t = 1;
    float quot = 1.0 / (float) (s * t);
    float xshift_fx = (float) width / (float) newwidth;
    float yshift_fx = 0.0;
    float xshift_fy = (float) height / (float) newheight;
    float yshift_fy = 0.0;
    float curx = (float) width / 2.0 - (float) newwidth / 2.0 * xshift_fx + (float) newheight / 2.0 * yshift_fy;
    float cury = (float) height / 2.0 - (float) newwidth / 2.0 * yshift_fx - (float) newheight / 2.0 * xshift_fy;
    float tmpx;
    float tmpy;
    float intx, inty, mintx, minty;
    EB_Band band (newwidth, newheight, giveMinFloat ());
    EB_ImageAdvancement a;
    unsigned int tenpercent = newheight / 10;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;

    ind = 0;
    for (unsigned int j = 0; j < newheight; j++)
	{
	    if ((advancementcallback) && (!(j % tenpercent)))
		{
		    (*advancementcallback) (a);
		    a.a_percentage += 10;
		}
	    tmpx = curx;
	    curx -= yshift_fy;
	    tmpy = cury;
	    cury += xshift_fy;
	    for (unsigned int i = 0; i < newwidth; i++)
		{
		    tmpx += xshift_fx;
		    u = (int) tmpx;
		    intx = tmpx - (int) tmpx;
		    mintx = 1.0 - intx;
		    tmpy += yshift_fx;
		    if ((u >= s) && (u < width))
			{
			    v = (int) tmpy;
			    inty = tmpy - (int) tmpy;
			    minty = 1.0 - inty;
			    if ((v >= t) && (v < height))
				{
				    float summe = 0.0;
				    if (t == 1)
					{
					    if (s == 1)
						{
						    band.setValue (ind, minty * (mintx * giveFloatValue (u - 1, v - 1) + intx * giveFloatValue (u, v - 1)) + inty * (mintx * giveFloatValue (u - 1, v) + intx * giveFloatValue (u, v)));
						}
					    else
						{
						    for (l = 1; l <= s; l++)
							summe += minty * giveFloatValue (u - l, v - 1) + inty * giveFloatValue (u - l, v);
						    band.setValue (ind, summe * quot);
						}
					}
				    else
					{
					    if (s == 1)
						{
						    for (m = 1; m <= t; m++)
							summe += minty * giveFloatValue (u - 1, v - m) + inty * giveFloatValue (u, v - m);
						    band.setValue (ind, summe * quot);
						}
					    else
						{
						    for (l = 1; l <= s; l++)
							for (m = 1; m <= t; m++)
							    summe += giveFloatValue (u - l, v - m);
						    band.setValue (ind, summe * quot);
						}
					}
				}
			    else
				band.setValue (ind, 0.0F);
			}
		    else
			band.setValue (ind, 0.0F);
		    ind++;
		}
	}
    operator = (band);
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::overlay(EB_Band &other,float otherfac, float ownfac,int left,int top)*/
EB_Band & EB_Band::overlay (EB_Band & other, float otherfac, float ownfac, int left, int top)
{
    float uf = giveMaxFloat (), lf = giveMinFloat (), ownf = ownfac, otherf = otherfac;
    int ow = other.giveWidth (), oh = other.giveHeight (), xoff, yoff,index=0;
	unsigned int x, y;
    EB_ImageAdvancement a;
    unsigned int tenpercent = height / 10;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;

    otherfac = fabs (otherfac);
    ownfac = fabs (ownfac);

    otherf /= otherfac + ownfac;
    ownf /= otherfac + ownfac;
    if (other.giveDataPointer ())
	{
	    yoff = -top;
	    rescaleIntensity (other.giveMinFloat (), other.giveMaxFloat ());
	    for (y = 0; y < height; y++)
		{
			if ((advancementcallback) && (!(y % tenpercent)))
			{
				(*advancementcallback) (a);
				a.a_percentage += 10;
			}
		    if ((yoff >= 0) && (yoff < oh))
			{
			    xoff = -left;
			    for (x = 0; x < width; x++)
				{
				    if (((xoff >= 0) && (xoff < ow))&&(bregion.isInside(x,y)))
//                            if((x<ow)&&(y<oh))
					{
					    values[index]= ownf * values[index] + otherf * other.giveFloatValue (xoff, yoff);
					}
				    xoff++;
					index++;
				}
			}
			else
				index+=width;
		    yoff++;
		}
	    rescaleIntensity (lf, uf);
	}
    else
	throw EBINoValuesInBandEXP ();
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::overlay(EB_Band &other,EB_Band &alphachannel,int oleft,int otop,int aleft,int atop,float factor)*/
EB_Band & EB_Band::overlay (EB_Band & other, EB_Band & alphachannel, int oleft, int otop, int aleft, int atop, float factor)
{
    float uf = giveMaxFloat (), lf = giveMinFloat ();
    int index=0,aw = alphachannel.giveWidth (), ah = alphachannel.giveHeight ();
	int ow = other.giveWidth (), oh = other.giveHeight (), xaoff, yaoff, xooff, yooff;
    EB_ImageAdvancement a;
    unsigned int tenpercent = height / 10;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;

    if (factor > 1.0)
	factor = 1.0;
    if (factor < 0.0)
	factor = 0.0;
    if ((other.giveDataPointer ()) && (alphachannel.giveDataPointer ()))
	{
	    rescaleIntensity (other.giveMinFloat (), other.giveMaxFloat ());
	    alphachannel.rescaleIntensity (0.0, 1.0);
	    yaoff = -atop;
	    yooff = -otop;
	    for (unsigned int y = 0; y < height; y++)
		{
			if ((advancementcallback) && (!(y % tenpercent)))
			{
				(*advancementcallback) (a);
				a.a_percentage += 10;
			}
		    if (((yooff >= 0) && (yooff < oh)) && ((yaoff >= 0) && (yaoff < ah)))
			{
			    xaoff = -aleft;
			    xooff = -oleft;
			    for (unsigned int x = 0; x < width; x++)
				{
				    if ((((xooff >= 0) && (xooff < ow)) && ((xaoff >= 0) && (xaoff < aw)))&&(bregion.isInside(x,y)))
					values[index]= (1.0F - factor * alphachannel.giveFloatValue (xaoff, yaoff)) * values[index] + factor * alphachannel.giveFloatValue (xaoff, yaoff) * other.giveFloatValue (xooff, yooff);
				    xaoff++;
				    xooff++;
					index++;
				}
			}
			else
				index+=width;
		    yaoff++;
		    yooff++;
		}
	    rescaleIntensity (lf, uf);
	}
    else
	throw EBINoValuesInBandEXP ();
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::doubleMirror(void)*/
EB_Band & EB_Band::doubleMirror (void)
{
    unsigned int i,invi,half = pixelcount / 2;
    float tpixel;
    EB_ImageAdvancement a;
    unsigned int tenpercent;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;

    if (half != (pixelcount + 1) / 2)
	half++;
	tenpercent = half / 10;
	invi=pixelcount-1;
    for (i = 0; i < half; i++)
	{
		if ((advancementcallback) && (!(i % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
	    tpixel = values[i];
	    values[i]= values[invi];
	    values[invi]=tpixel;
		--invi;
	}
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::verticalMirror(void)*/
EB_Band & EB_Band::verticalMirror (void)
{
    unsigned int i, merki,half = pixelcount / 2, partnerindex = pixelcount - width;
    float tpixel;
    EB_ImageAdvancement a;
    unsigned int tenpercent = half / 10;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;

//    if(half!=(pixelcount+1)/2)
//      half++;
    merki = 0;
    for (i = merki; i < half; i++, merki++)
	{
		if ((advancementcallback) && (!(i % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
	    if (merki == width)
		{
		    merki = 0;
		    partnerindex -= 2 * width;
		}
	    tpixel = values[i];
	    values[i]=values[partnerindex];
	    values[partnerindex]= tpixel;
		++partnerindex;
	}
    return (*this);
}

/*\*/
/*\EB_Band &EB_Band::horizontalMirror(void)*/
EB_Band & EB_Band::horizontalMirror (void)
{
    int i, merki,partnerindex = width - 1;
    float tpixel;
    EB_ImageAdvancement a;
    unsigned int tenpercent = pixelcount / 20;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
	int p=pixelcount;
	
    merki = 0;
    for (i = merki; i < p; i++)
	{
		if ((advancementcallback) && (!(i % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
	    tpixel = values[i];
	    values[i]= values[partnerindex+i];
	    values[partnerindex+i]=tpixel;
	    partnerindex -= 2;
	    if (partnerindex == 0)
		{
		    partnerindex = width - 1;
		    i = merki + width;
		    merki = i;
		}
	    else if (partnerindex < 0)
		{
		    partnerindex = width - 1;
		    i = merki + width;
		    merki = i;
		}
	}
    return (*this);
}

/*\*/
EB_Band & EB_Band::setRegion (EB_ImageRegion & region)
{
    bregion = region;
    return (*this);
};

EB_Band & EB_Band::unsetRegion (void)
{
    bregion.undefine ();
    return (*this);
};
EB_Band &EB_Band::smoothBox(unsigned int smoothwidth)
{
	unsigned int x,y,smooth_width_half,smooth_width_sqr,rowstart,colstart,index,index1,index2,leftend,botend;
	float smooth_sum,quotient;
    float *oldv = values,*t=NULL;
	EB_ImageAdvancement a;
    unsigned int tenpercent = height / 5;

    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
    values = new float[pixelcount];
    if (!(values))
	{
	    values = oldv;
	    throw EBIOutOfMemoryEXP ();
	}
//	cout<<smoothwidth<<" "<<smoothwidth/2<<" "<<(smoothwidth+1)/2<<" ";
	if(smoothwidth/2==(smoothwidth+1)/2)
		smoothwidth++;
//	cout<<smoothwidth<<" ";
	smooth_width_half=smoothwidth/2;
//	cout<<smooth_width_half<<endl;
	smooth_width_sqr=smoothwidth*smoothwidth;
	quotient=1.0/((float)smooth_width_sqr);
//	quotient=1.0/((float)smoothwidth+0.0F);
	leftend=width-smooth_width_half;
	botend=height-smooth_width_half;
	rowstart=0;
//	cout<<"height: "<<height<<endl;
	for (y=0; y<height; y++)
	{
//	cout<<y<<" "<<height<<endl;
		if ((advancementcallback) && (!(y % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
		smooth_sum=0.0;
		index1=rowstart;
		for (x=0; x<=smooth_width_half; x++)
		{
			smooth_sum+=oldv[index1++];
		}
		index1=rowstart;
		index2=rowstart+smooth_width_half+1;
		for (x=0; x<smooth_width_half; x++)
		{
			setValue(index1,smooth_sum*quotient);
			smooth_sum+=oldv[index2];
			index1++;
			index2++;
		}
		index1=rowstart; //\E4ltester Index
		index=smooth_width_half+index1;
		//i2=smooth_width; //neuer Index
		for (x=smooth_width_half; x<leftend; x++)
		{
			setValue(index,smooth_sum*quotient);
			smooth_sum+=oldv[index2]-oldv[index1];
			index++;
			index1++;
			index2++;
		}
		index=rowstart+leftend;
		for (x=leftend; x<width; x++)
		{
			setValue(index,smooth_sum*quotient);
			smooth_sum-=oldv[index1];
			index++;
			index1++;
		}
		rowstart+=width;
	}
	t=oldv;
	oldv=values;
	values=t;
//	quotient=1.0/(float)smoothwidth;
	colstart=0;
    tenpercent = width / 10;
	for (x=0; x<width; x++)
	{
		if ((advancementcallback) && (!(x % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
		smooth_sum=0.0;
		index1=colstart;
		for (y=0; y<=smooth_width_half; y++)
		{
			smooth_sum+=oldv[index1];
			index1+=width;
		}
		index1=colstart;
		index2=colstart+(smooth_width_half+1)*width;
		for (y=0; y<smooth_width_half; y++)
		{
			setValue(index1,smooth_sum);//*quotient);
			smooth_sum+=oldv[index2];
			index1+=width;
			index2+=width;
		}
		index1=colstart;
		index=width*(smooth_width_half)+index1;
		//i2=smooth_width; //neuer Index
		for (y=smooth_width_half; y<botend; y++)
		{
			setValue(index,smooth_sum);//*quotient);
			smooth_sum+=oldv[index2]-oldv[index1];
			index+=width;
			index1+=width;
			index2+=width;
		}
		index=rowstart+botend;
		for (y=botend; y<height; y++)
		{
			setValue(index,smooth_sum);//*quotient);
			smooth_sum-=oldv[index1];
			index+=width;
			index1+=width;
		}
		colstart++;
	}
	delete(oldv);
	return(*this);
}
EB_Band &EB_Band::smoothBinom(unsigned int smoothwidth)
{
	EB_Filter row(smoothwidth,1);
	EB_Filter col(1,smoothwidth);
	unsigned int i,j;
	float denominator=1.0/pow(2.0, (float)(smoothwidth-1)),coeff;
	
    for (i = 0; i < smoothwidth; ++i)
		row[i]=0.0;	
	row[0]=1.0;
    for (i = 0; i < smoothwidth; ++i)
	{
    	for (j = i; j >0; --j)
		{
			coeff=row[j]+row[j-1];
			row[j]=coeff;
		}
	}
    for (i = 0; i < smoothwidth; ++i)
	{
		row[i]=row[i]*denominator;
		col[i]=row[i];
	}
	convolute(row);
	convolute(col);
	return(*this);
}
EB_Band &EB_Band::morphologicOperation(unsigned int kernelwidth,float gamma)
{
	unsigned int i,x,y,smooth_width_half,smooth_width_sqr,rowstart,colstart,index,index1,index2,leftend,botend;
	float sum;
    float *oldv = values;
	EB_ImageAdvancement a;
    unsigned int tenpercent = height / 5;
	float sums[pixelcount];
	float sums2[pixelcount];

	for(i=0;i<pixelcount;++i)
		sums[i]=0.0;
	if(gamma<0.0)
		gamma=0.0;
	if(gamma>1.0)
		gamma=1.0;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
    values = new float[pixelcount];
    if (!(values))
	{
	    values = oldv;
	    throw EBIOutOfMemoryEXP ();
	}
	if(kernelwidth/2==(kernelwidth+1)/2)
		kernelwidth++;
	smooth_width_half=kernelwidth/2;
	smooth_width_sqr=kernelwidth*kernelwidth;
//	quotient=1.0/((float)smooth_width_sqr);
//	quotient=1.0/(float)width;
	leftend=width-smooth_width_half;
	botend=height-smooth_width_half;
	rowstart=0;
//	cout<<"height: "<<height<<endl;
	for (y=0; y<height; y++)
	{
//	cout<<y<<" "<<height<<endl;
		if ((advancementcallback) && (!(y % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
		sum=0.0;
		index1=rowstart;
		for (x=0; x<=smooth_width_half; x++)
		{
			sum+=oldv[index1++];
		}
		index1=rowstart;
		index2=rowstart+smooth_width_half+1;
		for (x=0; x<=smooth_width_half; x++)
		{
			sums[index1++]=sum;
			sum+=oldv[index2++];
		}
		index1=rowstart; //\E4ltester Index
		index=smooth_width_half+1+index1;
		//i2=smooth_width; //neuer Index
		for (x=smooth_width_half+1; x<leftend; x++)
		{
			sums[index++]=sum;
			sum+=oldv[index2++]-oldv[index1++];
		}
		index=rowstart+leftend;
		for (x=leftend; x<width; x++)
		{
			sums[index++]=sum;
			sum-=oldv[index1++];
		}
		rowstart+=width;
	}
//	t=oldv;
//	oldv=values;
//	values=t;
//	quotient=1.0/(float)width;
	colstart=0;
    tenpercent = width / 5;
	for (x=0; x<width; x++)
	{
		if ((advancementcallback) && (!(x % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
		sum=0.0;
		index1=colstart;
		for (y=0; y<=smooth_width_half; y++)
		{
			sum+=sums[index1];
			index1+=width;
		}
		index1=colstart;
		index2=colstart+(smooth_width_half+1)*width;
		for (y=0; y<=smooth_width_half; y++)
		{
			sums2[index1]=sum;
			sum+=sums[index2];
			index1+=width;
			index2+=width;
		}
		index1=colstart;
		index=width*(smooth_width_half+1)+index1;
		//i2=smooth_width; //neuer Index
		for (y=smooth_width_half+1; y<botend; y++)
		{
			sums2[index]=sum;
			sum+=sums[index2]-sums[index1];
			index+=width;
			index1+=width;
			index2+=width;
		}
		index=rowstart+botend;
		for (y=botend; y<height; y++)
		{
			sums2[index]=sum;
			sum-=sums[index1];
			index+=width;
			index1+=width;
		}
		colstart++;
	}
	for(i=0;i<pixelcount;++i)
	{
		if(sums2[i]>kernelwidth*kernelwidth*giveMaxFloat()*gamma)
			values[i]=giveMaxFloat();
		else
			values[i]=giveMinFloat();	
	}
/*	for(i=0;i<pixelcount;++i)
	{
		if(sums2[i]<kernelwidth*kernelwidth*giveMaxFloat())
			values[i]=giveMinFloat();
		else
			values[i]=giveMaxFloat();	
	}
*/	delete(oldv);
	return(*this);
}
EB_Band &EB_Band::doMedianFilter(unsigned int fux,unsigned int fuy)
{
    int fx=fux,fy=fuy,fsx = fx / 2, fsy = fy / 2, x, y, bigx, bigy,w=width,h=height,findex,pindex;
    float *oldv = values;
    float fcolumn[fux*fuy];
    EB_ImageAdvancement a;
    unsigned int tenpercent = height / 10;
	unsigned int index=0;

    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
    values = new float[pixelcount];
    if (!(values))
	{
	    values = oldv;
	    throw EBIOutOfMemoryEXP ();
	}
	for (bigy = 0; bigy < h; bigy++)
	{
	    if ((advancementcallback) && (!(bigy % tenpercent)))
		{
		    (*advancementcallback) (a);
			a.a_percentage += 10;
		}
	    for (bigx = 0; bigx < w; bigx++)
		{
		    if (bregion.isInside (bigx, bigy))
			{
				findex=0;
				pindex=index-fsy*width-fsx;
			    for (y = -fsy; y <= fsy; y++)
				{
				    if ((y + bigy >= 0) && (y + bigy < h))
					{
					    for (x = -fsx; x <= fsx; x++)
						{
						    if ((x + bigx >= 0) && (x + bigx < w))
							{
							    fcolumn[findex]=oldv[pindex];
								findex++;
							}
							pindex++;
						}
						pindex+=(width-fx);
					}
					else
						pindex+=width;
				}
				select(fcolumn,findex,(findex+1)/2);
					values[index]=fcolumn[(findex+1)/2];
			}
		    else
				values[index]=oldv[index];				
			index++;
		}
	}
	delete(oldv);
    return (*this);
}
void EB_Band::select(float *a,int N, int k)
{
	int i,j,l,r;
	float v;
	
	l=1;
	r=N-1;
	while(r>l)
	{
		v=a[r];
		i=l-1;
		j=r;
		for(;;)
		{
			while(a[++i]<v);
			while(a[--j]>v);
			if(i>=j)
				break;
			swap(a,i,j);
		}
		swap(a,i,r);
		if(i>=k)
			r=i-1;
		if(i<=k)
			l=i+1;
	}
}
void EB_Band::calculateAverages(EB_Vector<float> &v,unsigned int startindex)
{
	unsigned int x,y,fieldx,fieldy=0,fieldxcounter=0,fieldycounter=0;
	unsigned int fieldwidth=(unsigned int)((double)width/6.0),fieldheight=(unsigned int)((double)height/8.0);
	float divisor=1.0/(((float)width/6.0)*((float)height/8.0)*minmaxinterval);
	unsigned int ind=0;
	
//	cout<<fieldheight<<" "<<fieldwidth<<endl;
	
	for(y=0;y<height;++y)
	{
		fieldx=0;
		for(x=0;x<width;++x)
		{
			
			v[startindex+fieldy*6+fieldx]+=values[ind]*divisor;
			fieldxcounter++;
			if(fieldxcounter>fieldwidth)
			{
				fieldx++;
				fieldxcounter=0;
			}
			ind++;
		}
		fieldycounter++;
		if(fieldycounter>fieldheight)
		{
			fieldy++;
			fieldycounter=0;
		}
	}
//	cout<<v[0]<<endl;
}
float EB_Band::calculateSinusFilterResponse(float frequency)
{
	unsigned int x,y;
	float midx=(float)width/2.0F,midy=(float)height/2.0F;
	unsigned int ind=0;
	float sum=0.0,xarg,yarg,fcy,fcx;
	
//	cout<<fieldheight<<" "<<fieldwidth<<endl;
	
	for(y=0;y<height;++y)
	{
		yarg=((float)y-midy)/midy*M_PI*frequency;
		fcy=sin(yarg);
		for(x=0;x<width;++x)
		{
			xarg=((float)x-midx)/midx*M_PI*frequency;
			fcx=sin(xarg);
			sum+=values[ind]*fcx*fcy;
			ind++;
		}
	}
	return(sum);
}
EB_Band EB_Band::doFire(void)
{
	unsigned int i,j,randoms;
    EB_ImageAdvancement a;
    unsigned int tenpercent;
    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
	double temp;
//	EB_IntensitySigmoidTrans siggy;
	
/*	rescaleIntensity(0.0,255.0);
	siggy.setXShift(0.1);
	siggy.setYShift(-0.1);
	siggy.setXStretch(5.0);
	siggy.setYStretch(1.0);
*/    a.a_percentage = 0;
    a.a_type = CURRENTPROCESS;
	tenpercent = height / 10;
/*    for (i = 0; i < pixelcount; ++i)
	{
		if ((advancementcallback) && (!(i % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
		ind++;
	}
*/
    for (j = 0; j < 1; ++j)
	{
/*		if ((advancementcallback) && (!(j % tenpercent)))
		{
			(*advancementcallback) (a);
			a.a_percentage += 10;
		}
*/		temp=drand48()*0.8+0.2;
		randoms=(int)(temp*(double)width);
		for(i=0;i<width;++i)
			values[i]=0.0F;
		for(i=0;i<randoms;++i)
			values[(random()&0x7f)*2]=random()%256;
	 	for(i = width+1;i<((height) * width) - 1; ++i)
	    	values[i]=(values[i-width-1]+values[i - width]+values[i-width+1]+values[i]*0.99)*0.25;
    }
//	rescaleIntensity(0.0,1.0);
//			transform(siggy);
	return (*this);
}
std::ostream &EB_Band::writeToStream(std::ostream &o,bool pack)
{
	unsigned int i,packint;
	unsigned char lonely;

/*	o<<giveWidth()<<" "<< 
		giveHeight()<<" "<<
		pixelcount<<" "<<
		giveMinFloat()<<" "<<
		giveMaxFloat()<<" "<<
		minmaxinterval<<" ";
*/
		o<<pack<<" ";
		if(!(pack))
		{	
			for(i=0;i<pixelcount;++i)
				o<<values[i]<<" ";
		}
		else
		{
			for(i=0;i<pixelcount/4;++i)
			{
				packint=((giveCharValue(i*4))<<24)+
					((giveCharValue(i*4+1))<<16)+
					((giveCharValue(i*4+2))<<8)+
					giveCharValue(i*4+3);
					o<<packint<<" ";
			}
			for(i=(pixelcount/4)*4;i<pixelcount;++i)
			{
				lonely=giveCharValue(i);
				o<<lonely<<" ";
			}
		}
	return (o);
}
std::istream &EB_Band::readFromStream(std::istream &i) throw (EBICouldNotLoadEXP)
{
	unsigned int loop,packint;
	unsigned char lonely;
	bool pack=false;
	
/*	i>>width>> 
		height>>
		pixelcount>>
		minfloat>>
		maxfloat>>
		minmaxinterval;
*/	
		if(!(i.fail()))
			i>>pack;
		else
			throw new EBICouldNotLoadEXP();
		if(!(pack))
		{	
			if(!(i.fail()))
				for(loop=0;loop<pixelcount;++loop)
					i>>values[loop];
			else
				throw new EBICouldNotLoadEXP();
		}
		else
		{
			for(loop=0;loop<pixelcount/4;++loop)
			{
				if(!(i.fail()))
					i>>packint;
				else
					throw new EBICouldNotLoadEXP();
				setValue(loop*4,(unsigned char)((packint&0xff000000)>>24));
				setValue(loop*4+1,(unsigned char)((packint&0x00ff0000)>>16));
				setValue(loop*4+2,(unsigned char)((packint&0x0000ff00)>>8));
				setValue(loop*4+3,(unsigned char)(packint&0x000000ff));
			}
			for(loop=(pixelcount/4)*4;loop<pixelcount;++loop)
			{
				if(!(i.fail()))
					i>>lonely;
				else
					throw new EBICouldNotLoadEXP();
				setValue(loop,lonely);
			}
		}
	return (i);
}

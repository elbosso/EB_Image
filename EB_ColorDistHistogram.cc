#include "EB_ColorDistHistogram.h"
#include "EB_Band.h"
/*\Konstruktoren*/
EB_ColorDistHistogram::EB_ColorDistHistogram (unsigned int count):
samplecount (0), spointcount (count), lookuptable (spointcount, 255), histogram (new float[spointcount]), scaledhistogram (new float[spointcount])
{
//cout<<"---------------"<<spointcount<<endl;
    eraseHistogram ();
//cout<<"---------------"<<spointcount<<endl;
}

EB_ColorDistHistogram::EB_ColorDistHistogram (const EB_ColorDistHistogram & other):samplecount (other.samplecount), spointcount (other.giveSPointCount ()), lookuptable (other.lookuptable), histogram (new float[spointcount]), scaledhistogram (new float[spointcount])
{
    unsigned int i;

    for (i = 0; i < spointcount; i++)
	{
	    scaledhistogram[i] = other.giveRelValue (i);
	    histogram[i] = other.giveAbsValue (i);
	}
}

/*\*/
/*\Destruktor*/
EB_ColorDistHistogram::~EB_ColorDistHistogram ()
{
    delete (histogram);
    delete (scaledhistogram);
}

/*\*/
/*\EB_ColorDistHistogram &EB_ColorDistHistogram::operator=(const EB_ColorDistHistogram &other)*/
EB_ColorDistHistogram & EB_ColorDistHistogram::operator = (const EB_ColorDistHistogram & other)
{
    unsigned int i;

    if (this != &other)
	{
	    samplecount = other.samplecount;
	    delete (histogram);
	    histogram = new float[spointcount];
	    delete (scaledhistogram);
	    scaledhistogram = new float[spointcount];
	    lookuptable = other.lookuptable;
	    spointcount = other.giveSPointCount ();
	    for (i = 0; i < spointcount; i++)
		{
		    scaledhistogram[i] = other.giveRelValue (i);
		    histogram[i] = other.giveAbsValue (i);
		}
	}
    return (*this);
}

/*\*/
/*\float *EB_ColorDistHistogram::giveRelHistogramPointer(void)*/
float *EB_ColorDistHistogram::giveRelHistogramPointer (void)
{
    unsigned int i;
    float rsc = 1.0 / (float) samplecount;

    for (i = 0; i < spointcount; i++)
	scaledhistogram[i] = histogram[i] * rsc;
    return (scaledhistogram);
}

/*\*/
/*\void EB_ColorDistHistogram::eraseHistogram(void)*/
void EB_ColorDistHistogram::eraseHistogram (void)
{
    unsigned int i;

    for (i = 0; i < spointcount; i++)
	scaledhistogram[i] = histogram[i] = 0.0;
    samplecount = 0;
}

/*\*/
/*\void EB_ColorDistHistogram::eraseHistogram(unsigned int newspointcount)*/
void EB_ColorDistHistogram::eraseHistogram (unsigned int newspointcount)
{
    delete (histogram);
    delete (scaledhistogram);
    spointcount = newspointcount;
    histogram = new float[spointcount];
    scaledhistogram = new float[spointcount];
    lookuptable.redimension (spointcount);
    samplecount = 0;
    eraseHistogram ();
}

/*\*/
/*\float EB_ColorDistHistogram::giveAbsValue(unsigned int index)*/
float EB_ColorDistHistogram::giveAbsValue (unsigned int index) const
{
    if (index < giveSPointCount ())
	return (histogram[index]);
    else
	return (0);
}

/*\*/
/*\float EB_ColorDistHistogram::giveRelValue(unsigned int index)*/
float EB_ColorDistHistogram::giveRelValue (unsigned int index) const
{
    if (index < giveSPointCount ())
	return (histogram[index] / giveSampleCount ());
    else
	return (0);
}

/*\void EB_ColorDistHistogram::calculateHistogram(EB_Band &band)*/
void EB_ColorDistHistogram::calculateHistogram (EB_Band & band)
{
    unsigned int i, pixelcount=band.giveWidth() * band.giveHeight (), ind;
    float min = band.giveMinFloat (), max = band.giveMaxFloat (), gg = 1.0 / (max - min);
    eraseHistogram ();

    for (i= 0; i < pixelcount; ++i)
	{
	    ind = (unsigned int) (255.0 * ((band[i] - min) * gg));
	    histogram[ind] += 1.0;
	    samplecount++;
	}
}

/*\*/
/*\void EB_ColorDistHistogram::aequalizeHistogram(void)*/
void EB_ColorDistHistogram::aequalizeHistogram (void)
{
    float akku = 0, td;
    unsigned int sp = giveSPointCount (), i, si;
    float m = 1.0 / (float) sp;
    float *hist;

    hist = new float[sp];
    for (i = 0; i < sp; i++)
	{
	    akku += giveRelValue (i);
	    hist[i] = akku;
	}
    si = 1;
    lookuptable.setValue (0, 0);
    for (i = 0; i < sp; i++)
	{
	    td = m * (float) i;
	    if (td < hist[si])
		lookuptable.setValue (si, lookuptable.giveValue (si - 1));
	    else
		{
			while (td > hist[si])
		    {
				si++;
				lookuptable.setValue (si, lookuptable.giveValue (si - 1));
		    }
	    	lookuptable.setValue (si, i);
		}
	}
    delete (hist);
}

/*\*/
/*void EB_ColorDistHistogram::smoothBox(unsigned int width)*/
void EB_ColorDistHistogram::smoothBox(unsigned int width)
{
    unsigned int i;
    int rand, k, ind;
    unsigned int sp = giveSPointCount ();
    float fwidth;

    if (width / 2 == (width - 1) / 2)
	width++;
    if (width < 2)
	width = 3;
    rand = width / 2;
    fwidth = 1.0 / (float) width;

    for (i = 0; i < sp; ++i)
	{
	    scaledhistogram[i]=0.0;
	}
    for (i = 0; i < sp; ++i)
	{
	    for (k = -rand; k < rand + 1; k++)
		{
		    ind = i + k;
		    if ((ind >= 0) && (ind < (int) sp))
			scaledhistogram[i] += histogram[ind];
		}
	    scaledhistogram[i] *= fwidth;
	}
    for (i = 0; i < sp; ++i)
	{
	    histogram[i] = scaledhistogram[i];
	}
}
/*void EB_ColorDistHistogram::smoothBoxClosed(unsigned int width)*/
void EB_ColorDistHistogram::smoothBoxClosed(unsigned int width)
{
    unsigned int i;
    int rand, k, ind;
    unsigned int sp = giveSPointCount ();
    float fwidth;

    if (width / 2 == (width - 1) / 2)
	width++;
    if (width < 2)
	width = 3;
    rand = width / 2;
    fwidth = 1.0 / (float) width;

    for (i = 0; i < sp; ++i)
	{
	    scaledhistogram[i]=0.0;
	}
    for (i = 0; i < sp; ++i)
	{
	    for (k = -rand; k < rand + 1; k++)
		{
		    ind = i + k;
		    if (ind < 0)
				ind+=sp;
			else if(ind >= (int) sp)
				ind-=sp;
			scaledhistogram[i] += histogram[ind];
		}
	    scaledhistogram[i] *= fwidth;
	}
    for (i = 0; i < sp; ++i)
	{
	    histogram[i] = scaledhistogram[i];
	}
}

/*\*/
float EB_ColorDistHistogram::giveMax(void)
{
	float max=histogram[0];
	unsigned int i;
    unsigned int sp = giveSPointCount ();
	
    for (i = 0; i < sp; ++i)
		if(max<histogram[i])
			max=histogram[i];
	return(max);
}
unsigned int EB_ColorDistHistogram::giveIndexOfMax(void)
{
	float max=histogram[0];
	unsigned int i,imax=0;
    unsigned int sp = giveSPointCount ();
	
    for (i = 0; i < sp; ++i)
	{
		if(max<histogram[i])
		{
			max=histogram[i];
			imax=i;
		}
	}
	return(imax);
}
float EB_ColorDistHistogram::giveMin(void)
{
	float min=histogram[0];
	unsigned int i;
    unsigned int sp = giveSPointCount ();
	
    for (i = 0; i < sp; ++i)
		if(min>histogram[i])
			min=histogram[i];
	return(min);
}
unsigned int EB_ColorDistHistogram::giveIndexOfMin(void)
{
	float min=histogram[0];
	unsigned int i,imin=0;
    unsigned int sp = giveSPointCount ();
	
    for (i = 0; i < sp; ++i)
	{
		if(min>histogram[i])
		{
			min=histogram[i];
			imin=i;
		}
	}
	return(imin);
}
EB_ColorDistHistogram &EB_ColorDistHistogram::mulElementwise (const EB_ColorDistHistogram &other)
{
	unsigned int smallestlength=(giveSPointCount()<other.giveSPointCount())?giveSPointCount():other.giveSPointCount();
	unsigned int i;
	
	for	(i=0;i<smallestlength;++i)
		histogram[i]*=other[i];
	for	(i=smallestlength;i<giveSPointCount();++i)
		histogram[i]=0.0;
	return(*this);
}

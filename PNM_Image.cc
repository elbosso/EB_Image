/****************************************************************************
*
* File        : PNM_Image.cc
* Class       : PNM_Image
*  based on   : EB_Image  
*  uses       : 
*
* Author      : VIP-Projektbearbeiter <jkey@cortex.informatik.tu-ilmenau.de>
*               TU Ilmenau / Fak. IA / FG Neuroinformatik
* Date        : Tue May 26 1998 created
*
* Version     : 0.04
* State       : final pre alpha
*
* Description : Root-class for the various image-file-formats known as
*               PBM.
*
****************************************************************************/


/***************************************************************************
 * Used Headerfile
 **************************************************************************/
#include "PNM_Image.h"
#include<iostream>


PNM_Image::PNM_Image (unsigned int x, unsigned int y, unsigned int usedbands, float color, float min, float max):EB_Image (x, y, usedbands, color, min, max),
pnmtype(NO_PNM),
charmax(255)
{
}
/*PNM_Image::PNM_Image (const PNM_Image & Image):EB_Image (Image)
{
    pnmtype = NO_PNM;
    charmax = 255;
}*/
PNM_Image::PNM_Image (const EB_Image & Image):EB_Image (Image),
pnmtype(NO_PNM),
charmax(255)
{
}
PNM_Image::PNM_Image (const char *name):EB_Image (name),
pnmtype(NO_PNM),
charmax(255)
{
    load (name);
}
PNM_Image::~PNM_Image ()
{
}
EB_Image & PNM_Image::load (const char *name)throw(EBIOutOfMemoryEXP,EBIPNMLoaderEXP,EBICouldNotLoadEXP,EBICouldNotOpenLoadFileEXP)
{
    FILE *infp;
    char buf[500], *crv = NULL;
    int tx, ty, tmax;
    unsigned int i,pixelcount,b,bc,tint;
    unsigned char tchar;
    unsigned char *tarray, *pointer;

	cout<<"a "<<name<<endl;
//  printf("%s\n",name);
    pnmtype = 0;
    infp = fopen (name, "rb");
    if (infp)
	{
	cout<<"a"<<endl;
	    crv = fgets (buf, 500, infp);
	    if (crv)
		{
		    while (buf[0] == '#')
			crv = fgets (buf, 500, infp);
		    if (crv)
			{
			    if (buf[0] == 'P')
				{
				    if ((buf[1] >= '1') && (buf[1] < '4'))
					{
					    pnmtype |= ASCII;
					}
				    else if ((buf[1] > '3') && (buf[1] < '8'))
					{
	cout<<"b "<<buf[1]<<endl;
					    pnmtype |= BIN;
					    buf[1] -= 3;
					}
				    if (pnmtype)
					{
					    switch (buf[1])
						{
						case '1':
						    {
							pnmtype |= PBM;
							pixel.changeComponentCount (1);
							break;
						    }
						case '2':
						    {
							pnmtype |= PGM;
							pixel.changeComponentCount (1);
							break;
						    }
						case '3':
						    {
							pnmtype |= PPM;
							pixel.changeComponentCount (3);
							break;
						    }
						case '4':
						    {
									cout<<"c"<<endl;

							pnmtype |= XVPICS;
							pixel.changeComponentCount (3);
							break;
						    }
						}
					    crv = fgets (buf, 500, infp);
					    while (buf[0] == '#')
						crv = fgets (buf, 500, infp);
					    sscanf (buf, "%d %d", &tx, &ty);
					    crv = fgets (buf, 500, infp);
					    while (buf[0] == '#')
						crv = fgets (buf, 500, infp);
					    sscanf (buf, "%d", &tmax);
					    width = tx;
					    height = ty;
						charmax = tmax;
					}
				}
				pixelcount=width*height;
				bc=giveBandCount();
    			deallocateBands ();
    			allocateBands (-1.0, 0.0, 1.0);
				unsigned char buf[bc*sizeof(unsigned char)*pixelcount];
    			if (pnmtype & PGM)
				{
 					if (pnmtype & ASCII)
					{
			    		for (i = 0; i < pixelcount; ++i)
						{
				    		fscanf (infp, "%d ", &tint);
				    		tchar = (unsigned char) tint;
				    		bands[0]->setValue (i, tchar);
						}
					}
		    		else
					{
						fread(buf,1,bc*sizeof(unsigned char)*pixelcount,infp);
			    		for (i = 0; i < pixelcount; ++i)
						{
//				    		fscanf (infp, "%c", &tchar);
				    		bands[0]->setValue (i, buf[i]);
						}
					}
				}
				else if (pnmtype & PPM)
				{
					if (pnmtype & ASCII)
					{
			    		for (i = 0; i < pixelcount; ++i)
						{
						    for (b = 0; b < giveBandCount (); b++)
							{
							    fscanf (infp, "%d ", &tint);
							    tchar = (unsigned char) tint;
							    bands[b]->setValue (i, tchar);
							}
						}
					}
		   			else
					{
			    		fread (buf, 1, bc*sizeof(unsigned char)*pixelcount, infp);
						pointer=buf;
		    			for (i = 0; i < pixelcount; ++i)
						{
					    	for (b = 0; b < giveBandCount (); b++)
							{
							    bands[b]->setValue (i,*pointer++);
							}
						}
					}
				}
				else if (pnmtype & XVPICS)
				{
					if (pnmtype & ASCII)
					{
						throw EBIPNMLoaderEXP();
					}
		   			else
					{
			    		fread (buf, 1, bc*sizeof(unsigned char)*pixelcount, infp);
						pointer=buf;
		    			for (i = 0; i < pixelcount; ++i)
						{
					    	for (b = 0; b < giveBandCount (); b++)
							{
							    bands[b]->setValue (i,*pointer++);
							}
						}
					}
				}
				else
					throw EBIPNMLoaderEXP();
			}
			else
				throw EBICouldNotLoadEXP();
		}
		else
			throw EBICouldNotLoadEXP();
	    fclose (infp);
	}
	else
		throw EBICouldNotOpenLoadFileEXP ();
    return (*this);
}
void PNM_Image::save (const char *name)throw(EBIWrongNumberOfBandsEXP,EBICouldNotOpenSaveFileEXP)
{
    FILE *outfp;
    unsigned int i,pixelcount=width*height,b,bc=giveBandCount();
	unsigned char buf[bc*sizeof(unsigned char)*pixelcount];
	unsigned int bufindex=0;
	
    outfp = fopen (name, "wb");
    if (outfp)
	{
		if(bc==1)
		{
			EB_Band &band=(*(bands[0]));
		    if (pnmtype & ASCII)
			fprintf (outfp, "P2\n");
		    else
			fprintf (outfp, "P5\n");
		    fprintf (outfp, "%d %d\n", width, height);
		    fprintf (outfp, "255\n");
		    if (pnmtype & ASCII)
			{
			    for (i = 0; i < pixelcount; ++i)
				    fprintf (outfp, "%d ", band.giveCharValue(i));
			}
		    else
			{
			    for (i = 0; i < pixelcount; ++i)
//					fprintf (outfp, "%c", band.giveCharValue(i));
					buf[bufindex++]=bands[b]->giveCharValue (i);
				fwrite(buf,1,bc*sizeof(unsigned char)*pixelcount,outfp);
			}
		}
		else if (giveBandCount () == 3)
		{
		    if (pnmtype & ASCII)
			fprintf (outfp, "P3\n");
		    else
			fprintf (outfp, "P6\n");
		    fprintf (outfp, "%d %d\n", giveWidth (), giveHeight ());
		    fprintf (outfp, "255\n");
		    if (pnmtype & ASCII)
			{
			    for (i = 0; i < pixelcount; ++i)
				    for (b = 0; b < giveBandCount (); b++)
					    fprintf (outfp, "%d ", bands[b]->giveCharValue (i));
			}
		    else
			{
			    for (i = 0; i < pixelcount; ++i)
				    for (b = 0; b < giveBandCount (); b++)
//					    fprintf (outfp, "%c", bands[b]->giveCharValue (i));
						buf[bufindex++]=bands[b]->giveCharValue (i);
				fwrite(buf,1,bc*sizeof(unsigned char)*pixelcount,outfp);
			}
		}
		else
			throw EBIWrongNumberOfBandsEXP();
	    fclose (outfp);
	}
	else
		throw EBICouldNotOpenSaveFileEXP ();
}
/*
int PNM_Image::giveType (void)
{
    return (pnmtype);
}
*/

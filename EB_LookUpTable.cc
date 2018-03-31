/****************************************************************************
*
* File        : EB_LookUpTable.cc
* Class       : EB_LookUpTable
*  based on   :   
*  uses       :   
*
* Author      : VIP-Projektbearbeiter <jkey@cortex.informatik.tu-ilmenau.de>
*               TU Ilmenau / Fak. IA / FG Neuroinformatik
* Date        : Wed May 27 1998 created
*
* Version     : 0.03
* State       : final pre-alpha
*
* Description : Implementation of a look-up table used in the classtree for image processing
*
****************************************************************************/


/***************************************************************************
 * Used Headerfile
 **************************************************************************/

#include "EB_LookUpTable.h"

/*\Konstruktoren*/
EB_LookUpTable::EB_LookUpTable(unsigned int number):
  entrycount(number),
  table(new float[entrycount])
{
}
EB_LookUpTable::EB_LookUpTable(unsigned int number,float initvalue):
  entrycount(number),
  table(new float[entrycount])
{
  init(initvalue);
}
EB_LookUpTable::EB_LookUpTable(const EB_LookUpTable &other):
  entrycount(other.giveEntryCount()),
  table(new float[entrycount])
{
  unsigned int i;

  for(i=0;i<entrycount;i++)
      table[i]=other.giveValue(i);
}
/*\*/
/*\Destruktor*/
EB_LookUpTable::~EB_LookUpTable()
{
}
/*\*/
/*\EB_LookUpTable &EB_LookUpTable::operator=(const EB_LookUpTable &other)*/
EB_LookUpTable &EB_LookUpTable::operator=(const EB_LookUpTable &other)
{
  unsigned int i;
	if(this!=&other)
	{
  entrycount=other.giveEntryCount();
  delete(table);
  table=new float[entrycount];

  for(i=0;i<entrycount;i++)
      table[i]=other.giveValue(i);
}
return(*this);

}
/*\*/
/*\void EB_LookUpTable::redimension(unsigned int newsize)*/
void EB_LookUpTable::redimension(unsigned int newsize)
{
  entrycount=newsize;
  delete(table);
  table=new float[entrycount];
}
/*\*/
/*\void EB_LookUpTable::redimension(unsigned int newsize,float initvalue)*/
void EB_LookUpTable::redimension(unsigned int newsize,float initvalue)
{
  entrycount=newsize;
  delete(table);
  table=new float[entrycount];
  init(initvalue);
}
/*\*/
/*\float EB_LookUpTable::giveValue(unsigned int index)*/
float EB_LookUpTable::giveValue(unsigned int index) const throw(EBIIndexOutOfRangeEXP)
{
  if(index<entrycount)
    return(table[index]);
  else
    throw EBIIndexOutOfRangeEXP();
}
/*\*/
/*\void EB_LookUpTable::init(float value)*/
void EB_LookUpTable::init(float value)
{
  unsigned int i;

  for(i=0;i<entrycount;i++)
      table[i]=value;
}
/*\*/
/*\void EB_LookUpTable::calculateGammas(float factor)*/
void EB_LookUpTable::calculateGammas(float factor)
{
  unsigned int i;
  for(i=0;i<entrycount;i++)
      table[i]=(float)(pow((float)i/(float)entrycount,factor)*(float)entrycount);
}
/*\*/
/*\void EB_LookUpTable::calculateContrast(float factor)*/
void EB_LookUpTable::calculateContrast(float factor)
{
  unsigned int i;
  float deltavalue,value;

  deltavalue=1.0/(1.0+exp(-(-0.5)*factor));

  for(i=0;i<entrycount;i++)
    {
      value=1.0/(1.0+exp(-(((float)i/(float)entrycount)-0.5)*factor))-deltavalue+(float)i/(float)entrycount*(2.0*deltavalue);
      table[i]=(float)(value*256.0);
    }
}
/*\*/
void EB_LookUpTable::calculateIntensityTransformation(const EB_IntensityTransformation &trans)
{
  unsigned int i;

  for(i=0;i<entrycount;i++)
      table[i]=trans.transform((float)i/(float)entrycount)*(float)entrycount;
}

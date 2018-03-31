#include"EB_Filter.h"

/*\Konstruktoren*/
EB_Filter::EB_Filter(unsigned int x, unsigned int y):EB_Band(((x/2==(x+1)/2)?x+1:x),((y/2==(y+1)/2)?y+1:y),0.0,-100000.0,100000.0)//,
//  semiwidth(width/2+1),
//  semiheight(height/2+1)
{  
}
EB_Filter::EB_Filter(const EB_Filter &other):EB_Band(other)//,
//  semiwidth(other.semiwidth),
//  semiheight(other.semiheight)
{
}
/*\*/
/*\EB_Filter &EB_Filter::operator=(const EB_Filter &other)*/
EB_Filter &EB_Filter::operator=(const EB_Filter &other)
{
	if(this!=&other)
	{
	EB_Band::operator=(other);
//  semiwidth=other.semiwidth;
//  semiheight=other.semiheight;
	}
	return(*this);
}
/*\*/
/*\void EB_Filter::setValueSquareSymm(unsigned int x, unsigned int y, float coefficient)*/
void EB_Filter::setValueSquareSymm(unsigned int x, unsigned int y, float coefficient)
{
  if ((x<giveWidth())&&(y<giveHeight()))
    {
/*      if(x>=semiwidth)
	x=giveWidth()-(x+1);
      if(y>=semiheight)
	y=giveHeight()-(y+1);
  */      giveFloatValue(x,y);
      setValue(x,y,coefficient);
      setValue(x,giveWidth()-(y+1),coefficient);
      setValue(giveWidth()-(x+1),y,coefficient);
      setValue(giveWidth()-(x+1),giveWidth()-(y+1),coefficient);
    }
}
/*\*/
/*\void EB_Filter::setValueXSymm(unsigned int x, unsigned int y, float coefficient)*/
void EB_Filter::setValueXSymm(unsigned int x, unsigned int y, float coefficient)
{
  if ((x<giveWidth())&&(y<giveHeight()))
    {
//      if(y>=semiheight)
//	y=giveHeight()-(y+1);
      setValue(x,y,coefficient);
      setValue(x,giveWidth()-(y+1),coefficient);
    }
}
/*\*/
/*\void EB_Filter::setValueYSymm(unsigned int x, unsigned int y, float coefficient)*/
void EB_Filter::setValueYSymm(unsigned int x, unsigned int y, float coefficient)
{
  if ((x<giveWidth())&&(y<giveHeight()))
    {
//      if(x>=semiwidth)
//	x=giveWidth()-(x+1);
      setValue(x,y,coefficient);
      setValue(giveWidth()-(x+1),y,coefficient);
    }
}
/*\*/
/*\void EB_Filter::setValueFunc(cffuncptr function)*/
void EB_Filter::setValueFunc(cffuncptr function)
{
  int x,y,sw=giveWidth()/2,sh=giveHeight()/2;

  if(function);
  for (x=-sw;x<=sw;x++)
    {
      for (y=-sh;y<=sh;y++)
	{
	  setValue(x+sw,y+sh,(*function)(x,y,sw,sh));
	}
    }
}
float EB_Filter::givePosSum(void)const
{
	unsigned int i;
	float sum=0;
	
	for(i=0;i<pixelcount;++i)
		if((*this)[i]>0.0)
			sum+=(*this)[i];
	return(sum);
}
float EB_Filter::giveNegSum(void)const
{
	unsigned int i;
	float sum=0;
	
	for(i=0;i<pixelcount;++i)
		if((*this)[i]<0.0)
			sum+=(*this)[i];
	return(sum);
}
/*\*/

#include"EB_SinusFilter.h"

void EB_SinusFilter::calculateFilterMask(void)
{
	unsigned int x,y;
	float midx=(float)width/2.0F,midy=(float)height/2.0F;
	unsigned int ind=0;
	float xarg,yarg,fcy,fcx;
	
//	cout<<fieldheight<<" "<<fieldwidth<<endl;
	
	for(y=0;y<height;++y)
	{
		yarg=((float)y-midy)/midy*M_PI*yfreq+yoff;
		fcy=sin(yarg);
		for(x=0;x<width;++x)
		{
			xarg=((float)x-midx)/midx*M_PI*xfreq+xoff;
			fcx=sin(xarg);
			values[ind]=fcx*fcy;
			ind++;
		}
	}
	cout<<givePosSum()<<" "<<giveNegSum()<<endl;
}

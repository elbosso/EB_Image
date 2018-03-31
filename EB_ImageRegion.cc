#include"EB_ImageRegion.h"

bool EB_ImageRegion::isInside (int x, int y)
{
	EB_ImageCoordinatePair startpair;
	EB_ImageCoordinatePair endpair;
	EB_ImageCoordinatePair lastendpair;
	unsigned int i,crosscounter=0;
	int z1,z2,z3,z4;

	if(isRegion())
	{
		if (!preprocessed)
			  doPreprocess ();
		if (x < xmin)
			  return (false);
		else if (x > xmax)
			  return (false);
		else if (y < ymin)
			  return (false);
		else if (y > ymax)
			  return (false);
		else
		{
			coordlist.init ();
			crosscounter=0;		
			lastendpair = startpair = coordlist.giveNext ();
			for (i = 0; i < coordlist.giveNodeCount () - 1; ++i)
			{
				endpair = coordlist.giveNext ();
				if(MY_MAX(x,0)>MY_MIN(startpair.xposition,endpair.xposition)&&
					MY_MAX(startpair.xposition,endpair.xposition)>MY_MIN(x,0)&&
					MY_MAX(y,0)>MY_MIN(startpair.yposition,endpair.yposition)&&		
					MY_MAX(startpair.yposition,endpair.yposition)>MY_MIN(y,0))
				{
					z1=startpair.xposition*y-startpair.yposition*x;
					z2=endpair.xposition*y-endpair.yposition*x;
					z3=(-startpair.xposition)*(endpair.yposition-startpair.yposition)-(-startpair.yposition)*(endpair.xposition-startpair.xposition);						
					z4=(x-startpair.xposition)*(endpair.yposition-startpair.yposition)-(y-startpair.yposition)*(endpair.xposition-startpair.xposition);
/*					z1=((startpair.xposition-x)*(0-y))-((startpair.yposition-y)*(0-x));
					z2=((endpair.xposition-x)*(0-y))-((endpair.yposition-y)*(0-x));
					z3=((x-startpair.xposition)*(endpair.yposition-startpair.yposition))-
						((y-startpair.yposition)*(endpair.xposition-startpair.xposition));
					z4=((0-startpair.xposition)*(endpair.yposition-startpair.yposition))-
						((0-startpair.yposition)*(endpair.xposition-startpair.xposition));
					if((z1*z2<=0)&&(z3*z4<=0))
*/					if((z2*z1<0)&&(z3*z4<0))
					crosscounter++;
				}
				startpair = endpair;
			}
			endpair = lastendpair;
			if(MY_MAX(x,0)>MY_MIN(startpair.xposition,endpair.xposition)&&
				MY_MAX(startpair.xposition,endpair.xposition)>MY_MIN(x,0)&&
				MY_MAX(y,0)>MY_MIN(startpair.yposition,endpair.yposition)&&		
				MY_MAX(startpair.yposition,endpair.yposition)>MY_MIN(y,0))
			{
					z1=startpair.xposition*y-startpair.yposition*x;
					z2=endpair.xposition*y-endpair.yposition*x;
					z3=(-startpair.xposition)*(endpair.yposition-startpair.yposition)-(-startpair.yposition)*(endpair.xposition-startpair.xposition);						
					z4=(x-startpair.xposition)*(endpair.yposition-startpair.yposition)-(y-startpair.yposition)*(endpair.xposition-startpair.xposition);
/*				z1=((startpair.xposition-x)*(0-y))-((startpair.yposition-y)*(0-x));
				z2=((endpair.xposition-x)*(0-y))-((endpair.yposition-y)*(0-x));
				z3=((x-startpair.xposition)*(endpair.yposition-startpair.yposition))-
					((y-startpair.yposition)*(endpair.xposition-startpair.xposition));
				z4=((0-startpair.xposition)*(endpair.yposition-startpair.yposition))-
					((0-startpair.yposition)*(endpair.xposition-startpair.xposition));
				if((z1*z2<=0)&&(z3*z4<=0))
*/					if((z2*z1<0)&&(z3*z4<0))
					crosscounter++;
			}		
			if(crosscounter%2==1)
				return(true);
			else
				return(false);
		}
	}
	return (true);
}

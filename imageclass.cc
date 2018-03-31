#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream.h>
#include <unistd.h>
#include <dirent.h>

#include "EB_Image.h"
#include "JPG_Image.h"
#include "PNG_Image.h"
#include "PNM_Image.h"
#include "EB_Filter.h"
#include "MPEG_Encoder.h"
#include "MPEG_Decoder.h"

// Makestring Home
// gcc -DUNIX -L../../jpeg-6b -L. -Lmpeg2enc -Lmpeg2dec -I. -I.. -I../utils -Impeg2enc/ -Impeg2dec/ -I../../jpeg-6b imageclass.cc -lm -lttf -leb_wwimage -leb_mpeg2enc -leb_mpeg2dec -ljpega -o te
// Makestring Uni
// gcc -DUNIX -L.. -L../utils -L../../jpeg-6b -L. -Lmpeg2enc -Lmpeg2dec -I. -I.. -I../utils -Impeg2enc/ -Impeg2dec/ -I../../jpeg-6b -I/usr/include/freetype/ imageclass.cc -lm -lttf -leb_mpeg2dec -leb_wwimage -ljpega -leb_mpeg2enc -lstdc++ -leb_utils -o te

int main (int argc, char **argv)
{
	int ret;

	MPEG_Decoder mpgdec(argv[1]);
	EB_Image eb(1,1,3);
	JPG_Image img;
	printf("hallo\n");
  	ret = mpgdec.decodeImage(eb);
		cout<<eb.giveWidth()<<" "<<eb.giveHeight()<<" "<<eb.giveBandCount()<<endl;
		img=eb;
		img.save("hallo.jpg");
  return (0);
}

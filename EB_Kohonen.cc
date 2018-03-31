#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"EB_Kohonen.h"

EB_Kohonen::EB_Kohonen(int dimx, int dimy, int dimz ,int dimf,float Abs,float Relmin,float Relmax):
    NX(dimx),
	NY(dimy),
	NZ(dimz),
	NF(dimf),
    NeuronCount(NX*NY*NZ),
    KNField(new KNeuron*[NeuronCount]), 
    AbsoluteLimit(Abs),
	RelativeMin(Relmin),
	RelativeMax(Relmax)
{
    int x,y,z,NPlace;
    KNeuron *Neuron;

    srand(1234);
    for(z=0;z<NZ;z++)
    {
	for(y=0;y<NY;y++)
	{
	    for(x=0;x<NX;x++)
	    {
		NPlace=z*NY*NX+y*NX+x;
		Neuron=new KNeuron;
		Neuron->location=new float[3];
		Neuron->location[0]=(float)x;
		Neuron->location[1]=(float)y;
		Neuron->location[2]=(float)z;
		Neuron->weights=new float[NF];
		InitializeWeights(Neuron->weights,NF,AbsoluteLimit,RelativeMin,RelativeMax);
		Neuron->wcount=NF;
		KNField[NPlace]=Neuron;
	    }
	}
    }
}
EB_Kohonen::EB_Kohonen(const EB_Kohonen &other):
    NX(other.NX),
	NY(other.NY),
	NZ(other.NZ),
	NF(other.NF),
    NeuronCount(NX*NY*NZ),
    KNField(new KNeuron*[NeuronCount]), 
    AbsoluteLimit(other.AbsoluteLimit),
	RelativeMin(other.RelativeMin),
	RelativeMax(other.RelativeMax)
{
    int x,y,z,NPlace;
    KNeuron *Neuron;

    srand(1234);
    for(z=0;z<NZ;z++)
    {
	for(y=0;y<NY;y++)
	{
	    for(x=0;x<NX;x++)
	    {
		NPlace=z*NY*NX+y*NX+x;
		Neuron=new KNeuron;
		Neuron->location=new float[3];
		Neuron->location[0]=(float)x;
		Neuron->location[1]=(float)y;
		Neuron->location[2]=(float)z;
		Neuron->weights=new float[NF];
	    other.fillFeatures(x,y,z ,Neuron->weights);
		Neuron->wcount=NF;
		KNField[NPlace]=Neuron;
	    }
	}
    }
}
EB_Kohonen::~EB_Kohonen(void)
{
    freeNet();
}
EB_Kohonen &EB_Kohonen::operator=(const EB_Kohonen &other)
{
    int x,y,z,NPlace;
    KNeuron *Neuron;

	if(this!=&other)
	{
    freeNet();
    NX=other.NX;
	NY=other.NY;
	NZ=other.NZ;
	NF=other.NF;
    NeuronCount=NX*NY*NZ;
    KNField=new KNeuron*[NeuronCount];
    AbsoluteLimit=other.AbsoluteLimit;
	RelativeMin=other.RelativeMin;
	RelativeMax=other.RelativeMax;
    srand(1234);
    for(z=0;z<NZ;z++)
    {
	for(y=0;y<NY;y++)
	{
	    for(x=0;x<NX;x++)
	    {
		NPlace=z*NY*NX+y*NX+x;
		Neuron=new KNeuron;
		Neuron->location=new float[3];
		Neuron->location[0]=(float)x;
		Neuron->location[1]=(float)y;
		Neuron->location[2]=(float)z;
		Neuron->weights=new float[NF];
	    other.fillFeatures(x,y,z ,Neuron->weights);
		Neuron->wcount=NF;
		KNField[NPlace]=Neuron;
	    }
	}
    }
	}
    return(*this);
}
void EB_Kohonen::freeNet(void)
{
    int i;
    KNeuron *Neuron;
/*Freigeben des Speichers*/
    if(KNField)
    {
	for (i=0;i<NeuronCount;i++)
	{
	    Neuron=KNField[i];
	    if(Neuron->location)
		delete (Neuron->location);
	    if(Neuron->weights)
		delete (Neuron->weights);
	    delete(Neuron);
	}
	delete(KNField);
    }
}
float EB_Kohonen::Random(float min,float max)
{
    float t;

    t=((float)rand()/(float)RAND_MAX)*max+min;
    return(t);
}

void EB_Kohonen::InitializeWeights(float *weights,int dimf,float absolute,float relativemin,float relativemax)
{
    int i;

    for(i=0;i<dimf;i++)
    {
	weights[i]=Random(relativemin,relativemax);
    }
//    weights[3]=weights[2];
}
void EB_Kohonen::fillFeatures(int x, int y, int z ,float *array) const
{
    int i,NPlace;
    KNeuron *Neuron;

    NPlace=z*NY*NX+y*NX+x;
    Neuron=KNField[NPlace];
    for (i=0;i<NF;i++)
    {
	array[i]=Neuron->weights[i];
    }
}
void EB_Kohonen::setWeights(int x, int y, int z ,float *array)
{
    int i,NPlace;
    KNeuron *Neuron;

    NPlace=z*NY*NX+y*NX+x;
    Neuron=KNField[NPlace];
    for (i=0;i<NF;i++)
    {
	Neuron->weights[i]=array[i];
    }
}
void EB_Kohonen::UpdateNeurons(float *inputs,int inputcount,float sigma,float eta)
{
    int i;

    for(i=0;i<inputcount;i++)
    {
	UpdateNeuronsOnce(inputs+i*NF,sigma,eta);
    }
}
int EB_Kohonen::SearchBestMatching(float *input)
{
    int x,y,z,bm,NPlace;
    KNeuron *Neuron;
    float minimum=10000.0;
    float distance;

/*Suchen des best matching Neurons*/
    bm=0;
    for(z=0;z<NZ;z++)
    {
	for(y=0;y<NY;y++)
	{
	    for(x=0;x<NX;x++)
	    {
		NPlace=z*NY*NX+y*NX+x;
		Neuron=KNField[NPlace];
		distance=EuklidDistance(Neuron->weights,input,NF);
		if(distance<minimum)
		{
		    minimum=distance;
		    bm=NPlace;
		}
	    }
	}
    }
    return(bm);
}
void EB_Kohonen::UpdateNeuronsOnce(float *input,float sigma,float eta)
{
    int x,y,z,i,NPlace;
    KNeuron *Neuron, *BMNeuron;
    float griddistance,h;

/*Suchen des best matching Neurons*/
    BMNeuron=KNField[SearchBestMatching(input)];
/*Update aller Gewichtsvektoren*/
    for(z=0;z<NZ;z++)
    {
	for(y=0;y<NY;y++)
	{
	    for(x=0;x<NX;x++)
	    {
		NPlace=z*NY*NX+y*NX+x;
		Neuron=KNField[NPlace];
		griddistance=EuklidDistance(Neuron->location,BMNeuron->location,3);
/*                if(griddistance<sigma)
		    h=cos((griddistance/sigma)*(PI/2));
		else
		    h=0;
  */              h=exp(-(griddistance/(sigma*sigma)));
		for(i=0;i<NF;i++)
		{
		    Neuron->weights[i]+=eta*h*(input[i]-Neuron->weights[i]);
		}
	    }
	}
    }
}
float EB_Kohonen::EuklidNorm(float *vec,int dim)
{
    return(EuklidDistance(vec,vec,dim));
}
float EB_Kohonen::EuklidDistance(float *vec1, float *vec2,int dim)
{
    int i;
    float result=0;

    for (i=0;i<dim;i++)
    {
	result+=((vec1[i]-vec2[i])*(vec1[i]-vec2[i]));
    }
    return(sqrt(result));
}
void EB_Kohonen::saveNet(char * name)
{
	FILE *outfp;
	float *tw=NULL;
	int x,y,z,i;

	if(name)
	{
		outfp=fopen(name,"wb");
		if(outfp)
		{
			tw=new float [NF];
			if(tw)
			{
				fprintf(outfp,"%d %d %d %d\n",NX,NY,NZ,NF);
				for(z=0;z<NZ;z++)
				{
					for(y=0;y<NY;y++)
					{
						for(x=0;x<NX;x++)
						{
							fillFeatures(x, y, z ,tw);
							for(i=0;i<NF;i++)
							{
								fprintf(outfp,"%f ",tw[i]);
							}
							fprintf(outfp,"\n");
						}
					}
				}
				delete(tw);
			}
			fclose(outfp);
		}
	}
}
void EB_Kohonen::loadNet(char * name)
{
	FILE *infp;
	float *tw=NULL;
	int x,y,z,i,NPlace;
	KNeuron *Neuron;

	if(name)
	{
		infp=fopen(name,"rb");
		if(infp)
		{
			freeNet();
			fscanf(infp,"%d %d %d %d\n",&NX,&NY,&NZ,&NF);
			NeuronCount=NX*NY*NZ;
		KNField=new KNeuron*[NeuronCount];
		for(z=0;z<NZ;z++)
		{
				for(y=0;y<NY;y++)
				{
				for(x=0;x<NX;x++)
				{
						NPlace=z*NY*NX+y*NX+x;
						Neuron=new KNeuron;
						Neuron->location=new float[3];
						Neuron->location[0]=(float)x;
						Neuron->location[1]=(float)y;
						Neuron->location[2]=(float)z;
						Neuron->weights=new float[NF];
						for(i=0;i<NF;i++)
						{
							fscanf(infp,"%lf ",&(tw[i]));
						}
						fscanf(infp,"\n");
						Neuron->wcount=NF;
						KNField[NPlace]=Neuron;
				}
				}
		}
			fclose(infp);
		}
	}
}



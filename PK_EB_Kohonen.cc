#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"PK_EB_Kohonen.h"

PK_EB_Kohonen::PK_EB_Kohonen (int dimz):EB_Kohonen (1, 1, dimz, 3, 1.0, 0.0, 4.5)
{
    int x, y, z, NPlace;
    KNeuron *Neuron;
    float zpos;

    for (z = 0; z < NZ; z++)
	{
	    for (y = 0; y < NY; y++)
		{
		    for (x = 0; x < NX; x++)
			{
			    NPlace = z * NY * NX + y * NX + x;
			    Neuron = KNField[NPlace];
			    zpos = (float) z / (float) NZ;
			    InitializeWeights (Neuron->weights, NF, AbsoluteLimit, RelativeMin, RelativeMax, zpos /*RelativeMax */ );
			}
		}
	}
}
PK_EB_Kohonen::PK_EB_Kohonen (const PK_EB_Kohonen & other):EB_Kohonen (other)
{
};
void PK_EB_Kohonen::InitializeWeights (float *weights, int dimf, float absolute, float relativemin, float relativemax, float lastkomponent)
{
    int i;

    for (i = 0; i < (dimf - 1); i++)
	{
	    weights[i] = 0.0;
	}
    weights[dimf - 1] = lastkomponent;
}

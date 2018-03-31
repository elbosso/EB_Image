#ifndef EB_KOHONEN_H

#define EB_KOHONEN_H

/*Struktur, welche die Neuronen darstellt*/
typedef struct
{
    float *location;
    float *weights;
    int wcount;
} KNeuron;



class EB_Kohonen
{
    public:
    EB_Kohonen(int dimx,int dimy,int dimz , int dimf,float Abs,float Relmin,float Relmax);
    EB_Kohonen(const EB_Kohonen &other);
    ~EB_Kohonen(void);
    EB_Kohonen &operator=(const EB_Kohonen &other);
    void freeNet(void);
    void InitializeWeights(float *weights,int dimf,float absolute,float relativemin,float relativemax);
    float Random(float min,float max);
    void fillFeatures(int x, int y, int z ,float *array)const;
    void setWeights(int x, int y, int z ,float *array);
    void UpdateNeurons(float *inputs,int inputcount,float sigma,float eta);
    int SearchBestMatching(float *input);
    void UpdateNeuronsOnce(float *input,float sigma,float eta);
    float EuklidNorm(float *vec,int dim);
    float EuklidDistance(float *vec1, float *vec2,int dim);
    void saveNet(char * name);
    void loadNet(char * name);
    protected:
    int NX,NY,NZ,NF;
    int NeuronCount;
    KNeuron **KNField;
    float AbsoluteLimit,RelativeMin,RelativeMax;

};
#endif

#ifndef PK_EB_KOHONEN_H

#define PK_EB_KOHONEN_H

#include"EB_Kohonen.h"

class PK_EB_Kohonen:public EB_Kohonen
{
  public:
    PK_EB_Kohonen (int dimz);
      PK_EB_Kohonen (const PK_EB_Kohonen & other);
     ~PK_EB_Kohonen (void)
    {
    };
    void InitializeWeights (float *weights, int dimf, float absolute, float relativemin, float relativemax, float lastkomponent);
};

#endif

/**************************************************************************\
**
**  tStreamNet.h: Header file for class tStreamNet.
**
**  tStreamNet objects contain data and functions related to flow routing
**  and sediment transport across the landscape surface.
**
**  $Id: tStreamNet.h,v 1.2 1998-01-15 19:37:09 gtucker Exp $
\**************************************************************************/
#ifndef TSTREAMNET_H
#define TSTREAMNET_H

#include "../Erosion/erosion.h"
 
/** class tStreamNet *********************************************************/
class tStreamNet
{
   friend class tStreamTransport;
   friend class tStreamMeander;
public:
   tStreamNet();
   tStreamNet( tGrid< tLNode > &, tStorm &, tInputFile & );
   ~tStreamNet();
   void ResetGrid( tGrid< tLNode > & );
   const tGrid< tLNode > *getGridPtr() const;
   tGrid< tLNode > *getGridPtrNC();
   int getFlowGenOpt() const;
   int getFillLakesOpt() const;
   float getRainRate() const;
   float getTransmissivity() const;
   float getInfilt() const;
   void setFlowGenOpt( int );
   void setFillLakesOpt( int );
   void setRainRate( float );
   void setTransmissivity( float );
   void setInfilt( float );
   void UpdateNet();
   void UpdateNet( tStorm & );
   void CalcSlopes();
   void CalcVAreas();
   void InitFlowDirs();
   void FlowDirs();
   void DrainAreaVoronoi();
   float VoronoiArea( tLNode * );
   void RouteFlowArea( tLNode *, float );
   void SetVoronoiVertices();
   void MakeFlow();
   void FlowUniform();
   void FlowSaturated();
   void FillLakes();
   int FindLakeNodeOutlet( tLNode * );
   void SortNodesByNetOrder();
   void ErodeDetachLim( float dtg );
   
protected:
   
   tGrid< tLNode > * gridPtr;
   int flowgen;
   int filllakes;
   float rainrate;
   float trans;
   float infilt;    
   tBedErodePwrLaw bedErode;
};

#endif
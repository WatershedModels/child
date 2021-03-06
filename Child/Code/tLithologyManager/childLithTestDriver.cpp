/**************************************************************************/
/**
**  childDriver.cpp: This provides a test and example of the CHILD
**  interface.
**
**  The variation "childTestDriver" is used to test new additions,
**  especially to the childInterface.
**
**  Apr 2010
**
**  For information regarding this program, please contact Greg Tucker at:
**
**     Cooperative Institute for Research in Environmental Sciences (CIRES)
**     and Department of Geological Sciences
**     University of Colorado
**     2200 Colorado Avenue, Campus Box 399
**     Boulder, CO 80309-0399
**
*/
/**************************************************************************/

#include "../ChildInterface/childInterface.h"
#include "tLithologyManager.h"

int main( int argc, char **argv )
{
	childInterface myChildInterface;
  tLithologyManager my_lith_mgr;
  tOption option( argc, argv );
  tInputFile infile( option.inputFile );
	
	myChildInterface.Initialize( argc, argv );
  
	std::cout << "CHILD has finished initializing. Now for some tests.\n\n";
	
	std::cout << "Testing GetNodeCount(): this run has " 
	          << myChildInterface.GetNodeCount() << " nodes.\n";

	std::cout << "Testing GetNodeCoords() and GetValueSet() with 'elevation' and 'discharge':\n";
	std::cout << "The coordinates and properties of these nodes are:\n" 
	          << "(note that the two z columns should be the same)\n"
	          << "x\ty\tz\tz\tQ\tQs\n";
	int nn = myChildInterface.GetNodeCount();
	std::vector<double> coords = myChildInterface.GetNodeCoords();
	std::vector<double> elevs = myChildInterface.GetValueSet( "elevation" );
	std::vector<double> q = myChildInterface.GetValueSet( "discharge" );
	std::vector<double> qs = myChildInterface.GetValueSet( "sedflux" );
	for( long i=0; i<nn; i++ )
	{
	   std::cout << coords[3*i] << "\t" 
	             << coords[3*i+1] << "\t" << coords[3*i+2];
	   std::cout << "\t" << elevs[i]
	             << "\t" << q[i]
	             << "\t" << qs[i] << std::endl;
	}

	std::cout << "Testing GetTriangleCount(): this run has " 
	          << myChildInterface.GetTriangleCount() << " triangles.\n";

	std::cout << "Testing GetTriangleVertexIDs(): the vertex IDs of these triangles are:\n" 
	          << "Tri ID\tp0\tp1\tp2\n";
	int nt = myChildInterface.GetTriangleCount();
	std::vector<long> vertices = myChildInterface.GetTriangleVertexIDs();
	for( long i=0; i<nt; i++ )
	{
	   std::cout << i << "\t";
	   std::cout << vertices[3*i] << "\t";
	   std::cout << vertices[3*i+1] << "\t" 
	             << vertices[3*i+2] << std::endl;
	}

  // Now some tests on the lithology manager: 
  // First, initialize it.
  my_lith_mgr.InitializeFromInputFile( infile, myChildInterface.GetMeshPointer() );
	
  
  // Now, see if we can re-set the rock erodibility at each node
  std::vector<double> erody( coords.size() );
  for( long i=0; i<nn; i++ )
    if( coords[3*i] > 4000.0 && coords[3*i] < 6000.0 )
      erody[i] = 500.0;
    else
      erody[i] = 1.0e-6;
  my_lith_mgr.SetRockErodibilityValuesAtAllDepths( erody );
  
  // Test the point in polygon routine
  std::vector<double> polyx( 4 );
  std::vector<double> polyy( 4 );
  polyx[0] = 2.0;
  polyx[1] = 4.0;
  polyx[2] = 4.0;
  polyx[3] = 2.0;
  polyy[0] = 2.0;
  polyy[1] = 2.0;
  polyy[2] = 4.0;
  polyy[3] = 4.0;
  std::cout << "This should be false: " << my_lith_mgr.PointInPolygon( polyx, polyy, 1.0, 3.0 ) << std::endl;
  std::cout << "This should be false: " << my_lith_mgr.PointInPolygon( polyx, polyy, 3.0, 5.0 ) << std::endl;
  std::cout << "This should be false: " << my_lith_mgr.PointInPolygon( polyx, polyy, 5.0, 3.0 ) << std::endl;
  std::cout << "This should be false: " << my_lith_mgr.PointInPolygon( polyx, polyy, 3.0, 1.0 ) << std::endl;
  std::cout << "This should be true: " << my_lith_mgr.PointInPolygon( polyx, polyy, 3.0, 3.0 ) << std::endl;
  
  // Now back to the main attraction ... running the model!
    
  myChildInterface.Run( 0 );
	
	std::cout << "CHILD run has finished. Now re-run the tests.\n\n";

	std::cout << "Testing GetNodeCount(): this run has " 
	          << myChildInterface.GetNodeCount() << " nodes.\n";

	std::cout << "Testing GetNodeCoords() and GetValueSet() with 'elevation' and 'discharge':\n";
	std::cout << "The coordinates and properties of these nodes are:\n" 
	          << "(note that the two z columns should be the same)\n"
	          << "x\ty\tz\tz\tQ\tQs\n";
	nn = myChildInterface.GetNodeCount();
	coords = myChildInterface.GetNodeCoords();
	elevs = myChildInterface.GetValueSet( "elevation" );
	q = myChildInterface.GetValueSet( "discharge" );
	qs = myChildInterface.GetValueSet( "sedflux" );
	for( long i=0; i<nn; i++ )
	{
	   std::cout << coords[3*i] << "\t" 
	             << coords[3*i+1] << "\t" << coords[3*i+2];
	   std::cout << "\t" << elevs[i]
	             << "\t" << q[i] << "\t" << qs[i] << std::endl;
	}

	std::cout << "Testing GetTriangleCount(): this run has " 
	          << myChildInterface.GetTriangleCount() << " triangles.\n";

	std::cout << "Testing GetTriangleVertexIDs(): the vertex IDs of these triangles are:\n" 
	          << "Tri ID\tp0\tp1\tp2\n";
	nt = myChildInterface.GetTriangleCount();
	vertices = myChildInterface.GetTriangleVertexIDs();
	for( long i=0; i<nt; i++ )
	{
	   std::cout << i << "\t";
	   std::cout << vertices[3*i] << "\t";
	   std::cout << vertices[3*i+1] << "\t" 
	             << vertices[3*i+2] << std::endl;
	}
  
  std::cout << "\nTest of AdjustElevations() and AdjustInteriorElevations:\n\n";
  std::vector<double> dz( nn, 1.0 );
  myChildInterface.AdjustInteriorElevations( dz );
	std::vector<double> new_elevs = myChildInterface.GetValueSet( "elevation" );
  std::cout << "The interior nodes should now be 1m higher, but not the boundary nodes:\n";
  std::cout << "ID\tOld z\tNew z:\n";
	for( long i=0; i<nt; i++ )
	{
	   std::cout << i << "\t";
	   std::cout << elevs[i] << "\t" << new_elevs[i] << std::endl;
	}
  myChildInterface.AdjustElevations( dz );
	new_elevs = myChildInterface.GetValueSet( "elevation" );
  std::cout << "Now all nodes should be another 1m higher:\n";
  std::cout << "ID\tOriginal z\tNew z:\n";
	for( long i=0; i<nt; i++ )
	{
	   std::cout << i << "\t";
	   std::cout << elevs[i] << "\t" << new_elevs[i] << std::endl;
	}
  
  // Now we'll test ExternalErodeAndDepositToElevation
  elevs = new_elevs;
  std::vector<double> desired_elevs( nn, 2.5 );
  myChildInterface.ExternalErodeAndDepositToElevation( desired_elevs );
	new_elevs = myChildInterface.GetValueSet( "elevation" );
  std::cout << "Now all interior nodes should be 2.5m elevation\n";
  std::cout << "ID\tPrevious z\tNew z:\n";
	for( long i=0; i<nt; i++ )
	{
	   std::cout << i << "\t";
	   std::cout << elevs[i] << "\t" << new_elevs[i] << std::endl;
	}

  desired_elevs = elevs;
  myChildInterface.ExternalErodeAndDepositToElevation( elevs );
	new_elevs = myChildInterface.GetValueSet( "elevation" );
  std::cout << "Now they should be back to their previous values\n";
  std::cout << "ID\tPrevious z\tNew z:\n";
	for( long i=0; i<nt; i++ )
	{
	   std::cout << i << "\t";
	   std::cout << new_elevs[i] << std::endl;
	}
  
	// Note that calling CleanUp() isn't strictly necessary, as the destructor will automatically clean it
	// up when myChildInterface is deleted ... but it's nice to be able to do this at will (and free up
	// memory)
	myChildInterface.CleanUp();
	
	return 0;
}

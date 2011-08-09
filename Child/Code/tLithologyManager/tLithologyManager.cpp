//-*-c++-*-

/**************************************************************************/
/**
 **  @file tLithologyManager.cpp
 **
 **  @brief Implementation of the tLithologyManager class.
 **
 **  For information regarding this program, please contact Greg Tucker at:
 **
 **     Cooperative Institute for Research in Environmental Sciences (CIRES)
 **     and Department of Geological Sciences
 **     University of Colorado
 **     2200 Colorado Avenue, Campus Box 399
 **     Boulder, CO 80309-0399
 */
/**************************************************************************/

#include <vector>
#include <sstream>
#include "tLithologyManager.h"

using namespace std;

/**************************************************************************/
/**
 **  Basic constructor
 **
 **  (does nothing at the moment)
 */
/**************************************************************************/
tLithologyManager::
tLithologyManager()
{
  if(1) cout << "tLithologyManager default constructor" << endl;
}


/**************************************************************************/
/**
 **  Destructor
 **
 **  Closes and deletes any remaining ofstream objects.
 */
/**************************************************************************/
tLithologyManager::
~tLithologyManager()
{
  if(1) cout << "tLithologyManager destructor" << endl;
}


/**************************************************************************/
/**
 **  InitializeFromInputFile
 **
 */
/**************************************************************************/
void tLithologyManager::
InitializeFromInputFile( tInputFile &inputFile, tMesh<tLNode> *meshPtr )
{
  if(1) cout << "tLithologyManager::InitializeFromInputFile" << endl;
  
  meshPtr_ = meshPtr;
  
  // Read from the input file and do what the user requests
  
  /* Notes:
  
  Things we ought to handle include:
  - read from a restart run
  - read from another CHILD-format lith file with given name
  - set lithology according to a strat sequence of uniform thicknesses
    - with or without a uniform or varying regolith layer on top
  - set lithology according to a map pattern (uniform in depth)
    - with or without a uniform or varying regolith layer on top
  - set regolith thickness according to a map pattern
  - set to simple "bedrock and regolith" (current default)
  - ?? set according to a set of "contact surfaces" that represent the lower
    boundary of a unit
  
  */
	
	// See if the user wants to start with an existing .lay file. If so, read it
	// and set layers accordingly.
	bool user_wants_to_read_from_layfile = inputFile.ReadBool( "OPT_READ_LAYFILE", false );
	if( user_wants_to_read_from_layfile )
	{
		SetLithologyFromChildLayFile( inputFile );
	}
  
}


/**************************************************************************/
/**
 **  SetLithologyFromChildLayFile
 **
 */
/**************************************************************************/
void tLithologyManager::
SetLithologyFromChildLayFile( const tInputFile &infile )
{
  int i, item, numl;
  int righttime, numlayernodes;
  double time, intime;
  double ditem;
  char thestring[80], inname[80];
  char headerLine[kMaxNameLength];
  std::ifstream layerinfile;
  infile.ReadItem( thestring, sizeof(thestring), "INPUT_LAY_FILE" );
  
  if (1) //DEBUG
    std::cout<<"in ReadLayersFromLayFile..."<<std::endl;
  
  strcpy( inname, thestring );
  layerinfile.open(inname); /* Layer input file pointer */
  assert( layerinfile.good() );
  
  intime = infile.ReadDouble( "INPUTTIME", false );

  // Read first line, which should contain the time corresponding to the run
  layerinfile >> time;

  if(1) //DEBUG
    std::cout<<"Time="<<time<<std::endl;
  
  // Read second line, which specified number of interior nodes in file.
  layerinfile >> numlayernodes;
  if(1) //DEBUG
    std::cout<<"nnodes in file="<<numlayernodes<<std::endl;
  if( numlayernodes != meshPtr_->getNodeList()->getActiveSize() )
    ReportFatalError( "Number of nodes in layer file doesn't match number in mesh" );
  
  // Create and initialize a tLayer object that will serve as the basic template
  // for all layers. Set its # of grain size classes.
  tLayer layer_template;
  int numg;
  numg = infile.ReadInt( "NUMGRNSIZE" );
  layer_template.setDgradesize(numg);
  
  // Get an iterator for the node list
  tLNode * cn;
  tMesh<tLNode>::nodeListIter_t ni( meshPtr_->getNodeList() );
    // Hack: make layers input backwards compatible for simulations
    // without bulk density; :
  bool optNewLayersInput = infile.ReadBool( "OPT_NEW_LAYERSINPUT", false );
  
  for( cn = ni.FirstP(); ni.IsActive(); cn = ni.NextP() )
  {
    // Read the number of layers at the current node
    layerinfile >> numl;
    if( numl<1 )
    {
      std::cout << "In ReadLayersFromLayFile: node " 
      << cn->getPermID() << " has " << numl << " layers." 
      << std::endl;
      ReportFatalError( "Each interior point must have at least one layer." );
    }
    
    // For each layer, read its properties, assign them to our template layer,
    // and insert a copy of that layer to the bottom of the layer list for this
    // node.
    for(i = 1; i<=numl; i++)
    {
      // Read layer properties
      layerinfile >> ditem;     // layer creation time
      layer_template.setCtime(ditem);
      layerinfile >> ditem;     // layer "recent activity" time
      layer_template.setRtime(ditem);
      layerinfile >> ditem;     // layer surface exposure duration
      layer_template.setEtime(ditem);
      layerinfile >> ditem;     // layer thickness (m)
      if( ditem <= 0. )
      {
        std::cout << "MakeLayersFromInputData: Layer " << i 
        << " at node " << cn->getID()
        << " has thickness " << ditem << std::endl;
        ReportFatalError("Layers must have positive thickness.");
      }
      layer_template.setDepth(ditem);
      layerinfile >> ditem;     // layer erodibility
      layer_template.setErody(ditem);
        // Hack: to make this backwards compatible,
        // read bulk density if optNewLayersInput == true:
      if( optNewLayersInput )
      {
        layerinfile >> ditem;   // layer bulk density
        layer_template.setBulkDensity(ditem);
      }
      layerinfile >> item;      // layer alluvial-bedrock flag
      if( item<0 || item>1 )
      {
        std::cout << "MakeLayersFromInputData: Layer " << i 
        << " at node " << cn->getID()
        << " has flag value " << item << std::endl;
        ReportFatalError("Flag value must be either zero or one.");
      }
      {
        tLayer::tSed_t item_ = static_cast<tLayer::tSed_t>(item);
        layer_template.setSed(item_);
      }
      for(int g=0; g<numg; g++){
        layerinfile >> ditem;   // equivalent thickness of grain size g
        layer_template.setDgrade(g, ditem);
      }
      
      // Insert a copy of the layer on the bottom of the layer stack
      cn->InsertLayerBack( layer_template );
    }
    
  }
    
}

    

/**************************************************************************/
/**
 */
/**************************************************************************/

/**************************************************************************/
/**
 */
/**************************************************************************/

/**************************************************************************/
/**
 */
/**************************************************************************/

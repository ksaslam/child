/*************************************************************************\
**
**  tOutput.cpp: Functions for output objects
**
**  
**
**  $Id: tOutput.cpp,v 1.2 1998-01-27 23:42:14 gtucker Exp $
\*************************************************************************/

#include <string.h>
#include <assert.h>
#include "../errors/errors.h"
#include "../tGridList/tGridList.h"
#include "../GridElements/gridElements.h"
#include "tOutput.h"

/*************************************************************************\
**
**  Constructor
**
**  The constructor takes two arguments, a pointer to the grid mesh and
**  a reference to an open input file. It reads the base name for the
**  output files from the input file, and opens and initializes these.
**
**  Input: gridPtr -- pointer to a tGrid object (or descendant), assumed
**                    valid
**         infile -- reference to an open input file, assumed valid
**
\*************************************************************************/
template< class tSubNode >
tOutput<tSubNode>::tOutput( tGrid<tSubNode> * gridPtr, tInputFile &infile )
{
   char fullName[kMaxNameSize+6];
   
   assert( gridPtr > 0 );
   g = gridPtr;

   infile.ReadItem( baseName, "OUTFILENAME" );
   strcpy( fullName, baseName );
   strcat( fullName, ".nodes" );
   nodeofs.open( fullName );
   strcpy( fullName, baseName );
   strcat( fullName, ".edges" );
   edgofs.open( fullName );
   strcpy( fullName, baseName );
   strcat( fullName, ".tri" );
   triofs.open( fullName );
   strcpy( fullName, baseName );
   strcat( fullName, ".z" );
   zofs.open( fullName );

   if( !nodeofs.good() || !edgofs.good() || !triofs.good() || !zofs.good() )
       ReportFatalError(
           "I can't create files for output. Memory may be exhausted." );
   
}



/*************************************************************************\
**
**  WriteOutput
**
**  This function writes information about the mesh to four files called
**  name.nodes, name.edges, name.tri, and name.z, where "name" is a
**  name that the user has specified in the input file and which is
**  stored in the data member baseName.
**
**  Input: time -- time of the current output time-slice
**  Output: the node, edge, and triangle ID numbers are modified so that
**          they are numbered according to their position on the list
**  Assumes: the four file ofstreams have been opened by the constructor
**           and are valid
**
**  TODO: deal with option for once-only printing of mesh when mesh not
**        deforming
\*************************************************************************/
template< class tSubNode >
void tOutput<tSubNode>::WriteOutput( float time )
{
   tGridListIter<tSubNode> niter( g->GetNodeList() );
   tGridListIter<tEdge> eiter( g->GetEdgeList() );
   tListIter<tTriangle> titer( g->GetTriList() );
   tNode * cn;
   tEdge * ce;
   tTriangle * ct;
   int id;
   int nnodes = g->GetNodeList()->getSize();
   int nedges = g->GetEdgeList()->getSize();
   int ntri = g->GetTriList()->getSize();

   cout << "tOutput::WriteOutput()\n";
   
   // Renumber IDs in order by position on list
   for( cn=niter.FirstP(), id=0; id<nnodes; cn=niter.NextP(), id++ )
       cn->setID( id );
   for( ce=eiter.FirstP(), id=0; id<nedges; ce=eiter.NextP(), id++ )
       ce->setID( id );
   for( ct=titer.FirstP(), id=0; id<ntri; ct=titer.NextP(), id++ )
       ct->setID( id );

   // Write node file and z file
   nodeofs << " " << time << endl << nnodes << endl;
   zofs << " " << time << endl << nnodes << endl;
   for( cn=niter.FirstP(); !(niter.AtEnd()); cn=niter.NextP() )
   {
      nodeofs << cn->getX() << " " << cn->getY() << " "
              << cn->GetEdg()->getID() << " " << cn->getBoundaryFlag() << endl;
      zofs << cn->getZ() << endl;
   }
   
   // Write edge file
   edgofs << " " << time << endl << nedges << endl;
   for( ce=eiter.FirstP(); !(eiter.AtEnd()); ce=eiter.NextP() )
      edgofs << ce->getOriginPtrNC()->getID() << " "
             << ce->getDestinationPtrNC()->getID() << " "
             << ce->GetCCWEdg()->getID() << endl;
   
   // Write triangle file
   int i;
   triofs << " " << time << endl << ntri << endl;
   for( ct=titer.FirstP(); !(titer.AtEnd()); ct=titer.NextP() )
   {
      for( i=0; i<=2; i++ )
          triofs << ct->pPtr(i)->getID() << " ";
      for( i=0; i<=2; i++ )
      {
          if( ct->tPtr(i) ) triofs << ct->tPtr(i)->getID() << " ";
          else triofs << "-1 ";
      }
      triofs << ct->ePtr(0)->getID() << " " 
             << ct->ePtr(1)->getID() << " " 
             << ct->ePtr(2)->getID() << endl;
   }
   
   WriteNodeData( time );
   
}

template< class tSubNode >
void tOutput<tSubNode>::WriteNodeData( float time ) 
{}


template< class tSubNode >
tLOutput<tSubNode>::tLOutput( tGrid<tSubNode> *g, tInputFile &infile ) 
        : tOutput<tSubNode>( g, infile )  // call base-class constructor
{
   char fullName[kMaxNameSize+6];

   strcpy( fullName, baseName );
   strcat( fullName, ".area" );
   drareaofs.open( fullName );
   strcpy( fullName, baseName );
   strcat( fullName, ".net" );
   netofs.open( fullName );

   if( !drareaofs.good() || !netofs.good() )
       ReportFatalError(
           "Unable to open output file. Storage space may be exhausted." );
   
}



template< class tSubNode >
void tLOutput<tSubNode>::WriteNodeData( float time )
{
   tGridListIter<tSubNode> ni( g->GetNodeList() );
   tSubNode *cn;
   int nActiveNodes = g->GetNodeList()->getActiveSize();
   
   drareaofs << " " << time << "\n " << nActiveNodes << endl;
   netofs << " " << time << "\n " << nActiveNodes << endl;
   for( cn = ni.FirstP(); ni.IsActive(); cn = ni.NextP() )
   {
      assert( cn>0 );
      drareaofs << cn->getDrArea() << endl;
      netofs << cn->GetDownstrmNbr()->getID() << endl;
   }
   
}
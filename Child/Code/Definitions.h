/****************************************************************************\
**   Header file for defining global constants
**   $Id: Definitions.h,v 1.2 1998-01-16 22:07:24 stlancas Exp $
\****************************************************************************/

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/** DEFINITIONS *************************************************************/
#define kMaxNameLength 80
#define kCommentMark '#'
#define kUniformGrid 0     /*method of grid construction*/
#define kPerturbedGrid 1
#define kRandomGrid 2
#define kCornerOutlet 0   /*type of open boundary*/
#define kOpenSide 1
#define kOppositeSidesOpen 2
#define kAllSidesOpen 3
#define kSpecifyOutlet 4
#define kClosedBoundary 1
#define kOpenBoundary 2
#define kNonBoundary 0
#define kFlowAllowed 1
#define kFlowNotAllowed 0
#define kSaturatedFlow 1   // Option for saturation-excess flow generation
#define kDetachmentLimited      1
#define kDetachLimThreshold     2
#define kTransportLimited       3
#define kTransLimThreshold      4
#define kBedrockAlluvial        5
#define kFlooded     1  // Flooding (lake) codes: part of a lake...
#define kNotFlooded  0  // ...or not...
#define kCurrentLake 2  // ...or part one that is currently being computed...
#define kSink        3  // ...or a dry sink (unfilled depression).
#define kOutletFlag  4  // Used as temporary flag in FillLakes.
#define kOutletPreFlag 5 // ditto
#define kMeanderNode 1
#define kNonMeanderNode 0
#define kVeryHigh 100000  // Used in FillLakes
#define RHO 1000.0      /* Density of water (kg/m^3) */
#define RHOSED 2650. /* density of sediment, [kg/m^3] */
#define GRAV 9.81       /* Gravitational acceleration, m/s^2 */
#define POROSITY 0.3        /* porosity of sediment on bed */
#define VISC .00000112      /* viscosity of water [m^2/s] */
#define ACTDEPTH 3.0   /* depth of active layer [m] */
#define DEPDEPTH 1.0   /* maximum depth of layers below active layer [m] */
#define NUMG 2         /* number of grain sizes */

// Macros
#define ROUND(x)    (int)(x+0.5)
#define SIGN(x)     ( x>0 ? 1 : 0 )

// Redefine floats to be double precision (8 byte) for compatibility
// with meandering routine
//#define float double

#endif
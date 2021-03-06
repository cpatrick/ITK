/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkQuadrilateralCell.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkQuadrilateralCell_txx
#define __itkQuadrilateralCell_txx
#include "itkQuadrilateralCell.h"

namespace itk
{

/**
 * Standard CellInterface:
 */
template <typename TCellInterface>
void
QuadrilateralCell< TCellInterface >
::MakeCopy(CellAutoPointer & cellPointer) const
{
  cellPointer.TakeOwnership( new Self );
  cellPointer->SetPointIds(this->GetPointIds());
}

  
/**
 * Standard CellInterface:
 * Get the topological dimension of this cell.
 */
template <typename TCellInterface>
unsigned int
QuadrilateralCell< TCellInterface >
::GetDimension(void) const
{
  return Self::CellDimension;
}


/**
 * Standard CellInterface:
 * Get the number of points required to define the cell.
 */
template <typename TCellInterface>
unsigned int
QuadrilateralCell< TCellInterface >
::GetNumberOfPoints(void) const
{
  return Self::NumberOfPoints;
}  


/**
 * Standard CellInterface:
 * Get the number of boundary features of the given dimension.
 */
template <typename TCellInterface>
typename QuadrilateralCell< TCellInterface >::CellFeatureCount
QuadrilateralCell< TCellInterface >
::GetNumberOfBoundaryFeatures(int dimension) const
{
  switch (dimension)
    {
    case 0: return GetNumberOfVertices();
    case 1: return GetNumberOfEdges();
    default: return 0;
    }
}


/**
 * Standard CellInterface:
 * Get the boundary feature of the given dimension specified by the given
 * cell feature Id.
 * The Id can range from 0 to GetNumberOfBoundaryFeatures(dimension)-1.
 */
template <typename TCellInterface>
bool
QuadrilateralCell< TCellInterface >
::GetBoundaryFeature(int dimension, CellFeatureIdentifier featureId,CellAutoPointer & cellPointer)
{
  switch (dimension)
    {
    case 0: 
      {
      VertexAutoPointer vertexPointer;
      if( this->GetVertex(featureId,vertexPointer) )
        {
        TransferAutoPointer(cellPointer,vertexPointer);
        return true;
        }
      else
        {
        cellPointer.Reset();
        return false;
        }
      break;
      }
    case 1: 
      {
      EdgeAutoPointer edgePointer;
      if( this->GetEdge(featureId,edgePointer) )
        {
        TransferAutoPointer(cellPointer,edgePointer);
        return true;
        }
      else
        {
        cellPointer.Reset();
        return false;
        }
      break;
      }

    default: 
      {
      cellPointer.Reset();
      return false;
      }
    }
  return false;
}

/**
 * Standard CellInterface:
 * Set the point id list used by the cell.  It is assumed that the given
 * iterator can be incremented and safely de-referenced enough times to 
 * get all the point ids needed by the cell.
 */
template <typename TCellInterface>
void
QuadrilateralCell< TCellInterface >
::SetPointIds(PointIdConstIterator first)
{
  PointIdConstIterator ii(first);
  for(unsigned int i=0; i < Self::NumberOfPoints; ++i)
    {
    m_PointIds[i] = *ii++;
    }
}


/**
 * Standard CellInterface:
 * Set the point id list used by the cell.  It is assumed that the range
 * of iterators [first, last) contains the correct number of points needed to
 * define the cell.  The position *last is NOT referenced, so it can safely
 * be one beyond the end of an array or other container.
 */
template <typename TCellInterface>
void
QuadrilateralCell< TCellInterface >
::SetPointIds(PointIdConstIterator first, PointIdConstIterator last)
{
  int localId=0;
  PointIdConstIterator ii(first);
  
  while(ii != last)
    {
    m_PointIds[localId++] = *ii++;
    }
}


/**
 * Standard CellInterface:
 * Set an individual point identifier in the cell.
 */
template <typename TCellInterface>
void
QuadrilateralCell< TCellInterface >
::SetPointId(int localId, PointIdentifier ptId)
{
  m_PointIds[localId] = ptId;
}


/**
 * Standard CellInterface:
 * Get a begin iterator to the list of point identifiers used by the cell.
 */
template <typename TCellInterface >
typename QuadrilateralCell< TCellInterface >::PointIdIterator
QuadrilateralCell< TCellInterface >
::PointIdsBegin(void)
{
  return &m_PointIds[0];
}


/**
 * Standard CellInterface:
 * Get a const begin iterator to the list of point identifiers used
 * by the cell.
 */
template <typename TCellInterface>
typename QuadrilateralCell< TCellInterface >::PointIdConstIterator
QuadrilateralCell< TCellInterface >
::PointIdsBegin(void) const
{
  return &m_PointIds[0];
}


/**
 * Standard CellInterface:
 * Get an end iterator to the list of point identifiers used by the cell.
 */
template <typename TCellInterface>
typename QuadrilateralCell< TCellInterface >::PointIdIterator
QuadrilateralCell< TCellInterface >
::PointIdsEnd(void)
{
  return &m_PointIds[Self::NumberOfPoints-1] + 1;
}

/**
 * Standard CellInterface:
 * Get a const end iterator to the list of point identifiers used
 * by the cell.
 */
template <typename TCellInterface >
typename QuadrilateralCell< TCellInterface >::PointIdConstIterator
QuadrilateralCell< TCellInterface >
::PointIdsEnd(void) const
{
  return &m_PointIds[Self::NumberOfPoints-1] + 1;
}

/**
 * Quadrilateral-specific:
 * Get the number of vertices defining the quadrilateral.
 */
template <typename TCellInterface>
typename QuadrilateralCell< TCellInterface >::CellFeatureCount
QuadrilateralCell< TCellInterface >
::GetNumberOfVertices(void) const
{
  return NumberOfVertices;
}

/**
 * Quadrilateral-specific:
 * Get the number of edges defined for the quadrilateral.
 */
template <typename TCellInterface>
typename QuadrilateralCell< TCellInterface >::CellFeatureCount
QuadrilateralCell< TCellInterface >
::GetNumberOfEdges(void) const
{
  return Self::NumberOfEdges;
}

/**
 * Quadrilateral-specific:
 * Get the vertex specified by the given cell feature Id.
 * The Id can range from 0 to GetNumberOfVertices()-1.
 */
template <typename TCellInterface>
bool
QuadrilateralCell< TCellInterface >
::GetVertex(CellFeatureIdentifier vertexId,VertexAutoPointer & vertexPointer )
{
  VertexType * vert = new VertexType;
  vert->SetPointId(0, m_PointIds[vertexId]);
  vertexPointer.TakeOwnership( vert );
  return true;  
}

/**
 * Quadrilateral-specific:
 * Get the edge specified by the given cell feature Id.
 * The Id can range from 0 to GetNumberOfEdges()-1.
 */
template <typename TCellInterface>
bool
QuadrilateralCell< TCellInterface >
::GetEdge(CellFeatureIdentifier edgeId, EdgeAutoPointer & edgePointer )
{
  EdgeType * edge = new EdgeType;
  for(int i=0; i < EdgeType::NumberOfPoints; ++i)
    {
    edge->SetPointId(i, m_PointIds[ m_Edges[edgeId][i] ]);
    }
  edgePointer.TakeOwnership( edge ); 
  return true;
}


} // end namespace itk

#endif

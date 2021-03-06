/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWarpHarmonicEnergyCalculator.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWarpHarmonicEnergyCalculator_txx
#define __itkWarpHarmonicEnergyCalculator_txx

#include "itkWarpHarmonicEnergyCalculator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkImageRegionIterator.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"

#include "vnl/vnl_matrix.h"
#include "vnl/vnl_math.h"

namespace itk
{

/**
 *   Constructor
 */
template<class TInputImage>
WarpHarmonicEnergyCalculator<TInputImage>
::WarpHarmonicEnergyCalculator()
{
  m_Image = TInputImage::New();
  m_HarmonicEnergy = 0.0;
  m_RegionSetByUser = false;
  unsigned int i;
  m_UseImageSpacing = true;
  for (i = 0; i < ImageDimension; i++)
    {
    m_NeighborhoodRadius[i] = 1; // radius of neighborhood we will use
    m_DerivativeWeights[i] = 1.0;
    }
}

template <class TInputImage>
void
WarpHarmonicEnergyCalculator<TInputImage>
::SetDerivativeWeights(double data[])
{
  m_UseImageSpacing = false;

  for (unsigned int i = 0; i < ImageDimension; ++i)
    {
    if (m_DerivativeWeights[i] != data[i])
      {
      this->Modified();
      m_DerivativeWeights[i] = data[i];
      }
    }
}

template <class TInputImage>
void
WarpHarmonicEnergyCalculator<TInputImage>
::SetUseImageSpacing(bool f)
{
  if (m_UseImageSpacing == f)
    {
    return;
    }

  // Only reset the weights if they were previously set to the image spacing,
  // otherwise, the user may have provided their own weightings.
  if (f == false && m_UseImageSpacing == true)
    {
    for (unsigned int i = 0; i < ImageDimension; ++i)
      {
      m_DerivativeWeights[i] = 1.0;
      }
    }

  m_UseImageSpacing = f;
}


/*
 * Compute
 */
template<class TInputImage>
void
WarpHarmonicEnergyCalculator<TInputImage>
::Compute(void)
{
  if( !m_RegionSetByUser )
    {
    m_Region = m_Image->GetRequestedRegion();
    }

  // Set the weights on the derivatives.
  // Are we using image spacing in the calculations?  If so we must update now
  // in case our input image has changed.
  if (m_UseImageSpacing == true)
    {

    for (unsigned int i = 0; i < ImageDimension; i++)
      {
      if (m_Image->GetSpacing()[i] <= 0.0)
        {
        itkExceptionMacro(<< "Image spacing in dimension " << i << " is zero.");
        }
      m_DerivativeWeights[i] = 1.0 / static_cast<double>(m_Image->GetSpacing()[i]);
      }
    }

  m_HarmonicEnergy = 0.0;

  ZeroFluxNeumannBoundaryCondition<ImageType> nbc;
  ConstNeighborhoodIteratorType bit;

  // Find the data-set boundary "faces"
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<ImageType>::
    FaceListType faceList;
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<ImageType> bC;
  faceList = bC(m_Image, m_Region, m_NeighborhoodRadius);

  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<ImageType>::
    FaceListType::iterator fit;
  fit = faceList.begin();

  // Process each of the data set faces.  The iterator is reinitialized on each
  // face so that it can determine whether or not to check for boundary
  // conditions.
  for (fit=faceList.begin(); fit != faceList.end(); ++fit)
    {
    bit = ConstNeighborhoodIteratorType(m_NeighborhoodRadius,
                                        m_Image,
                                        *fit);
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();

    while ( ! bit.IsAtEnd() )
      {
      m_HarmonicEnergy += this->EvaluateAtNeighborhood(bit);
      ++bit;
      }
    }

  m_HarmonicEnergy /= m_Region.GetNumberOfPixels();
}


template <class TInputImage>
double
WarpHarmonicEnergyCalculator<TInputImage>
::EvaluateAtNeighborhood(ConstNeighborhoodIteratorType &it) const
{
  // Simple method using field derivatives

  unsigned int i, j;
  vnl_matrix_fixed<double,ImageDimension,VectorDimension> J;

  PixelType next, prev;

  double weight;

  for (i = 0; i < ImageDimension; ++i)
    {
    next = it.GetNext(i);
    prev = it.GetPrevious(i);

    weight = 0.5*m_DerivativeWeights[i];

    for (j = 0; j < VectorDimension; ++j)
      {
      J[i][j]=weight*(static_cast<double>(next[j])-static_cast<double>(prev[j]));
      }

    // add one on the diagonal to consider the warping and not only the deformation field
    //J[i][i] += 1.0;
    }

  const double norm = J.fro_norm();
  return norm*norm;
}


template<class TInputImage>
void
WarpHarmonicEnergyCalculator<TInputImage>
::SetRegion( const RegionType & region )
{
  m_Region = region;
  m_RegionSetByUser = true;
}


template<class TInputImage>
void
WarpHarmonicEnergyCalculator<TInputImage>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "HarmonicEnergy: "<<m_HarmonicEnergy<< std::endl;
  os << indent << "Image: " << std::endl;
  m_Image->Print(os, indent.GetNextIndent());
  os << indent << "Region: " << std::endl;
  m_Region.Print(os,indent.GetNextIndent());
  os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
  os << indent << "Use image spacing: " << this->m_UseImageSpacing << std::endl;
  os << indent << "Derivative Weights: " << this->m_DerivativeWeights << std::endl;
  os << indent << "Neighborhood Radius: " << this->m_NeighborhoodRadius << std::endl;
}

} // end namespace itk

#endif

/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrap_itkImageToImageMetric.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImage.h"
#include "itkImageToImageMetric.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkImageToImageMetric);
  namespace wrappers
  {
    ITK_WRAP_OBJECT2(ImageToImageMetric, image::F2, image::F2,
                     itkImageToImageMetricF2F2);
    ITK_WRAP_OBJECT2(ImageToImageMetric, image::F3, image::F3,
                     itkImageToImageMetricF3F3);
    ITK_WRAP_OBJECT2(ImageToImageMetric, image::US2, image::US2,
                     itkImageToImageMetricUS2US2);
    ITK_WRAP_OBJECT2(ImageToImageMetric, image::US3, image::US3,
                     itkImageToImageMetricUS3US3);
  }
}

#endif

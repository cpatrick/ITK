/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkAutoCropLabelMapFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAutoCropLabelMapFilter_h
#define __itkAutoCropLabelMapFilter_h

#include "itkChangeRegionLabelMapFilter.h"

namespace itk
{
  
/** \class AutoCropLabelMapFilter
 * \brief Crop a LabelMap image to fit exactly the objects in the LabelMap.
 *
 * The CropBorder can be used to add a border which will never be larger than
 * the input image. To add a border of size independent of the input image,
 * PadLabelMapFilter can be used.
 *
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/1926/584  or 
 * http://www.insight-journal.org/browse/publication/176
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa PadLabelMapFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template <class TInputImage>
class ITK_EXPORT AutoCropLabelMapFilter :
    public ChangeRegionLabelMapFilter<TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef AutoCropLabelMapFilter                   Self;
  typedef ChangeRegionLabelMapFilter<TInputImage>  Superclass;
  typedef SmartPointer<Self>                       Pointer;
  typedef SmartPointer<const Self>                 ConstPointer;
  
  /** Standard New method. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(AutoCropLabelMapFilter, ChangeRegionImageFilter);

  /** Superclass typedefs. */
  typedef typename Superclass::OutputImageType       OutputImageType;
  typedef typename Superclass::OutputImagePointer    OutputImagePointer;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType  OutputImagePixelType;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType; 
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename InputImageType::LabelObjectType LabelObjectType;

  typedef typename InputImageType::PixelType       PixelType;
  typedef typename InputImageType::IndexType       IndexType;
  typedef typename InputImageType::SizeType        SizeType;
  typedef typename InputImageType::RegionType      RegionType;
  
  typedef TInputImage TOutputImage;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int, TOutputImage::ImageDimension);


  /**
   * Set/Get the border added to the mask before the crop. The default is 0 on * all the axis.
   */
  itkSetMacro(CropBorder, SizeType);
  itkGetConstReferenceMacro(CropBorder, SizeType);
  
  itkGetConstReferenceMacro(MinIndex, IndexType);
  itkGetConstReferenceMacro(MaxIndex, IndexType);

protected:
  AutoCropLabelMapFilter();
  ~AutoCropLabelMapFilter() {};

  void PrintSelf(std::ostream& os, Indent indent) const;

  virtual void GenerateOutputInformation();

private:
  AutoCropLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  void FindBoundingBox();
  void SetAndPadCropRegion();

  SizeType      m_CropBorder;
  TimeStamp     m_CropTimeStamp;

  IndexType     m_MinIndex;
  IndexType     m_MaxIndex;
  
  InputImageRegionType m_CropRegion;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAutoCropLabelMapFilter.txx"
#endif

#endif

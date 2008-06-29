/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSubtractConstantFromImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkSubtractConstantFromImageFilter_h
#define __itkSubtractConstantFromImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkNumericTraits.h"

namespace itk
{
  
/** \class SubtractConstantFromImageFilter
 *
 * \brief Subract a constant from all input pixels.
 *
 * This filter is templated over the input image type
 * and the output image type.
 *
 * \author Tom Vercauteren, INRIA & Mauna Kea Technologies
 *
 * 
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/1926/510
 *
 * \ingroup IntensityImageFilters  Multithreaded
 * \sa UnaryFunctorImageFilter
 */
namespace Functor {  
  
template< class TInput, class TConstant, class TOutput>
class SubtractConstantFrom
{
public:
  SubtractConstantFrom() : m_Constant(NumericTraits<TConstant>::One) {};
  ~SubtractConstantFrom() {};
  bool operator!=( const SubtractConstantFrom & other ) const
    {
    return !(*this == other);
    }
  bool operator==( const SubtractConstantFrom & other ) const
    {
    return other.m_Constant == m_Constant;
    }
  inline TOutput operator()( const TInput & A )
    {
    // Because the user has to specify the constant we don't
    // check if the cte is not 0;
    return static_cast<TOutput>( A - m_Constant );
    }
  void SetConstant(TConstant ct) {this->m_Constant = ct; }
  const TConstant GetConstant() const { return m_Constant; }
  
  TConstant m_Constant;
};
}

template <class TInputImage, class TConstant, class TOutputImage>
class ITK_EXPORT SubtractConstantFromImageFilter :
      public
UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                        Functor::SubtractConstantFrom< 
   typename TInputImage::PixelType, TConstant,
   typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef SubtractConstantFromImageFilter                 Self;
  typedef UnaryFunctorImageFilter<
    TInputImage,TOutputImage, 
    Functor::SubtractConstantFrom< 
      typename TInputImage::PixelType, TConstant,
      typename TOutputImage::PixelType>   >             Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SubtractConstantFromImageFilter, UnaryFunctorImageFilter);

  
  /** Set the constant that will be used to multiply all the image pixels */
  void SetConstant(TConstant ct)
    {
    this->GetFunctor().SetConstant(ct);
    this->Modified();
    }
  

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputConvertibleToOutputCheck,
                  (Concept::Convertible<typename TInputImage::PixelType,
                   typename TOutputImage::PixelType>));
  itkConceptMacro(Input1Input2OutputSubtractOperatorCheck,
                  (Concept::AdditiveOperators<typename TInputImage::PixelType,
                   TConstant,
                   typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  SubtractConstantFromImageFilter() {};
  virtual ~SubtractConstantFromImageFilter() {};
   
  SubtractConstantFromImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};


} // end namespace itk

#endif

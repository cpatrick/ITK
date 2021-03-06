/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSampleTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "itkSample.h"
#include "itkObjectFactory.h"

namespace itk {
namespace Statistics {
namespace SampleTest {

template <class TMeasurementVector>
class MySample : public Sample< TMeasurementVector >
{
public:
  /** Standard class typedef. */
  typedef MySample  Self;

  typedef Sample< TMeasurementVector > Superclass;

  typedef SmartPointer< Self > Pointer;

  typedef SmartPointer<const Self> ConstPointer;

  /** Standard macros */
  itkTypeMacro(MySample, Sample);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename Superclass::MeasurementVectorType MeasurementVectorType;

  typedef typename Superclass::TotalAbsoluteFrequencyType TotalAbsoluteFrequencyType;

  typedef typename Superclass::AbsoluteFrequencyType AbsoluteFrequencyType;

  typedef typename Superclass::InstanceIdentifier InstanceIdentifier;

  /** Get the size of the sample (number of measurements) */
  virtual InstanceIdentifier Size() const
    {
    return m_Values.size();
    }

  /** Get the measurement associated with a particular
   * InstanceIdentifier. */
  virtual const MeasurementVectorType & GetMeasurementVector(InstanceIdentifier id) const
    {
    return m_Values[id];
    }

  /** Get the frequency of a measurement specified by instance
   * identifier. */
  virtual AbsoluteFrequencyType GetFrequency(InstanceIdentifier id) const
    {
    return m_Frequencies[id];
    }

  /** Get the total frequency of the sample. */
  virtual TotalAbsoluteFrequencyType GetTotalFrequency() const
    {
    TotalAbsoluteFrequencyType sum = NumericTraits< TotalAbsoluteFrequencyType >::Zero;
    typedef typename std::vector< AbsoluteFrequencyType >::const_iterator Iterator;
    Iterator itr = m_Frequencies.begin();
    while( itr != m_Frequencies.end() )
      {
      sum += *itr;
      ++itr;
      }
    return sum;
    }

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf(os,indent);
    os << indent << m_Values.size() << std::endl;
    os << indent << m_Frequencies.size() << std::endl;
    }

  void AddMeasurementVector( const MeasurementVectorType & measure, AbsoluteFrequencyType frequency )
    {
    m_Values.push_back( measure );
    m_Frequencies.push_back( frequency );
    }

private:

  std::vector< TMeasurementVector >  m_Values;

  std::vector< AbsoluteFrequencyType >       m_Frequencies;

};

}
}
}
int itkSampleTest(int, char* [] )
{

  const unsigned int MeasurementVectorSize = 17;

  typedef itk::FixedArray<
    float, MeasurementVectorSize >  MeasurementVectorType;

  typedef itk::Statistics::SampleTest::MySample<
    MeasurementVectorType >   SampleType;

  SampleType::Pointer sample = SampleType::New();

  std::cout << sample->GetNameOfClass() << std::endl;
  std::cout << sample->SampleType::Superclass::GetNameOfClass() << std::endl;

  sample->Print(std::cout);

  sample->SetMeasurementVectorSize( MeasurementVectorSize ); // for code coverage

  if( sample->GetMeasurementVectorSize() != MeasurementVectorSize )
    {
    std::cerr << "GetMeasurementVectorSize() Failed !" << std::endl;
    return EXIT_FAILURE;
    }


  std::cout << sample->Size() << std::endl;

  MeasurementVectorType measure;
  for( unsigned int i=0; i<MeasurementVectorSize; i++)
    {
    measure[i] = 29 * i * i;
    }

  typedef SampleType::AbsoluteFrequencyType AbsoluteFrequencyType;

  AbsoluteFrequencyType frequency = 17;

  sample->AddMeasurementVector( measure, frequency );

  MeasurementVectorType measureBack = sample->GetMeasurementVector( 0 );
  AbsoluteFrequencyType frequencyBack = sample->GetFrequency( 0 );

  if( frequencyBack != frequency )
    {
    std::cerr << "Error in GetFrequency()" << std::endl;
    return EXIT_FAILURE;
    }

  for( unsigned int j=0; j<MeasurementVectorSize; j++)
    {
    if( measureBack[j] != measure[j] )
      {
      std::cerr << "Error in Set/Get MeasurementVector()" << std::endl;
      return EXIT_FAILURE;
      }
    }

  std::cout << sample->GetTotalFrequency() << std::endl;

  //Test if an exception will be thrown if we try to resize the measurement vector
  //size
  try
    {
    sample->SetMeasurementVectorSize( MeasurementVectorSize + 1 );
    std::cerr << "Exception should have been thrown since we are trying to resize\
                  non-resizeable measurement vector type " << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Caughted expected exception: " << excp << std::endl;
    }

  return EXIT_SUCCESS;
}

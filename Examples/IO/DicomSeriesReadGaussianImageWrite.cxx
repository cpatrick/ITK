/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    DicomSeriesReadGaussianImageWrite.cxx
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

#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"

int main( int argc, char* argv[] )
{

  if( argc < 4 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " DicomDirectory  outputFileName sigma [seriesName] " 
              << std::endl;
    return EXIT_FAILURE;
    }


  typedef signed short    PixelType;
  const unsigned int      Dimension = 3;

  typedef itk::Image< PixelType, Dimension >         ImageType;

  typedef itk::ImageSeriesReader< ImageType >        ReaderType;
  ReaderType::Pointer reader = ReaderType::New();


  typedef itk::GDCMImageIO       ImageIOType;
  ImageIOType::Pointer dicomIO = ImageIOType::New();
  
  reader->SetImageIO( dicomIO );


  typedef itk::GDCMSeriesFileNames NamesGeneratorType;
  NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

  nameGenerator->SetUseSeriesDetails( true );
  nameGenerator->AddSeriesRestriction("0008|0021" );

  nameGenerator->SetDirectory( argv[1] );
  

  try
    {
    std::cout << std::endl << "The directory: " << std::endl;
    std::cout << std::endl << argv[1] << std::endl << std::endl;
    std::cout << "Contains the following DICOM Series: ";
    std::cout << std::endl << std::endl;


    

    typedef std::vector< std::string >    SeriesIdContainer;
    
    const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
    
    SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
    SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
    while( seriesItr != seriesEnd )
      {
      std::cout << seriesItr->c_str() << std::endl;
      seriesItr++;
      }
  



    std::string seriesIdentifier;

    if( argc > 4 ) // If no optional series identifier
      {
      seriesIdentifier = argv[4];
      }
    else
      {
      seriesIdentifier = seriesUID.begin()->c_str();
      }


    std::cout << std::endl << std::endl;
    std::cout << "Now reading series: " << std::endl << std::endl;
    std::cout << seriesIdentifier << std::endl;
    std::cout << std::endl << std::endl;


    typedef std::vector< std::string >   FileNamesContainer;
    FileNamesContainer fileNames;

    fileNames = nameGenerator->GetFileNames( seriesIdentifier );

  
    reader->SetFileNames( fileNames );


    try
      {
      reader->Update();
      }
    catch (itk::ExceptionObject &ex)
      {
      std::cout << ex << std::endl;
      return EXIT_FAILURE;
      }


    typedef itk::SmoothingRecursiveGaussianImageFilter< ImageType, ImageType >  FilterType;

    FilterType::Pointer filter = FilterType::New();

    filter->SetInput( reader->GetOutput() );

    const double sigma = atof( argv[3] );
    filter->SetSigma( sigma );

    typedef itk::ImageFileWriter< ImageType > WriterType;
    WriterType::Pointer writer = WriterType::New();
    
    writer->SetFileName( argv[2] );

    writer->SetInput( filter->GetOutput() );

    std::cout  << "Writing the image as " << std::endl << std::endl;
    std::cout  << argv[2] << std::endl << std::endl;



    try
      {
      writer->Update();
      }
    catch (itk::ExceptionObject &ex)
      {
      std::cout << ex << std::endl;
      return EXIT_FAILURE;
      }
    }
  catch (itk::ExceptionObject &ex)
    {
    std::cout << ex << std::endl;
    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}

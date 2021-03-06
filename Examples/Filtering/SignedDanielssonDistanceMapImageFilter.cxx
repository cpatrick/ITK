/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    SignedDanielssonDistanceMapImageFilter.cxx
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

// Software Guide : BeginLatex
// 
// This example illustrates the use of the
// \doxygen{SignedDanielssonDistanceMapImageFilter}.  This filter generates a
// distance map by running Danielsson distance map twice, once on the input
// image and once on the flipped image. 
//
// \index{itk::Signed\-Danielsson\-Distance\-Map\-Image\-Filter!Instantiation}
// \index{itk::Signed\-Danielsson\-Distance\-Map\-Image\-Filter!Header}
//
// The first step required to use this filter is to include its header file. 
//
// Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "itkSignedDanielssonDistanceMapImageFilter.h"
// Software Guide : EndCodeSnippet

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"


int main( int argc, char * argv[] )
{
  if( argc < 5 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputDistanceMapImageFile ";
    std::cerr << " outputVoronoiMapImageFilter ";
    std::cerr << " outputVectorMapImageFilter ";
    std::cerr << std::endl;  
    return EXIT_FAILURE;
    }

  // Software Guide : BeginLatex
  //
  //  Then we must decide what pixel types to use for the input and output
  //  images. Since the output will contain distances measured in pixels, the
  //  pixel type should be able to represent at least the width of the image,
  //  or said in $N-D$ terms, the maximum extension along all the dimensions.
  //  The input and output image types are now defined using their respective
  //  pixel type and dimension.
  //  
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef  unsigned char   InputPixelType;
  typedef  float  OutputPixelType;
  const unsigned int Dimension = 2;

  typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  // 
  // The only change with respect to the previous example is to replace the
  // DanielssonDistanceMapImageFilter with the
  // SignedDanielssonDistanceMapImageFilter
  //
  // SoftwareGuide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::SignedDanielssonDistanceMapImageFilter<
                                         InputImageType, 
                                         OutputImageType >  FilterType;
  
  FilterType::Pointer filter = FilterType::New();
  // Software Guide : EndCodeSnippet
  


  typedef itk::RescaleIntensityImageFilter< 
                   OutputImageType, OutputImageType > RescalerType;

  RescalerType::Pointer scaler = RescalerType::New();

  

  // Software Guide : BeginLatex
  // 
  // The inside is considered as having negative distances. Outside is 
  // treated as having positive distances. To change the convention, 
  // use the InsideIsPositive(bool) function.
  //
  // Software Guide : EndLatex

  // Reader and Writer types are instantiated.
  //
  typedef itk::ImageFileReader< InputImageType  >  ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );


  //  The input to the filter is taken from a reader and its output is passed
  //  to a \doxygen{RescaleIntensityImageFilter} and then to a writer.

  filter->SetInput( reader->GetOutput() );
  scaler->SetInput( filter->GetOutput() );
  writer->SetInput( scaler->GetOutput() );

  scaler->SetOutputMaximum( 65535L );
  scaler->SetOutputMinimum( 0L );
  
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject exp )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr <<     exp    << std::endl;
    }

  
  const char * voronoiMapFileName = argv[3];

  //  The Voronoi map is obtained with the \code{GetVoronoiMap()} method. In
  //  the lines below we connect this output to the intensity rescaler and
  //  save the result in a file.
  //
  //  \index{itk::Danielsson\-Distance\-Map\-Image\-Filter!GetVoronoiMap()}
  //
  scaler->SetInput( filter->GetVoronoiMap() );
  writer->SetFileName( voronoiMapFileName );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject exp )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr <<     exp    << std::endl;
    }


  //  The distance filter also produces an image of \doxygen{Offset} pixels
  //  representing the vectorial distance to the closest object in the scene.
  //  The type of this output image is defined by the VectorImageType
  //  trait of the filter type.

  typedef FilterType::VectorImageType   OffsetImageType;

  //  We can use this type for instantiating an \doxygen{ImageFileWriter} type
  //  and creating an object of this class in the following lines.

  typedef itk::ImageFileWriter< OffsetImageType >  WriterOffsetType;
  WriterOffsetType::Pointer offsetWriter = WriterOffsetType::New();
  offsetWriter->SetInput(  filter->GetVectorDistanceMap()  );
  offsetWriter->SetFileName( argv[4]  );

  try
    {
    offsetWriter->Update();
    }
  catch( itk::ExceptionObject exp )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr <<     exp    << std::endl;
    }

  //  Software Guide : BeginLatex
  //  
  // \begin{figure}
  // \center
  // \includegraphics[width=0.32\textwidth]{Circle.eps}
  // \includegraphics[width=0.32\textwidth]{SignedDanielssonDistanceMapImageFilterOutput.eps}
  // \itkcaption[SignedDanielssonDistanceMapImageFilter
  // output]{SignedDanielssonDistanceMapImageFilter applied on a binary circle image. 
  // The intensity has been rescaled for purposes of display.}
  // \label{fig:SignedDanielssonDistanceMapImageFilterInputOutput}
  // \end{figure}
  //
  //  Figure \ref{fig:SignedDanielssonDistanceMapImageFilterInputOutput} illustrates
  //  the effect of this filter. The
  //  input image and the distance map are shown.
  //
  //  \index{Distance Map!itk::Signed\-Danielsson\-Distance\-Map\-Image\-Filter}
  //
  //  Software Guide : EndLatex 


  return EXIT_SUCCESS;
}


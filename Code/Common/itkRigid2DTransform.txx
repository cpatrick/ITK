/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRigid2DTransform.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRigid2DTransform_txx
#define __itkRigid2DTransform_txx

#include "itkRigid2DTransform.h"
#include "vnl/algo/vnl_svd.h"


namespace itk
{

// Constructor with default arguments
template<class TScalarType>
Rigid2DTransform<TScalarType>::
Rigid2DTransform():
  Superclass(OutputSpaceDimension, ParametersDimension)
{
  m_Angle = NumericTraits< TScalarType >::Zero;
}
 

// Constructor with arguments
template<class TScalarType>
Rigid2DTransform<TScalarType>::
Rigid2DTransform( unsigned int spaceDimension, 
                  unsigned int parametersDimension):
  Superclass(spaceDimension,parametersDimension)
{
  m_Angle = NumericTraits< TScalarType >::Zero;
}


// Destructor
template<class TScalarType>
Rigid2DTransform<TScalarType>::
~Rigid2DTransform()
{
}


// Print self
template<class TScalarType>
void
Rigid2DTransform<TScalarType>::
PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Angle       = " << m_Angle        << std::endl;
}


// Set the rotation matrix
template<class TScalarType>
void
Rigid2DTransform<TScalarType>::
SetMatrix(const MatrixType & matrix )
{
  itkDebugMacro("setting  m_Matrix  to " << matrix ); 
  // The matrix must be orthogonal otherwise it is not
  // representing a valid rotaion in 2D space
  typename MatrixType::InternalMatrixType test = 
    matrix.GetVnlMatrix() * matrix.GetTranspose();

  const double tolerance = 1e-10;
  if( !test.is_identity( tolerance ) ) 
    {
    itk::ExceptionObject ex(__FILE__,__LINE__,"Attempt to set a Non-Orthogonal matrix",ITK_LOCATION);
    throw ex;
    }

  this->SetVarMatrix( matrix );
  this->ComputeOffset();
  this->ComputeMatrixParameters();
  this->Modified();

}


/** Compute the Angle from the Rotation Matrix */
template <class TScalarType>
void
Rigid2DTransform<TScalarType>
::ComputeMatrixParameters( void )
{
  // Extract the orthogonal part of the matrix
  //
  vnl_matrix<TScalarType> p(2, 2);
  p = this->GetMatrix().GetVnlMatrix();
  vnl_svd<TScalarType> svd(p);
  vnl_matrix<TScalarType> r(2, 2);
  r = svd.U() * svd.V().transpose();

  m_Angle = vcl_acos(r[0][0]); 

  if(r[1][0]<0.0)
    {
    m_Angle = -m_Angle;
    }

  if(r[1][0]-vcl_sin(m_Angle) > 0.000001)
    {
    itkWarningMacro("Bad Rotation Matrix " << this->GetMatrix() ); 
    }
}


// Compose with a translation
template<class TScalarType>
void
Rigid2DTransform<TScalarType>::
Translate(const OffsetType &offset, bool)
{
  OutputVectorType newOffset = this->GetOffset();
  newOffset += offset;
  this->SetOffset(newOffset);
  this->ComputeTranslation();
}


// Create and return an inverse transformation
template<class TScalarType>
void
Rigid2DTransform<TScalarType>::
CloneInverseTo( Pointer & result ) const
{
  result = New();
  this->GetInverse(result.GetPointer());
}

// return an inverse transformation
template<class TScalarType>
bool
Rigid2DTransform<TScalarType>::
GetInverse( Self* inverse) const
{
  if(!inverse)
    {
    return false;
    }

  inverse->SetCenter( this->GetCenter() );  // inverse have the same center
  inverse->SetAngle( -this->GetAngle() );
  inverse->SetTranslation( -( this->GetInverseMatrix() * this->GetTranslation() ) );

  return true;
}

// Return an inverse of this transform
template<class TScalarType>
typename Rigid2DTransform<TScalarType>::InverseTransformBasePointer
Rigid2DTransform<TScalarType>
::GetInverseTransform() const
{
  Pointer inv = New();
  return GetInverse(inv) ? inv.GetPointer() : NULL;
}

// Create and return a clone of the transformation
template<class TScalarType>
void
Rigid2DTransform<TScalarType>::
CloneTo( Pointer & result ) const
{
  result = New();
  result->SetCenter( this->GetCenter() );
  result->SetAngle( this->GetAngle() );
  result->SetTranslation( this->GetTranslation() );
}

  
// Reset the transform to an identity transform 
template<class TScalarType >
void
Rigid2DTransform< TScalarType >::
SetIdentity( void ) 
{
  this->Superclass::SetIdentity();
  m_Angle = NumericTraits< TScalarType >::Zero;
}

// Set the angle of rotation
template <class TScalarType>
void
Rigid2DTransform<TScalarType>
::SetAngle(TScalarType angle)
{
  m_Angle = angle;
  this->ComputeMatrix();
  this->ComputeOffset();
  this->Modified();
}


// Set the angle of rotation
template <class TScalarType>
void
Rigid2DTransform<TScalarType>
::SetAngleInDegrees(TScalarType angle)
{
  const TScalarType angleInRadians = angle * vcl_atan(1.0) / 45.0;
  this->SetAngle( angleInRadians );
}

// Compute the matrix from the angle
template <class TScalarType>
void
Rigid2DTransform<TScalarType>
::ComputeMatrix( void )
{
  const MatrixValueType ca = vcl_cos(m_Angle);
  const MatrixValueType sa = vcl_sin(m_Angle);

  MatrixType rotationMatrix;
  rotationMatrix[0][0]= ca; rotationMatrix[0][1]=-sa;
  rotationMatrix[1][0]= sa; rotationMatrix[1][1]= ca;

  this->SetVarMatrix( rotationMatrix );

}

// Set Parameters
template <class TScalarType>
void
Rigid2DTransform<TScalarType>::
SetParameters( const ParametersType & parameters )
{
  itkDebugMacro( << "Setting parameters " << parameters );

  // Set angle
  const TScalarType angle = parameters[0];
  this->SetVarAngle( angle );
 
  // Set translation
  OutputVectorType translation;

  for(unsigned int i=0; i < OutputSpaceDimension; i++) 
    {
    translation[i] = parameters[i+1];
    }
  this->SetVarTranslation( translation );

  // Update matrix and offset
  this->ComputeMatrix();
  this->ComputeOffset();

  // Modified is always called since we just have a pointer to the
  // parameters and cannot know if the parameters have changed.
  this->Modified();

  itkDebugMacro(<<"After setting parameters ");
}

// Get Parameters
template <class TScalarType>
const typename Rigid2DTransform<TScalarType>::ParametersType &
Rigid2DTransform<TScalarType>::
GetParameters( void ) const
{
  itkDebugMacro( << "Getting parameters ");

  // Get the angle
  this->m_Parameters[0] = this->GetAngle();
 
  // Get the translation
  for(unsigned int i=0; i < OutputSpaceDimension; i++) 
    {
    this->m_Parameters[i+1] = this->GetTranslation()[i];
    }

  itkDebugMacro(<<"After getting parameters " << this->m_Parameters );

  return this->m_Parameters;
}

// Compute transformation Jacobian
template<class TScalarType>
const typename Rigid2DTransform<TScalarType>::JacobianType &
Rigid2DTransform<TScalarType>::
GetJacobian( const InputPointType & p ) const
{

  const double ca = vcl_cos(this->GetAngle() );
  const double sa = vcl_sin(this->GetAngle() );

  this->m_Jacobian.Fill(0.0);

  const double cx = this->GetCenter()[0];
  const double cy = this->GetCenter()[1];

  // derivatives with respect to the angle
  this->m_Jacobian[0][0] = -sa * ( p[0] - cx ) - ca * ( p[1] - cy );
  this->m_Jacobian[1][0] =  ca * ( p[0] - cx ) - sa * ( p[1] - cy ); 

  // compute derivatives for the translation part
  unsigned int blockOffset = 1;  
  for(unsigned int dim=0; dim < OutputSpaceDimension; dim++ ) 
    {
    this->m_Jacobian[ dim ][ blockOffset + dim ] = 1.0;
    }

  return this->m_Jacobian;

}

 
} // namespace

#endif

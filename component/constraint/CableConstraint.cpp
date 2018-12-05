/******************************************************************************
*               SOFA, Simulation Open-Framework Architecture                  *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                           Plugin SoftRobots v1.0                            *
*                                                                             *
* This plugin is also distributed under the GNU LGPL (Lesser General          *
* Public License) license with the same conditions than SOFA.                 *
*                                                                             *
* Contributors: Defrost team  (INRIA, University of Lille, CNRS,              *
*               Ecole Centrale de Lille)                                      *
*                                                                             *
* Contact information: https://project.inria.fr/softrobot/contact/            *
*                                                                             *
******************************************************************************/


#include "CableConstraint.inl"

#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace constraintset
{

//////////////////////////////////////CableConstraintConstraintResolution1Dof/////////////////////////////////////////////
using namespace sofa::defaulttype;
using namespace sofa::helper;
using namespace sofa::core;

//----------- Displacement constraint --------------
CableDisplacementConstraintResolution::CableDisplacementConstraintResolution(double& imposedDisplacement, double* force)
    : ConstraintResolution(1)
    , m_imposedDisplacement(imposedDisplacement)
    , m_force(force)
{ }


void CableDisplacementConstraintResolution::init(int line, double** w, double * lambda)
{
    SOFA_UNUSED(lambda);

    m_wActuatorActuator = w[line][line];

}


void CableDisplacementConstraintResolution::resolution(int line, double** w, double* d, double* lambda, double* dfree)
{
    SOFA_UNUSED(dfree);
    SOFA_UNUSED(w);

    // da=Waa*(lambda_a) + Sum Wai * lambda_i  = m_imposedDisplacement

    lambda[line] -= (d[line]-m_imposedDisplacement) / m_wActuatorActuator;

    if (lambda[line]<0)
        lambda[line]=0;

    storeForce(line, lambda);
}


void CableDisplacementConstraintResolution::storeForce(int line,  double* lambda)
{
    *m_force = lambda[line];
}


//--------------- Force constraint -------------
CableForceConstraintResolution::CableForceConstraintResolution(double& imposedForce, double *displacement)
    : ConstraintResolution(1)
    , m_imposedForce(imposedForce)
    , m_displacement(displacement)
{ }


void CableForceConstraintResolution::init(int line, double** w, double * lambda)
{
    SOFA_UNUSED(lambda);

    m_wActuatorActuator = w[line][line];
}

 void CableForceConstraintResolution::initForce(int line, double* lambda)
{
     if (m_imposedForce<0.0)
         m_imposedForce=0.0;

     lambda[line] = m_imposedForce;
}

void CableForceConstraintResolution::resolution(int line, double**, double* d, double* lambda, double* dfree)
{
    SOFA_UNUSED(dfree);
    SOFA_UNUSED(d);
    SOFA_UNUSED(line);

    if (m_imposedForce<0.0)
        m_imposedForce=0.0;

    lambda[line] = m_imposedForce;
    storeDisplacement(line, d);
}


void CableForceConstraintResolution::storeDisplacement(int line,  double* d)
{
    *m_displacement = d[line];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////    FACTORY    //////////////////////////////////////////////
// Registering the component
// see: http://wiki.sofa-framework.org/wiki/ObjectFactory
// 1-RegisterObject("description") + .add<> : Register the component
// 2-.add<>(true) : Set default template

int CableConstraintClass = RegisterObject("Simulate cable actuation.")
.add< CableConstraint<Vec3Types> >(true)

;
////////////////////////////////////////////////////////////////////////////////////////////////////////

// Force template specialization for the most common sofa type.
// This goes with the extern template declaration in the .h. Declaring extern template
// avoid the code generation of the template for each compilation unit.
// see: http://www.stroustrup.com/C++11FAQ.html#extern-templates
template class CableConstraint<Vec3Types>;


} // namespace constraintset

} // namespace component

} // namespace sofa


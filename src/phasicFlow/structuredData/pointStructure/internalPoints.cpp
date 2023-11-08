/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow       
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------  
Licence:
  This file is part of phasicFlow code. It is a free software for simulating 
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions. 
 
  phasicFlow is distributed to help others in their research in the field of 
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/


#include "internalPoints.hpp"


void pFlow::internalPoints::syncPFlag()const
{
	if(!pFlagSync_)
	{
		pFlagsH_ = pFlagsD_.clone<DefaultHostExecutionSpace>();
		pFlagSync_ = true;
	}
}

/*#include "setFieldList.hpp"
#include "error.hpp"
#include "iOstream.hpp"

#include "mortonIndexing.hpp"*/

/*FUNCTION_H
bool pFlow::internalPoints::evaluateinternalPoints()
{
	if(pointFlag_.size() != pointPosition_.size())
	{
		fatalErrorInFunction<<
		"number of elements in pointFlag and pointPosition is not equal \n";
		return false;
	}

	setNumMaxPoints();

	int32 minActive, maxActive;
	numActivePoints_ = pFlow::internalPointsKernels::scanPointFlag(
		0,
		numPoints_,
		static_cast<int8>(internalPoints::ACTIVE),
		pointFlag_.deviceVectorAll(),
		minActive,
		maxActive
		);
	
	activeRange_ = {minActive, maxActive};	
	
	return true;
}

FUNCTION_H
void pFlow::internalPoints::setNumMaxPoints()
{
	maxPoints_ = pointFlag_.capacity();
	numPoints_ = pointFlag_.size();
}

FUNCTION_H
pFlow::realx3Field_D& pFlow::internalPoints::pointPosition()
{
	return pointPosition_;
}

FUNCTION_H
pFlow::int8Field_HD& pFlow::internalPoints::pointFlag()
{
	return pointFlag_;
}
		
pFlow::uniquePtr<pFlow::int32IndexContainer> 
	pFlow::internalPoints::getNewPointsIndices(int32 numNewPoints)const
{


	if( capacity() - activeRange_.second >=  numNewPoints )
	{
		// fill the sequence starting from activeRange_.second-1
		return makeUnique<int32IndexContainer>(
			activeRange_.second,
			activeRange_.second+numNewPoints);

	}

	// second, check if there is space at the beginning
	if( activeRange_.first >= numNewPoints)
	{
		return makeUnique<int32IndexContainer>(
			0,
			numNewPoints);	
	}

	// otherwise scan the points from first to the end to find empty spaces 
	int32Vector newPoints(
		numNewPoints,
		RESERVE());

	newPoints.clear();
	int32 numAdded = 0;
	ForAll(i, pointFlag_)
	{
		if(!isActive(i))
		{
			newPoints.push_back(static_cast<int32>(i));
			numAdded++;
		}

		if(numAdded == numNewPoints)
		{
			return makeUnique<int32IndexContainer>(
				newPoints.data(),
				numNewPoints);
		}
	}

	// check if there is space at the end for the remaining of points
	if( numAdded <numNewPoints && capacity() - size() >= numNewPoints - numAdded )
	{
		int32 ind = size();
		for(int32 i=numAdded; i<numNewPoints; i++)
		{
			newPoints.push_back(ind);
			ind++;
		}
		
		return makeUnique<int32IndexContainer>(
			newPoints.data(),
			numNewPoints);
	}
	else
	{
		fatalErrorInFunction<<"not enough capacity for inserting particles into the point structure\n";
		return nullptr;
	}

	return nullptr;
}*/

pFlow::internalPoints::internalPoints()
:
	pointPosition_("internalPoints", "internalPoints", initialCapacity_, 0, RESERVE()),
	pFlagsD_(initialCapacity_, 0 , 0)
{
	syncPFlag();
}


pFlow::internalPoints::internalPoints
(
	const std::vector<realx3>& posVec
)
:
	pointPosition_("internalPoints", "internalPoints", posVec.capacity(), 0, RESERVE()),
	pFlagsD_(posVec.capacity(), 0, posVec.size())
{

	pointPosition_.assign(posVec);

	syncPFlag();

}


const pFlow::internalPoints::pFlagTypeDevice& 
	pFlow::internalPoints::activePointsMaskD() const 
{
	return pFlagsD_;
}


const pFlow::internalPoints::pFlagTypeHost&
	pFlow::internalPoints::activePointsMaskH() const
{
	syncPFlag();
	return pFlagsH_;
}

FUNCTION_H
void pFlow::internalPoints::updateFlag
(
	const domain& dm, 
	real dist
)
{
	pFlagsD_.markDeleteInDomain
	(
		dm,
		pointPosition_.deviceVectorAll(),
		dist
	);
}


FUNCTION_H
bool pFlow::internalPoints::read
(
	iIstream& is, 
	IOPattern::IOType iotype
)
{
	
	if( !pointPosition_.read(is, iotype))
	{
		fatalErrorInFunction<<
		"Error in reading pointPosition from stream "<< is.name()<<endl;
		return false;
	}

	pFlagsD_ = pFlagTypeDevice(pointPosition_.capacity(), 0, pointPosition_.size());
	pFlagSync_ = false;
	syncPFlag();

	return true;
}
		


FUNCTION_H
bool pFlow::internalPoints::write
(
	iOstream& os, 
	IOPattern::IOType iotype
)const
{
	if( pFlagsD_.isAllActive())
	{
		return pointPosition_.write(os, iotype);
	}
	else
	{
		return pointPosition_.write(os, iotype, activePointsMaskH());
	}
}



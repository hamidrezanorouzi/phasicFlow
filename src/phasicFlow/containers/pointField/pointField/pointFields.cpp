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


#include "pointFields.hpp"
#include "createBoundaryFields.hpp"
#include "periodicBoundaryField.hpp"

#define createAllBoundary(DataType, MemorySpaceType)          \
    template class pFlow::exitBoundaryField<DataType, MemorySpaceType>; \
    template class pFlow::periodicBoundaryField<DataType, MemorySpaceType>;



// uint8
template class pFlow::pointField<pFlow::uint8, pFlow::HostSpace>; 
createBaseBoundary(pFlow::uint8, pFlow::HostSpace);
createAllBoundary(pFlow::uint8, pFlow::HostSpace);

template class pFlow::pointField<pFlow::uint8>; 
createBaseBoundary(pFlow::uint8, void);
createAllBoundary(pFlow::uint8, void);

/// uint32
template class pFlow::pointField<pFlow::uint32, pFlow::HostSpace>; 
createBaseBoundary(pFlow::uint32, pFlow::HostSpace);
createAllBoundary(pFlow::uint32, pFlow::HostSpace);

template class pFlow::pointField<pFlow::uint32>; 
createBaseBoundary(pFlow::uint32, void);
createAllBoundary(pFlow::uint32, void);

/// uint64
template class pFlow::pointField<pFlow::uint64, pFlow::HostSpace>; 
createBaseBoundary(pFlow::uint64, pFlow::HostSpace);
createAllBoundary(pFlow::uint64, pFlow::HostSpace);

template class pFlow::pointField<pFlow::uint64>; 
createBaseBoundary(pFlow::uint64, void);
createAllBoundary(pFlow::uint64, void);

/// real 
template class pFlow::pointField<pFlow::real, pFlow::HostSpace>; 
createBaseBoundary(pFlow::real, pFlow::HostSpace);
createAllBoundary(pFlow::real, pFlow::HostSpace);

template class pFlow::pointField<pFlow::real>; 
createBaseBoundary(pFlow::real, void);
createAllBoundary(pFlow::real, void);

/// realx3
template class pFlow::pointField<pFlow::realx3, pFlow::HostSpace>; 
createBaseBoundary(pFlow::realx3, pFlow::HostSpace);
createAllBoundary(pFlow::realx3, pFlow::HostSpace);


template class pFlow::pointField<pFlow::realx3>; 
createBaseBoundary(pFlow::realx3, void);
createAllBoundary(pFlow::realx3, void);


/// realx4
template class pFlow::pointField<pFlow::realx4, pFlow::HostSpace>; 
createBaseBoundary(pFlow::realx4, pFlow::HostSpace);
createAllBoundary(pFlow::realx4, pFlow::HostSpace);


template class pFlow::pointField<pFlow::realx4>; 
createBaseBoundary(pFlow::realx4, void);
createAllBoundary(pFlow::realx4, void);


/// word 
template class pFlow::pointField<pFlow::word, pFlow::HostSpace>; 
createBaseBoundary(pFlow::word, pFlow::HostSpace);
createAllBoundary(pFlow::word, pFlow::HostSpace);
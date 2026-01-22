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

#ifndef __mixingIndexPostprocessComponent_hpp__
#define __mixingIndexPostprocessComponent_hpp__

#include <vector>

#include "postprocessComponent.hpp"
#include "mixingIndex.hpp"
#include "regionPoints.hpp"
#include "fieldsDataBase.hpp"
#include "dictionary.hpp"


namespace pFlow::postprocessData
{

template<typename RegionType>
class mixingIndexPostprocessComponent
:
    public postprocessComponent
{
private:


    //
    uniquePtr<mixingIndex>        mixingIndex_; 

    /// Region type for selecting a subset of particles for processing
    uniquePtr<RegionType>         regionPointsPtr_;
    
    //regionField<real>               volumeFactor_;

    bool                          executed_{false};

    word                          name_;

    mutable uniquePtr<oFstream>   osPtr_ = nullptr;

public:

    // type info 
    TypeInfoTemplate111(
        "PostprocessComponent",
        RegionType,
        "mixingIndex");

    mixingIndexPostprocessComponent(
        const dictionary& dict,
        fieldsDataBase& fieldsDB,
        const baseTimeControl& defaultTimeControl);
    
    ~mixingIndexPostprocessComponent() override = default;

    /// virtual constructor
    add_vCtor(
        postprocessComponent,
        mixingIndexPostprocessComponent,
        dictionary
    );

    word name()const override
    {
        return name_;
    }

    regionPoints& regPoints() override
    {
        return static_cast<regionPoints&>(regionPointsPtr_());
    }

    const regionPoints& regPoints()const override
    {
        return static_cast<const regionPoints&>(regionPointsPtr_());
    }

    bool execute(const timeInfo& ti, bool forceUpdate = false) override;

    bool executed()const override
    {
        return executed_;
    }

    bool write(const fileSystem& parDir)const override;

};

} // pFlow::postprocessData

#include "mixingIndexPostprocessComponent.cpp"

#endif
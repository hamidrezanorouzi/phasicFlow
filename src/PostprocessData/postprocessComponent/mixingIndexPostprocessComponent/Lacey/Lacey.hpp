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

#ifndef __Lacey_hpp__
#define __Lacey_hpp__

#include "mixingIndex.hpp"
#include "regionField.hpp"
#include "includeMask.hpp"

namespace pFlow::postprocessData
{


class Lacey
:
    public mixingIndex
{
private:
    
    using Mask = typename includeMask::Mask;

    /// @brief number fraction of type 1
    real        type1Frac_;

    /// @brief threshold for number of particles in each sample
    uint32      threshold_;

    /// @brief calculated mixing index 
    real        calculatedMI_;
    
    /// @brief number of valid samples 
    uint32      numSamples_;

    /// @brief average number of instances in samples
    real        avSampleSize_;

    uniquePtr<includeMask>  type1Selection_;

    dictionary  LaceyDict_;

public:

    TypeInfo("Lacey");

    Lacey(
        const dictionary& dict,
        const regionPoints& regPoints,
        fieldsDataBase& fieldsDB);

    virtual 
    ~Lacey()=default;

    add_vCtor(
        mixingIndex,
        Lacey,
        dictionary
    );
    
    bool execute()override;
 
    bool write(iOstream& os)const override;

    bool writeHeader(iOstream& os)const override;

};

}

#endif // __Lacey_hpp__

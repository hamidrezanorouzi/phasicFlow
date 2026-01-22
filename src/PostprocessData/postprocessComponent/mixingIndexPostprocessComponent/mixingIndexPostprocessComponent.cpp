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


namespace pFlow::postprocessData
{

template<typename RegionType>
mixingIndexPostprocessComponent<RegionType>::mixingIndexPostprocessComponent
(
    const dictionary& dict,
    fieldsDataBase& fieldsDB,
    const baseTimeControl& defaultTimeControl
)
:
    postprocessComponent(dict, fieldsDB, defaultTimeControl),
    regionPointsPtr_
    (
        makeUnique<RegionType>(dict, fieldsDB)
    ),
    name_
    (
        dict.name()
    )
{
    mixingIndex_ = mixingIndex::create
    (
        dict,
        regionPointsPtr_(),
        fieldsDB
    );
}

template<typename RegionType>
bool mixingIndexPostprocessComponent<RegionType>::execute
(
    const timeInfo& ti, 
    bool forceUpdate
)
{
     if( !forceUpdate && !timeControl().eventTime(ti))
    {
        executed_ = false;
        return true;
    }

    REPORT(1)<<"Executing postprocess component ("
        <<Blue_Text(ti.timeName())<<" s) : "
        << name()
        <<END_REPORT;
    
    // update regionPoints    
    if(!regionPointsPtr_().update())
    {
        fatalErrorInFunction
            << "regionPoints update failed  for mixing index component: "
            << name_ 
            << endl;
        return false;
    }

    mixingIndex_().execute();

    executed_ = true;

    return true;
}

template<typename RegionType>
bool mixingIndexPostprocessComponent<RegionType>::write
(
    const fileSystem& parDir
)const
{
    if(!executed_) return true;

    const auto ti = database().time().TimeInfo();
    
    if( !osPtr_)
    {
        // file is not open yet
        fileSystem path = parDir + (name_+".Start_"+ti.timeName());
        osPtr_ = makeUnique<oFstream>(path);
        
        if(regionPointsPtr_().scientific())
        {
            osPtr_().stdStream() << std::scientific;
        }
        osPtr_().precision(regionPointsPtr_().precision());
        
        mixingIndex_().writeHeader(osPtr_());
    }

    mixingIndex_().write(osPtr_());
    return true;
}



} // pFlow::postprocessData

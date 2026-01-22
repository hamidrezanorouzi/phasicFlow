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

#include "mixingIndex.hpp"

namespace pFlow::postprocessData
{

mixingIndex::mixingIndex
(
    const dictionary& dict,
    const regionPoints& regPoints,
    fieldsDataBase& fieldsDB
)
:
    regPoints_(regPoints),
    database_(fieldsDB)
{}

uniquePtr<mixingIndex> mixingIndex::create
(
    const dictionary& dict,
    const regionPoints& regPoints,
    fieldsDataBase& fieldsDB
)
{
    word indType = dict.getVal<word>("indexType");

    if( dictionaryvCtorSelector_.search(indType) )
    {
        REPORT(2)<<"Creating mixing index "<< Green_Text(indType)<<" ..."<<endl;
        return dictionaryvCtorSelector_[indType](dict, regPoints, fieldsDB);
    }
    else
    {
        printKeys
		( 
			fatalError << "Ctor Selector "<< Yellow_Text(indType) << " dose not exist. \n"
			<<"Avaiable ones are: \n"
			,
			dictionaryvCtorSelector_
		);
		fatalExit;
        return nullptr;
    }
}

}
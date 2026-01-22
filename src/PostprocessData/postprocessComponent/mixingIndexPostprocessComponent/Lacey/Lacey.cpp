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

#include "Lacey.hpp"

namespace pFlow::postprocessData
{

Lacey::Lacey
(
    const dictionary& parDict,
    const regionPoints& regPoints,
    fieldsDataBase& fieldsDB
)
:
    mixingIndex(parDict, regPoints, fieldsDB),
    type1Frac_(parDict.subDict("LaceyInfo").getVal<real>("type1Frac")),
    threshold_(parDict.subDict("LaceyInfo").getVal<uint32>("threshold")),
    calculatedMI_(0),
    numSamples_(0),
    type1Selection_
    (
        includeMask::create
        (
            parDict.subDict("LaceyInfo").getVal<word>("type1Selection"),
            parDict.subDict("LaceyInfo"),
            fieldsDB
        )
    ),
    LaceyDict_(parDict.subDict("LaceyInfo"))
{}

bool Lacey::execute()
{
    auto mask =  type1Selection_().getMask();
    const auto& regP = this->regPoints();
    
    std::vector<real> samples;
    std::vector<real> samplesNum;

    for(uint32 reg =0; reg<regP.size(); reg++)
    {
        auto partIndices = regP.indices(reg);

        uint32 type1Sum = 0;
        uint32 totalSum = 0;
        uint32 n = 0;

        for(auto index:partIndices)
        {
            if( index!= -1 )
            {
                totalSum++;
                if(mask(index))
                {
                    type1Sum++;
                }
            }
            
        }

        if( totalSum>= threshold_ )
        {
            samples.push_back(1.0*type1Sum/totalSum);
            samplesNum.push_back(totalSum);
        }
    }
    

    numSamples_ = samples.size();
    if(numSamples_ != 0)
    {
        avSampleSize_ = std::accumulate
        (
            samplesNum.begin(), 
            samplesNum.end(), 
            0.0
        )/numSamples_;

        real S0 = type1Frac_*(1-type1Frac_);
        real Sr = S0 / avSampleSize_;

        real S = std::accumulate(
            samples.begin(), 
            samples.end(),
            0.0,
            [&](real acc, real frac){
                return acc + std::pow((frac-type1Frac_),2);
            })/numSamples_;
        
        calculatedMI_ = (S0 - S)/(S0-Sr);
    }
    else
    {
        avSampleSize_ = 0;
        calculatedMI_ = 0;
    }

    return true;
}

bool Lacey::write(iOstream& os)const
{
    const auto tName = this->database().time().TimeInfo().timeName();
    os<<tName<<"\t\t"<<numSamples_<<"\t\t"<<avSampleSize_<<'\t'<<calculatedMI_<<endl;
    return true;
}

bool Lacey::writeHeader(iOstream& os)const
{
    os<<"Parameters for mixing index calculations"<<endl;
    os<<LaceyDict_<<endl<<endl;
    os<<"time\tnumberOfSampels\tavSampleSize\tLaceyMixingIndex"<<endl;
    return true;
}

}
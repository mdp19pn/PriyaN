#include "MammaryCellCycleModel.hpp"
#include "DifferentiatedCellProliferativeType.hpp"

MammaryCellCycleModel::MammaryCellCycleModel()
    : AbstractSimpleCellCycleModel(),
      mMinCellCycleDuration(14.0), // Hours
      mMaxCellCycleDuration(16.0)  // Hours
{
}

MammaryCellCycleModel::MammaryCellCycleModel(const MammaryCellCycleModel& rModel)
   : AbstractSimpleCellCycleModel(rModel),
     mMinCellCycleDuration(rModel.mMinCellCycleDuration),
     mMaxCellCycleDuration(rModel.mMaxCellCycleDuration)
{
    /*
     * Initialize only those member variables defined in this class.
     *
     * The member variable mCellCycleDuration is initialized in the
     * AbstractSimpleCellCycleModel constructor.
     *
     * The member variables mBirthTime, mReadyToDivide and mDimension
     * are initialized in the AbstractCellCycleModel constructor.
     *
     * Note that mCellCycleDuration is (re)set as soon as
     * InitialiseDaughterCell() is called on the new cell-cycle model.
     */
}

AbstractCellCycleModel* MammaryCellCycleModel::CreateCellCycleModel()
{
    return new MammaryCellCycleModel(*this);
}

void MammaryCellCycleModel::SetCellCycleDuration()
{
    RandomNumberGenerator* p_gen = RandomNumberGenerator::Instance();

    if (mpCell->GetCellProliferativeType()->IsType<DifferentiatedCellProliferativeType>())
    {
        mCellCycleDuration = DBL_MAX;
    }
    else
    {
        mCellCycleDuration = mMinCellCycleDuration + (mMaxCellCycleDuration - mMinCellCycleDuration) * p_gen->ranf(); // U[MinCCD,MaxCCD]
    }
}

double MammaryCellCycleModel::GetMinCellCycleDuration()
{
    return mMinCellCycleDuration;
}

void MammaryCellCycleModel::SetMinCellCycleDuration(double minCellCycleDuration)
{
    mMinCellCycleDuration = minCellCycleDuration;
}

double MammaryCellCycleModel::GetMaxCellCycleDuration()
{
    return mMaxCellCycleDuration;
}

void MammaryCellCycleModel::SetMaxCellCycleDuration(double maxCellCycleDuration)
{
    mMaxCellCycleDuration = maxCellCycleDuration;
}

double MammaryCellCycleModel::GetAverageTransitCellCycleTime()
{
    return 0.5*(mMinCellCycleDuration + mMaxCellCycleDuration);
}

double MammaryCellCycleModel::GetAverageStemCellCycleTime()
{
    return 0.5*(mMinCellCycleDuration + mMaxCellCycleDuration);
}

void MammaryCellCycleModel::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<MinCellCycleDuration>" << mMinCellCycleDuration << "</MinCellCycleDuration>\n";
    *rParamsFile << "\t\t\t<MaxCellCycleDuration>" << mMaxCellCycleDuration << "</MaxCellCycleDuration>\n";

    // Call method on direct parent class
    AbstractSimpleCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(MammaryCellCycleModel)

#include "MammaryCellCycleModel.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "LuminalStemCellProperty.hpp"
#include "MyoepithelialStemCellProperty.hpp"

MammaryCellCycleModel::MammaryCellCycleModel()
    : AbstractSimpleCellCycleModel(),
      mMinCellCycleDuration(12.0), // Hours
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

void MammaryCellCycleModel::InitialiseDaughterCell()
{
    if (mpCell->HasCellProperty<LuminalStemCellProperty>())
    {
        boost::shared_ptr<AbstractCellProperty> p_luminal =
        mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<LuminalCellProperty>();
        mpCell->AddCellProperty(p_luminal);
    }
    else if (mpCell->HasCellProperty<MyoepithelialStemCellProperty>())
    {
        boost::shared_ptr<AbstractCellProperty> p_myo =
        mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>();
        mpCell->AddCellProperty(p_myo);
    }
}

AbstractCellCycleModel* MammaryCellCycleModel::CreateCellCycleModel()
{
    return new MammaryCellCycleModel(*this);
}

void MammaryCellCycleModel::SetCellCycleDuration()
{
    RandomNumberGenerator* p_gen = RandomNumberGenerator::Instance();

    if (mpCell->HasCellProperty<LuminalCellProperty>()) // luminal cell is DifferentiatedCellProliferativeType
    {
        mCellCycleDuration = DBL_MAX;
    }
    else if (mpCell->HasCellProperty<MyoepithelialCellProperty>()) // myoepithelial cell is DifferentiatedCellProliferativeType
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

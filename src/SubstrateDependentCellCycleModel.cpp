#include "SubstrateDependentCellCycleModel.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "Debug.hpp"

SubstrateDependentCellCycleModel::SubstrateDependentCellCycleModel()
    : AbstractSimplePhaseBasedCellCycleModel(),
      mQuiescentHeightFraction(DOUBLE_UNSET),
      mEquilibriumHeight(DOUBLE_UNSET),
      mCurrentQuiescentOnsetTime(SimulationTime::Instance()->GetTime()),
      mCurrentQuiescentDuration(0.0)
{
}

SubstrateDependentCellCycleModel::SubstrateDependentCellCycleModel(const SubstrateDependentCellCycleModel& rModel)
    : AbstractSimplePhaseBasedCellCycleModel(rModel),
      mQuiescentHeightFraction(rModel.mQuiescentHeightFraction),
      mEquilibriumHeight(rModel.mEquilibriumHeight),
      mCurrentQuiescentOnsetTime(rModel.mCurrentQuiescentOnsetTime),
      mCurrentQuiescentDuration(rModel.mCurrentQuiescentDuration)
{
    /*
     * Initialize only those member variables defined in this class.
     *
     * The member variables mCurrentCellCyclePhase, mG1Duration,
     * mMinimumGapDuration, mStemCellG1Duration, mTransitCellG1Duration,
     * mSDuration, mG2Duration and mMDuration are initialized in the
     * AbstractPhaseBasedCellCycleModel constructor.
     *
     * The member variables mBirthTime, mReadyToDivide and mDimension
     * are initialized in the AbstractCellCycleModel constructor.
     *
     * Note that mG1Duration is (re)set as soon as InitialiseDaughterCell()
     * is called on the new cell-cycle model.
     */
}

void SubstrateDependentCellCycleModel::UpdateCellCyclePhase()
{
    if ((mQuiescentHeightFraction == DOUBLE_UNSET) || (mEquilibriumHeight == DOUBLE_UNSET))
    {
        EXCEPTION("The member variables mQuiescentHeightFraction and mEquilibriumHeight have not yet been set.");
    }

    // Get cell Height
    double cell_height = mpCell->GetCellData()->GetItem("height");
    PRINT_VARIABLE("cycle");

    // Removes the luminal cell property
    mpCell->RemoveCellProperty<LuminalCellProperty>();

    // Removes the myoepithelial cell property
    mpCell->RemoveCellProperty<MyoepithelialCellProperty>();    

    if (mCurrentCellCyclePhase == G_ONE_PHASE)
    {
        // Update G1 duration based on cell height
        double dt = SimulationTime::Instance()->GetTimeStep();
        double quiescent_height = mEquilibriumHeight * mQuiescentHeightFraction;
        
        if (cell_height < quiescent_height)
        {
            // Update the duration of the current period of contact inhibition.
            mCurrentQuiescentDuration = SimulationTime::Instance()->GetTime() - mCurrentQuiescentOnsetTime;
            mG1Duration += dt;

            /*
             * This method is usually called within a CellBasedSimulation, after the CellPopulation
             * has called CellPropertyRegistry::TakeOwnership(). This means that were we to call
             * CellPropertyRegistry::Instance() here when adding the CellLabel, we would be creating
             * a new CellPropertyRegistry. In this case the CellLabel's cell count would be incorrect.
             * We must therefore access the CellLabel via the cell's CellPropertyCollection.
             */
            boost::shared_ptr<AbstractCellProperty> p_luminal = mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<LuminalCellProperty>();
            mpCell->AddCellProperty(p_luminal);

            boost::shared_ptr<AbstractCellProperty> p_myoepithelial = mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>();
            mpCell->AddCellProperty(p_myoepithelial);
        }
        else
        {
            // Reset the cell's quiescent duration and update the time at which the onset of quiescent occurs
            mCurrentQuiescentDuration = 0.0;
            mCurrentQuiescentOnsetTime = SimulationTime::Instance()->GetTime();
        }
    }

    double time_since_birth = GetAge();
    assert(time_since_birth >= 0);

    if (mpCell->GetCellProliferativeType()->IsType<DifferentiatedCellProliferativeType>())
    {
        mCurrentCellCyclePhase = G_ZERO_PHASE;
    }
    else if (time_since_birth < GetMDuration())
    {
        mCurrentCellCyclePhase = M_PHASE;
    }
    else if (time_since_birth < GetMDuration() + mG1Duration)
    {
        mCurrentCellCyclePhase = G_ONE_PHASE;
    }
    else if (time_since_birth < GetMDuration() + mG1Duration + GetSDuration())
    {
        mCurrentCellCyclePhase = S_PHASE;
    }
    else if (time_since_birth < GetMDuration() + mG1Duration + GetSDuration() + GetG2Duration())
    {
        mCurrentCellCyclePhase = G_TWO_PHASE;
    }
}

AbstractCellCycleModel* SubstrateDependentCellCycleModel::CreateCellCycleModel()
{
    return new SubstrateDependentCellCycleModel(*this);
}

void SubstrateDependentCellCycleModel::SetQuiescentHeightFraction(double quiescentHeightFraction)
{
    mQuiescentHeightFraction = quiescentHeightFraction;
}

double SubstrateDependentCellCycleModel::GetQuiescentHeightFraction() const
{
    return mQuiescentHeightFraction;
}

void SubstrateDependentCellCycleModel::SetEquilibriumHeight(double equilibriumHeight)
{
    mEquilibriumHeight = equilibriumHeight;
}

double SubstrateDependentCellCycleModel::GetEquilibriumHeight() const
{
    return mEquilibriumHeight;
}

double SubstrateDependentCellCycleModel::GetCurrentQuiescentDuration() const
{
    return mCurrentQuiescentDuration;
}

double SubstrateDependentCellCycleModel::GetCurrentQuiescentOnsetTime() const
{
    return mCurrentQuiescentOnsetTime;
}

void SubstrateDependentCellCycleModel::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<QuiescentHeightFraction>" << mQuiescentHeightFraction << "</QuiescentHeightFraction>\n";
    *rParamsFile << "\t\t\t<EquilibriumHeight>" << mEquilibriumHeight << "</EquilibriumHeight>\n";

    // Call method on direct parent class
    AbstractSimplePhaseBasedCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(SubstrateDependentCellCycleModel)

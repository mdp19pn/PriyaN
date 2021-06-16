#include "CellCoverslipBasedCellKiller.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "Debug.hpp"

template<unsigned DIM>
CellCoverslipBasedCellKiller<DIM>::CellCoverslipBasedCellKiller(AbstractCellPopulation<DIM>* pCellPopulation, double probabilityOfDeathInAnHour)
: AbstractCellKiller<DIM>(pCellPopulation),
mProbabilityOfDeathInAnHour(probabilityOfDeathInAnHour)
{   
    if ((mProbabilityOfDeathInAnHour<0) || (mProbabilityOfDeathInAnHour>1))
    {
        EXCEPTION("Probability of death must be between zero and one");
    }
}

template<unsigned DIM>
double CellCoverslipBasedCellKiller<DIM>::GetDeathProbabilityInAnHour() const
{
    return mProbabilityOfDeathInAnHour;
}

template<unsigned DIM>
void CellCoverslipBasedCellKiller<DIM>::CheckAndLabelSingleCellForApoptosis(CellPtr pCell)
{
    /*
     * We assume a constant time step and that there are an integer number (n = 1/dt)
     * of time steps per hour. We also assume that this method is called every time step
     * and that the probabilities of dying at different times are independent.
     *
     * Let q=mProbabilityOfDeathInAnHour and p="probability of death in a given time step".
     *
     * Probability of not dying in an hour:
     * (1-q) = (1-p)^n = (1-p)^(1/dt).
     *
     * Rearranging for p:
     * p = 1 - (1-q)^dt.
     */
    double death_prob_this_timestep = 1.0 - pow((1.0 - mProbabilityOfDeathInAnHour), SimulationTime::Instance()->GetTimeStep());

    if (!pCell->HasApoptosisBegun() &&
        RandomNumberGenerator::Instance()->ranf() < death_prob_this_timestep)
    {
        pCell->StartApoptosis();
    }
}


template<unsigned DIM>
void CellCoverslipBasedCellKiller<DIM>::CheckAndLabelCellsForApoptosisOrDeath()
{   
    //PRINT_VARIABLE(SimulationTime::Instance()->GetTime())
    
    // Iterate over cell population
    for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = this->mpCellPopulation->Begin();
    cell_iter != this->mpCellPopulation->End();
    ++cell_iter)
    {
        c_vector<double, DIM> location;
        location = this->mpCellPopulation->GetLocationOfCellCentre(*cell_iter);

        if (location[DIM-1] > 0.75)
        {
            CheckAndLabelSingleCellForApoptosis(*cell_iter);
            // PRINT_VARIABLE("kill");
            // PRINT_VARIABLE(cell_iter->GetCellId());
            // PRINT_VARIABLE(location[0]);
            // PRINT_VARIABLE(location[1]);
            // PRINT_VARIABLE(location[2]);
        }
    }
}

template<unsigned DIM>
void CellCoverslipBasedCellKiller<DIM>::OutputCellKillerParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<ProbabilityOfDeathInAnHour>" << mProbabilityOfDeathInAnHour << "</ProbabilityOfDeathInAnHour>\n";
    // No parameters to output, so just call method on direct parent class
    AbstractCellKiller<DIM>::OutputCellKillerParameters(rParamsFile);
}

// Explicit instantiation
template class CellCoverslipBasedCellKiller<1>;
template class CellCoverslipBasedCellKiller<2>;
template class CellCoverslipBasedCellKiller<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(CellCoverslipBasedCellKiller)
#include "CellCoverslipBasedCellKiller.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "Debug.hpp"

template<unsigned DIM>
CellCoverslipBasedCellKiller<DIM>::CellCoverslipBasedCellKiller(AbstractCellPopulation<DIM>* pCellPopulation)
: AbstractCellKiller<DIM>(pCellPopulation)
{   
}

template<unsigned DIM>
void CellCoverslipBasedCellKiller<DIM>::CheckAndLabelCellsForApoptosisOrDeath()
{   
    PRINT_VARIABLE(SimulationTime::Instance()->GetTime())
    {
        // Iterate over cell population
        for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = this->mpCellPopulation->Begin();
        cell_iter != this->mpCellPopulation->End();
        ++cell_iter)
        {
            double cell_height = this->mpCellPopulation->GetLocationOfCellCentre(*cell_iter)[DIM-1];
            double cell_y = this->mpCellPopulation->GetLocationOfCellCentre(*cell_iter)[DIM-2];
            double cell_x = this->mpCellPopulation->GetLocationOfCellCentre(*cell_iter)[DIM-3];

            if (cell_height > 1.5)
            {
                cell_iter->Kill();
                PRINT_VARIABLE("kill");
                PRINT_VARIABLE(cell_height);
                PRINT_VARIABLE(cell_y);
                PRINT_VARIABLE(cell_x);
                PRINT_VARIABLE(cell_iter->GetAge());
            }
        }
    }
}

template<unsigned DIM>
void CellCoverslipBasedCellKiller<DIM>::OutputCellKillerParameters(out_stream& rParamsFile)
{
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
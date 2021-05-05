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
    switch (DIM)
    {
        default:
        // Iterate over cell population
        for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = this->mpCellPopulation->Begin();
        cell_iter != this->mpCellPopulation->End();
        ++cell_iter)
        {
            double cell_height = this->mpCellPopulation->GetLocationOfCellCentre(*cell_iter)[DIM-1];

            if (cell_height>1.0)
            {
                cell_iter->Kill();
            }
        }
    }
}

template<unsigned DIM>
void CellCoverslipBasedCellKiller<DIM>::OutputCellKillerParameters(out_stream& rParamsFile)
{
    // No parameters to output, so just call method on direct parent class
    AbstractCellKiller<DIM>::OutputCellKillerParameters(rParamsFile);
    PRINT_VARIABLE("kill");
}

// Explicit instantiation
template class CellCoverslipBasedCellKiller<1>;
template class CellCoverslipBasedCellKiller<2>;
template class CellCoverslipBasedCellKiller<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(CellCoverslipBasedCellKiller)
#include "CellCoverslipBasedCellKiller.hpp"

template<unsigned SPACE_DIM>
CellCoverslipBasedCellKiller<SPACE_DIM>::CellCoverslipBasedCellKiller(AbstractCellPopulation<SPACE_DIM>* pCellPopulation)
: AbstractCellKiller<SPACE_DIM>(pCellPopulation)
{   
}

template<unsigned SPACE_DIM>
void CellCoverslipBasedCellKiller<SPACE_DIM>::CheckAndLabelCellsForApoptosisOrDeath()
{
    for (typename AbstractCellPopulation<SPACE_DIM>::Iterator cell_iter = this->mpCellPopulation->Begin();
        cell_iter != this->mpCellPopulation->End();
        ++cell_iter)
    {
        c_vector<double, 3> location;
        location = this->mpCellPopulation->GetLocationOfCellCentre(*cell_iter);
        double z = location[2]

        if (z > 2.0)
        {
            cell_iter->Kill();
        }
    }
}

template<unsigned SPACE_DIM>
void CellCoverslipBasedCellKiller<SPACE_DIM>::OutputCellKillerParameters(out_stream& rParamsFile)
{
    // No parameters to output, so just call method on direct parent class
    AbstractCellKiller<SPACE_DIM>::OutputCellKillerParameters(rParamsFile);
}

// Explicit instantiation
template class CellCoverslipBasedCellKiller<1>;
template class CellCoverslipBasedCellKiller<2>;
template class CellCoverslipBasedCellKiller<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(CellCoverslipBasedCellKiller)
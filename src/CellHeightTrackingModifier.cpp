#include "CellHeightTrackingModifier.hpp"
#include "NodeBasedCellPopulation.hpp"

template<unsigned DIM>
CellHeightTrackingModifier<DIM>::CellHeightTrackingModifier()
    : AbstractCellBasedSimulationModifier<DIM>()
{
}

template<unsigned DIM>
CellHeightTrackingModifier<DIM>::~CellHeightTrackingModifier()
{
}

template<unsigned DIM>
void CellHeightTrackingModifier<DIM>::UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void CellHeightTrackingModifier<DIM>::SetupSolve(AbstractCellPopulation<DIM,DIM>& rCellPopulation, std::string outputDirectory)
{
    /*
     * We must update CellData in SetupSolve(), otherwise it will not have been
     * fully initialised by the time we enter the main time loop.
     */
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void CellHeightTrackingModifier<DIM>::UpdateCellData(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    // Make sure the cell population is updated
    rCellPopulation.Update();

    // Throw an exception message if not using a NodeBasedCellPopulation
    if (dynamic_cast<NodeBasedCellPopulation<DIM>*>(&rCellPopulation) == nullptr)
    {
        EXCEPTION("CellCoverslipAdhesionForce is to be used with a NodeBasedCellPopulation only");
    }

    // Helper variable that is a static cast of the cell population
    NodeBasedCellPopulation<DIM>* p_cell_population = static_cast<NodeBasedCellPopulation<DIM>*>(&rCellPopulation);

    // Iterate over cell population
    for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = rCellPopulation.Begin();
         cell_iter != rCellPopulation.End();
         ++cell_iter)
    {
        // Get the height of this cell
        double cell_height = rCellPopulation.GetVolumeOfCell(*cell_iter);

        // Store the cell's volume in CellData
        cell_iter->GetCellData()->SetItem("height", cell_height);
    }
}

template<unsigned DIM>
void CellHeightTrackingModifier<DIM>::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    // No parameters to output, so just call method on direct parent class
    AbstractCellBasedSimulationModifier<DIM>::OutputSimulationModifierParameters(rParamsFile);
}

// Explicit instantiation
template class CellHeightTrackingModifier<1>;
template class CellHeightTrackingModifier<2>;
template class CellHeightTrackingModifier<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(CellHeightTrackingModifier)


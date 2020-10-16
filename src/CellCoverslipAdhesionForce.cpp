#include "CellCoverslipAdhesionForce.hpp"
#include "NodeBasedCellPopulation.hpp"

template<unsigned DIM>
CellCoverslipAdhesionForce<DIM>::CellCoverslipAdhesionForce()
    : AbstractForce<DIM>(),
      mStiffness(1.0),
      mEquilibriumLength(1.0)
{
}

template<unsigned DIM>
CellCoverslipAdhesionForce<DIM>::~CellCoverslipAdhesionForce()
{
}

template<unsigned DIM>
void CellCoverslipAdhesionForce<DIM>::SetStiffness(double stiffness)
{
    mStiffness = stiffness;
}

template<unsigned DIM>
void CellCoverslipAdhesionForce<DIM>::SetEquilibriumLength(double equilibriumLength)
{
    mEquilibriumLength = equilibriumLength;
}

template<unsigned DIM>
void CellCoverslipAdhesionForce<DIM>::AddForceContribution(AbstractCellPopulation<DIM>& rCellPopulation)
{
    // Throw an exception message if not using a NodeBasedCellPopulation
    if (dynamic_cast<NodeBasedCellPopulation<DIM>*>(&rCellPopulation) == nullptr)
    {
        EXCEPTION("CellCoverslipAdhesionForce is to be used with a NodeBasedCellPopulation only");
    }

    // Helper variable that is a static cast of the cell population
    NodeBasedCellPopulation<DIM>* p_cell_population = static_cast<NodeBasedCellPopulation<DIM>*>(&rCellPopulation);

    // Iterate over vertices in the cell population
    for (unsigned node_index=0; node_index<p_cell_population->GetNumNodes(); node_index++)
    {
        c_vector<double, DIM> force_contribution;
        for (unsigned i=0; i<DIM; i++)
        {
            ///\todo use spring stiffness and equilibrium length here
            double cell_height = p_cell_population->GetNode(node_index)->rGetLocation()[2];
            c_vector<double, DIM> force_contribution;
            force_contribution[0] = 0.0;
            force_contribution[1] = 0.0;
            force_contribution[2] = -10*cell_height;
        }
        rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force_contribution);
    }
}

template<unsigned DIM>
void CellCoverslipAdhesionForce<DIM>::OutputForceParameters(out_stream& rParamsFile)
{
    // Output member variable then call method on direct parent class
    *rParamsFile << "\t\t\t<Stiffness>" << mStiffness << "</Stiffness> \n";
    *rParamsFile << "\t\t\t<EquilibriumLength>" << mEquilibriumLength << "</EquilibriumLength> \n";

    AbstractForce<DIM>::OutputForceParameters(rParamsFile);
}

// Explicit instantiation
template class CellCoverslipAdhesionForce<1>;
template class CellCoverslipAdhesionForce<2>;
template class CellCoverslipAdhesionForce<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(CellCoverslipAdhesionForce)

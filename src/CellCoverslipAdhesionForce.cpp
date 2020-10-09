#include "CellCoverslipAdhesionForce.hpp"
#include "NodeBasedCellPopulation.hpp"

template<unsigned DIM>
CellCoverslipAdhesionForce<DIM>::CellCoverslipAdhesionForce(double diffusionConstant)
    : AbstractForce<DIM>(),
      mDiffusionConstant(diffusionConstant)
{
}

template<unsigned DIM>
CellCoverslipAdhesionForce<DIM>::~CellCoverslipAdhesionForce()
{
}

template<unsigned DIM>
void CellCoverslipAdhesionForce<DIM>::SetDiffusionConstant(double diffusionConstant)
{
    mDiffusionConstant = diffusionConstant;
}

template<unsigned DIM>
void CellCoverslipAdhesionForce<DIM>::AddForceContribution(AbstractCellPopulation<DIM>& rCellPopulation)
{
    // Throw an exception message if not using a VertexBasedCellPopulation
    if (dynamic_cast<NodeBasedCellPopulation<DIM>*>(&rCellPopulation) == nullptr)
    {
        EXCEPTION("CellCoverslipAdhesionForce is to be used with a VertexBasedCellPopulation only");
    }

    // Helper variable that is a static cast of the cell population
    NodeBasedCellPopulation<DIM>* p_cell_population = static_cast<NodeBasedCellPopulation<DIM>*>(&rCellPopulation);

    double dt = SimulationTime::Instance()->GetTimeStep();

    // Iterate over vertices in the cell population
    for (unsigned node_index=0; node_index<p_cell_population->GetNumNodes(); node_index++)
    {
        c_vector<double, DIM> force_contribution;
        for (unsigned i=0; i<DIM; i++)
        {
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
    *rParamsFile << "\t\t\t<DiffusionConstant>" << mDiffusionConstant << "</DiffusionConstant> \n";

    AbstractForce<DIM>::OutputForceParameters(rParamsFile);
}

// Explicit instantiation
template class CellCoverslipAdhesionForce<1>;
template class CellCoverslipAdhesionForce<2>;
template class CellCoverslipAdhesionForce<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(CellCoverslipAdhesionForce)

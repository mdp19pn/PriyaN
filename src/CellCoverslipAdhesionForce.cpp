#include "CellCoverslipAdhesionForce.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"

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

// https://chaste.cs.ox.ac.uk/chaste/docs/release_2.1/CryptProjectionForce_8cpp-source.html

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

    // Iterate over nodes in the cell population
    for (unsigned node_index=0; node_index<p_cell_population->GetNumNodes(); node_index++)
    {
        c_vector<double, DIM> force_contribution;
        for (unsigned i=0; i<DIM; i++)
        {
            ///\todo use spring stiffness and equilibrium length here
            double cell_height = p_cell_population->GetNode(node_index)->rGetLocation()[2];
            
            // Determine if cell is luminal (if not, assume it is myoepithelial)
            CellPtr p_cell = p_cell_population->GetCellUsingLocationIndex(node_index);
            bool cell_is_luminal = p_cell->template HasCellProperty<LuminalCellProperty>();

            // Determine if cell expresses b1 and/or b4 integrin
            bool cell_b1_expn = true;
            bool cell_b4_expn = true;
            
            if (cell_is_luminal)
            {
                CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<LuminalCellProperty>();
                boost::shared_ptr<LuminalCellProperty> p_prop = boost::static_pointer_cast<LuminalCellProperty>(collection.GetProperty());
                cell_b1_expn = p_prop->GetB1IntegrinExpression();
                cell_b4_expn = p_prop->GetB4IntegrinExpression();
            }
            else
            {
                CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
                boost::shared_ptr<MyoepithelialCellProperty> p_prop = boost::static_pointer_cast<MyoepithelialCellProperty>(collection.GetProperty());
                cell_b1_expn = p_prop->GetB1IntegrinExpression();
                cell_b4_expn = p_prop->GetB4IntegrinExpression();
            }
            
            c_vector<double, DIM> force_contribution;
            force_contribution[0] = 0.0;
            force_contribution[1] = 0.0;
            force_contribution[2] = -10*cell_height;
            
            // double strength = 10; 
            if (cell_is_luminal)
            {
                if (cell_b1_expn && cell_b4_expn)
                {
                    c_vector<double, DIM> boundary_force = zero_vector<double>(DIM);
                    boundary_force[2] = mStiffness*1(z, 2);
                    rForces[node_iter->GetIndex()] += boundary_force;
                }
                else if (cell_b1_expn != cell_b4_expn)
                {
                    mStiffness = 0.5;
                }
                else
                {
                    mStiffness = 0.0;
                }
            }
          else // if cell is myoepithelial
          {
                if (cell_b1_expn && cell_b4_expn)
                {
                    c_vector<double, DIM> boundary_force = zero_vector<double>(DIM);
                    boundary_force[2] = mStiffness*5(z, 2);
                    rForces[node_iter->GetIndex()] += boundary_force;
                }
                else if (cell_b1_expn != cell_b4_expn)
                {
                    mStiffness = 1.0;
                }
                else
                {
                    mStiffness = 0.5;
                }
        }
        rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force_contribution);
    }
}
} // This close bracket fixed a compilation issue supposedly??

template<unsigned DIM>
void CellCoverslipAdhesionForce<DIM>::OutputForceParameters(out_stream& rParamsFile)
{
    // Output member variable
    *rParamsFile << "\t\t\t<Stiffness>" << mStiffness << "</Stiffness> \n";
    *rParamsFile << "\t\t\t<EquilibriumLength>" << mEquilibriumLength << "</EquilibriumLength> \n";

    // Call direct parent class
    AbstractForce<DIM>::OutputForceParameters(rParamsFile);
}

// Explicit instantiation
template class CellCoverslipAdhesionForce<1>;
template class CellCoverslipAdhesionForce<2>;
template class CellCoverslipAdhesionForce<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(CellCoverslipAdhesionForce)

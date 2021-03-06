#include "CellCoverslipAdhesionForce.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "MammaryStemCellProperty.hpp"
#include "Debug.hpp"

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
    // This force class is defined for NodeBasedCellPopulations only
    assert(dynamic_cast<NodeBasedCellPopulation<DIM>*>(&rCellPopulation) != nullptr);

    // Helper variable that is a static cast of the cell population
    NodeBasedCellPopulation<DIM>* p_cell_population = static_cast<NodeBasedCellPopulation<DIM>*>(&rCellPopulation);

    // loop over nodes in the cell population
    for (typename AbstractMesh<DIM, DIM>::NodeIterator node_iter = rCellPopulation.rGetMesh().GetNodeIteratorBegin(); 
    node_iter != rCellPopulation.rGetMesh().GetNodeIteratorEnd();++node_iter)
    {
        // Get the index, height of this node
        unsigned node_index = node_iter->GetIndex();
        double cell_height = p_cell_population->GetNode(node_index)->rGetLocation()[2];

        c_vector<double, DIM> force_contribution;
        for (unsigned i=0; i<DIM; i++)
        {           
            // Determine if cell is luminal (if not, assume it is myoepithelial)
            CellPtr p_cell = p_cell_population->GetCellUsingLocationIndex(node_index);
            bool cell_is_luminal = p_cell->template HasCellProperty<LuminalCellProperty>();
            bool cell_is_myoepithelial = p_cell->template HasCellProperty<MyoepithelialCellProperty>();

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
            else if (cell_is_myoepithelial)
            {
                CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
                boost::shared_ptr<MyoepithelialCellProperty> p_prop = boost::static_pointer_cast<MyoepithelialCellProperty>(collection.GetProperty());
                cell_b1_expn = p_prop->GetB1IntegrinExpression();
                cell_b4_expn = p_prop->GetB4IntegrinExpression();
            }
            else // if cell is mammary stem cell
            {
                CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<MammaryStemCellProperty>();
                boost::shared_ptr<MammaryStemCellProperty> p_prop = boost::static_pointer_cast<MammaryStemCellProperty>(collection.GetProperty());
                cell_b1_expn = p_prop->GetB1IntegrinExpression();
                cell_b4_expn = p_prop->GetB4IntegrinExpression();
            }
            
            if (cell_is_luminal) // if cell is luminal
            {
                if (cell_b1_expn && cell_b4_expn)
                {
                    mEquilibriumLength = 2.0;
                }
                else if (cell_b1_expn || cell_b4_expn)
                {
                    mEquilibriumLength = 1.0;
                }
                else
                {
                    mEquilibriumLength = 0.5;
                }
            }
            else if (cell_is_myoepithelial) // if cell is myoepithelial
            {
                if (cell_b1_expn && cell_b4_expn)
                {
                    mEquilibriumLength = 4.0;
                }
                else if (cell_b1_expn || cell_b4_expn)
                {
                    mEquilibriumLength = 2.0;
                }
                else
                {
                    mEquilibriumLength = 1.0;
                }
            }
            else // if cell is mammary stem cell
            {
                if (cell_b1_expn && cell_b4_expn)
                {
                    mEquilibriumLength = 4.0;
                }
                else if (cell_b1_expn || cell_b4_expn)
                {
                    mEquilibriumLength = 2.0;
                }
                else
                {
                    mEquilibriumLength = 1.0;
                }
            }
        
            force_contribution[0] = 0.0;
            force_contribution[1] = 0.0;
            force_contribution[2] = -mEquilibriumLength*cell_height;
        }
        node_iter->AddAppliedForceContribution(force_contribution);
    }
}

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

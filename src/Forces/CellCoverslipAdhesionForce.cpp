#include "CellCoverslipAdhesionForce.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "LuminalStemCellProperty.hpp"
#include "MyoepithelialStemCellProperty.hpp"
#include "Debug.hpp"

template<unsigned DIM>
CellCoverslipAdhesionForce<DIM>::CellCoverslipAdhesionForce()
    : AbstractForce<DIM>(),
      mStiffness(1.0),
      mEquilibriumLength(1.0)
{
    assert(mStiffness > 0.0);
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
    /* Inside the method, we loop over cells, and add a vector to each node associated with cells 
     * with the LuminalCellPorperty, which is proportional (with constant mStiffness) to the negative of the position. 
    */

    for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = rCellPopulation.Begin();
         cell_iter != rCellPopulation.End();
         ++cell_iter)
    { 
        unsigned node_index = rCellPopulation.GetLocationIndexUsingCell(*cell_iter);
    
        // Determine cell type
        CellPtr p_cell = rCellPopulation.GetCellUsingLocationIndex(node_index);
        bool cell_is_luminal = p_cell->template HasCellProperty<LuminalCellProperty>();
        bool cell_is_myo = p_cell->template HasCellProperty<MyoepithelialCellProperty>();
        bool cell_is_luminal_stem = p_cell->template HasCellProperty<LuminalStemCellProperty>();
        bool cell_is_myo_stem = p_cell->template HasCellProperty<MyoepithelialStemCellProperty>();

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
        else if (cell_is_myo)
        {
            CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<MyoepithelialCellProperty>();
            boost::shared_ptr<MyoepithelialCellProperty> p_prop = boost::static_pointer_cast<MyoepithelialCellProperty>(collection.GetProperty());
            cell_b1_expn = p_prop->GetB1IntegrinExpression();
            cell_b4_expn = p_prop->GetB4IntegrinExpression();
        }
        else if (cell_is_luminal_stem) 
        {
            CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<LuminalStemCellProperty>();
            boost::shared_ptr<LuminalStemCellProperty> p_prop = boost::static_pointer_cast<LuminalStemCellProperty>(collection.GetProperty());
            cell_b1_expn = p_prop->GetB1IntegrinExpression();
            cell_b4_expn = p_prop->GetB4IntegrinExpression();
        }
        else
        {
            CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<MyoepithelialStemCellProperty>();
            boost::shared_ptr<MyoepithelialStemCellProperty> p_prop = boost::static_pointer_cast<MyoepithelialStemCellProperty>(collection.GetProperty());
            cell_b1_expn = p_prop->GetB1IntegrinExpression();
            cell_b4_expn = p_prop->GetB4IntegrinExpression();
        }

        // Get the height of the node
        double cell_height = rCellPopulation.GetLocationOfCellCentre(*cell_iter)[2];
        c_vector<double, DIM> force;

        // Myoepithelial cells move towards the coverslip (ECM)
        if (cell_is_luminal || cell_is_luminal_stem)
        {
            if (cell_b1_expn && cell_b4_expn)
            {
                force[2]= -2.0 * mStiffness * cell_height;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
            else if (cell_b1_expn || cell_b4_expn)
            {
                force[2]= -1.0 * mStiffness * cell_height;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
            else
            {
                force[2]= -0.5 * mStiffness * cell_height;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
        }
        else if (cell_is_myo || cell_is_myo_stem)
        {
            if (cell_b1_expn && cell_b4_expn)
            {
                force[2]= -8.0 * mStiffness * cell_height;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
            else if (cell_b1_expn || cell_b4_expn)
            {
                force[2]= -4.0 * mStiffness * cell_height;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
            else
            {
                force[2]= -2.0 * mStiffness * cell_height;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
        }
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

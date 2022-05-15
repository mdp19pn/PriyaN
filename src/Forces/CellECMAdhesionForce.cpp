#include "CellECMAdhesionForce.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "CellLabel.hpp"
#include "Debug.hpp"

#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "LuminalStemCellProperty.hpp"
#include "MyoepithelialStemCellProperty.hpp"

template<unsigned DIM>
CellECMAdhesionForce<DIM>::CellECMAdhesionForce()
    : AbstractForce<DIM>(),
      mStiffness(1.0)
{
    assert(mStiffness > 0.0);
}

template<unsigned DIM>
CellECMAdhesionForce<DIM>::~CellECMAdhesionForce()
{
}

template<unsigned DIM>
void CellECMAdhesionForce<DIM>::SetStiffness(double stiffness)
{
    mStiffness = stiffness;
}


template<unsigned DIM>
void CellECMAdhesionForce<DIM>::AddForceContribution(AbstractCellPopulation<DIM>& rCellPopulation)
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
        else if (cell_is_myo_stem)
        {
            CellPropertyCollection collection = p_cell->rGetCellPropertyCollection().GetProperties<MyoepithelialStemCellProperty>();
            boost::shared_ptr<MyoepithelialStemCellProperty> p_prop = boost::static_pointer_cast<MyoepithelialStemCellProperty>(collection.GetProperty());
            cell_b1_expn = p_prop->GetB1IntegrinExpression();
            cell_b4_expn = p_prop->GetB4IntegrinExpression();
        }

        c_vector<double, DIM> location;
        location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);

        // Myoepithelial cells move away from the origin towards the ECM
        if (cell_is_luminal || cell_is_luminal_stem)
        {
            if (cell_b1_expn && cell_b4_expn)
            {
                c_vector<double, DIM> force = -2.0 * mStiffness * location;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
            else if (cell_b1_expn || cell_b4_expn)
            {
                c_vector<double, DIM> force = -4.0 * mStiffness * location;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
            else
            {
                c_vector<double, DIM> force = -8.0 * mStiffness * location;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
        }
        else if (cell_is_myo || cell_is_myo_stem)
        {
            if (cell_b1_expn && cell_b4_expn)
            {
                c_vector<double, DIM> force = -0.5 * mStiffness * location;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
            else if (cell_b1_expn || cell_b4_expn)
            {
                c_vector<double, DIM> force = -1.0 * mStiffness * location;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
            else
            {
                c_vector<double, DIM> force = -2.0 * mStiffness * location;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
        }
    }
}

template<unsigned DIM>
void CellECMAdhesionForce<DIM>::OutputForceParameters(out_stream& rParamsFile)
{
    // Output member variable
    *rParamsFile << "\t\t\t<Stiffness>" << mStiffness << "</Stiffness> \n";

    // Call direct parent class
    AbstractForce<DIM>::OutputForceParameters(rParamsFile);
}

// Explicit instantiation
template class CellECMAdhesionForce<1>;
template class CellECMAdhesionForce<2>;
template class CellECMAdhesionForce<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(CellECMAdhesionForce)
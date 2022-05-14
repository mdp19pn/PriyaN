#include "CellECMAdhesionForce.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "CellLabel.hpp"
#include "Debug.hpp"

#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"

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
        
        CellPtr p_cell = rCellPopulation.GetCellUsingLocationIndex(node_index);
        bool cell_is_luminal = p_cell->template HasCellProperty<LuminalCellProperty>();

        // Only labelled cells move inwards towards teh origin
        if (cell_is_luminal)
        {
            unsigned node_index = rCellPopulation.GetLocationIndexUsingCell(*cell_iter);

            c_vector<double, DIM> location;
            location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
            c_vector<double, DIM> force = -2.0 * mStiffness * location;
            rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
        }
        else
        {
            unsigned node_index = rCellPopulation.GetLocationIndexUsingCell(*cell_iter);

            c_vector<double, DIM> location;
            location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
            c_vector<double, DIM> force = -1.0 * mStiffness * location;
            rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
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
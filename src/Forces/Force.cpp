#include "Force.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "CellLabel.hpp"
#include "Debug.hpp"

template<unsigned DIM>
Force<DIM>::Force()
    : AbstractForce<DIM>(),
      mStiffness(1.0)
{
    assert(mStiffness > 0.0);
}

template<unsigned DIM>
Force<DIM>::~Force()
{
}

template<unsigned DIM>
void Force<DIM>::SetStiffness(double stiffness)
{
    mStiffness = stiffness;
}


template<unsigned DIM>
void Force<DIM>::AddForceContribution(AbstractCellPopulation<DIM>& rCellPopulation)
{
    /* Inside the method, we loop over cells, and add a vector to each node associated with cells 
     * with the CellLabel, which is proportional (with constant mStiffness) to the negative of the position. 
    */

    for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = rCellPopulation.Begin();
         cell_iter != rCellPopulation.End();
         ++cell_iter)
    {
        // Labelled cells move away from the origin
        if (cell_iter->template HasCellProperty<CellLabel>())
        {
            unsigned node_index = rCellPopulation.GetLocationIndexUsingCell(*cell_iter);

            c_vector<double, DIM> location;
            location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
            c_vector<double, DIM> force = -1.0 * mStiffness * location;
            rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
        }
        else
        {
            unsigned node_index = rCellPopulation.GetLocationIndexUsingCell(*cell_iter);

            c_vector<double, DIM> location;
            location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
            c_vector<double, DIM> force = -2.0 * mStiffness * location;
            rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
        }
    }
}

template<unsigned DIM>
void Force<DIM>::OutputForceParameters(out_stream& rParamsFile)
{
    // Output member variable
    *rParamsFile << "\t\t\t<Stiffness>" << mStiffness << "</Stiffness> \n";

    // Call direct parent class
    AbstractForce<DIM>::OutputForceParameters(rParamsFile);
}

// Explicit instantiation
template class Force<1>;
template class Force<2>;
template class Force<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(Force)
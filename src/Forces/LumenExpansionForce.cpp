#include "LumenExpansionForce.hpp"
#include "NodeBasedCellPopulation.hpp"

template<unsigned DIM>
LumenExpansionForce<DIM>::LumenExpansionForce()
    : AbstractForce<DIM>(),
      mStrength(5.0)
{
}

template<unsigned DIM>
double LumenExpansionForce<DIM>::GetStrength()
{
    return mStrength;
}

template<unsigned DIM>
void LumenExpansionForce<DIM>::SetStrength(double strength)
{
    mStrength = strength;
}


template<unsigned DIM>
void LumenExpansionForce<DIM>::AddForceContribution(AbstractCellPopulation<DIM>& rCellPopulation)
{
    // Iterate over cell population
    for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = rCellPopulation.Begin();
        cell_iter != rCellPopulation.End();
        ++cell_iter)
    {
        // Define the initiall organoid lumen by centre and radius 
        c_vector<double,3> organoid_centre;
        organoid_centre(0) = 0;
        organoid_centre(1) = 0;
        organoid_centre(2) = 0;

        double dt = SimulationTime::Instance()->GetTimeStep();

        c_vector<double, 3> force = zero_vector<double>(3);
        force(0) = -mStrength;
        force(1) = -mStrength;
        force(2) = -mStrength;

        for (unsigned node_index=0; node_index<rCellPopulation.GetNumNodes(); node_index++)
        {
            double x = rCellPopulation->GetNode(node_index)->rGetLocation()[0];
            double y = rCellPopulation->GetNode(node_index)->rGetLocation()[1];
            double z = rCellPopulation->GetNode(node_index)->rGetLocation()[2];

            Node<DIM>* p_node = rCellPopulation.GetNode(node_index);
            double strength = mStrength * (p_node->rGetLocation());

            // If the location of the node falls inside the defined lumen region...
            if ((x,y,z)!=(organoid_centre(0),organoid_centre(1),organoid_centre(2)))
            {
                // ...move the cell away from the lumen
                p_node->rGetModifiableLocation() += strength*dt;
            }

            rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
        }
    }
}

template<unsigned DIM>
void LumenExpansionForce<DIM>::OutputForceParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<Strength>" << mStrength << "</Strength>\n";
    
    // Call method on direct parent class
    AbstractForce<DIM>::OutputForceParameters(rParamsFile);
}

// Explicit instantiation
template class LumenExpansionForce<1>;
template class LumenExpansionForce<2>;
template class LumenExpansionForce<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(LumenExpansionForce)

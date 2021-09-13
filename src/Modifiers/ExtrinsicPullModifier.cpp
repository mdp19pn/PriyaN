#include "ExtrinsicPullModifier.hpp"
#include "NodeBasedCellPopulation.hpp"

template<unsigned DIM>
ExtrinsicPullModifier<DIM>::ExtrinsicPullModifier()
    : AbstractCellBasedSimulationModifier<DIM>(),
      mSpeed(1.0)
{
}

template<unsigned DIM>
ExtrinsicPullModifier<DIM>::~ExtrinsicPullModifier()
{
}

template<unsigned DIM>
void ExtrinsicPullModifier<DIM>::UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    // Define the initiall organoid lumen by centre and radius 
    c_vector<double,3> organoid_centre;
    organoid_centre(0) = 0;
    organoid_centre(1) = 0;
    organoid_centre(2) = 0;

    // unsigned num_nodes = rCellPopulation.GetNumNodes();
    // double dist_to_nb = 0.75;
    // double cell_radius = 0.5;
    // double lumen_radius = pow(num_nodes / 0.64, 1. / 3) * dist_to_nb / 2;

    // // Iterate over cell population
    // for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = rCellPopulation.Begin();
    //     cell_iter != rCellPopulation.End();
    //     ++cell_iter)
    // {
    //     // The increase of the lumen radius is proportional to the number of cells in each timestep. The cells are then pushed back.
    //     lumen_radius = pow(pow(lumen_radius,3)+num_nodes*pow(cell_radius,3), 1./3);

    //     for (unsigned node_index=0; node_index<num_nodes; node_index++)
    //     {
    //         c_vector<double, 3> cell_location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
            
    //         double previous_radius = norm_2(cell_location);
            
    //         // If the location of the node falls inside the defined lumen region...
    //         if (previous_radius<lumen_radius)
    //         {
    //             double speed = lumen_radius*(cell_location)/previous_radius;
                
    //             unsigned node_index = rCellPopulation.GetLocationIndexUsingCell(*cell_iter);
                
    //             Node<3>* p_node = rCellPopulation.GetNode(node_index);
                
    //             // ...move the cell away from the lumen
    //             p_node->rGetModifiableLocation() = speed;
    //         }
    //     }
    // }
}

template<unsigned DIM>
void ExtrinsicPullModifier<DIM>::SetupSolve(AbstractCellPopulation<DIM,DIM>& rCellPopulation, std::string outputDirectory)
{
}

template<unsigned DIM>
void ExtrinsicPullModifier<DIM>::SetSpeed(double speed)
{
    mSpeed = speed;
}

template<unsigned DIM>
void ExtrinsicPullModifier<DIM>::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<Speed>" << mSpeed << "</Speed>\n";

    // Next, call method on direct parent class
    AbstractCellBasedSimulationModifier<DIM>::OutputSimulationModifierParameters(rParamsFile);
}

// Explicit instantiation
template class ExtrinsicPullModifier<1>;
template class ExtrinsicPullModifier<2>;
template class ExtrinsicPullModifier<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(ExtrinsicPullModifier)
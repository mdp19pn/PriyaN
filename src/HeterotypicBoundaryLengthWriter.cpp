#include "HeterotypicBoundaryLengthWriter.hpp"
#include "AbstractCellPopulation.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
HeterotypicBoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::HeterotypicBoundaryLengthWriter()
    : AbstractCellPopulationWriter<ELEMENT_DIM, SPACE_DIM>("heterotypicboundary.dat")
{
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void HeterotypicBoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::Visit(NodeBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    // Make sure the cell population is updated so that mNodeNeighbours is set up
    ///\todo #2273 - check if this call to Update() is needed
    pCellPopulation->Update();

    // Initialise helper variables
    double heterotypic_boundary_length = 0.0;
    double total_shared_edges_length = 0.0;
    double num_heterotypic_pairs = 0.0;
    double total_num_pairs = 0.0;

    // Loop over cells
    for (typename AbstractCellPopulation<SPACE_DIM>::Iterator cell_iter = pCellPopulation->Begin();
         cell_iter != pCellPopulation->End();
         ++cell_iter)
    {
        // Store whether this cell is Luminal
        bool cell_is_labelled = cell_iter->template HasCellProperty<LuminalCellProperty>();

        // Store the radius of the node corresponding to this cell
        unsigned node_index = pCellPopulation->GetLocationIndexUsingCell(*cell_iter);
        double node_radius = pCellPopulation->GetNode(node_index)->GetRadius();

        // Get the set of neighbouring node indices
        std::set<unsigned> neighbour_indices = pCellPopulation->GetNeighbouringNodeIndices(node_index);

        if (!neighbour_indices.empty())
        {
            // Iterate over these neighbours
            for (std::set<unsigned>::iterator neighbour_iter = neighbour_indices.begin();
                 neighbour_iter != neighbour_indices.end();
                 ++neighbour_iter)
            {
                // Store the radius of the node corresponding to this neighbour
                double neighbour_radius = pCellPopulation->GetNode(*neighbour_iter)->GetRadius();

                // Get the (approximate) length of the edge shared with this neighbour
                double separation = pCellPopulation->rGetMesh().GetDistanceBetweenNodes(node_index, *neighbour_iter);
                double sum_of_radii = node_radius + neighbour_radius;

                // If the neighbours are close enough, then approximate their 'edge length'
                if (separation < sum_of_radii)
                {
                    // Use Heron's formula to compute the edge length
                    double a = node_radius;
                    double b = neighbour_radius;
                    double c = separation;
                    double s = 0.5*(a + b + c);
                    double A = sqrt(s*(s-a)*(s-b)*(s-c));
                    double edge_length = 4.0*A/c;

                    total_shared_edges_length += edge_length;
                    total_num_pairs += 1.0;

                    // Store whether this neighbour is myoepithelial
                    CellPtr p_neighbour_cell = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                    bool neighbour_is_labelled = p_neighbour_cell->template HasCellProperty<MyoepithelialCellProperty>();

                    // If this cell is labelled and its neighbour is not, or vice versa...
                    if (cell_is_labelled != neighbour_is_labelled)
                    {
                        // ... then increment the fractional boundary length
                        heterotypic_boundary_length += edge_length;
                        num_heterotypic_pairs += 1.0;
                    }
                }
            }
        }
    }

    // We have counted each cell-cell edge twice
    heterotypic_boundary_length *= 0.5;
    total_shared_edges_length *= 0.5;

    // We have counted each pair of neighbouring cells twice
    num_heterotypic_pairs *= 0.5;
    total_num_pairs *= 0.5;

    *this->mpOutStream << heterotypic_boundary_length << "\t" << total_shared_edges_length << "\t" << num_heterotypic_pairs << "\t" << total_num_pairs;
}

// Explicit instantiation
template class HeterotypicBoundaryLengthWriter<1,1>;
template class HeterotypicBoundaryLengthWriter<1,2>;
template class HeterotypicBoundaryLengthWriter<2,2>;
template class HeterotypicBoundaryLengthWriter<1,3>;
template class HeterotypicBoundaryLengthWriter<2,3>;
template class HeterotypicBoundaryLengthWriter<3,3>;

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(HeterotypicBoundaryLengthWriter)

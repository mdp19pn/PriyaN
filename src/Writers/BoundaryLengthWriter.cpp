#include "BoundaryLengthWriter.hpp"

#include "AbstractCellPopulation.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "CaBasedCellPopulation.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "PottsBasedCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"

#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "LuminalStemCellProperty.hpp"
#include "MyoepithelialStemCellProperty.hpp"
#include "Debug.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
BoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::BoundaryLengthWriter()
    : AbstractCellPopulationWriter<ELEMENT_DIM, SPACE_DIM>("heterotypicboundary.dat")
{
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void BoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::Visit(NodeBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    // Make sure the cell population is updated so that mNodeNeighbours is set up
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
        assert(cell_iter->HasApoptosisBegun() == false);

        // Store whether this cell is luminal or myoepithelial
        bool cell_is_luminal = cell_iter->template HasCellProperty<LuminalCellProperty>();
        bool cell_is_myoepithelial = cell_iter->template HasCellProperty<MyoepithelialCellProperty>();
        bool cell_is_luminal_stem = cell_iter->template HasCellProperty<LuminalStemCellProperty>();
        bool cell_is_myoepithelial_stem = cell_iter->template HasCellProperty<MyoepithelialStemCellProperty>();

        // Store the radius of the node corresponding to this cell
        unsigned node_index = pCellPopulation->GetLocationIndexUsingCell(*cell_iter);
        double node_radius = pCellPopulation->GetNode(node_index)->GetRadius();

        // Get the set of neighbouring node indices
        std::set<unsigned> neighbour_indices = pCellPopulation->GetNeighbouringNodeIndices(node_index);

        if (!neighbour_indices.empty())
        {
            PRINT_VARIABLE(SimulationTime::Instance()->GetTime());
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

                    // Store whether this neighbour is luminal or myoepithelial
                    CellPtr p_neighbour_cell_A = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                    bool neighbour_is_luminal = p_neighbour_cell_A->template HasCellProperty<LuminalCellProperty>();

                    CellPtr p_neighbour_cell_B = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                    bool neighbour_is_myoepithelial = p_neighbour_cell_B->template HasCellProperty<MyoepithelialCellProperty>();
                    
                    CellPtr p_neighbour_cell_C = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                    bool neighbour_is_luminal_stem = p_neighbour_cell_C->template HasCellProperty<LuminalStemCellProperty>();

                    CellPtr p_neighbour_cell_D = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                    bool neighbour_is_myoepithelial_stem = p_neighbour_cell_D->template HasCellProperty<MyoepithelialStemCellProperty>();

                    // If this cell is luminal and its neighbour is not, or vice versa...
                    if ((cell_is_luminal || cell_is_luminal_stem) != (neighbour_is_luminal || neighbour_is_luminal_stem))
                    {
                        // ... then increment the fractional boundary length
                        heterotypic_boundary_length += edge_length;
                        num_heterotypic_pairs += 1.0;
                    }
                    
                    // If this cell is luminal and its neighbour is not, or vice versa...
                    if ((cell_is_luminal || cell_is_luminal_stem) != (neighbour_is_myoepithelial || neighbour_is_myoepithelial_stem))
                    {
                        // ... then increment the fractional boundary length
                        heterotypic_boundary_length += 0;
                        num_heterotypic_pairs += 1.0;
                    }
                    
                    // If this cell is myoepithelial and its neighbour is not, or vice versa...
                    if ((cell_is_myoepithelial || cell_is_myoepithelial_stem) != (neighbour_is_luminal || neighbour_is_luminal_stem))
                    {
                        // ... then increment the fractional boundary length
                        heterotypic_boundary_length += 0;
                        num_heterotypic_pairs += 1.0;
                    }
                    
                    // If this cell is myoepithelial and its neighbour is not, or vice versa...
                    if ((cell_is_myoepithelial || cell_is_myoepithelial_stem) != (neighbour_is_myoepithelial || neighbour_is_myoepithelial_stem))
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

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void BoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::Visit(MeshBasedCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    EXCEPTION("BoundaryLengthWriter cannot be used with a MeshBasedCellPopulation");
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void BoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::Visit(CaBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    EXCEPTION("BoundaryLengthWriter cannot be used with a CaBasedCellPopulation");
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void BoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::Visit(PottsBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    EXCEPTION("BoundaryLengthWriter cannot be used with a PottsBasedCellPopulation");
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void BoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::Visit(VertexBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    EXCEPTION("BoundaryLengthWriter cannot be used with a VertexBasedCellPopulation");
}

// Explicit instantiation
template class BoundaryLengthWriter<1,1>;
template class BoundaryLengthWriter<1,2>;
template class BoundaryLengthWriter<2,2>;
template class BoundaryLengthWriter<1,3>;
template class BoundaryLengthWriter<2,3>;
template class BoundaryLengthWriter<3,3>;

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(BoundaryLengthWriter)

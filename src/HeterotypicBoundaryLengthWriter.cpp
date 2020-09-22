#include "HeterotypicBoundaryLengthWriter.hpp"

#include "AbstractCellPopulation.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "CaBasedCellPopulation.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "PottsBasedCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"

#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
HeterotypicBoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::HeterotypicBoundaryLengthWriter()
    : AbstractCellPopulationWriter<ELEMENT_DIM, SPACE_DIM>("heterotypicboundary.dat")
{
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void HeterotypicBoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::Visit(MeshBasedCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    // Initialise helper variables
    double heterotypic_boundary_length = 0.0;
    double total_shared_edges_length = 0.0;
    double num_heterotypic_pairs = 0.0;
    double total_num_pairs = 0.0;

    // Make sure the Voronoi tessellation has been created
    ///\todo #2273 - check if this call is needed
    pCellPopulation->CreateVoronoiTessellation();

    // Iterate over cells
    for (typename AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>::Iterator cell_iter = pCellPopulation->Begin();
         cell_iter != pCellPopulation->End();
         ++cell_iter)
    {
        // Get the location index corresponding to this cell
        unsigned index = pCellPopulation->GetLocationIndexUsingCell(*cell_iter);

        // Store whether this cell is luminal or myoepithelial
        bool cell_is_luminal = cell_iter->template HasCellProperty<LuminalCellProperty>();
        bool cell_is_myoepithelial = cell_iter->template HasCellProperty<MyoepithelialCellProperty>();

        // Get the set of neighbouring location indices
        std::set<unsigned> neighbour_indices = pCellPopulation->GetNeighbouringNodeIndices(index);

        // Iterate over these neighbours
        for (std::set<unsigned>::iterator neighbour_iter = neighbour_indices.begin();
             neighbour_iter != neighbour_indices.end();
             ++neighbour_iter)
        {
            // Get the length of the edge shared with this neighbour
            unsigned neighbour_index = *neighbour_iter;
            double edge_length = pCellPopulation->GetVoronoiEdgeLength(index,neighbour_index);

            // Ignore ghost nodes (in the case of a MeshBasedCellPopulationWithGhostNodes)
            ///\todo #2273 - check we have correctly dealt with ghost nodes
            if (!pCellPopulation->IsGhostNode(neighbour_index))
            {
                total_shared_edges_length += edge_length;
                total_num_pairs += 1.0;

                // Store whether this neighbour is luminal or myoepithelial
                CellPtr p_neighbour_cell_A = pCellPopulation->GetCellUsingLocationIndex(neighbour_index);
                bool neighbour_is_luminal = p_neighbour_cell_A->template HasCellProperty<LuminalCellProperty>();
                
                CellPtr p_neighbour_cell_B = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                bool neighbour_is_myoepithelial = p_neighbour_cell_B->template HasCellProperty<MyoepithelialCellProperty>();

                // If this cell is luminal and its neighbour is not, or vice versa...
                if (cell_is_luminal != neighbour_is_luminal)
                {
                    // ... then increment the fractional boundary length
                    heterotypic_boundary_length += edge_length;
                    num_heterotypic_pairs += 1.0;
                }
                
                // If this cell is luminal and its neighbour is not, or vice versa...
                if (cell_is_luminal != neighbour_is_myoepithelial)
                {
                    // ... then increment the fractional boundary length
                    heterotypic_boundary_length += 0;
                    num_heterotypic_pairs += 1.0;
                }
                
                // If this cell is myoepithelial and its neighbour is not, or vice versa...
                if (cell_is_myoepithelial != neighbour_is_luminal)
                {
                    // ... then increment the fractional boundary length
                    heterotypic_boundary_length += 0;
                    num_heterotypic_pairs += 1.0;
                }
                
                // If this cell is myoepithelial and its neighbour is not, or vice versa...
                if (cell_is_myoepithelial != neighbour_is_myoepithelial)
                {
                    // ... then increment the fractional boundary length
                    heterotypic_boundary_length += edge_length;
                    num_heterotypic_pairs += 1.0;
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
void HeterotypicBoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::Visit(CaBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    // Initialise helper variables
    double heterotypic_boundary_length = 0.0;
    double total_shared_edges_length = 0.0;
    double num_heterotypic_pairs = 0.0;
    double total_num_pairs = 0.0;

    // Iterate over cells
    for (typename AbstractCellPopulation<SPACE_DIM>::Iterator cell_iter = pCellPopulation->Begin();
         cell_iter != pCellPopulation->End();
         ++cell_iter)
    {
        // Get the location index corresponding to this cell
        unsigned index = pCellPopulation->GetLocationIndexUsingCell(*cell_iter);

        // Store whether this cell is luminal or myoepithelial
        bool cell_is_luminal = cell_iter->template HasCellProperty<LuminalCellProperty>();
        bool cell_is_myoepithelial = cell_iter->template HasCellProperty<MyoepithelialCellProperty>();

        // Get this node's von Neumann neighbours (not Moore neighbours, since they must share an edge)
        std::set<unsigned> neighbour_node_indices = pCellPopulation->rGetMesh().GetVonNeumannNeighbouringNodeIndices(index);

        // Iterate over these neighbours
        for (std::set<unsigned>::iterator neighbour_iter = neighbour_node_indices.begin();
             neighbour_iter != neighbour_node_indices.end();
             ++neighbour_iter)
        {
            // Assume the lattice is comprised of uniform unit lattice sites
            double edge_length = 1.0;

            unsigned neighbour_index = *neighbour_iter;

            if (pCellPopulation->IsCellAttachedToLocationIndex(neighbour_index))
            {
                CellPtr p_neighbour_cell_A = pCellPopulation->GetCellUsingLocationIndex(neighbour_index);
                CellPtr p_neighbour_cell_B = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                
                total_shared_edges_length += edge_length;
                total_num_pairs += 1.0;

                // Store whether this neighbour is luminal or myoepithelial
                bool neighbour_is_luminal = p_neighbour_cell_A->template HasCellProperty<LuminalCellProperty>();
                bool neighbour_is_myoepithelial = p_neighbour_cell_B->template HasCellProperty<MyoepithelialCellProperty>();
                
                // If this cell is luminal and its neighbour is not, or vice versa...
                if (cell_is_luminal != neighbour_is_luminal)
                {
                    // ... then increment the fractional boundary length
                    heterotypic_boundary_length += edge_length;
                    num_heterotypic_pairs += 1.0;
                }
                
                // If this cell is luminal and its neighbour is not, or vice versa...
                if (cell_is_luminal != neighbour_is_myoepithelial)
                {
                    // ... then increment the fractional boundary length
                    heterotypic_boundary_length += 0;
                    num_heterotypic_pairs += 1.0;
                }
                
                // If this cell is myoepithelial and its neighbour is not, or vice versa...
                if (cell_is_myoepithelial != neighbour_is_luminal)
                {
                    // ... then increment the fractional boundary length
                    heterotypic_boundary_length += 0;
                    num_heterotypic_pairs += 1.0;
                }
                
                // If this cell is myoepithelial and its neighbour is not, or vice versa...
                if (cell_is_myoepithelial != neighbour_is_myoepithelial)
                {
                    // ... then increment the fractional boundary length
                    heterotypic_boundary_length += edge_length;
                    num_heterotypic_pairs += 1.0;
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
        // Store whether this cell is luminal or myoepithelial
        bool cell_is_luminal = cell_iter->template HasCellProperty<LuminalCellProperty>();
        bool cell_is_myoepithelial = cell_iter->template HasCellProperty<MyoepithelialCellProperty>();

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

                    // Store whether this cell is luminal or myoepithelial
                    CellPtr p_neighbour_cell_A = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                    bool neighbour_is_luminal = p_neighbour_cell_A->template HasCellProperty<LuminalCellProperty>();

                    CellPtr p_neighbour_cell_B = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                    bool neighbour_is_myoepithelial = p_neighbour_cell_B->template HasCellProperty<MyoepithelialCellProperty>();
                    
                    // If this cell is luminal and its neighbour is not, or vice versa...
                    if (cell_is_luminal != neighbour_is_luminal)
                    {
                        // ... then increment the fractional boundary length
                        heterotypic_boundary_length += edge_length;
                        num_heterotypic_pairs += 1.0;
                    }
                    
                    // If this cell is luminal and its neighbour is not, or vice versa...
                    if (cell_is_luminal != neighbour_is_myoepithelial)
                    {
                        // ... then increment the fractional boundary length
                        heterotypic_boundary_length += 0;
                        num_heterotypic_pairs += 1.0;
                    }
                    
                    // If this cell is myoepithelial and its neighbour is not, or vice versa...
                    if (cell_is_myoepithelial != neighbour_is_luminal)
                    {
                        // ... then increment the fractional boundary length
                        heterotypic_boundary_length += 0;
                        num_heterotypic_pairs += 1.0;
                    }
                    
                    // If this cell is myoepithelial and its neighbour is not, or vice versa...
                    if (cell_is_myoepithelial != neighbour_is_myoepithelial)
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
void HeterotypicBoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::Visit(PottsBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    ///\todo #2273 - investigate whether there is a hard-coded assumption that neighbouring nodes in Potts simulations are unit distance apart

    // Initialise helper variables
    double heterotypic_boundary_length = 0.0;
    double total_shared_edges_length = 0.0;
    double num_heterotypic_pairs = 0.0;
    double total_num_pairs = 0.0;

    // Iterate over cells
    for (typename AbstractCellPopulation<SPACE_DIM>::Iterator cell_iter = pCellPopulation->Begin();
         cell_iter != pCellPopulation->End();
         ++cell_iter)
    {
        // Store whether this cell is luminal or myoepithelial
        bool cell_is_luminal = cell_iter->template HasCellProperty<LuminalCellProperty>();
        bool cell_is_myoepithelial = cell_iter->template HasCellProperty<MyoepithelialCellProperty>();


        unsigned elem_index = pCellPopulation->GetLocationIndexUsingCell(*cell_iter);
        unsigned num_nodes_in_elem = pCellPopulation->rGetMesh().GetElement(elem_index)->GetNumNodes();

        // Iterate over nodes contained in the element corresponding to this cell
        for (unsigned local_index=0; local_index<num_nodes_in_elem; local_index++)
        {
            // Get this node's von Neumann neighbours (not Moore neighbours, since they must share an edge)
            unsigned global_index = pCellPopulation->rGetMesh().GetElement(elem_index)->GetNodeGlobalIndex(local_index);
            std::set<unsigned> neighbour_node_indices = pCellPopulation->rGetMesh().GetVonNeumannNeighbouringNodeIndices(global_index);

            // Iterate over these neighbours
            for (std::set<unsigned>::iterator neighbour_iter = neighbour_node_indices.begin();
                 neighbour_iter != neighbour_node_indices.end();
                 ++neighbour_iter)
            {
                // Get the elements containing this neighbour
                std::set<unsigned> neighbour_elem_indices = pCellPopulation->GetNode(*neighbour_iter)->rGetContainingElementIndices();

                if (neighbour_elem_indices.size() == 1)
                {
                    unsigned neigbour_elem_index = *(neighbour_elem_indices.begin());

                    if (neigbour_elem_index != elem_index)
                    {
                        // Edge is between two different elements
                        total_shared_edges_length += 1.0;

                        // Store whether the cell corresponding to this element index is labelled
                        CellPtr p_neighbour_cell_A = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                        CellPtr p_neighbour_cell_B = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                        
                        bool neighbour_is_luminal = p_neighbour_cell_A->template HasCellProperty<LuminalCellProperty>();
                        bool neighbour_is_myoepithelial = p_neighbour_cell_B->template HasCellProperty<MyoepithelialCellProperty>();
                        
                        // If this cell is luminal and its neighbour is not, or vice versa...
                        if (cell_is_luminal != neighbour_is_luminal)
                        {
                            // ... then increment the fractional boundary length
                            heterotypic_boundary_length += edge_length;
                            num_heterotypic_pairs += 1.0;
                        }
                        
                        // If this cell is luminal and its neighbour is not, or vice versa...
                        if (cell_is_luminal != neighbour_is_myoepithelial)
                        {
                            // ... then increment the fractional boundary length
                            heterotypic_boundary_length += 0;
                            num_heterotypic_pairs += 1.0;
                        }
                        
                        // If this cell is myoepithelial and its neighbour is not, or vice versa...
                        if (cell_is_myoepithelial != neighbour_is_luminal)
                        {
                            // ... then increment the fractional boundary length
                            heterotypic_boundary_length += 0;
                            num_heterotypic_pairs += 1.0;
                        }
                        
                        // If this cell is myoepithelial and its neighbour is not, or vice versa...
                        if (cell_is_myoepithelial != neighbour_is_myoepithelial)
                        {
                            // ... then increment the fractional boundary length
                            heterotypic_boundary_length += 1.0;
                        }
                    }
                }
                else
                {
                    // Original node is on boundary of mesh so don't include in the edge calculation.
                }
            }
        }

        std::set<unsigned> neighbour_node_indices = pCellPopulation->GetNeighbouringLocationIndices(*cell_iter);

        // Iterate over these neighbours
        for (std::set<unsigned>::iterator neighbour_iter = neighbour_node_indices.begin();
             neighbour_iter != neighbour_node_indices.end();
             ++neighbour_iter)
        {
            total_num_pairs += 1.0;

            CellPtr p_neighbour_cell_A = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
            bool neighbour_is_luminal = p_neighbour_cell_A->template HasCellProperty<LuminalCellProperty>();

            CellPtr p_neighbour_cell_B = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
            bool neighbour_is_myoepithelial = p_neighbour_cell_B->template HasCellProperty<MyoepithelialCellProperty>();
            
            // If this cell is luminal and its neighbour is not, or vice versa...
            if (cell_is_luminal != neighbour_is_luminal)
            {
                // ... then increment the fractional boundary length
                heterotypic_boundary_length += edge_length;
                num_heterotypic_pairs += 1.0;
            }
                
            // If this cell is luminal and its neighbour is not, or vice versa...
            if (cell_is_luminal != neighbour_is_myoepithelial)
            {
                // ... then increment the fractional boundary length
                heterotypic_boundary_length += 0;
                num_heterotypic_pairs += 1.0;
            }
                
            // If this cell is myoepithelial and its neighbour is not, or vice versa...
            if (cell_is_myoepithelial != neighbour_is_luminal)
            {
                // ... then increment the fractional boundary length
                heterotypic_boundary_length += 0;
                num_heterotypic_pairs += 1.0;
            }
                
            // If this cell is myoepithelial and its neighbour is not, or vice versa...
            if (cell_is_myoepithelial != neighbour_is_myoepithelial)
            {
                // ... then increment the fractional boundary length
                num_heterotypic_pairs += 1.0;
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
void HeterotypicBoundaryLengthWriter<ELEMENT_DIM, SPACE_DIM>::Visit(VertexBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    // Make sure the cell population is updated
    ///\todo #2273 - check if this call to Update() is needed
    pCellPopulation->Update();

    // Initialise helper variables
    double heterotypic_boundary_length = 0.0;
    double total_shared_edges_length = 0.0;
    double num_heterotypic_pairs = 0.0;
    double total_num_pairs = 0.0;

    // Iterate over cells
    for (typename AbstractCellPopulation<SPACE_DIM>::Iterator cell_iter = pCellPopulation->Begin();
         cell_iter != pCellPopulation->End();
         ++cell_iter)
    {
        // Store whether this cell is luminal or myoepithelial
        bool cell_is_luminal = cell_iter->template HasCellProperty<LuminalCellProperty>();
        bool cell_is_myoepithelial = cell_iter->template HasCellProperty<MyoepithelialCellProperty>();

        // Get the set of neighbouring element indices
        unsigned elem_index = pCellPopulation->GetLocationIndexUsingCell(*cell_iter);
        std::set<unsigned> neighbour_elem_indices = pCellPopulation->rGetMesh().GetNeighbouringElementIndices(elem_index);

        // Iterate over these neighbours
        for (std::set<unsigned>::iterator neighbour_iter = neighbour_elem_indices.begin();
             neighbour_iter != neighbour_elem_indices.end();
             ++neighbour_iter)
        {
            // Get the length of the edge shared with this neighbour
            unsigned neighbour_index = *neighbour_iter;
            double edge_length = pCellPopulation->rGetMesh().GetEdgeLength(elem_index, neighbour_index);

            total_shared_edges_length += edge_length;
            total_num_pairs += 1.0;

            // Store whether this neighbour is labelled
            CellPtr p_neighbour_cell_A = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
            bool neighbour_is_luminal = p_neighbour_cell_A->template HasCellProperty<LuminalCellProperty>();
            
            CellPtr p_neighbour_cell_B = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
            bool neighbour_is_myoepithelial = p_neighbour_cell_B->template HasCellProperty<MyoepithelialCellProperty>();
            
            // If this cell is luminal and its neighbour is not, or vice versa...
            if (cell_is_luminal != neighbour_is_luminal)
            {
                // ... then increment the fractional boundary length
                heterotypic_boundary_length += edge_length;
                num_heterotypic_pairs += 1.0;
            }
                
            // If this cell is luminal and its neighbour is not, or vice versa...
            if (cell_is_luminal != neighbour_is_myoepithelial)
            {
                // ... then increment the fractional boundary length
                heterotypic_boundary_length += 0;
                num_heterotypic_pairs += 1.0;
            }
                
            // If this cell is myoepithelial and its neighbour is not, or vice versa...
            if (cell_is_myoepithelial != neighbour_is_luminal)
            {
                // ... then increment the fractional boundary length
                heterotypic_boundary_length += 0;
                num_heterotypic_pairs += 1.0;
            }
                
            // If this cell is myoepithelial and its neighbour is not, or vice versa...
            if (cell_is_myoepithelial != neighbour_is_myoepithelial)
            {
                // ... then increment the fractional boundary length
                heterotypic_boundary_length += edge_length;
                num_heterotypic_pairs += 1.0;
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

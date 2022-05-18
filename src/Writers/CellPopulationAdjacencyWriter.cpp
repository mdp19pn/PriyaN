#include "CellPopulationAdjacencyWriter.hpp"

#include "AbstractCellPopulation.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "CaBasedCellPopulation.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "PottsBasedCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"

#include "CellLabel.hpp"

#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "LuminalStemCellProperty.hpp"
#include "MyoepithelialStemCellProperty.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
CellPopulationAdjacencyWriter<ELEMENT_DIM, SPACE_DIM>::CellPopulationAdjacencyWriter()
    : AbstractCellPopulationWriter<ELEMENT_DIM, SPACE_DIM>("cellpopulationadjacency.dat")
{
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellPopulationAdjacencyWriter<ELEMENT_DIM, SPACE_DIM>::VisitAnyPopulation(AbstractCellPopulation<SPACE_DIM, SPACE_DIM>* pCellPopulation)
{
    // Make sure the cell population is updated
    ///\todo #2645 - if efficiency is an issue, check if this is really needed
    pCellPopulation->Update();

    unsigned num_cells = pCellPopulation->GetNumRealCells();

    // Store a map between cells numbered 1 to n and location indices
    std::map<unsigned,unsigned> local_cell_id_location_index_map;

    unsigned local_cell_id = 0;
    for (typename AbstractCellPopulation<SPACE_DIM, SPACE_DIM>::Iterator cell_iter = pCellPopulation->Begin();
         cell_iter != pCellPopulation->End();
         ++cell_iter)
    {
        local_cell_id_location_index_map[pCellPopulation->GetLocationIndexUsingCell(*cell_iter)] = local_cell_id;
        local_cell_id++;
    }
    assert(local_cell_id = num_cells+1);

    // Iterate over cells and calculate the adjacency matrix (stored as a long vector)
    std::vector<double> adjacency_matrix(num_cells*num_cells);
    for (unsigned i=0; i<num_cells*num_cells; i++)
    {
        adjacency_matrix[i] = 0;
    }

    for (typename AbstractCellPopulation<SPACE_DIM, SPACE_DIM>::Iterator cell_iter = pCellPopulation->Begin();
         cell_iter != pCellPopulation->End();
         ++cell_iter)
    {
        // Store whether this cell is luminal or myoepithelial
        bool cell_is_luminal = cell_iter->template HasCellProperty<LuminalCellProperty>();
        bool cell_is_myo = cell_iter->template HasCellProperty<MyoepithelialCellProperty>();
        bool cell_is_luminal_stem = cell_iter->template HasCellProperty<LuminalStemCellProperty>();
        bool cell_is_myo_stem = cell_iter->template HasCellProperty<MyoepithelialStemCellProperty>();


        // Get the location index corresponding to this cell
        unsigned index = pCellPopulation->GetLocationIndexUsingCell(*cell_iter);

        // Get the set of neighbouring location indices
        std::set<unsigned> neighbour_indices = pCellPopulation->GetNeighbouringLocationIndices(*cell_iter);

        // If this cell has any neighbours (as defined by mesh/population/interaction distance)...
        if (!neighbour_indices.empty())
        {
            unsigned local_cell_index = local_cell_id_location_index_map[index];

            for (std::set<unsigned>::iterator neighbour_iter = neighbour_indices.begin();
                 neighbour_iter != neighbour_indices.end();
                 ++neighbour_iter)
            {
                // If both cell_iter and p_neighbour_cell are not labelled, then set type_of_link to 1
                unsigned type_of_link = 1;

                // Determine whether this neighbour is luminal, myoepithelial or stem
                CellPtr p_neighbour_cell = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                bool neighbour_is_luminal = p_neighbour_cell->template HasCellProperty<LuminalCellProperty>();
                bool neighbour_is_myo = p_neighbour_cell->template HasCellProperty<MyoepithelialCellProperty>();
                bool neighbour_is_luminal_stem = p_neighbour_cell->template HasCellProperty<LuminalStemCellProperty>();
                bool neighbour_is_myo_stem = p_neighbour_cell->template HasCellProperty<MyoepithelialStemCellProperty>();

                if ((cell_is_luminal || cell_is_luminal_stem) != (neighbour_is_luminal || neighbour_is_luminal_stem))
                {
                    // Here cell_iter is luminal but p_neighbour_cell is not, or vice versa, so set type_of_link to 3
                    type_of_link = 3;
                }
                else if (cell_is_luminal || cell_is_luminal_stem)
                {
                    // Here both cell_iter and p_neighbour_cell are luminal, so set type_of_link to 2
                    type_of_link = 2;
                }

                unsigned local_neighbour_index = local_cell_id_location_index_map[*neighbour_iter];
                adjacency_matrix[local_cell_index + num_cells*local_neighbour_index] = type_of_link;
                adjacency_matrix[num_cells*local_cell_index + local_neighbour_index] = type_of_link;
            }
        }
    }

    // Output the number of cells and the elements of the adjacency matrix
    *this->mpOutStream << num_cells << "\t";
    for (unsigned i=0; i<num_cells*num_cells; i++)
    {
        *this->mpOutStream << adjacency_matrix[i] << "\t";
    }
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellPopulationAdjacencyWriter<ELEMENT_DIM, SPACE_DIM>::Visit(MeshBasedCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    // Make sure the cell population is updated
    ///\todo #2645 - if efficiency is an issue, check if this is really needed
    pCellPopulation->Update();

    unsigned num_cells = pCellPopulation->GetNumRealCells();

    // Store a map between cells numbered 1 to n and location indices
    std::map<unsigned,unsigned> local_cell_id_location_index_map;

    unsigned local_cell_id = 0;
    for (typename AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>::Iterator cell_iter = pCellPopulation->Begin();
         cell_iter != pCellPopulation->End();
         ++cell_iter)
    {
        local_cell_id_location_index_map[pCellPopulation->GetLocationIndexUsingCell(*cell_iter)] = local_cell_id;
        local_cell_id++;
    }
    assert(local_cell_id = num_cells+1);

    // Iterate over cells and calculate the adjacency matrix (stored as a long vector)
    std::vector<double> adjacency_matrix(num_cells*num_cells);
    for (unsigned i=0; i<num_cells*num_cells; i++)
    {
        adjacency_matrix[i] = 0;
    }

    for (typename AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>::Iterator cell_iter = pCellPopulation->Begin();
         cell_iter != pCellPopulation->End();
         ++cell_iter)
    {
        // Determine whether this cell is labelled
        bool cell_is_labelled = cell_iter->template HasCellProperty<CellLabel>();

        // Get the location index corresponding to this cell
        unsigned index = pCellPopulation->GetLocationIndexUsingCell(*cell_iter);

        // Get the set of neighbouring location indices
        std::set<unsigned> neighbour_indices = pCellPopulation->GetNeighbouringLocationIndices(*cell_iter);

        // If this cell has any neighbours (as defined by mesh/population/interaction distance)...
        if (!neighbour_indices.empty())
        {
            unsigned local_cell_index = local_cell_id_location_index_map[index];

            for (std::set<unsigned>::iterator neighbour_iter = neighbour_indices.begin();
                 neighbour_iter != neighbour_indices.end();
                 ++neighbour_iter)
            {
                // If both cell_iter and p_neighbour_cell are not labelled, then set type_of_link to 1
                unsigned type_of_link = 1;

                // Determine whether this neighbour is labelled
                CellPtr p_neighbour_cell = pCellPopulation->GetCellUsingLocationIndex(*neighbour_iter);
                bool neighbour_is_labelled = p_neighbour_cell->template HasCellProperty<CellLabel>();

                if (cell_is_labelled != neighbour_is_labelled)
                {
                    // Here cell_iter is labelled but p_neighbour_cell is not, or vice versa, so set type_of_link to 3
                    type_of_link = 3;
                }
                else if (cell_is_labelled)
                {
                    // Here both cell_iter and p_neighbour_cell are labelled, so set type_of_link to 2
                    type_of_link = 2;
                }

                unsigned local_neighbour_index = local_cell_id_location_index_map[*neighbour_iter];
                adjacency_matrix[local_cell_index + num_cells*local_neighbour_index] = type_of_link;
                adjacency_matrix[num_cells*local_cell_index + local_neighbour_index] = type_of_link;
            }
        }
    }

    // Output the number of cells and the elements of the adjacency matrix
    *this->mpOutStream << num_cells << "\t";
    for (unsigned i=0; i<num_cells*num_cells; i++)
    {
        *this->mpOutStream << adjacency_matrix[i] << "\t";
    }
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellPopulationAdjacencyWriter<ELEMENT_DIM, SPACE_DIM>::Visit(CaBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellPopulationAdjacencyWriter<ELEMENT_DIM, SPACE_DIM>::Visit(NodeBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    VisitAnyPopulation(pCellPopulation);
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellPopulationAdjacencyWriter<ELEMENT_DIM, SPACE_DIM>::Visit(PottsBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    VisitAnyPopulation(pCellPopulation);
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellPopulationAdjacencyWriter<ELEMENT_DIM, SPACE_DIM>::Visit(VertexBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    VisitAnyPopulation(pCellPopulation);
}

// Explicit instantiation
template class CellPopulationAdjacencyWriter<1,1>;
template class CellPopulationAdjacencyWriter<1,2>;
template class CellPopulationAdjacencyWriter<2,2>;
template class CellPopulationAdjacencyWriter<1,3>;
template class CellPopulationAdjacencyWriter<2,3>;
template class CellPopulationAdjacencyWriter<3,3>;

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellPopulationAdjacencyWriter)

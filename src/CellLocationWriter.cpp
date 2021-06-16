#include "CellLocationWriter.hpp"
#include "AbstractCellPopulation.hpp"
#include "CellVolumesWriter.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "Exception.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
CellLocationWriter<ELEMENT_DIM, SPACE_DIM>::CellLocationWriter()
    : AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>("location.dat")
{
    this->mVtkCellDataName = "Cell Location";
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellLocationWriter<ELEMENT_DIM, SPACE_DIM>::GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    double cell_id = pCell->GetCellId();
    return cell_id;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellLocationWriter<ELEMENT_DIM, SPACE_DIM>::VisitCell(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    if (dynamic_cast<NodeBasedCellPopulation<SPACE_DIM>*>(pCellPopulation))
    {
        unsigned node_index = pCellPopulation->GetLocationIndexUsingCell(pCell);
        Node<SPACE_DIM>* p_node = pCellPopulation->GetNode(node_index);
        
        // We should never encounter nodes that are deleted or associated with dead cells when calling this method
        assert(!p_node->IsDeleted());
        assert(!pCell->IsDead());

        // Write whether the cell is luminal or myoepithelial 
            if (pCell->HasCellProperty<LuminalCellProperty>())
            {
                *this->mpOutStream << "Luminal"<< " ";
            }
            else 
            {
                *this->mpOutStream << "Myoepithelial"<< " ";
            }
        
            // Write the cell's ID to file
            unsigned cell_id = pCell->GetCellId();
            *this->mpOutStream << cell_id << " ";

            // Write cell location
            c_vector<double, SPACE_DIM> cell_location = pCellPopulation->GetLocationOfCellCentre(pCell);
            for (unsigned i=0; i<SPACE_DIM; i++)
            {
                *this->mpOutStream << cell_location[i] << " ";
            }
    }
    else
    {
        EXCEPTION("This method is implemented only for a NodeBasedCellPopulation");
    }
}

// Explicit instantiation
template class CellLocationWriter<1,1>;
template class CellLocationWriter<1,2>;
template class CellLocationWriter<2,2>;
template class CellLocationWriter<1,3>;
template class CellLocationWriter<2,3>;
template class CellLocationWriter<3,3>;

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellLocationWriter)
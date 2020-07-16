#include "CellVelocityWriter.hpp"
#include "AbstractCellPopulation.hpp"


#include "CellVolumesWriter.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "Exception.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
CellVelocityWriter<ELEMENT_DIM, SPACE_DIM>::CellVelocityWriter()
    : AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>("velocity.dat")
{
    this->mVtkCellDataName = "Cell velocity";
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellVelocityWriter<ELEMENT_DIM, SPACE_DIM>::GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    double volume = pCellPopulation->GetVolumeOfCell(pCell);
    return volume;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellVelocityWriter<ELEMENT_DIM, SPACE_DIM>::VisitCell(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    if (dynamic_cast<NodeBasedCellPopulation<SPACE_DIM>*>(pCellPopulation))
    {
        unsigned node_index = pCellPopulation->GetLocationIndexUsingCell(pCell);
        
        // We should never encounter nodes that are deleted or associated with dead cells when calling this method
        assert(!node_iter->IsDeleted());
        if (!(pCellPopulation->IsGhostNode(node_index)))
        {
            assert(!pCell->IsDead());
        }
        
        unsigned cell_id = pCell->GetCellId();
        
        
        // Write the cell's ID to file
        *this->mpOutStream << cell_id << " ";

        // Write the cell's position to file
        const c_vector<double, SPACE_DIM>& position = pCellPopulation->GetNode(node_index)->rGetLocation();
        for (unsigned i=0; i<SPACE_DIM; i++)
        {
            *this->mpOutStream << position[i] << " ";
        }

        // Write the cell's velocity to file
        double time_step = SimulationTime::Instance()->GetTimeStep();
        double damping_constant = pCellPopulation->GetDampingConstant(node_index);
        c_vector<double, SPACE_DIM> velocity = time_step * node_iter->rGetAppliedForce() / damping_constant;
        for (unsigned i=0; i<SPACE_DIM; i++)
        {
            *this->mpOutStream << velocity[i] << " ";
        }      
    }
    else
    {
        EXCEPTION("This method is implemented only for a NodeBasedCellPopulation")
    }
}

// Explicit instantiation
template class CellVelocityWriter<1,1>;
template class CellVelocityWriter<1,2>;
template class CellVelocityWriter<2,2>;
template class CellVelocityWriter<1,3>;
template class CellVelocityWriter<2,3>;
template class CellVelocityWriter<3,3>;

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellVelocityWriter)

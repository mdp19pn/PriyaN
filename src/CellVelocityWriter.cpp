#include "CellVelocityWriter.hpp"
#include "AbstractCellPopulation.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "CaBasedCellPopulation.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "PottsBasedCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
CellVelocityWriter<ELEMENT_DIM, SPACE_DIM>::CellVelocityWriter()
    : AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>("cellvelocities.dat")
{
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellVelocityWriter<ELEMENT_DIM, SPACE_DIM>::Visit(MeshBasedCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    for (typename AbstractMesh<ELEMENT_DIM, SPACE_DIM>::NodeIterator node_iter = pCellPopulation->rGetMesh().GetNodeIteratorBegin();
         node_iter != pCellPopulation->rGetMesh().GetNodeIteratorEnd();
         ++node_iter)
    {
        // We should never encounter deleted nodes when calling this method
        assert(!node_iter->IsDeleted());

        unsigned node_index = node_iter->GetIndex();

        // Check that results should be written for this node
        bool is_real_node = !(pCellPopulation->IsGhostNode(node_index));

        // We should never encounter nodes associated with dead cells when calling this method
        if (is_real_node)
        {
            assert(!pCellPopulation->GetCellUsingLocationIndex(node_index)->IsDead());
        }

        if (is_real_node)
        {
            // Write this node's index to file
            *this->mpOutStream << node_index  << " ";

            // Write this node's position to file
            const c_vector<double, SPACE_DIM>& position = node_iter->rGetLocation();
            for (unsigned i=0; i<SPACE_DIM; i++)
            {
                *this->mpOutStream << position[i] << " ";
            }

            // Write this node's velocity to file
            double time_step = SimulationTime::Instance()->GetTimeStep(); ///\todo correct time step? (#2404)
            double damping_constant = pCellPopulation->GetDampingConstant(node_index);
            c_vector<double, SPACE_DIM> velocity = time_step * node_iter->rGetAppliedForce() / damping_constant;
            for (unsigned i=0; i<SPACE_DIM; i++)
            {
                *this->mpOutStream << velocity[i] << " ";
            }
        }
    }
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellVelocityWriter<ELEMENT_DIM, SPACE_DIM>::Visit(CaBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    EXCEPTION("CellVelocityWriter cannot be used with a CaBasedCellPopulation");
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellVelocityWriter<ELEMENT_DIM, SPACE_DIM>::Visit(NodeBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    for (typename AbstractMesh<SPACE_DIM, SPACE_DIM>::NodeIterator node_iter = pCellPopulation->rGetMesh().GetNodeIteratorBegin();
         node_iter != pCellPopulation->rGetMesh().GetNodeIteratorEnd();
         ++node_iter)
    {
        // We should never encounter deleted nodes when calling this method
        assert(!node_iter->IsDeleted());

        unsigned node_index = node_iter->GetIndex();

        // Check that results should be written for this node
        bool is_real_node = !(pCellPopulation->IsGhostNode(node_index));

        // We should never encounter nodes associated with dead cells when calling this method
        if (is_real_node)
        {
            assert(!pCellPopulation->GetCellUsingLocationIndex(node_index)->IsDead());
        }

        if (is_real_node)
        {
            // Write this node's index to file
            *this->mpOutStream << node_index  << " ";

            // Write this node's position to file
            const c_vector<double, SPACE_DIM>& position = node_iter->rGetLocation();
            for (unsigned i=0; i<SPACE_DIM; i++)
            {
                *this->mpOutStream << position[i] << " ";
            }

            // Write this node's velocity to file
            double time_step = SimulationTime::Instance()->GetTimeStep(); ///\todo correct time step? (#2404)
            double damping_constant = pCellPopulation->GetDampingConstant(node_index);
            c_vector<double, SPACE_DIM> velocity = time_step * node_iter->rGetAppliedForce() / damping_constant;
            for (unsigned i=0; i<SPACE_DIM; i++)
            {
                *this->mpOutStream << velocity[i] << " ";
            }
        }
    }
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellVelocityWriter<ELEMENT_DIM, SPACE_DIM>::Visit(PottsBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    EXCEPTION("CellVelocityWriter cannot be used with a PottsBasedCellPopulation");
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellVelocityWriter<ELEMENT_DIM, SPACE_DIM>::Visit(VertexBasedCellPopulation<SPACE_DIM>* pCellPopulation)
{
    for (typename AbstractMesh<SPACE_DIM, SPACE_DIM>::NodeIterator node_iter = pCellPopulation->rGetMesh().GetNodeIteratorBegin();
         node_iter != pCellPopulation->rGetMesh().GetNodeIteratorEnd();
         ++node_iter)
    {
        // We should never encounter deleted nodes when calling this method
        assert(!node_iter->IsDeleted());

        // Write this node's index to file
        unsigned node_index = node_iter->GetIndex();
        *this->mpOutStream << node_index  << " ";

        // Write this node's position to file
        const c_vector<double, SPACE_DIM>& position = node_iter->rGetLocation();
        for (unsigned i=0; i<SPACE_DIM; i++)
        {
            *this->mpOutStream << position[i] << " ";
        }

        // Write this node's velocity to file
        double time_step = SimulationTime::Instance()->GetTimeStep(); ///\todo correct time step? (#2404)
        double damping_constant = pCellPopulation->GetDampingConstant(node_index);
        c_vector<double, SPACE_DIM> velocity = time_step * node_iter->rGetAppliedForce() / damping_constant;
        for (unsigned i=0; i<SPACE_DIM; i++)
        {
            *this->mpOutStream << velocity[i] << " ";
        }
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

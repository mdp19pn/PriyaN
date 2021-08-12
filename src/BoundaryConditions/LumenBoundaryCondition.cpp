#include "LumenBoundaryCondition.hpp"
#include "NodeBasedCellPopulation.hpp"

template<unsigned DIM>
LumenBoundaryCondition<DIM>::LumenBoundaryCondition(AbstractCellPopulation<DIM>* pCellPopulation,
                                                                      c_vector<double, DIM> centre,
                                                                      double radius,
                                                                      double distance)
    : AbstractCellPopulationBoundaryCondition<DIM>(pCellPopulation),
      mCentreOfSphere(centre),
      mRadiusOfSphere(radius),
      mMaximumDistance(distance)
{
    assert(mRadiusOfSphere > 0.0);
    assert(mMaximumDistance > 3.0);

    if (dynamic_cast<NodeBasedCellPopulation<DIM>*>(this->mpCellPopulation) == nullptr)
    {
        EXCEPTION("A NodeBasedCellPopulation must be used with this boundary condition object.");
    }
    if (DIM == 1)
    {
        EXCEPTION("This boundary condition is not implemented in 1D.");
    }
}

template<unsigned DIM>
const c_vector<double, DIM>& LumenBoundaryCondition<DIM>::rGetCentreOfSphere() const
{
    return mCentreOfSphere;
}

template<unsigned DIM>
double LumenBoundaryCondition<DIM>::GetRadiusOfSphere() const
{
    return mRadiusOfSphere;
}

template<unsigned DIM>
void LumenBoundaryCondition<DIM>::ImposeBoundaryCondition(const std::map<Node<DIM>*, c_vector<double, DIM> >& rOldLocations)
{
    // Iterate over the cell population
    for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = this->mpCellPopulation->Begin();
         cell_iter != this->mpCellPopulation->End();
         ++cell_iter)
    {
        // Find the radial distance between this cell and the surface of the sphere
        c_vector<double,DIM> cell_location = this->mpCellPopulation->GetLocationOfCellCentre(*cell_iter);
        double radius = norm_2(cell_location - mCentreOfSphere);
        assert(radius != 0.0); //Can't project the centre to anywhere sensible

        // If the cell is too far from the surface of the sphere...
        if (fabs(radius - mRadiusOfSphere) > mMaximumDistance)
        {
            // ...move the cell back onto the surface of the sphere
            c_vector<double, DIM> location_on_sphere =
                mCentreOfSphere + mRadiusOfSphere*(cell_location - mCentreOfSphere)/radius;

            unsigned node_index = this->mpCellPopulation->GetLocationIndexUsingCell(*cell_iter);
            Node<DIM>* p_node = this->mpCellPopulation->GetNode(node_index);

            p_node->rGetModifiableLocation() = location_on_sphere;
        }
    }
}

template<unsigned DIM>
bool LumenBoundaryCondition<DIM>::VerifyBoundaryCondition()
{
    bool condition_satisfied = true;

    // Iterate over the cell population
    for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = this->mpCellPopulation->Begin();
         cell_iter != this->mpCellPopulation->End();
         ++cell_iter)
    {
        // Find the radial distance between this cell and the surface of the sphere
        c_vector<double,DIM> cell_location = this->mpCellPopulation->GetLocationOfCellCentre(*cell_iter);
        double radius = norm_2(cell_location - mCentreOfSphere);

        // If the cell is too far from the surface of the sphere...
        if (fabs(radius - mRadiusOfSphere) > mMaximumDistance)
        {
            // ...then the boundary condition is not satisfied
            condition_satisfied = false;
            break;
        }
    }
    return condition_satisfied;
}

template<unsigned DIM>
void LumenBoundaryCondition<DIM>::OutputCellPopulationBoundaryConditionParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<CentreOfSphere>";
    for (unsigned index=0; index != DIM-1U; index++) // Note: inequality avoids testing index < 0U when DIM=1
    {
        *rParamsFile << mCentreOfSphere[index] << ",";
    }
    *rParamsFile << mCentreOfSphere[DIM-1] << "</CentreOfSphere>\n";

    *rParamsFile << "\t\t\t<RadiusOfSphere>" << mRadiusOfSphere << "</RadiusOfSphere>\n";
    *rParamsFile << "\t\t\t<MaximumDistance>" << mMaximumDistance << "</MaximumDistance>\n";

    // Call method on direct parent class
    AbstractCellPopulationBoundaryCondition<DIM>::OutputCellPopulationBoundaryConditionParameters(rParamsFile);
}

// Explicit instantiation
template class LumenBoundaryCondition<1>;
template class LumenBoundaryCondition<2>;
template class LumenBoundaryCondition<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(LumenBoundaryCondition)

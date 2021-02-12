#include "PlaneBasedCellKiller.hpp"

template<unsigned DIM>
PlaneBasedCellKiller<DIM>::PlaneBasedCellKiller(AbstractCellPopulation<DIM>* pCellPopulation,
                                                  c_vector<double, DIM> point,
                                                  c_vector<double, DIM> normal)
    : AbstractCellKiller<DIM>(pCellPopulation),
      mPointOnPlane(point)
{
    assert(norm_2(normal) > 0.0);
    mNormalToPlane = normal/norm_2(normal);
}

template<unsigned DIM>
const c_vector<double, DIM>& PlaneBasedCellKiller<DIM>::rGetPointOnPlane() const
{
    return mPointOnPlane;
}

template<unsigned DIM>
const c_vector<double, DIM>& PlaneBasedCellKiller<DIM>::rGetNormalToPlane() const
{
    return mNormalToPlane;
}

template<unsigned DIM>
void PlaneBasedCellKiller<DIM>::CheckAndLabelCellsForApoptosisOrDeath()
{
    for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = this->mpCellPopulation->Begin();
         cell_iter != this->mpCellPopulation->End();
         ++cell_iter)
    {
        c_vector<double, DIM> cell_location = this->mpCellPopulation->GetLocationOfCellCentre(*cell_iter);

        if (inner_prod(cell_location - mPointOnPlane, mNormalToPlane) > 0.0)
        {
            cell_iter->Kill();
        }
    }
}

template<unsigned DIM>
void PlaneBasedCellKiller<DIM>::OutputCellKillerParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<PointOnPlane>";
    for (unsigned index=0; index != DIM-1U; index++) //Note: inequality avoids testing index < 0U when DIM=1
    {
        *rParamsFile << mPointOnPlane[index] << ",";
    }
    *rParamsFile << mPointOnPlane[DIM-1] << "</PointOnPlane>\n";

    *rParamsFile << "\t\t\t<NormalToPlane>";
     for (unsigned index=0; index != DIM-1U; index++) //Note: inequality avoids testing index < 0U when DIM=1
     {
         *rParamsFile << mNormalToPlane[index] << ",";
     }
     *rParamsFile << mNormalToPlane[DIM-1] << "</NormalToPlane>\n";

    // Call method on direct parent class
    AbstractCellKiller<DIM>::OutputCellKillerParameters(rParamsFile);
}

// Explicit instantiation
template class PlaneBasedCellKiller<1>;
template class PlaneBasedCellKiller<2>;
template class PlaneBasedCellKiller<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(PlaneBasedCellKiller)

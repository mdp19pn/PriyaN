#include "AnoikisCellKiller3D.hpp"
#include "AbstractCentreBasedCellPopulation.hpp"
#include "NodeBasedCellPopulationWithParticles.hpp"
#include "NodeBasedCellPopulation.hpp"

AnoikisCellKiller3D::AnoikisCellKiller3D(AbstractCellPopulation<3>* pCellPopulation, c_vector<double,3> centre, double radius)
    : AbstractCellKiller<3>(pCellPopulation),
      mCentre(centre),
      mRadius(radius)
{
}

c_vector<double,3> AnoikisCellKiller3D::GetCentre() const
{
    return mCentre;
}

double AnoikisCellKiller3D::GetRadius() const
{
    return mRadius;
}

void AnoikisCellKiller3D::CheckAndLabelCellsForApoptosisOrDeath()
{
    // for (AbstractCellPopulation<3>::Iterator cell_iter = this->mpCellPopulation->Begin();
    //      cell_iter != this->mpCellPopulation->End();
    //      ++cell_iter)
    // {
    //     // Get distance from centre of cell population
    //     double r = norm_2(this->mpCellPopulation->GetLocationOfCellCentre(*cell_iter) - mCentre);

    //     if (r < mRadius)
    //     {
    //         cell_iter->Kill();
    //     }
    // }
}

void AnoikisCellKiller3D::OutputCellKillerParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<xCentre>" << mCentre[0] << "</xCentre>\n";
    *rParamsFile << "\t\t\t<yCentre>" << mCentre[1] << "</yCentre>\n";
    *rParamsFile << "\t\t\t<yCentre>" << mCentre[2] << "</zCentre>\n";
    *rParamsFile << "\t\t\t<mRadius>" << mRadius << "</mRadius>\n";

    // Call method on direct parent class
    AbstractCellKiller<3>::OutputCellKillerParameters(rParamsFile);
}

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(AnoikisCellKiller3D)

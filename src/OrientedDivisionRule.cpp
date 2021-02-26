#include "OrientedDivisionRule.hpp"
#include "RandomNumberGenerator.hpp"

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
std::pair<c_vector<double, SPACE_DIM>, c_vector<double, SPACE_DIM> > OrientedDivisionRule<ELEMENT_DIM, SPACE_DIM>::CalculateCellDivisionVector(
    CellPtr pParentCell,
    AbstractCentreBasedCellPopulation<ELEMENT_DIM, SPACE_DIM>& rCellPopulation)
{
    // Get separation parameter
    double separation = rCellPopulation.GetMeinekeDivisionSeparation();

    // Make a random direction vector of the required length
    c_vector<double, SPACE_DIM> random_vector;

    /*
     * Pick a random direction and move the parent cell backwards by 0.5*separation
     * in that direction and return the position of the daughter cell 0.5*separation
     * forwards in that direction.
     */
    switch (SPACE_DIM)
    {
        case 1:
        {
            double random_direction = -1.0 + 2.0*(RandomNumberGenerator::Instance()->ranf() < 0.5);

            random_vector(0) = 0.5*separation*random_direction;
            break;
        }
        case 2:
        {
            double random_angle = 2.0*M_PI*RandomNumberGenerator::Instance()->ranf();

            random_vector(0) = 0.5*separation*cos(random_angle);
            random_vector(1) = 0.5*separation*sin(random_angle);
            break;
        }
        case 3:
        {
            /*
             * Note that to pick a random point on the surface of a sphere, it is incorrect
             * to select spherical coordinates from uniform distributions on [0, 2*pi) and
             * [0, pi) respectively, since points picked in this way will be 'bunched' near
             * the poles. See #2230.
             */
            double u = RandomNumberGenerator::Instance()->ranf();
            double v = RandomNumberGenerator::Instance()->ranf();

            double random_azimuth_angle = 2*M_PI*u;
            double random_zenith_angle = std::acos(2*v - 1);

            random_vector(0) = 0.5*separation*cos(random_azimuth_angle)*sin(random_zenith_angle);
            random_vector(1) = 0.5*separation*sin(random_azimuth_angle)*sin(random_zenith_angle);
            random_vector(2) = 0.0;
            break;
        }
        default:
            // This can't happen
            NEVER_REACHED;
    }

    c_vector<double, SPACE_DIM> parent_position = rCellPopulation.GetLocationOfCellCentre(pParentCell) - random_vector;
    c_vector<double, SPACE_DIM> daughter_position = parent_position + random_vector;

    std::pair<c_vector<double, SPACE_DIM>, c_vector<double, SPACE_DIM> > positions(parent_position, daughter_position);

    return positions;
}

// Explicit instantiation
template class OrientedDivisionRule<1,1>;
template class OrientedDivisionRule<1,2>;
template class OrientedDivisionRule<2,2>;
template class OrientedDivisionRule<1,3>;
template class OrientedDivisionRule<2,3>;
template class OrientedDivisionRule<3,3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(OrientedDivisionRule)

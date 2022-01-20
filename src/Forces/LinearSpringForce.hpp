#ifndef LINEARSPRINGFORCE_HPP_
#define LINEARSPRINGFORCE_HPP_

#include "AbstractTwoBodyInteractionForce.hpp"
#include "GeneralisedLinearSpringForce.hpp"

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

/**
 * A force law employed by Meineke et al (2001) in their off-lattice
 * model of the intestinal crypt (doi:10.1046/j.0960-7722.2001.00216.x).
 *
 * Each pair of neighbouring nodes are assumed to be connected by a linear
 * spring. The force of node \f$i\f$ is given
 * by
 *
 * \f[
 * \mathbf{F}_{i}(t) = \sum_{j} \mu_{i,j} ( || \mathbf{r}_{i,j} || - s_{i,j}(t) ) \hat{\mathbf{r}}_{i,j}.
 * \f]
 *
 * Here \f$\mu_{i,j}\f$ is the spring constant for the spring between nodes
 * \f$i\f$ and \f$j\f$, \f$s_{i,j}(t)\f$ is its natural length at time \f$t\f$,
 * \f$\mathbf{r}_{i,j}\f$ is their relative displacement and a hat (\f$\hat{}\f$)
 * denotes a unit vector.
 *
 * Length is scaled by natural length.
 * Time is in hours.
 */
template<unsigned  ELEMENT_DIM, unsigned SPACE_DIM=ELEMENT_DIM>
class LinearSpringForce : public AbstractTwoBodyInteractionForce<ELEMENT_DIM, SPACE_DIM>
{
    friend class TestForces;

private:

    /**
     * A scalar determining the relative spring constant for homotypic
     * interactions between neighbouring LE-LE/ME-ME,SLE-SLE,SME-SME cells, used in the
     * overridden method VariableSpringConstantMultiplicationFactor().
     *
     * Defaults to 1.0 in the constructor.
     */
    double mHomotypicSpringConstantMultiplier;

    /**
     * A scalar determining the relative spring constant for heterotypic
     * (LE-ECM, ME-ECM, SLE-ECM, SME-ECM) interactions between neighbouring cells, used
     * in the overridden method VariableSpringConstantMultiplicationFactor().
     *
     * Defaults to 1.0 in the constructor.
     */
    double mHeterotypicSpringConstantMultiplier;

    /** Needed for serialization. */
    friend class boost::serialization::access;
    /**
     * Archive the object and its member variables.
     *
     * @param archive the archive
     * @param version the current version of this class
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractTwoBodyInteractionForce<ELEMENT_DIM, SPACE_DIM> >(*this);
        archive & mCellCellSpringStiffness;
        archive & mCellECMSpringStiffness;
        archive & mECMECMSpringStiffness;
        archive & mMeinekeDivisionRestingSpringLength;
        archive & mMeinekeSpringGrowthDuration;
        
        archive & boost::serialization::base_object<GeneralisedLinearSpringForce<ELEMENT_DIM, SPACE_DIM> >(*this);
        archive & mHomotypicSpringConstantMultiplier;
        archive & mHeterotypicSpringConstantMultiplier;
    }

protected:
    
    /**
     * Cell cell Spring stiffness.
     */
    double mCellCellSpringStiffness;

    /**
     * Cell ECM Spring stiffness.
     */
    double mCellECMSpringStiffness;

    /**
     * ECM ECM Spring stiffness.
     */
    double mECMECMSpringStiffness;

    /**
     * Initial resting spring length after cell division.
     * Has units of cell size at equilibrium rest length
     *
     * The value of this parameter should be larger than mDivisionSeparation,
     * because of pressure from neighbouring springs.
     */
    double mMeinekeDivisionRestingSpringLength;

    /**
     * The time it takes for the springs rest length to increase from
     * mMeinekeDivisionRestingSpringLength to its natural length.
     *
     * The value of this parameter is usually the same as the M Phase of the cell cycle and defaults to 1.
     */
    double mMeinekeSpringGrowthDuration;

    double mHomotypicSpringConstantMultiplier;
    
    double mHeterotypicSpringConstantMultiplier;

public:

    /**
     * Constructor.
     */
    LinearSpringForce();

    /**
     * Destructor.
     */
    virtual ~LinearSpringForce();

    /**
     * Return a multiplication factor for the spring constant, which
     * returns a default value of 1.
     *
     * This method may be overridden in subclasses.
     *
     * @param nodeAGlobalIndex index of one neighbouring node
     * @param nodeBGlobalIndex index of the other neighbouring node
     * @param rCellPopulation the cell population
     * @param isCloserThanRestLength whether the neighbouring nodes lie closer than the rest length of their connecting spring
     *
     * @return the multiplication factor.
     */
    virtual double VariableSpringConstantMultiplicationFactor(unsigned nodeAGlobalIndex,
                                                              unsigned nodeBGlobalIndex,
                                                              AbstractCellPopulation<ELEMENT_DIM,SPACE_DIM>& rCellPopulation,
                                                              bool isCloserThanRestLength);

    /**
     * Overridden CalculateForceBetweenNodes() method.
     *
     * Calculates the force between two nodes.
     *
     * Note that this assumes they are connected and is called by AddForceContribution()
     *
     * @param nodeAGlobalIndex index of one neighbouring node
     * @param nodeBGlobalIndex index of the other neighbouring node
     * @param rCellPopulation the cell population
     * @return The force exerted on Node A by Node B.
     */
    c_vector<double, SPACE_DIM> CalculateForceBetweenNodes(unsigned nodeAGlobalIndex,
                                                     unsigned nodeBGlobalIndex,
                                                     AbstractCellPopulation<ELEMENT_DIM,SPACE_DIM>& rCellPopulation);

    /**
     * @return mCellCellSpringStiffness
     */
    double GetCellCellSpringStiffness();

    /**
     * @return mCellECMSpringStiffness
     */
    double GetCellECMSpringStiffness();

    /**
     * @return mCellCellSpringStiffness
     */
    double GetECMECMSpringStiffness();

    /**
     * @return mMeinekeDivisionRestingSpringLength
     */
    double GetMeinekeDivisionRestingSpringLength();

    /**
     * @return mMeinekeSpringGrowthDuration
     */
    double GetMeinekeSpringGrowthDuration();

    /**
     * Set mCellCellSpringStiffness
     *
     * @param cellcellSpringStiffness the new value of mCellCellSpringStiffness
     */
    void SetCellCellSpringStiffness(double cellcellSpringStiffness);

    /**
     * Set mCellECMSpringStiffness
     *
     * @param cellECMSpringStiffness the new value of mCellECMSpringStiffness
     */
    void SetCellECMSpringStiffness(double cellECMSpringStiffness);

    /**
     * Set mECMECMSpringStiffness
     *
     * @param eCMECMSpringStiffness the new value of mECMECMSpringStiffness
     */
    void SetECMECMSpringStiffness(double eCMECMSpringStiffness);

    /**
     * Set mMeinekeDivisionRestingSpringLength.
     *
     * @param divisionRestingSpringLength the new value of mMeinekeDivisionRestingSpringLength
     */
    void SetMeinekeDivisionRestingSpringLength(double divisionRestingSpringLength);

    /**
     * Set mMeinekeSpringGrowthDuration.
     *
     * @param springGrowthDuration the new value of mMeinekeSpringGrowthDuration
     */
    void SetMeinekeSpringGrowthDuration(double springGrowthDuration);
    
    /**
      * @return #mHomotypicSpringConstantMultiplier.
      */
     double GetHomotypicSpringConstantMultiplier();

     /**
      * Set mHomotypicSpringConstantMultiplier.
      *
      * @param SpringConstantMultiplier the new value of mHomotypicSpringConstantMultiplier
      */
     void SetHomotypicSpringConstantMultiplier(double SpringConstantMultiplier);

     /**
      * @return #mHeterotypicSpringConstantMultiplier.
      */
     double GetHeterotypicSpringConstantMultiplier();

     /**
      * Set mHeterotypicSpringConstantMultiplier.
      *
      * @param heterotypicSpringConstantMultiplier the new value of mHeterotypicSpringConstantMultiplier
      */
     void SetHeterotypicSpringConstantMultiplier(double heterotypicSpringConstantMultiplier);

    /**
     * Overridden OutputForceParameters() method.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    virtual void OutputForceParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_ALL_DIMS(LinearSpringForce)

#endif /*LINEARSPRINGFORCE_HPP_*/

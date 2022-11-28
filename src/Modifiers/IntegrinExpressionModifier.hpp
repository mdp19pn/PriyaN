#ifndef INTEGRINEXPRESSIONMODIFIER_HPP_
#define INTEGRINEXPRESSIONMODIFIER_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

#include "AbstractCellBasedSimulationModifier.hpp"

/**
 * A modifier class which at each simulation time step checks whether a certain time has elapsed, 
 * and as soon as it has, alters the integrin expression of one or more mammary cell types 
 * present in a simulation.
 */
template<unsigned DIM>
class IntegrinExpressionModifier : public AbstractCellBasedSimulationModifier<DIM,DIM>
{
private:

    /**
     * Boolean encoding whether integrin expression has been modified. Defaults to false and 
     * is reset to true as soon as mIntegrinExpressionModificationTime has elapsed, to avoid 
     * modifying integrin expression more than once.
     */
	bool mIntegrinExpressionModified;
	
	/**
	 * Double encoding time at which integrin expression should be modified. Defaults to zero.
	 */
	double mIntegrinExpressionModificationTime;
	
    /**
     * Boolean encoding whether there is a B1 integrin gain of function.
     */
	bool mB1GainOfFunction;
	
    /**
     * Boolean encoding whether there is a B1 integrin loss of function.
     */	
	bool mB1LossOfFunction;
	
    /**
     * Boolean encoding whether there is a B4 integrin gain of function.
     */
	bool mB4GainOfFunction;
	
    /**
     * Boolean encoding whether there is a B4 integrin loss of function.
     */	
	bool mB4LossOfFunction;
	  
    /** Needed for serialization. */
    friend class boost::serialization::access;
    /**
     * Boost Serialization method for archiving/checkpointing.
     * Archives the object and its member variables.
     *
     * @param archive  The boost archive.
     * @param version  The current version of this class.
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellBasedSimulationModifier<DIM,DIM> >(*this);
		
		archive & mIntegrinExpressionModified;
		archive & mIntegrinExpressionModificationTime;
		archive & mB1GainOfFunction;
		archive & mB1LossOfFunction;
		archive & mB4GainOfFunction;
		archive & mB4LossOfFunction;
    }

public:

    /**
     * Default constructor.
     */
    IntegrinExpressionModifier();

    /**
     * Destructor.
     */
    virtual ~IntegrinExpressionModifier();

    /**
     * Overridden UpdateAtEndOfTimeStep() method.
     *
     * Specify what to do in the simulation at the end of each time step.
     *
     * @param rCellPopulation reference to the cell population
     */
    virtual void UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM,DIM>& rCellPopulation);

    /**
     * Overridden SetupSolve() method.
     *
     * Specify what to do in the simulation before the start of the time loop.
     *
     * @param rCellPopulation reference to the cell population
     * @param outputDirectory the output directory, relative to where Chaste output is stored
     */
    virtual void SetupSolve(AbstractCellPopulation<DIM,DIM>& rCellPopulation, std::string outputDirectory);

    /**
     * Helper method to compute the volume of each cell in the population and store these in the CellData.
     *
     * @param rCellPopulation reference to the cell population
     */
    void UpdateCellData(AbstractCellPopulation<DIM,DIM>& rCellPopulation);

    /**
     * Overridden OutputSimulationModifierParameters() method.
     * Output any simulation modifier parameters to file.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    void OutputSimulationModifierParameters(out_stream& rParamsFile);
	
	/**
     * Set mB1GainOfFunction.
     *
     * @param b1GainOfFunction Boolean encoding whether there is a B1 integrin gain of function.
     */
	void SetB1GainOfFunction(bool b1GainOfFunction);
	
	/**
     * Set mB1LossOfFunction.
     *
     * @param b1LossOfFunction Boolean encoding whether there is a B1 integrin loss of function.
     */
    void SetB1LossOfFunction(bool b1LossOfFunction);
		
	/**
     * Set mB4GainOfFunction.
     *
     * @param b4GainOfFunction Boolean encoding whether there is a B4 integrin gain of function.
     */
	void SetB4GainOfFunction(bool b4GainOfFunction);
		
	/**
     * Set mB4LossOfFunction.
     *
     * @param b4LossOfFunction Boolean encoding whether there is a B4 integrin loss of function.
     */
	void SetB4LossOfFunction(bool b4LossOfFunction);
}

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(IntegrinExpressionModifier)

#endif /*INTEGRINEXPRESSIONMODIFIER_HPP_*/

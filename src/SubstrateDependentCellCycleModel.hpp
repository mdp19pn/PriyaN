#ifndef SUBSTRATEDEPENDENTCELLCYCLEMODEL_HPP_
#define SUBSTRATEDEPENDENTCELLCYCLEMODEL_HPP_

#include "AbstractSimplePhaseBasedCellCycleModel.hpp"

/**
 * A simple anchorage-dependent cell-cycle model that inherits from
 * AbstractSimplePhaseBasedCellCycleModel. The duration of G1 phase depends
 * on the area of contact, interpreted here as deviation from target
 * cell height.
 *
 * This model allows for quiescence imposed by transient periods
 * of high stress, followed by relaxation.
 *
 * Note that in this cell cycle model, quiescence is implemented
 * by extending the G1 phase. If a cell is compressed during G2
 * or S phases then it will still divide, and thus cells whose
 * cell height are smaller than the given threshold may still divide.
 */
class SubstrateDependentCellCycleModel : public AbstractSimplePhaseBasedCellCycleModel
{
private:

    friend class boost::serialization::access;

    /**
     * Boost Serialization method for archiving/checkpointing
     * @param archive  The boost archive.
     * @param version  The current version of this class.
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractSimplePhaseBasedCellCycleModel>(*this);
        archive & mQuiescentHeightFraction;
        archive & mEquilibriumHeight;
        archive & mCurrentQuiescentDuration;
        archive & mCurrentQuiescentOnsetTime;
    }

protected:

    /** The cell that this model is associated with. */
    // CellPtr p_cell_A;
    // CellPtr p_cell_B;

    /**
     * The fraction of the cells' equilibrium height in G1 phase below which these cells are quiescent.
     */
    double mQuiescentHeightFraction;

    /**
     * The cell equilibrium height while in G1 phase.
     */
    double mEquilibriumHeight;

    /**
     * The time when the current period of quiescence began.
     */
    double mCurrentQuiescentOnsetTime;

    /**
     * How long the current period of quiescence has lasted.
     * Has units of hours.
     */
    double mCurrentQuiescentDuration;

    /**
     * Protected copy-constructor for use by CreateCellCycleModel.
     * The only way for external code to create a copy of a cell cycle model
     * is by calling that method, to ensure that a model of the correct subclass is created.
     * This copy-constructor helps subclasses to ensure that all member variables are correctly copied when this happens.
     *
     * This method is called by child classes to set member variables for a daughter cell upon cell division.
     * Note that the parent cell cycle model will have had ResetForDivision() called just before CreateCellCycleModel() is called,
     * so performing an exact copy of the parent is suitable behaviour. Any daughter-cell-specific initialisation
     * can be done in InitialiseDaughterCell().
     *
     * @param rModel the cell cycle model to copy.
     */
    SubstrateDependentCellCycleModel(const SubstrateDependentCellCycleModel& rModel);

public:

    /**
     * Constructor.
     */
    SubstrateDependentCellCycleModel();

    /**
     * Overridden UpdateCellCyclePhase() method.
     */
    void UpdateCellCyclePhase();

    /**
     * Overridden builder method to create new instances of
     * the cell-cycle model.
     *
     * @return new cell-cycle model
     *
     */
    AbstractCellCycleModel* CreateCellCycleModel();

    /**
     * Gives the cell-cycle model a pointer to its host cell.
     *
     * Some cell-cycle models pass this pointer to other classes,
     * which use this information to determine other information based upon the location
     * of the cell (e.g. the Wnt concentration at this location).
     *
     * @param pCell pointer to the cell
     */
    //void SetCell(CellPtr pCell);

    /**
     * @param quiescentHeightFraction
     */
    void SetQuiescentHeightFraction(double quiescentHeightFraction);

    /**
     * @return mQuiescentHeightFraction
     */
    double GetQuiescentHeightFraction() const;

    /**
     * @param equilibriumHeight
     */
    void SetEquilibriumHeight(double equilibriumHeight);

    /**
     * @return mEquilibriumHeight
     */
    double GetEquilibriumHeight() const;

    /**
     * @return mCurrentQuiescentDuration
     */
    double GetCurrentQuiescentDuration() const;

    /**
     * @return mCurrentQuiescentOnsetTime
     */
    double GetCurrentQuiescentOnsetTime() const;

    /**
     * Overridden OutputCellCycleModelParameters() method.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    virtual void OutputCellCycleModelParameters(out_stream& rParamsFile);
};

// Declare identifier for the serializer
#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(SubstrateDependentCellCycleModel)

#endif // SUBSTRATEDEPENDENTCELLCYCLEMODEL_HPP_

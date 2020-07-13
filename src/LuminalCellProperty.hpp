#ifndef LUMINALCELLPROPERTY_HPP_
#define LUMINALCELLPROPERTY_HPP_

/*
 * = Include necessary header files
 */
#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

/* The next header defines a base class for cell properties. Our new
 * cell property will inherit from this abstract class. */
#include "AbstractCellProperty.hpp"
/* The remaining header files define classes that will be used in the cell-based
 * simulation test. We have encountered each of these header files in previous cell-based
 * Chaste tutorials. */
#include "AbstractForce.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "NodesOnlyMesh.hpp"
#include "WildTypeCellMutationState.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "CellLabel.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "CellMutationStatesCountWriter.hpp"
#include "OffLatticeSimulation.hpp"
#include "SmartPointers.hpp"
//This test is always run sequentially (never in parallel)
#include "FakePetscSetup.hpp"

/*
 * == Defining the cell property class ==
 *
 * Note that usually this code would be separated out into a separate declaration
 * in a .hpp file and definition in a .cpp file.
 */
class LuminalCellProperty : public AbstractCellProperty
{
private:

    /* We define a member variable {{{mColour}}}, which can be used by visualization tools
     * to paint cells with this mutation state a distinct colour if required. */
    unsigned mColour;

    /* The next block of code allows us to archive (save or load) the cell property object
     * in a cell-based simulation. The code consists of a serialize() method, in which we first
     * archive the cell property using the serialization code defined in the base class
     * {{{AbstractCellProperty}}}, then archive the member variable {{{mColour}}}. */
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellProperty>(*this);
        archive & mColour;
    }

public:

    /* The default constructor allows us to specify a value for the member variable {{{mColour}}},
     * or leave it with a default value. */
    LuminalCellProperty(unsigned colour=5)
        : AbstractCellProperty(),
          mColour(colour)
    {
    }

    /* We then define a destructor and a get method for the member variable {{{mColour}}}. */
    ~LuminalCellProperty()
    {}

    unsigned GetColour() const
    {
        return mColour;
    }
};
/* This completes the code for {{{LuminalCellProperty}}}.  Note that usually this code would
 * be separated out into a separate declaration in a .hpp file and definition in a .cpp file.
 */

/*
 * EMPTYLINE
 *
 * == Defining the motive force class ==
 *
 * In order to illustrate the use of cell properties we make a simple force law which
 * causes all cells with the {{{LuminalCellProperty}}} to move towards the origin. To do this we
 * create a new force class, {{{MyMotiveForce}}}, which inherits from
 * {{{AbstractForce}}} and overrides the methods {{{AddForceContribution()}}} and
 * {{{OutputForceParameters()}}}.
 *
 * Note that usually this code would be separated out into a separate declaration
 * in a .hpp file and definition in a .cpp file.
 */
class MyMotiveForce : public AbstractForce<2>
{
private:

    /* This force class includes a member variable, {{{mStrength}}}, which
     * defines the strength of the force. This member variable will be set
     * in the constructor.
     */
    double mStrength;

    /* We only need to include the next block of code if we wish to be able
     * to archive (save or load) the force model object in a cell-based simulation.
     * The code consists of a serialize method, in which we first archive the force
     * using the serialization code defined in the base class {{{AbstractForce}}},
     * then archive the member variable. */
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractForce<2> >(*this);
        archive & mStrength;
    }

public:
    /* The first public method is a default constructor, which calls the base
     * constructor. There is a single input argument, which defines the strength
     * of the force. We provide a default value of 2.0 for this argument. Inside
     * the method, we add an assertion to make sure that the strength is strictly
     * positive.
     */
    MyMotiveForce(double strength=2.0)
        : AbstractForce<2>(),
          mStrength(strength)
    {
        assert(mStrength > 0.0);
    }

    /* The second public method overrides {{{AddForceContribution()}}}.
     * This method takes in one argument, a reference to the cell population itself.
     */
    void AddForceContribution(AbstractCellPopulation<2>& rCellPopulation)
    {
        /* Inside the method, we loop over cells, and add a vector to
         * each node associated with cells with the {{{LuminalCellProperty}}}, which is proportional (with constant {{{mStrength}}}) to the negative of the position. Causing
         * cells to move inwards towards the origin. Note that this will currently only work with subclasses of {{{AbstractCentreBasedCellPopulation}}}s as
         * we associate cells with nodes in the force calculation. However, this could easily be modified to make it work for {{{VertexBasedCellPopulation}}}s.
         */
        for (AbstractCellPopulation<2>::Iterator cell_iter = rCellPopulation.Begin();
             cell_iter != rCellPopulation.End();
             ++cell_iter)
        {
            if (cell_iter->HasCellProperty<LuminalCellProperty>())
            {
                unsigned node_index = rCellPopulation.GetLocationIndexUsingCell(*cell_iter);

                c_vector<double, 2> location;
                location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
                c_vector<double, 2> force = -1.0 * mStrength * location;
                rCellPopulation.GetNode(node_index)->AddAppliedForceContribution(force);
            }
        }
    }

    /* Just as we encountered in [wiki:UserTutorials/CreatingAndUsingANewCellKiller], here we must override
     * a method that outputs any member variables to a specified results file {{{rParamsFile}}}.
     * In our case, we output the member variable {{{mStrength}}}, then call the method on the base class.
     */
    void OutputForceParameters(out_stream& rParamsFile)
    {
        *rParamsFile << "\t\t\t<Strength>" << mStrength << "</Strength>\n";
        AbstractForce<2>::OutputForceParameters(rParamsFile);
    }
};

/* As mentioned in previous cell-based Chaste tutorials, we need to include the next block
 * of code to be able to archive the cell property and force objects in a cell-based simulation,
 * and to obtain a unique identifier for our new classes for when writing results to file.
 *
 * Identifiers for both classes are defined together here, since we can only have each #include once
 * in this source file.  Normally the first include and export would go in the class' header, and the second
 * include and export in the .cpp file for each respective class.
 */
#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(LuminalCellProperty)
CHASTE_CLASS_EXPORT(MyMotiveForce)
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(LuminalCellProperty)
CHASTE_CLASS_EXPORT(MyMotiveForce)

/*
 * This completes the code for {{{MyMotiveForce}}}. Note that usually this code
 * would be separated out into a separate declaration in a .hpp file and definition
 * in a .cpp file.
 *
 * EMPTYLINE
 *
 * == The Tests ==
 *
 * We now define the test class, which inherits from {{{AbstractCellBasedTestSuite}}}.
 */
class LuminalCellProperty : public AbstractCellBasedTestSuite
{
public:

    /*
     * === Testing the cell property ===
     *
     * We begin by testing that our new cell property is implemented correctly.
     */
    void TestLuminalCellProperty()
    {
        /* We begin by testing that some of the base class methods work correctly.
         * We typically use shared pointers to create and access a cell property
         * like {{{LuminalCellProperty}}}, for which it makes sense for all cells
         * that have the same mutation to share a pointer to the same cell property
         * object (although strictly speaking, they are not required to). Observe that
         * in this case we have provided a value for the member variable {{{mColour}}}
         * in the {{{LuminalCellProperty}}} constructor.*/
        MAKE_PTR_ARGS(LuminalCellProperty, p_property, (8));

        /* Each cell property has a member variable, {{{mCellCount}}}, which
         * stores the number of cells with this cell property. We can test whether
         * {{{mCellCount}}} is being updated correctly by our cell property, as follows. */
        TS_ASSERT_EQUALS(p_property->GetCellCount(), 0u);
        p_property->IncrementCellCount();
        TS_ASSERT_EQUALS(p_property->GetCellCount(), 1u);
        p_property->DecrementCellCount();
        TS_ASSERT_EQUALS(p_property->GetCellCount(), 0u);
        TS_ASSERT_THROWS_THIS(p_property->DecrementCellCount(),
                "Cannot decrement cell count: no cells have this cell property");

        /* We can also test whether our cell property is of a given type, as follows. */
        TS_ASSERT_EQUALS(p_property->IsType<WildTypeCellMutationState>(), false);
        TS_ASSERT_EQUALS(p_property->IsType<LuminalCellProperty>(), true);

        /* We can also test that archiving is implemented correctly for our cell
         * property, as follows (further details on how to implement and
         * test archiving can be found at ChasteGuides/BoostSerialization).  */
        OutputFileHandler handler("archive", false);
        std::string archive_filename = handler.GetOutputDirectoryFullPath() + "property.arch";

        {
            AbstractCellProperty* const p_const_property = new LuminalCellProperty(7);
            p_const_property->IncrementCellCount();

            TS_ASSERT_EQUALS(p_const_property->GetCellCount(), 1u);
            TS_ASSERT_EQUALS(dynamic_cast<LuminalCellProperty*>(p_const_property)->GetColour(), 7u);

            std::ofstream ofs(archive_filename.c_str());
            boost::archive::text_oarchive output_arch(ofs);

            output_arch << p_const_property;

            delete p_const_property;
        }

        {
            AbstractCellProperty* p_arch_property;

            std::ifstream ifs(archive_filename.c_str());
            boost::archive::text_iarchive input_arch(ifs);

            input_arch >> p_arch_property;

            TS_ASSERT_EQUALS(p_arch_property->GetCellCount(), 1u);

            LuminalCellProperty* p_real_property = dynamic_cast<LuminalCellProperty*>(p_arch_property);
            TS_ASSERT(p_real_property != NULL);
            TS_ASSERT_EQUALS(p_real_property->GetColour(), 7u);

            delete p_arch_property;
        }
    }

#endif /*LUMINALCELLPROPERTY_HPP_*/
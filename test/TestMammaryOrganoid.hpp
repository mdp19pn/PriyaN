#ifndef TESTMAMMARYORGANOID_HPP_
#define TESTMAMMARYORGANOID_HPP_

// Include necessary header files
#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "CellBasedSimulationArchiver.hpp"

#include "SmartPointers.hpp"
#include "PetscSetupAndFinalize.hpp"
#include "CellsGenerator.hpp"
#include "OffLatticeSimulation.hpp"
#include "NodesOnlyMesh.hpp"
#include "NodeBasedCellPopulationWithVariableDamping.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "MammaryStemCellProperty.hpp"
#include "CellHeightTrackingModifier.hpp"
#include "CellVelocityWriter.hpp"
#include "CellLocationWriter.hpp"
#include "BoundaryLengthWriter.hpp"
#include "MammaryCellTypeWriter.hpp"
#include "MammaryCellCycleModel.hpp"
#include "UniformCellCycleModel.hpp"
#include "SubstrateDependentCellCycleModel.hpp"
#include "WildTypeCellMutationState.hpp"
#include "StemCellProliferativeType.hpp"
#include "OrientedDivisionRule.hpp"
#include "AnoikisCellKiller.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "RepulsionForce.hpp"
#include "CellCellAdhesionForce.hpp"
#include "CellCoverslipAdhesionForce.hpp"
#include "PlaneBoundaryCondition.hpp"
#include "VertexMeshWriter.hpp"
#include "MutableVertexMesh.hpp"
#include "Debug.hpp"

/*
 * The results of each test below can be visualised using Paraview. 
 */
class TestMammaryOrganoid : public AbstractCellBasedTestSuite
{
public:

    void TestMammaryOrganoidMammaryCellCycleModel()
    {
        EXIT_IF_PARALLEL;

        // Create a 3D 'nodes only' mesh, specifying nodes manually
        std::vector<Node<3>*> nodes;
        nodes.push_back(new Node<3>(0,  false,  0.0, 0.0, 0.0));
        nodes.push_back(new Node<3>(1,  false,  0.75, 0.0, 0.0));
        nodes.push_back(new Node<3>(2,  false,  -0.5, -0.5, 0.0));
        nodes.push_back(new Node<3>(3,  false,  1.25, -0.5, 0.0));
        nodes.push_back(new Node<3>(4,  false,  0.0, 0.75, 0.0));
        nodes.push_back(new Node<3>(5,  false,  0.75, 0.75, 0.0));
        nodes.push_back(new Node<3>(6,  false,  0.0, 0.0, 1.0));
        nodes.push_back(new Node<3>(7,  false,  -0.5, -0.5, 1.0));
        nodes.push_back(new Node<3>(8,  false,  -0.5, 0.5, 1.0));
        
        NodesOnlyMesh<3> mesh;
        mesh.ConstructNodesWithoutMesh(nodes, 1.5);
       
        // Create a vector of proliferative cells using the helper CellsGenerator
        std::vector<CellPtr> cells;
        CellsGenerator<MammaryCellCycleModel, 3> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes());
      
        // Use the mesh and cells to create a cell population
        NodeBasedCellPopulation<3> cell_population(mesh, cells);
        
        // Create the luminal/myoepithelial cell properties (we do it this way
        // to make sure they're tracked correctly in the simulation)
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_stem(cell_population.GetCellPropertyRegistry()->Get<MammaryStemCellProperty>());
        
        // Assign these properties to cells (change these lines if you want e.g. only luminal cells)
        cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_stem);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_stem);
        cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(5)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(6)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(7)->AddCellProperty(p_stem);
        cell_population.GetCellUsingLocationIndex(8)->AddCellProperty(p_myo);

        // Set the division rule for our population to be the oriented division rule
        boost::shared_ptr<AbstractCentreBasedDivisionRule<3,3> > p_division_rule_to_set(new OrientedDivisionRule<3,3>());
        cell_population.SetCentreBasedDivisionRule(p_division_rule_to_set);

        // Add a cell writer so that mammary cell types are written to file
        cell_population.AddCellWriter<MammaryCellTypeWriter>();

        // Add a cell writer so that the cell location is written to file
        cell_population.AddCellWriter<CellLocationWriter>();

        // Add a cell writer so that cell velocities are written to file
        cell_population.AddCellWriter<CellVelocityWriter>();

        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryOrganoidWT");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(96.0); // Hours

        MAKE_PTR(GeneralisedLinearSpringForce<3>, p_linear_force);
        p_linear_force->SetCutOffLength(3);
        simulator.AddForce(p_linear_force);
       
        // Run the simulation
        simulator.Solve();
      
        // Since we created pointers to nodes, we delete them here to avoid memory leaks
        for (unsigned i=0; i<nodes.size(); i++)
        {
            delete nodes[i];
        }
    }
};
#endif /* TESTMAMMARYORGANOID_HPP_ */
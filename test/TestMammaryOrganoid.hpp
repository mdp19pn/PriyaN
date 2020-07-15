#ifndef TESTMAMMARYORGANOID_HPP_
#define TESTMAMMARYORGANOID_HPP_

// Include necessary header files
#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "PetscSetupAndFinalize.hpp"
#include "CellsGenerator.hpp"
#include "UniformCellCycleModel.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "SmartPointers.hpp"
#include "NodesOnlyMesh.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "CellLabel.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "CellVelocityWriter.hpp"

/*
 * The next block of code is included to be able to archive the cell property and force objects in a cell-based simulation,
 * and to obtain a unique identifier for our new classes for when writing results to file. 
 * Identifiers for all classes are defined together here, since we can only have each #include once in this source file. 
 * 
 * The first include and export would go in the class' header, 
 * and the second include and export in the .cpp file for each respective class.
 */
#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(LuminalCellProperty)
CHASTE_CLASS_EXPORT(MyoepithelialCellProperty)
CHASTE_CLASS_EXPORT(CellVelocityWriter)
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(LuminalCellProperty)
CHASTE_CLASS_EXPORT(MyoepithelialCellProperty)
CHASTE_CLASS_EXPORT(CellVelocityWriter)

/*
 * To visualize the results of each test below, use Paraview or Chaste's 
 * Java visualizer.
 */
class TestMammaryOrganoid : public AbstractCellBasedTestSuite
{
public:

    void TestMammaryMonolayer()
    {
        EXIT_IF_PARALLEL;

        // Create a 2D 'nodes only' mesh using the helper class HoneycombMeshGenerator
        HoneycombMeshGenerator generator(2, 2);
        MutableMesh<2,2>* p_generating_mesh = generator.GetMesh();
        NodesOnlyMesh<2> mesh;
        mesh.ConstructNodesWithoutMesh(*p_generating_mesh, 1.5);

        // Create a vector of proliferative cells using the helper CellsGenerator
        std::vector<CellPtr> cells;
        MAKE_PTR(LuminalCellProperty, p_luminal);
        MAKE_PTR(CellLabel, p_label);
        CellsGenerator<UniformCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes(), p_luminal);

        // Use the mesh and cells to create a cell population, and specify which results to output to file.
        NodeBasedCellPopulation<2> cell_population(mesh, cells);
        cell_population.AddCellWriter<CellVelocityWriter>();

        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("NodeBasedMonolayer");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(10.0);

        // Pass a force law for cell mechanics to the simulation
        MAKE_PTR(GeneralisedLinearSpringForce<2>, p_force);
        simulator.AddForce(p_force);

        // Run the simulation
        simulator.Solve();
    }

    void TestOrganoid()
    {
        EXIT_IF_PARALLEL;

        // Create a 3D 'nodes only' mesh, specifying nodes manually
        std::vector<Node<3>*> nodes;
        nodes.push_back(new Node<3>(0u,  false,  0.5, 0.0, 0.0));
        nodes.push_back(new Node<3>(1u,  false,  -0.5, 0.0, 0.0));
        nodes.push_back(new Node<3>(2u,  false,  0.0, 0.5, 0.0));
        nodes.push_back(new Node<3>(3u,  false,  0.0, -0.5, 0.0));
        NodesOnlyMesh<3> mesh;
        mesh.ConstructNodesWithoutMesh(nodes, 1.5);

        // Create a vector of proliferative cells using the helper CellsGenerator
        std::vector<CellPtr> cells;
        MAKE_PTR(LuminalCellProperty, p_luminal);
        MAKE_PTR(CellLabel, p_label);
        CellsGenerator<UniformCellCycleModel, 3> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes(), p_luminal);

        // Use the mesh and cells to create a cell population, and specify which results to output to file.
        NodeBasedCellPopulation<3> cell_population(mesh, cells);
        cell_population.AddCellWriter<CellVelocityWriter>();

        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("NodeBasedSpheroid");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(10.0);

        // Pass a force law for cell mechanics to the simulation
        MAKE_PTR(GeneralisedLinearSpringForce<3>, p_force);
        simulator.AddForce(p_force);

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

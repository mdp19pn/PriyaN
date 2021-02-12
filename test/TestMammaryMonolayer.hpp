#ifndef TESTMAMMARYMONOLAYER_HPP_
#define TESTMAMMARYMONOLAYER_HPP_

// Include necessary header files
#include <cxxtest/TestSuite.h>

#include "CellBasedSimulationArchiver.hpp"

#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "PetscSetupAndFinalize.hpp"
#include "CellsGenerator.hpp"
#include "MammaryCellCycleModel.hpp"
#include "CellCoverslipBasedCellKiller.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "SmartPointers.hpp"
#include "NodesOnlyMesh.hpp"
#include "NodeBasedCellPopulationWithVariableDamping.hpp"
#include "LuminalCellProperty.hpp"
#include "MyoepithelialCellProperty.hpp"
#include "CellVelocityWriter.hpp"
#include "CellLocationWriter.hpp"
#include "HeterotypicBoundaryLengthWriter.hpp"
#include "MammaryCellTypeWriter.hpp"
#include "RepulsionForce.hpp"
#include "CellCoverslipAdhesionForce.hpp"
#include "PlaneBoundaryCondition.hpp"
#include "VertexMeshWriter.hpp"
#include "MutableVertexMesh.hpp"
#include "Debug.hpp"

/*
 * The results of each test below can be visualised using Paraview. 
 */
class TestMammaryMonolayer : public AbstractCellBasedTestSuite
{
public:

    // void TestMammaryMonolayer2D()
    // {
    //     EXIT_IF_PARALLEL;
        
    //     // Create a 2D 'nodes only' mesh, specifying nodes manually
    //     std::vector<Node<2>*> nodes;
    //     nodes.push_back(new Node<2>(0, false, 0.5, 0.0));
    //     nodes.push_back(new Node<2>(1, false, 0.0, 0.5));
    //     nodes.push_back(new Node<2>(2, false, 1.0, 0.0));
    //     nodes.push_back(new Node<2>(3, false, 0.5, 1.0));
    //     nodes.push_back(new Node<2>(4, false, 1.0, 0.5));
    //     NodesOnlyMesh<2> mesh;
    //     mesh.ConstructNodesWithoutMesh(nodes, 1.5);
        
    //     // Create a vector of proliferative cells using the helper CellsGenerator
    //     std::vector<CellPtr> cells;
    //     CellsGenerator<MammaryCellCycleModel, 2> cells_generator;
    //     cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes());
        
    //     // Use the mesh and cells to create a cell population
    //     NodeBasedCellPopulationWithVariableDamping<2> cell_population(mesh, cells);

    //     cell_population.SetLuminalCellDampingConstant(2.0); 
    //     cell_population.SetMyoepithelialCellDampingConstant(5.0);
        
    //     // Create the luminal/myoepithelial cell properties (we do it this way to make sure they're tracked correctly in the simulation)
    //     boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
    //     boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        
    //     // Assign these properties to cells (change these lines if you want e.g. only luminal cells)
    //     cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
    //     cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_luminal);
    //     cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_luminal);
    //     cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_myo);
    //     cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_myo);
        
    //     // Add a cell writer so that cell velocities are written to file
    //     cell_population.AddCellWriter<CellVelocityWriter>();
        
    //     // Add a cell writer so that mammary cell types are written to file
    //     cell_population.AddCellWriter<MammaryCellTypeWriter>();

    //     // Add a population writer so that cell sorting (bilayer formation) is written to file
    //     cell_population.AddPopulationWriter<HeterotypicBoundaryLengthWriter>();

    //     // Add a vertex mesh writer so that a rectangular coverslip  is written to file
    //     std::vector<Node<2>*> coverslip;
    //     coverslip.push_back(new Node<2>(0, true, -25.0, 25.0));
    //     coverslip.push_back(new Node<2>(1, true, 25.0, 25.0));
    //     coverslip.push_back(new Node<2>(2, true, 25.0, -25.0));
    //     coverslip.push_back(new Node<2>(3, true, -25.0, -25.0));

    //     std::vector<VertexElement<2,2>* > elements = {new VertexElement<2,2>(0, coverslip)};
    //     MutableVertexMesh<2,2>* p_mesh = new MutableVertexMesh<2,2>(coverslip, elements);
        
    //     VertexMeshWriter<2,2> vertex_mesh_writer("Coverslip 2D", "rectangle", false);
    //     vertex_mesh_writer.WriteVtkUsingMesh(*p_mesh, "0");
        
    //     // Pass the cell population to the simulation and specify duration and output parameters
    //     OffLatticeSimulation<2> simulator(cell_population);
    //     simulator.SetOutputDirectory("TestMammaryMonolayer2D");
    //     simulator.SetSamplingTimestepMultiple(12);
    //     simulator.SetEndTime(96.0); // Hours
        
    //     // Create a cell-cell replusion force law and pass it to the simulation
    //     MAKE_PTR(RepulsionForce<2>, p_force);
    //     simulator.AddForce(p_force);
        
    //     // Run the simulation
    //     simulator.Solve();
    // }

    void TestMammaryMonolayer3D()
    {
        // EXIT_IF_PARALLEL;
        
        // Create a 3D 'nodes only' mesh, specifying nodes manually
        std::vector<Node<3>*> nodes;
        nodes.push_back(new Node<3>(0,  false,  0.5, 0.0, 0.0));
        nodes.push_back(new Node<3>(1,  false,  0.0, 0.5, 0.0));
        nodes.push_back(new Node<3>(2,  false,  1.0, 0.0, 0.0));
        nodes.push_back(new Node<3>(3,  false,  1.0, 0.0, 1.0));
        nodes.push_back(new Node<3>(4,  false,  0.5, 1.0, 0.0));
        nodes.push_back(new Node<3>(5,  false,  1.0, 0.5, 0.0));
        nodes.push_back(new Node<3>(6,  false,  1.0, 0.5, 1.0));

        NodesOnlyMesh<3> mesh;
        mesh.ConstructNodesWithoutMesh(nodes, 1.5);
       
        // Create a vector of proliferative cells using the helper CellsGenerator
        std::vector<CellPtr> cells;
        CellsGenerator<MammaryCellCycleModel, 3> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes());
      
        // Use the mesh and cells to create a cell population
        NodeBasedCellPopulationWithVariableDamping<3> cell_population(mesh, cells);

        cell_population.SetLuminalCellDampingConstant(3.0); 
        cell_population.SetMyoepithelialCellDampingConstant(5.0);
        
        // Create the luminal/myoepithelial cell properties (we do it this way to make sure they're tracked correctly in the simulation)
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        
        // Assign these properties to cells (change these lines if you want e.g. only luminal cells)
        cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(5)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(6)->AddCellProperty(p_luminal);
        
        // Add a cell writer so that mammary cell types are written to file
        cell_population.AddCellWriter<MammaryCellTypeWriter>();

        // Add a cell writer so that the cell location is written to file
        cell_population.AddCellWriter<CellLocationWriter>();

        // Add a cell writer so that cell velocities are written to file
        cell_population.AddCellWriter<CellVelocityWriter>();

        // Add a population writer so that cell sorting (bilayer formation) is written to file
        cell_population.AddPopulationWriter<HeterotypicBoundaryLengthWriter>();
      
        // Add a vertex mesh writer so that a rectangular coverslip  is written to file
        std::vector<Node<3>*> coverslip;
        coverslip.push_back(new Node<3>(0, true, -25.0, 25.0, -0.5));
        coverslip.push_back(new Node<3>(1, true, 25.0, 25.0, -0.5));
        coverslip.push_back(new Node<3>(2, true, 25.0, -25.0, -0.5));
        coverslip.push_back(new Node<3>(3, true, -25.0, -25.0, -0.5));
        coverslip.push_back(new Node<3>(4, true, -25.0, 25.0, -1.0));
        coverslip.push_back(new Node<3>(5, true, 25.0, 25.0, -1.0));
        coverslip.push_back(new Node<3>(6, true, 25.0, -25.0, -1.0));
        coverslip.push_back(new Node<3>(7, true, -25.0, -25.0, -1.0));
        std::vector<VertexElement<3,3>* > elements = {new VertexElement<3,3>(0, coverslip)};
        MutableVertexMesh<3,3>* p_mesh = new MutableVertexMesh<3,3>(coverslip, elements);
        
        VertexMeshWriter<3,3> vertex_mesh_writer("Coverslip 3D", "rectangle", false);
        vertex_mesh_writer.WriteVtkUsingMesh(*p_mesh, "0");
        
        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryMonolayer3DWT");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(96.0); // Hours
       
        // Create a cell-cell repulsion force law and pass it to the simulation
        MAKE_PTR(RepulsionForce<3>, p_force); 
        simulator.AddForce(p_force);

        // Create a cell-coverslip adhesion force law and pass it to the simulation
        MAKE_PTR(CellCoverslipAdhesionForce<3>, p_cell_coverslip_force);
        simulator.AddForce(p_cell_coverslip_force);

        // Define a point on the plane boundary and a normal to the plane.
        c_vector<double,3> point = zero_vector<double>(3);
        c_vector<double,3> normal = zero_vector<double>(3);
        normal(2) = -0.5;
        
        // Make a pointer to a PlaneBoundaryCondition (passing the point and normal to the plane) and pass it to the OffLatticeSimulation.
        MAKE_PTR_ARGS(PlaneBoundaryCondition<3>, p_bc, (&cell_population, point, normal));
        simulator.AddCellPopulationBoundaryCondition(p_bc);
        
        // Construct a cell killer object and pass the cell killer into the cell-based simulation
        MAKE_PTR_ARGS(CellCoverslipBasedCellKiller, p_killer, (&cell_population));
        simulator.AddCellKiller(p_killer);
       
        // Run the simulation
        simulator.Solve();
      
        // Since we created pointers to nodes, we delete them here to avoid memory leaks
        for (unsigned i=0; i<nodes.size(); i++)
        {
            delete nodes[i];
        }
    }
};

#endif /* TESTMAMMARYMONOLAYER_HPP_ */

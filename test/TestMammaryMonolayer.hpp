#ifndef TESTMAMMARYMONOLAYER_HPP_
#define TESTMAMMARYMONOLAYER_HPP_

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
#include "LuminalStemCellProperty.hpp"
#include "MyoepithelialStemCellProperty.hpp"
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
class TestMammaryMonolayer : public AbstractCellBasedTestSuite
{
public:

    void xTestMammaryMonolayer2D()
    {
        EXIT_IF_PARALLEL;
        
        // Create a 2D 'nodes only' mesh, specifying nodes manually
        std::vector<Node<2>*> nodes;
        nodes.push_back(new Node<2>(0, false, 0.5, 0.0));
        nodes.push_back(new Node<2>(1, false, 0.0, 0.5));
        nodes.push_back(new Node<2>(2, false, 1.0, 0.0));
        nodes.push_back(new Node<2>(3, false, 0.5, 1.0));
        nodes.push_back(new Node<2>(4, false, 1.0, 0.5));
        NodesOnlyMesh<2> mesh;
        mesh.ConstructNodesWithoutMesh(nodes, 1.5);
        
        // Create a vector of proliferative cells using the helper CellsGenerator
        std::vector<CellPtr> cells;
        CellsGenerator<MammaryCellCycleModel, 2> cells_generator;
        cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes());
        
        // Use the mesh and cells to create a cell population
        NodeBasedCellPopulationWithVariableDamping<2> cell_population(mesh, cells);

        cell_population.SetLuminalCellDampingConstant(1.0); 
        cell_population.SetMyoepithelialCellDampingConstant(5.0);
        
        // Create the luminal/myoepithelial cell properties (we do it this way to make sure they're tracked correctly in the simulation)
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        
        // Assign these properties to cells (change these lines if you want e.g. only luminal cells)
        cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_myo);
        
        // Add a cell writer so that cell velocities are written to file
        cell_population.AddCellWriter<CellVelocityWriter>();
        
        // Add a cell writer so that mammary cell types are written to file
        cell_population.AddCellWriter<MammaryCellTypeWriter>();

        // Add a population writer so that cell sorting (bilayer formation) is written to file
        cell_population.AddPopulationWriter<BoundaryLengthWriter>();

        // Add a vertex mesh writer so that a rectangular coverslip  is written to file
        std::vector<Node<2>*> coverslip;
        coverslip.push_back(new Node<2>(0, true, -25.0, 25.0));
        coverslip.push_back(new Node<2>(1, true, 25.0, 25.0));
        coverslip.push_back(new Node<2>(2, true, 25.0, -25.0));
        coverslip.push_back(new Node<2>(3, true, -25.0, -25.0));

        std::vector<VertexElement<2,2>* > elements = {new VertexElement<2,2>(0, coverslip)};
        MutableVertexMesh<2,2>* p_mesh = new MutableVertexMesh<2,2>(coverslip, elements);
        
        VertexMeshWriter<2,2> vertex_mesh_writer("Coverslip 2D", "rectangle", false);
        vertex_mesh_writer.WriteVtkUsingMesh(*p_mesh, "0");
        
        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryMonolayer2D");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(96.0); // Hours
        
        // Create a cell-cell replusion force law and pass it to the simulation
        MAKE_PTR(RepulsionForce<2>, p_force);
        simulator.AddForce(p_force);
        
        // Run the simulation
        simulator.Solve();
    }

    void TestMammaryMonolayerSubstrateDependentCellCycleModel()
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
        nodes.push_back(new Node<3>(9,  false,  1.25, 0.0, 0.0));
        nodes.push_back(new Node<3>(10,  false,  0.0, 1.0, 0.0));
        nodes.push_back(new Node<3>(11,  false,  0.0, 0.75, 1.0));
        nodes.push_back(new Node<3>(12,  false,  -1.5, -0.5, 0.0)); //
        nodes.push_back(new Node<3>(13,  false,  1.0, 0.75, 0.0));
        nodes.push_back(new Node<3>(14,  false,  1.75, 0.75, 0.0));
        nodes.push_back(new Node<3>(15,  false,  1.0, 0.0, 1.0));
        nodes.push_back(new Node<3>(16,  false,  -1.5, -0.5, 1.0));
        nodes.push_back(new Node<3>(17,  false,  -1.5, 0.5, 1.0));
        nodes.push_back(new Node<3>(18,  false,  2.25, 0.0, 0.0));
        nodes.push_back(new Node<3>(19,  false,  1.0, 1.0, 0.0));
        nodes.push_back(new Node<3>(20,  false,  1.0, 0.75, 1.0));
        nodes.push_back(new Node<3>(21,  false,  0.0, 1.75, 1.0));
        nodes.push_back(new Node<3>(22,  false,  -1.5, -1.5, 0.0)); //
        nodes.push_back(new Node<3>(23,  false,  1.0, 1.75, 0.0));
        nodes.push_back(new Node<3>(24,  false,  1.75, 1.75, 0.0));
        nodes.push_back(new Node<3>(25,  false,  1.0, 1.0, 1.0));
        nodes.push_back(new Node<3>(26,  false,  -1.5, -1.5, 1.0));
        nodes.push_back(new Node<3>(27,  false,  -1.5, 1.5, 1.0));

        NodesOnlyMesh<3> mesh;
        mesh.ConstructNodesWithoutMesh(nodes, 1.5);
       
        /* Create a vector of cell pointers. */
        std::vector<CellPtr> cells;

        /*
         * This line defines a mutation state to be used for all cells, of type
         * `WildTypeCellMutationState` (i.e. 'healthy'):
         */
        MAKE_PTR(WildTypeCellMutationState, p_state);
        MAKE_PTR(StemCellProliferativeType, p_stem_type);

        /* Create a cell-cycle (only contact inhibited) model for these cells and loop over the
        * nodes of the mesh to create as many elements in the vector of cell pointers as there are
        * in the initial mesh. */
        for (unsigned i=0; i<mesh.GetNumNodes(); i++)
        {
            SubstrateDependentCellCycleModel* p_cycle_model = new SubstrateDependentCellCycleModel();
            p_cycle_model->SetDimension(3);
            p_cycle_model->SetBirthTime(-2.0*(double)i);
            p_cycle_model->SetQuiescentHeightFraction(0.5);
            p_cycle_model->SetEquilibriumHeight(1.0);
            
            CellPtr p_cell(new Cell(p_state,p_cycle_model));
            p_cell->SetCellProliferativeType(p_stem_type);
            
            //Alter the defult cell-cyle duration
            p_cycle_model->SetStemCellG1Duration(8.0);
            p_cycle_model->SetTransitCellG1Duration(8.0);

            /*
            * push the cell back into the vector of cells.
            */
            p_cell->InitialiseCellCycleModel();
            cells.push_back(p_cell);
        }
      
        // Use the mesh and cells to create a cell population
        NodeBasedCellPopulationWithVariableDamping<3> cell_population(mesh, cells);

        cell_population.SetLuminalCellDampingConstant(1.0); 
        cell_population.SetMyoepithelialCellDampingConstant(1.0);

        // Create the different cell types: luminal stem cells, myoepithelial stem differentiated luminal cells and differentiated myoepithelial cell, (we do it this way to make sure they're tracked correctly in the simulation)
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_luminal_stem(cell_population.GetCellPropertyRegistry()->Get<LuminalStemCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo_stem(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialStemCellProperty>());
        
        // Assign these properties to cells
        cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(5)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(6)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(7)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(8)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(9)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(10)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(11)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(12)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(13)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(14)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(15)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(16)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(17)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(18)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(19)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(20)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(21)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(22)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(23)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(24)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(25)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(26)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(27)->AddCellProperty(p_myo);

        // Set the division rule for our population to be the oriented division rule
        boost::shared_ptr<AbstractCentreBasedDivisionRule<3,3> > p_division_rule_to_set(new OrientedDivisionRule<3,3>());
        cell_population.SetCentreBasedDivisionRule(p_division_rule_to_set);

        // Add a cell writer so that mammary cell types are written to file
        cell_population.AddCellWriter<MammaryCellTypeWriter>();

        // Add a cell writer so that the cell location is written to file
        cell_population.AddCellWriter<CellLocationWriter>();

        // Add a cell writer so that cell velocities are written to file
        cell_population.AddCellWriter<CellVelocityWriter>();

        // // Add a population writer so that cell sorting (bilayer formation) is written to file
        // cell_population.AddPopulationWriter<BoundaryLengthWriter>();

        // // Construct a cell killer object
        // MAKE_PTR_ARGS(AnoikisCellKiller<3>, p_killer, (&cell_population, 0.5));

        // Add a vertex mesh writer so that a rectangular coverslip is written to file
        std::vector<Node<3>*> coverslip;
        coverslip.push_back(new Node<3>(0, true, -25.0, 25.0, -0.4));
        coverslip.push_back(new Node<3>(1, true, 25.0, 25.0, -0.4));
        coverslip.push_back(new Node<3>(2, true, 25.0, -25.0, -0.4));
        coverslip.push_back(new Node<3>(3, true, -25.0, -25.0, -0.4));
        coverslip.push_back(new Node<3>(4, true, -25.0, 25.0, -0.5));
        coverslip.push_back(new Node<3>(5, true, 25.0, 25.0, -0.5));
        coverslip.push_back(new Node<3>(6, true, 25.0, -25.0, -0.5));
        coverslip.push_back(new Node<3>(7, true, -25.0, -25.0, -0.5));
        std::vector<VertexElement<3,3>* > elements = {new VertexElement<3,3>(0, coverslip)};
        MutableVertexMesh<3,3>* p_mesh = new MutableVertexMesh<3,3>(coverslip, elements);
        
        VertexMeshWriter<3,3> vertex_mesh_writer("Coverslip 3D", "rectangle", false);
        vertex_mesh_writer.WriteVtkUsingMesh(*p_mesh, "0");
        
        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryMonolayerWT");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(96.0); // Hours
        
        // Create a cell-cell repulsion force law and pass it to the simulation
        MAKE_PTR(RepulsionForce<3>, p_force); 
        simulator.AddForce(p_force);

        // Create a cell-coverslip adhesion force law and pass it to the simulation
        MAKE_PTR(CellCoverslipAdhesionForce<3>, p_cell_coverslip_force);
        simulator.AddForce(p_cell_coverslip_force);

        // Define a point on the plane boundary and a normal to the plane z=0.
        c_vector<double,3> point = zero_vector<double>(3);
        c_vector<double,3> normal = zero_vector<double>(3);
        normal(2) = -1.0;

        // Make a pointer to a PlaneBoundaryCondition (passing the point and normal to the plane) and pass it to the OffLatticeSimulation.
        MAKE_PTR_ARGS(PlaneBoundaryCondition<3>, p_bc, (&cell_population, point, normal));
        simulator.AddCellPopulationBoundaryCondition(p_bc);

        // // Pass the cell killer into the cell-based simulation
        // simulator.AddCellKiller(p_killer);

        // Add and pass the modifier to the simulation
        MAKE_PTR(CellHeightTrackingModifier<3>, p_modifier);
        simulator.AddSimulationModifier(p_modifier);
       
        // Run the simulation
        simulator.Solve();
      
        // Since we created pointers to nodes, we delete them here to avoid memory leaks
        for (unsigned i=0; i<nodes.size(); i++)
        {
            delete nodes[i];
        }
    }

    void xTestMammaryMonolayerMammaryCellCycleModel()
    {
        // EXIT_IF_PARALLEL;
        
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
        nodes.push_back(new Node<3>(9,  false,  1.25, 0.0, 0.0));
        nodes.push_back(new Node<3>(10,  false,  0.0, 1.0, 0.0));
        nodes.push_back(new Node<3>(11,  false,  0.0, 0.75, 1.0));
        nodes.push_back(new Node<3>(12,  false,  -1.5, -0.5, 0.0)); //
        nodes.push_back(new Node<3>(13,  false,  1.0, 0.75, 0.0));
        nodes.push_back(new Node<3>(14,  false,  1.75, 0.75, 0.0));
        nodes.push_back(new Node<3>(15,  false,  1.0, 0.0, 1.0));
        nodes.push_back(new Node<3>(16,  false,  -1.5, -0.5, 1.0));
        nodes.push_back(new Node<3>(17,  false,  -1.5, 0.5, 1.0));
        nodes.push_back(new Node<3>(18,  false,  2.25, 0.0, 0.0));
        nodes.push_back(new Node<3>(19,  false,  1.0, 1.0, 0.0));
        nodes.push_back(new Node<3>(20,  false,  1.0, 0.75, 1.0));
        nodes.push_back(new Node<3>(21,  false,  0.0, 1.75, 1.0));
        nodes.push_back(new Node<3>(22,  false,  -1.5, -1.5, 0.0)); //
        nodes.push_back(new Node<3>(23,  false,  1.0, 1.75, 0.0));
        nodes.push_back(new Node<3>(24,  false,  1.75, 1.75, 0.0));
        nodes.push_back(new Node<3>(25,  false,  1.0, 1.0, 1.0));
        nodes.push_back(new Node<3>(26,  false,  -1.5, -1.5, 1.0));
        nodes.push_back(new Node<3>(27,  false,  -1.5, 1.5, 1.0));

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
        
        // Create the different cell types: luminal stem cells, myoepithelial stem differentiated luminal cells and differentiated myoepithelial cell, (we do it this way to make sure they're tracked correctly in the simulation)
        boost::shared_ptr<AbstractCellProperty> p_luminal(cell_population.GetCellPropertyRegistry()->Get<LuminalCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_luminal_stem(cell_population.GetCellPropertyRegistry()->Get<LuminalStemCellProperty>());
        boost::shared_ptr<AbstractCellProperty> p_myo_stem(cell_population.GetCellPropertyRegistry()->Get<MyoepithelialStemCellProperty>());
        
        // Assign these properties to cells
        cell_population.GetCellUsingLocationIndex(0)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(1)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(2)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(3)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(4)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(5)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(6)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(7)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(8)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(9)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(10)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(11)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(12)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(13)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(14)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(15)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(16)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(17)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(18)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(19)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(20)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(21)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(22)->AddCellProperty(p_myo_stem);
        cell_population.GetCellUsingLocationIndex(23)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(24)->AddCellProperty(p_myo);
        cell_population.GetCellUsingLocationIndex(25)->AddCellProperty(p_luminal);
        cell_population.GetCellUsingLocationIndex(26)->AddCellProperty(p_luminal_stem);
        cell_population.GetCellUsingLocationIndex(27)->AddCellProperty(p_myo);

        // Set the division rule for our population to be the oriented division rule
        boost::shared_ptr<AbstractCentreBasedDivisionRule<3,3> > p_division_rule_to_set(new OrientedDivisionRule<3,3>());
        cell_population.SetCentreBasedDivisionRule(p_division_rule_to_set);
        
        // Add a cell writer so that mammary cell types are written to file
        cell_population.AddCellWriter<MammaryCellTypeWriter>();

        // Add a cell writer so that the cell location is written to file
        cell_population.AddCellWriter<CellLocationWriter>();

        // Add a cell writer so that cell velocities are written to file
        cell_population.AddCellWriter<CellVelocityWriter>();

        // // Add a population writer so that cell sorting (bilayer formation) is written to file
        // cell_population.AddPopulationWriter<BoundaryLengthWriter>();

        // Construct a cell killer object
        MAKE_PTR_ARGS(AnoikisCellKiller<3>, p_killer, (&cell_population, 0.5));
      
        // Add a vertex mesh writer so that a rectangular coverslip  is written to file
        std::vector<Node<3>*> coverslip;
        coverslip.push_back(new Node<3>(0, true, -25.0, 25.0, -0.4));
        coverslip.push_back(new Node<3>(1, true, 25.0, 25.0, -0.4));
        coverslip.push_back(new Node<3>(2, true, 25.0, -25.0, -0.4));
        coverslip.push_back(new Node<3>(3, true, -25.0, -25.0, -0.4));
        coverslip.push_back(new Node<3>(4, true, -25.0, 25.0, -0.5));
        coverslip.push_back(new Node<3>(5, true, 25.0, 25.0, -0.5));
        coverslip.push_back(new Node<3>(6, true, 25.0, -25.0, -0.5));
        coverslip.push_back(new Node<3>(7, true, -25.0, -25.0, -0.5));
        std::vector<VertexElement<3,3>* > elements = {new VertexElement<3,3>(0, coverslip)};
        MutableVertexMesh<3,3>* p_mesh = new MutableVertexMesh<3,3>(coverslip, elements);
        
        VertexMeshWriter<3,3> vertex_mesh_writer("Coverslip 3D", "rectangle", false);
        vertex_mesh_writer.WriteVtkUsingMesh(*p_mesh, "0");
        
        // Pass the cell population to the simulation and specify duration and output parameters
        OffLatticeSimulation<3> simulator(cell_population);
        simulator.SetOutputDirectory("TestMammaryMonolayerWT");
        simulator.SetSamplingTimestepMultiple(12);
        simulator.SetEndTime(96.0); // Hours
       
        // Create a cell-cell repulsion force law and pass it to the simulation
        MAKE_PTR(RepulsionForce<3>, p_force); 
        simulator.AddForce(p_force);

        // Create a cell-coverslip adhesion force law and pass it to the simulation
        MAKE_PTR(CellCoverslipAdhesionForce<3>, p_cell_coverslip_force);
        simulator.AddForce(p_cell_coverslip_force);

        // Define a point on the plane boundary and a normal to the plane z=0.
        c_vector<double,3> point = zero_vector<double>(3);
        c_vector<double,3> normal = zero_vector<double>(3);
        normal(2) = -1.0;
        
        // Make a pointer to a PlaneBoundaryCondition (passing the point and normal to the plane) and pass it to the OffLatticeSimulation.
        MAKE_PTR_ARGS(PlaneBoundaryCondition<3>, p_bc, (&cell_population, point, normal));
        simulator.AddCellPopulationBoundaryCondition(p_bc);
        
        // Pass the cell killer into the cell-based simulation
        simulator.AddCellKiller(p_killer);
       
        // Add and pass the modifier to the simulation
        MAKE_PTR(CellHeightTrackingModifier<3>, p_modifier);
        simulator.AddSimulationModifier(p_modifier);

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
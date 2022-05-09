#include <cxxtest/TestSuite.h>

// Must be included before other cell_based headers
#include "CellBasedSimulationArchiver.hpp"

#include "AbstractCellBasedWithTimingsTestSuite.hpp"
#include "CellLabel.hpp"
#include "SmartPointers.hpp"
#include "CellsGenerator.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"
#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "NodeBasedCellPopulationWithParticles.hpp"

#include "HeterotypicBoundaryLengthWriter.hpp"

#include "HoneycombMeshGenerator.hpp"
#include "DifferentialAdhesionGeneralisedLinearSpringForce.hpp"
#include "RandomMotionForce.hpp"

#include "OnLatticeSimulation.hpp"
#include "CellPopulationAdjacencyMatrixWriter.hpp"

#include "CellIdWriter.hpp"
#include "CellMutationStatesWriter.hpp"

#include "PetscSetupAndFinalize.hpp"

static const double M_TIME_TO_STEADY_STATE = 10; //10
static const double M_TIME_FOR_SIMULATION = 100; //100
static const double M_NUM_CELLS_ACROSS = 20; //20 // this ^2 cells
static const double M_CELL_FLUCTUATION = 1.0;

class TestCellSortingWithParticles : public AbstractCellBasedWithTimingsTestSuite
{
private:

    void RandomlyLabelCells(std::list<CellPtr>& rCells, boost::shared_ptr<AbstractCellProperty> pLabel, double labelledRatio)
        {
            for (std::list<CellPtr>::iterator cell_iter = rCells.begin();
                 cell_iter != rCells.end();
                 ++cell_iter)
            {
                if (RandomNumberGenerator::Instance()->ranf() < labelledRatio)
                {
                   (*cell_iter)->AddCellProperty(pLabel);
                }
            }
        }

public:

    void TestNodeBasedMonolayerCellSorting()
        {
            // Create a simple mesh
            HoneycombMeshGenerator generator(M_NUM_CELLS_ACROSS, M_NUM_CELLS_ACROSS, 0);
            TetrahedralMesh<2,2>* p_generating_mesh = generator.GetMesh();

            //Extended to allow sorting for longer distances
            double cut_off_length = 2.5;

            // Convert this to a NodesOnlyMesh
            NodesOnlyMesh<2> mesh;
            mesh.ConstructNodesWithoutMesh(*p_generating_mesh, cut_off_length);

            // Set up cells, one for each Node
            std::vector<CellPtr> cells;
            MAKE_PTR(DifferentiatedCellProliferativeType, p_differentiated_type);
            CellsGenerator<UniformG1GenerationalCellCycleModel, 2> cells_generator;
            cells_generator.GenerateBasicRandom(cells, mesh.GetNumNodes(), p_differentiated_type);

            // Create cell population
            NodeBasedCellPopulation<2> cell_population(mesh, cells);

            // Set population to output all data to results files
            cell_population.AddCellWriter<CellIdWriter>();
            cell_population.AddCellWriter<CellMutationStatesWriter>();
            cell_population.AddPopulationWriter<HeterotypicBoundaryLengthWriter>();
            cell_population.AddPopulationWriter<CellPopulationAdjacencyMatrixWriter>();

            // Set up cell-based simulation and output directory
            OffLatticeSimulation<2> simulator(cell_population);
            simulator.SetOutputDirectory("CellSorting/Node");

            // Set time step and end time for simulation
            simulator.SetDt(1.0/200.0);
            simulator.SetSamplingTimestepMultiple(200);
            simulator.SetEndTime(M_TIME_TO_STEADY_STATE);

            // Create a force law and pass it to the simulation
            MAKE_PTR(DifferentialAdhesionGeneralisedLinearSpringForce<2>, p_differential_adhesion_force);
            p_differential_adhesion_force->SetMeinekeSpringStiffness(50.0);
            p_differential_adhesion_force->SetHomotypicLabelledSpringConstantMultiplier(1.0);
            p_differential_adhesion_force->SetHeterotypicSpringConstantMultiplier(0.1);
            p_differential_adhesion_force->SetCutOffLength(cut_off_length);
            simulator.AddForce(p_differential_adhesion_force);

            // Add some noise to avoid local minimum
            MAKE_PTR(RandomMotionForce<2>, p_random_force);
            p_random_force->SetMovementParameter(0.05); //0.1 causes dissasociation, 0.001 is not enough
            simulator.AddForce(p_random_force);

            // Run simulation
            simulator.Solve();

            // Now label some cells
            boost::shared_ptr<AbstractCellProperty> p_state(CellPropertyRegistry::Instance()->Get<CellLabel>());
            RandomlyLabelCells(simulator.rGetCellPopulation().rGetCells(), p_state, 0.5);

            // Adjust parameters
            p_random_force->SetMovementParameter(0.05*M_CELL_FLUCTUATION); //0.1 causes dissasociation

            // Run simulation
            simulator.SetEndTime(M_TIME_TO_STEADY_STATE + M_TIME_FOR_SIMULATION);
            simulator.Solve();

            // Check that the same number of cells
            TS_ASSERT_EQUALS(simulator.rGetCellPopulation().GetNumRealCells(), M_NUM_CELLS_ACROSS*M_NUM_CELLS_ACROSS);

            // Test no births or deaths
            TS_ASSERT_EQUALS(simulator.GetNumBirths(), 0u);
            TS_ASSERT_EQUALS(simulator.GetNumDeaths(), 0u);
        }
   };

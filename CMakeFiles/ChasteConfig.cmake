# - Config file for the Chaste package
# It defines the following variables
#  Chaste_INCLUDE_DIRS - include directories for FooBar
#  Chaste_LIBRARIES    - libraries to link against
 
# Compute paths

cmake_minimum_required(VERSION 2.8.10)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy) 
get_filename_component(Chaste_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

set(Chaste_VERSION_MAJOR 2019)
set(Chaste_VERSION_MINOR 1)

# If not in a build tree set required variables
if(NOT TARGET global AND NOT Chaste_BINARY_DIR)
    set(Chaste_BINARY_DIR ${Chaste_CMAKE_DIR})
    message("-- Found Chaste libraries in ${Chaste_BINARY_DIR}")
    add_definitions(-DCHASTE_CMAKE)

    set(Chaste_USE_VTK ON)
    if (Chaste_USE_VTK)
        add_definitions(-DCHASTE_VTK)
    endif()


    set(Chaste_USE_CVODE ON)
    if (Chaste_USE_CVODE)
        add_definitions(-DCHASTE_CVODE)
        add_definitions(-DCHASTE_SUNDIALS_VERSION=30102)
    endif()

    set(Chaste_USE_XERCES ON)
    if (Chaste_USE_XERCES)
        add_definitions(-DCHASTE_XERCES)
    endif()

    find_package(PythonInterp REQUIRED)
    set(Chaste_PYTHON_DIR ${Chaste_CMAKE_DIR}/python)

    option(BUILD_SHARED_LIBS
    "Set whether we are set whether to generate dynamic-linked libraries. ON by default"
    ON)

    include(${Chaste_CMAKE_DIR}/cmake/Modules/ChasteMacros.cmake)

    set(Chaste_NUM_CPUS_TEST 1 CACHE STRING  "Number of cpus to use when running tests.")
    set(Chaste_ENABLE_TESTING ON CACHE BOOL "Enable Chaste Testing")

    set(TestPackTypes Continuous;Failing;Nightly;Parallel;Production;Weekly;Profile;ProfileAssembly;ExtraSimulations)

    if(Chaste_ENABLE_TESTING)
        enable_testing()
        set(CXXTEST_INCLUDES "${Chaste_BINARY_DIR}/cxxtest")
        set(CXXTEST_PYTHON_TESTGEN_EXECUTABLE ${CXXTEST_INCLUDES}/cxxtestgen.py)
        foreach(type ${TestPackTypes})
            add_custom_target(${type})
        endforeach()
        add_custom_target(tutorials)
    endif()

    set(Chaste_DEPENDS_global )
    set(Chaste_DEPENDS_io global)
    set(Chaste_DEPENDS_linalg global)
    set(Chaste_DEPENDS_mesh linalg;global)
    set(Chaste_DEPENDS_ode linalg;io;global)
    set(Chaste_DEPENDS_pde ode;mesh;linalg;io;global)
    set(Chaste_DEPENDS_cell_based pde;ode;mesh;linalg;io;global)
    set(Chaste_DEPENDS_crypt cell_based;pde;ode;mesh;linalg;io;global)
    set(Chaste_DEPENDS_continuum_mechanics pde;ode;mesh;linalg;io;global)
    set(Chaste_DEPENDS_heart pde;ode;mesh;linalg;io;global;continuum_mechanics)
    set(Chaste_DEPENDS_lung pde;ode;mesh;linalg;io;global;continuum_mechanics)
    set(Chaste_DEPENDS_core global;io;linalg;mesh;ode;pde;continuum_mechanics)

    set(Chaste_global_INCLUDE_DIRS ${Chaste_CMAKE_DIR}/../../include/chaste/global/;${Chaste_CMAKE_DIR}/../../include/chaste/global/checkpointing;${Chaste_CMAKE_DIR}/../../include/chaste/global/fortests;${Chaste_CMAKE_DIR}/../../include/chaste/global/parallel;${Chaste_CMAKE_DIR}/../../include/chaste/global/random;${Chaste_CMAKE_DIR}/../../include/chaste/global/timing)
    set(Chaste_io_INCLUDE_DIRS ${Chaste_CMAKE_DIR}/../../include/chaste/io/common;${Chaste_CMAKE_DIR}/../../include/chaste/io/fortests;${Chaste_CMAKE_DIR}/../../include/chaste/io/reader;${Chaste_CMAKE_DIR}/../../include/chaste/io/writer)
    set(Chaste_linalg_INCLUDE_DIRS ${Chaste_CMAKE_DIR}/../../include/chaste/linalg/)
    set(Chaste_mesh_INCLUDE_DIRS ${Chaste_CMAKE_DIR}/../../include/chaste/mesh/3rdparty/tetgen1.4.2;${Chaste_CMAKE_DIR}/../../include/chaste/mesh/3rdparty/triangle;${Chaste_CMAKE_DIR}/../../include/chaste/mesh/common;${Chaste_CMAKE_DIR}/../../include/chaste/mesh/mutable;${Chaste_CMAKE_DIR}/../../include/chaste/mesh/reader;${Chaste_CMAKE_DIR}/../../include/chaste/mesh/utilities;${Chaste_CMAKE_DIR}/../../include/chaste/mesh/vertex;${Chaste_CMAKE_DIR}/../../include/chaste/mesh/writer)
    set(Chaste_ode_INCLUDE_DIRS ${Chaste_CMAKE_DIR}/../../include/chaste/ode/common;${Chaste_CMAKE_DIR}/../../include/chaste/ode/fortests;${Chaste_CMAKE_DIR}/../../include/chaste/ode/solver)
    set(Chaste_pde_INCLUDE_DIRS ${Chaste_CMAKE_DIR}/../../include/chaste/pde/common;${Chaste_CMAKE_DIR}/../../include/chaste/pde/postprocessing;${Chaste_CMAKE_DIR}/../../include/chaste/pde/problem;${Chaste_CMAKE_DIR}/../../include/chaste/pde/solver)
    set(Chaste_continuum_mechanics_INCLUDE_DIRS ${Chaste_CMAKE_DIR}/../../include/chaste/continuum_mechanics/common;${Chaste_CMAKE_DIR}/../../include/chaste/continuum_mechanics/problem;${Chaste_CMAKE_DIR}/../../include/chaste/continuum_mechanics/problem/material_laws;${Chaste_CMAKE_DIR}/../../include/chaste/continuum_mechanics/solver)
    set(Chaste_cell_based_INCLUDE_DIRS ${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/cell;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/cell/cycle;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/cell/properties;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/cell/properties/mutations;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/cell/properties/proliferative_types;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/cell/srn;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/cell_based_pde;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/cell_based_pde/pdes;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/common;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/fortests;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/mesh;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/odes;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/population;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/population/boundary_conditions;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/population/division_rules;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/population/forces;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/population/killers;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/population/update_rules;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/simulation;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/simulation/modifiers;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/simulation/numerical_methods;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/writers;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/writers/cell_writers;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/writers/population_count_writers;${Chaste_CMAKE_DIR}/../../include/chaste/cell_based/writers/population_writers)
    set(Chaste_crypt_INCLUDE_DIRS ${Chaste_CMAKE_DIR}/../../include/chaste/crypt/boundary_conditions;${Chaste_CMAKE_DIR}/../../include/chaste/crypt/cell;${Chaste_CMAKE_DIR}/../../include/chaste/crypt/cell/cycle;${Chaste_CMAKE_DIR}/../../include/chaste/crypt/division_rules;${Chaste_CMAKE_DIR}/../../include/chaste/crypt/forces;${Chaste_CMAKE_DIR}/../../include/chaste/crypt/killers;${Chaste_CMAKE_DIR}/../../include/chaste/crypt/odes;${Chaste_CMAKE_DIR}/../../include/chaste/crypt/simulation;${Chaste_CMAKE_DIR}/../../include/chaste/crypt/statistics;${Chaste_CMAKE_DIR}/../../include/chaste/crypt/writers)
    set(Chaste_lung_INCLUDE_DIRS ${Chaste_CMAKE_DIR}/../../include/chaste/lung/airway_generation;${Chaste_CMAKE_DIR}/../../include/chaste/lung/common;${Chaste_CMAKE_DIR}/../../include/chaste/lung/impedance;${Chaste_CMAKE_DIR}/../../include/chaste/lung/ventilation;${Chaste_CMAKE_DIR}/../../include/chaste/lung/ventilation/odes)
    set(Chaste_heart_INCLUDE_DIRS ${Chaste_CMAKE_DIR}/../../include/chaste/heart/convergence;${Chaste_CMAKE_DIR}/../../include/chaste/heart/fortests;${Chaste_CMAKE_DIR}/../../include/chaste/heart/io;${Chaste_CMAKE_DIR}/../../include/chaste/heart/odes;${Chaste_CMAKE_DIR}/../../include/chaste/heart/odes/contractionmodels;${Chaste_CMAKE_DIR}/../../include/chaste/heart/odes/ionicmodels;${Chaste_CMAKE_DIR}/../../include/chaste/heart/odes/ionicmodels/noncardiac;${Chaste_CMAKE_DIR}/../../include/chaste/heart/odes/modifiers;${Chaste_CMAKE_DIR}/../../include/chaste/heart/odes/single_cell;${Chaste_CMAKE_DIR}/../../include/chaste/heart/postprocessing;${Chaste_CMAKE_DIR}/../../include/chaste/heart/problem;${Chaste_CMAKE_DIR}/../../include/chaste/heart/problem/cell_factories;${Chaste_CMAKE_DIR}/../../include/chaste/heart/problem/conductivity_tensors;${Chaste_CMAKE_DIR}/../../include/chaste/heart/solver/electrics;${Chaste_CMAKE_DIR}/../../include/chaste/heart/solver/electrics/bidomain;${Chaste_CMAKE_DIR}/../../include/chaste/heart/solver/electrics/extended_bidomain;${Chaste_CMAKE_DIR}/../../include/chaste/heart/solver/electrics/monodomain;${Chaste_CMAKE_DIR}/../../include/chaste/heart/solver/mechanics;${Chaste_CMAKE_DIR}/../../include/chaste/heart/stimulus;${Chaste_CMAKE_DIR}/../../include/chaste/heart/stimulus/tissue_electrodes;${Chaste_CMAKE_DIR}/../../include/chaste/heart/tissue;${Chaste_CMAKE_DIR}/../../include/chaste/heart/odes/cellml;${Chaste_CMAKE_DIR}/../../include/chaste/heart/io)
    

    set(Chaste_INCLUDES "/home/chaste/src;/usr/include;/usr/include/hdf5/openmpi;/usr/lib/petscdir/petsc3.10/x86_64-linux-gnu-real/include;/usr/include/suitesparse;/usr/include/superlu;/usr/include/superlu-dist;/usr/include/hypre;/usr/include/hdf5/openmpi;/usr/include/scotch;/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi;/usr/lib/x86_64-linux-gnu/openmpi/include;/usr/include;/usr/include/sundials;/usr/include/vtk-6.3;/usr/include;/usr/include;/usr/include")
    set(Chaste_LINK_LIBRARIES "/usr/lib/x86_64-linux-gnu/libboost_filesystem.so;/usr/lib/x86_64-linux-gnu/libboost_system.so;/usr/lib/x86_64-linux-gnu/libboost_serialization.so;/usr/lib/x86_64-linux-gnu/libboost_program_options.so;/usr/lib/petscdir/petsc3.10/x86_64-linux-gnu-real/lib/libpetsc_real.so;/usr/lib/x86_64-linux-gnu/libdmumps.so;/usr/lib/x86_64-linux-gnu/libzmumps.so;/usr/lib/x86_64-linux-gnu/libsmumps.so;/usr/lib/x86_64-linux-gnu/libcmumps.so;/usr/lib/x86_64-linux-gnu/libmumps_common.so;/usr/lib/x86_64-linux-gnu/libpord.so;/usr/lib/x86_64-linux-gnu/libscalapack-openmpi.so;/usr/lib/x86_64-linux-gnu/libumfpack.so;/usr/lib/x86_64-linux-gnu/libamd.so;/usr/lib/x86_64-linux-gnu/libcholmod.so;/usr/lib/x86_64-linux-gnu/libklu.so;/usr/lib/x86_64-linux-gnu/libsuperlu.so;/usr/lib/x86_64-linux-gnu/libsuperlu_dist.so;/usr/lib/x86_64-linux-gnu/libHYPRE_core.so;/usr/lib/x86_64-linux-gnu/libfftw3.so;/usr/lib/x86_64-linux-gnu/libfftw3_mpi.so;/usr/lib/x86_64-linux-gnu/liblapack.so;/usr/lib/x86_64-linux-gnu/libblas.so;/usr/lib/x86_64-linux-gnu/hdf5/openmpi/libhdf5.so;/usr/lib/x86_64-linux-gnu/libptesmumps.so;/usr/lib/x86_64-linux-gnu/libptscotch.so;/usr/lib/x86_64-linux-gnu/libptscotcherr.so;/usr/lib/x86_64-linux-gnu/openmpi/lib/libmpi_usempif08.so;/usr/lib/x86_64-linux-gnu/openmpi/lib/libmpi_usempi_ignore_tkr.so;/usr/lib/x86_64-linux-gnu/openmpi/lib/libmpi_mpifh.so;/usr/lib/x86_64-linux-gnu/openmpi/lib/libmpi.so;/usr/lib/gcc/x86_64-linux-gnu/8/libgfortran.so;/usr/lib/x86_64-linux-gnu/libm.so;/usr/lib/gcc/x86_64-linux-gnu/8/libgcc_s.so;/usr/lib/gcc/x86_64-linux-gnu/8/libquadmath.so;/usr/lib/x86_64-linux-gnu/libpthread.so;/usr/lib/gcc/x86_64-linux-gnu/8/libstdc++.so;/usr/lib/x86_64-linux-gnu/libdl.so;/usr/lib/x86_64-linux-gnu/hdf5/openmpi/libhdf5.so;/usr/lib/x86_64-linux-gnu/libsz.so;/usr/lib/x86_64-linux-gnu/libz.so;/usr/lib/x86_64-linux-gnu/libdl.so;/usr/lib/x86_64-linux-gnu/libm.so;/usr/lib/libparmetis.so;/usr/lib/x86_64-linux-gnu/libmetis.so;/usr/lib/x86_64-linux-gnu/libsundials_cvode.so;/usr/lib/x86_64-linux-gnu/libsundials_nvecserial.so;/usr/lib/x86_64-linux-gnu/liblapack.so;/usr/lib/x86_64-linux-gnu/libblas.so;/usr/lib/x86_64-linux-gnu/openmpi/lib/libmpi_cxx.so;/usr/lib/x86_64-linux-gnu/openmpi/lib/libmpi.so;vtkCommonCore;vtksys;vtkCommonDataModel;vtkCommonMath;vtkCommonMisc;vtkCommonSystem;vtkCommonTransforms;vtkFiltersCore;vtkCommonExecutionModel;vtkFiltersGeneral;vtkCommonComputationalGeometry;vtkFiltersGeneric;vtkFiltersSources;vtkFiltersGeometry;vtkFiltersModeling;vtkIOCore;/usr/lib/x86_64-linux-gnu/libz.so;vtkIOGeometry;vtkIOLegacy;vtkIOParallelXML;vtkIOXML;vtkIOXMLParser;/usr/lib/x86_64-linux-gnu/libexpat.so;vtkParallelCore;/usr/lib/x86_64-linux-gnu/libxerces-c.so")

    set(Chaste_ALL_LIBRARIES "chaste_global;chaste_io;chaste_linalg;chaste_mesh;chaste_ode;chaste_pde;chaste_continuum_mechanics;chaste_cell_based;chaste_crypt;chaste_lung;chaste_heart")

    # Our library dependencies (contains definitions for IMPORTED targets)
    include("${Chaste_CMAKE_DIR}/ChasteTargets.cmake")
endif()

set(Chaste_THIRD_PARTY_INCLUDE_DIRS "${Chaste_INCLUDES}")
set(Chaste_THIRD_PARTY_LIBRARIES "${Chaste_LINK_LIBRARIES}")
set(Chaste_LIBRARIES "")
set(Chaste_INCLUDE_DIRS "")
set(Chaste_FIND_COMPONENTS_AND_DEPS "")


# If no components are specified include core
if (NOT Chaste_FIND_COMPONENTS)
    set(Chaste_FIND_COMPONENTS_AND_DEPS ${Chaste_DEPENDS_core})
endif()

# Setup dependent components in case they are not specified
set(remaining_components ${Chaste_FIND_COMPONENTS})
foreach(component ${Chaste_FIND_COMPONENTS})
    list(REMOVE_AT remaining_components 0)
    if (NOT(Chaste_FIND_COMPONENTS STREQUAL "none"))
        list(APPEND Chaste_FIND_COMPONENTS_AND_DEPS ${component}) 
        foreach(dependant_component ${Chaste_DEPENDS_${component}})
            list(FIND remaining_components ${dependant_component} dep_component_found)
            if (dep_component_found EQUAL -1)
                list(APPEND Chaste_FIND_COMPONENTS_AND_DEPS ${Chaste_DEPENDS_${component}})
            endif()
        endforeach()
    endif()
endforeach(component)

list(REMOVE_DUPLICATES Chaste_FIND_COMPONENTS_AND_DEPS)

#message("Chaste_FIND_COMPONENTS = ${Chaste_FIND_COMPONENTS}, Chaste_FIND_COMPONENTS_AND_DEPS = ${Chaste_FIND_COMPONENTS_AND_DEPS}")
foreach(component ${Chaste_FIND_COMPONENTS_AND_DEPS})
    # Set up the component include directories
    list(APPEND Chaste_INCLUDE_DIRS "${Chaste_${component}_INCLUDE_DIRS}")

    if (NOT TARGET chaste_${component})
        message(SEND_ERROR "Cannot find component ${component}")
    elseif(Chaste_USE_SHARED_LIBS)
        get_target_property(is_shared chaste_${component} POSITION_INDEPENDENT_CODE)
        if (NOT is_shared)
            message(SEND_ERROR "Requesting shared libraries but ${component} is not a shared library!")
        endif()
    endif()

    # These are IMPORTED targets created by ChasteTargets.cmake
    list(APPEND Chaste_LIBRARIES chaste_${component})
endforeach(component)


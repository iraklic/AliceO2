# Copyright 2019-2020 CERN and copyright holders of ALICE O2.
# See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
# All rights not expressly granted are reserved.
#
# This software is distributed under the terms of the GNU General Public
# License v3 (GPL Version 3), copied verbatim in the file "COPYING".
#
# In applying this license CERN does not waive the privileges and immunities
# granted to it by virtue of its status as an Intergovernmental Organization
# or submit itself to any jurisdiction.

o2_add_library(TPCReconstruction
               TARGETVARNAME targetName
               SOURCES src/AdcClockMonitor.cxx
                       src/ClustererTask.cxx
                       src/GBTFrame.cxx
                       src/GBTFrameContainer.cxx
                       src/HalfSAMPAData.cxx
                       src/HwClusterer.cxx
                       src/HwClustererParam.cxx
                       src/KrBoxClusterFinder.cxx
                       src/KrBoxClusterFinderParam.cxx
                       src/RawReader.cxx
                       src/RawReaderCRU.cxx
                       src/RawReaderEventSync.cxx
                       src/SyncPatternMonitor.cxx
                       src/TPCTrackingDigitsPreCheck.cxx
                       src/HardwareClusterDecoder.cxx
                       src/DigitalCurrentClusterIntegrator.cxx
                       src/TPCFastTransformHelperO2.cxx
                       src/CTFCoder.cxx
                       src/RawProcessingHelpers.cxx
               PUBLIC_LINK_LIBRARIES FairRoot::Base
                                     O2::SimulationDataFormat
                                     O2::TPCBase
                                     O2::GPUO2Interface
                                     O2::TPCFastTransformation
                                     O2::DetectorsRaw)

o2_target_root_dictionary(
  TPCReconstruction
  HEADERS include/TPCReconstruction/AdcClockMonitor.h
          include/TPCReconstruction/Clusterer.h
          include/TPCReconstruction/ClustererTask.h
          include/TPCReconstruction/GBTFrame.h
          include/TPCReconstruction/GBTFrameContainer.h
          include/TPCReconstruction/HalfSAMPAData.h
          include/TPCReconstruction/HwClusterer.h
          include/TPCReconstruction/HwClustererParam.h
          include/TPCReconstruction/KrBoxClusterFinder.h
          include/TPCReconstruction/KrBoxClusterFinderParam.h
          include/TPCReconstruction/RawReader.h
          include/TPCReconstruction/RawReaderCRU.h
          include/TPCReconstruction/RawReaderEventSync.h
          include/TPCReconstruction/SyncPatternMonitor.h
          include/TPCReconstruction/TPCTrackingDigitsPreCheck.h
          include/TPCReconstruction/HardwareClusterDecoder.h
          include/TPCReconstruction/DigitalCurrentClusterIntegrator.h
          include/TPCReconstruction/TPCFastTransformHelperO2.h
          include/TPCReconstruction/CTFCoder.h
          include/TPCReconstruction/RawProcessingHelpers.h)

o2_add_executable(read-gbtframes
                  COMPONENT_NAME tpc
                  PUBLIC_LINK_LIBRARIES O2::TPCReconstruction
                  SOURCES run/readGBTFrames.cxx)

o2_add_executable(read-rawdata
                  COMPONENT_NAME tpc
                  PUBLIC_LINK_LIBRARIES O2::TPCReconstruction
                  SOURCES run/readRawData.cxx)

o2_add_executable(rawreader-cru
                  COMPONENT_NAME tpc
                  PUBLIC_LINK_LIBRARIES O2::TPCReconstruction
                  SOURCES run/rawReaderCRU.cxx)

o2_add_test(SyncPatternMonitor
            COMPONENT_NAME tpc
            LABELS tpc
            PUBLIC_LINK_LIBRARIES O2::TPCReconstruction
            SOURCES test/testTPCSyncPatternMonitor.cxx)

o2_add_test(AdcClockMonitor
            COMPONENT_NAME tpc
            LABELS tpc
            PUBLIC_LINK_LIBRARIES O2::TPCReconstruction
            SOURCES test/testTPCAdcClockMonitor.cxx)

o2_add_test(GPUCATracking
            COMPONENT_NAME tpc
            LABELS tpc
            PUBLIC_LINK_LIBRARIES O2::TPCReconstruction
            SOURCES test/testGPUCATracking.cxx
            ENVIRONMENT O2_ROOT=${CMAKE_BINARY_DIR}/stage)

o2_add_test(HwClusterer
            COMPONENT_NAME tpc
            LABELS tpc
            PUBLIC_LINK_LIBRARIES O2::TPCReconstruction
            SOURCES test/testTPCHwClusterer.cxx
            ENVIRONMENT O2_ROOT=${CMAKE_BINARY_DIR}/stage)

# The FastTransform  test seems really slow in Debug mode, so use it only in
# release mode (use CONFIGURATIONS keyword)
# update: currently it is fast, switch the test on also for debug
o2_add_test(FastTransform
            COMPONENT_NAME tpc
            LABELS tpc
            PUBLIC_LINK_LIBRARIES O2::TPCReconstruction
            SOURCES test/testTPCFastTransform.cxx
            ENVIRONMENT O2_ROOT=${CMAKE_BINARY_DIR}/stage
            CONFIGURATIONS RelWithDebInfo Release MinRelSize)

# FIXME: should be moved to TPCCalibration as it requires O2::TPCCalibration
# which is built after TPCReconstruction
# o2_add_test_root_macro(macro/RawClusterFinder.C PUBLIC_LINK_LIBRARIES
# O2::DataFormatsTPC O2::TPCBase O2::TPCCalibration O2::TPCReconstruction)

o2_add_test_root_macro(macro/readClusters.C
                       PUBLIC_LINK_LIBRARIES O2::DataFormatsTPC
                                             O2::SimulationDataFormat
                                             O2::TPCBase
                       LABELS tpc COMPILE_ONLY)
o2_add_test_root_macro(macro/testRawRead.C
                       PUBLIC_LINK_LIBRARIES O2::TPCReconstruction
                       LABELS tpc)

o2_add_test_root_macro(macro/createTPCSpaceChargeCorrection.C
                       COMPILE_ONLY
                       PUBLIC_LINK_LIBRARIES O2::TPCReconstruction
                                             O2::CommonConstants
                                             O2::CommonUtils
                                             O2::TPCSpaceCharge
                       LABELS tpc)

o2_add_test_root_macro(macro/findKrBoxCluster.C
                      PUBLIC_LINK_LIBRARIES O2::TPCReconstruction
                      LABELS tpc)

# FIXME: should be moved to TPCSimulation as it requires O2::TPCSimulation
# target which is built after reconstruction
# o2_add_test_root_macro(macro/testTracks.C PUBLIC_LINK_LIBRARIES
# ReconstructionDataFormats O2::TPCBase O2::DataFormatsTPC O2::TPCSimulation)

if(OpenMP_CXX_FOUND)
  message(STATUS "GPU: Using OpenMP: ${OpenMP_CXX_SPEC_DATE}")
  # Must be private, depending libraries might be compiled by compiler not understanding -fopenmp
  target_compile_definitions(${targetName} PRIVATE WITH_OPENMP)
  target_link_libraries(${targetName} PRIVATE OpenMP::OpenMP_CXX)
endif()

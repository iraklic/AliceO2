// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file   MCHWorkflow/DigitReaderSpec.h
/// \brief  Data processor specs for MCH digits reader device
/// \author Michael Winn <Michael.Winn at cern.ch>
/// \date   17 April 2021

#ifndef O2_MCH_DIGITREADERSPEC_H
#define O2_MCH_DIGITREADERSPEC_H

#include "Framework/DataProcessorSpec.h"

namespace o2
{
namespace mch
{
framework::DataProcessorSpec getDigitReaderSpec(bool useMC, const char* name = "mch-sim-digit-reader");
}
} // namespace o2

#endif

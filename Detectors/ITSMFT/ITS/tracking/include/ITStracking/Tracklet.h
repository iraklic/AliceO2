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
///
/// \file Tracklet.h
/// \brief
///

#ifndef TRACKINGITSU_INCLUDE_TRACKLET_H_
#define TRACKINGITSU_INCLUDE_TRACKLET_H_

#include "ITStracking/Cluster.h"
#include <iostream>
#include "GPUCommonMath.h"
#include "GPUCommonDef.h"

namespace o2
{
namespace its
{

struct Tracklet final {
  Tracklet();
  GPUdi() Tracklet(const int, const int, const Cluster&, const Cluster&, int rof0, int rof1);
#ifdef _ALLOW_DEBUG_TREES_ITS_
  unsigned char isEmpty() const;
  void dump();
  unsigned char operator<(const Tracklet&) const;
#endif

  int firstClusterIndex;
  int secondClusterIndex;
  float tanLambda;
  float phi;
  unsigned short rof[2];
};

inline Tracklet::Tracklet() : firstClusterIndex{0}, secondClusterIndex{0}, tanLambda{0.0f}, phi{0.0f}
{
  // Nothing to do
}

GPUdi() Tracklet::Tracklet(const int firstClusterOrderingIndex, const int secondClusterOrderingIndex,
                           const Cluster& firstCluster, const Cluster& secondCluster, int rof0 = -1, int rof1 = -1)
  : firstClusterIndex{firstClusterOrderingIndex},
    secondClusterIndex{secondClusterOrderingIndex},
    tanLambda{(firstCluster.zCoordinate - secondCluster.zCoordinate) /
              (firstCluster.radius - secondCluster.radius)},
    phi{o2::gpu::GPUCommonMath::ATan2(firstCluster.yCoordinate - secondCluster.yCoordinate,
                                      firstCluster.xCoordinate - secondCluster.xCoordinate)},
    rof{static_cast<unsigned short>(rof0), static_cast<unsigned short>(rof1)}
{
  // Nothing to do
}

#ifdef _ALLOW_DEBUG_TREES_ITS_
inline unsigned char Tracklet::isEmpty() const
{
  return !firstClusterIndex && !secondClusterIndex && !tanLambda && !phi;
}

inline unsigned char Tracklet::operator<(const Tracklet& t) const
{
  if (isEmpty() && t.isEmpty()) {
    return false;
  } else {
    if (isEmpty())
      return false;
  }
  return true;
}

inline void Tracklet::dump()
{
  std::cout << "firstClusterIndex: " << firstClusterIndex << std::endl;
  std::cout << "secondClusterIndex: " << secondClusterIndex << std::endl;
  std::cout << "tanLambda: " << tanLambda << std::endl;
  std::cout << "phi: " << phi << std::endl;
}
#endif

} // namespace its
} // namespace o2

#endif /* TRACKINGITSU_INCLUDE_TRACKLET_H_ */

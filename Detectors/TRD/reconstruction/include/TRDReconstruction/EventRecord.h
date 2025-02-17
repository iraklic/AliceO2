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

#ifndef ALICEO2_TRD_EVENTRECORD_H
#define ALICEO2_TRD_EVENTRECORD_H

#include <iosfwd>
#include "Rtypes.h"
#include "TH2F.h"
#include "CommonDataFormat/InteractionRecord.h"
#include "CommonDataFormat/RangeReference.h"
#include "FairLogger.h"
#include "DataFormatsTRD/Tracklet64.h"
#include "DataFormatsTRD/RawDataStats.h"
#include "DataFormatsTRD/Digit.h"

namespace o2::framework
{
class ProcessingContext;
}

namespace o2::trd
{
class TriggerRecord;

/// \class EventRecord
/// \brief Stores a TRD event
/// adapted from TriggerRecord

class EventRecord
{
  using BCData = o2::InteractionRecord;

 public:
  EventRecord() = default;
  EventRecord(BCData& bunchcrossing) : mBCData(bunchcrossing)
  {
    mTracklets.reserve(30);
    mDigits.reserve(20);
  }
  ~EventRecord() = default;

  void setBCData(const BCData& data) { mBCData = data; }

  const BCData& getBCData() const { return mBCData; }
  BCData& getBCData() { return mBCData; }

  //Digit information
  std::vector<Digit>& getDigits();
  void addDigits(Digit& digit);
  void addDigits(std::vector<Digit>::iterator& start, std::vector<Digit>::iterator& end);

  //tracklet information
  std::vector<Tracklet64>& getTracklets();
  void addTracklet(Tracklet64& tracklet);
  void addTracklets(std::vector<Tracklet64>::iterator& start, std::vector<Tracklet64>::iterator& end);
  void addTracklets(std::vector<Tracklet64>& tracklets);
  void popTracklets(int popcount);
  //void printStream(std::ostream& stream) const;
  void sortByHCID();

  bool operator==(const EventRecord& o) const
  {
    return mBCData == o.mBCData; //&& mDigits == o.mDigits && mTracklets == o.mTracklets ;
  }
  void clear()
  {
    mDigits.clear();
    mTracklets.clear();
  }

 private:
  BCData mBCData;                       /// orbit and Bunch crossing data of the physics trigger
  std::vector<Digit> mDigits{};         /// digit data, for this event
  std::vector<Tracklet64> mTracklets{}; /// tracklet data, for this event
  o2::trd::TRDDataCountersPerEvent mEventStats;
};

class EventStorage
{
  //store a timeframes events for later collating sending on as a message
 public:
  EventStorage() = default;
  ~EventStorage() = default;
  //storage of eventrecords
  //a vector of eventrecords and the associated funationality to go with it.
  void clear() { mEventRecords.clear(); }
  void addDigits(InteractionRecord& ir, Digit& digit);
  void addDigits(InteractionRecord& ir, std::vector<Digit>::iterator start, std::vector<Digit>::iterator end);
  void addTracklet(InteractionRecord& ir, Tracklet64& tracklet);
  void addTracklets(InteractionRecord& ir, std::vector<Tracklet64>& tracklets);
  void addTracklets(InteractionRecord& ir, std::vector<Tracklet64>::iterator& start, std::vector<Tracklet64>::iterator& end);
  void unpackData(std::vector<TriggerRecord>& triggers, std::vector<Tracklet64>& tracklets, std::vector<Digit>& digits);
  void sendData(o2::framework::ProcessingContext& pc, bool displaytracklets = false);
  EventRecord& getEventRecord(InteractionRecord& ir);
  //this could replace by keeing a running total on addition TODO
  void sumTrackletsDigitsTriggers(uint64_t& tracklets, uint64_t& digits, uint64_t& triggers);
  int sumTracklets();
  int sumDigits();
  std::vector<Tracklet64>& getTracklets(InteractionRecord& ir);
  std::vector<Digit>& getDigits(InteractionRecord& ir);
  void printIR();
  void setHisto(TH1F* packagetime) { mPackagingTime = packagetime; }
  //TODO what would be nice is to write this out as a root tree event by event instead of using the sendData method where its all packaged together to then be unpackaged again.
  TRDDataCountersPerTimeFrame mTFStats;

 private:
  std::vector<EventRecord> mEventRecords;
  //these 2 are hacks to be able to send bak a blank vector if interaction record is not found.
  std::vector<Tracklet64> mDummyTracklets;
  std::vector<Digit> mDummyDigits;
  TH1F* mPackagingTime{nullptr};
};

std::ostream& operator<<(std::ostream& stream, const EventRecord& trg);

} // namespace o2::trd

#endif

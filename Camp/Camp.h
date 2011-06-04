// Copyright 2011 Kevin Heifner.  All rights reserved.

#ifndef CAMP_H
#define CAMP_H
#pragma once

#include "Archiver.h"
#include "Camper.h"
#include "refptr.h"
#include <map>
#include <string>

class Archiver;

/// Main control class of application
class Camp {
public:
  Camp() {}

  /// Init finger print reader, open database, load values.
  void start();
  /// Close finger print reader, close database
  void stop();

  /// Interact with finger print reader to get finger print
  /// and save new Camper to database.
  FPKey addCamper(
    const std::string& firstName,
    const std::string& lastName,
    float amount);

  /// Interact with finger print reader to find Camper.
  FPKey findCamper() const;

  /// Does not interact with finger print reader.
  /// @return Camper from memory given id.
  ref_ptr<Camper> getCamper(FPKey id) const;

  /// Remove Camper from database.
  /// Does not interact with finger print reader.
  void deleteCamper(FPKey id);

  /// Update Camper amount in database.
  /// Does not interact with finger print reader.
  void updateAmount(FPKey id, float amount);

  /// Cancel current finger print reader operation.
  /// Should be called from different thread.
  void cancelOperation();

  /// Provide iteration over all Campers.
  typedef std::map< FPKey, ref_ptr<Camper> > FPKeyToCamperMap;
  FPKeyToCamperMap::const_iterator begin() const { return idToCamper_.begin(); }
  FPKeyToCamperMap::const_iterator end() const { return idToCamper_.end(); }

  /// Config Values

  /// DefaultPrice = 0.50
  /// @return the default price for items from the config file.
  ///         If no default price in config file, return $0.50.
  float getDefaultPrice() const;

protected: // friendly
  friend class Archiver;

  // For use by Archiver only
  void addCamper(const ref_ptr<Camper>& camper, const void* blob);

private:
  Camp(const Camp&); // disable
  Camp& operator=(const Camp&); // disable

private:
  Archiver db_;
  FPKeyToCamperMap idToCamper_;
  std::string fileName_;
};

#endif // CAMP_H

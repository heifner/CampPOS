// Copyright 2011 Kevin Heifner.  All rights reserved.

#ifndef CAMP_H
#define CAMP_H
#pragma once

#include "Camper.h"
#include "refptr.h"
#include <map>
#include <string>

/// Control class of application
class Camp {
public:
  Camp() {}

  void start();
  void stop();

  FPKey addCamper(
    const std::string& firstName,
    const std::string& lastName,
    float amount);

  FPKey findCamper() const;
  ref_ptr<Camper> getCamper(FPKey id) const;

  void deleteCamper(FPKey id);

  void updateAmount(FPKey id, float amount);

  typedef std::map< FPKey, ref_ptr<Camper> > FPKeyToCamperMap;

  FPKeyToCamperMap::const_iterator begin() const { return idToCamper_.begin(); }
  FPKeyToCamperMap::const_iterator end() const { return idToCamper_.end(); }

private:
  Camp(const Camp&); // disable
  Camp& operator=(const Camp&); // disable

private:
  FPKeyToCamperMap idToCamper_;
  std::string fileName_;
};

#endif // CAMP_H

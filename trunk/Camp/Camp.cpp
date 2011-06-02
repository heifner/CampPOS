// Copyright 2011 Kevin Heifner.  All rights reserved.

#include "Camp.h"
#include "Config.h"
#include "FPManager.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

namespace {

  void createBackup(const std::string& fileName) {
    ifstream ifs(fileName.c_str(), ios::binary);
    if (!ifs) {
      // if unable to open, then likely doesn't exist, so no need to backup
      return;
    }

    // create backup
    const std::string backupFileName = fileName + ".bak";
    ofstream ofs(backupFileName.c_str(), ios::binary);
    if (!ofs) {
      std::string err = "Unable to open file: " + backupFileName;
      throw std::runtime_error(err);
    }
    char ch;
    while (ifs && ifs.get(ch)) {
      ofs.put(ch);
    }
  }
}

void
Camp::start()
{
  if (!Config::init("CampPOS.ini")) {
    std::string err = "Unable to find CampPOS.ini file.";
    throw std::runtime_error(err);
  }

  if (!FPManager::instance().open()) {
    std::string err = "Unable to start finger print reader:\n";
    err += FPManager::instance().getLastError();
    throw std::runtime_error(err);
  }

  fileName_ = Config::getString("DBFile");
  if (fileName_.empty()) {
    std::string err = "DBFile not specified in CampPOS.ini.";
    throw std::runtime_error(err);
  }

  createBackup(fileName_);

  db_.open(fileName_);
  db_.load(*this);
}

void 
Camp::addCamper(const ref_ptr<Camper>& camper, const void* blob)
{
  FPManager::instance().createEntry(camper->getId(), blob);
  idToCamper_[camper->getId()] = camper;
}


void
Camp::stop()
{
  db_.close();

  // doesn't matter if it fails
  FPManager::instance().close();
}

FPKey 
Camp::addCamper(const std::string& firstName,
                const std::string& lastName,
                float amount)
{
  FPKey id = 0;
  if (!FPManager::instance().createEntry(id)) {
    std::string err = "Unable to add camper (" + firstName + ", " + lastName + "):\n";
    err += FPManager::instance().getLastError();
    throw std::runtime_error(err);
  }

  ref_ptr<Camper> camper(new Camper(id, firstName, lastName));
  camper->setAmount(amount);
  idToCamper_[id] = camper;
  
  // add to database
  std::pair<size_t,void*> entry = FPManager::instance().getEntry(id);
  db_.insertRow(*camper, entry.first, entry.second);
  return id;
}

FPKey 
Camp::findCamper() const
{
  FPKey id = 0;
  if (!FPManager::instance().findEntry(id)) {
    std::string err = "Unable to find camper.\n";
    err += FPManager::instance().getLastError();
    throw std::runtime_error(err);
  }
  return id;
}

ref_ptr<Camper> 
Camp::getCamper(FPKey id) const
{
  FPKeyToCamperMap::const_iterator i = idToCamper_.find(id);
  if (i == idToCamper_.end()) {
    std::stringstream ss;
    ss << "Unable to find camper id: " << id << endl;
    throw std::runtime_error(ss.str());
  }
  return i->second;
}

void 
Camp::deleteCamper(FPKey id)
{
  FPKeyToCamperMap::iterator i = idToCamper_.find(id);
  if (i == idToCamper_.end()) {
    std::stringstream ss;
    ss << "Unable to find camper id: " << id << endl;
    throw std::runtime_error(ss.str());
  }
  ref_ptr<Camper> camper = i->second;
  // Go ahead and remove from Camp map even if unable to remove from FPManager.
  idToCamper_.erase(i);
  // Also remove from FPManager
  if (!FPManager::instance().deleteEntry(id)) {
    std::string err = "Unable to delete camper (" + camper->getFirstName() + ", " + camper->getLastName() + "):\n";
    err += FPManager::instance().getLastError();
    throw std::runtime_error(err);
  }
  // Remove from database
  db_.deleteRow(id);
}

void 
Camp::updateAmount(FPKey id, float amount)
{
  FPKeyToCamperMap::iterator i = idToCamper_.find(id);
  if (i == idToCamper_.end()) {
    std::stringstream ss;
    ss << "Unable to find camper id: " << id << endl;
    throw std::runtime_error(ss.str());
  }
  ref_ptr<Camper> camper = i->second;
  camper->setAmount(amount);
  // Update database
  db_.updateRow(id, amount);
}

void
Camp::cancelOperation()
{
  FPManager::instance().cancel();
}

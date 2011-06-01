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




/////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Writes down simple help. */
static void
cmd_help(void)
{
    printf("Press any of the following keys to do the corresponding action:\n");
    printf("   a ... add new template into the template set\n");
    printf("   i ... import a template into the template set\n");
    printf("   e ... export a template from the template set to a file\n");
    printf("   d ... delete template from the template set\n");
    printf("   D ... delete all templates from the template set\n");
    printf("   l ... list templates in the template set\n");
    printf("   v ... verify finger against one template\n");
    printf("   V ... verify finger against complete template set\n");
    printf("   m ... match two templates from the template set\n");
    printf("   h ... writes down this help message\n");
    printf("   q ... quit this sample program\n");
}

int
main_test(int, char**)
{
  try {
    char c = 48;
    int done = 0;
    char buffer[256];
    
    /* On the start, always show the help */
    cmd_help();

    Camp camp;
    camp.start();

    FPKey key = 0;
    while(!done) {
      ++c;
        /* Ask the use what to do... */
        printf("\n");
        printf("\n>> ");
        scanf("%s", buffer);
        
        /* ...and then just do that. */
        switch(buffer[0]) {
            case 'a': {
              std::string f = "kevin";
              f += c;
              key = camp.addCamper(f, "heifner", 50.0f);
              break;
                      }
//            case 'i': cmd_import();  break;
//            case 'e': cmd_export();  break;
            case 'd': 
              camp.deleteCamper(key);
              break;
//            case 'D': cmd_delete_all(); break;
//            case 'l': cmd_list(); break;
//            case 'v': cmd_verify(); break;
            case 'V': { 
              key = camp.findCamper();
              ref_ptr<Camper> camper = camp.getCamper(key);
              cout << "Camper: " << camper->getFirstName() << endl;
                      }
              break;
//            case 'm': cmd_verify_match(); break;
            case 'q': done = 1; break;
            
            case '?':
            case 'h': cmd_help(); break;
            
            default:  
                printf("Unknown command. Press 'h' to get help.\n"); 
                break;
        }
    }

    camp.stop();

  } catch (std::exception& e) {
    cerr << "Exception: " << e.what() << endl;
  }
  return 0;
}

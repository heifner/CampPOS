// Copyright 2011 Kevin Heifner.  All rights reserved.
#include "Archiver.h"
#include "Camp.h"
#include "Camper.h"
#include "sdsqlite.h"
#include <ctime>
#include <cstdio>
#include <sstream>
#include <stdexcept>
#include <vector>

Archiver::Archiver()
: db_(new sd::sqlite())
, transLog_()
{
}

Archiver::~Archiver()
{
  delete db_;
}

void
Archiver::open(const std::string& dbFileName, const std::string& transFileName)
{
  db_->open(dbFileName);
  *db_ << "create table if not exists Campers (Id integer primary key, FirstName text, LastName text, Amount real, FingerPrint blob)";

  transLog_.open(transFileName.c_str(), std::ios::app);
  if (!transLog_) {
    std::string err("Unable to open transaction log: ");
    err += transFileName;
    throw std::runtime_error(err);
  }
}

void
Archiver::close()
{
  db_->close();
}

void 
Archiver::load(Camp& camp)
{
  sd::sql query(*db_);
  query << "select Id, FirstName, LastName, Amount, FingerPrint from Campers";

  while (query.step()) {
    long long id = 0;
    std::string firstName, lastName;
    float amount = 0.0f;
    const void* blob = NULL;
    size_t bytes = 0;

    query >> id >> firstName >> lastName >> amount;
    query.unbind(blob, bytes);

    ref_ptr<Camper> camper(new Camper(id, firstName, lastName, amount));
    camp.addCamper(camper, blob);
  }	
}

void 
Archiver::insertRow(const Camper& camper, size_t blobSize, const void* blob)
{
  sd::sql query(*db_);	// build an sql query
  query << "insert into Campers (Id, FirstName, LastName, Amount, FingerPrint) VALUES(?, ?, ?, ?, ?)";
  query << camper.getId() << camper.getFirstName() << camper.getLastName() << camper.getAmount();
  query.bind(blob, blobSize);

  // log transaction
  std::stringstream ssLog;
  ssLog << "Added new Camper: " << camper.getId() << " " << camper.getFirstName() << " " << camper.getLastName() << " with amount " << camper.getAmount();
  logTransaction(ssLog.str());
}

void 
Archiver::updateRow(long long id, float amount)
{
  std::stringstream ss;
  ss << "update Campers set Amount='" << amount << "' where Id =" << id;
  *db_ << ss.str();

  // log transaction
  std::stringstream ssLog;
  ref_ptr<Camper> camper = getCamper(id);
  if (camper != 0) {
    ssLog << "Updated Camper: " << camper->getId() << " " << camper->getFirstName() << " " << camper->getLastName() << " with amount " << camper->getAmount();
  } else {
    ssLog << "WARN: Unable to find Camper after: " << ss.str();
  }
  logTransaction(ssLog.str());
}

void 
Archiver::deleteRow(long long id)
{
  std::stringstream ss;
  ss << "delete from Campers where Id =" << id;

  std::stringstream ssLog;
  ref_ptr<Camper> camper = getCamper(id);
  if (camper != 0) {
    ssLog << "Deleted Camper: " << camper->getId() << " " << camper->getFirstName() << " " << camper->getLastName() << " with amount " << camper->getAmount();    
  } else {
    ssLog << "WARN: Unable to find Camper for: " << ss.str();
  }

  // execute delete
  *db_ << ss.str();

  // log transaction
  logTransaction(ssLog.str());
}

ref_ptr<Camper> 
Archiver::getCamper(long long id)
{
  sd::sql query(*db_);
  std::stringstream ss;
  ss << "select Id, FirstName, LastName, Amount from Campers where Id = " << id;
  query << ss.str();

  ref_ptr<Camper> camper;
  if (query.step()) {
    long long id = 0;
    std::string firstName, lastName;
    float amount = 0.0f;
    query >> id >> firstName >> lastName >> amount;
    camper = ref_ptr<Camper>(new Camper(id, firstName, lastName, amount));
  }
  return camper;
}

void 
Archiver::logTransaction(const std::string& logEntry)
{
  transLog_ << getDateTimeString("%X") << " " << logEntry << std::endl;
}

std::string 
Archiver::getDateTimeString(const std::string& fmt)
{
  using namespace std;
  string result;
  size_t size = fmt.size() + 256;
  vector<char> carr(size, '\0');
  time_t now = 0;
  time(&now);
  tm t = {};
  // if failures then return an empty string.
  if (0 == localtime_s(&t, &now)) {
    strftime(&carr[0], carr.size(), fmt.c_str(), &t);
  }
  result = &carr[0];
  return result;
}

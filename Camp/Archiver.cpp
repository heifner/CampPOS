#include "Archiver.h"
#include "Camp.h"
#include "Camper.h"
#include "sdsqlite.h"
#include <sstream>
#include <stdexcept>

Archiver::Archiver()
: db_(new sd::sqlite())
{
}

Archiver::~Archiver()
{
  delete db_;
}

void
Archiver::open(const std::string& fileName)
{
  db_->open(fileName);
  *db_ << "create table if not exists Campers (Id integer primary key, FirstName text, LastName text, Amount real, FingerPrint blob)";
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
}

void 
Archiver::updateRow(long long id, float amount)
{
  std::stringstream ss;
  ss << "update Campers set Amount='" << amount << "' where Id =" << id;
  *db_ << ss.str();
}

void 
Archiver::deleteRow(long long id)
{
  std::stringstream ss;
  ss << "delete from Campers where Id =" << id;
  *db_ << ss.str();
}

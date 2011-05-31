#include "Archiver.h"
#include <sqlite3.h>
#include <stdexcept>

Archiver::Archiver()
: db_(NULL)
{
}

void
Archiver::open(const std::string& fileName)
{
  //int rc = sqlite3_open(fileName.c_str(), &db_);
  //if (rc != SQLITE_OK) {
  //  std::string err = "Can't open database: ";
  //  err += sqlite3_errmsg(db_);
  //  sqlite3_close(db_);
  //  throw std::runtime_error(err);
  //}
}


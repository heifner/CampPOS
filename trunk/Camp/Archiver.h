// Copyright 2011 Kevin Heifner.  All rights reserved.
#ifndef ARCHIVER_H
#define ARCHIVER_H

#include "refptr.h"
#include <fstream>
#include <string>

class Camper;
class Camp;
namespace sd {
  struct sqlite;
}

class Archiver
{
public:
  Archiver();
  ~Archiver();

  /// Open the database and transaction log.
  /// @param dbFileName the path and name of database.
  /// @param transFileName the path and name of the transaction log.
  /// @throw std::exception if unable to open the files.
  void open(const std::string& dbFileName, const std::string& transFileName);

  /// Close the database.
  void close();

  /// @param camp is filled with contents of database.
  void load(Camp& camp);

  /// Add Camper to the database.
  void insertRow(const Camper& camper, size_t blobSize, const void* blob);

  /// Update the Camper's amount.
  void updateRow(long long id, float amount);

  /// Remove the Camper from the database.
  void deleteRow(long long id);

  /// Example: fmt="%y-%m-%d-%X" = 11-06-21-14:55:02
  /// @param fmt A strftime format string.
  /// @return the current date time string for fmt or empty string if error.
  static std::string getDateTimeString(const std::string& fmt);

private:

  /// Retrieve camper from the database.
  /// @return null if unable to find.
  ref_ptr<Camper> getCamper(long long id);

  /// Append the logEntry to the transaction log file.
  void logTransaction(const std::string& logEntry);

private:
  sd::sqlite* db_;
  std::ofstream transLog_;
};

#endif // ARCHIVER

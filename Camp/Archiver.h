// Copyright 2011 Kevin Heifner.  All rights reserved.
#ifndef ARCHIVER_H
#define ARCHIVER_H

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

  void open(const std::string& fileName);
  void close();

  void load(Camp& camp);

  void insertRow(const Camper& camper, size_t blobSize, const void* blob);

  void updateRow(long long id, float amount);

  void deleteRow(long long id);

private:
  sd::sqlite* db_;
};

#endif // ARCHIVER

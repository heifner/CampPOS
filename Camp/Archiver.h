#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <string>

struct sqlite3;

class Archiver
{
public:
  Archiver();

  void open(const std::string& fileName);

  void insertRow(
    long long id,
    const std::string& firstName,
    const std::string& lastName,
    float amount,
    size_t blobSize,
    void* blob);

  void updateRow(long long id, float amount);

  void deleteRow(long long id);

private:
  sqlite3* db_;
};

#endif // ARCHIVER

// Copyright 2011 Kevin Heifner.  All rights reserved.
#ifndef FPMANAGER_H
#define FPMANAGER_H
#pragma once

// Finger Print Hardware API
#include <bsapi.h>

#include <map>
#include <string>
#include <vector>

typedef long long FPKey;

/// Wrapper around BSAPI.
class FPManager {
public:
  static FPManager& instance();

  static const FPKey NOT_FOUND = -1;

  /// Used to get description of last error if any method returns false.
  std::string getLastError() const { return err_; }

  /// Connects with scanner hardware.
  /// @return true if successful, false if unsuccessful.
  bool open();

  /// Disconnects from scanner hardware.
  /// Frees all memory.
  bool close();

  /// Interacts with scanner hardware to create a new entry.
  /// @param key out param of key for new entry.
  bool createEntry(FPKey& key);

  /// Removes entry.
  /// @return false if unable to find.
  bool deleteEntry(FPKey key);
  void deleteAllEntries();

  /// Interacts with scanner hardware to find an existing entry.
  /// @return false if unable to find.
  bool findEntry(FPKey& key) const;

  /// Cancel any current ABS operation.
  /// Can/should be called from different thread.
  void cancel() const;

  /// Used to append to current error message, not intented for general use.
  void appendError(const std::string& err) { err_ += "\n"; err_ += err; }

  /// @return entry for storage
  std::pair<size_t,void*> getEntry(FPKey key) const;
  /// Create entry from storage
  /// @return true if key already exists and is replaced
  bool createEntry(FPKey key, const void* entryAsVoid);

private:
  /// Initialize ABS API
  FPManager();

private:
  typedef std::vector<ABS_BIR*> RawEntries;
  typedef std::map<ABS_BIR*, FPKey> EntryToFPKeyMap;

  FPKey nextKey_;
  ABS_CONNECTION absConnection_;
  mutable std::string err_;
  RawEntries rawEntries_;
  EntryToFPKeyMap entries_;
};

#endif // FPMANAGER_H
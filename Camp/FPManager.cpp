// Copyright 2011 Kevin Heifner.  All rights reserved.
#include "FPManager.h"
// System
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

// Used to supress compiler warnings
#ifndef UNREFERENCED_PARAMETER
    #define UNREFERENCED_PARAMETER(param)   (void)param
#endif

namespace {

  template <class T>
  std::string toString(const T& v) {
    stringstream ss;
    ss << v;
    return ss.str();
  }

  // Writes info about last error.
  void status_info(ABS_STATUS status, std::string& err)
  {
    if (status == ABS_STATUS_OK) return;

    // ABSGetLastErrorInfo() provides some diagnostical informations
    // about the last BSAPI error which occured in the current thread.
    ABS_DWORD code = 0;
    const ABS_CHAR* message = NULL;
    ABSGetLastErrorInfo(&code, &message);
    err += "  status  : " + toString(status) + "\n";
    err += "  code    : " + toString(code) + "\n";
    err += "  message : " + std::string(message) + "\n";
  }

  // Open BSAPI session.
  bool bsapi_open(ABS_CONNECTION& conn, std::string& err)
  {
    // Check whether it's not already open.
    if (conn != 0) {
      err += "The session is already open.\n";
      return false;
    }
    
    cout << "Opening a session..." << endl;

    // Initialize
    ABS_STATUS res = ABSInitialize();
    if (res != ABS_STATUS_OK) {
      err += "ABSInitilize() failed\n";
      status_info(res, err);
      return false;
    }

    // Enumerate all supported USB devices and decide which one of them to use.
    ABS_DEVICE_LIST* dev_list = NULL;
    res = ABSEnumerateDevices("usb", &dev_list);
    if (res != ABS_STATUS_OK) {
      err += "ABSEnumerateDevices() failed\n";
      status_info(res, err);
      return false;
    }
    if (dev_list->NumDevices == 0) {
      err += "No fingerprint device found.\n";
      ABSFree(dev_list);
      return false;
    }
    if (dev_list->NumDevices > 1) {
      // There are more then one devices connected.
      // For now we will just use the first device.
      cout << "Found more than one device:" << endl;
      for (size_t i = 0; i < dev_list->NumDevices; ++i) {
        cout << "[" << i << "]: " << dev_list->List[i].DsnSubString << endl;
      }
      cout << "Using device [0]" << endl;
    }
    int dev_index = 0;

    // Open device.
    cout << "Opening device: " << dev_list->List[dev_index].DsnSubString << endl;
    res = ABSOpen(dev_list->List[dev_index].DsnSubString, &conn);
    if (res != ABS_STATUS_OK) {
      ABSFree(dev_list);
      err += "ABSOpen() failed.\n";
      status_info(res, err);
      return false;
    }

    // Release memory allocated for the device list.
    ABSFree(dev_list);
    return true;
  }

  // Close BSAPI session
  bool bsapi_close(ABS_CONNECTION& conn, std::string& err)
  {
    cout << "Closing the current session..." << endl;

    // close the connection
    ABS_STATUS res = ABSClose(conn);
    if (res != ABS_STATUS_OK) {
      err += "ABSClose() failed.\n";
      status_info(res, err);
      return false;
    }
    conn = 0;

    // Free any resources allocated in ABSInitialize.
    res = ABSTerminate();
    if (res != ABS_STATUS_OK) {
      err += "ABSTerminate() failed.\n";
      status_info(res, err);
      return false;
    }

    return true;
  }

  // Callback is the primary way how the interactive biometric operations 
  // can interract with the user. Each BSAPI function which expects user 
  // to interact with the fingerprint sensor, takes poitner to structure
  // ABS_OPERATION as a parameter. One of its members is poitner to a 
  // function.
  //
  // BSAPI uses the function as a communication channel between the biometry
  // logic and user. 
  //
  // The callback is the way how to bind the biometry to GUI in your 
  // applications.
  void BSAPI callback(const ABS_OPERATION* p_operation, ABS_DWORD msg, void* data) 
  {
    UNREFERENCED_PARAMETER(p_operation);

    switch(msg) {
      // These messages just inform us how the interactive operation
      // progresses. Typical applications do not need it. */
    case ABS_MSG_PROCESS_BEGIN:
    case ABS_MSG_PROCESS_END:
      break;

      /* On some platforms, the biometric operastion can be suspended
      * when other process acquires sensor for other operation. */
    case ABS_MSG_PROCESS_SUSPEND:
      FPManager::instance().appendError("  operation has been suspended\n");
      printf("   operation has been suspended\n");
      break;
    case ABS_MSG_PROCESS_RESUME:
      printf("   operation has been resumed\n");
      break;

      /* Sometimes some info how the operation progresses is sent. */
    case ABS_MSG_PROCESS_PROGRESS:
      {
        ABS_PROCESS_PROGRESS_DATA* progress_data = 
          (ABS_PROCESS_PROGRESS_DATA*) data;
        if(progress_data->Percentage <= 100) {
          printf("   operation in progress (%d%%)...\n", 
            (int)progress_data->Percentage);
        } else {
          printf("   operation in progress...\n");
        }
        break;
      }
    case ABS_MSG_PROCESS_SUCCESS:
      printf("   success\n");
      break;
    case ABS_MSG_PROCESS_FAILURE:
      FPManager::instance().appendError("  failure\n");
      printf("   failure\n");
      break;

      /* Prompt messages should inform the user that he should do 
      * something. */
    case ABS_MSG_PROMPT_SCAN:
      printf("   swipe the finger\n"); 
      break;
    case ABS_MSG_PROMPT_TOUCH:
      printf("   touch the sensor\n");
      break;
    case ABS_MSG_PROMPT_KEEP:
      printf("   keep finger on the sensor\n"); 
      break;
    case ABS_MSG_PROMPT_LIFT:
      printf("   lift your finger away from the sensor\n");
      break;
    case ABS_MSG_PROMPT_CLEAN:
      printf("   clean the sensor\n"); 
      break;

      /* Quality messages come if something went wrong. E.g. the user
      * did not scan his finger in the right way. */
    case ABS_MSG_QUALITY_CENTER_HARDER:
      printf("   bad quality: center and harder\n"); 
      break;
    case ABS_MSG_QUALITY_CENTER:
      printf("   bad quality: center\n"); 
      break;
    case ABS_MSG_QUALITY_TOO_LEFT:
      printf("   bad quality: too left\n"); 
      break;
    case ABS_MSG_QUALITY_TOO_RIGHT:
      printf("   bad quality: too right\n"); 
      break;
    case ABS_MSG_QUALITY_HARDER:
      printf("   bad quality: harder\n"); 
      break;
    case ABS_MSG_QUALITY_TOO_LIGHT:
      printf("   bad quality: too light\n"); 
      break;
    case ABS_MSG_QUALITY_TOO_DRY:
      printf("   bad quality: too dry\n");
      break;
    case ABS_MSG_QUALITY_TOO_SMALL:
      printf("   bad quality: too small\n");
      break;
    case ABS_MSG_QUALITY_TOO_SHORT:
      printf("   bad quality: too short\n"); 
      break;
    case ABS_MSG_QUALITY_TOO_HIGH:
      printf("   bad quality: too high\n"); 
      break;
    case ABS_MSG_QUALITY_TOO_LOW:
      printf("   bad quality: too low\n"); 
      break;
    case ABS_MSG_QUALITY_TOO_FAST:
      printf("   bad quality: too fast\n"); 
      break;
    case ABS_MSG_QUALITY_TOO_SKEWED:
      printf("   bad quality: too skewed\n"); 
      break;
    case ABS_MSG_QUALITY_TOO_DARK:
      printf("   bad quality: too dark\n"); 
      break;
    case ABS_MSG_QUALITY_BACKWARD:
      printf("   bad quality: backward movement detected\n"); 
      break;
    case ABS_MSG_QUALITY_JOINT:
      printf("   bad quality: joint detected\n"); 
      break;

      /* Navigation messages are sent only from ABSNavigate. Its not used
      * in this sample but we list the messages here for completeness. */
    case ABS_MSG_NAVIGATE_CHANGE:
    case ABS_MSG_NAVIGATE_CLICK:
      break;

      /* Real application would probably use some GUI to provide feedback
      * for user. On these messages the GUI dialog should be made vsiible
      * and invisible respectivelly. */
    case ABS_MSG_DLG_SHOW:
    case ABS_MSG_DLG_HIDE:
      break;

      /* Idle message can come only if flag ABS_OPERATION_FLAG_USE_IDLE
      * was specified in ABS_OPERATION::dwFlags (i.e. never in this sample).
      * If the flag is specified, this message comes very often, hence 
      * giving the callback a chance to cancel the operation with 
      * ABSCancelOperation() without long time delays. In multithreaded 
      * applications, canceling the operation from another thread can be
      * better alternative. Consult BSAPI documentation for more info about
      * the topic. */
    case ABS_MSG_IDLE:
      break;
    }
  }

  // Pointer to ABS_OPERATION is taken as a parameter by those BSAPI 
  // funtions which work as interactive operation. The main purpose of it is 
  // to pass pointer to the callback function into the interactive functions. 
  //
  // It also allows to specifie some flags and/or timeout.
  //
  // In this sample we reuse this one operation instance. In real complex 
  // application you may need to use special ABS_OPERATION instance for 
  // each BSAPI interactive operation. This allows you to use specialized
  // callback for them, past various flags etc.
  ABS_OPERATION abs_op = { 
    // ID of the operation. We don't need to identify the operation in this 
    // application. When non-zero, the ID identifies the operation and allows it
    // to be canceled from any other thread with ABSCancelOperation().
    0,         

    // Arbitrary pointer, which allows application to pass any data into
    // the callback.
    NULL,      

    // Pointer to a simple callback implementation function.
    callback,  

    // Timeout. For example, lets set timeout to 60 sec. Note the value does 
    // not limit how long the operation (e.g. ABSVerify()) can take. The 
    // timeout only specifies time the operation waits for user to put his 
    // finger on a sensor. Zero would mean no timeout (i.e. the operation can 
    // never end if user never puts his finger on the sensor.)
    60000,

    // By default BSAPI places short time delays between sending some important
    // callback messages. The purpose of this is to guarantee that if multiple
    // messages come very closely in sequence, then the user still has enough
    // time to see all the messages and not just the lat one of the fast
    // sequence.
    //
    // For application developer, this simplifies callback implementation
    // which in most cases can be just showing an appropriate message in a 
    // window or dialog.
    //
    // However the time delays are not needed when user can see all history
    // of the messages, e.g. (in this command line sample application) the 
    // messages are outputted to standard output stream. Hence we would disable 
    // the time delays with with the flag ABS_OPERATION_FLAG_LL_CALLBACK here.
    ////ABS_OPERATION_FLAG_LL_CALLBACK
    0
  };


  // Add (enroll) new template into the template set.
  bool bsapi_add(ABS_CONNECTION conn, ABS_BIR*& newEntry, std::string& err)
  {
    // Enroll the tamplate
    ABS_BIR* enrolledTemplate = NULL;
    ABS_STATUS res = ABSEnroll(conn, &abs_op, &enrolledTemplate, 0);
    if (res != ABS_STATUS_OK) {
      err += "ABSEnroll() failed.\n";
      status_info(res, err);
      return false;
    }

    newEntry = reinterpret_cast<ABS_BIR*>(new char[enrolledTemplate->Header.Length]);
    memcpy(newEntry, enrolledTemplate, enrolledTemplate->Header.Length);
    ABSFree(enrolledTemplate);

    return true;
  }

} // anonymous namespace

FPManager&
FPManager::instance()
{
  static FPManager theOne;
  return theOne;
}

FPManager::FPManager()
: nextKey_(1)
, absConnection_(0)
, err_()
, rawEntries_()
, entries_()
{
}

bool
FPManager::open()
{
  err_.clear();
  return bsapi_open(absConnection_, err_);
}

bool
FPManager::close()
{
  err_.clear();
  deleteAllEntries();
  return bsapi_close(absConnection_, err_);
}

bool
FPManager::createEntry(FPKey& key)
{
  err_.clear();
  ABS_BIR* newEntry = NULL;
  if (!bsapi_add(absConnection_, newEntry, err_)) {
    return false;
  }

  key = nextKey_;
  ++nextKey_;

  rawEntries_.push_back(newEntry);
  entries_[newEntry] = key;

  return true;
}

bool 
FPManager::deleteEntry(FPKey key)
{
  err_.clear();
  EntryToFPKeyMap::iterator it = entries_.begin(), end = entries_.end();
  for (; it != end; ++it) {
    if (it->second == key) break;
  }
  if (it == entries_.end()) {
    err_ += "Unable to find entry for key: ";
    err_ += toString(key) + "\n";
    return false;
  }
  ABS_BIR* entry = it->first;
  RawEntries::iterator i = std::find(rawEntries_.begin(), rawEntries_.end(), entry);
  if (i == rawEntries_.end()) {
    err_ += "Unable to find raw entry for key: ";
    err_ += toString(key) + "\n";
    return false;
  }
  rawEntries_.erase(i);
  entries_.erase(it);

  delete[] reinterpret_cast<char*>(entry);
  return true;
}

void
FPManager::deleteAllEntries()
{
  for (RawEntries::iterator i = rawEntries_.begin(), end = rawEntries_.end(); i != end; ++i) {
    delete[] reinterpret_cast<char*>(*i);
  }
  rawEntries_.clear();
  entries_.clear();
  nextKey_ = 1;
}

bool 
FPManager::findEntry(FPKey& key) const
{
  err_.clear();
  if (rawEntries_.empty()) {
    err_ += "Unable to find entry, none registered.\n";
    return false;
  }

  // Function ABSVerify accepts an arbitrary count of templates to verify
  // the scanned finger against. It expects pointers to the templates in an
  // array of pointers to ABS_BIR structures. This matches our rawEntries_ 
  // vector representation.
  //
  // Now we can compare the finger against all the templates in the temporary array.
  ABS_LONG index = 0;
  ABS_BIR** templateArray = const_cast<ABS_BIR**>(&rawEntries_[0]);
  ABS_STATUS res = ABSVerify(absConnection_, &abs_op, rawEntries_.size(), templateArray, &index, 0);
  if (res != ABS_STATUS_OK) {
    err_ += "ABSVerify() failed\n";
    status_info(res, err_);
    return false;
  }

  ABS_BIR* entry = rawEntries_[index];
  EntryToFPKeyMap::const_iterator i = entries_.find(entry);
  if (i == entries_.end()) {
    err_ += "Unable to find key for entry.\n";
    return false;
  }

  key = i->second;
  return true;
}

std::pair<size_t,void*> 
FPManager::getEntry(FPKey key) const
{
  err_.clear();
  std::pair<size_t,void*> entry(0, NULL);

  EntryToFPKeyMap::const_iterator it = entries_.begin(), end = entries_.end();
  for (; it != end; ++it) {
    if (it->second == key) break;
  }
  if (it == entries_.end()) {
    err_ += "Unable to find entry for key: ";
    err_ += toString(key) + "\n";
    return entry;
  }
  entry.first = it->first->Header.Length;
  entry.second = it->first;
  return entry;
}

bool
FPManager::createEntry(FPKey key, const void* entryAsVoid)
{
  // remove entry if already exists, ignore error
  bool existed = deleteEntry(key);
  err_.clear();

  const ABS_BIR* entry = reinterpret_cast<const ABS_BIR*>(entryAsVoid);

  char* newEntry = new char[entry->Header.Length];
  memcpy(newEntry, entry, entry->Header.Length);

  ABS_BIR* newTemplate = reinterpret_cast<ABS_BIR*>(newEntry);

  nextKey_ = key >= nextKey_ ? key + 1 : nextKey_;

  rawEntries_.push_back(newTemplate);
  entries_[newTemplate] = key;

  return existed;
}


#include "Config.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

using namespace std;

namespace {
  std::string trim(const std::string& str, const std::string& whitespace = " \t") {
    const size_t beginStr = str.find_first_not_of(whitespace);
    if (beginStr == std::string::npos) {
      // no content
      return "";
    }

    const size_t endStr = str.find_last_not_of(whitespace);
    const size_t range = endStr - beginStr + 1;

    return str.substr(beginStr, range);
  }

  bool isInt(const std::string& s, int& number) {
    if (s.empty()) {
      return false;
    }
    istringstream ss(s);
    return ((ss >> number) && (ss >> std::ws).eof());
  }

  bool isLongLong(const std::string& s, long long& number) {
    if (s.empty()) {
      return false;
    }
    istringstream ss(s);
    return ((ss >> number) && (ss >> std::ws).eof());
  }
}

bool 
Config::loadConfigFile(const std::string& configFileName) {
  
  std::ifstream in(configFileName.c_str());
  
  if (! in.is_open()) {
    // All config parameters automatically get default values, so it's not 
    // really an error for the config file not to exist. 
    return false;
  }

  fileName_ = configFileName;
  
  std::string pkey = "";
  std::string st;
  
  while (! in.eof()) {
    std::getline(in, st);

    st = trim(st);
    
    if (! st.empty() && st[0] != ';') {
      
      unsigned int n = 0;
      if (st[0] == '[') {     // it's a parent key
        
        n = st.find(']');
        if (n == std::string::npos) {
          pkey = ""; // bad line. skip to next
        } else {
          pkey = st.substr(1, n - 1);
          sections_.insert(pkey);
        }
      } else {
        n = st.find('=');
        if (n == std::string::npos) {   // bad line. skip to next.
        } else {
          std::string key = st.substr(0, n);
          key = trim(key);
          std::string val = st.substr(n + 1);
          val = trim(val);
          if (! pkey.empty()) {
            key = pkey + "/" + key;
          }
          cmap_[key] = val;
        }
      }
    } // else this is a blank line or comment
  }
  return true;
}

Config::Config() {
}

Config::Config(const std::string& configFileName) 
  : fileName_()
{
  // Don't set fileName_ in initialization list because we
  // only save it if we are able to open the file.
  loadConfigFile(configFileName);
}

const std::string& 
Config::getFileName() const {
  return fileName_;
}

const Config::StringSet& 
Config::getSections() const {
  return sections_;
}

std::string 
Config::getStringSetting(const std::string& name, const std::string& def) const {
  ConfigMap::const_iterator i = cmap_.find(name); 
  if (i != cmap_.end()) {
    return i->second;
  }
  return def;
}

int 
Config::getIntSetting(const std::string& name, int def) const {
  std::string tmp = getStringSetting(name);
  if (tmp.empty()) {
    return def;
  }
  int ret;
  if (isInt(tmp, ret)) {
    return ret;
  }
  return def;
}

bool 
Config::getBoolSetting(const std::string& name, bool def) const {
  std::string tmp = getStringSetting(name);
  if (tmp.empty()) {
    return def;
  }
  StrCaseLess cmp;
  if (cmp(tmp, "true") || cmp(tmp, "t") || cmp(tmp, "yes") || cmp(tmp, "y")) {
    return true;
  }
  int n;
  if (isInt(tmp, n)) {
    return n != 0;
  }
  return false;
}

long long
Config::getLongLongSetting(const std::string& name, long long def) const {
  std::string tmp = getStringSetting(name);
  if (tmp.empty()) {
    return def;
  }
  long long ret;
  if (isLongLong(tmp, ret)) {
    return ret;
  }
  return def;
}

std::string 
Config::getString(const std::string& name, const std::string& def) {
  return Config::instance().getStringSetting(name, def);
}

int 
Config::getInt(const std::string& name, int def) {
  return Config::instance().getIntSetting(name, def);
}

bool 
Config::getBool(const std::string& name, bool def) {
  return Config::instance().getBoolSetting(name, def);
}

long long
Config::getLongLong(const std::string& name, long long def) {
  return Config::instance().getLongLongSetting(name, def);
}

void 
Config::setStringSetting(const std::string& key, const std::string& value) {
  cmap_[key] = value;
}

Config& 
Config::instance() {
  static Config inst;
  return inst;
}

bool 
Config::init(const std::string& configFileName) {
  return Config::instance().loadConfigFile(configFileName);
}

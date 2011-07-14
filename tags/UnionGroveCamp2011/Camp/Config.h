#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <set>
#include <string>

/// A very simple configuration class.
///
/// Typically you will access the singleton config object using
/// the provided simple static methods.
/// examples:
/// bool f = Config::getBool("enabled");
/// int n = Config::getInt("Server/Connection/Timeout", 5)
///
class Config {
public:

  struct StrCaseLess {
    bool operator()(const std::string& lhs, const std::string& rhs) const {
      return stricmp(lhs.c_str(), rhs.c_str()) < 0;
    }
  };

  typedef std::set<std::string, StrCaseLess> StringSet;
  typedef std::map<std::string, std::string, StrCaseLess> ConfigMap;

  Config();
  Config(const std::string& configFileName);

  const std::string& getFileName() const;

  const StringSet& getSections() const;
  
  std::string getStringSetting(const std::string& name, const std::string& def = "") const;
  int getIntSetting(const std::string& name, int def = 0) const;
  bool getBoolSetting(const std::string& name, bool def = false) const;
  long long getLongLongSetting(const std::string& name, long long def = 0) const;
  double getDoubleSetting(const std::string& name, double def = 0.0) const;
  
  static bool init(const std::string& configFileName);

  static std::string getString(const std::string& name, const std::string& def = "");
  static int getInt(const std::string& name, int def = 0);
  static bool getBool(const std::string& name, bool def = false);
  static long long getLongLong(const std::string& name, long long def = 0);
  static double getDouble(const std::string& name, double def = 0.0);

  void setStringSetting(const std::string& name, const std::string& def);

  /// Add list of values to a name
  template<class Iter>
  void setStringSettings(const std::string& name, Iter begin, Iter end) {
    std::string values;
    for (; begin != end; ++begin) {
      if (!values.empty()) {
        values.append(',');
      }
      values.append(std::string(*begin));
    }
    setStringSetting(name, values);
  }

  static Config& instance();
  
private:
  Config(const Config&); // disable
  Config& operator=(const Config&); // disable

  bool loadConfigFile(const std::string& configFileName);

private:
  std::string fileName_;
  StringSet sections_;
  ConfigMap cmap_;
};

#endif

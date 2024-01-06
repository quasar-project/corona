module;

#include <string>

export module launcher.projectinfo;

using namespace std;

namespace launcher
{
  export struct ProjectInfo
  {
    string name;
    string version;
    string organization;
    string homepage;
  };
}
#pragma once

#include <qcoreapplication.h>
#include <floppy/floppy.h>
#include <floppy/traits.h>

namespace corona::standalone::app
{
  enum class QuickStyle : char
  {
    Default,
    Material,
    Universal,
    Fusion
  };

  template <std::derived_from<QCoreApplication> T>
  class IApplication : public T, // NOLINT(*-special-member-functions)
                       public fl::pin
  {
   public:
    explicit IApplication(int& argc, char** argv)
      : T(argc, argv)
    {}

    virtual ~IApplication() = default;
  };
}

#include <iostream>
#include <qguiapplication.h>
#include <qqmlapplicationengine.h>

using std::nullopt;
using namespace std::chrono_literals;

auto main(int argc, char** argv) -> int
{
  auto app = QGuiApplication(argc, argv);
  auto engine = QQmlApplicationEngine("qrc:/dev/Main.qml");
  return QGuiApplication::exec();
}
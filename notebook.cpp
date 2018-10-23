#include <QApplication>
#include <QWidget>

#include "notebook_app.hpp"
int
main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  // QWidget widget;
  NotebookApp widget;
  widget.setWindowTitle(QApplication::translate("toplevel", "Plotscript"));
  widget.show();

  return app.exec();
}

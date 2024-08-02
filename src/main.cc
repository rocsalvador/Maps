#include <QApplication>
#include "Form.hh"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QSurfaceFormat f;
    f.setVersion(3, 3);
    f.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(f);

    Form myf;
    myf.show();

    return a.exec();
}

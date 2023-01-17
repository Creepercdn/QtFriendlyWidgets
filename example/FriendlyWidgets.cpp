#include <QApplication>
#include <QMainWindow>
#include <QLayout>
#include <QDial>
#include <QSlider>
#include "QtFriendlyWidgets/qadvancedslider.hpp"
#include "QtFriendlyWidgets/qadvanceddial.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QMainWindow  wnd;
    auto*  layout = new QHBoxLayout;
    auto* center = new QWidget;
    layout->addWidget(new QAdvancedDial);
    layout->addWidget(new QAdvancedSlider(Qt::Vertical));
    center->setLayout(layout);
    wnd.setCentralWidget(center);
    wnd.show();
    return app.exec();
}

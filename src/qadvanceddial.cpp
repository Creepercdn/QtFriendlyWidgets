#include "qadvanceddial.hpp"
#include "private/qadvancedslidercommon_p.hpp"
#include <QMouseEvent>
#include <utility>
#include <cmath>
#include <numbers>

class QAdvancedDialPrivate {
public:
    std::unique_ptr<QAdvancedSliderMenu>                     menu        = std::make_unique<QAdvancedSliderMenu>();
    QAdvancedDial::ValueMapFunction        valmapfunc  = [](int val) { return static_cast<double>(val); };
    QAdvancedDial::ReverseValueMapFunction rvalmapfunc = [](double val) { return static_cast<int>(val); };
    int                                      defaultVal{};
    QPoint                                   moveBegin{};
    int                                      moveBeginVal{};
    std::unique_ptr<QAdvancedSliderValueBox> valueBox = std::make_unique<QAdvancedSliderValueBox>();
};

QAdvancedDial::QAdvancedDial(QWidget* parent)
    : QDial(parent)
    , d_ptr(new QAdvancedDialPrivate)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);
    connect(d_ptr->menu.get(), &QAdvancedSliderMenu::changeValue, this, [&](double val) {
        setValue(d_ptr->rvalmapfunc(val));
    });
    connect(this, &QAdvancedDial::valueChanged, this, [&](int val) {
        d_ptr->valueBox->setValue(d_ptr->valmapfunc(val));
    });
}

QAdvancedDial::~QAdvancedDial()
{
    delete d_ptr;
}

void QAdvancedDial::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton) {
        setSliderDown(false);
        d_ptr->menu->openEdit(event->globalPosition().toPoint(), d_ptr->valmapfunc(value()), d_ptr->valmapfunc(minimum()), d_ptr->valmapfunc(maximum()));
        event->accept();
        return;
    }
    d_ptr->moveBegin    = event->pos();
    d_ptr->moveBeginVal = value();
    if (event->button() == Qt::LeftButton) {
        d_ptr->valueBox->trigger(mapToGlobal(contentsRect().bottomRight()), d_ptr->valmapfunc(value()));
    }
    if (QAdvancedSliderMenu::mousemoveMode().type == QAdvancedSliderMenu::MousemoveMode::Axis) {
        setSliderDown(true);
        triggerAction(SliderMove);
    } else {
        QDial::mousePressEvent(event);
    }
}

void QAdvancedDial::mouseDoubleClickEvent(QMouseEvent* event)
{
    QDial::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        setValue(defaultValue());
        event->accept();
    }
}

void QAdvancedDial::setValueMapFunction(ValueMapFunction function)
{
    d_ptr->valmapfunc = std::move(function);
}

QAdvancedDial::ValueMapFunction QAdvancedDial::valueMapFunction() const
{
    return d_ptr->valmapfunc;
}

void QAdvancedDial::setReverseValueMapFunction(ReverseValueMapFunction function)
{
    d_ptr->rvalmapfunc = std::move(function);
}

QAdvancedDial::ReverseValueMapFunction QAdvancedDial::reverseValueMapFunction() const
{
    return d_ptr->rvalmapfunc;
}

int QAdvancedDial::defaultValue() const
{
    return d_ptr->defaultVal;
}

void QAdvancedDial::setDefaultValue(int val)
{
    d_ptr->defaultVal = val;
}

void QAdvancedDial::contextMenuEvent(QContextMenuEvent* event)
{
    value();
    d_ptr->menu->trigger(event->globalPos(), d_ptr->valmapfunc(value()), d_ptr->valmapfunc(d_ptr->defaultVal), d_ptr->valmapfunc(minimum()), d_ptr->valmapfunc(maximum()));
    event->accept();
}

void QAdvancedDial::mouseMoveEvent(QMouseEvent* event)
{
    if (QAdvancedSliderMenu::mousemoveMode().type == QAdvancedSliderMenu::MousemoveMode::Axis) {
        event->accept();
        double xsliderLength;
        double ysliderLength;
        xsliderLength = ysliderLength = std::numbers::pi * std::min(height(), width());
        xsliderLength                 = std::clamp(xsliderLength, 250 * devicePixelRatio(), screen()->geometry().width() / 2.0); // min 6.6 cm (250 dip)
        ysliderLength = std::clamp(ysliderLength, 250*devicePixelRatio(), screen()->geometry().height() / 2.0);
        double scale                 = static_cast<double>(maximum() - minimum()) / std::min(xsliderLength, ysliderLength);
        int    posOffest              = calcuateAxisMousemove(
            event->pos() - d_ptr->moveBegin,
            QAdvancedSliderMenu::mousemoveMode().xmode,
            QAdvancedSliderMenu::mousemoveMode().ymode,
            scale,
            scale
        );
        if (invertedAppearance()) {
            posOffest = -posOffest;
        }
        setSliderPosition(d_ptr->moveBeginVal + posOffest);
    } else {
        QDial::mouseMoveEvent(event);
    }
    d_ptr->valueBox->setValue(d_ptr->valmapfunc(value()));
    d_ptr->valueBox->resize(d_ptr->valueBox->sizeHint());
}

void QAdvancedDial::mouseReleaseEvent(QMouseEvent* event)
{
    // QDial::mouseReleaseEvent(event);
    event->accept();
    d_ptr->valueBox->end();
    setSliderDown(false);
}
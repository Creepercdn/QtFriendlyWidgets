#include "qadvancedslider.hpp"
#include "private/qadvancedslidercommon_p.hpp"
#include <QMouseEvent>
#include <utility>

class QAdvancedSliderPrivate {
public:
    std::unique_ptr<QAdvancedSliderMenu>                     menu        = std::make_unique<QAdvancedSliderMenu>();
    QAdvancedSlider::ValueMapFunction        valmapfunc  = [](int val) { return static_cast<double>(val); };
    QAdvancedSlider::ReverseValueMapFunction rvalmapfunc = [](double val) { return static_cast<int>(val); };
    int                                      defaultVal{};
    QPoint                                   moveBegin{};
    int                                      moveBeginVal{};
    std::unique_ptr<QAdvancedSliderValueBox> valueBox = std::make_unique<QAdvancedSliderValueBox>();
};

QAdvancedSlider::QAdvancedSlider(QWidget* parent)
    : QSlider(parent)
    , d_ptr(new QAdvancedSliderPrivate)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);
    connect(d_ptr->menu.get(), &QAdvancedSliderMenu::changeValue, this, [&](double val) {
        setValue(d_ptr->rvalmapfunc(val));
    });
    connect(this, &QAdvancedSlider::valueChanged, this, [&](int val) {
        d_ptr->valueBox->setValue(d_ptr->valmapfunc(val));
    });
}

QAdvancedSlider::QAdvancedSlider(Qt::Orientation orident, QWidget* parent)
    : QAdvancedSlider(parent)
{
    setOrientation(orident);
}

QAdvancedSlider::~QAdvancedSlider()
{
    delete d_ptr;
}

void QAdvancedSlider::mousePressEvent(QMouseEvent* event)
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
        QSlider::mousePressEvent(event);
    }
}

void QAdvancedSlider::mouseDoubleClickEvent(QMouseEvent* event)
{
    QSlider::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        setValue(defaultValue());
        event->accept();
    }
}

void QAdvancedSlider::setValueMapFunction(ValueMapFunction function)
{
    d_ptr->valmapfunc = std::move(function);
}

QAdvancedSlider::ValueMapFunction QAdvancedSlider::valueMapFunction() const
{
    return d_ptr->valmapfunc;
}

void QAdvancedSlider::setReverseValueMapFunction(ReverseValueMapFunction function)
{
    d_ptr->rvalmapfunc = std::move(function);
}

QAdvancedSlider::ReverseValueMapFunction QAdvancedSlider::reverseValueMapFunction() const
{
    return d_ptr->rvalmapfunc;
}

int QAdvancedSlider::defaultValue() const
{
    return d_ptr->defaultVal;
}

void QAdvancedSlider::setDefaultValue(int val)
{
    d_ptr->defaultVal = val;
}

void QAdvancedSlider::contextMenuEvent(QContextMenuEvent* event)
{
    value();
    d_ptr->menu->trigger(event->globalPos(), d_ptr->valmapfunc(value()), d_ptr->valmapfunc(d_ptr->defaultVal), d_ptr->valmapfunc(minimum()), d_ptr->valmapfunc(maximum()));
    event->accept();
}

void QAdvancedSlider::mouseMoveEvent(QMouseEvent* event)
{
    if (QAdvancedSliderMenu::mousemoveMode().type == QAdvancedSliderMenu::MousemoveMode::Axis) {
        event->accept();
        int sliderLength;
        if (orientation() == Qt::Horizontal) {
            sliderLength = size().width();
        } else {
            sliderLength = size().height();
        }
        double scale = static_cast<double>(maximum() - minimum()) / sliderLength;
        int    posOffest = calcuateAxisMousemove(
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
        QSlider::mouseMoveEvent(event);
    }
    d_ptr->valueBox->setValue(d_ptr->valmapfunc(value()));
    d_ptr->valueBox->resize(d_ptr->valueBox->sizeHint());
}

void QAdvancedSlider::mouseReleaseEvent(QMouseEvent* event)
{
    QSlider::mouseReleaseEvent(event);
    d_ptr->valueBox->end();
}

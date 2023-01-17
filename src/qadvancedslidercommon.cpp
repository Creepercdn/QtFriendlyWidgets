#include "private/qadvancedslidercommon_p.hpp"

#include <QMenu>
#include <QActionGroup>
#include <QAction>
#include <QCoreApplication>
#include <utility>
#include <QValidator>
#include <QLineEdit>
#include <QKeyEvent>

class QEscLineEdit : public QLineEdit {
protected:
    void keyPressEvent(QKeyEvent* event) override
    {
        if (event->key() == Qt::Key_Escape && event->modifiers() == Qt::NoModifier) {
            close();
            event->accept();
        }
        QLineEdit::keyPressEvent(event);
    }
};

QAdvancedSliderMenu::QAdvancedSliderMenu(QWidget* parent)
    : QMenu(parent)
{
    setup();
    setWindowModality(Qt::WindowModal);
}

QAdvancedSliderMenu::~QAdvancedSliderMenu()
{
    delete actionMax_value_2;
    delete actionMin_value_1;
    delete actionEdit_value;
    delete actionDefault_value_1;
    delete actionCurrent_value_1;
    delete actionMouse_Behavior;
}

void QAdvancedSliderMenu::setup()
{
    //! Mouse Behavior
    actionMouse_Behavior = new QAction();
    actionMouse_Behavior->setEnabled(false);
    m_mmmode = new QActionGroup(this);
    m_mmmode->setExclusive(true);
    actionAngular_Mousemove_Mode = new QAction(m_mmmode);
    actionAngular_Mousemove_Mode->setCheckable(true);
    // actionAngular_Mousemove_Mode->setChecked(true);
    actionAxis_Mousemove_Mode = new QAction(m_mmmode);
    actionAxis_Mousemove_Mode->setCheckable(true);
    //! Axis Mousemove Settings
    menuAxis_Mousemove_Mode = new QMenu(this);
    m_x                     = new QActionGroup(this);
    m_x->setExclusive(true);
    actionX_No_effects = new QAction(m_x);
    actionX_No_effects->setCheckable(true);
    menuAxis_Mousemove_Mode->addAction(actionX_No_effects);
    actionX_Val = new QAction(m_x);
    actionX_Val->setCheckable(true);
    // actionX_Val->setChecked(true);
    menuAxis_Mousemove_Mode->addAction(actionX_Val);
    actionX_Val_2 = new QAction(m_x);
    actionX_Val_2->setCheckable(true);
    menuAxis_Mousemove_Mode->addAction(actionX_Val_2);
    menuAxis_Mousemove_Mode->addSeparator();
    m_y = new QActionGroup(this);
    m_y->setExclusive(true);
    actionY_No_effects = new QAction(m_y);
    actionY_No_effects->setCheckable(true);
    menuAxis_Mousemove_Mode->addAction(actionY_No_effects);
    actionY_Val = new QAction(m_y);
    actionY_Val->setCheckable(true);
    // actionY_Val->setChecked(true);
    menuAxis_Mousemove_Mode->addAction(actionY_Val);
    actionY_Val_2 = new QAction(m_y);
    actionY_Val_2->setCheckable(true);
    menuAxis_Mousemove_Mode->addAction(actionY_Val_2);

    //! Edit value
    actionCurrent_value_1 = new QAction();
    actionCurrent_value_1->setEnabled(false);
    actionDefault_value_1 = new QAction();
    actionEdit_value      = new QAction();

    //! Max/Min value
    actionMin_value_1 = new QAction();
    actionMax_value_2 = new QAction();

    connect(m_mmmode, &QActionGroup::triggered, this, &QAdvancedSliderMenu::processMM);
    connect(m_x, &QActionGroup::triggered, this, &QAdvancedSliderMenu::processMM);
    connect(m_y, &QActionGroup::triggered, this, &QAdvancedSliderMenu::processMM);

    syncMousemove();
    setupActions();
    retranslate();
}

void QAdvancedSliderMenu::processMM()
{
    if (m_mmmode->checkedAction() == actionAngular_Mousemove_Mode) {
        m_mmmodedata.type = MousemoveMode::Angular;
    } else if (m_mmmode->checkedAction() == actionAxis_Mousemove_Mode) {
        m_mmmodedata.type = MousemoveMode::Axis;
    } else {
        qWarning("mousemove slot: Unknown mousemove mode");
    }
    if (m_x->checkedAction() == actionX_No_effects) {
        m_mmmodedata.xmode = MousemoveMode::NoEffect;
    } else if (m_x->checkedAction() == actionX_Val) {
        m_mmmodedata.xmode = MousemoveMode::Positive;
    } else if (m_x->checkedAction() == actionX_Val_2) {
        m_mmmodedata.xmode = MousemoveMode::Negative;
    } else {
        qWarning("mousemove slot: Unknown X mode");
    }
    if (m_y->checkedAction() == actionY_No_effects) {
        m_mmmodedata.ymode = MousemoveMode::NoEffect;
    } else if (m_y->checkedAction() == actionY_Val) {
        m_mmmodedata.ymode = MousemoveMode::Positive;
    } else if (m_y->checkedAction() == actionY_Val_2) {
        m_mmmodedata.ymode = MousemoveMode::Negative;
    } else {
        qWarning("mousemove slot: Unknown Y mode");
    }
}

void QAdvancedSliderMenu::setupActions()
{
    this->clear();
    for (QAction* action : m_actions) {
        this->addAction(action);
    }
    this->addSeparator();
    this->addAction(actionMouse_Behavior);
    this->addAction(actionAngular_Mousemove_Mode);
    this->addAction(actionAxis_Mousemove_Mode);
    this->addAction(menuAxis_Mousemove_Mode->menuAction());
    this->addSeparator();
    this->addAction(actionCurrent_value_1);
    this->addAction(actionDefault_value_1);
    this->addAction(actionEdit_value);
    this->addSeparator();
    this->addAction(actionMin_value_1);
    this->addAction(actionMax_value_2);
}

void QAdvancedSliderMenu::retranslate()
{
    actionMouse_Behavior->setText(tr("Mouse Behavior", nullptr));
    actionAngular_Mousemove_Mode->setText(tr("A&ngular Mousemove Mode", nullptr));
    actionX_Val->setText(tr("X+ -> Val+ (Positive ratio) (&R)", nullptr));
    actionX_Val_2->setText(tr("X+ -> Val- (Negative ratio) (&T)", nullptr));
    actionY_Val->setText(tr("Y+ -> Val+ (Positive ratio) (&F)", nullptr));
    actionY_Val_2->setText(tr("Y- -> Val+ (Negative ratio) (&G)", nullptr));
    actionX_No_effects->setText(tr("X No effects (&Y)", nullptr));
    actionY_No_effects->setText(tr("Y No effects (&H)", nullptr));
    actionAxis_Mousemove_Mode->setText(tr("A&xis Mousemove Mode", nullptr));
    actionEdit_value->setText(tr("&Edit value [MIDCLK]", nullptr));
    menuAxis_Mousemove_Mode->setTitle(tr("Axis Mousemove &Settings", nullptr));
}

void QAdvancedSliderMenu::updateText(double currentValue, double def, double min, double max)
{
    actionCurrent_value_1->setText(tr("Current value: %1", nullptr).arg(currentValue));
    actionDefault_value_1->setText(tr("&Default value: %1 [DBLCLK]", nullptr).arg(def));
    actionMin_value_1->setText(tr("M&in value: %1", nullptr).arg(min));
    actionMax_value_2->setText(tr("M&ax value: %1", nullptr).arg(max));
}

void QAdvancedSliderMenu::setExternalActions(QList<QAction*> actions)
{
    m_actions = std::move(actions);
}

QList<QAction*> QAdvancedSliderMenu::externalActions() const
{
    return m_actions;
}

void QAdvancedSliderMenu::trigger(QPoint globalPoint, double val, double def, double min, double max)
{
    syncMousemove();
    setupActions();
    updateText(val, def, min, max);
    auto editcon = connect(actionEdit_value, &QAction::triggered, this, [&] {
        openEdit(globalPoint, val, min, max);
    });
    auto defcon  = connect(actionDefault_value_1, &QAction::triggered, this, [&] {
        emit changeValue(def);
    });
    auto mincon  = connect(actionMin_value_1, &QAction::triggered, this, [&] {
        emit changeValue(min);
    });
    auto maxcon  = connect(actionMax_value_2, &QAction::triggered, this, [&] {
        emit changeValue(max);
    });
    exec(globalPoint);
    disconnect(editcon);
}

void QAdvancedSliderMenu::openEdit(QPoint globalPoint, double val, double min, double max)
{
    std::unique_ptr<QDoubleValidator> validator = std::make_unique<QDoubleValidator>(min, max, -1);
    QEscLineEdit                      edit;
    edit.move(globalPoint);
    edit.setWindowFlag(Qt::Popup);
    validator->setNotation(QDoubleValidator::StandardNotation);
    edit.setValidator(validator.get());
    edit.setWindowModality(Qt::ApplicationModal);
    edit.setText(QLocale().toString(val, 'f'));
    edit.show();
    edit.setFocus(Qt::PopupFocusReason);
    QEventLoop eventloop;
    connect(&edit, &QLineEdit::editingFinished, &eventloop, &QEventLoop::quit);
    eventloop.exec();
    emit changeValue(QLocale().toDouble(edit.text()));
}

QAdvancedSliderMenu::MousemoveMode QAdvancedSliderMenu::mousemoveMode()
{
    return m_mmmodedata;
}

void QAdvancedSliderMenu::syncMousemove()
{
    switch (m_mmmodedata.type) {
    case MousemoveMode::Angular:
        actionAngular_Mousemove_Mode->setChecked(true);
        break;
    case MousemoveMode::Axis:
        actionAxis_Mousemove_Mode->setChecked(true);
        break;
    }
    switch (m_mmmodedata.xmode) {
    case MousemoveMode::Positive:
        actionX_Val->setChecked(true);
        break;
    case MousemoveMode::Negative:
        actionX_Val_2->setChecked(true);
        break;
    case MousemoveMode::NoEffect:
        actionX_No_effects->setChecked(true);
        break;
    default:
        //? hicpp-multiway-paths-covered vs covered-switch-default
        break;
    }
    switch (m_mmmodedata.ymode) {
    case MousemoveMode::Positive:
        actionY_Val->setChecked(true);
        break;
    case MousemoveMode::Negative:
        actionY_Val_2->setChecked(true);
        break;
    case MousemoveMode::NoEffect:
        actionY_No_effects->setChecked(true);
        break;
    default:
        //? hicpp-multiway-paths-covered vs covered-switch-default
        break;
    }
}

int calcuateAxisMousemove(QPoint offest, unsigned char xmode, unsigned char ymode, double xscale, double yscale)
{
    double result = 0;
    result += (xmode == 2 ? -1 : xmode) * offest.x() * xscale;
    result += (ymode == 2 ? -1 : ymode) * offest.y() * yscale;
    return std::round(result);
}

QAdvancedSliderValueBox::QAdvancedSliderValueBox(QWidget* parent)
    : QLabel(parent)
{
    setWindowFlags(Qt::ToolTip);
    setWindowModality(Qt::NonModal);
}

void QAdvancedSliderValueBox::updateText(double val)
{
    setText(QLocale().toString(val, 'f'));
}

void QAdvancedSliderValueBox::trigger(QPoint globalPos, double value)
{
    move(globalPos);
    updateText(value);
    show();
}

void QAdvancedSliderValueBox::setValue(double value)
{
    updateText(value);
}

void QAdvancedSliderValueBox::end()
{
    hide();
}

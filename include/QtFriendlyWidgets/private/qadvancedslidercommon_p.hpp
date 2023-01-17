#pragma once

#include <QMenu>
#include <QLabel>

// xmode and ymode:
// 0 NoEffect
// 1 Positive
// 2 Negative
// return offest
int calcuateAxisMousemove(QPoint offest, unsigned char xmode, unsigned char ymode, double xscale, double yscale);

class QAdvancedSliderMenu : public QMenu {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(QAdvancedSliderMenu)
public:
    struct MousemoveMode {
        MousemoveMode()
            : type(Angular)
            , xmode(Positive)
            , ymode(Negative)
        {
        }

        enum Mode : unsigned char {
            Angular,
            Axis
        };

        Mode type : 1;

        enum AxisMode : unsigned char {
            NoEffect = 0,
            Positive = 1,
            Negative = 2
        };

        AxisMode xmode : 2;
        AxisMode ymode : 2;
    };

    explicit QAdvancedSliderMenu(QWidget* parent = nullptr);
    ~QAdvancedSliderMenu() override;
    void                               setup();
    void                               setupActions();
    void                               retranslate();
    void                               syncMousemove();
    void                               updateText(double currentValue, double def, double min, double max);
    [[nodiscard]] QList<QAction*>      externalActions() const;
    void                               setExternalActions(QList<QAction*> actions);
    [[nodiscard]] static MousemoveMode mousemoveMode();
public slots:
    void trigger(QPoint globalPoint, double val, double def, double min, double max);
    void openEdit(QPoint globalPoint, double val, double min, double max);
private slots:
    void processMM();
signals:
    void changeValue(double value);

private:
    QAction*                    actionMouse_Behavior{};
    QAction*                    actionAngular_Mousemove_Mode{};
    QAction*                    actionX_Val{};
    QAction*                    actionX_Val_2{};
    QAction*                    actionY_Val{};
    QAction*                    actionY_Val_2{};
    QAction*                    actionX_No_effects{};
    QAction*                    actionY_No_effects{};
    QAction*                    actionAxis_Mousemove_Mode{};
    QAction*                    actionCurrent_value_1{};
    QAction*                    actionDefault_value_1{};
    QAction*                    actionEdit_value{};
    QAction*                    actionMin_value_1{};
    QAction*                    actionMax_value_2{};
    QMenu*                      menuAxis_Mousemove_Mode{};
    QList<QAction*>             m_actions;  // external actions
    QActionGroup*               m_mmmode{}; // mousemove mode
    QActionGroup*               m_x{};
    QActionGroup*               m_y{};
    double                      m_current{};
    inline static MousemoveMode m_mmmodedata{};
};


class QAdvancedSliderValueBox : public QLabel {
    Q_OBJECT
public:
    explicit QAdvancedSliderValueBox(QWidget* parent = nullptr);
public slots:
    void trigger(QPoint globalPos, double value);
    void setValue(double value);
    void end();

private:
    void updateText(double val);
};

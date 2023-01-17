#pragma once

#include <QSlider>

class QAdvancedSliderPrivate;

class QAdvancedSlider : public QSlider {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(QAdvancedSlider)
    Q_PROPERTY(int defaultValue READ defaultValue WRITE setDefaultValue)
public:
    using ValueMapFunction        = std::function<double(int)>;
    using ReverseValueMapFunction = std::function<int(double)>;
    explicit QAdvancedSlider(QWidget* parent = nullptr);
    explicit QAdvancedSlider(Qt::Orientation orident, QWidget* parent = nullptr);
    ~QAdvancedSlider() override;
    [[nodiscard]] int                     defaultValue() const;
    void                                  setDefaultValue(int val);
    void                                  setValueMapFunction(ValueMapFunction function);
    [[nodiscard]] ValueMapFunction        valueMapFunction() const;
    void                                  setReverseValueMapFunction(ReverseValueMapFunction function);
    [[nodiscard]] ReverseValueMapFunction reverseValueMapFunction() const;

protected:
    void                    mousePressEvent(QMouseEvent* event) override; // middle click: edit value
    void                    contextMenuEvent(QContextMenuEvent* event) override; // advanced context menu
    void                    mouseDoubleClickEvent(QMouseEvent* event) override; // double click: default
    void                    mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    QAdvancedSliderPrivate* d_ptr;
};

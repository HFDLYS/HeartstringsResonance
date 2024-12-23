#ifndef SQUARECHART_H
#define SQUARECHART_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QRectF>

class SquarePieChart : public QWidget {
    Q_OBJECT

public:
    SquarePieChart(QWidget* parent = nullptr) : QWidget(parent) {
        values[0] = 0;
        values[1] = 0;
        values[2] = 0;
        values[3] = 0;
        values[4] = 0;
    }

    void setValues(int index, int value);
protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int values[5];
};

#endif // SQUARECHART_H
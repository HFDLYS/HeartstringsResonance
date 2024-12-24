#include "squarechart.h"

void SquarePieChart::setValues(int index, int value) {
    values[index] = value;
    update();
}

void SquarePieChart::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF squareRect(0, 0, width(), height());
    painter.setClipRect(squareRect);
    QVector<QColor> colors = {Qt::cyan, Qt::red, Qt::green, Qt::yellow};
    double side = 16 * qMax(width(), height());
    QRectF bigCircleRect(
        (width() - side) / 2.0,   // 居中一下
        (height() - side) / 2.0,
        side,
        side
    );
    double sum = 0;
    for (int i = 1; i <= 4; i++) {
        sum += values[i];
    }
    double startAngle = 0;
    for (int i = 1; i <= 4; i++) {
        double angle = values[i] / sum * 360;
        painter.setBrush(QBrush(colors[i - 1]));
        painter.drawPie(bigCircleRect, startAngle * 16, angle * 16);
        startAngle += angle;
    }
}
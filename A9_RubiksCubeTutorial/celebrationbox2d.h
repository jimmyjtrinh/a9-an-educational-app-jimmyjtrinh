/*
 * Jimmy Trinh && Jacob Day && Amitoj Singh && Michael Shin
 * Software Practice II, CS 3505
 * Fall 2021
 * A9: An Educational App
 * Video: Video: https://youtu.be/uIMg9xUNVgY
 */

#ifndef CELEBRATIONBOX2D_H
#define CELEBRATIONBOX2D_H
#include <QWidget>
#include <box2d/include/box2d/box2d.h>
#include <QTimer>
#include <QPainter>

class celebrationBox2D : public QWidget
{
    Q_OBJECT
public:
    explicit celebrationBox2D(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
signals:

public slots:
    void updateWorld();
    void startWorld();
    void clearWorld();

private:
    b2World world;
    b2Body* body;
    QTimer timer;
    QImage image;
};

#endif // CELEBRATIONBOX2D_H

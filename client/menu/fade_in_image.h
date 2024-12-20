#ifndef FADE_IN_IMAGE_H
#define FADE_IN_IMAGE_H

#include <QWidget>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class FadeInImage {
public:
    FadeInImage(QWidget *parent, const QString &imagePath, int duration);
    void start();
    void resize();  // Method to center the image when window is resized

private:
    std::shared_ptr<QLabel> fadeInLabel;
    std::shared_ptr<QGraphicsOpacityEffect> opacityEffect;
    std::shared_ptr<QPropertyAnimation> fadeInAnimation;
    QPixmap fadeInPixmap;
};

#endif // FADE_IN_IMAGE_H

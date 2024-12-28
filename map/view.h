// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef VIEW_H
#define VIEW_H

#include <QFrame>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE


class View : public QGraphicsView
{
    Q_OBJECT
public:
    View();
    void mouseMoveEvent(QMouseEvent *event);

};

#endif // VIEW_H

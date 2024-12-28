// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QGraphicsView>
#include "QGraph.h"
#include "vertex.h"
#include <string>
#include <QSlider>
#include <QMenuBar>
#include <QDockWidget>
#include <QLineEdit>
#include <QPushButton>
#include "QEdge.h"

using std::string;

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QSplitter;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:

    void setupMatrix();
    void populateScene();
    void chooseGraph();
    void drawVertices();
    void drawEdges();
    void zoomIn();
    void zoomOut();
    void highlightPathAstar();
    void highlightPathDijkstra();
    void highlightPathBFS();
    void reset();
    void zoomInBy(int level);
    void zoomOutBy(int level);

    QGraph *scene;
    QGraphicsView *view;
    QSplitter *h1Splitter;
    QSplitter *h2Splitter;
    int width_ = 1080;
    int height_ = 720;
    string graphname_;
    Graph graph_;

    QSlider *zoomSlider;
    QMenu *menu;
    QAction *menu_choose_action;
    QLineEdit *startLineEdit;
    QLineEdit *endLineEdit;
    vector<QEdge*> highlighted;
    vector<QVertex*> highlightedQVertices;
};

#endif

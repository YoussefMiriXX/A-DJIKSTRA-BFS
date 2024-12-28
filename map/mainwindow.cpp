// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "mainwindow.h"

#include "QVertex.h"
#include "vertex.h"
#include "edge.h"
#include "QGraph.h"

#include <ostream>
#include <iostream>

#include <QToolButton>
#include <QStyle>
#include <QSlider>
#include <QMenuBar>
#include <QDockWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QSplitter>
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QErrorMessage>
#include <QGraphicsView>
#include <QStatusBar>

using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), scene(new QGraph()), view(new QGraphicsView(this))
{
    this->resize(width_, height_);

    menu = new QMenu();
    menu = menuBar()->addMenu("File");
    menu_choose_action = new QAction("Choose a graph");
    menu->addAction(menu_choose_action);
    connect(menu_choose_action, &QAction::triggered, this, &MainWindow::chooseGraph);

    scene->setBackgroundBrush(Qt::black);
    view->resize(width_, height_);
    view->setScene(scene);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    view->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);

    setCentralWidget(new QWidget);


    setWindowTitle(tr("Youssef and Ahmed Graph"));
}

void MainWindow::populateScene()
{
    drawVertices();
    drawEdges();


    QHBoxLayout *algoLayout = new QHBoxLayout();

    QVBoxLayout *qvb = new QVBoxLayout();

    QLabel *fromLabel = new QLabel();
    QLabel *toLabel = new QLabel();
    QPushButton *bfsButton = new QPushButton();
    QPushButton *dijkstraButton = new QPushButton();
    QPushButton *astarButton = new QPushButton();
    QPushButton *resetButton = new QPushButton();


    bfsButton->setText("BFS");
    dijkstraButton->setText("Dijkstra");
    astarButton->setText("Astar");
    resetButton->setText("Reset");

    connect(bfsButton, &QPushButton::released, this, &MainWindow::highlightPathBFS);
    connect(dijkstraButton, &QPushButton::released, this, &MainWindow::highlightPathDijkstra);
    connect(astarButton, &QPushButton::released, this, &MainWindow::highlightPathAstar);
    connect(resetButton, &QPushButton::released, this, &MainWindow::reset);

    QHBoxLayout *zoomSliderLayout = new QHBoxLayout();
    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap("../images/zoomin_icon.png"));
    zoomInIcon->setIconSize(iconSize);

    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap("../images/zoomout_icon.png"));
    zoomOutIcon->setIconSize(iconSize);

    zoomSlider = new QSlider(Qt::Horizontal);
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    connect(zoomSlider, &QAbstractSlider::valueChanged, this, &MainWindow::setupMatrix);
    connect(zoomInIcon, &QAbstractButton::clicked, this, &MainWindow::zoomIn);
    connect(zoomOutIcon, &QAbstractButton::clicked, this, &MainWindow::zoomOut);
    zoomSliderLayout->addWidget(zoomOutIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomInIcon);

    fromLabel->setText("From : ");
    toLabel->setText("To : ");
    startLineEdit = new QLineEdit();
    endLineEdit = new QLineEdit();


    algoLayout->addWidget(fromLabel);
    algoLayout->addWidget(startLineEdit);
    algoLayout->addWidget(toLabel);
    algoLayout->addWidget(endLineEdit);
    algoLayout->addWidget(bfsButton);
    algoLayout->addWidget(dijkstraButton);
    algoLayout->addWidget(astarButton);
    algoLayout->addWidget(resetButton);

    QStatusBar *statusBar = new QStatusBar();
    statusBar->showMessage(QString::fromStdString(graphname_));
    setStatusBar(statusBar);



    qvb->addLayout(algoLayout);
    qvb->addWidget(view);
    qvb->addLayout(zoomSliderLayout);



    centralWidget()->setLayout(qvb);
}

void MainWindow::chooseGraph()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "./", "Text Files (*.txt)");
    graphname_ = filePath.toStdString();
    graph_ = Graph(graphname_);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->setSceneRect(-width_/2, -height_/2, width_, height_);
    scene->setGraph(graphname_);
    populateScene();

}

void MainWindow::drawVertices()
{
    map<unsigned int, Vertex>::iterator itVertex;
    map<unsigned int, Vertex> &m = graph_.getAllVertices();
    QColor color(255, 0, 0);
    for(itVertex = m.begin(); itVertex != m.end(); ++itVertex)
    {
        Vertex &vertex = itVertex->second;
        QVertex *qvertex = new QVertex(vertex, color);
        scene->addVertex(qvertex->getVertexCenter(), *qvertex);
    }
}

void MainWindow::drawEdges()
{
    map<unsigned int, Vertex> &m = graph_.getAllVertices();
    map<pair<unsigned int, unsigned int>, Edge>::iterator itEdge;
    map<pair<unsigned int, unsigned int>, Edge>&edges = graph_.getAllEdges();
    QColor color(255, 0, 0);
    for(itEdge = edges.begin(); itEdge != edges.end(); ++itEdge)

    {
        unsigned int startVertexId = itEdge->first.first;
        unsigned int endVertexId = itEdge->first.second;

        Vertex &startVertex = m.at(startVertexId);
        Vertex &endVertex = m.at(endVertexId);
        QVertex qStartVertex(startVertex, color);
        QVertex qEndVertex(endVertex, color);

        Edge &edge = itEdge->second;
        QEdge *qedge = new QEdge(edge,
                                 color,
                                 qStartVertex.getVertexCenter(),
                                 qEndVertex.getVertexCenter()
                                 );
        scene->addEdge(
                       *qedge);
    }
}

void MainWindow::zoomIn()
{
    zoomSlider->setValue(zoomSlider->value() + 1);
}

void MainWindow::zoomOut()
{
    zoomSlider->setValue(zoomSlider->value() - 1);
}

void MainWindow::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(125));

    QTransform matrix;
    matrix.scale(scale, scale);


    view->setTransform(matrix);

}

void MainWindow::highlightPathAstar()
{
    map<unsigned int, Vertex> &m = graph_.getAllVertices();
    unsigned int startVertexId = startLineEdit->text().toUInt();
    unsigned int endVertexId = endLineEdit->text().toUInt();
    Vertex startVertex;
    Vertex nextVertex;
    Vertex endVertex;
    vector<Vertex> astarVector;
    set<Vertex>everyVisitedVertex;

    try
    {
        startVertex = m.at(startVertexId);
    }
    catch(const std::out_of_range e)
    {
        QErrorMessage *startVertexErrorMessage = new QErrorMessage();
        startVertexErrorMessage->showMessage("Start vertex does not exist");
        return;
    }

    try
    {
        endVertex = m.at(endVertexId);
    }
    catch (const std::out_of_range e)
    {
        QErrorMessage *endVertexErrorMessage = new QErrorMessage();
        endVertexErrorMessage->showMessage("End vertex does not exist");
        return;
    }


    if(endVertex == startVertex)
    {
        QErrorMessage *sameVerticesErrorMessage = new QErrorMessage();
        sameVerticesErrorMessage->showMessage("Start vertex and End vertex are the same");
        return;
    }

    reset();

    pair<vector<Vertex>, set<Vertex>> p = graph_.astar(m.at(startVertexId), m.at(endVertexId));
    astarVector = p.first;
    everyVisitedVertex = p.second;


    QColor color(255, 0, 0);

    unsigned int childVertexId;
    unsigned int parentVertexId = startVertexId;

    QPen bluePen(Qt::blue);
    bluePen.setWidth(3);

    QPen orangePen(QColor(255, 165, 0));
    for(auto it = everyVisitedVertex.begin(); it != everyVisitedVertex.end(); ++it)
    {
        Vertex v = *it;
        QVertex *QVisitedVertex = new QVertex(v, color);
        QVisitedVertex->setPen(orangePen);
        scene->addVertex(QVisitedVertex->getVertexCenter(), *QVisitedVertex);
        highlightedQVertices.push_back(QVisitedVertex);
    }

    for (auto it = astarVector.rbegin(); it != astarVector.rend(); ++it)
    {
        childVertexId = it->getObjectId();
        if(parentVertexId == childVertexId) continue;

        Edge &edge = graph_.getAllEdges().at({parentVertexId, childVertexId});

        QVertex sqv(graph_.getAllVertices().at(parentVertexId), color);
        QVertex eqv(*it, color);

        QEdge *qedge = new QEdge(edge,
                                 color,
                                 sqv.getVertexCenter(),
                                 eqv.getVertexCenter()
                                 );
        highlighted.push_back(qedge);
        qedge->setPen(bluePen);
        scene->addEdge(*qedge);
        parentVertexId = childVertexId;
        scene->update();
    }
}

void MainWindow::highlightPathDijkstra()
{
    map<unsigned int, Vertex> &m = graph_.getAllVertices();
    unsigned int startVertexId = startLineEdit->text().toUInt();
    unsigned int endVertexId = endLineEdit->text().toUInt();
    Vertex startVertex;
    Vertex nextVertex;
    Vertex endVertex;
    vector<Vertex> dijkstraVector;
    set<Vertex>everyVisitedVertex;

    try
    {
        startVertex = m.at(startVertexId);
    }
    catch(const std::out_of_range e)
    {
        QErrorMessage *startVertexErrorMessage = new QErrorMessage();
        startVertexErrorMessage->showMessage("Start vertex does not exist");
        return;
    }

    try
    {
        endVertex = m.at(endVertexId);
    }
    catch (const std::out_of_range e)
    {
        QErrorMessage *endVertexErrorMessage = new QErrorMessage();
        endVertexErrorMessage->showMessage("End vertex does not exist");
        return;
    }

    if(endVertex == startVertex)
    {
        QErrorMessage *sameVerticesErrorMessage = new QErrorMessage();
        sameVerticesErrorMessage->showMessage("Start vertex and End vertex are the same");
        return;
    }

    reset();
    pair<vector<Vertex>, set<Vertex>> p = graph_.dijkstra(m.at(startVertexId), m.at(endVertexId));
    dijkstraVector = p.first;
    everyVisitedVertex = p.second;

    QColor color(255, 0, 0);

    unsigned int childVertexId;
    unsigned int parentVertexId = startVertexId;

    QPen orangePen(QColor(255, 165, 0));
    for(auto it = everyVisitedVertex.begin(); it != everyVisitedVertex.end(); ++it)
    {
        Vertex v = *it;
        QVertex *QVisitedVertex = new QVertex(v, color);
        QVisitedVertex->setPen(orangePen);
        scene->addVertex(QVisitedVertex->getVertexCenter(), *QVisitedVertex);
        highlightedQVertices.push_back(QVisitedVertex);
    }

    QPen bluePen(Qt::blue);
    bluePen.setWidth(3);



    for (auto it = dijkstraVector.rbegin(); it != dijkstraVector.rend(); ++it)
    {
        childVertexId = it->getObjectId();
        if(parentVertexId == childVertexId) continue;

        Edge &edge = graph_.getAllEdges().at({parentVertexId, childVertexId});

        QVertex sqv(graph_.getAllVertices().at(parentVertexId), color);
        QVertex eqv(*it, color);

        QEdge *qedge = new QEdge(edge,
                                 color,
                                 sqv.getVertexCenter(),
                                 eqv.getVertexCenter()
                                 );
        highlighted.push_back(qedge);
        qedge->setPen(bluePen);
        scene->addEdge(*qedge);
        parentVertexId = childVertexId;
        scene->update();

    }
}

void MainWindow::highlightPathBFS()
{

    map<unsigned int, Vertex> &m = graph_.getAllVertices();
    unsigned int startVertexId = startLineEdit->text().toUInt();
    unsigned int endVertexId = endLineEdit->text().toUInt();
    Vertex startVertex;
    Vertex nextVertex;
    Vertex endVertex;
    vector<Vertex> bfsVector;
    set<Vertex>everyVisitedVertex;

    try
    {
        startVertex = m.at(startVertexId);
    }
    catch(const std::out_of_range e)
    {
        QErrorMessage *startVertexErrorMessage = new QErrorMessage();
        startVertexErrorMessage->showMessage("Start vertex does not exist");
        return;
    }

    try
    {
        endVertex = m.at(endVertexId);
    }
    catch (const std::out_of_range e)
    {
        QErrorMessage *endVertexErrorMessage = new QErrorMessage();
        endVertexErrorMessage->showMessage("End vertex does not exist");
        return;
    }

    if(endVertex == startVertex)
    {
        QErrorMessage *sameVerticesErrorMessage = new QErrorMessage();
        sameVerticesErrorMessage->showMessage("Start vertex and End vertex are the same");
        return;
    }

    reset();

    pair<vector<Vertex>, set<Vertex>> p = graph_.bfs(m.at(startVertexId), m.at(endVertexId));
    bfsVector = p.first;
    everyVisitedVertex = p.second;

    QColor color(255, 0, 0);

    unsigned int childVertexId;
    unsigned int parentVertexId = startVertexId;

    QPen orangePen(QColor(255, 165, 0));
    for(auto it = everyVisitedVertex.begin(); it != everyVisitedVertex.end(); ++it)
    {
        Vertex v = *it;
        QVertex *QVisitedVertex = new QVertex(v, color);
        QVisitedVertex->setPen(orangePen);
        scene->addVertex(QVisitedVertex->getVertexCenter(), *QVisitedVertex);
        highlightedQVertices.push_back(QVisitedVertex);
    }

    QPen bluePen(Qt::blue);
    bluePen.setWidth(3);

    for (auto it = bfsVector.rbegin(); it != bfsVector.rend(); ++it)
    {
        childVertexId = it->getObjectId();
        if(parentVertexId == childVertexId) continue;

        Edge &edge = graph_.getAllEdges().at({parentVertexId, childVertexId});

        QVertex sqv(graph_.getAllVertices().at(parentVertexId), color);
        QVertex eqv(*it, color);

        QEdge *qedge = new QEdge(edge,
                                 color,
                                 sqv.getVertexCenter(),
                                 eqv.getVertexCenter()
                                 );
        highlighted.push_back(qedge);
        qedge->setPen(bluePen);
        scene->addEdge(*qedge);
        parentVertexId = childVertexId;
        scene->update();
    }

}

void MainWindow::reset()
{
    for(QEdge* qedge : highlighted)
    {
        scene->removeItem(qedge);
        scene->update();
    }
    highlighted.clear();

    for (QVertex* qvertex : highlightedQVertices)
    {
        scene->removeItem(qvertex);
        scene->update();
    }
    highlightedQVertices.clear();
}
void MainWindow::zoomInBy(int level)
{
    zoomSlider->setValue(zoomSlider->value() + level);
}

void MainWindow::zoomOutBy(int level)
{
    zoomSlider->setValue(zoomSlider->value() - level);
}


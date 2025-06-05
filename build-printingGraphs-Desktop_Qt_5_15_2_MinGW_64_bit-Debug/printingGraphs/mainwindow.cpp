
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSplitter>
#include <QListView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>
#include<QHBoxLayout>
#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Устанавливаем размер главного окна
    this->setGeometry(100, 100, 1500, 500);

    //central
    QWidget* central = new QWidget();

    //кнопки
    m_openFolder = new QPushButton("set files", central);
    m_printGraph = new QPushButton("Print graph", central);
    m_blackWhite = new QCheckBox("black-white", central);

    //надписи
    m_chartDiscription = new QLabel("select the chart type", central);

    m_chartsType = new QComboBox();

    //horizontal
    QHBoxLayout* settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(m_openFolder);
    settingsLayout->addWidget(m_chartDiscription);
    settingsLayout->addWidget(m_chartsType);
    settingsLayout->addWidget(m_blackWhite);
    settingsLayout->addWidget(m_printGraph);

    // vertical layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(settingsLayout);


    //central
    central->setLayout(mainLayout);
    setCentralWidget(central);


    //connects
    connect(m_openFolder, &QPushButton::clicked, this, &MainWindow::on_openFolder);
}

/*
 * Слот для обработки выбора элемента в TreeView.
 * Выбор осуществляется с помощью курсора.
 */

void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
}



void MainWindow::on_openFolder()
{
    qDebug() << "chek ";
}

MainWindow::~MainWindow()
{
    delete ui;
}





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
#include<QFileDialog>
#include <QtCharts/QChartView>

#include "ui_mainwindow.h"

#include "mainwindow.h"

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
    m_setFiles = new QPushButton("set files", central);
    m_printGraph = new QPushButton("Print graph", central);
    m_blackWhite = new QCheckBox("black-white", central);

    //надписи
    m_chartDiscription = new QLabel("select the chart type", central);

    m_chartsType = new QComboBox();

    //horizontal
    QHBoxLayout* settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(m_setFiles);
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
    connect(m_setFiles, &QPushButton::clicked, this, &MainWindow::on_setFiles);
}

/*
 * Слот для обработки выбора элемента в TreeView.
 * Выбор осуществляется с помощью курсора.
 */

void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
}

void MainWindow::on_printGraph() {
    // Твой код, например:
    qDebug() << "Кнопка нажата!";
}


void MainWindow::on_setFiles()
{
    QFileDialog dlg(this, "Choose folder");
    dlg.setFileMode(QFileDialog::Directory);
    dlg.setOption(QFileDialog::ShowDirsOnly, true);
    dlg.setOption(QFileDialog::DontUseNativeDialog);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }
    const QStringList files = dlg.selectedFiles();
    const QString dir = files.first();
    if (dir.isEmpty()) {
        return;
    }
    m_fileExplorer->setRootPath(dir);
    m_listView->setRootIndex(m_fileExplorer->index(dir));
    statusBar()->showMessage("Current dir: " + dir);
}

MainWindow::~MainWindow()
{
    delete ui;
}



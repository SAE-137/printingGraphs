#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QFile>
#include <QTextStream>

#include "mainwindow.h"
#include "qdebug.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(std::shared_ptr<GraphFactory> graph, std::shared_ptr<ReaderFactory> reader, QWidget *parent)
    : QMainWindow(parent)
    , m_readerFactory(std::move(reader))
    , m_graphFactory(std::move(graph))
{
    ui->setupUi(this);
    setGeometry(100, 100, 1500, 500);
    setWindowTitle("Charts App");

    // Центральный виджет
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Кнопки и элементы управления
    m_openFolder = new QPushButton("Open folder", central);
    m_printGraph = new QPushButton("Print graph", central);
    m_blackWhite = new QCheckBox("Black and white", central);
    m_graphsType = new QComboBox(central);
    m_GraphDescription = new QLabel("Select the graph type", central);

    // Layout для настроек
    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(m_openFolder);
    settingsLayout->addWidget(m_GraphDescription);
    settingsLayout->addWidget(m_graphsType);
    settingsLayout->addWidget(m_blackWhite);
    settingsLayout->addWidget(m_printGraph);

    // Файловый менеджер
    m_listView = new QListView(central);
    m_fileExplorer = new QFileSystemModel(this);
    m_fileExplorer->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    m_fileExplorer->setNameFilters({"*.sqlite", "*.json"}); // Фильтр для текстовых файлов
    m_fileExplorer->setNameFilterDisables(false);
    m_listView->setModel(m_fileExplorer);
    m_listView->setRootIndex(m_fileExplorer->index(QDir::homePath()));
    m_listView->setSelectionMode(QAbstractItemView::SingleSelection);
    statusBar()->showMessage("Current dir: " + QDir::homePath());

    // График
    graphView = new QtCharts::QChartView(this);
    graphView->setRenderHint(QPainter::Antialiasing);

    // Разделитель
    QSplitter *splitter = new QSplitter(central);
    splitter->addWidget(m_listView);
    splitter->addWidget(graphView);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    // Компоновка
    mainLayout->addLayout(settingsLayout);
    mainLayout->addWidget(splitter);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    // Заполняем QComboBox
    FillComboBox();

    // Подключение сигналов
    connect(m_openFolder, &QPushButton::clicked, this, &MainWindow::on_openFolder);
    connect(m_listView, &QListView::clicked, this, &MainWindow::on_fileSelected);
    connect(m_printGraph, &QPushButton::clicked, this, &MainWindow::on_printGraph);
    connect(m_blackWhite, &QCheckBox::toggled, this, &MainWindow::on_blackWhiteToggled);
}

void MainWindow::FillComboBox()
{
    m_graphsType->addItem("Line Graph", QVariant("line"));
    m_graphsType->addItem("Scatter Graph", QVariant("scatter"));

}

void MainWindow::on_openFolder()
{
    QFileDialog dlg(this, "Choose folder");
    dlg.setFileMode(QFileDialog::Directory);
    dlg.setOption(QFileDialog::ShowDirsOnly, false);
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



void MainWindow::on_fileSelected(const QModelIndex &index)
{

    QString path = m_fileExplorer->filePath(index);

    QString ext  = QFileInfo(path).suffix();

    auto reader = m_readerFactory->getReader(ext);


    if (!reader) {
        qDebug() << "ljkrn;cn";
        return;
    }
    m_data = reader->loadFromFile(path);

    auto graph = m_graphFactory->getGraph(m_graphsType->currentData().value<GraphType>());

    graph->show(m_data, graphView);


}




void MainWindow::on_printGraph()
{

}

void MainWindow::on_blackWhiteToggled(bool checked)
{
    if (graphView->chart()) {
        graphView->chart()->setTheme(checked ? QtCharts::QChart::ChartThemeHighContrast : QtCharts::QChart::ChartThemeLight);
        graphView->update();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

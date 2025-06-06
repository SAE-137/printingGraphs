#include "mainwindow.h"
#include "qsplitter.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QtCharts/QChart>

MainWindow::MainWindow(std::shared_ptr<GraphFactory> graphFactory,
                       std::shared_ptr<ReaderFactory> readerFactory,
                       QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    readerFactory(std::move(readerFactory)),
    graphFactory(std::move(graphFactory)) {
    ui->setupUi(this);
    setWindowTitle("Graph Viewer");
    setGeometry(200, 200, 1400, 600);

    setupInterface();
    connectSignals();
    PopulateGraphSelector();
}

void MainWindow::setupInterface() {
    // Центральный виджет
    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(mainWidget);

    // Элементы управления
    openDirBtn = new QPushButton("Browse Folder", mainWidget);
    drawGraphBtn = new QPushButton("Draw Graph", mainWidget);
    monoMode = new QCheckBox("Monochrome", mainWidget);
    graphSelector = new QComboBox(mainWidget);
    graphLabel = new QLabel("Choose Graph Type:", mainWidget);

    // Компоновка настроек
    QGridLayout* controlsLayout = new QGridLayout();
    controlsLayout->addWidget(graphLabel, 0, 0);
    controlsLayout->addWidget(graphSelector, 0, 1);
    controlsLayout->addWidget(openDirBtn, 0, 2);
    controlsLayout->addWidget(monoMode, 1, 0);
    controlsLayout->addWidget(drawGraphBtn, 1, 1);

    // Файловый менеджер
    fileList = new QListView(mainWidget);
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    fileModel->setNameFilters(readerFactory->getExpansions());
    fileModel->setNameFilterDisables(false);
    fileList->setModel(fileModel);
    fileList->setRootIndex(fileModel->index(QDir::homePath()));
    fileList->setSelectionMode(QAbstractItemView::SingleSelection);
    statusBar()->showMessage("Directory: " + QDir::homePath());

    // График
    chartDisplay = new QtCharts::QChartView(mainWidget);
    chartDisplay->setRenderHint(QPainter::Antialiasing);

    // Разделитель
    QSplitter* divider = new QSplitter(mainWidget);
    divider->addWidget(fileList);
    divider->addWidget(chartDisplay);
    divider->setStretchFactor(0, 0);
    divider->setStretchFactor(1, 1);

    // Итоговая компоновка
    layout->addLayout(controlsLayout);
    layout->addWidget(divider);
    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);
}

void MainWindow::connectSignals() {
    connect(openDirBtn, &QPushButton::clicked, this, &MainWindow::handleFolderOpen);
    connect(fileList, &QListView::clicked, this, &MainWindow::handleFilePick);
    connect(drawGraphBtn, &QPushButton::clicked, this, &MainWindow::renderGraph);
    connect(monoMode, &QCheckBox::toggled, this, &MainWindow::toggleMonochrome);
    connect(graphSelector, &QComboBox::currentTextChanged, this,
            [this]() { if (!currentData.isEmpty()) renderGraph(); });
}

void MainWindow::PopulateGraphSelector() {
    for (const auto& graph : graphFactory->show()) {
        graphSelector->addItem(graph->getName(), QVariant::fromValue(graph->getType()));
    }
}

void MainWindow::handleFolderOpen() {
    QFileDialog dialog(this, "Select Directory");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    const QString dir = dialog.selectedFiles().first();
    if (dir.isEmpty()) {
        return;
    }
    fileModel->setRootPath(dir);
    fileList->setRootIndex(fileModel->index(dir));
    statusBar()->showMessage("Directory: " + dir);
}

void MainWindow::handleFilePick(const QModelIndex& index) {
    QString filePath = fileModel->filePath(index);
    QString extension = QFileInfo(filePath).suffix().toLower();

    auto reader = readerFactory->getReader(extension);
    if (!reader) {
        QMessageBox::warning(this, "Error", "Unsupported file format: " + extension);
        return;
    }

    currentData = reader->loadFromFile(filePath);
    if (currentData.isEmpty()) {
        QMessageBox::warning(this, "Error", "No valid data in file: " + filePath);
        return;
    }

    renderGraph();
}

void MainWindow::renderGraph() {
    if (currentData.isEmpty()) {
        QMessageBox::warning(this, "Error", "No data to display");
        return;
    }

    GraphType type = graphSelector->currentData().value<GraphType>();
    auto graph = graphFactory->getGraph(type);
    if (!graph) {
        QMessageBox::warning(this, "Error", "No graph renderer for selected type");
        return;
    }

    // Очистка предыдущего графика
    if (chartDisplay->chart()) {
        chartDisplay->setChart(nullptr);
    }

    graph->show(currentData, chartDisplay);

    if (monoMode->isChecked()) {
        chartDisplay->chart()->setTheme(QtCharts::QChart::ChartThemeHighContrast);
    } else {
        chartDisplay->chart()->setTheme(QtCharts::QChart::ChartThemeLight);
    }
}

void MainWindow::toggleMonochrome(bool checked) {
    if (chartDisplay->chart()) {
        chartDisplay->chart()->setTheme(checked ? QtCharts::QChart::ChartThemeHighContrast
                                                : QtCharts::QChart::ChartThemeLight);
        chartDisplay->update();
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

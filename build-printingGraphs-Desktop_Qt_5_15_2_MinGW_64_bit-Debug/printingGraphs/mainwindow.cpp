#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QVariant>
#include <QMessageBox>

#include "MainWindow.h"

MainWindow::MainWindow(std::shared_ptr<GraphFactory> chart, std::shared_ptr<ReaderFactory> reader, QWidget *parent)
    : QMainWindow(parent)
    , m_readerFactory(std::move(reader))
    , m_chartFactory(std::move(chart))
{
    setWindowTitle("printingGraphs");
    setMinimumSize(800, 600);

    m_comboBoxCharts = initChartTypeSelector();

    QWidget* central = new QWidget(this);

    QLabel* labelCharts = new QLabel("Choose charts:", central);

    m_pushButtonSave = new QPushButton("Save chart", central);
    m_pushButtonSave->setEnabled(false);
    m_checkBoxBlackAndWhite = new QCheckBox("Black and white", central);
    m_checkBoxBlackAndWhite->setEnabled(false);

    m_pushButtonFolder = new QPushButton("Open folder", central);

    QHBoxLayout* settingsLayout = new QHBoxLayout();
    QHBoxLayout* chartsLayout = new QHBoxLayout();

    chartsLayout->setSpacing(5);
    chartsLayout->setContentsMargins(0, 0, 0, 0);
    settingsLayout->addWidget(m_pushButtonFolder);
    chartsLayout->addWidget(labelCharts, 0);
    chartsLayout->addWidget(m_comboBoxCharts, 1);
    settingsLayout->addLayout(chartsLayout);
    settingsLayout->addWidget(m_checkBoxBlackAndWhite);
    settingsLayout->addWidget(m_pushButtonSave);

    m_listView = new QListView(this);
    m_chartView = new QtCharts::QChartView(this);
    QSplitter* splitter = new QSplitter(central);
    splitter->addWidget(m_listView);
    splitter->addWidget(m_chartView);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(settingsLayout);
    mainLayout->addWidget(splitter);

    central->setLayout(mainLayout);
    setCentralWidget(central);

    m_fileExplorer = new QFileSystemModel(this);
    m_fileExplorer->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    m_fileExplorer->setRootPath(QDir::homePath());
    m_listView->setModel(m_fileExplorer);
    m_listView->setRootIndex(m_fileExplorer->index(QDir::homePath()));
    statusBar()->showMessage("Current dir: " + QDir::homePath());
    m_listView->setSelectionMode(QAbstractItemView::SingleSelection);

    QStringList filters;
    for(int i = 0; i < m_readerFactory->getExpansions().size(); ++i) {
        filters << "*." + m_readerFactory->getExpansions()[i];
    }
    m_fileExplorer->setNameFilters(filters);
    m_fileExplorer->setNameFilterDisables(false);

    connect(m_pushButtonFolder, &QPushButton::clicked, this, &MainWindow::handleFolderSelection);

    connect(m_listView, &QListView::clicked, this, &MainWindow::loadDataFromSelection);

    connect(m_comboBoxCharts, &QComboBox::currentTextChanged, this, &MainWindow::updateGraphView);

    connect(m_checkBoxBlackAndWhite, &QCheckBox::toggled, this, &MainWindow::toggleMonochromeMode);
    connect(m_pushButtonSave, &QPushButton::clicked, this, &MainWindow::exportChartToPDF);
}

void MainWindow::handleFolderSelection()
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

void MainWindow::loadDataFromSelection(const QModelIndex& ix)
{
    QString path = m_fileExplorer->filePath(ix);
    QString ext  = QFileInfo(path).suffix();
    auto reader = m_readerFactory->getReader(ext);
    if (!reader) {
        return;
    }
    m_currentData = reader->loadFromFile(path);
    if(m_currentData.isEmpty()) {
        resetGraphView();
        QMessageBox::warning(this, "ERROR", "the file is empty!");
        deactivateBWToggle();
        deactivateSaveButton();
        return;
    }
    auto renderer = m_chartFactory->getGraph(
        m_comboBoxCharts->currentData().value<GraphType>());
    if (!renderer) {
        QMessageBox::warning(this, "ERROR", "can't read this file!");
        deactivateBWToggle();
        deactivateSaveButton();
        return;
    }
    renderer->show(m_currentData, m_chartView);
    if(m_checkBoxBlackAndWhite->isChecked()) {
        applyMonochromeStyle();
    }
    activateBWToggleIfDataReady();
    enableSaveButtonIfNeeded();
}

void MainWindow::updateGraphView()
{
    if (m_currentData.isEmpty()) {
        return;
    }
    QVariant v = m_comboBoxCharts->currentData();
    GraphType type = v.value<GraphType>();
    auto renderer = m_chartFactory->getGraph(type);
    if (!renderer) {
        return;
    }
    // перерисовываем график
    renderer->show(m_currentData, m_chartView);
    if(m_checkBoxBlackAndWhite->isChecked()) {
        applyMonochromeStyle();
    }
}

void MainWindow::toggleMonochromeMode(bool checked)
{
    if (m_chartView->chart()) {
        if (checked) {
            applyMonochromeStyle();
        } else {

            GraphType type = m_comboBoxCharts->currentData().value<GraphType>();
            auto renderer = m_chartFactory->getGraph(type);
            renderer->show(m_currentData, m_chartView);
        }
    }
}

void MainWindow::exportChartToPDF()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save to...", "", "PDF (*.pdf)");
    if (!filePath.isEmpty())
    {
        QPdfWriter pdfWriter(filePath);
        QPainter painter(&pdfWriter);
        m_chartView->render(&painter);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if (m_currentData.isEmpty()) {
        return;
    }
    QVariant v = m_comboBoxCharts->currentData();
    GraphType type = v.value<GraphType>();
    auto renderer = m_chartFactory->getGraph(type);
    if (!renderer) {
        return;
    }
    // перерисовываем график
    renderer->show(m_currentData, m_chartView);
    if(m_checkBoxBlackAndWhite->isChecked()) {
        applyMonochromeStyle();
    }
}

QComboBox* MainWindow::initChartTypeSelector() const
{
    QComboBox* comboBox = new QComboBox();
    QVector<std::shared_ptr<IGraphs>> renderers = m_chartFactory->show();
    for(auto& r: renderers) {
        comboBox->addItem(r->getName(), QVariant::fromValue(r->getType()));
    }
    return comboBox;
}

void MainWindow::enableSaveButtonIfNeeded()
{
    if(!m_pushButtonSave->isEnabled()) {
        m_pushButtonSave->setEnabled(true);
    } else {
        return;
    }
}

void MainWindow::activateBWToggleIfDataReady()
{
    if(!m_checkBoxBlackAndWhite->isEnabled()) {
        m_checkBoxBlackAndWhite->setEnabled(true);
    } else {
        return;
    }
}

void MainWindow::deactivateSaveButton()
{
    if(m_pushButtonSave->isEnabled()) {
        m_pushButtonSave->setEnabled(false);
    } else {
        return;
    }
}

void MainWindow::deactivateBWToggle()
{
    if(m_checkBoxBlackAndWhite->isEnabled()) {
        m_checkBoxBlackAndWhite->setEnabled(false);
    } else {
        return;
    }
}

void MainWindow::resetGraphView()
{
    if (auto *chart = m_chartView->chart()) {
        chart->removeAllSeries();
        const auto axes = chart->axes();
        for (auto *axis : axes) {
            chart->removeAxis(axis);
        }
        chart->setTitle({});
    }
}

void MainWindow::applyMonochromeStyle()
{
    m_chartView->chart()->setTheme(QtCharts::QChart::ChartThemeHighContrast);
    if (auto *axisX = m_chartView->chart()->axes(Qt::Horizontal).value(0)) {
        axisX->setShadesVisible(false);
    }
    if (auto *axisY = m_chartView->chart()->axes(Qt::Vertical).value(0)) {
        axisY->setShadesVisible(false);
    }
}

MainWindow::~MainWindow()
{
}


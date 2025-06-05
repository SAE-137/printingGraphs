#include "sqlreader.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>  // Добавлено для QSqlError
#include <QThread>
#include <QDebug>

DataContainer SqlReader::loadFromFile(const QString& filePath) const {
    DataContainer result;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "temp_db_" + QString::number(qintptr(QThread::currentThreadId())));
    db.setDatabaseName(filePath);
    if (!db.open()) {
        qWarning() << "Cannot open SQLite database:" << filePath;
        return result;
    }

    QStringList tables = db.tables();
    if (tables.isEmpty()) {
        db.close();
        QSqlDatabase::removeDatabase("temp_db_" + QString::number(qintptr(QThread::currentThreadId())));
        return result;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT time, value FROM " + tables.first())) {
        qWarning() << "Query failed:" << query.lastError().text();
        db.close();
        QSqlDatabase::removeDatabase("temp_db_" + QString::number(qintptr(QThread::currentThreadId())));
        return result;
    }

    while (query.next()) {
        QDateTime dt = interpretDate(query.value(0).toString());
        if (!dt.isValid()) {
            continue;
        }
        bool ok;
        double val = query.value(1).toDouble(&ok);
        if (ok) {
            result.DataPoints.append({dt, val});
        }
    }

    db.close();
    QSqlDatabase::removeDatabase("temp_db_" + QString::number(qintptr(QThread::currentThreadId())));
    return result;
}

QString SqlReader::getName() const {
    return "sqlite";
}

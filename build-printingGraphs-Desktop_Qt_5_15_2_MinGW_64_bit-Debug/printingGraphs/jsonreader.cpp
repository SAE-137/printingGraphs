#include "jsonreader.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

DataContainer JsonReader::loadFromFile(const QString &filePath)
{
    DataContainer model;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        return model;
    }

    const QByteArray data = file.readAll();
    file.close();

    const QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isArray()) {
        return model;
    }

    for (const QJsonValue &item : doc.array()) {
        if (!item.isObject())
            continue;

        const QJsonObject obj = item.toObject();
        QString rawDate;
        double value = 0.0;
        bool hasDate = false, hasValue = false;

        for (const QString &key : obj.keys()) {
            const QJsonValue val = obj.value(key);

            if (val.isString() && !hasDate) {
                rawDate = val.toString();
                hasDate = true;
            } else if (val.isDouble() && !hasValue) {
                value = val.toDouble();
                hasValue = true;
            }
        }

        if (hasDate && hasValue) {
            QDateTime dt = interpretDate(rawDate);
            if (dt.isValid()) {
                model.DataPoints.append({ dt, value });
            }
        }
    }

    return model;
}


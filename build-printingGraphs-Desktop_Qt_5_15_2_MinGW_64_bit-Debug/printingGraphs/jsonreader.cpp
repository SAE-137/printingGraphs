#include "jsonreader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

DataContainer JsonReader::loadFromFile(const QString& filePath) const {
    DataContainer result;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open JSON file:" << filePath;
        return result;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isNull() || !doc.isArray()) {
        qWarning() << "Invalid JSON format in file:" << filePath;
        return result;
    }

    QJsonArray arr = doc.array();
    for (const QJsonValue& value : arr) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            if (obj.contains("time") && obj.contains("value")) {
                QDateTime date = interpretDate(obj["time"].toString());
                QJsonValue val = obj["value"];
                if (date.isValid() && val.isDouble()) {
                    result.DataPoints.append({date, val.toDouble()});
                } else {
                    qWarning() << "Invalid data in JSON object: time =" << obj["time"].toString() << ", value =" << val;
                }
            } else {
                qWarning() << "JSON object missing 'time' or 'value' keys";
            }
        }
    }
    return result;
}

QString JsonReader::getName() const {
    return "json";
}

#include "idatareader.h"



const QVector<QString> IDataReader::DATE_PATTERNS = {
    "dd.MM.yyyy HH:mm", "dd.MM.yyyy",
    "yyyy.MM.dd HH:mm", "yyyy.MM.dd",
    "dd-MM-yyyy HH:mm", "dd-MM-yyyy",
    "yyyy-MM-dd HH:mm", "yyyy-MM-dd"
};

QDateTime IDataReader::interpretDate(const QString &input) const
{
    const auto parts = input.split(' ', Qt::SkipEmptyParts);

    // Если есть и дата, и время
    if (parts.size() == 2) {
        const QString datePart = parts[0];
        const QString time = parts[1];

        for (const auto& format : DATE_PATTERNS) {
            QDateTime dt = QDateTime::fromString(input, format);
            if (dt.isValid()) {
                return dt;
            }
        }

        bool ok = false;
        int mins = time.toInt(&ok);
        if (ok) {
            for (const auto& format : DATE_PATTERNS) {
                QDate d = QDate::fromString(datePart, format);
                if (d.isValid()) {
                    return d.startOfDay().addSecs(mins * 60);
                }
            }
        }
    }

    // Если только дата без времени (разбиение по дням)
    if (parts.size() == 1) {
        QString dateStr = parts[0];
        for (const auto& format : DATE_PATTERNS) {
            QDate d = QDate::fromString(dateStr, format);
            if (d.isValid()) {
                return d.startOfDay(); // Установим 00:00
            }
        }
    }

    return {};
}



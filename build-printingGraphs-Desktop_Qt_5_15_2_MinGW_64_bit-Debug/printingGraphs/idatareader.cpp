#include "idatareader.h"



const QVector<QString> IDataReader::DATE_PATTERNS = {
    "dd.MM.yyyy HH:mm", "dd.MM.yyyy",
    "yyyy.MM.dd HH:mm", "yyyy.MM.dd",
    "dd-MM-yyyy HH:mm", "dd-MM-yyyy",
    "yyyy-MM-dd HH:mm", "yyyy-MM-dd"
};

QDateTime IDataReader::interpretDate(const QString &input) const
{
    for (const QString &pattern : DATE_PATTERNS) {
        QDateTime dt = QDateTime::fromString(input, pattern);
        if (dt.isValid())
            return dt;
    }

    // разбиение на минуты
    const int spaceIdx = input.indexOf(' ');
    if (spaceIdx > 0) {
        QString dateStr = input.left(spaceIdx);
        QString timeStr = input.mid(spaceIdx + 1);

        bool ok = false;
        int minutes = timeStr.toInt(&ok);
        if (ok) {
            for (const QString &pattern : DATE_PATTERNS) {
                QDate date = QDate::fromString(dateStr, pattern);
                if (date.isValid())
                    return QDateTime(date).addSecs(minutes * 60);
            }
        }
    }

    return QDateTime();
}

//
// Created by pepa on 14.9.22.
//

#include "ReviewLoader.h"
#ifdef Q_OS_WIN
#include <OpenXLSX.hpp>
#else
#include <OpenXLSX/OpenXLSX.hpp>
#endif
#include <QString>
#include <QStringList>
#include <QCalendar>
#include "data.h"

using namespace OpenXLSX;

ReviewLoader::Status ReviewLoader::loadFile(const QString& file) {
    XLDocument doc;
    doc.open(file.toStdString());

    if (!doc.isOpen()) {
        return NOT_XLSX;
    }

    if (doc.workbook().worksheetCount() == 0) {
        return NOT_REPORT;
    }

    std::string sheetName = doc.workbook().worksheetNames()[0];
    auto sheet = doc.workbook().worksheet(sheetName);

    auto parseDate = [](const std::string& strDate) -> std::tuple<bool, QStringList> {
        auto listDateTime = QString(strDate.c_str()).split(" ");

        if (listDateTime.count() != 2) {
            return {false, QStringList()};
        }

        auto listDate = listDateTime[0].split(".");

        if (listDate.count() != 3) {
            return {false, QStringList()};
        }

        return {true, listDate};
    };

    auto [parsed, listDate] = parseDate(sheet.cell(5,1).value());

    if (!parsed) {
        return NOT_REPORT;
    }

    m_review = Review({listDate[1].toInt(), listDate[2].toInt()});
    int numDays = QCalendar().daysInMonth(listDate[1].toInt(), listDate[2].toInt());

    auto loc = std::setlocale(LC_NUMERIC, NULL);
    std::setlocale(LC_NUMERIC, "C");

    for (int i = 0; i < numDays * 24 * 4; i++) {
        auto [subParsed, subDate] = parseDate(sheet.cell(i + 5,1).value());
        if (!subParsed) {
            continue;
        }
        try {
            float buy = sheet.cell(i + 5, 3).value();
            m_review.addBuy(subDate[0].toInt(), buy);
        } catch (XLValueTypeError&) {
            m_review.addBuy(subDate[0].toInt(), 0);
        }

        try {
            float sell = sheet.cell(i + 5, 13).value();
            m_review.addSell(subDate[0].toInt(), sell);
        } catch (XLValueTypeError&) {
            m_review.addSell(subDate[0].toInt(), 0);
        }
    }

    std::setlocale(LC_NUMERIC, loc);

    return OK;
}

const Review& ReviewLoader::review() const {
    return m_review;
}

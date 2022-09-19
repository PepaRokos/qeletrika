//
// Created by pepa on 14.9.22.
//

#ifndef QELETRIKA_REVIEWLOADER_H
#define QELETRIKA_REVIEWLOADER_H

#include <QString>
#include "data.h"

class ReviewLoader {
public:
    enum Status {
        OK = 0,
        NOT_XLSX,
        NOT_REPORT
    };

    ReviewLoader() = default;
    ~ReviewLoader() = default;

    Status loadFile(const QString& file);

    const Review& review() const;

private:
    Review m_review;
};


#endif //QELETRIKA_REVIEWLOADER_H

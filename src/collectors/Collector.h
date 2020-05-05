#ifndef QC_COLLECTOR_H
#define QC_COLLECTOR_H

#include <queue>

#include "common/Expressions.h"

class Collector {
public:
    virtual ~Collector() = default;
    virtual std::deque<std::shared_ptr<Expression>> collect(const std::shared_ptr<Expression> &root) = 0;
};

#endif //QC_COLLECTOR_H

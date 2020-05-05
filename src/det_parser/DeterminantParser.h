#ifndef QC_DETERMINANTPARSER_H
#define QC_DETERMINANTPARSER_H

#include <iosfwd>
#include <memory>
#include <vector>
#include <functional>

#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem/path.hpp>

#include "common/Expressions.h"
#include "det_parser/NodeParser.h"
#include "utils/LinkedHashMap.h"

class DeterminantParser {
public:
    DeterminantParser();
    static void registerNodeParserFabric(NodeParser::Fabric factory);
    std::shared_ptr<Determinant> parse(const boost::filesystem::path &path);
    std::shared_ptr<Determinant> parseDeterminant(std::istream &istream);
    std::shared_ptr<Term> parseTerm(std::istream &istream);
    std::shared_ptr<Expression> parseExpression(std::istream &istream);
    void prepareExpression(const std::shared_ptr<Expression> &expression);

    LinkedHashMap<std::string> strings;
};


#endif //QC_DETERMINANTPARSER_H

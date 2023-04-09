#include "logger_builder_concrete.h"
#include "logger_concrete.h"
#include <map>
#include <fstream>

logger_builder *logger_builder_concrete::add_stream(std::string const &path, logger::severity severity) {
    _construction_info[path] = severity;
    return this;
}

logger *logger_builder_concrete::construct() const {
    return new logger_concrete(_construction_info);
}

// TODO: сделать json
logger *logger_builder_concrete::conduct(std::string const &path)
{
    std::fstream file(path);
    std::string f_path, level;
    std::map<std::string, logger::severity> lev = {{"trace", logger::severity::trace}, 
    {"debug", logger::severity::debug}, 
    {"information", logger::severity::information}, 
    {"warning", logger::severity::warning}, 
    {"error", logger::severity::error}, 
    {"critical", logger::severity::critical}};
    if (file.is_open()){
        while(!file.eof()){
        file >> f_path;
        file >> level;
        add_stream(f_path, lev[level]);
        }
    }
    else{
        throw -1;
    }
    file.close();
    return new logger_concrete(_construction_info);
}

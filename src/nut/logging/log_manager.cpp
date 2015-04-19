﻿
#include <nut/platform/platform.h>
#include <nut/util/txtcfg/property_dom.h>
#include <nut/util/txtcfg/txt_file.h>
#include <nut/threading/sync/mutex.h>
#include <nut/threading/sync/guard.h>

#include "log_manager.h"
#include "logger.h"

namespace nut
{

Logger* LogManager::get_logger(const std::string &path)
{
    return ((Logger*)nut_get_root_logger())->get_logger(path);
}

void LogManager::load_config(rc_ptr<PropertyDom> config)
{
    assert(!config.is_null());
    std::vector<std::string> handlers;
    config->get_list("LogHandlers", &handlers, ',');
    for (size_t i = 0, szi = handlers.size(); i < szi; ++i)
    {
        const std::string& h = handlers.at(i);
        std::string str_type = config->get_string((h + "_type").c_str());
        std::vector<std::string> strpos;
        config->get_list((h + "_pos").c_str(), &strpos, ':');
        std::string str_filter = config->get_string((h + "_filter").c_str());

        rc_ptr<LogHandler> handler = LogHandlerFactory::create_log_handler(str_type);
        rc_ptr<LogFilter> filter = LogFilterFactory::create_log_filter(str_filter);
        handler->add_filter(filter);
        if (strpos.size() == 0)
        {
            ((Logger*)nut_get_root_logger())->add_handler(handler);
        }
        else
        {
            for (size_t j = 0, szj = strpos.size(); j < szj; ++j)
                get_logger(strpos.at(j))->add_handler(handler);
        }
    }
}

void LogManager::load_config(const char *config_file)
{
    assert(NULL != config_file);
    rc_ptr<PropertyDom> pd = rc_new<PropertyDom>();
    std::string all;
    TxtFile::read_file(config_file, &all);
    pd->parse(all);
    load_config(pd);
}

}

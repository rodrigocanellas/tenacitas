#ifndef TENACITAS_BUSINESS_CROSSWORDS_LOG_H
#define TENACITAS_BUSINESS_CROSSWORDS_LOG_H


#define crosswords_log_test(log, p_params...)                                  \
    log::test(__FILE__, __LINE__, p_params)

#define crosswords_log_debug(log, p_params...)                                 \
    log::debug(__FILE__, __LINE__, p_params)

#define crosswords_log_info(log, p_params...)                                  \
    log::info(__FILE__, __LINE__, p_params)

#define crosswords_log_warn(log, p_params...)                                  \
    log::warn(__FILE__, __LINE__, p_params)

#define crosswords_log_error(log, p_params...)                                 \
    log::error(__FILE__, __LINE__, p_params)

#define crosswords_log_fatal(log, p_params...)                                 \
    log::fatal(__FILE__, __LINE__, p_params)

#endif // LOG_H

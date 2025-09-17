/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LOG_CPT_MACROS_H
#define TNCT_LOG_CPT_MACROS_H

#define TNCT_LOG_TRA(logger, msg)                                              \
  do                                                                           \
  {                                                                            \
    if (logger.can_tra())                                                      \
    {                                                                          \
      logger.tra(msg);                                                         \
    }                                                                          \
  } while (0)

#define TNCT_LOG_DEB(logger, msg)                                              \
  do                                                                           \
  {                                                                            \
    if (logger.can_deb())                                                      \
    {                                                                          \
      logger.deb(msg);                                                         \
    }                                                                          \
  } while (0)

#define TNCT_LOG_INF(logger, msg)                                              \
  do                                                                           \
  {                                                                            \
    if (logger.can_inf())                                                      \
    {                                                                          \
      logger.inf(msg);                                                         \
    }                                                                          \
  } while (0)

#define TNCT_LOG_WAR(logger, msg)                                              \
  do                                                                           \
  {                                                                            \
    if (logger.can_war())                                                      \
    {                                                                          \
      logger.war(msg);                                                         \
    }                                                                          \
  } while (0)

#define TNCT_LOG_ERR(logger, msg) logger.err(msg);

#define TNCT_LOG_FAT(logger, msg) logger.fat(msg);

#define TNCT_LOG_TST(logger, msg) logger.tst(msg);

#endif

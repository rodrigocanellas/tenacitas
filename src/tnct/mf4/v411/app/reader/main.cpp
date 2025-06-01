#include "tnct/log/cout.h"
#include "tnct/mf4/v411/mem/file.h"
#include "tnct/mf4/v411/per/file.h"

using namespace tnct;

int main(int argc, char **argv) {
  try {
    if (argc == 1) {
      std::cout << "Syntax: " << argv[0] << " <mf4-file>\n";
      return -1;
    }

    log::cout _logger;
    _logger.set_tra();

    mf4::v411::mem::file _mf4_file;

    mf4::v411::per::read(argv[1], _logger, _mf4_file);

    std::cout << '\n';

    _mf4_file.report(std::cout, _logger);
  } catch (...) {
  }
}

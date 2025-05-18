

#include "tnct/format/fmt.h"
#include "tnct/log/cout.h"
#include "tnct/mf4/mem/classes.h"
#include "tnct/mf4/per/classes.h"
#include "tnct/traits/log/logger.h"

// template <>
// struct Block<HD>
// {
//     static constexpr std::streamsize size{HeaderSection<HD>::size +
//     LinkSection<HD>::size};
// };

// template <Id t_block_Id>
// class Block
// {
//   public:
//     constexpr static Id id_{t_block_Id};

//   private:
//     HeaderSection header_section_;
//     LinkSection<t_block_Id> link_section_;
//     DataSection<t_block_Id> data_section_;
// };

// class File
// {
//   private:
//     template <Id t_block_Id>
//     using BlocksVector = std::vector<Block<t_block_Id>>;

//     using Blocks = std::tuple<BlocksVector<Id::MD>,
//                               BlocksVector<Id::TX>,
//                               BlocksVector<Id::FH>,
//                               BlocksVector<Id::CH>,
//                               BlocksVector<Id::AT>,
//                               BlocksVector<Id::AT>,
//                               BlocksVector<Id::EV>,
//                               BlocksVector<Id::DG>,
//                               BlocksVector<Id::CG>,
//                               BlocksVector<Id::SI>,
//                               BlocksVector<Id::CN>,
//                               BlocksVector<Id::CC>,
//                               BlocksVector<Id::CA>,
//                               BlocksVector<Id::DT>,
//                               BlocksVector<Id::SR>,
//                               BlocksVector<Id::RD>,
//                               BlocksVector<Id::SD>,
//                               BlocksVector<Id::DL>,
//                               BlocksVector<Id::DZ>,
//                               BlocksVector<Id::HL>>;

//     Block<Id::ID> identification_;
//     Block<Id::HD> header_;
// };

// template <std::unsigned_integral TNum> static bool to_from_to(TNum num_to) {
//   auto array_to{tnct::byte_array::to_little(num_to)};
//   std::cout << tnct::byte_array::to_str<decltype(num_to)>(array_to) <<
//   std::endl;

//   auto num_from_1{tnct::byte_array::from_little<TNum>(array_to)};
//   std::cout << num_from_1 << std::endl;

//   auto num_from_2{tnct::byte_array::from_little<TNum>(
//       reinterpret_cast<const char *>(array_to.data()))};
//   std::cout << num_from_1 << std::endl;

//   return (num_to == num_from_1) && (num_to == num_from_2);
// }

// template <std::unsigned_integral TNum> void test_00(TNum num) {
//   std::cout << "####\n";
//   if (to_from_to<TNum>(num)) {
//     std::cout << "ok for ";
//   } else {
//     std::cout << "NOT of for ";
//   }
//   std::cout << num << std::endl;
// }

using namespace tnct;

int main(int argc, char **argv) {
  // test_00(static_cast<uint16_t>(411));

  // test_00(static_cast<uint32_t>(104));
  // test_00(std::numeric_limits<std::uint32_t>::max());

  // test_00(static_cast<uint64_t>(104));
  // test_00(std::numeric_limits<std::uint64_t>::max() - 1);

  // ctw::log::tra("test trace");
  // ctw::log::deb("test debug");
  // ctw::log::inf("test inf");
  // ctw::log::war("test warning");
  // ctw::log::err("test error");

  if (argc == 1) {
    std::cout << "Syntax: " << argv[0] << " <mf4-file>\n";
    return -1;
  }

  log::cout _logger;

  mf4::mem::result result{mf4::per::mf4_reader(argv[1], _logger)};
  TNCT_LOG_INF(_logger, tnct::format::fmt("result = ", result));
}



#include "tnct/format/fmt.h"
#include "tnct/log/cout.h"
#include "tnct/mf4/mem/classes.h"
#include "tnct/mf4/per/classes.h"
#include "tnct/traits/log/logger.h"

// template <>
// struct Block<Traits::HD>
// {
//     static constexpr std::streamsize size{HeaderSection<Traits::HD>::size +
//     LinkSection<Traits::HD>::size};
// };

// template <Traits::Id TBlockId>
// class Block
// {
//   public:
//     constexpr static Traits::Id id_{TBlockId};

//   private:
//     HeaderSection header_section_;
//     LinkSection<TBlockId> link_section_;
//     DataSection<TBlockId> data_section_;
// };

// class File
// {
//   private:
//     template <Traits::Id TBlockId>
//     using BlocksVector = std::vector<Block<TBlockId>>;

//     using Blocks = std::tuple<BlocksVector<Traits::Id::MD>,
//                               BlocksVector<Traits::Id::TX>,
//                               BlocksVector<Traits::Id::FH>,
//                               BlocksVector<Traits::Id::CH>,
//                               BlocksVector<Traits::Id::AT>,
//                               BlocksVector<Traits::Id::AT>,
//                               BlocksVector<Traits::Id::EV>,
//                               BlocksVector<Traits::Id::DG>,
//                               BlocksVector<Traits::Id::CG>,
//                               BlocksVector<Traits::Id::SI>,
//                               BlocksVector<Traits::Id::CN>,
//                               BlocksVector<Traits::Id::CC>,
//                               BlocksVector<Traits::Id::CA>,
//                               BlocksVector<Traits::Id::DT>,
//                               BlocksVector<Traits::Id::SR>,
//                               BlocksVector<Traits::Id::RD>,
//                               BlocksVector<Traits::Id::SD>,
//                               BlocksVector<Traits::Id::DL>,
//                               BlocksVector<Traits::Id::DZ>,
//                               BlocksVector<Traits::Id::HL>>;

//     Block<Traits::Id::ID> identification_;
//     Block<Traits::Id::HD> header_;
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

  mf4::mem::Result result{mf4::per::mf4_reader(argv[1], _logger)};
  TNCT_LOG_INF(_logger, tnct::format::fmt("result = ", result));
}

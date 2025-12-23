#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "pproto/serialize/json.h"

#include "catch2/log_saver.h"
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_session.hpp"

namespace pproto {
namespace data {

struct F
{
    float  p1 = {0};
    double p2 = {0};

    J_SERIALIZE_BEGIN
        J_SERIALIZE_ITEM( p1 )
        J_SERIALIZE_ITEM( p2 )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto

TEST_CASE( "Deserialize real number fields from NULL", "[json]" )
{
    SECTION( "Filling out F all fields as NaN (p1, p1 is NULL)" )
    {
        QByteArray json = R"({"p1":null,"p2":null})";

        pproto::data::F f;
        pproto::SResult sr = f.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( std::isnan(f.p1) );
        REQUIRE( std::isnan(f.p2) );
    }
}

int main(int argc, char* argv[])
{
    alog::logger().start();

    alog::Saver::Ptr saver {new alog::CatchSaver()};
    alog::logger().addSaver(saver);

    int result = Catch::Session().run(argc, argv);

    alog::stop();

    return result;
}

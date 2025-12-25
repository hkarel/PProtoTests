#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "pproto/serialize/json.h"

#include "catch2/log_saver.h"
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_session.hpp"

namespace pproto {
namespace data {

struct A
{
    qint32     p1 = {0};
    QString    p2;
    QByteArray p3;

    J_SERIALIZE_BEGIN
        J_SERIALIZE_ITEM( p1 )
        J_SERIALIZE_ITEM( p2 )
        J_SERIALIZE_ITEM( p3 )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto

TEST_CASE( "Serialize A structure", "[json]" )
{
    using namespace pproto::data;

    A a;
    a.p1 = 10;
    a.p2 = "string";
    a.p3 = "bytearray long string 12345678900";

    QByteArray json = a.toJson();
    REQUIRE( json == R"({"p1":10,"p2":"string","p3":"bytearray long string 12345678900"})" );

    A aa;
    pproto::SResult sr = aa.fromJson(json);
    ALOG_FLUSH();
    REQUIRE( bool(sr) == true );

    REQUIRE( aa.p1 == 10 );
    REQUIRE( aa.p2 == "string" );
    REQUIRE( aa.p3 == "\"bytearray long string 12345678900\"" );
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

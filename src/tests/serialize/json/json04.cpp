#include "shared/clife_base.h"
#include "shared/clife_ptr.h"
#include "shared/container_ptr.h"
#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "pproto/serialize/json.h"

#include "catch2/log_saver.h"
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_session.hpp"

namespace pproto {
namespace data {

struct A : clife_base
{
    typedef clife_ptr<A> Ptr;

    qint32  v1 = {0};
    QString v2 = {"str 1234"};

    J_SERIALIZE_BEGIN
        J_SERIALIZE_ITEM( v1 )
        J_SERIALIZE_OPT ( v2 )
    J_SERIALIZE_END
};

struct B
{
    typedef container_ptr<B> Ptr;

    qint64  v1 = {0};
    QString v2 = {"str 5678"};

    J_SERIALIZE_BEGIN
        J_SERIALIZE_ITEM( v1 )
        J_SERIALIZE_OPT ( v2 )
    J_SERIALIZE_END
};

struct C
{
    qint32  p1 = {0};
    QString p2 = {"1234"};
    A::Ptr  p3;
    B::Ptr  p4;

    J_SERIALIZE_BEGIN
        J_SERIALIZE_ITEM( p1 )
        J_SERIALIZE_ITEM( p2 )
        J_SERIALIZE_ITEM( p3 )
        J_SERIALIZE_ITEM( p4 )
    J_SERIALIZE_END
};

struct D
{
    qint32  p1 = {0};
    QString p2 = {"5678"};
    A::Ptr  p3;
    B::Ptr  p4;

    J_SERIALIZE_BEGIN
        J_SERIALIZE_ITEM( p1 )
        J_SERIALIZE_ITEM( p2 )
        J_SERIALIZE_OPT ( p3 )
        J_SERIALIZE_OPT ( p4 )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto

TEST_CASE( "Serialize smart-pointer fields", "[json]" )
{
    pproto::data::C c;
    c.p1 = 12;
    c.p2 = "string 123";

    SECTION( "Empty all smart-pointers" )
    {
        QByteArray json = c.toJson();
        REQUIRE( json == R"({"p1":12,"p2":"string 123","p3":null,"p4":null})" );
    }
    SECTION( "Empty container_ptr smart-pointer" )
    {
        c.p3 = pproto::data::A::Ptr{new pproto::data::A};
        c.p3->v1 = 15;

        QByteArray json = c.toJson();
        REQUIRE( json == R"({"p1":12,"p2":"string 123","p3":{"v1":15,"v2":"str 1234"},"p4":null})" );
    }
    SECTION( "Empty clife_ptr smart-pointer" )
    {
        c.p4 = pproto::data::B::Ptr{new pproto::data::B};
        c.p4->v1 = -58;
        c.p4->v2 = "str 6789";

        QByteArray json = c.toJson();
        REQUIRE( json == R"({"p1":12,"p2":"string 123","p3":null,"p4":{"v1":-58,"v2":"str 6789"}})" );
    }
    SECTION( "Fill all smart-pointers" )
    {
        c.p3 = pproto::data::A::Ptr{new pproto::data::A};
        c.p3->v1 = 25;

        c.p4 = pproto::data::B::Ptr{new pproto::data::B};
        c.p4->v1 = 34;
        c.p4->v2 = "str 6789";

        QByteArray json = c.toJson();
        REQUIRE( json == R"({"p1":12,"p2":"string 123","p3":{"v1":25,"v2":"str 1234"},"p4":{"v1":34,"v2":"str 6789"}})" );
    }
}

TEST_CASE( "Deserialize smart-pointer fields", "[json]" )
{
    SECTION( "Empty all smart-pointers" )
    {
        QByteArray json = R"({"p1":14,"p2":"string 890","p3":null,"p4":null})";

        pproto::data::C c;
        pproto::SResult sr = c.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( c.p1 == 14 );
        REQUIRE( c.p2 == "string 890" );
        REQUIRE( c.p3.empty() == true );
        REQUIRE( c.p4.empty() == true );
    }
    SECTION( "Undefined the smart-pointers (1)" )
    {
        QByteArray json = R"({"p1":14,"p2":"string 890"})";

        pproto::data::C c;
        pproto::SResult sr = c.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == false );
    }
    SECTION( "Undefined the smart-pointers (2)" )
    {
        QByteArray json = R"({"p1":14,"p2":"string 890"})";

        pproto::data::D d;
        pproto::SResult sr = d.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( d.p1 == 14 );
        REQUIRE( d.p2 == "string 890" );
        REQUIRE( d.p3.empty() == true );
        REQUIRE( d.p4.empty() == true );
    }

    SECTION( "Undefined clife_ptr smart-pointer (1)" )
    {
        QByteArray json = R"({"p1":12,"p2":"string 123","p3":null,"p4":{"v1":-58,"v2":"str 6789"}})";

        pproto::data::C c;
        pproto::SResult sr = c.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( c.p1 == 12             );
        REQUIRE( c.p2 == "string 123"   );
        REQUIRE( c.p3.empty() == true   );
        REQUIRE( c.p4->v1 == -58        );
        REQUIRE( c.p4->v2 == "str 6789" );
    }
    SECTION( "Undefined clife_ptr smart-pointer (2)" )
    {
        QByteArray json = R"({"p1":12,"p2":"string 123","p4":{"v1":-58,"v2":"str 6789"}})";

        pproto::data::D d;
        pproto::SResult sr = d.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( d.p1 == 12             );
        REQUIRE( d.p2 == "string 123"   );
        REQUIRE( d.p3.empty() == true   );
        REQUIRE( d.p4->v1 == -58        );
        REQUIRE( d.p4->v2 == "str 6789" );
    }

    SECTION( "Undefined container_ptr smart-pointer (1)" )
    {
        QByteArray json = R"({"p1":12,"p2":"string 123","p3":{"v1":15,"v2":"str 234"},"p4":null})";

        pproto::data::C c;
        pproto::SResult sr = c.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( c.p1 == 12            );
        REQUIRE( c.p2 == "string 123"  );
        REQUIRE( c.p3->v1 == 15        );
        REQUIRE( c.p3->v2 == "str 234" );
        REQUIRE( c.p4.empty() == true  );
    }
    SECTION( "Undefined container_ptr smart-pointer (2)" )
    {
        QByteArray json = R"({"p1":12,"p2":"string 123","p3":{"v1":15,"v2":"str 234"}})";

        pproto::data::D d;
        pproto::SResult sr = d.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( d.p1 == 12            );
        REQUIRE( d.p2 == "string 123"  );
        REQUIRE( d.p3->v1 == 15        );
        REQUIRE( d.p3->v2 == "str 234" );
        REQUIRE( d.p4.empty() == true  );
    }

    SECTION( "Undefined fields in smart-pointers" )
    {
        QByteArray json = R"({"p1":12,"p2":"string 123","p3":{"v1":15},"p4":{"v1":-58}})";

        pproto::data::C c;
        pproto::SResult sr = c.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( c.p1 == 12             );
        REQUIRE( c.p2 == "string 123"   );
        REQUIRE( c.p3->v1 == 15         );
        REQUIRE( c.p3->v2 == "str 1234" );
        REQUIRE( c.p4->v1 == -58        );
        REQUIRE( c.p4->v2 == "str 5678" );
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

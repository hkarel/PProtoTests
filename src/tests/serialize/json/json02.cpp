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

struct Asub
{
    qint64 v1 = {0};
    double v2 = {0};

    J_SERIALIZE_BEGIN
        J_SERIALIZE_ITEM( v1 )
        J_SERIALIZE_ITEM( v2 )
    J_SERIALIZE_END
};

struct A2
{
    qint32  p1 = {0};
    QString p2;
    Asub    p3;

    J_SERIALIZE_BEGIN
        J_SERIALIZE_ITEM( p1 )
        J_SERIALIZE_ITEM( p2 )
        J_SERIALIZE_ITEM( p3 )
    J_SERIALIZE_END
};

struct A3
{
    qint32      p1 = {0};
    QString     p2;
    QList<Asub> p3;

    J_SERIALIZE_BEGIN
        J_SERIALIZE_ITEM( p1 )
        J_SERIALIZE_ITEM( p2 )
        J_SERIALIZE_ITEM( p3 )
    J_SERIALIZE_END
};


} // namespace data
} // namespace pproto

TEST_CASE( "Conversion of bytearray field", "[json]" )
{
    pproto::data::A a;
    a.p1 = 10;
    a.p2 = "string";

    SECTION( "Bytearray is data struct (1)" )
    {
        a.p3 = R"({"v1":37,"v2":0.987})";

        QByteArray json = a.toJson();
        REQUIRE( json == R"({"p1":10,"p2":"string","p3":{"v1":37,"v2":0.987}})" );

        pproto::data::A2 a2;
        pproto::SResult sr = a2.fromJson(json);
        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( a2.p1 == 10 );
        REQUIRE( a2.p2 == "string" );
        REQUIRE( a2.p3.v1 == 37 );
        REQUIRE( a2.p3.v2 == 0.987 );
    }
    SECTION( "Bytearray is data struct (2)" )
    {
        pproto::data::A2 a2;
        a2.p1 = 10;
        a2.p2 = "string";
        a2.p3.v1 = 42;
        a2.p3.v2 = 121.05;

        QByteArray json = a2.toJson();
        REQUIRE( json == R"({"p1":10,"p2":"string","p3":{"v1":42,"v2":121.05}})" );

        pproto::data::A aa;
        pproto::SResult sr = aa.fromJson(json);
        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( aa.p1 == 10 );
        REQUIRE( aa.p2 == "string" );
        REQUIRE( aa.p3 == R"({"v1":42,"v2":121.05})" );
    }

    SECTION( "Bytearray is array (1)" )
    {
        a.p3 = R"([{"v1":1,"v2":0.1},{"v1":2,"v2":0.2},{"v1":3,"v2":0.3}])";

        QByteArray json = a.toJson();
        REQUIRE( json == R"({"p1":10,"p2":"string","p3":[{"v1":1,"v2":0.1},{"v1":2,"v2":0.2},{"v1":3,"v2":0.3}]})" );

        pproto::data::A3 a3;
        pproto::SResult sr = a3.fromJson(json);
        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( a3.p1 == 10 );
        REQUIRE( a3.p2 == "string"  );
        REQUIRE( a3.p3[0].v1 == 1   );
        REQUIRE( a3.p3[0].v2 == 0.1 );
        REQUIRE( a3.p3[1].v1 == 2   );
        REQUIRE( a3.p3[1].v2 == 0.2 );
        REQUIRE( a3.p3[2].v1 == 3   );
        REQUIRE( a3.p3[2].v2 == 0.3 );
    }
    SECTION( "Bytearray is array (2)" )
    {
        pproto::data::A3 a3;
        a3.p1 = 10;
        a3.p2 = "string";
        a3.p3 = {{4, 0.4}, {5, 0.5}, {6, 0.6}};

        QByteArray json = a3.toJson();
        REQUIRE( json == R"({"p1":10,"p2":"string","p3":[{"v1":4,"v2":0.4},{"v1":5,"v2":0.5},{"v1":6,"v2":0.6}]})" );

        pproto::data::A aa;
        pproto::SResult sr = aa.fromJson(json);
        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( aa.p1 == 10 );
        REQUIRE( aa.p2 == "string" );
        REQUIRE( aa.p3 == R"([{"v1":4,"v2":0.4},{"v1":5,"v2":0.5},{"v1":6,"v2":0.6}])" );
    }

    SECTION( "Bytearray is long string" )
    {
        a.p3 = "bytearray long string 12345678900";

        QByteArray json = a.toJson();
        REQUIRE( json == R"({"p1":10,"p2":"string","p3":"bytearray long string 12345678900"})" );

        pproto::data::A aa;
        pproto::SResult sr = aa.fromJson(json);
        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( aa.p3 == "\"bytearray long string 12345678900\"" );
    }
    SECTION( "Bytearray is short string" )
    {
        a.p3 = "bytearray short string";

        QByteArray json = a.toJson();
        REQUIRE( json == R"({"p1":10,"p2":"string","p3":"bytearray short string"})" );

        pproto::data::A aa;
        pproto::SResult sr = aa.fromJson(json);
        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( aa.p3 == "\"bytearray short string\"" );
    }
    SECTION( "Bytearray as bool type" )
    {
        SECTION( "Bytearray is 'true'" )
        {
            a.p3 = "true";

            QByteArray json = a.toJson();
            REQUIRE( json == R"({"p1":10,"p2":"string","p3":true})" );

            pproto::data::A aa;
            pproto::SResult sr = aa.fromJson(json);
            ALOG_FLUSH();
            REQUIRE( bool(sr) == true );

            REQUIRE( aa.p3 == "true" );
        }
        SECTION( "Bytearray is 'True'" )
        {
            a.p3 = "True";

            QByteArray json = a.toJson();
            REQUIRE( json == R"({"p1":10,"p2":"string","p3":true})" );

            pproto::data::A aa;
            pproto::SResult sr = aa.fromJson(json);
            ALOG_FLUSH();
            REQUIRE( bool(sr) == true );

            REQUIRE( aa.p3 == "true" );
        }
        SECTION( "Bytearray is 'false'" )
        {
            a.p3 = "false";

            QByteArray json = a.toJson();
            REQUIRE( json == R"({"p1":10,"p2":"string","p3":false})" );

            pproto::data::A aa;
            pproto::SResult sr = aa.fromJson(json);
            ALOG_FLUSH();
            REQUIRE( bool(sr) == true );

            REQUIRE( aa.p3 == "false" );
        }
        SECTION( "Bytearray is 'False'" )
        {
            a.p3 = "False";

            QByteArray json = a.toJson();
            REQUIRE( json == R"({"p1":10,"p2":"string","p3":false})" );

            pproto::data::A aa;
            pproto::SResult sr = aa.fromJson(json);
            ALOG_FLUSH();
            REQUIRE( bool(sr) == true );

            REQUIRE( aa.p3 == "false" );
        }
    }
    SECTION( "Bytearray is long int" )
    {
        a.p3 = "-125";

        QByteArray json = a.toJson();

        REQUIRE( json == R"({"p1":10,"p2":"string","p3":-125})" );

        pproto::data::A aa;
        pproto::SResult sr = aa.fromJson(json);
        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( aa.p3 == "-125" );
    }
    SECTION( "Bytearray is unsigned long int" )
    {
        a.p3 = "18446744073709551614";

        QByteArray json = a.toJson();

        REQUIRE( json == R"({"p1":10,"p2":"string","p3":18446744073709551614})" );

        pproto::data::A aa;
        pproto::SResult sr = aa.fromJson(json);
        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( aa.p3 == "18446744073709551614" );
    }
    SECTION( "Bytearray is double" )
    {
        a.p3 = "125.01";

        QByteArray json = a.toJson();

        REQUIRE( json == R"({"p1":10,"p2":"string","p3":125.01})" );

        pproto::data::A aa;
        pproto::SResult sr = aa.fromJson(json);
        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( aa.p3 == "125.01" );
    }
}

int main(int argc, char* argv[])
{
    alog::logger().start();

    alog::SaverPtr saver {new alog::CatchSaver()};
    alog::logger().addSaver(saver);

    int result = Catch::Session().run( argc, argv );

    alog::stop();

    return result;
}

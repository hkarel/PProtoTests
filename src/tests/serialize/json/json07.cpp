#include "shared/container_ptr.h"
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
    typedef container_ptr<A> Ptr;

    qint32  p1 = {0};
    QString p2 = {"str"};

    bool operator< (const A& a) const
    {
        return (p1 < a.p1) && (p2 < a.p2);
    }

    J_SERIALIZE_BEGIN
        J_SERIALIZE_ITEM( p1 )
        J_SERIALIZE_OPT ( p2 )
    J_SERIALIZE_END
};

bool operator== (const A& a1, const A& a2)
{
    return (a1.p1 == a2.p1) && (a1.p2 == a2.p2);
}

template<typename MapT>
struct M
{
    MapT map;
    J_SERIALIZE_ONE( map )
};

} // namespace data
} // namespace pproto

TEST_CASE( "Serialize/Deserialize Map-s", "[json]" )
{
    using namespace pproto::data;

    //--- QMap ---

    SECTION( "Serialize QMap<int, A>" )
    {
        M<QMap<int, A>> m;
        m.map[1] = {10, "mp1"};
        m.map[2] = {20, "mp2"};

        QByteArray json = m.toJson();
        REQUIRE( json == R"({"map":[{"k":1,"v":{"p1":10,"p2":"mp1"}},{"k":2,"v":{"p1":20,"p2":"mp2"}}]})" );
    }

    SECTION( "Deserialize QMap<int, A>" )
    {
        QByteArray json = R"({"map":[{"k":1,"v":{"p1":10,"p2":"mp1"}},{"k":2,"v":{"p1":20,"p2":"mp2"}}]})";

        M<QMap<int, A>> m;
        pproto::SResult sr = m.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( m.map[1] == A{10, "mp1"} );
        REQUIRE( m.map[2] == A{20, "mp2"} );
    }

    SECTION( "Deserialize QMap<int, A> (A::p2 is NULL)" )
    {
        QByteArray json = R"({"map":[{"k":1,"v":{"p1":10,"p2":"mp1"}},{"k":2,"v":{"p1":20,"p2":null}}]})";

        M<QMap<int, A>> m;
        pproto::SResult sr = m.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( m.map[1] == A{10, "mp1"} );
        REQUIRE( m.map[2] == A{20, ""   } );
    }

    SECTION( "Serialize QMap<A, int>" )
    {
        M<QMap<A, int>> m;
        m.map[A{10, "mp1"}] = 1;
        m.map[A{20, "mp2"}] = 2;

        QByteArray json = m.toJson();
        REQUIRE( json == R"({"map":[{"k":{"p1":10,"p2":"mp1"},"v":1},{"k":{"p1":20,"p2":"mp2"},"v":2}]})" );
    }

    SECTION( "Deserialize QMap<A, int>" )
    {
        QByteArray json = R"({"map":[{"k":{"p1":10,"p2":"mp1"},"v":1},{"k":{"p1":20,"p2":"mp2"},"v":2}]})";

        M<QMap<A, int>> m;
        pproto::SResult sr = m.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( m.map[A{10, "mp1"}] == 1 );
        REQUIRE( m.map[A{20, "mp2"}] == 2 );
    }

    SECTION( "Serialize QMap<int, A::Ptr> smart-pointers" )
    {
        M<QMap<int, A::Ptr>> m;
        m.map[1] = A::Ptr(new A{10, "mp1"});
        m.map[2] = A::Ptr(new A{20, "mp2"});
        m.map[3] = {};

        QByteArray json = m.toJson();
        REQUIRE( json == R"({"map":[{"k":1,"v":{"p1":10,"p2":"mp1"}},{"k":2,"v":{"p1":20,"p2":"mp2"}},{"k":3,"v":null}]})" );
    }

    SECTION( "Deserialize QMap<int, A::Ptr> smart-pointers" )
    {
        QByteArray json = R"({"map":[{"k":1,"v":{"p1":10,"p2":"mp1"}},{"k":2,"v":{"p1":20,"p2":"mp2"}},{"k":3,"v":null}]})";

        M<QMap<int, A::Ptr>> m;
        pproto::SResult sr = m.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( m.map[1].empty() == false );
        REQUIRE( m.map[2].empty() == false );
        REQUIRE( m.map[3].empty() == true  );

        if (!m.map[1])
            REQUIRE( *m.map[1] == A{10, "mp1"} );

        if (!m.map[2])
            REQUIRE( *m.map[2] == A{20, "mp2"} );
    }

    //--- std::map ---

    SECTION( "Serialize std::map<int, A>" )
    {
        M<std::map<int, A>> m;
        m.map[1] = {10, "mp1"};
        m.map[2] = {20, "mp2"};

        QByteArray json = m.toJson();
        REQUIRE( json == R"({"map":[{"k":1,"v":{"p1":10,"p2":"mp1"}},{"k":2,"v":{"p1":20,"p2":"mp2"}}]})" );
    }

    SECTION( "Deserialize std::map<int, A>" )
    {
        QByteArray json = R"({"map":[{"k":1,"v":{"p1":10,"p2":"mp1"}},{"k":2,"v":{"p1":20,"p2":"mp2"}}]})";

        M<std::map<int, A>> m;
        pproto::SResult sr = m.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( m.map[1] == A{10, "mp1"} );
        REQUIRE( m.map[2] == A{20, "mp2"} );
    }

    SECTION( "Deserialize std::map<int, A> (A::p2 is NULL)" )
    {
        QByteArray json = R"({"map":[{"k":1,"v":{"p1":10,"p2":"mp1"}},{"k":2,"v":{"p1":20,"p2":null}}]})";

        M<std::map<int, A>> m;
        pproto::SResult sr = m.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( m.map[1] == A{10, "mp1"} );
        REQUIRE( m.map[2] == A{20, ""   } );
    }

    SECTION( "Serialize std::map<A, int>" )
    {
        M<std::map<A, int>> m;
        m.map[A{10, "mp1"}] = 1;
        m.map[A{20, "mp2"}] = 2;

        QByteArray json = m.toJson();
        REQUIRE( json == R"({"map":[{"k":{"p1":10,"p2":"mp1"},"v":1},{"k":{"p1":20,"p2":"mp2"},"v":2}]})" );
    }

    SECTION( "Deserialize std::map<A, int>" )
    {
        QByteArray json = R"({"map":[{"k":{"p1":10,"p2":"mp1"},"v":1},{"k":{"p1":20,"p2":"mp2"},"v":2}]})";

        M<std::map<A, int>> m;
        pproto::SResult sr = m.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( m.map[A{10, "mp1"}] == 1 );
        REQUIRE( m.map[A{20, "mp2"}] == 2 );
    }

    SECTION( "Serialize std::map<int, A::Ptr> smart-pointers" )
    {
        M<std::map<int, A::Ptr>> m;
        m.map[1] = A::Ptr(new A{10, "mp1"});
        m.map[2] = A::Ptr(new A{20, "mp2"});
        m.map[3] = {};

        QByteArray json = m.toJson();
        REQUIRE( json == R"({"map":[{"k":1,"v":{"p1":10,"p2":"mp1"}},{"k":2,"v":{"p1":20,"p2":"mp2"}},{"k":3,"v":null}]})" );
    }

    SECTION( "Deserialize std::map<int, A::Ptr> smart-pointers" )
    {
        QByteArray json = R"({"map":[{"k":1,"v":{"p1":10,"p2":"mp1"}},{"k":2,"v":{"p1":20,"p2":"mp2"}},{"k":3,"v":null}]})";

        M<std::map<int, A::Ptr>> m;
        pproto::SResult sr = m.fromJson(json);

        ALOG_FLUSH();
        REQUIRE( bool(sr) == true );

        REQUIRE( m.map[1].empty() == false );
        REQUIRE( m.map[2].empty() == false );
        REQUIRE( m.map[3].empty() == true  );

        if (!m.map[1])
            REQUIRE( *m.map[1] == A{10, "mp1"} );

        if (!m.map[2])
            REQUIRE( *m.map[2] == A{20, "mp2"} );
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

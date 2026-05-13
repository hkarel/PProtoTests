import qbs
import "pproto_test_base.qbs" as PProtoTest

PProtoTest {
    name: "PProtoTest"

    references: [
        "src/catch2/catch2.qbs",
        "src/pproto/pproto.qbs",
        "src/rapidjson/rapidjson.qbs",
        "src/shared/shared.qbs",
        "src/tests/tests.qbs",
    ]
}

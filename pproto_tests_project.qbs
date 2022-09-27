import qbs
import "pproto_tests_base.qbs" as PProtoDemoTests

PProtoDemoTests {
    name: "PProtoTests (Project)"

    references: [
        "src/catch2/catch2.qbs",
        "src/pproto/pproto.qbs",
        "src/rapidjson/rapidjson.qbs",
        "src/shared/shared.qbs",
    ]
}

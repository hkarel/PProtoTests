import qbs

Project {
    name: "Tests"

    Product {
        name: "LogSaver"
        targetName: "logsaver"

        type: "staticlibrary"

        Depends { name: "cpp" }
        Depends { name: "Catch2" }
        Depends { name: "SharedLib" }

        cpp.defines: project.cppDefines
        cpp.cxxFlags: project.cxxFlags
        cpp.cxxLanguageVersion: project.cxxLanguageVersion

        property var includePaths: [
            "./",
        ]
        cpp.includePaths: includePaths;

        files: [
            "catch2/log_saver.cpp",
            "catch2/log_saver.h",
        ]

        Export {
            Depends { name: "cpp" }
            cpp.includePaths: exportingProduct.includePaths
        }
    }

    references: [
        "serialize/serialize.qbs",
    ]

    AutotestRunner { }
}

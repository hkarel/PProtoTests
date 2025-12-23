import qbs
import qbs.FileInfo

Product {
    name: "Catch2"
    targetName: "catch2"

    type: "staticlibrary"

    Depends { name: "cpp" }

    cpp.defines: project.cppDefines
    cpp.cxxFlags: project.cxxFlags //.concat(["-fPIC"])
    cpp.cxxLanguageVersion: project.cxxLanguageVersion

    cpp.includePaths: [
        "catch2/src",
        project.buildDirectory,
    ]

    Group {
        name: "benchmark"
        prefix: "catch2/src/catch2/benchmark/"
        files: [
            "**/*.cpp",
            "**/*.hpp",
        ]
    }
    Group {
        name: "generators"
        prefix: "catch2/src/catch2/generators/"
        files: [
            "*.cpp",
            "*.hpp",
        ]
    }
    Group {
        name: "interfaces"
        prefix: "catch2/src/catch2/interfaces/"
        files: [
            "*.cpp",
            "*.hpp",
        ]
    }
    Group {
        name: "internal"
        prefix: "catch2/src/catch2/internal/"
        files: [
            "*.cpp",
            "*.hpp",
        ]
    }
    Group {
        name: "matchers"
        prefix: "catch2/src/catch2/matchers/"
        files: [
            "**/*.cpp",
            "**/*.hpp",
        ]
    }
    Group {
        name: "reporters"
        prefix: "catch2/src/catch2/reporters/"
        files: [
            "*.cpp",
            "*.hpp",
        ]
    }
//    Group {
//        name: "othen"
//        prefix: "catch2/src/catch2"
//        files: [
//            "*.cpp",
//            "*.hpp",
//        ]
//    }

    files: [
      "catch2/src/catch2/*.cpp",
      "catch2/src/catch2/*.hpp",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            FileInfo.joinPaths(exportingProduct.sourceDirectory, "catch2/src"),
            project.buildDirectory
        ]
    }
}

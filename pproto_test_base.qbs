import qbs
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "qbs/imports/QbsUtl/qbsutl.js" as QbsUtl

Project {
    minimumQbsVersion: "1.23.0"
    qbsSearchPaths: ["qbs"]

    property bool useSodium: false       // Использовать собранную libsodium (директория расположения /opt/sodium/1.0.18)
    property bool useSystemSodium: false // Использовать системную libsodium
    property string sodiumVersion: "1.0.18"

    Probe {
        id: projectProbe

        readonly property string projectBuildDirectory:  project.buildDirectory
        readonly property string projectSourceDirectory: project.sourceDirectory

        configure: {
            File.makePath(projectBuildDirectory + "/catch2");

            var conffile = new TextFile(projectBuildDirectory + "/catch2/catch_user_config.hpp", TextFile.WriteOnly);
            conffile.write("// Dummy file, required for compatibility with CMAKE build system\n");
            conffile.close();
        }
    }

    property var cppDefines: {
        var def = [
            "QDATASTREAM_VERSION=QDataStream::Qt_5_12",
          //"QDATASTREAM_BYTEORDER=QDataStream::LittleEndian",
            "PPROTO_VERSION_LOW=0",
            "PPROTO_VERSION_HIGH=0",
            "PPROTO_JSON_SERIALIZE",
            "PPROTO_QBINARY_SERIALIZE",
            "PPROTO_UDP_SIGNATURE=\"PPTS\"",
            //--- Catch2 ---
            "CATCH_CONFIG_COUNTER",
            "CATCH_CONFIG_CPP11_TO_STRING",
            "CATCH_CONFIG_CPP17_BYTE",
            "CATCH_CONFIG_CPP17_OPTIONAL",
            "CATCH_CONFIG_CPP17_STRING_VIEW",
            "CATCH_CONFIG_CPP17_UNCAUGHT_EXCEPTIONS",
            "CATCH_CONFIG_CPP17_VARIANT",
            "CATCH_CONFIG_GLOBAL_NEXTAFTER",
            "CATCH_CONFIG_POSIX_SIGNALS",
          //"CATCH_CONFIG_USE_ASYNC", ??
            "CATCH_CONFIG_NO_WCHAR",
          //"CATCH_CONFIG_PREFIX_ALL", ??
        ];

        if (useSodium === true || useSystemSodium === true)
            def.push("SODIUM_ENCRYPTION");

        return def;
    }

    property var cxxFlags: [
        "-ggdb3",
        "-Wall",
        "-Wextra",
        "-Wno-unused-parameter",
        "-Wno-variadic-macros",
        "-Wno-register",
    ]
    property string cxxLanguageVersion: "c++17"
}

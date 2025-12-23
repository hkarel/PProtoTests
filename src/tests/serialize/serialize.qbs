import qbs
import "serialize_base.qbs" as SerializeBase

Project {
    name: "Serialize"

    SerializeBase {
        name: "Json 01"
        targetName: "json01"
        condition: true

        files: [
            "json/json01.cpp",
        ]
    }
    SerializeBase {
        name: "Json 02"
        targetName: "json02"
        condition: true

        files: [
            "json/json02.cpp",
        ]
    }
    SerializeBase {
        name: "Json 03"
        targetName: "json03"
        condition: true

        files: [
            "json/json03.cpp",
        ]
    }
    SerializeBase {
        name: "Json 04"
        targetName: "json04"
        condition: true

        files: [
            "json/json04.cpp",
        ]
    }
    SerializeBase {
        name: "Json 05"
        targetName: "json05"
        condition: true

        files: [
            "json/json05.cpp",
        ]
    }
    SerializeBase {
        name: "Json 06"
        targetName: "json06"
        condition: true

        files: [
            "json/json06.cpp",
        ]
    }
}

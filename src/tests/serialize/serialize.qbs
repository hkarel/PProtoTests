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
}

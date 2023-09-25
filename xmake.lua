set_project("mcu")

rule("mcu")
    on_config(function (target) 
        target:add("includedirs", "include")
    end)

rule("map")
    on_config(function (target) 
        local mapfile = format("%s/%s.map", target:targetdir(), target:name())
        target:add("ldflags", format("-Wl,-Map,%s", mapfile))
    end)

target("mcu")
    set_kind("static")
    add_rules("mcu")
    add_files("src/time.c")
    add_files("src/debug.c")
    add_files("src/routine.c")
    add_files("src/manifest.c")
 

target("example")
    set_kind("binary")
    add_deps("mcu")
    add_rules("mcu")
    add_files("example/*.c")
    add_rules("map")

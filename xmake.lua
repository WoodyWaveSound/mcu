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
    add_cxflags("-Wall")
    
    add_files("src/time.c")
    add_files("src/debug.c")

    add_files("src/byte.c")
    add_files("src/data.c")
    add_files("src/countdown.c")
    add_files("src/memory.c")
    add_files("src/manifest.c")

    add_files("src/service.c")
    add_files("src/tick.c")

    add_files("src/i2c.c")
    add_files("src/spi.c")
    
    add_files("src/state_machine.c") 
    add_files("src/cli.c")
    add_files("src/database.c")
    add_files("src/logic_filter.c")
    add_files("src/button.c")

    add_files("src/aw9523b.c")
    add_files("src/encoder.c")
    add_files("src/eeprom.c")
    add_files("src/ir.c")

target("example")
    set_kind("binary")
    add_deps("mcu")
    add_rules("mcu")
    add_files("example/*.c")
    add_rules("map")
    add_cxflags("-Wall")

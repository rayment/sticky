function(target_populate_linker_features TARGET)
    target_link_options(${TARGET} PRIVATE
        # Security/hardening
        $<$<PLATFORM_ID:Linux>:
            -Wl,-z,relro # read-only segments after relocation
            -Wl,-z,now # disable lazy binding
            -Wl,-z,noexecstack # mark stack as non-executable
            -Wl,--no-undefined # fail link if unresolved symbols are present
        >
        $<$<PLATFORM_ID:Windows>:
            -Wl,--dynamicbase # enable ASLR
            -Wl,--enable-reloc-section # ensure relocation section is present
            -Wl,--nxcompat # mark compatible with Windows Data Execution Prevention
            -Wl,--high-entropy-va # enable high-entropy 64-bit ASLR support
        >
    )
endfunction()

function(target_export_all_symbols TARGET)
    if(WIN32)
        target_link_options(${TARGET} PRIVATE -fno-use-linker-plugin)
    endif()
endfunction()

function(target_populate_linker_features TARGET)
    target_link_options(${TARGET} PRIVATE
        # Security/hardening
        $<$<PLATFORM_ID:Linux>:
            -Wl,-z,relro # read-only segments after relocation
            -Wl,-z,now # disable lazy binding
        >
        $<$<PLATFORM_ID:Windows>:
            -Wl,--dynamicbase # enable ASLR
            -Wl,--nxcompat # mark compatible with Windows Data Execution Prevention
            -Wl,--high-entropy-va # enable high-entropy 64-bit ASLR support
        >
    )
endfunction()

/*
 * test_runtime.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>

#include <doctest/doctest.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/spdlog.h>

#include "io/log.h"
#include "lua/runtime.h"
#include "symlink.h"

namespace
{
    namespace fs = std::filesystem;

    const fs::path res_dir = fs::path(ST_TEST_RES_DIR);
    const fs::path lua_dir = res_dir / "0008-lua";

    void capture_into(const std::string  &logger_name,
                      std::ostringstream &output)
    {
        spdlog::get(logger_name)->sinks().push_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(output));
    }

    struct bad_script
    {
        const st::char_t *file;
        const st::char_t *message;
    };

    struct bad_function
    {
        const st::char_t *name;
        const st::char_t *message;
    };

    // Runtime errors are reported through the default logger and Lua output
    // through the runtime's own, so both are redirected into one buffer.
    struct runtime_fixture
    {
        std::ostringstream              output;
        std::shared_ptr<spdlog::logger> previous_default = spdlog::default_logger();
        st::io::log::logger             default_logger{"test_lua_default"};
        st::lua::runtime                runtime{"test_lua", lua_dir};

        runtime_fixture()
        {
            capture_into("test_lua_default", output);
            capture_into("test_lua", output);
            default_logger.set_default();
        }

        ~runtime_fixture() { spdlog::set_default_logger(previous_default); }

        ST_DELETE_COPY_CTORS(runtime_fixture);
        ST_DELETE_MOVE_CTORS(runtime_fixture);

        [[nodiscard]] st::bool_t contains(std::string_view text) const
        {
            return output.str().find(text) != std::string::npos;
        }

        void clear_output() { output.str(""); }

        void check_script_fails(const bad_script &script)
        {
            INFO(script.file);
            clear_output();

            CHECK_FALSE(runtime.execute_file(script.file));
            CHECK(contains("[error]"));
            CHECK(contains(script.message));
        }

        void check_function_fails(const bad_function &function)
        {
            INFO(function.name);
            clear_output();

            CHECK_FALSE(runtime.execute_function(function.name));
            CHECK(contains("[error]"));
            CHECK(contains(function.message));
        }
    };

    // An empty message only checks that something was reported.
    const bad_script bad_scripts[] = {
        {"0101-bad-syntax.lua", "unexpected symbol"},
        {"0102-bad-runtime-error.lua", "runtime failure"},
        {"0103-bad-error-table.lua", "error object is a table value"},
        {"0104-bad-nil-call.lua", "undefined_function"},
        {"0105-bad-assert.lua", "assertion message"},
        {"0106-bad-stack-overflow.lua", "stack overflow"},
        {"0107-bad-require-missing.lua", "could not be resolved"},
        {"0108-bad-require-syntax.lua", "expected"},
        {"0109-bad-require-error.lua", "module failure"},
        {"0110-bad-require-cycle.lua", ""},
        {"0111-bad-require-invalid.lua", "invalid module name"},
        {"0112-bad-not-lua.txt", ""},
        {"0113-bad-bytecode.luac", "binary chunk"},
        {"0114-bad-error-meta.lua", ""},
        {"0115-bad-error-in-coroutine.lua", "inside coroutine"},
        {"0116-bad-out-of-memory.lua", ""},
        {"0117-bad-valid-bytecode.luac", "binary chunk"},
        {"0118-bad-require-bytecode.lua", "binary chunk"},
    };

    const bad_function bad_functions[] = {
        {"on_error", "boom from on_error"},
        {"on_error_table", "error object is a table value"},
        {"on_error_meta", "custom error object"},
        {"on_error_nil", "error object is a nil value"},
        {"on_overflow", "stack overflow"},
        {"on_yield", "outside a coroutine"},
        {"on_bad_arith", "attempt to perform arithmetic"},
        {"on_print_error", "tostring failed"},
        {"on_print_bad_tostring", "must return a string"},
    };

    // Builds a throwaway runtime root containing symlinks that point out of it.
    struct link_fixture
    {
        fs::path root = fs::temp_directory_path() / "sticky_test_lua";

        fs::path inside = root / "inside.lua";
        fs::path inside_link = root / "inside_link.lua";
        fs::path escape_link = root / "escape_link.lua";
        fs::path escape_dir_link = root / "escape_dir";
        fs::path module_link = root / "mod_escape.lua";
        fs::path require_escape = root / "require_escape.lua";

        st::bool_t  symlinks_available = false;
        std::string symlink_error;

        link_fixture()
        {
            std::error_code ec;
            cleanup();
            fs::create_directories(root, ec);

            std::ofstream{inside} << "x = 1\n";
            std::ofstream{require_escape} << "require(\"mod_escape\")\n";

            st::test::make_symlink(inside, inside_link, false, ec);
            if (ec)
            {
                symlink_error = ec.message();
                return;
            }

            symlinks_available = true;
            st::test::make_symlink(fs::canonical(lua_dir / "0002-good-print.lua"), escape_link, false, ec);
            st::test::make_symlink(fs::canonical(lua_dir), escape_dir_link, true, ec);
            st::test::make_symlink(fs::canonical(lua_dir / "mod_good.lua"), module_link, false, ec);
        }

        ~link_fixture() { cleanup(); }

        // remove_all must never run here: some standard libraries follow the
        // directory link and wipe the real resources it points at.
        void cleanup() const
        {
            for (const fs::path *link : {&inside_link, &escape_link, &escape_dir_link, &module_link})
                st::test::remove_link(*link);

            std::error_code ec;
            fs::remove(inside, ec);
            fs::remove(require_escape, ec);
            fs::remove(root, ec);
        }

        link_fixture(const link_fixture &) = delete;
        link_fixture &operator=(const link_fixture &) = delete;
    };
} // namespace

#define SKIP_WITHOUT_SYMLINKS()                                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!symlinks_available)                                                                                       \
        {                                                                                                              \
            MESSAGE("skipped, cannot create symlinks: ", symlink_error);                                               \
            return;                                                                                                    \
        }                                                                                                              \
    }                                                                                                                  \
    while (false)

TEST_SUITE("lua::runtime")
{
    TEST_CASE_FIXTURE(runtime_fixture, "execute_file runs an empty script")
    {
        CHECK(runtime.execute_file("0001-good-empty.lua"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_file runs a script and print reaches the log")
    {
        CHECK(runtime.execute_file("0002-good-print.lua"));

        CHECK(contains("[test_lua]"));
        CHECK(contains("[info] hello from lua"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_file accepts relative and absolute paths inside the root")
    {
        CHECK(runtime.execute_file("0002-good-print.lua"));
        CHECK(runtime.execute_file("./0002-good-print.lua"));
        CHECK(runtime.execute_file("sub/0001-good-nested.lua"));
        CHECK(runtime.execute_file("sub/../0002-good-print.lua"));
        CHECK(runtime.execute_file("../0008-lua/0002-good-print.lua"));
        CHECK(runtime.execute_file(lua_dir / "0002-good-print.lua"));
        CHECK(runtime.execute_file(lua_dir / "sub" / "0001-good-nested.lua"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_file rejects paths that are not files inside the root")
    {
        CHECK_FALSE(runtime.execute_file("0099-bad-missing.lua"));
        CHECK_FALSE(runtime.execute_file(""));
        CHECK_FALSE(runtime.execute_file("."));
        CHECK_FALSE(runtime.execute_file("sub"));
        CHECK_FALSE(runtime.execute_file("../0002-good-text.txt"));
        CHECK_FALSE(runtime.execute_file("sub/../../0002-good-text.txt"));
        CHECK_FALSE(runtime.execute_file(res_dir));
        CHECK_FALSE(runtime.execute_file(res_dir / "0002-good-text.txt"));
        CHECK_FALSE(runtime.execute_file(lua_dir.root_path() / "0099-bad-missing.lua"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_file does not run a rejected script")
    {
        CHECK_FALSE(runtime.execute_file("../0099-bad-missing.lua"));
        CHECK_FALSE(contains("[error]"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_file can run the same script repeatedly")
    {
        for (st::int32 i = 0; i < 3; ++i)
            CHECK(runtime.execute_file("0002-good-print.lua"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_file reports failures and keeps the runtime usable")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));

        for (const bad_script &script : bad_scripts)
            check_script_fails(script);

        clear_output();
        CHECK(runtime.execute_file("0002-good-print.lua"));
        CHECK(runtime.execute_function("on_counter"));
        CHECK(contains("counter 1"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "a runtime error includes a stack traceback")
    {
        CHECK_FALSE(runtime.execute_file("0102-bad-runtime-error.lua"));

        CHECK(contains("before error"));
        CHECK(contains("runtime failure"));
        CHECK(contains("stack traceback"));
        CHECK_FALSE(contains("after error"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "the standard libraries work")
    {
        CHECK(runtime.execute_file("0004-good-stdlib.lua"));
        CHECK(contains("stdlib ok"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "unsafe globals are removed")
    {
        CHECK(runtime.execute_file("0005-good-sandbox.lua"));
        CHECK(contains("sandbox ok"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "non-ASCII source and output survive")
    {
        const std::string text = "h\xc3\xa9llo \xe2\x9c\x93 \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e";

        CHECK(runtime.execute_file("0008-good-utf8.lua"));
        CHECK(contains(text));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "scripts cannot break print or the runtime by overwriting globals")
    {
        CHECK(runtime.execute_file("0009-good-tamper.lua"));
        CHECK(contains("print survives tampering"));

        CHECK(runtime.execute_function("on_tampered"));
        CHECK(contains("function survives tampering"));

        CHECK_FALSE(runtime.execute_file("0102-bad-runtime-error.lua"));
        CHECK(contains("runtime failure"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "print joins its arguments with tabs")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));
        CHECK(runtime.execute_function("on_print_values"));

        CHECK(contains("[info] a\t1\t2.5\ttrue\tnil"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "print with no arguments logs an empty line")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));
        CHECK(runtime.execute_function("on_print_empty"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "print honours __tostring")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));
        CHECK(runtime.execute_function("on_print_meta"));

        CHECK(contains("custom tostring"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "require loads cached modules from the root")
    {
        CHECK(runtime.execute_file("0006-good-require.lua"));
        CHECK(contains("require ok"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "require rejects invalid and unresolvable module names")
    {
        CHECK(runtime.execute_file("0007-good-require-invalid.lua"));
        CHECK(contains("require invalid ok"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_function calls a global function")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));
        CHECK(runtime.execute_function("on_start"));

        CHECK(contains("on_start called"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_function keeps state between calls")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));

        CHECK(runtime.execute_function("on_counter"));
        CHECK(runtime.execute_function("on_counter"));
        CHECK(runtime.execute_function("on_counter"));

        CHECK(contains("counter 3"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_function converts every argument type")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));

        CHECK(runtime.execute_function(
            "on_args",
            {st::int64{42}, st::float64{1.5}, std::string_view{"text"}, st::bool_t{true}, std::monostate{}}));

        CHECK(contains("42\t1.5\ttext\ttrue"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_function passes floats such as delta time")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));
        CHECK(runtime.execute_function("on_step", {st::float64{0.25}}));

        CHECK(contains("on_step 0.25"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_function fails when the script rejects an argument")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));

        CHECK_FALSE(runtime.execute_function("on_step", {st::int64{1}}));
        CHECK(contains("dt must be a float"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_function passes nil arguments positionally")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));

        CHECK(runtime.execute_function("on_arg_count"));
        CHECK(contains("arg_count 0"));

        CHECK(runtime.execute_function("on_arg_count", {st::int64{1}, std::monostate{}, st::int64{3}}));
        CHECK(contains("arg_count 3"));

        CHECK(runtime.execute_function("on_arg_count", {std::monostate{}, std::monostate{}}));
        CHECK(contains("arg_count 2"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_function keeps string length with embedded NULs")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));

        CHECK(runtime.execute_function("on_string_length", {std::string_view{"a\0b", 3}}));
        CHECK(contains("string_length 3"));

        CHECK(runtime.execute_function("on_string_length", {std::string_view{}}));
        CHECK(contains("string_length 0"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_function fails for functions that do not exist")
    {
        CHECK_FALSE(runtime.execute_function("on_start"));

        CHECK(runtime.execute_file("0003-good-functions.lua"));

        CHECK_FALSE(runtime.execute_function("on_missing"));
        CHECK_FALSE(runtime.execute_function(""));
        CHECK_FALSE(runtime.execute_function("not_a_function"));
        CHECK_FALSE(runtime.execute_function("tbl"));
        CHECK_FALSE(runtime.execute_function("tbl.fn"));
        CHECK_FALSE(runtime.execute_function("on_start()"));
        CHECK_FALSE(runtime.execute_function("on_missing", {st::int64{1}}));
        CHECK_FALSE(contains("[error]"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_function reports errors and keeps the runtime usable")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));

        for (const bad_function &function : bad_functions)
            check_function_fails(function);

        clear_output();
        CHECK(runtime.execute_function("on_counter"));
        CHECK(contains("counter 1"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_function survives many successful and failing calls")
    {
        CHECK(runtime.execute_file("0003-good-functions.lua"));

        for (st::int32 i = 0; i < 1000; ++i)
            CHECK(runtime.execute_function("on_noop"));
        for (st::int32 i = 0; i < 100; ++i)
            CHECK_FALSE(runtime.execute_function("on_error"));
        for (st::int32 i = 0; i < 1000; ++i)
            CHECK_FALSE(runtime.execute_function("on_missing"));

        CHECK(runtime.execute_function("on_noop"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "a mod lifecycle follows the order used by the engine")
    {
        CHECK(runtime.execute_file("main.lua"));

        CHECK(runtime.execute_function("on_start"));
        CHECK(runtime.execute_function("on_fixed_step", {st::float64{0.01}}));
        CHECK(runtime.execute_function("on_step", {st::float64{0.016}}));
        CHECK_FALSE(runtime.execute_function("on_gui_render"));
        CHECK(runtime.execute_function("on_quit"));

        CHECK(contains("main start 42"));
        CHECK(contains("main fixed_step"));
        CHECK(contains("main step"));
        CHECK(contains("main quit"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_string runs code")
    {
        CHECK(runtime.execute_string("print('string ran')"));
        CHECK(contains("[info] string ran"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_string accepts empty and comment-only code")
    {
        CHECK(runtime.execute_string(""));
        CHECK(runtime.execute_string("-- nothing"));
        CHECK(runtime.execute_string(std::string_view{}));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_string shares state with files and functions")
    {
        CHECK(runtime.execute_string("shared = 5\nfunction on_string() print('shared ' .. shared) end"));
        CHECK(runtime.execute_function("on_string"));
        CHECK(contains("shared 5"));

        CHECK(runtime.execute_file("0003-good-functions.lua"));
        CHECK(runtime.execute_string("on_counter()"));
        CHECK(contains("counter 1"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_string reports errors and keeps the runtime usable")
    {
        const std::string_view bad_code[] = {
            "local x = = 1",
            "error('string failure')",
            "error({ code = 1 })",
            "undefined_function()",
            "assert(false, 'string assertion')",
            "local function f() return 1 + f() end f()",
            "return require('does_not_exist')",
        };

        for (const std::string_view code : bad_code)
        {
            INFO(code);
            clear_output();

            CHECK_FALSE(runtime.execute_string(code));
            CHECK(contains("[error]"));
        }

        clear_output();
        CHECK(runtime.execute_string("print('still alive')"));
        CHECK(contains("still alive"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_string error messages name the string chunk")
    {
        CHECK_FALSE(runtime.execute_string("error('string failure')"));

        CHECK(contains("(string)"));
        CHECK(contains("string failure"));
        CHECK(contains("stack traceback"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_string rejects precompiled bytecode")
    {
        CHECK_FALSE(runtime.execute_string(std::string_view{"\x1bLua\x55\x00garbage", 13}));
        CHECK(contains("[error]"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "valid bytecode is rejected and never executed")
    {
        CHECK_FALSE(runtime.execute_file("0117-bad-valid-bytecode.luac"));
        CHECK(contains("attempt to load a binary chunk"));
        CHECK_FALSE(contains("bytecode executed"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "valid bytecode is rejected as a required module")
    {
        CHECK_FALSE(runtime.execute_file("0118-bad-require-bytecode.lua"));
        CHECK(contains("attempt to load a binary chunk"));

        CHECK(runtime.execute_string("assert(bytecode_module_ran == nil)"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "a metatable on the globals cannot crash execute_function")
    {
        CHECK(runtime.execute_file("0010-good-global-metatable.lua"));

        CHECK(runtime.execute_function("on_after_metatable"));
        CHECK(contains("function found despite metatable"));

        clear_output();
        CHECK_FALSE(runtime.execute_function("on_missing"));
        CHECK(contains("global lookup of on_missing"));

        clear_output();
        CHECK(runtime.execute_function("on_after_metatable"));
        CHECK(contains("function found despite metatable"));
    }

    TEST_CASE_FIXTURE(runtime_fixture, "execute_string only runs the given length of its buffer")
    {
        const std::string_view code = "print('first') print('second')";

        CHECK(runtime.execute_string(code.substr(0, 14)));
        CHECK(contains("first"));
        CHECK_FALSE(contains("second"));
    }

    TEST_CASE("destroying a runtime closes the Lua state and runs finalizers")
    {
        std::ostringstream output;
        {
            st::lua::runtime runtime{"test_lua_close", lua_dir};
            capture_into("test_lua_close", output);

            CHECK(runtime.execute_file("0011-good-finalizers.lua"));
            CHECK_EQ(std::string::npos, output.str().find("finalizer ran"));
        }

        CHECK_NE(std::string::npos, output.str().find("finalizer ran"));
        CHECK_NE(std::string::npos, output.str().find("second finalizer ran"));
    }

    TEST_CASE("destroying a runtime does not disturb another runtime's logging")
    {
        std::ostringstream output;
        st::lua::runtime   survivor{"test_lua_survivor_log", lua_dir};
        capture_into("test_lua_survivor_log", output);
        {
            st::lua::runtime short_lived{"test_lua_short_log", lua_dir};
            CHECK(short_lived.execute_file("0011-good-finalizers.lua"));
        }

        CHECK(survivor.execute_function("on_missing") == false);
        CHECK(survivor.execute_string("print('survivor logged')"));
        CHECK_NE(std::string::npos, output.str().find("survivor logged"));
        CHECK_EQ(std::string::npos, output.str().find("finalizer ran"));
    }

    TEST_CASE("runtimes keep globals and log output separate")
    {
        std::ostringstream output_a;
        std::ostringstream output_b;
        st::lua::runtime   a{"test_lua_a", lua_dir};
        st::lua::runtime   b{"test_lua_b", lua_dir};
        capture_into("test_lua_a", output_a);
        capture_into("test_lua_b", output_b);

        CHECK(a.execute_file("0003-good-functions.lua"));
        CHECK(a.execute_function("on_noop"));
        CHECK_FALSE(b.execute_function("on_noop"));

        CHECK(b.execute_file("0003-good-functions.lua"));
        CHECK(a.execute_function("on_counter"));
        CHECK(a.execute_function("on_counter"));
        CHECK(b.execute_function("on_counter"));

        const std::string text_a = output_a.str();
        const std::string text_b = output_b.str();
        CHECK_NE(std::string::npos, text_a.find("counter 2"));
        CHECK_EQ(std::string::npos, text_a.find("[test_lua_b]"));
        CHECK_NE(std::string::npos, text_b.find("counter 1"));
        CHECK_EQ(std::string::npos, text_b.find("counter 2"));
    }

    TEST_CASE("a runtime keeps working after another runtime is destroyed")
    {
        std::ostringstream output;
        st::lua::runtime   survivor{"test_lua_survivor", lua_dir};
        capture_into("test_lua_survivor", output);
        {
            st::lua::runtime short_lived{"test_lua_short", lua_dir};
            CHECK(short_lived.execute_file("0002-good-print.lua"));
        }

        CHECK(survivor.execute_file("0002-good-print.lua"));
        CHECK_NE(std::string::npos, output.str().find("hello from lua"));
    }

    TEST_CASE_FIXTURE(link_fixture, "execute_file follows symlinks that stay inside the root")
    {
        SKIP_WITHOUT_SYMLINKS();

        st::lua::runtime runtime{"test_lua_link", root};

        CHECK(runtime.execute_file("inside.lua"));
        CHECK(runtime.execute_file("inside_link.lua"));
    }

    TEST_CASE_FIXTURE(link_fixture, "a root that is itself a symlink resolves to its target")
    {
        SKIP_WITHOUT_SYMLINKS();

        st::lua::runtime runtime{"test_lua_link_root", escape_dir_link};

        CHECK(runtime.execute_file("0002-good-print.lua"));
        CHECK(runtime.execute_file("main.lua"));
    }

    TEST_CASE_FIXTURE(link_fixture, "execute_file rejects file symlinks that escape the root")
    {
        SKIP_WITHOUT_SYMLINKS();

        CHECK(fs::exists(escape_link));

        st::lua::runtime runtime{"test_lua_link", root};

        CHECK_FALSE(runtime.execute_file("escape_link.lua"));
        CHECK_FALSE(runtime.execute_file(root / "escape_link.lua"));
    }

    TEST_CASE_FIXTURE(link_fixture, "execute_file rejects directory symlinks that escape the root")
    {
        SKIP_WITHOUT_SYMLINKS();

        CHECK(fs::exists(escape_dir_link / "0002-good-print.lua"));

        st::lua::runtime runtime{"test_lua_link", root};

        CHECK_FALSE(runtime.execute_file("escape_dir/0002-good-print.lua"));
        CHECK_FALSE(runtime.execute_file(root / "escape_dir" / "0002-good-print.lua"));
    }

    TEST_CASE_FIXTURE(link_fixture, "require rejects modules reached through symlinks that escape the root")
    {
        SKIP_WITHOUT_SYMLINKS();

        CHECK(fs::exists(module_link));

        st::lua::runtime runtime{"test_lua_link", root};

        CHECK_FALSE(runtime.execute_file("require_escape.lua"));
        CHECK_FALSE(runtime.execute_string("require('mod_escape')"));
        CHECK_FALSE(runtime.execute_string("require('escape_dir.mod_good')"));
    }

    TEST_CASE("a runtime with an unresolvable root fails to load anything")
    {
        st::lua::runtime runtime{"test_lua_no_root", res_dir / "0099-bad-missing-dir"};

        CHECK_FALSE(runtime.execute_file("0002-good-print.lua"));
        CHECK_FALSE(runtime.execute_file(lua_dir / "0002-good-print.lua"));
        CHECK_FALSE(runtime.execute_string("require('mod_good')"));
        CHECK(runtime.execute_string("x = 1"));
    }
}

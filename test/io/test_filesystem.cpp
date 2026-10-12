/*
 * test_filesystem.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#include <doctest/doctest.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "io/filesystem.h"

namespace
{
    namespace fs = std::filesystem;
    namespace stfs = st::io::filesystem;

    const fs::path res_dir = fs::path(ST_TEST_RES_DIR);

    const fs::path good_empty = res_dir / "0001-good-empty.txt";
    const fs::path good_text = res_dir / "0002-good-text.txt";
    const fs::path good_multi = res_dir / "0003-good-multi.tar.gz";
    const fs::path good_noext = res_dir / "0004-good-noext";
    const fs::path good_tree = res_dir / "0005-good-tree";
    const fs::path tree_file_a = good_tree / "0001-good-file.txt";
    const fs::path tree_file_b = good_tree / "0002-good-file.dat";
    const fs::path tree_sub_a = good_tree / "0003-good-subdir";
    const fs::path tree_sub_b = good_tree / "0004-good-subdir";

    // Never created on disk.
    const fs::path bad_missing = res_dir / "0099-bad-missing.txt";

    [[nodiscard]] std::vector<std::string> sorted(std::vector<std::string> values)
    {
        std::sort(values.begin(), values.end());
        return values;
    }

    [[nodiscard]] std::vector<std::string> sorted_paths(const std::vector<fs::path> &paths)
    {
        std::vector<std::string> values;
        values.reserve(paths.size());
        for (const fs::path &path : paths)
            values.push_back(path.string());
        return sorted(std::move(values));
    }

    void check_invalid_entry(const fs::path &path)
    {
        CHECK_FALSE(stfs::exists(path));
        CHECK_FALSE(stfs::is_file(path));
        CHECK_FALSE(stfs::is_directory(path));
    }

    // libstdc++ on MinGW doesn't implement create_symlink.
    void make_symlink(const fs::path  &target,
                      const fs::path  &link,
                      bool             directory,
                      std::error_code &ec)
    {
#ifdef _WIN32
        DWORD flags = SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;
        if (directory)
            flags |= SYMBOLIC_LINK_FLAG_DIRECTORY;
        if (CreateSymbolicLinkW(link.c_str(), target.c_str(), flags) != 0)
            ec.clear();
        else
            ec.assign(static_cast<int>(GetLastError()), std::system_category());
#else
        if (directory)
            fs::create_directory_symlink(target, link, ec);
        else
            fs::create_symlink(target, link, ec);
#endif
    }

    void remove_link(const fs::path &link)
    {
#ifdef _WIN32
        if (DeleteFileW(link.c_str()) == 0)
            RemoveDirectoryW(link.c_str());
#else
        std::error_code ec;
        fs::remove(link, ec);
#endif
    }

    // Symlinks can't be stored in the repository on every platform, so they
    // are built in a scratch directory at runtime and point into test/res.
    struct link_fixture
    {
        fs::path root = fs::temp_directory_path() / "sticky_test_filesystem";

        fs::path real_file = root / "0001-good-real.txt";
        fs::path real_dir = root / "0002-good-real-dir";
        fs::path file_link = root / "0003-good-file-link.txt";
        fs::path dir_link = root / "0004-good-dir-link";
        fs::path broken = root / "0005-bad-broken-link.txt";
        fs::path loop_a = root / "0006-bad-loop-a";
        fs::path loop_b = root / "0007-bad-loop-b";

        bool        symlinks_available = false;
        std::string symlink_error;

        link_fixture()
        {
            std::error_code ec;
            cleanup();
            fs::create_directories(root, ec);
            fs::create_directory(real_dir, ec);
            std::ofstream{real_file}.close();

            make_symlink(good_text, file_link, false, ec);
            if (ec)
            {
                symlink_error = ec.message();
                return;
            }

            symlinks_available = true;
            make_symlink(good_tree, dir_link, true, ec);
            make_symlink(root / "0099-missing.txt", broken, false, ec);
            make_symlink(loop_b.filename(), loop_a, false, ec);
            make_symlink(loop_a.filename(), loop_b, false, ec);
        }

        ~link_fixture() { cleanup(); }

        // remove_all must never run here: some standard libraries follow the
        // directory link and wipe the real resources it points at.
        void cleanup() const
        {
            for (const fs::path *link : {&file_link, &dir_link, &broken, &loop_a, &loop_b})
                remove_link(*link);

            std::error_code ec;
            fs::remove(real_file, ec);
            fs::remove(real_dir, ec);
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

TEST_SUITE("io::filesystem")
{
    TEST_CASE("exists is true for files and directories")
    {
        CHECK(stfs::exists(good_empty));
        CHECK(stfs::exists(good_text));
        CHECK(stfs::exists(good_multi));
        CHECK(stfs::exists(good_noext));
        CHECK(stfs::exists(good_tree));
        CHECK(stfs::exists(tree_sub_a));
    }

    TEST_CASE("exists is false for missing paths")
    {
        CHECK_FALSE(stfs::exists(bad_missing));
        CHECK_FALSE(stfs::exists(good_tree / "0099-bad-missing"));
        CHECK_FALSE(stfs::exists(good_text / "0099-bad-below-file"));
        CHECK_FALSE(stfs::exists(""));
    }

    TEST_CASE("is_file is true only for regular files")
    {
        CHECK(stfs::is_file(good_empty));
        CHECK(stfs::is_file(good_text));
        CHECK(stfs::is_file(good_multi));
        CHECK(stfs::is_file(good_noext));
        CHECK(stfs::is_file(tree_file_a));

        CHECK_FALSE(stfs::is_file(good_tree));
        CHECK_FALSE(stfs::is_file(bad_missing));
        CHECK_FALSE(stfs::is_file(""));
    }

    TEST_CASE("is_directory is true only for directories")
    {
        CHECK(stfs::is_directory(res_dir));
        CHECK(stfs::is_directory(good_tree));
        CHECK(stfs::is_directory(tree_sub_a));

        CHECK_FALSE(stfs::is_directory(good_empty));
        CHECK_FALSE(stfs::is_directory(tree_file_a));
        CHECK_FALSE(stfs::is_directory(bad_missing));
        CHECK_FALSE(stfs::is_directory(""));
    }

    TEST_CASE_FIXTURE(link_fixture, "file symlinks are files")
    {
        SKIP_WITHOUT_SYMLINKS();

        CHECK(stfs::exists(file_link));
        CHECK(stfs::is_file(file_link));
        CHECK_FALSE(stfs::is_directory(file_link));
    }

    TEST_CASE_FIXTURE(link_fixture, "directory symlinks are directories")
    {
        SKIP_WITHOUT_SYMLINKS();

        CHECK(stfs::exists(dir_link));
        CHECK(stfs::is_directory(dir_link));
        CHECK_FALSE(stfs::is_file(dir_link));
    }

    TEST_CASE_FIXTURE(link_fixture, "broken and looping symlinks are not valid entries")
    {
        SKIP_WITHOUT_SYMLINKS();

        for (const fs::path &bad : {broken, loop_a, loop_b})
            check_invalid_entry(bad);
    }

    TEST_CASE("basename returns the final path component")
    {
        CHECK_EQ("0002-good-text.txt", stfs::basename(good_text));
        CHECK_EQ("0005-good-tree", stfs::basename(good_tree));
        CHECK_EQ("file.txt", stfs::basename("file.txt"));
        CHECK_EQ("file.txt", stfs::basename("a/b/file.txt"));
        CHECK_EQ(".hidden", stfs::basename("a/.hidden"));
    }

    TEST_CASE("basename of empty or trailing separator paths is empty")
    {
        CHECK_EQ("", stfs::basename(""));
        CHECK_EQ("", stfs::basename("a/b/"));
    }

    TEST_CASE("dirname returns everything before the final component")
    {
        CHECK_EQ(res_dir.string(), stfs::dirname(good_text));
        CHECK_EQ(res_dir.string(), stfs::dirname(good_tree));
        CHECK_EQ(good_tree.string(), stfs::dirname(tree_file_a));
        CHECK_EQ("a/b", stfs::dirname("a/b/file.txt"));
    }

    TEST_CASE("dirname of a bare name or empty path is empty")
    {
        CHECK_EQ("", stfs::dirname("file.txt"));
        CHECK_EQ("", stfs::dirname(""));
    }

    TEST_CASE("extension includes the dot and only the last suffix")
    {
        CHECK_EQ(".txt", stfs::extension(good_text));
        CHECK_EQ(".gz", stfs::extension(good_multi));
        CHECK_EQ(".dat", stfs::extension(tree_file_b));
        CHECK_EQ(".c", stfs::extension("a.b/file.c"));
    }

    TEST_CASE("extension is empty without a suffix")
    {
        CHECK_EQ("", stfs::extension(good_noext));
        CHECK_EQ("", stfs::extension(good_tree));
        CHECK_EQ("", stfs::extension(".hidden"));
        CHECK_EQ("", stfs::extension("a.b/file"));
        CHECK_EQ("", stfs::extension(""));
    }

    TEST_CASE("stem strips only the last suffix")
    {
        CHECK_EQ("0002-good-text", stfs::stem(good_text));
        CHECK_EQ("0003-good-multi.tar", stfs::stem(good_multi));
        CHECK_EQ("0004-good-noext", stfs::stem(good_noext));
        CHECK_EQ(".hidden", stfs::stem(".hidden"));
        CHECK_EQ("", stfs::stem(""));
    }

    TEST_CASE("list_all returns files and subdirectories")
    {
        const auto entries = sorted(stfs::list_all(good_tree));

        CHECK_EQ(sorted_paths({tree_file_a, tree_file_b, tree_sub_a, tree_sub_b}), entries);
    }

    TEST_CASE("list_files returns only regular files")
    {
        const auto entries = sorted(stfs::list_files(good_tree));

        CHECK_EQ(sorted_paths({tree_file_a, tree_file_b}), entries);
    }

    TEST_CASE("list_subdirs returns only directories")
    {
        const auto entries = sorted(stfs::list_subdirs(good_tree));

        CHECK_EQ(sorted_paths({tree_sub_a, tree_sub_b}), entries);
    }

    TEST_CASE("list functions are not recursive")
    {
        const auto files = stfs::list_files(tree_sub_a);

        CHECK_EQ(1u, files.size());
        CHECK(stfs::list_subdirs(tree_sub_a).empty());
        CHECK_EQ(1u, stfs::list_all(tree_sub_a).size());
    }

    TEST_CASE("list functions return nothing for missing paths and plain files")
    {
        for (const fs::path &bad : {bad_missing, good_text, fs::path{}})
        {
            CHECK(stfs::list_all(bad).empty());
            CHECK(stfs::list_files(bad).empty());
            CHECK(stfs::list_subdirs(bad).empty());
        }
    }

    TEST_CASE_FIXTURE(link_fixture, "list functions return nothing for empty directories")
    {
        CHECK(stfs::list_all(real_dir).empty());
        CHECK(stfs::list_files(real_dir).empty());
        CHECK(stfs::list_subdirs(real_dir).empty());
    }

    TEST_CASE_FIXTURE(link_fixture, "list functions ignore symlinks by default")
    {
        SKIP_WITHOUT_SYMLINKS();

        CHECK_EQ(sorted_paths({real_file, real_dir}), sorted(stfs::list_all(root)));
        CHECK_EQ(sorted_paths({real_file}), sorted(stfs::list_files(root)));
        CHECK_EQ(sorted_paths({real_dir}), sorted(stfs::list_subdirs(root)));
    }

    TEST_CASE_FIXTURE(link_fixture, "list_all resolves valid symlinks when following")
    {
        SKIP_WITHOUT_SYMLINKS();

        const auto entries = sorted(stfs::list_all(root, true));
        const auto expected = sorted_paths({real_file, real_dir, fs::canonical(good_text), fs::canonical(good_tree)});

        CHECK_EQ(expected, entries);
    }

    TEST_CASE_FIXTURE(link_fixture, "list_files resolves file symlinks when following")
    {
        SKIP_WITHOUT_SYMLINKS();

        const auto entries = sorted(stfs::list_files(root, true));
        const auto expected = sorted_paths({real_file, fs::canonical(good_text)});

        CHECK_EQ(expected, entries);
    }

    TEST_CASE_FIXTURE(link_fixture, "list_subdirs resolves directory symlinks when following")
    {
        SKIP_WITHOUT_SYMLINKS();

        const auto entries = sorted(stfs::list_subdirs(root, true));
        const auto expected = sorted_paths({real_dir, fs::canonical(good_tree)});

        CHECK_EQ(expected, entries);
    }

    TEST_CASE_FIXTURE(link_fixture, "following symlinks drops broken and looping links")
    {
        SKIP_WITHOUT_SYMLINKS();

        const auto all = stfs::list_all(root, true);

        for (const fs::path &bad : {broken, loop_a, loop_b})
        {
            CHECK_EQ(all.end(), std::find(all.begin(), all.end(), bad.string()));
        }
        CHECK_EQ(4u, all.size());
    }
}

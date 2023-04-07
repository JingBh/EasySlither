export module Utils.Filesystem;

import <cstdlib>;
import <filesystem>;
import <fstream>;
import <string>;
import <utility>;

#ifdef _WIN32
static const std::filesystem::path basePath{[]() {
    char *localAppData = nullptr;
    size_t len;
    auto err = _dupenv_s(&localAppData, &len, "LOCALAPPDATA");

    if (err != 0) {
        return std::filesystem::path{};
    }

    auto result = std::filesystem::path{localAppData} / "JingBh" / "EasySlither";

    delete localAppData;
    return result;
}()};
#else
static const std::filesystem::path basePath {"/var/games/JingBh/EasySlither"};
#endif //_WIN32

export std::fstream openPersistFile(std::string_view filename,
                                    std::fstream::openmode mode = std::ios::in) {
    if (!std::filesystem::exists(basePath)) {
        std::filesystem::create_directories(basePath);
    }

    const auto path = basePath / filename;
    return std::fstream{path, mode};
}

export std::fstream openTemporaryFile(std::string_view filename,
                                      std::fstream::openmode mode = std::ios::in) {
    const auto path = std::filesystem::temp_directory_path() / filename;
    return std::fstream{path, mode};
}

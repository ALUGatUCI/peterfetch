#include <iostream>
#include <optional>
#include <string>

#include <args.hxx>
#include <cpr/cpr.h>
#include <libxml/xmlversion.h>
#include <nlohmann/json.hpp>

#include "cli_args.hpp"
#include "config.hpp"
#include "directory_info.hpp"
#include "weather_info.hpp"

using json = nlohmann::json;

static std::optional<CliArgs> handle_args(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    std::optional<CliArgs> args = handle_args(argc, argv);
    if (!args.has_value()) {
        return 1;
    }

    // Mandatory library initialization
    LIBXML_TEST_VERSION

    std::cout << args->netid << "\n";

    DirectoryInfo base_info { args->netid };
    switch (base_info.fetch()) {
        case DirectoryFetchResult::DOESNT_EXIST:
            std::cerr << "No student with the UCInetID " << args->netid
                      << " exists!\n";
            return 1;
        case DirectoryFetchResult::OK:
            break;
        default:
            std::cerr << "Failed to fetch UCI Directory info!\n";
            return 1;
    }

    WeatherInfo weather_info;
    switch (weather_info.fetch()) {
        case WeatherFetchResult::OK:
            break;
        default:
            std::cerr << "Failed to fetch weather info!\n";
            return 1;
    }

    std::cout << base_info << "\n";
    weather_info.print(std::cout);

    return 0;
}

static std::optional<CliArgs> handle_args(int argc, char *argv[]) {
    args::ArgumentParser parser("peterfetch v" PROJECT_VERSION "\n" PROJECT_DESCRIPTION);
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Group required_args(parser, "", args::Group::Validators::All);
    args::Positional<std::string> netid(required_args, "netid", "Your UCInetID");
    args::CompletionFlag completion(parser, {"complete"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Completion &e) {
        std::cerr << e.what();
        return std::nullopt;
    } catch (const args::Help &e) {
        std::cerr << parser;
        return std::nullopt;
    } catch (const args::ParseError &e) {
        std::cerr << e.what() << "\n\n";
        std::cerr << parser;
        return std::nullopt;
    } catch (const args::ValidationError &e) {
        std::cerr << parser;
        return std::nullopt;
    }

    return CliArgs { args::get(netid) };
}

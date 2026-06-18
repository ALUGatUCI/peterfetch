#include <iostream>
#include <memory>
#include <optional>
#include <string>

#include <args.hxx>
#include <cpr/cpr.h>
#include <libxml/xmlversion.h>
#include <nlohmann/json.hpp>

#include "artwork.hpp"
#include "cli_args.hpp"
#include "config.hpp"
#include "directory_info.hpp"
#include "layout.hpp"

using namespace std;
using json = nlohmann::json;

static std::optional<CliArgs> handleArgs(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    std::optional<CliArgs> args = handleArgs(argc, argv);
    if (!args.has_value()) {
        return 1;
    }

    // Mandatory library initialization
    LIBXML_TEST_VERSION;

    TextLayout layout({ artwork::UCI, artwork::UCI_OFFSET });

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
    auto info_section = make_shared<DirectoryInfoSection>(base_info);
    layout.addSection(info_section);

    std::cout << layout;

    return 0;
}

static std::optional<CliArgs> handleArgs(int argc, char *argv[]) {
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

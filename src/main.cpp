#include <iostream>

#include <cpr/cpr.h>
#include <args.hxx>

#include <config.hpp>

int main(int argc, char *argv[]) {
    args::ArgumentParser parser("peterfetch v" PROJECT_VERSION "\n" PROJECT_DESCRIPTION);
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::CompletionFlag completion(parser, {"complete"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Completion &e) {
        std::cerr << e.what();
        return 1;
    } catch (const args::Help &e) {
        std::cerr << parser;
        return 0;
    } catch (const args::ParseError &e) {
        std::cerr << e.what() << "\n\n";
        std::cerr << parser;
        return 1;
    }

    cpr::Response r = cpr::Get(cpr::Url{ANTEATERAPI_URL});
    std::cout << "Response code: " << r.status_code << "\n";
    std::cout << r.text << "\n";

    return 0;
}

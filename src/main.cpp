#include <iostream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <args.hxx>

#include <config.hpp>

using json = nlohmann::json;

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
    json rjson = json::parse(r.text);

    if (r.status_code != 200)
        std::cerr << rjson["message"] << "\n";
    else
        std::cout << rjson << "\n";

    return 0;
}
